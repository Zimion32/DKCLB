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
/**CFileInfo::CFileInfo()
{
    icon_ = create_bitmap_ex(32,16,16);
    icon32_ = create_bitmap_ex(32,32,32);
    clear_to_color(icon_, 0x00FF00FF);
    clear_to_color(icon32_, 0x00FF00FF);
    thumb_ = NULL;
    photo_ = NULL;
    what_ = "NULL";
    author_ = "NULL";
    description_ = "NULL";
    notes_ = "NULL";
    deleted_on_hdd_ = false;
    is_from_cdl_file_ = false;
    changed_what_ = false;
    version_newest_ = 0;
}
//#############################################################################
//#############################################################################
void CFileInfo::WriteHeaderTo(CFileIO& file, INT filecount)
{
    dtime dt;
    dt.SetToCurrentTime();
    exthd_.SetDateLastSaved(dt);
    file.SvHeader("DELTA", type_, filecount, 64);
    file.SvLUCID(lucid_);
    file.SvEXTHD(exthd_);
}
void CFileInfo::WriteHeaderTo(CFileIO& file)
{
    dtime dt;
    dt.SetToCurrentTime();
    exthd_.SetDateLastSaved(dt);
    file.SvHeader("DELTA", type_, 0, 64);
    file.SvLUCID(lucid_);
    file.SvEXTHD(exthd_);
}
void CFileInfo::WriteSubHeaderTo(CFileIO& file)
{
    file.SvHex(size_actual_,4);
    file.SvHex(type_,4);
    file.SvLUCID(lucid_);
    file.SvEXTHD(exthd_);
}
void CFileInfo::WriteDescriptionTo(CFileIO& file)
{
    //The description is a 8192-byte statically-sized structure.
    //Predetermined size allows for CFileIO to LoadSectionEx() on **only** this data.
    file.SvTextBlock(what_,        256);      // 256 bytes
    file.SvTextBlock(author_,      256);      // 256 bytes
    file.SvTextBlock(description_, 256+1024); //1280 bytes
    file.SvTextBlock(notes_,       256+1024); //1280 bytes
    file.SvBitmapBlock(icon_,        16, 16); //1024 bytes
    file.SvBitmapBlock(icon32_,      32, 32); //4096 bytes (used sparingly)
}
*/
