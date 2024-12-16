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
#include "CConfigSection.h"
class CConfig
{
	friend class CConfigSection;
	public:
		U32		GetSecondaryID();
		void	Load(TXT str);
		void	Set(TXT str);
		void	Set(S32 value);
		void	Set(U32 value,INT index);
		void	Set(S32 value,INT index);
		void	Set(U64 value,INT index);
		void	Set(S64 value,INT index);
		TXT		Name();
		TXT		GetString();
		U32		GetHex(INT index);
		S32		GetVsx(INT index);
		U64		GetHex64(INT index);
		S64		GetVsx64(INT index);
		S32		GetInt();
		void	SetWinPos(S16 x, S16 y, S16 w, S16 h, BIT m);
		void	GetWinPos(S16& x, S16& y, S16& w, S16& h, BIT& m);
		void	GetWinPosNoOverwrite(S16& x, S16& y, S16& w, S16& h, BIT& m);
		void	WriteDataTo(CFileIO& ini_bfa);
                ~CConfig(){};
		
	protected:
		U08&	operator[](INT i);
		U32		GetDataSz() const;
                CConfig(TXT variable_name, U32 secondary_id);
		
	private:
 static U08		static_zero_;
		TXT		variable_name_;
 Vx<U32>		hexadecimal_data_;
 Vx<U64>		hexadecimal_data_64_;
		S32		integer_data_;
		TXT		string_data_;
		BIT		is_integer_data_;
		BIT		is_hexadecimal_data_;
		BIT		is_hexadecimal_data_64_;
		BIT		is_string_data_;
		BIT		is_window_position_data_;
		BIT		is_empty_, is_unedited_;
		U32		secondary_id_;
                CConfig(); // Only friends can create.
                CConfig(const CConfig&); // No copy-construction
                CConfig& operator=(const CConfig&); // No assignment
};
class CConfigList
{
	public:
CConfig**       pointer;
                CConfigList(): pointer(NULL) {}
		TXT     operator[](INT i);
};

