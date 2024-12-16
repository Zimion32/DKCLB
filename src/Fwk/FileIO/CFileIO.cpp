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

CFileIO FIO;

CFileIO::CFileIO()
{
    get_position_ = 0;
    set_position_ = 0;
    last_error_ = "";
    SetupChunkValues_();
    has_subfiles_ = false;
    subfile_offset_ = 0;
    subfile_sizes_.clear();
    subfile_dgeids_.clear();
}
CFileIO::~CFileIO()
{
	
}
U08& CFileIO::operator[](U32 index)
{
	return buffer_[index];
}
U08& CFileIO::operator()(U32 index)
{
	return buffer_[index];
}
UVL CFileIO::operator()(U32 index, U32 bytes)
{
	// Type-wrench the U08 pointer into a UVL and return.
	UVL h(((PTR)&buffer_[index]), bytes); return h;
}
can_throw_t CFileIO::Reserve(U32 reserve_memory)
{
	buffer_.reserve(reserve_memory);
	return can_throw_t();
}
void CFileIO::FreeMemory()
{
    get_position_ = 0;
	set_position_ = 0;
    buffer_.clear();
    Vx<U08>().swap(buffer_);///This de-allocates any used memory.
    SetupChunkValues_();
    has_subfiles_ = false;
    subfile_offset_ = 0;
    subfile_sizes_.clear();
    subfile_dgeids_.clear();
}
//############################################################################//
//############################################################################//
can_throw_t CFileIO::SetR(U32 location)
{
	get_position_ = location;
	return can_throw_t();
}
can_throw_t CFileIO::SetW(U32 location)
{
	set_position_ = location;
	return can_throw_t();
}
U32 CFileIO::GetR()
{
	return get_position_;
}
U32 CFileIO::GetW()
{
	return set_position_;
}
U32 CFileIO::ByteSize()
{
	return buffer_.size();
}
Vx<U08>& CFileIO::Vector()
{
	return buffer_;
}
