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
#include "include.h"
#include "CConfigSection.h"
#include "CConfig.h"
U08 CConfigSection::static_zero_ = 0x00; // KEEP THIS (see below)
CConfigSection::CConfigSection(TXT section_name, U32 secondary_id): 
section_name_(section_name), secondary_id_(secondary_id), forced_sort_(false)
{
	forced_sort_ = (section_name_.size() && (section_name_[0]=='$'));
}
CConfigSection::~CConfigSection()
{
	for(IND i = configs_.size()-1; i >= 0; --i){
		delete configs_[i];
	}
    configs_.clear();
    ordered_indexes_.clear();
    ordered_configs_.clear();
}
U08& CConfigSection::operator[](INT i)
{
	if(i >= section_name_.size()) return static_zero_; // KEEP THIS: USED FOR DATA BLOCK SORTING PURPOSES
	return *((U08*)(&section_name_[i]));
}
U32 CConfigSection::GetDataSz() const
{
	return section_name_.size();
}
INT CConfigSection::FindFirst_(U08* key, INT i, INT first, INT last)
{
    last++; //Adjust up by one since last is also element-based.
    INT l = first, r = last;
    INT mid = (l + r) / 2;
    if(last <= 0) return 0x80000000;
    if(mid <= first) return ((((*configs_[first])[i] != key[i]) << 31) | first);
	if(mid >= last) return last | 0x80000000;
	while(l != r)
    {
    	if((*configs_[mid-1])[i] < key[i])
    	{
    		if((*configs_[mid])[i] == key[i]) return mid;
			if((*configs_[mid])[i]  > key[i]) return mid | 0x80000000;
			l = ((l+1 == r)?(r):(mid));
    	}
    	else
    	{//We MUST have ((*configs_[mid])[i] >= key[i]) because ((*configs_[mid])[i] >= (*configs_[mid-1])[i])
    		r = ((l+1 == r)?(l):(mid));
    	}
    	mid = (l + r) / 2;
    	if(mid <= first) return ((((*configs_[first])[i] != key[i]) << 31) | first);
    	if(mid >= last) return last | 0x80000000;
    }
    return r | 0x80000000;
}
INT CConfigSection::FindLast_(U08* key, INT i, INT first, INT last)
{
    last++; //Adjust up by one since last is also element-based.
    INT l = first, r = last;
    INT mid = (l + r) / 2;
    if(last <= 0) return 0x80000000;
    if(mid >= last) return (((*configs_[last-1])[i] != key[i])?(last | 0x80000000):(last-1));
	if(mid <= first) return first | 0x80000000;
	while(l != r)
    {
    	if((*configs_[mid])[i] > key[i])
    	{
    		if((*configs_[mid-1])[i] == key[i]) return (mid-1);
			if((*configs_[mid-1])[i]  < key[i]) return mid | 0x80000000;
			r = ((l+1 == r)?(l):(mid));
    	}
    	else
    	{//We MUST have ((*configs_[mid-1])[i] <= key[i]) because ((*configs_[mid-1])[i] <= (*configs_[mid])[i])
    		l = ((l+1 == r)?(r):(mid));
    	}
    	mid = (l + r) / 2;
    	if(mid >= last) return (((*configs_[last-1])[i] != key[i])?(last | 0x80000000):(last-1));
    	if(mid <= first) return first | 0x80000000;
    }
    return l | 0x80000000;
}
INT CConfigSection::CompareToForInsert_(U08* key, U08* v_item, INT start, INT keylen, INT v_itemlen)
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
U32 CConfigSection::FindInsertionPoint_(CConfig* cfg, INT first, INT last, CConfig*& duplicate)
{
	if(configs_.size() == 0) return 0;
	if(first < 0) first = 0;
	if(last  < 0) last  = 0;
	if(first >= configs_.size()) first = (configs_.size()-1);
	if(last  >= configs_.size()) last  = (configs_.size()-1);
	U08* key = &((*cfg)[0]);
	INT di = 0;
	INT fst = first, lst = last;
	while(true)
	{
		if(fst == lst)
		{
			INT where = CompareToForInsert_(
                key, &((*configs_[fst])[0]), di, cfg->GetDataSz(), configs_[fst]->GetDataSz());
			if(where == -1)
			{
				duplicate = configs_[fst];
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
U32 CConfigSection::Find_(CConfig* cfg)
{
	if(configs_.size() == 0) return 0xFFFFFFFF;
	INT first = 0, last = (configs_.size()-1);
	U08* key = &((*cfg)[0]);
	INT di = 0;
	INT fst = first, lst = last;
	while(true)
	{
		if(fst == lst)
		{
			INT where = CompareToForInsert_(
                key, &((*configs_[fst])[0]), di, cfg->GetDataSz(), configs_[fst]->GetDataSz());
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
CConfig* CConfigSection::AddConfig(TXT variable_name, U32 secondary_id)
{
	// If we try to add a main config to a non-main group, the group becomes main'd.
	CheckSecondaryId_(secondary_id);
	CConfig* cfg = new CConfig(variable_name,secondary_id);
	CConfig* duplicate;
	U32 index = FindInsertionPoint_(cfg, 0, configs_.size()-1, duplicate);
	if(index == 0xFFFFFFFF){
	    delete cfg;
		return duplicate;
	}else{
		ordered_configs_.push_back(cfg);
		configs_.insert(configs_.begin()+index, cfg);
		ordered_indexes_.insert(ordered_indexes_.begin()+index, (ordered_configs_.size()-1));
		return cfg;
	}
}
void CConfigSection::DelConfig(TXT variable_name, U32 secondary_id)
{
	CConfig* cfg = new CConfig(variable_name,secondary_id); // Temporary object for search.
	U32 index = Find_(cfg);
	if((index != 0xFFFFFFFF) && (index < configs_.size()))
	{// Best to check whether we actually found it (just in case something goes haywire).
		ordered_configs_.erase(ordered_configs_.begin()+ordered_indexes_[index]);
		ordered_indexes_.erase(ordered_indexes_.begin()+index);
		configs_.erase(configs_.begin()+index);
        for(IND j = 0; j < ordered_indexes_.size(); ++j){
            if(ordered_indexes_[j] >= index){
                ordered_indexes_[j]--;
            }
        }
	}
}
CConfig* CConfigSection::GetConfig(TXT variable_name, U32 secondary_id)
{
	CConfig* cfg = new CConfig(variable_name,secondary_id); // Temporary object for search.
	U32 index = Find_(cfg);
	if((index != 0xFFFFFFFF) && (index < configs_.size()))
	{// Best to check whether we actually found it (just in case something goes haywire).
	    delete cfg;
		return configs_[index];
	}
	delete cfg;
	return NULL;
}
void CConfigSection::CloseAll()
{
    for(IND i = configs_.size()-1; i >= 0; --i){
		delete configs_[i];
	}
    configs_.clear();
    ordered_indexes_.clear();
    ordered_configs_.clear();
}
void CConfigSection::Close(U32 secondary_id)
{
	if(secondary_id_ != secondary_id){
		for(IND i = 0; i < configs_.size(); ++i){
			if(configs_[i]->GetSecondaryID() == secondary_id){
				INT index = ordered_indexes_[i];
                ordered_configs_.erase(ordered_configs_.begin()+index);
				ordered_indexes_.erase(ordered_indexes_.begin()+i);
				configs_.erase(configs_.begin()+i); i--;//BUG FIX SUBTRACT INDEX!!
                for(IND j = 0; j < ordered_indexes_.size(); ++j){
                    if(ordered_indexes_[j] >= index){
                        ordered_indexes_[j]--;
                    }
                }
			}
		}
	}else{
	    //If whole thing is from secondary_id, we take the fast route!
		configs_.clear(); //NEEDED: BUG FIX!, Prevents destructor from double-deleting.
		ordered_configs_.clear();
		ordered_indexes_.clear();
	}
}
U32 CConfigSection::GetSecondaryID()
{
	return secondary_id_;
}
void CConfigSection::CheckSecondaryId_(U32 secondary_id)
{
	if((secondary_id_ != 0xFFFFFFFF) && (secondary_id == 0xFFFFFFFF)) secondary_id_ = 0xFFFFFFFF;
}
void CConfigSection::WriteDataTo(CFileIO& ini_bfa)
{
	if(secondary_id_ != 0xFFFFFFFF) return;
	if(configs_.size() == 0) return;
	if(section_name_ != "\x01")
	{
		TXT to_save = TXT("[") + section_name_ + "]\r\n";
		ini_bfa.SvText(to_save);
	}
	if(!forced_sort_)
	{
		for(IND i = 0; i < ordered_configs_.size(); ++i)
		{
			ordered_configs_[i]->WriteDataTo(ini_bfa);
		}
	}
	else
	{
		for(IND i = 0; i < configs_.size(); ++i)
		{
			configs_[i]->WriteDataTo(ini_bfa);
		}
	}
	ini_bfa.SvText("\r\n");
}
INT CConfigSection::GetVectorPointer(CConfig**& pointer)
{
	pointer = ((CConfig**)(&ordered_configs_[0]));
	return ordered_configs_.size();
}
void CConfigSection::CopyTo(CConfigSection* other)
{
	for(IND i = 0; i < ordered_configs_.size(); ++i)
	{
		CConfig* cfg = other->AddConfig(ordered_configs_[i]->variable_name_, ordered_configs_[i]->secondary_id_);
		cfg->Load(ordered_configs_[i]->GetString());
	}
}
void CConfigSection::MoveTo(CConfigSection* other)
{
	for(IND i = 0; i < ordered_configs_.size(); ++i)
	{
		CConfig* cfg = other->AddConfig(ordered_configs_[i]->variable_name_, ordered_configs_[i]->secondary_id_);
		cfg->Load(ordered_configs_[i]->GetString());
	}
	ordered_configs_.clear();
	ordered_indexes_.clear();
	configs_.clear();
}
