/*===========================================================================*\
 *  DKC Level Builder Toolkit
 *  Copyright (C) 2025 Simion32
 *
 *  This file is part of the DKC Level Builder Toolkit (DKCLB).
 *
 *  DKCLB is free software: you can redistribute it and/or modify it under the
 *  terms of the GNU General Public License as published by the Free Software 
 *  Foundation, either version 3 of the License, or (at your option) any later 
 *  version.
 *
 *  DKCLB is distributed in the hope that it will be useful, but WITHOUT ANY 
 *  WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS 
 *  FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License along 
 *  with DKCLB. If not, see <https://www.gnu.org/licenses/>. 
**===========================================================================*/
#include "include.h"
#include "CXmlFileIO.h"
// This software uses pugixml library (https://pugixml.org).
// pugixml is Copyright (C) 2006-2024 Arseny Kapoulkine.
//---------------------------------------------------------------
void    CXmlFileIO::GvSelSubFile (U32 subfile)
{///selects subfile, but does not open whole XML
    selected_subfile_ = subfile;
}
void    CXmlFileIO::GvUseSubFile ()
{///actually loads the XML portion of the selected subfile; closes previous
    try{
        CFileIO buff;
        if(res_data_ && res_bytes_){
            buff.Fill(res_data_ + xmldelta_headers_[selected_subfile_].offset,
                xmldelta_headers_[selected_subfile_].size);
        }else{
            buff.LoadSection(filepath_, 
                xmldelta_headers_[selected_subfile_].offset, 
                xmldelta_headers_[selected_subfile_].size);
        }
        xmldoc_.load_buffer(&buff.Vector()[0], buff.ByteSize());
        xmlcurrentnode_ = xmldoc_.document_element().child("file");
        buff.Close();
    }catch(const exception& e){
    }endtry;
}
SCdlStream CXmlFileIO::GvCdlStream()
{
    SCdlStream info;
    if(res_data_ && res_bytes_){
        info.file     = "";
        info.resource = res_data_;
        info.offset   = xmldelta_headers_[selected_subfile_].offset; 
        info.size     = xmldelta_headers_[selected_subfile_].size;
    }else{
        info.file     = filepath_;
        info.resource = NULL;
        info.offset   = xmldelta_headers_[selected_subfile_].offset; 
        info.size     = xmldelta_headers_[selected_subfile_].size;
    }
    return info;
}
//---------------------------------------------------------------
U32	    CXmlFileIO::GvHeaderData (LUCID& lucid, EXTHD& exthd)
{
    lucid = xmldelta_headers_[selected_subfile_].lucid;
    exthd = xmldelta_headers_[selected_subfile_].exthd;
    return  xmldelta_headers_[selected_subfile_].type;
}
BIT     CXmlFileIO::GvBegSection (TXT section)
{
    xml_node x = xmlcurrentnode_.child(section.c_str());
    if(!x) return false;
    xmlcurrentnode_ = x;
    return true;
}
void    CXmlFileIO::GvEndSection ()
{
    xmlcurrentnode_ = xmlcurrentnode_.parent();
}
BIT     CXmlFileIO::GvBegArray   (TXT name)
{
    if(!xmlcurrentnode_.first_child()) return false;
    xmlcurrentnode_ = xmlcurrentnode_.first_child();
    while(xmlcurrentnode_.next_sibling() && (xmlcurrentnode_.name() != name)){
        xmlcurrentnode_ = xmlcurrentnode_.next_sibling();
    }
    if(xmlcurrentnode_.name() != name) return false;
    return true;
}
BIT     CXmlFileIO::GvNxtArray   (TXT name)
{
    if(!xmlcurrentnode_.next_sibling()) return false;
    xmlcurrentnode_ = xmlcurrentnode_.next_sibling();
    while(xmlcurrentnode_.next_sibling() && (xmlcurrentnode_.name() != name)){
        xmlcurrentnode_ = xmlcurrentnode_.next_sibling();
    }
    if(xmlcurrentnode_.name() != name) return false;
    return true;
}
BIT     CXmlFileIO::GvLoopArray   ()
{
    TXT name = xmlcurrentnode_.name();
    if(!GvNxtArray(name)){
        GvEndArray();
        return false;
    }
    return true;
}
void    CXmlFileIO::GvEndArray   ()
{
    xmlcurrentnode_ = xmlcurrentnode_.parent();
}
S64	    CXmlFileIO::GvInt        (TXT name, S64 def)
{
    xml_node x = xmlcurrentnode_.child(name.c_str());
    if(!x) return def;
    return x.text().as_llong(def);
}
U64	    CXmlFileIO::GvHex        (TXT name, U32 bytes, U64 def)
{
    xml_node x = xmlcurrentnode_.child(name.c_str());
    if(!x) return def;
    U32 shift = ((8-bytes)*8);
    return (((U64)(String.ToHex(x.text().get()) << shift)) >> shift);
}
S64	    CXmlFileIO::GvVsx        (TXT name, U32 bytes, S64 def)
{
    xml_node x = xmlcurrentnode_.child(name.c_str());
    if(!x) return def;
    U32 shift = ((8-bytes)*8);
    return (((S64)(String.ToHex(x.text().get()) << shift)) >> shift);
}
F32     CXmlFileIO::GvFloat      (TXT name, F32 def)
{
    xml_node x = xmlcurrentnode_.child(name.c_str());
    if(!x) return def;
    return x.text().as_float();
}
F32     CXmlFileIO::GvFloatAsHex (TXT name, F32 def)
{
    xml_node x = xmlcurrentnode_.child(name.c_str());
    if(!x) return def;
    U32 data = String.ToHex(x.text().get());
    return *(F32*)&data;
}
F64     CXmlFileIO::GvDouble     (TXT name, F64 def)
{
    xml_node x = xmlcurrentnode_.child(name.c_str());
    if(!x) return def;
    return x.text().as_double();
}
F64     CXmlFileIO::GvDoubleAsHex(TXT name, F64 def)
{
    xml_node x = xmlcurrentnode_.child(name.c_str());
    if(!x) return def;
    U64 data = String.ToHex(x.text().get());
    return *(F64*)&data;
}
TXT	    CXmlFileIO::GvString     (TXT name, TXT def)
{
    xml_node x = xmlcurrentnode_.child(name.c_str());
    if(!x) return def;
    return x.text().get();
}
TXT	    CXmlFileIO::GvText       (TXT name, TXT def)
{
    xml_node x = xmlcurrentnode_.child(name.c_str());
    if(!x) return def;
    return x.text().get();
}
BMP 	CXmlFileIO::GvBitmapBlock(TXT name)
{
    xml_node x = xmlcurrentnode_.child(name.c_str());
    if(!x) return NULL;
    INT w = x.attribute("w").as_int();
    INT h = x.attribute("h").as_int();
    BMP bmp = create_bitmap_ex(32, w, h);
    Vx<U08> data = Utils.ConvertFromUrlBase64(x.text().get());
    INT datsz = w*h*4;
    memcpy(bmp->line[0], &data[0], ((data.size() < datsz) ? data.size() : datsz));
    return bmp;
}
BMP	    CXmlFileIO::GvPNG        (TXT name)
{
    xml_node x = xmlcurrentnode_.child(name.c_str());
    if(!x) return NULL;
    BMP ret = NULL;
    CFileIO png;
    try{
        png.Vector() = Utils.ConvertFromUrlBase64(x.text().get());
        ret = png.GvPNG();
        png.Close();
    }catch(const exception& e){
    }endtry;
    return ret;
}
LUCID   CXmlFileIO::GvLUCID      (TXT name, LUCID def)
{
    xml_node x = xmlcurrentnode_.child(name.c_str());
    if(!x) return def;
    return LUCID(x.text().get());
}
Vx<U08> CXmlFileIO::GvFile       (TXT name)
{
    xml_node x = xmlcurrentnode_.child(name.c_str());
    if(!x) return Vx<U08>();
    INT sz = x.attribute("size").as_int();
    Vx<U08> data = Utils.ConvertFromUrlBase64(x.text().get());
    data.resize(sz);
    return data;
}

















