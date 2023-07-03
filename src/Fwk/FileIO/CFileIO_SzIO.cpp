/*===========================================================================*\
 *  DKC Level Builder Toolkit
 *  Copyright (C) 2023 Simion32
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
#include <cstdio>
#include "include.h"
can_throw_tt<U64> CFileIO::FileSize(TXT filename)
{
    FILE* F = fopen(filename.c_str(),"rb");
    if(F){
        fseek(F, 0, SEEK_END);
        INT length = ftell(F);
        fclose(F);
        return length;
    }else{
        return 0;
    }
	return can_throw_tt<U64>(0LL);
}
can_throw_t CFileIO::Open(TXT filename)
{
	return Load(filename);
}
can_throw_t CFileIO::Load(TXT filename)
{
    FILE* F = fopen(filename.c_str(),"rb");
    if(F)
    {
        Vx<U08>& buffer = buffer_;
        fseek(F, 0, SEEK_END);
        INT length = ftell(F);
        rewind(F);
        buffer.clear();
        buffer.reserve(length);
        buffer.insert(buffer.begin(), length, 0x00);
        fread( (PTR)&buffer[0], 1, length, F);
        fclose(F);
        get_position_ = 0;
    }
    else
    {
        last_error_ = "Could not open \"";
        last_error_ += filename;
        last_error_ += "\".\nPlease make sure that the file exists and is not in use by another program.";
        throw exception(last_error_);
    }
    SetupChunkValues_();
	return can_throw_t();
}
can_throw_t CFileIO::LoadSection(TXT filename, U32 offset, U32 size)
{
    FILE* F = fopen(filename.c_str(),"rb");
    if(F)
    {
        Vx<U08>& buffer = buffer_;
        fseek(F, 0, SEEK_END);
        INT length = ftell(F);
        if(size == 0xFFFFFFFF){
            size = (length - offset);
        }
        if(length < (offset+size)){
            //WARNING: This function can fail, in which case it loads an empty file buffer!!
            buffer.clear();
            fclose(F);
            get_position_ = 0;
            return can_throw_t();
        }
        rewind(F);
        fseek(F, offset, SEEK_SET); ///BUGFIX: This was missing!!
        buffer.clear();
        buffer.reserve(size);
        buffer.insert(buffer.begin(), size, 0x00);
        fread( (PTR)&buffer[0], 1, size, F);
        fclose(F);
        get_position_ = 0;
    }
    else
    {
        last_error_ = "Could not open \"";
        last_error_ += filename;
        last_error_ += "\".\nPlease make sure that the file exists and is not in use by another program.";
        throw exception(last_error_);
    }
    SetupChunkValues_();
	return can_throw_t();
}
can_throw_t CFileIO::LoadSectionEx(TXT filename, U32 offset, U32 size, U08* res_data, U32 res_size)
{
    if((res_data != NULL) && (res_size != 0)){
        if(size == 0xFFFFFFFF){
            size = (res_size - offset);
        }
        if(offset+size > res_size){
            last_error_ = "Could not open \"";
            last_error_ += filename;
            last_error_ += "\". There is not enough data in the resource buffer.";
            throw exception(last_error_);
        }
        Fill(res_data + offset, size);
        SetupChunkValues_();
        return can_throw_t();
    }else{
        return LoadSection(filename, offset, size);
    }
}
can_throw_t CFileIO::Fill(Vx<U08>& data)
{
	buffer_ = data;
	set_position_ = buffer_.size();
	return can_throw_t();
}
can_throw_t CFileIO::Fill(U08* data, U32 bytes)
{
	buffer_.clear();
	buffer_.reserve(bytes);
	buffer_.insert(buffer_.begin(),bytes,0x00);
	memcpy((PTR)&buffer_[0], data, bytes);
	set_position_ = buffer_.size();
	return can_throw_t();
}
can_throw_t CFileIO::Append(Vx<U08>& data)
{
	buffer_.insert(buffer_.end(), data.begin(), data.end());
	set_position_ = buffer_.size();
	return can_throw_t();
}
can_throw_t CFileIO::Append(U08* data, U32 bytes)
{
	size_t oldsize = buffer_.size();
	buffer_.reserve(oldsize+bytes);
	buffer_.insert(buffer_.end(),bytes,0x00);
	memcpy((PTR)&buffer_[oldsize], data, bytes);
	set_position_ = buffer_.size();
	return can_throw_t();
}
can_throw_t CFileIO::Dump(Vx<U08>& data)
{
	data = buffer_;
	return can_throw_t();
}
can_throw_t CFileIO::Flush(TXT filename)
{
	if(filename == ""){
		last_error_ = "Empty Filename";
		throw exception(last_error_);
	}
	FILE* F = fopen(filename.c_str(),"wb+");
	if(!F){
		last_error_ = "Could not create \"";
		last_error_ += filename;
		last_error_ += "\".";
		throw exception(last_error_);
	}
	Vx<U08>& buffer = buffer_;
	U32 length = buffer.size();
	fwrite((PTR)&buffer[0], 1, length, F);
	fclose(F);
	return can_throw_t();
}
can_throw_t CFileIO::Save(TXT filename)
{
	return Flush(filename);
}
void CFileIO::Close()
{
    FreeMemory();//Will automatically set proper variables.
}
can_throw_t CFileIO::FlushToCxx(TXT filename)
{
	return FlushToCxx(filename, "data", "");
}
can_throw_t CFileIO::FlushToCxx(TXT filename, TXT dataname, TXT homepath)
{
	//DELETED FUNCTION
	return can_throw_t();
}
