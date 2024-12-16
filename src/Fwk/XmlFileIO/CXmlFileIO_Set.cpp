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
void    CXmlFileIO::SvBegSubFile ()
{
    if(selected_subfile_ == 0xFFFFFFFF){
        selected_subfile_ = 0;
        cdl_build_detect_ = 0;
        is_cdl_ = false;
    }else{
        selected_subfile_++;
        is_cdl_ = true;
    }
    xmldelta_headers_.push_back( SXmlDelta() );
    xmldoc_.reset();
    xmlcurrentnode_ = xmldoc_.document_element();
    xmlcurrentnode_ = xmlcurrentnode_.append_child("file");
}
void    CXmlFileIO::SvEndSubFile ()
{
    if(cdl_build_detect_ == 0) is_cdl_ = true;
    if(is_cdl_ && selected_subfile_){
        //save xml to cdl buffer
        xmldelta_headers_.back().offset = cdl_buffer_.size();
        SaveXML(cdl_buffer_);
        xmldelta_headers_.back().size = (cdl_buffer_.size() - xmldelta_headers_.back().offset);
    }//else wait to save XML until Save() is called
}
//The following three functions insert a subfile with content from an existing headered file/buffer.
void    CXmlFileIO::SvCpySubFile (Vx<U08>& datavec)
{
    is_cdl_ = true;
    selected_subfile_++;
    CXmlFileIO subfile;
    try{
        subfile.LoadAndUse(&datavec[0], datavec.size());
        xmldelta_headers_.push_back(subfile.xmldelta_headers_[0]);
        xmldelta_headers_.back().offset = cdl_buffer_.size();
        subfile.SaveXML(cdl_buffer_);
        xmldelta_headers_.back().size = (cdl_buffer_.size() - xmldelta_headers_.back().offset);
    }catch(const exception& e){
    }endtry;
}
void    CXmlFileIO::SvCpySubFile (U08* data, U32 bytes)
{
    is_cdl_ = true;
    selected_subfile_++;
    CXmlFileIO subfile;
    try{
        subfile.LoadAndUse(data, bytes);
        xmldelta_headers_.push_back(subfile.xmldelta_headers_[0]);
        xmldelta_headers_.back().offset = cdl_buffer_.size();
        subfile.SaveXML(cdl_buffer_);
        xmldelta_headers_.back().size = (cdl_buffer_.size() - xmldelta_headers_.back().offset);
    }catch(const exception& e){
    }endtry;
}
void    CXmlFileIO::SvCpySubFile (TXT filename)
{
    is_cdl_ = true;
    selected_subfile_++;
    CXmlFileIO subfile;
    try{
        subfile.Load(filename);
        subfile.GvSelSubFile(0);
        subfile.GvUseSubFile();
        xmldelta_headers_.push_back(subfile.xmldelta_headers_[0]);
        xmldelta_headers_.back().offset = cdl_buffer_.size();
        subfile.SaveXML(cdl_buffer_);
        xmldelta_headers_.back().size = (cdl_buffer_.size() - xmldelta_headers_.back().offset);
    }catch(const exception& e){
    }endtry;
}
void    CXmlFileIO::SvChangeLUCID(LUCID lucid)
{///only changes subfile zero
    xmldelta_headers_[selected_subfile_].lucid = lucid;
}
void    CXmlFileIO::SvChangeEXTHD(EXTHD exthd)
{///only changes subfile zero
    xmldelta_headers_[selected_subfile_].exthd = exthd;
}
void    CXmlFileIO::SvRawSubFile (U08* data, U32 bytes, U32 type, LUCID lucid)
{
    is_cdl_ = true;
    selected_subfile_++;
    CFileIO file;
    try{
        file.Fill(data, bytes);
        SXmlDelta xd;
        xd.type = type;
        xd.lucid = lucid;
        xd.exthd = EXTHD();
        dtime dt; dt.SetToCurrentTime();
        xd.exthd.SetDateFinalized(dt);
        xmldelta_headers_.push_back(xd);
        xmldelta_headers_.back().offset = cdl_buffer_.size();
        cdl_buffer_.insert(cdl_buffer_.end(), file.Vector().begin(), file.Vector().end());
        xmldelta_headers_.back().size = (cdl_buffer_.size() - xmldelta_headers_.back().offset);
        file.Close();
    }catch(const exception& e){
    }endtry;
}
void    CXmlFileIO::SvRawSubFile (TXT filename, U32 type, LUCID lucid)
{
    is_cdl_ = true;
    selected_subfile_++;
    CFileIO file;
    try{
        file.Load(filename);
        SXmlDelta xd;
        xd.type = type;
        xd.lucid = lucid;
        xd.exthd = EXTHD();
        SFileInfo info;
        FileSystem.GetFileInfo(filename, info);
        dtime dt; dt.SetToCurrentTime();
        xd.exthd.SetDateFinalized(dt);
        xd.exthd.SetDateLastSaved(info.modified);
        xmldelta_headers_.push_back(xd);
        xmldelta_headers_.back().offset = cdl_buffer_.size();
        cdl_buffer_.insert(cdl_buffer_.end(), file.Vector().begin(), file.Vector().end());
        xmldelta_headers_.back().size = (cdl_buffer_.size() - xmldelta_headers_.back().offset);
        file.Close();
    }catch(const exception& e){
    }endtry;
}
//---------------------------------------------------------------
void	CXmlFileIO::SvHeaderData (U32 type, LUCID lucid, EXTHD exthd)
{
    xmldelta_headers_[selected_subfile_].type = type;
    xmldelta_headers_[selected_subfile_].lucid = lucid;
    xmldelta_headers_[selected_subfile_].exthd = exthd;
}
void    CXmlFileIO::SvBegSection (TXT section)
{
    cdl_build_detect_ = 1;
    xmlcurrentnode_ = xmlcurrentnode_.append_child(section.c_str());
}
void    CXmlFileIO::SvEndSection ()
{
    cdl_build_detect_ = 1;
    xmlcurrentnode_ = xmlcurrentnode_.parent();
}
BIT     CXmlFileIO::SvLoopArray  (TXT name, INT i, INT len)
{
    cdl_build_detect_ = 1;
    BIT reverse_mode = (len < 0);
    len = abs(len);
    if(!len) return false;
    if((i == -1) || (i >= len)){
        //loop is done
        SvEndArray();
        return false;
    }else{
        if(reverse_mode){
            if(i == (len-1)){
                SvBegArray(name);
            }else{
                SvNxtArray(name);
            }
        }else{
            if(i == 0){
                SvBegArray(name);
            }else{
                SvNxtArray(name);
            }
        }
    }
    return true;
}
void    CXmlFileIO::SvBegArray   (TXT name)
{
    cdl_build_detect_ = 1;
    xmlcurrentnode_ = xmlcurrentnode_.append_child(name.c_str());
}
void    CXmlFileIO::SvNxtArray   (TXT name)
{
    cdl_build_detect_ = 1;
    xmlcurrentnode_ = xmlcurrentnode_.parent().append_child(name.c_str());
}
void    CXmlFileIO::SvEndArray   ()
{
    cdl_build_detect_ = 1;
    xmlcurrentnode_ = xmlcurrentnode_.parent();
}
void	CXmlFileIO::SvInt        (TXT name, S64 value)
{
    cdl_build_detect_ = 1;
    xml_node x = xmlcurrentnode_.append_child(name.c_str());
    x.append_attribute("type").set_value("int");
    x.text().set(value);
}
void	CXmlFileIO::SvHex        (TXT name, U32 bytes, U64 value)
{
    cdl_build_detect_ = 1;
    xml_node x = xmlcurrentnode_.append_child(name.c_str());
    x.append_attribute("type").set_value("hex");
    x.text().set(String.FromHex(value, bytes*2).c_str());
}
void	CXmlFileIO::SvVsx        (TXT name, U32 bytes, S64 value)
{
    cdl_build_detect_ = 1;
    xml_node x = xmlcurrentnode_.append_child(name.c_str());
    x.append_attribute("type").set_value("hex");
    x.text().set(String.FromHex(value, bytes*2).c_str());
}
void	CXmlFileIO::SvFloat      (TXT name, F32 value)
{
    cdl_build_detect_ = 1;
    xml_node x = xmlcurrentnode_.append_child(name.c_str());
    x.append_attribute("type").set_value("flo");
    x.text().set(value);
}
void	CXmlFileIO::SvFloatAsHex (TXT name, F32 value)
{
    cdl_build_detect_ = 1;
    xml_node x = xmlcurrentnode_.append_child(name.c_str());
    x.append_attribute("type").set_value("fhx");
    x.text().set(String.FromHex(*(U32*)(&value), 8).c_str());
}
void	CXmlFileIO::SvDouble     (TXT name, F64 value)
{
    cdl_build_detect_ = 1;
    xml_node x = xmlcurrentnode_.append_child(name.c_str());
    x.append_attribute("type").set_value("dbl");
    x.text().set(value);
}
void	CXmlFileIO::SvDoubleAsHex(TXT name, F64 value)
{
    cdl_build_detect_ = 1;
    xml_node x = xmlcurrentnode_.append_child(name.c_str());
    x.append_attribute("type").set_value("dhx");
    x.text().set(String.FromHex(*(U64*)(&value), 16).c_str());
}
void	CXmlFileIO::SvString     (TXT name, TXT text)
{
    cdl_build_detect_ = 1;
    xml_node x = xmlcurrentnode_.append_child(name.c_str());
    x.append_attribute("type").set_value("txt");
    x.text().set(text.c_str());
}
void	CXmlFileIO::SvText       (TXT name, TXT text)
{
    cdl_build_detect_ = 1;
    xml_node x = xmlcurrentnode_.append_child(name.c_str());
    x.append_attribute("type").set_value("txt");
    x.text().set(text.c_str());
}
void	CXmlFileIO::SvBitmapBlock(TXT name, BMP bmp)
{
    cdl_build_detect_ = 1;
    xml_node x = xmlcurrentnode_.append_child(name.c_str());
    x.append_attribute("type").set_value("bmp");
    x.append_attribute("w").set_value(bmp->w);
    x.append_attribute("h").set_value(bmp->h);
    TXT base64 = Utils.ConvertToUrlBase64(bmp->line[0], bmp->w * bmp->h * 4);
    x.text().set(base64.c_str());
}
void	CXmlFileIO::SvPNG        (TXT name, BMP bmp)
{
    cdl_build_detect_ = 1;
    xml_node x = xmlcurrentnode_.append_child(name.c_str());
    x.append_attribute("type").set_value("png");
    CFileIO png;
    try{
        png.SvPNG(bmp);
        TXT base64 = Utils.ConvertToUrlBase64(&(png.Vector()[0]), png.Vector().size());
        x.text().set(base64.c_str());
        png.Close();
    }catch(const exception& e){
    }endtry;
}
void    CXmlFileIO::SvLUCID      (TXT name, LUCID value)
{
    cdl_build_detect_ = 1;
    xml_node x = xmlcurrentnode_.append_child(name.c_str());
    x.append_attribute("type").set_value("luc");
    x.text().set(value.ToString().c_str());
}
void    CXmlFileIO::SvFile       (TXT name, Vx<U08>& datavec)
{
    cdl_build_detect_ = 1;
    xml_node x = xmlcurrentnode_.append_child(name.c_str());
    x.append_attribute("type").set_value("dat");
    x.append_attribute("size").set_value((INT)datavec.size());
    TXT base64 = Utils.ConvertToUrlBase64(&datavec[0], datavec.size());
    x.text().set(base64.c_str());
}
void    CXmlFileIO::SvFile       (TXT name, TXT path)
{
    cdl_build_detect_ = 1;
    xml_node x = xmlcurrentnode_.append_child(name.c_str());
    x.append_attribute("type").set_value("dat");
    CFileIO data;
    try{
        data.Load(path);
        x.append_attribute("size").set_value((INT)data.Vector().size());
        TXT base64 = Utils.ConvertToUrlBase64(&(data.Vector()[0]), data.Vector().size());
        x.text().set(base64.c_str());
        data.Close();
    }catch(const exception& e){
    }endtry;
}











