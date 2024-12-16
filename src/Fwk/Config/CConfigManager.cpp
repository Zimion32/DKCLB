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
#include "CConfigManager.h"
#include "CConfigSection.h"
#include "CConfig.h"

INT CConfigManager::FindFirst_(U08* key, INT i, INT first, INT last)
{
    last++; //Adjust up by one since last is also element-based.
    INT l = first, r = last;
    INT mid = (l + r) / 2;
    if(last <= 0) return 0x80000000;
    if(mid <= first) return ((((*config_sections_[first])[i] != key[i]) << 31) | first);
	if(mid >= last) return last | 0x80000000;
	while(l != r)
    {
        if((*config_sections_[mid-1])[i] < key[i])
    	{
    		if((*config_sections_[mid])[i] == key[i]) return mid;
			if((*config_sections_[mid])[i]  > key[i]) return mid | 0x80000000;
			l = ((l+1 == r)?(r):(mid));
    	}
    	else
    	{//We MUST have ((*config_sections_[mid])[i] >= key[i]) because ((*config_sections_[mid])[i] >= (*config_sections_[mid-1])[i])
    		r = ((l+1 == r)?(l):(mid));
    	}
    	mid = (l + r) / 2;
    	if(mid <= first) return ((((*config_sections_[first])[i] != key[i]) << 31) | first);
    	if(mid >= last) return last | 0x80000000;
    }
    return r | 0x80000000;
}
INT CConfigManager::FindLast_(U08* key, INT i, INT first, INT last)
{
    last++; //Adjust up by one since last is also element-based.
    INT l = first, r = last;
    INT mid = (l + r) / 2;
    if(last <= 0) return 0x80000000;
    if(mid >= last) return (((*config_sections_[last-1])[i] != key[i])?(last | 0x80000000):(last-1));
	if(mid <= first) return first | 0x80000000;
	while(l != r)
    {
    	if((*config_sections_[mid])[i] > key[i])
    	{
    		if((*config_sections_[mid-1])[i] == key[i]) return (mid-1);
			if((*config_sections_[mid-1])[i]  < key[i]) return mid | 0x80000000;
			r = ((l+1 == r)?(l):(mid));
    	}
    	else
    	{//We MUST have ((*config_sections_[mid-1])[i] <= key[i]) because ((*config_sections_[mid-1])[i] <= (*config_sections_[mid])[i])
    		l = ((l+1 == r)?(r):(mid));
    	}
    	mid = (l + r) / 2;
    	if(mid >= last) return (((*config_sections_[last-1])[i] != key[i])?(last | 0x80000000):(last-1));
    	if(mid <= first) return first | 0x80000000;
    }
    return l | 0x80000000;
}
INT CConfigManager::CompareToForInsert_(U08* key, U08* v_item, INT start, INT keylen, INT v_itemlen)
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
U32 CConfigManager::FindInsertionPoint_(CConfigSection* cfgs, INT first, INT last, CConfigSection*& duplicate)
{
	if(config_sections_.size() == 0) return 0;
	if(first < 0) first = 0;
	if(last  < 0) last  = 0;
	if(first >= config_sections_.size()) first = (config_sections_.size()-1);
	if(last  >= config_sections_.size()) last  = (config_sections_.size()-1);
	U08* key = &((*cfgs)[0]);
	INT di = 0;
	INT fst = first, lst = last;
	while(true)
	{
		if(fst == lst)
		{
			INT where = CompareToForInsert_(
                key, &((*config_sections_[fst])[0]), di, cfgs->GetDataSz(), config_sections_[fst]->GetDataSz());
			if(where == -1)
			{
				duplicate = config_sections_[fst];
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
U32 CConfigManager::Find_(CConfigSection* cfgs)
{
	if(config_sections_.size() == 0) return 0xFFFFFFFF;
	INT first = 0, last = (config_sections_.size()-1);
	U08* key = &((*cfgs)[0]);
	INT di = 0;
	INT fst = first, lst = last;
	while(true)
	{
		if(fst == lst)
		{
			INT where = CompareToForInsert_(
                key, &((*config_sections_[fst])[0]), di, cfgs->GetDataSz(), config_sections_[fst]->GetDataSz());
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
CConfigSection* CConfigManager::AddConfigSection(TXT section_name, U32 secondary_id)
{
	CConfigSection* cfgs = new CConfigSection(section_name,secondary_id);
	CConfigSection* duplicate;
	U32 index = FindInsertionPoint_(cfgs, 0, config_sections_.size()-1, duplicate);
	if(index == 0xFFFFFFFF){
        delete cfgs;
		return duplicate;
	}else{
		ordered_config_sections_.push_back(cfgs);
		config_sections_.insert(config_sections_.begin()+index, cfgs);
		ordered_indexes_.insert(ordered_indexes_.begin()+index, (ordered_config_sections_.size()-1));
		return cfgs;
	}
}
void CConfigManager::DelConfigSection(TXT section_name, U32 secondary_id)
{
	CConfigSection* cfgs = new CConfigSection(section_name,secondary_id); // Temporary object for search.
	U32 index = Find_(cfgs);
	if((index != 0xFFFFFFFF) && (index < config_sections_.size()))
	{// Best to check whether we actually found it (just in case something goes haywire).
		ordered_config_sections_.erase(ordered_config_sections_.begin()+ordered_indexes_[index]);
		ordered_indexes_.erase(ordered_indexes_.begin()+index);
		config_sections_.erase(config_sections_.begin()+index);
        for(IND j = 0; j < ordered_indexes_.size(); ++j){
            if(ordered_indexes_[j] >= index){
                ordered_indexes_[j]--;
            }
        }
	}
}
CConfigSection* CConfigManager::GetConfigSection(TXT section_name, U32 secondary_id)
{
	CConfigSection* cfgs = new CConfigSection(section_name,secondary_id); // Temporary object for search.
	U32 index = Find_(cfgs);
	if((index != 0xFFFFFFFF) && (index < config_sections_.size()))
	{// Best to check whether we actually found it (just in case something goes haywire).
        delete cfgs;
		return config_sections_[index];
	}
	delete cfgs;
	return NULL;
}
void CConfigManager::CloseAll()
{
	for(IND i = 0; i < config_sections_.size(); ++i){
		config_sections_[i]->CloseAll();
		delete config_sections_[i];
	}
	config_sections_.clear();
	ordered_indexes_.clear();
	ordered_config_sections_.clear();
}
void CConfigManager::Close(U32 secondary_id)
{
	for(IND i = 0; i < config_sections_.size(); ++i){
		config_sections_[i]->Close(secondary_id);
		if(config_sections_[i]->GetSecondaryID() == secondary_id){
			INT index = ordered_indexes_[i];
            ordered_config_sections_.erase(ordered_config_sections_.begin()+index);
			ordered_indexes_.erase(ordered_indexes_.begin()+i);
			config_sections_.erase(config_sections_.begin()+i); i--;//BUG FIX SUBTRACT INDEX!!
			for(IND j = 0; j < ordered_indexes_.size(); ++j){
			    if(ordered_indexes_[j] >= index){
			        ordered_indexes_[j]--;
                }
            }
		}
	}
}
void CConfigManager::WriteDataTo(CFileIO& ini_bfa)
{
	if(config_sections_.size() && (config_sections_[0]->section_name_ == "\x01")){
		config_sections_[0]->WriteDataTo(ini_bfa);
	}
	for(IND i = 0; i < ordered_config_sections_.size(); ++i){
		if(ordered_config_sections_[i]->section_name_ != "\x01"){
			ordered_config_sections_[i]->WriteDataTo(ini_bfa);
		}
	}
}
static CFileIO ini_bfa; 

BIT KConfig::Open(TXT file)
{// Call with "" to close the INI.
	if(file == "")
	{
		config_manager_.Close(0xFFFFFFFF);
		ini_main_file = "";
		ini_last_load_failure = false;
		return false;
	}
	if(ini_main_file == file) return false; //already open!!!
	TXT ini = ""; //Reserved characters: []=#
	try{
	ini_bfa.Load(file);
	if((ini_main_file != "") || ini_last_load_failure)
	{//if we already had a main INI loaded, close that one and load the new one!
		config_manager_.Close(0xFFFFFFFF);
	}
	for(IND i = 0, j = ini_bfa.ByteSize(); i < j; ++i)
	{
		if((i < j) && ((ini_bfa[i]=='\r') || (ini_bfa[i]=='\n')))
		{
			ini += '\x10';
			while((i < j) && 
            ((ini_bfa[i]=='\r') || (ini_bfa[i]=='\n') || (ini_bfa[i]==' ') || (ini_bfa[i]=='\t'))){i++;}
			i--; continue;
		}
		if((i < j) && (ini_bfa[i]=='#'))
		{
			while((i < j) && (ini_bfa[i]!='\r') && (ini_bfa[i]!='\n')){i++;}
			if(ini.size() && (ini[ini.size()-1]!='\x10')) ini += '\x10';
			while((i < j) && ((ini_bfa[i]=='\r') || (ini_bfa[i]=='\n') || (ini_bfa[i]==' ') || (ini_bfa[i]=='\t'))){i++;}
			i--; continue;
		}
		if((i < j) && ((ini_bfa[i]==' ') || (ini_bfa[i]=='\t')))
		{//Need to check for ...<whitespace><= or \n><whitespace>... pattern and remove ONLY that whitespace.
			TXT the_whitespace = "";
			while((i < j) && ((ini_bfa[i]==' ') || (ini_bfa[i]=='\t')))
			{
				the_whitespace += ini_bfa[i];
				i++;
			}
			if((i < j) && (ini_bfa[i]=='='))
			{
				i++; ini += '\x11';
				while((i < j) && ((ini_bfa[i]==' ') || (ini_bfa[i]=='\t'))){i++;}
				i--; continue;
			}
			else if((i < j) && ((ini_bfa[i]=='\r') || (ini_bfa[i]=='\n')))
			{
				i++; ini += '\x10';
				while((i < j) && ((ini_bfa[i]=='\r') || (ini_bfa[i]=='\n') || (ini_bfa[i]==' ') || (ini_bfa[i]=='\t'))){i++;}
				i--; continue;
			}
			else
			{
				if(ini.size()) ini += the_whitespace; // only add if not at start of file.
				i--; continue;
			}
		}
		//check for whitespace after = when no space was before that token.
		if((i < j) && (ini_bfa[i]=='='))
		{
			i++; ini += '\x11';
			while((i < j) && ((ini_bfa[i]==' ') || (ini_bfa[i]=='\t'))){i++;}
			i--; continue;
		}
		//regular characters and escape sequences
		if((i < j) && (ini_bfa[i]<' ') && (ini_bfa[i]!='\t')) continue;
		if((i<(ini_bfa.ByteSize()-1)) && (ini_bfa[i]=='\\')
		&& ((ini_bfa[i+1]=='#') || (ini_bfa[i+1]=='n') || (ini_bfa[i+1]=='\\') 
		|| (ini_bfa[i+1]=='t') || (ini_bfa[i+1]=='r') || (ini_bfa[i+1]=='=')))
		{
			if(ini_bfa[i+1]=='r'){i++; ini += '\r'; continue;}
			if(ini_bfa[i+1]=='n'){i++; ini += '\n'; continue;}
			if(ini_bfa[i+1]=='#'){i++; ini += '#'; continue;}
			if(ini_bfa[i+1]=='='){i++; ini += '='; continue;}
			if(ini_bfa[i+1]=='t'){i++; ini += '\t'; continue;}
			if(ini_bfa[i+1]=='\\'){i++; ini += '\\'; continue;}
			i++; ini += ini_bfa[i+1]; continue;
		}
		if(i < j) ini += ini_bfa[i];
	}
	ini_bfa.Close();
	}catch(const exception& e){
		// ini routines will still preserve runtime INI data if open fails.
		ini_main_file = file;
		ini_bfa.Close();
		ini_last_load_failure = true;
		return true;
	}endtry;
	CConfigSection* current_section = config_manager_.AddConfigSection("\x01");
	U32 position = 0;
	while(true)
	{
		if(position >= ini.size()) break;
		if(ini[position] == '\x10') position++;
		U32 found = ini.find_first_of("\x10",position);
		BIT end_file = (found == TXT::npos);
		if(end_file) found = ini.size();
		if((found-position) <= 0) break;
		TXT thisline = ini.substr(position,(found-position));
		position = found+1; // skip over the \x10
		U32 vpos = thisline.find_first_of("\x11",0);
		BIT is_valid_section_line  = ((thisline.size() >= 2) && (thisline[thisline.size()-1]==']') && (thisline[0]=='['));
		BIT is_valid_variable_line = ((vpos != TXT::npos) && ((thisline.size()-1) >= vpos));
		if(current_section){
		if(is_valid_variable_line)
		{//prefer adding a variable over making a section, if we get [...=...] in the file.
			TXT variable = thisline.substr(0,vpos);
			TXT value = thisline.substr(vpos+1,(thisline.size()-(vpos+1)));
			CConfig* config = current_section->AddConfig(variable);
			config->Load(value);
		}
		else if(is_valid_section_line)
		{//technically, a section called [] is both possible and allowed.
			TXT section = thisline.substr(1,(thisline.size()-2));
			current_section = config_manager_.AddConfigSection(section);
		}
		else
		{// OOPS. We found an invalid line! IGNORE EVERYTHING UNTIL NEXT SECTION.
			current_section = NULL;
		}}
		else if(is_valid_section_line && !is_valid_variable_line)
		{//We want to be ABSOLUTELY SURE that this is a section name!
			TXT section = thisline.substr(1,(thisline.size()-2));
			current_section = config_manager_.AddConfigSection(section);
		}
	}
	ini_main_file = file;
	ini_last_load_failure = false;
	return false;
}
BIT KConfig::OpenSecondary(TXT file, INT secondary_id)
{// Call with NULL to close a secondary INI.
	if(file == "")
	{
		config_manager_.Close(secondary_id);
		return false;
	}
	TXT ini = ""; //Reserved characters: []=#
	try{
	ini_bfa.Load(file);
	for(IND i = 0, j = ini_bfa.ByteSize(); i < j; ++i)
	{
		if((i < j) && ((ini_bfa[i]=='\r') || (ini_bfa[i]=='\n')))
		{
			ini += '\x10';
			while((i < j) && ((ini_bfa[i]=='\r') || (ini_bfa[i]=='\n') || (ini_bfa[i]==' ') || (ini_bfa[i]=='\t'))){i++;}
			i--; continue;
		}
		if((i < j) && (ini_bfa[i]=='#'))
		{
			while((i < j) && ((ini_bfa[i]!='\r') && (ini_bfa[i]!='\n'))){i++;}
			if(ini.size() && (ini[ini.size()-1]!='\x10')) ini += '\x10';
			while((i < j) && ((ini_bfa[i]=='\r') || (ini_bfa[i]=='\n') || (ini_bfa[i]==' ') || (ini_bfa[i]=='\t'))){i++;}
			i--; continue;
		}
		if((i < j) && ((ini_bfa[i]==' ') || (ini_bfa[i]=='\t')))
		{//Need to check for ...<whitespace><= or \n><whitespace>... pattern and remove ONLY that whitespace.
			TXT the_whitespace = "";
			while((i < j) && ((ini_bfa[i]==' ') || (ini_bfa[i]=='\t')))
			{
				the_whitespace += ini_bfa[i];
				i++;
			}
			if((i < j) && (ini_bfa[i]=='='))
			{
				i++; ini += '\x11';
				while((ini_bfa[i]==' ') || (ini_bfa[i]=='\t')){i++;}
				i--; continue;
			}
			else if((i < j) && ((ini_bfa[i]=='\r') || (ini_bfa[i]=='\n')))
			{
				i++; ini += '\x10';
				while((i < j) && ((ini_bfa[i]=='\r') || (ini_bfa[i]=='\n') || (ini_bfa[i]==' ') || (ini_bfa[i]=='\t'))){i++;}
				i--; continue;
			}
			else
			{
				if(ini.size()) ini += the_whitespace; // only add if not at start of file.
				i--; continue;
			}
		}
		//check for whitespace after = when no space was before that token.
		if((i < j) && (ini_bfa[i]=='='))
		{
			i++; ini += '\x11';
			while((ini_bfa[i]==' ') || (ini_bfa[i]=='\t')){i++;}
			i--; continue;
		}
		//regular characters and escape sequences
		if((i < j) && (ini_bfa[i]<' ') && (ini_bfa[i]!='\t')) continue;
		if((i<(ini_bfa.ByteSize()-1)) && (ini_bfa[i]=='\\')
		&& ((ini_bfa[i+1]=='#') || (ini_bfa[i+1]=='n') || (ini_bfa[i+1]=='\\') 
		|| (ini_bfa[i+1]=='t') || (ini_bfa[i+1]=='r') || (ini_bfa[i+1]=='=')))
		{
			if(ini_bfa[i+1]=='r'){i++; ini += '\r'; continue;}
			if(ini_bfa[i+1]=='n'){i++; ini += '\n'; continue;}
			if(ini_bfa[i+1]=='#'){i++; ini += '#'; continue;}
			if(ini_bfa[i+1]=='='){i++; ini += '='; continue;}
			if(ini_bfa[i+1]=='t'){i++; ini += '\t'; continue;}
			if(ini_bfa[i+1]=='\\'){i++; ini += '\\'; continue;}
			i++; ini += ini_bfa[i+1]; continue;
		}
		if(i < j) ini += ini_bfa[i];
	}
	ini_bfa.Close();
	}catch(const exception& e){
		ini_bfa.Close();
		return true;
	}endtry;
	CConfigSection* current_section = config_manager_.AddConfigSection("\x01",secondary_id);
	U32 position = 0;
	while(true)
	{
		if(position >= ini.size()) break;
		if(ini[position] == '\x10') position++;
		U32 found = ini.find_first_of("\x10",position);
		BIT end_file = (found == TXT::npos);
		if(end_file) found = ini.size();
		if((found-position) <= 0) break;
		TXT thisline = ini.substr(position,(found-position));
		position = found+1; // skip over the \x10
		U32 vpos = thisline.find_first_of("\x11",0);
		BIT is_valid_section_line  = ((thisline.size() >= 2) && (thisline[thisline.size()-1]==']') && (thisline[0]=='['));
		BIT is_valid_variable_line = ((vpos != TXT::npos) && ((thisline.size()-1) >= vpos));
		if(current_section){
		if(is_valid_variable_line)
		{//prefer adding a variable over making a section, if we get [...=...] in the file.
			TXT variable = thisline.substr(0,vpos);
			TXT value = thisline.substr(vpos+1,(thisline.size()-(vpos+1)));
			CConfig* config = current_section->AddConfig(variable,secondary_id);
			config->Load(value);
		}
		else if(is_valid_section_line)
		{//technically, a section called [] is both possible and allowed.
			TXT section = thisline.substr(1,(thisline.size()-2));
			current_section = config_manager_.AddConfigSection(section,secondary_id);
		}
		else
		{// OOPS. We found an invalid line! IGNORE EVERYTHING UNTIL NEXT SECTION.
			current_section = NULL;
		}}
		else if(is_valid_section_line && !is_valid_variable_line)
		{//We want to be ABSOLUTELY SURE that this is a section name!
			TXT section = thisline.substr(1,(thisline.size()-2));
			current_section = config_manager_.AddConfigSection(section);
		}
	}
	return false;
}
void KConfig::Exit()
{
    config_manager_.CloseAll();
}
BIT KConfig::Save()
{// Saves main INI file.
	if(ini_main_file == "") return false;
	try{
		config_manager_.WriteDataTo(ini_bfa);
		ini_bfa.Save(ini_main_file);
		ini_bfa.Close();
	}catch(const exception& e){
		ini_bfa.Close();
		return true;
	}endtry;
	return false;
}

static TXT nullsection_name = "\x01";
static TXT nullvariable_name = "";
// These routines set ini entries in only main ini file.
TXT	 KConfig::SetStr(TXT section, TXT variable, TXT str)
{
	if(section == "") section = nullsection_name;
	if(variable == "") variable = nullvariable_name;
	config_manager_.AddConfigSection(section)->AddConfig(variable)->Set(str);
	return str;
}
U64 KConfig::SetHex64(TXT section, TXT variable, U64 value, INT index)
{
	if(section == "") section = nullsection_name;
	if(variable == "") variable = nullvariable_name;
	config_manager_.AddConfigSection(section)->AddConfig(variable)->Set((U64)value,index);
	return value;
}
S64 KConfig::SetVsx64(TXT section, TXT variable, S64 value, INT index)
{
	if(section == "") section = nullsection_name;
	if(variable == "") variable = nullvariable_name;
	config_manager_.AddConfigSection(section)->AddConfig(variable)->Set((S64)value,index);
	return value;
}
U32 KConfig::SetHex(TXT section, TXT variable, U32 value, INT index)
{
	if(section == "") section = nullsection_name;
	if(variable == "") variable = nullvariable_name;
	config_manager_.AddConfigSection(section)->AddConfig(variable)->Set((U32)value,index);
	return value;
}
S32 KConfig::SetVsx(TXT section, TXT variable, S32 value, INT index)
{
	if(section == "") section = nullsection_name;
	if(variable == "") variable = nullvariable_name;
	config_manager_.AddConfigSection(section)->AddConfig(variable)->Set((S32)value,index);
	return value;
}
S32 KConfig::SetInt(TXT section, TXT variable, S32 value)
{
	if(section == "") section = nullsection_name;
	if(variable == "") variable = nullvariable_name;
	config_manager_.AddConfigSection(section)->AddConfig(variable)->Set((S32)value);
	return value;
}
void KConfig::SetWP(TXT section, U32 window_id, S16 x, S16 y, S16 w, S16 h, BIT m)
{
	if(section == "") section = nullsection_name;
	config_manager_.AddConfigSection(section)->AddConfig(String.FromHex(window_id,8))->SetWinPos(x,y,w,h,m);
}
S16 KConfig::SetWPx(TXT section, U32 window_id, S16 x)
{
	if(section == "") section = nullsection_name;
	S16 _dummy, y, w, h; BIT m;
	GetWP(section, window_id, _dummy, y, w, h, m);
	config_manager_.AddConfigSection(section)->AddConfig(String.FromHex(window_id,8))->SetWinPos(x,y,w,h,m);
	return x;
}
S16 KConfig::SetWPy(TXT section, U32 window_id, S16 y)
{
	if(section == "") section = nullsection_name;
	S16 x, _dummy, w, h; BIT m;
	GetWP(section, window_id, x, _dummy, w, h, m);
	config_manager_.AddConfigSection(section)->AddConfig(String.FromHex(window_id,8))->SetWinPos(x,y,w,h,m);
	return y;
}
S16 KConfig::SetWPw(TXT section, U32 window_id, S16 w)
{
	if(section == "") section = nullsection_name;
	S16 x, y, _dummy, h; BIT m;
	GetWP(section, window_id, x, y, _dummy, h, m);
	config_manager_.AddConfigSection(section)->AddConfig(String.FromHex(window_id,8))->SetWinPos(x,y,w,h,m);
	return w;
}
S16 KConfig::SetWPh(TXT section, U32 window_id, S16 h)
{
	if(section == "") section = nullsection_name;
	S16 x, y, w, _dummy; BIT m;
	GetWP(section, window_id, x, y, w, _dummy, m);
	config_manager_.AddConfigSection(section)->AddConfig(String.FromHex(window_id,8))->SetWinPos(x,y,w,h,m);
	return h;
}
S16 KConfig::SetWPm(TXT section, U32 window_id, BIT m)
{
	if(section == "") section = nullsection_name;
	S16 x, y, w, h; BIT _dummy;
	GetWP(section, window_id, x, y, w, h, _dummy);
	config_manager_.AddConfigSection(section)->AddConfig(String.FromHex(window_id,8))->SetWinPos(x,y,w,h,m);
	return m;
}

// These routines get ini entries in either main or secondary ini file.
TXT KConfig::GetStr(TXT section, TXT variable, TXT def)
{
	if(section == "") section = nullsection_name;
	if(variable == "") variable = nullvariable_name;
	CConfigSection* section_ptr = config_manager_.GetConfigSection(section); if(!section_ptr) return def;
	CConfig* variable_ptr = section_ptr->GetConfig(variable); if(!variable_ptr) return def;
	return variable_ptr->GetString();
}
U64 KConfig::GetHex64(TXT section, TXT variable, U64 def, INT index)
{
	if(section == "") section = nullsection_name;
	if(variable == "") variable = nullvariable_name;
	CConfigSection* section_ptr = config_manager_.GetConfigSection(section); if(!section_ptr) return def;
	CConfig* variable_ptr = section_ptr->GetConfig(variable); if(!variable_ptr) return def;
	U64 result = variable_ptr->GetHex64(index);
	if(result == 0x8000000000000000LL) return def;
	return result;
}
S64 KConfig::GetVsx64(TXT section, TXT variable, S64 def, INT index)
{
	if(section == "") section = nullsection_name;
	if(variable == "") variable = nullvariable_name;
	CConfigSection* section_ptr = config_manager_.GetConfigSection(section); if(!section_ptr) return def;
	CConfig* variable_ptr = section_ptr->GetConfig(variable); if(!variable_ptr) return def;
	S64 result = variable_ptr->GetVsx64(index);
	if(result == 0x8000000000000000LL) return def;
	return result;
}
U32 KConfig::GetHex(TXT section, TXT variable, U32 def, INT index)
{
	if(section == "") section = nullsection_name;
	if(variable == "") variable = nullvariable_name;
	CConfigSection* section_ptr = config_manager_.GetConfigSection(section); if(!section_ptr) return def;
	CConfig* variable_ptr = section_ptr->GetConfig(variable); if(!variable_ptr) return def;
	U32 result = variable_ptr->GetHex(index);
	if(result == 0x80000000) return def;
	return result;
}
S32 KConfig::GetVsx(TXT section, TXT variable, S32 def, INT index)
{
	if(section == "") section = nullsection_name;
	if(variable == "") variable = nullvariable_name;
	CConfigSection* section_ptr = config_manager_.GetConfigSection(section); if(!section_ptr) return def;
	CConfig* variable_ptr = section_ptr->GetConfig(variable); if(!variable_ptr) return def;
	S32 result = variable_ptr->GetVsx(index);
	if(result == 0x80000000) return def;
	return result;
}
S32 KConfig::GetInt(TXT section, TXT variable, S32 def)
{
	if(section == "") section = nullsection_name;
	if(variable == "") variable = nullvariable_name;
	CConfigSection* section_ptr = config_manager_.GetConfigSection(section); if(!section_ptr) return def;
	CConfig* variable_ptr = section_ptr->GetConfig(variable); if(!variable_ptr) return def;
	S32 result = variable_ptr->GetInt();
	if(result == 0x80000000) return def;
	return result;
}
void KConfig::GetWP(TXT section, U32 window_id, S16& x, S16& y, S16& w, S16& h, BIT& m)
{
	if(section == "") section = nullsection_name;
	CConfigSection* section_ptr = config_manager_.GetConfigSection(section); if(!section_ptr) return;
	CConfig* variable_ptr = section_ptr->GetConfig(String.FromHex(window_id,8)); if(!variable_ptr) return;
	variable_ptr->GetWinPosNoOverwrite(x, y, w, h, m); return;
}
S16 KConfig::GetWPx(TXT section, U32 window_id, S16 def)
{
	if(section == "") section = nullsection_name;
	CConfigSection* section_ptr = config_manager_.GetConfigSection(section); if(!section_ptr) return def;
	CConfig* variable_ptr = section_ptr->GetConfig(String.FromHex(window_id,8)); if(!variable_ptr) return def;
	S16 x, y, w, h; BIT m; variable_ptr->GetWinPos(x, y, w, h, m);
	if(x == ((S16)0x8000)) return def;
	return x;
}
S16 KConfig::GetWPy(TXT section, U32 window_id, S16 def)
{
	if(section == "") section = nullsection_name;
	CConfigSection* section_ptr = config_manager_.GetConfigSection(section); if(!section_ptr) return def;
	CConfig* variable_ptr = section_ptr->GetConfig(String.FromHex(window_id,8)); if(!variable_ptr) return def;
	S16 x, y, w, h; BIT m; variable_ptr->GetWinPos(x, y, w, h, m);
	if(y == ((S16)0x8000)) return def;
	return y;
}
S16 KConfig::GetWPw(TXT section, U32 window_id, S16 def)
{
	if(section == "") section = nullsection_name;
	CConfigSection* section_ptr = config_manager_.GetConfigSection(section); if(!section_ptr) return def;
	CConfig* variable_ptr = section_ptr->GetConfig(String.FromHex(window_id,8)); if(!variable_ptr) return def;
	S16 x, y, w, h; BIT m; variable_ptr->GetWinPos(x, y, w, h, m);
	if(w == ((S16)0x8000)) return def;
	return w;
}
S16 KConfig::GetWPh(TXT section, U32 window_id, S16 def)
{
	if(section == "") section = nullsection_name;
	CConfigSection* section_ptr = config_manager_.GetConfigSection(section); if(!section_ptr) return def;
	CConfig* variable_ptr = section_ptr->GetConfig(String.FromHex(window_id,8)); if(!variable_ptr) return def;
	S16 x, y, w, h; BIT m; variable_ptr->GetWinPos(x, y, w, h, m);
	if(h == ((S16)0x8000)) return def;
	return h;
}
S16 KConfig::GetWPm(TXT section, U32 window_id, BIT def)
{
	if(section == "") section = nullsection_name;
	CConfigSection* section_ptr = config_manager_.GetConfigSection(section); if(!section_ptr) return def;
	CConfig* variable_ptr = section_ptr->GetConfig(String.FromHex(window_id,8)); if(!variable_ptr) return def;
	S16 x, y, w, h; BIT m = false; variable_ptr->GetWinPos(x, y, w, h, m);
	return m;
}
INT KConfig::GetList(TXT section, CConfigList& list)
{
	if(section == "") section = nullsection_name;
	CConfig** pointer = NULL;
	CConfigSection* section_ptr = config_manager_.GetConfigSection(section);
	if(!section_ptr){
		return 0;
	}
	INT values = section_ptr->GetVectorPointer(pointer);
	list.pointer = pointer;
	return values;
}
void KConfig::Move(TXT src_section, TXT dst_section)
{
	if(src_section == "") src_section = nullsection_name;
	if(dst_section == "") dst_section = nullsection_name;
	CConfigSection* ss = config_manager_.GetConfigSection(src_section);
	if(!ss) return; // Nothing to move!
	CConfigSection* ds = config_manager_.AddConfigSection(dst_section);
	ss->MoveTo(ds);
}
void KConfig::Copy(TXT src_section, TXT dst_section)
{
	if(src_section == "") src_section = nullsection_name;
	if(dst_section == "") dst_section = nullsection_name;
	CConfigSection* ss = config_manager_.GetConfigSection(src_section);
	if(!ss) return; // Nothing to move!
	CConfigSection* ds = config_manager_.AddConfigSection(dst_section);
	ss->CopyTo(ds);
}
void KConfig::Delete(TXT section)
{
	if(section == "") section = nullsection_name;
	config_manager_.DelConfigSection(section);
}
S32 KConfig::GetSetInt(TXT section, TXT variable, S32 value)
{
    return SetInt(section, variable, GetInt(section, variable, value));
}
U32 KConfig::GetSetHex(TXT section, TXT variable, U32 value)
{
    return SetHex(section, variable, GetHex(section, variable, value));
}
U64 KConfig::GetSetHex64(TXT section, TXT variable, U64 value)
{
    return SetHex64(section, variable, GetHex64(section, variable, value));
}
S32 KConfig::GetSetVsx(TXT section, TXT variable, S32 value)
{
    return SetVsx(section, variable, GetVsx(section, variable, value));
}
S64 KConfig::GetSetVsx64(TXT section, TXT variable, S64 value)
{
    return SetVsx64(section, variable, GetVsx64(section, variable, value));
}
TXT KConfig::GetSetStr(TXT section, TXT variable, TXT value)
{
    return SetStr(section, variable, GetStr(section, variable, value));
}
