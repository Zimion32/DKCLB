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
can_throw_tt<U32> CXmlFileIO::Load(TXT filename)
{///returns number of subfiles - does not actually load the XML
    TXT headersblock;
    filepath_ = filename;
    U32 num_subfiles = 0;
    if(!(headers_end_ = ScanHeaders_(0, headersblock))) throw exception("File read error.");
    if(!(num_subfiles = ParseHeaders_(headers_end_, headersblock))) throw exception("Headers are malformed.");
    return can_throw_tt<U32>(num_subfiles);
}
can_throw_tt<U32> CXmlFileIO::LoadAndUse(U08* data, U32 bytes)
{///NON-CDL BUFFER LOADER
    TXT headersblock;
    U32 num_subfiles = 0;
    if(!(headers_end_ = ScanHeaders_Buffer_(data, bytes, headersblock))) throw exception("File read error.");
    if(!(num_subfiles = ParseHeaders_(headers_end_, headersblock))) throw exception("Headers are malformed.");
    if(!xmldoc_.load_buffer(&data[headers_end_], bytes-headers_end_)){
        throw exception("XML Data is malformed.");
    }
    xmlcurrentnode_ = xmldoc_.document_element().child("file");
    return can_throw_tt<U32>(num_subfiles);
}
can_throw_tt<U32> CXmlFileIO::LoadEx(TXT filename, U32 offset, U08* res_data, U32 res_bytes)
{///size is ignored during header loader, if $FFFFFFFF it reads until end of file
    TXT headersblock;
    filepath_ = filename;
    U32 num_subfiles = 0;
    if(res_data && res_bytes){
        res_data_ = res_data;
        res_bytes_ = res_bytes;
        if(!(headers_end_ = ScanHeaders_Buffer_(res_data, res_bytes, headersblock))) throw exception("File read error.");
        if(!(num_subfiles = ParseHeaders_(headers_end_, headersblock))) throw exception("Headers are malformed.");
        return can_throw_tt<U32>(num_subfiles);
    }else{
        if(!(headers_end_ = ScanHeaders_(offset, headersblock))) throw exception("File read error.");
        if(!(num_subfiles = ParseHeaders_(headers_end_, headersblock))) throw exception("Headers are malformed.");
        return can_throw_tt<U32>(num_subfiles);
    }
    return can_throw_tt<U32>(0); //should never happen
}
can_throw_t	CXmlFileIO::Save(TXT filename)
{///automatically adds DELTA headers before saveout
    if(filename == "") filename = filepath_;
    if(filename == "") throw exception("Trying to save without a filename.");
    CFileIO writer;
    if(is_cdl_){
        //if CDL mode: the data is already in the buffer; write headers, then buffer, then saveout 
        WriteHeaders_(writer);
        writer.Append(cdl_buffer_);
        writer.Save(filename);
        writer.Close();
    }else{
        //if not CDL mode:  save headers, then write xmldoc_ into file buffer and saveout the buffer
        xmldelta_headers_[0].offset = 0;
        xmldelta_headers_[0].size = 0xFFFFFFFF;
        WriteHeaders_(writer);
        SaveXML(writer.Vector());
        writer.Save(filename);
        writer.Close();
    }
    return can_throw_t();
}
struct xml_binary_writer: xml_writer
{
    Vx<U08>& datavec_;
    xml_binary_writer(Vx<U08>& datavec): datavec_(datavec) {}
    virtual void write(const void* data, size_t size){
        datavec_.resize(datavec_.size()+size);
        memcpy(&datavec_[datavec_.size()-size], data, size);
    }
};
can_throw_t	CXmlFileIO::SaveXML(Vx<U08>& datavec)
{///**appends** xml to a vector buffer
    xml_binary_writer xml(datavec);
    xmldoc_.save(xml);
    return can_throw_t();
}
void	    CXmlFileIO::Close()
{
    xmldoc_.reset();
    filepath_ = "";
	headers_end_ = 0;
	selected_subfile_ = 0xFFFFFFFF;
	cdl_build_detect_ = 0;
	is_cdl_ = false;
	res_data_ = NULL;
	res_bytes_ = 0;
	xmldelta_headers_.clear();
    cdl_buffer_.clear();
}
//---------------------------------------------------------------
U32         CXmlFileIO::ScanHeaders_(U32 offset, TXT& headersblock)
{
    FILE* F = fopen(filepath_.c_str(),"rb");
    U32 i = 0;
    fseek(F, offset, SEEK_SET);
    TXT first_scan = "";
    char c = '\0';
    BIT is_DELTA_file = false;
    while((c = fgetc(F)) != '$'){
        if(ferror(F)){
            fclose(F);
            return 0;
        }
        if(!is_DELTA_file){
            first_scan += c;
            if((first_scan.size() >= 8) 
            && (first_scan.substr(first_scan.size()-8,8) == "XMLDELTA")){
                is_DELTA_file = true;
            }else{
                if(i >= 17) return 0; //not a DELTA file at all!!!
            }
        }
        i++;
    }
    headersblock.resize(i, ' ');
    fseek(F, offset, SEEK_SET);
    fread( (PTR)&headersblock[0], 1, i, F);
    fclose(F);
    i++; //account for finishing $ marker
    return i;
}
U32         CXmlFileIO::ScanHeaders_Buffer_(U08* data, U32 bytes, TXT& headersblock)
{
    U32 i = 0;
    TXT first_scan = "";
    BIT is_DELTA_file = false;
    while((i < bytes) && (data[i] != '$')){
        if(!is_DELTA_file){
            first_scan += (char)data[i];
            if((first_scan.size() >= 8) 
            && (first_scan.substr(first_scan.size()-8,8) == "XMLDELTA")){
                is_DELTA_file = true;
            }else{
                if(i >= 17) return 0; //not a DELTA file at all!!!
            }
        }
        i++;
    }
    headersblock.resize(i, ' ');
    memcpy(&headersblock[0], &data[0], i);
    i++; //account for finishing $ marker
    return i;
}
U32         CXmlFileIO::ParseHeaders_(U32 headers_end, TXT& headersblock)
{
    xmldelta_headers_.clear();
    Vx<U32> blocks;
    size_t found = 0;
    while((found = headersblock.find("XMLDELTA",found)) != TXT::npos){
        blocks.push_back(found); found += 8;
    }
    blocks.push_back(headersblock.size());
    size_t offset = headers_end;
    for(U32 i = 0; i < (blocks.size()-1); i++){
        TXT curr = headersblock.substr(blocks[i], blocks[i+1] - blocks[i]);
        found = 0;
        while((found = curr.find_first_of(" \t\r\n",found)) != TXT::npos){
            curr.erase(curr.begin()+found);
        }
        // "XMLDELTAttSSSSSSSSLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL
        //  ddddddddddddddDDDDDDDDDDDDDDdddddddddddddd
        //  MMMMMMMMMMMMMMMMbbbbbbbbbbbbbbbbAAAAAAAAAAAAAAAAffffffff"  148 characters
        if(curr.size() != 148) return 0; ///if error
        INT j = 8;
        SXmlDelta xd;
        xd.type = String.ToHex(curr.substr(j,2));                        j += 2;
        xd.size = String.ToHex(curr.substr(j,8));                        j += 8;
        xd.lucid = LUCID(curr.substr(j,32));                             j += 32;
        xd.exthd.SetDateLastSaved(dtime().SetToData(curr.substr(j,14))); j += 14;
        xd.exthd.SetDateFinalized(dtime().SetToData(curr.substr(j,14))); j += 14;
        xd.exthd.SetDateBugFixed(dtime().SetToData(curr.substr(j,14)));  j += 14;
        xd.exthd.SetMajorVersion(String.ToHex(curr.substr(j,16)));       j += 16;
        xd.exthd.SetSaveCountBF(String.ToHex(curr.substr(j,16)));        j += 16;
        xd.exthd.SetSaveCountAF(String.ToHex(curr.substr(j,16)));        j += 16;
        xd.exthd.SetSaveFlags(String.ToHex(curr.substr(j,8)));           j += 8;
        xd.offset = offset;
        xmldelta_headers_.push_back(xd);
        if(i) offset += xd.size; //only adjust if this is not the main header
    }
    return blocks.size()-1;
}
void        CXmlFileIO::WriteHeaders_(CFileIO& file)
{
    for(U32 i = 0; i < xmldelta_headers_.size(); i++){
        SXmlDelta& xd = xmldelta_headers_[i];
        file.SvText("XMLDELTA ");
        file.SvText(String.FromHex(xd.type, 2) + " ");
        file.SvText(String.FromHex(xd.size, 8) + " ");
        file.SvText(xd.lucid.ToString() + " ");
        file.SvText(xd.exthd.GetDateLastSaved().ToDataString() + " ");
        file.SvText(xd.exthd.GetDateFinalized().ToDataString() + " ");
        file.SvText(xd.exthd.GetDateBugFixed().ToDataString() + " ");
        file.SvText(String.FromHex(xd.exthd.GetMajorVersion(),16) + " ");
        file.SvText(String.FromHex(xd.exthd.GetSaveCountBF(),16) + " ");
        file.SvText(String.FromHex(xd.exthd.GetSaveCountAF(),16) + " ");
        file.SvText(String.FromHex(xd.exthd.GetSaveFlags(),8) + "\n");
    }
    file.SvText("$"); //finishing $ marker
}


















