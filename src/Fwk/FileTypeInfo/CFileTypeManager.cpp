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
#include "CFileTypeManager.h"
#include "CFileType.h"
Vx<CFileType* > CFileTypeManager::filetypes_;
Vx<CFileType* > CFileTypeManager::filetypes_to_delayed_load_;
Vx<CFileType* > CFileTypeManager::filetypes_to_delayed_load_percent1_;
INT CFileTypeManager::FindFirst_(U08* key, INT i, INT first, INT last)
{
    last++; //Adjust up by one since last is also element-based.
    INT l = first, r = last;
    INT mid = (l + r) / 2;
    if(last <= 0) return 0x80000000;
    if(mid <= first) return ((((*filetypes_[first])[i] != key[i]) << 31) | first);
	if(mid >= last) return last | 0x80000000;
	while(l != r)
    {
    	if((*filetypes_[mid-1])[i] < key[i])
    	{
    		if((*filetypes_[mid])[i] == key[i]) return mid;
			if((*filetypes_[mid])[i]  > key[i]) return mid | 0x80000000;
			l = ((l+1 == r)?(r):(mid));
    	}
    	else
    	{//We MUST have ((*filetypes_[mid])[i] >= key[i]) because ((*filetypes_[mid])[i] >= (*filetypes_[mid-1])[i])
    		r = ((l+1 == r)?(l):(mid));
    	}
    	mid = (l + r) / 2;
    	if(mid <= first) return ((((*filetypes_[first])[i] != key[i]) << 31) | first);
    	if(mid >= last) return last | 0x80000000;
    }
    return r | 0x80000000;
}
INT CFileTypeManager::FindLast_(U08* key, INT i, INT first, INT last)
{
    last++; //Adjust up by one since last is also element-based.
    INT l = first, r = last;
    INT mid = (l + r) / 2;
    if(last <= 0) return 0x80000000;
    if(mid >= last) return (((*filetypes_[last-1])[i] != key[i])?(last | 0x80000000):(last-1));
	if(mid <= first) return first | 0x80000000;
	while(l != r)
    {
    	if((*filetypes_[mid])[i] > key[i])
    	{
    		if((*filetypes_[mid-1])[i] == key[i]) return (mid-1);
			if((*filetypes_[mid-1])[i]  < key[i]) return mid | 0x80000000;
			r = ((l+1 == r)?(l):(mid));
    	}
    	else
    	{//We MUST have ((*filetypes_[mid-1])[i] <= key[i]) because ((*filetypes_[mid-1])[i] <= (*filetypes_[mid])[i])
    		l = ((l+1 == r)?(r):(mid));
    	}
    	mid = (l + r) / 2;
    	if(mid >= last) return (((*filetypes_[last-1])[i] != key[i])?(last | 0x80000000):(last-1));
    	if(mid <= first) return first | 0x80000000;
    }
    return l | 0x80000000;
}
INT CFileTypeManager::CompareToForInsert_(U08* key, U08* v_item, INT start, INT keylen, INT v_itemlen)
{
	INT i = start;
	while((i < keylen) && (i < v_itemlen))
	{// Compare for numerical difference in minimum length of both byte strings.
		if(key[i] < v_item[i]) return 0;
		if(key[i] > v_item[i]) return 1;
		i++;
	}
	// If the bytes were all the same up until here, tie breaker based on TXT length.
	if(keylen < v_itemlen) return 0;
	if(keylen > v_itemlen) return 1;
	// Tie breaker failed, so we know this is the exact same TXT of bytes.
	return -1;
}
U32 CFileTypeManager::FindInsertionPoint_(CFileType*  ft, INT first, INT last, CFileType* & duplicate)
{
	if(filetypes_.size() == 0) return 0;
	if(first < 0) first = 0;
	if(last  < 0) last  = 0;
	if(first >= filetypes_.size()) first = (filetypes_.size()-1);
	if(last  >= filetypes_.size()) last  = (filetypes_.size()-1);
	U08* key = &((*ft)[0]);
	INT di = 0;
	INT fst = first, lst = last;
	while(true)
	{
		if(fst == lst)
		{
			INT where = CompareToForInsert_(
                key, &((*filetypes_[fst])[0]), di, ft->GetDataSz(), filetypes_[fst]->GetDataSz());
            if(where == -1)
            {
                duplicate = filetypes_[fst];
                return 0xFFFFFFFF;
            }
			return (fst + where);
		}
		INT fst_res = FindFirst_(key, di, fst, lst);
		INT lst_res = FindLast_(key, di, fst, lst);
		if(fst_res & 0x80000000)
		{//IF no potential match is here (both fst_res AND lst_res must therefore have 0x80000000 set).
			return fst_res & 0x7FFFFFFF;
		}
		fst = fst_res;
		lst = lst_res;
		di++;
	}
}
U32 CFileTypeManager::Find_(CFileType*  ft)
{
	if(filetypes_.size() == 0) return 0xFFFFFFFF;
	INT first = 0, last = (filetypes_.size()-1);
	U08* key = &((*ft)[0]);
	INT di = 0;
	INT fst = first, lst = last;
	while(true)
	{
		if(fst == lst)
		{
			INT where = CompareToForInsert_(
                key, &((*filetypes_[fst])[0]), di, ft->GetDataSz(), filetypes_[fst]->GetDataSz());
			if(where == -1)
			{
				return fst; // Found it!
			}
			return 0xFFFFFFFF;
		}
		INT fst_res = FindFirst_(key, di, fst, lst);
		INT lst_res = FindLast_(key, di, fst, lst);
		if(fst_res & 0x80000000)
		{//IF no potential match is here (both fst_res AND lst_res must therefore have 0x80000000 set).
			return 0xFFFFFFFF;
		}
		fst = fst_res;
		lst = lst_res;
		di++;
	}
}
CFileType*  CFileTypeManager::CreateFileType_(TXT& ext, TXT& percent1_filepath, BIT delay_icon_load)
{
	// uses a copy that has only its extension member filled
	// this prevents a re-extract of already existing stuff
	CFileType*  ft = new CFileType(ext,percent1_filepath,delay_icon_load,1);
	CFileType*  duplicate;
	U32 index = FindInsertionPoint_(ft, 0, filetypes_.size()-1, duplicate);
	if(index == 0xFFFFFFFF)
	{
		if(!delay_icon_load && !duplicate->has_loaded_icons_) duplicate->LoadIcons(); //force icon to load up NOW
		return duplicate;
	}
	else
	{ 
		ft = new CFileType(ext,percent1_filepath,delay_icon_load,0);//create actual data-filled copy!
		filetypes_.insert(filetypes_.begin()+index, ft);
		if(delay_icon_load)
		{
			if(ft->is_particular_file_icons_){
				filetypes_to_delayed_load_percent1_.push_back(ft);
			}else{
				filetypes_to_delayed_load_.push_back(ft);
			}
		}
		return ft;
	}
}
void CFileTypeManager::RemoveFileType_(CFileType*  ft)
{
	U32 index = Find_(ft);
	if((index != 0xFFFFFFFF) && (index < filetypes_.size()))
	{// Best to check whether we actually found it (just in case something goes haywire).
		filetypes_.erase(filetypes_.begin()+index);
	}
}
BIT CFileTypeManager::DoDelayedLoadOfOneIcon_()
{
	INT sz = filetypes_to_delayed_load_.size();
	INT sz2 = filetypes_to_delayed_load_percent1_.size();
	if(sz)
	{	//load these first since file type more common than %1 files
		filetypes_to_delayed_load_[sz-1]->LoadIcons();
		filetypes_to_delayed_load_.pop_back();
		return true;
	}
	else if(sz2)
	{	//only load %1 icons after all other types have finished
		filetypes_to_delayed_load_percent1_[sz2-1]->LoadIcons();
		filetypes_to_delayed_load_percent1_.pop_back();
		return true;
	}
	return false;
}
void CFileTypeManager::Empty()
{
	filetypes_.clear();
}

