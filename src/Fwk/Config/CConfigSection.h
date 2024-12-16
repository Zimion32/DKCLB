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
class CFileIO;
class CConfig;
class CConfigManager;
class CConfigSection
{
	friend class CConfigManager;
	public:
		U32		GetSecondaryID();
		void	CloseAll();
		void	Close(U32 secondary_id);
		void	WriteDataTo(CFileIO& ini_bfa);
		INT		GetVectorPointer(CConfig**& pointer);
		void	CopyTo(CConfigSection* other);
		void	MoveTo(CConfigSection* other);
       CConfig*	AddConfig(TXT variable_name, U32 secondary_id=0xFFFFFFFF);
       CConfig*	GetConfig(TXT variable_name, U32 secondary_id=0xFFFFFFFF);
		void	DelConfig(TXT variable_name, U32 secondary_id=0xFFFFFFFF);
				~CConfigSection(); // Only friends can delete.
		
	protected:
		U08&	operator[](INT i);
		U32		GetDataSz() const;
				CConfigSection(TXT section_name, U32 secondary_id);
				
	private:
 static U08		static_zero_;
		TXT		section_name_;
   Vx<CConfig* > configs_;
		Vx<U32> ordered_indexes_;
   Vx<CConfig* > ordered_configs_;
		U32		secondary_id_;
		BIT		forced_sort_;
		U32		Find_(CConfig* cfg);
		U32		FindInsertionPoint_(CConfig* cfg, INT first, INT last, CConfig*& duplicate);
		INT		FindFirst_(U08* key, INT i, INT first, INT last);
		INT		FindLast_(U08* key, INT i, INT first, INT last);
		INT		CompareToForInsert_(U08* key, U08* v_item, INT start, INT keylen, INT v_itemlen);
		void	CheckSecondaryId_(U32 secondary_id);
				CConfigSection(); // Only friends can create.
				CConfigSection(const CConfigSection&); // No copy-construction
				CConfigSection& operator=(const CConfigSection&); // No assignment
};


