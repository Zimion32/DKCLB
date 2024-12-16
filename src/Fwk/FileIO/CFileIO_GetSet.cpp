#include "include.h"
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

can_throw_tt<U64> CFileIO::GvHex(U32 bytes)
{
	if(get_position_ < buffer_.size()){
        if(get_position_+bytes <= buffer_.size()){
            U64 result = 0x0000000000000000LL;
            for(IND i = 0; i < bytes; i++){
                result <<= 8;
                result |= (U64)buffer_[get_position_+i];
            }
            get_position_ += bytes;
            return result;
        }else{throw exception("Trying to read past end of file!!");}
    }else{throw exception("Trying to read past end of file!!");}
	return 0xFFFFFFFF;
}
can_throw_tt<S64> CFileIO::GvVsx(U32 bytes)
{
    S64 data = (S64)GvHex(bytes);
    U32 shift = ((8 - bytes) * 8);
    return ((S64(data << shift)) >> shift);
}
can_throw_tt<U64> CFileIO::GvHexLe(U32 bytes)
{
	if(get_position_ < buffer_.size()){
        if(get_position_+bytes <= buffer_.size()){
            U64 result = 0x0000000000000000LL;
            for(IND i = 0; i < bytes; i++){
                result |= (((U64)buffer_[get_position_+i]) << (i*8));
            }
            get_position_ += bytes;
            return result;
        }else{throw exception("Trying to read past end of file!!");}
    }else{throw exception("Trying to read past end of file!!");}
	return 0xFFFFFFFF;
}
can_throw_tt<S64> CFileIO::GvVsxLe(U32 bytes)
{
    S64 data = (S64)GvHexLe(bytes);
    U32 shift = ((8 - bytes) * 8);
    return ((S64(data << shift)) >> shift);
}
can_throw_tt<F32> CFileIO::GvFloat()
{
    U32 data = GvHex(4);
    F32 value = *((F32*)(&data));
    return value;
}
can_throw_tt<F64> CFileIO::GvDouble()
{
    U64 data = GvHex(8);
    F64 value = *((F64*)(&data));
    return value;
}
can_throw_tt<TXT> CFileIO::GvString()
{
	TXT result = "";
	for(CHR character = 0xFF; character != 0x00;){
		character = GvHex(1);
		if(character){result += character;}
	}
	return result;
}
can_throw_tt<TXT> CFileIO::GvText(U32 chars)
{
	TXT result = "";
	--chars;
	for(; chars != 0xFFFFFFFF; --chars){
		CHR character = GvHex(1);
		if(character){result += character;}
	}
	return result;
}
can_throw_tt<TXT> CFileIO::GvTextBlock(U32 bytes)
{
    TXT result = "";
    BIT reached_end = false;
	for(U32 i = 0; i < bytes; i++){
        CHR c = GvHex(1);
        if(c == '\0'){
            reached_end = true;
        }
        if(!reached_end){
            result += c;
        }
	}
	return result;
}
can_throw_tt<U32> CFileIO::GvHeader(TXT magic5)
{
	if(ByteSize() <= 8){
		//We assume a file wants to be read in - so an empty rest-of-file is an error.
		last_error_ = "The file is either corrupt, or is empty."; 
		throw exception(last_error_);
	}
	TXT in_magic = GvText(5);
	U32 in_type = GvHex(3);
	if(TXT(magic5).size() != 5){
		last_error_ = "Programmer Error: File header magic text must be exactly 5 characters."; 
		throw exception(last_error_);
	}
	if(in_magic != magic5) return 0xFFFFFFFF;
	return in_type;
}
can_throw_tt<LUCID> CFileIO::GvLUCID()
{
	LUCID identifier;
	for(IND d = 0; d < 24; ++d){
	    identifier.pData[d] = GvHex(1);
	}
	return identifier;
}
can_throw_tt<EXTHD> CFileIO::GvEXTHD()
{
	EXTHD extended_header;
	for(IND d = 0; d < 32; ++d){
	    extended_header.pData[d] = GvHex(1);
	}
	return extended_header;
}
can_throw_t CFileIO::GvSkipBytes(U32 bytes)
{
	if((get_position_+bytes) > buffer_.size()){
        last_error_ = "There aren't enough bytes in the file to be skipped over!"; 
		throw exception(last_error_);
	}
	get_position_ += bytes;
    return can_throw_t();
}
can_throw_tt< Vx<U08> > CFileIO::GvFile()
{
    U32 length = GvHex(4);
    Vx<U08> result;
    result.resize(length, 0);
    for(IND i = 0; i < length; i++){
        result[i] = GvHex(1);
    }
    return result;
}
can_throw_tt<BMP> CFileIO::GvBitmapBlock(U32 expected_w, U32 expected_h)
{
    BMP bmp = create_bitmap_ex(32, expected_w, expected_h);
    for(IND yy = 0; yy < bmp->h; yy++){
        for(IND xx = 0; xx < bmp->w; xx++){
            ((U32*)bmp->line[yy])[xx] = GvHex(4);
        }
    }
    return bmp;
}

