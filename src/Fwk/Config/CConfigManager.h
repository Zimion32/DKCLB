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
#define CCS CConfigSection*
class CFileIO;
class CConfigSection;
class CConfigManager
{
	public:
        void	CloseAll();
        void	Close(U32 secondary_id);
        void	WriteDataTo(CFileIO& ini_bfa);
        CCS     AddConfigSection(TXT section_name, U32 secondary_id=0xFFFFFFFF);
        CCS     GetConfigSection(TXT section_name, U32 secondary_id=0xFFFFFFFF);
        void	DelConfigSection(TXT section_name, U32 secondary_id=0xFFFFFFFF);
        
	private:
        Vx<CCS> config_sections_;//sorted by value list, *actual* allocation pointers
        Vx<U32>	ordered_indexes_;//index lookup to find element in the ini-ordered list
        Vx<CCS> ordered_config_sections_;//ini-ordered list, pointer copy(s)
        U32		Find_(CConfigSection* cfgs);
        U32		FindInsertionPoint_(CConfigSection* cfgs, INT first, INT last, CConfigSection*& duplicate);
        INT		FindFirst_(U08* key, INT i, INT first, INT last);
        INT		FindLast_(U08* key, INT i, INT first, INT last);
        INT		CompareToForInsert_(U08* key, U08* v_item, INT start, INT keylen, INT v_itemlen);
};
#undef CCS
