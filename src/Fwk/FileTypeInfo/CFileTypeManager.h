#pragma once
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
#define CFT CFileType* 
class CFileType;
class CFileTypeManager
{
	friend class CFileType;
	public:
static  void    Empty();
friend  CFT     FileType(TXT ext, TXT percent1_filepath, BIT delay_icon_load);
friend  BIT		FileType_DelayLoadAnIcon();
        
	private:
static  Vx<CFT> filetypes_;
static  Vx<CFT> filetypes_to_delayed_load_;
static  Vx<CFT>	filetypes_to_delayed_load_percent1_;
static  U32		Find_(CFileType*  ft);
static  U32		FindInsertionPoint_(CFileType*  ft, INT first, INT last, CFileType* & duplicate);
static  INT		FindFirst_(U08* key, INT i, INT first, INT last);
static  INT		FindLast_(U08* key, INT i, INT first, INT last);
static  INT     CompareToForInsert_(U08* key, U08* v_item, INT start, INT keylen, INT v_itemlen);
static  CFT		CreateFileType_(TXT& ext, TXT& percent1_filepath, BIT delay_icon_load);
static  void	RemoveFileType_(CFileType*  ft); // Only allowed from a CFileType instance.
static  BIT		DoDelayedLoadOfOneIcon_();
};
#undef CFT
