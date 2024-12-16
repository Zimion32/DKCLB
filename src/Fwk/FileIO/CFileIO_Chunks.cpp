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
void CFileIO::SetupChunkValues_()
{
    len_buffer_.clear();
    len_position_ = (ByteSize() - 4);
    len_begin_ = 0; //(if zero, no chunk started)
    sha256_calc_start_ = 0;
    last_chunk_length_ = 0;
    last_chunk_id_ = 0;
}
can_throw_t	CFileIO::GvCheckSHA256(U32 sha256_start)
{
    if(ByteSize() < (sha256_start+32)){
		last_error_ = "Error: The file isn't large enough to have a SHA256 checksum!!"; 
		throw exception(last_error_);
    }
    B32 file_value = *((B32*)&buffer_[ByteSize()-32]);
    B32 hash_value = SHA256(&buffer_[sha256_start], ByteSize()-sha256_start-32);
    len_position_ -= 32;
    if(hash_value != file_value){
        last_error_ = "Error: SHA256 checksum doesn't match saved value. The file is corrupt!!"; 
        throw exception(last_error_);
	}
	return can_throw_t();
}
can_throw_tt<U32> CFileIO::GvChunkID()
{
    U32 id = GvHex(4);
    if(id != 0){
        U32 px = GetR();
        SetR(len_position_);
        last_chunk_length_ = GvHex(4);
        len_position_ -= 4;
        SetR(px);
    }
    return id;
}
can_throw_tt<U32> CFileIO::GvPeekAtChunkID()
{
    U32 px = GetR();
    U32 id = GvHex(4);
    SetR(px);
    return id;
}
can_throw_t CFileIO::GvIgnoreChunk()
{
    return GvSkipBytes(last_chunk_length_);
}
can_throw_tt<BIT> CFileIO::GvIsSectionEntry()
{
    //return true while section is not over
    U32 id = GvPeekAtChunkID();
    if(id == 0){
        id = GvChunkID();
        return (id != 0);
    }
    return true;
}
can_throw_tt<BIT> CFileIO::GvIsSectionArrayEntry()
{
    //return true while section array is not over
    U32 id = GvPeekAtChunkID();
    if(id == 0){
        id = GvChunkID();
        return (id != 0);
    }
    return true;
}

//############################################################################//
//############################################################################//

void CFileIO::EndChunk_()
{
    if((len_begin_ != 0) && (last_chunk_id_ != 0)){
        len_buffer_.push_back(GetW() - len_begin_);
    }
}
can_throw_t CFileIO::SvChunkID(U32 id)
{
    EndChunk_();
    SvHex(id, 4);
    last_chunk_id_ = id;
    len_begin_ = GetW();
    return can_throw_t();
}
can_throw_t CFileIO::SvFooter()
{
    EndChunk_();
    for(IND i = (len_buffer_.size()-1); i >= 0; i--){ //must be in reverse order!!
        SvHex(len_buffer_[i], 4);
    }
    B32 hash_value = SHA256(
        &buffer_[has_subfiles_ ?            subfile_offset_ :            sha256_calc_start_ ], 
                (has_subfiles_ ?(ByteSize()-subfile_offset_):(ByteSize()-sha256_calc_start_)));
    U32 hash_write_position = GetW();
    for(IND i = 0; i < 32; i++){
        SvHex(0x00, 1);
        buffer_[hash_write_position++] = ((U08*)&hash_value)[i];
    }
    if(has_subfiles_){
        subfile_sizes_.push_back(GetW() - subfile_offset_);
        subfile_offset_ = GetW();
    }
    return can_throw_t();
}
can_throw_t CFileIO::SvBeginSection()
{
    return can_throw_t();
}
can_throw_t CFileIO::SvEndSection()
{
    return SvChunkID(0);
}
can_throw_t CFileIO::SvBeginSectionArray()
{
    return can_throw_t();
}
can_throw_t CFileIO::SvEndSectionArray()
{
    return SvChunkID(0);
}

//############################################################################//
//############################################################################//

void CFileIO::SvSubFile(U32 type)
{
    if(!has_subfiles_){
        has_subfiles_ = true;
        subfile_offset_ = GetW();
        subfile_sizes_.clear();
        subfile_dgeids_.clear();
    }
    subfile_dgeids_.push_back(type);
}
can_throw_tt<U32> CFileIO::GetSubFileCount()
{
    return subfile_sizes_.size();
}
can_throw_tt<U32> CFileIO::GetSubFileSize(U32 which)
{
    if(which >= subfile_sizes_.size()) return 0;
    return subfile_sizes_[which];
}
can_throw_tt<DGEID> CFileIO::GetSubFileDgeid(U32 which)
{
    if(which >= subfile_dgeids_.size()) return (DGEID)0;
    return subfile_dgeids_[which];
}
