#pragma once
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
class dtime
{
	public:
		U32 _s, _m, _h, _D, _M, _Y;
		U64 _stamp;
		operator TXT() const        {return ToString();};
		operator U64() const		{return _stamp;}
		operator FILETIME() const	{return ToFILETIME();}
		operator SYSTEMTIME() const	{return ToSYSTEMTIME();}
		dtime& operator=(const dtime& t)		{SetTo(t); return *this;}
		dtime& operator=(const U64& t)		    {SetTo(t); return *this;}
		dtime& operator=(const FILETIME& t)		{SetTo(t); return *this;}
		dtime& operator=(const SYSTEMTIME& t)	{SetTo(t); return *this;}
		dtime(){}
		dtime(U64 t){SetTo(t);}
		dtime(FILETIME& t){SetTo(t);}
		dtime(SYSTEMTIME& t){SetTo(t);}
		void SetToCurrentTime();
		void SetTo(const dtime& t);
		void SetTo(const U64 t);
		void SetTo(const FILETIME& ft);
		void SetTo(const SYSTEMTIME& st);
		FILETIME ToFILETIME() const;
		SYSTEMTIME ToSYSTEMTIME() const;
		TXT ToString() const;
		void RefreshStamp();
};