//############################################################################//
//############################################################################//
can_throw_t CFileIO::SvHex(U64 value, U32 bytes)
{
	if(!bytes) return can_throw_t(); //zero length not necessarily an error
	while(buffer_.size() < (set_position_+bytes)) buffer_.push_back(0x00); //make room
    for(IND i = bytes-1; i >= 0; i--){
        buffer_[set_position_+i] = (((U08)value) & 0xFF);
        value >>= 8;
    }
    set_position_ += bytes;
	return can_throw_t();
}
can_throw_t CFileIO::SvVsx(S64 value, U32 bytes)
{
	return SvHex((U64)value, bytes);
}
can_throw_t CFileIO::SvHexLe(U64 value, U32 bytes)
{
	if(!bytes) return can_throw_t(); //zero length not necessarily an error
	while(buffer_.size() < (set_position_+bytes)) buffer_.push_back(0x00); //make room
    for(IND i = 0; i < bytes; i++){
        buffer_[set_position_+i] = ((U08)((value >> (i*8)) & 0xFFLL));
    }
    set_position_ += bytes;
	return can_throw_t();
}
can_throw_t CFileIO::SvVsxLe(S64 value, U32 bytes)
{
	return SvHexLe((U64)value, bytes);
}
can_throw_t CFileIO::SvFloat(F32 value)
{
    U32 data = *((U32*)(&value));
    return SvHex(data, 4);
}
can_throw_t CFileIO::SvDouble(F64 value)
{
    U64 data = *((U64*)(&value));
    return SvHex(data, 8);
}
can_throw_t CFileIO::SvString(TXT text)
{
    for(IND i = 0; i < text.size(); i++){
        SvHex(text[i], 1);
    }
    SvHex(0,1);
    return can_throw_t();
}
can_throw_t CFileIO::SvText(TXT text)
{
    for(IND i = 0; i < text.size(); i++){
        SvHex(text[i], 1);
    }
    return can_throw_t();
}
can_throw_t CFileIO::SvTextBlock(TXT text, U32 bytes)
{
    for(IND i = 0; i < bytes; i++){
        if(i < text.size()){
            SvHex(text[i], 1);
        }else{
            SvHex(0x00, 1);
        }
    }
    return can_throw_t();
}
can_throw_t CFileIO::SvHeader(TXT magic5, U32 type, U32 filecount, U32 sha256_calc_start)
{
	if(TXT(magic5).size() != 5){
		last_error_ = "Programmer Error: File header magic text must be exactly 5 characters."; 
		throw exception(last_error_);
	}
	SvText(magic5);
	if(filecount > 0){
        SvHex(filecount,3);
	}else{
        SvHex(type,1);
        SvHex(0x0000,2);
	}
	sha256_calc_start_ = sha256_calc_start;
	return can_throw_t();
}
can_throw_t CFileIO::SvLUCID(LUCID identifier)
{
	for(IND d = 0; d < 24; ++d){
	    SvHex(identifier.pData[d], 1);
	}
	return can_throw_t();
}
can_throw_t CFileIO::SvEXTHD(EXTHD extended_header)
{
	for(IND d = 0; d < 32; ++d){
	    SvHex(extended_header.pData[d], 1);
	}
	return can_throw_t();
}
can_throw_t CFileIO::SvSkipBytes(U32 bytes)
{
	buffer_.resize(buffer_.size()+bytes);
    set_position_ += bytes;
    return can_throw_t();
}
can_throw_t CFileIO::SvFile(Vx<U08>& datavec)
{
    SvHex(datavec.size(), 4);
    Append(datavec);//automatically increments the set_position_ variable
    return can_throw_t();
}
can_throw_t CFileIO::SvFile(TXT path)
{
    CFileIO f;
    f.Load(path);
    SvHex(f.ByteSize(), 4);
    Append(f.Vector());//automatically increments the set_position_ variable
    f.Close();
    return can_throw_t();
}
can_throw_t CFileIO::SvFile_NoLen(TXT path)
{
    CFileIO f;
    f.Load(path);
    Append(f.Vector());//automatically increments the set_position_ variable
    f.Close();
    return can_throw_t();
}
can_throw_t	CFileIO::SvBitmapBlock(BITMAP *bmp, U32 expected_w, U32 expected_h)
{
    if(bmp){
        if((bmp->w != expected_w) || (bmp->h != expected_h)){
            last_error_ = "Programmer Error: Raw BITMAP* dimensions do not match expected values."; 
            throw exception(last_error_);
        }
        for(IND yy = 0; yy < bmp->h; yy++){
            for(IND xx = 0; xx < bmp->w; xx++){
                SvHex(((U32*)bmp->line[yy])[xx], 4);
            }
        }
    }else{
        for(IND yy = 0; yy < expected_h; yy++){
            for(IND xx = 0; xx < expected_w; xx++){
                SvHex(0x00FF00FF, 4);
            }
        }
    }
    return can_throw_t();
}
