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
#include <stdio.h>
#include <stdlib.h>
#include "include.h"
#include <time.h>
TXT dtime::ToString() const
{
    TXT result = String.FromInt(_Y,4) + "/";
    result += String.FromInt(_M+1,2) + "/";
    result += String.FromInt(_D+1,2) + " ";
    result += String.FromInt(_h,2) + ":";
    result += String.FromInt(_m,2) + ":";
    result += String.FromInt(_s,2);
    return result;
}
void dtime::SetToCurrentTime()
{
	time_t raw_time;
	struct tm* t;
	time(&raw_time);
	t = localtime(&raw_time);
	_s = (t->tm_sec % 60);//60 secs. modulus to account for **leap seconds** (see docs). not DELTA supported!
	_m = (t->tm_min);//60 minutes.
	_h = (t->tm_hour);//24 hours.
	_D = (t->tm_mday-1);//31 days. minus one because it counts from 1 like a computer noob.
	_M = (t->tm_mon);//12 months.
	_Y = (t->tm_year + 1900);// tm_year is 0 for 1900.
	RefreshStamp();
}
void dtime::SetTo(const dtime& t)
{
	_s = t._s;
	_m = t._m;
	_h = t._h;
	_D = t._D;
	_M = t._M;
	_Y = t._Y;
	_stamp = t._stamp;
}
void dtime::SetTo(const U64 t)
{
	_stamp = (t & 0xFFFFFFFFFFLL); //14+26 bits = 40 bits
	_s = ((U64(_stamp))                & 0x3F);
	_m = ((U64(_stamp) >> (6))         & 0x3F);
	_h = ((U64(_stamp) >> (6+6))       & 0x1F);
	_D = ((U64(_stamp) >> (6+6+5))     & 0x1F);
	_M = ((U64(_stamp) >> (6+6+5+5))   & 0x0F);
	_Y = ((U64(_stamp) >> (6+6+5+5+4)) & 0x3FFF);
}
void dtime::SetTo(const FILETIME& ft)
{
	SYSTEMTIME st;
	FileTimeToSystemTime(&ft,&st);
	SetTo(st);
}
void dtime::SetTo(const SYSTEMTIME& st)
{
	_s = (st.wSecond % 60);
	_m = (st.wMinute % 60);
	_h = (st.wHour % 24);
	_D = ((st.wDay - 1) % 31);
	_M = ((st.wMonth - 1) % 12);
	_Y = st.wYear;
	RefreshStamp();
}
FILETIME dtime::ToFILETIME() const
{
	FILETIME ft;
	SYSTEMTIME st = ToSYSTEMTIME();
	SystemTimeToFileTime(&st,&ft);
	return ft;
}
SYSTEMTIME dtime::ToSYSTEMTIME() const
{
	SYSTEMTIME st;
	st.wYear = _Y;
	st.wMonth = _M+1;
	st.wDay = _D+1;
	st.wHour = _h;
	st.wMinute = _m;
	st.wSecond = _s;
	st.wMilliseconds = 0;
	return st;
}
void dtime::RefreshStamp()
{
	/// 16(months) 32(days) 32(hours) 64(minutes) 64(secs)  
	///      4        5         5          6         6       = 26 bits
	_stamp = 0LL;
	_stamp |= (U64(_s));
	_stamp |= (U64(_m) << (6));
	_stamp |= (U64(_h) << (6+6));
	_stamp |= (U64(_D) << (6+6+5));
	_stamp |= (U64(_M) << (6+6+5+5));
	_stamp |= (U64(_Y) << (6+6+5+5+4));
	_stamp &= 0xFFFFFFFFFFLL; //14 bits for years
}
TXT dtime::ToDataString() const
{
    TXT result = String.FromInt(_Y,4);
    result += String.FromInt(_M+1,2);
    result += String.FromInt(_D+1,2);
    result += String.FromInt(_h,2);
    result += String.FromInt(_m,2);
    result += String.FromInt(_s,2);
    return result;
}
dtime& dtime::SetToData(TXT datastring)
{
    _Y = String.ToInt(datastring.substr(0,4));
    _M = String.ToInt(datastring.substr(4,2))-1;
    _D = String.ToInt(datastring.substr(6,2))-1;
    _h = String.ToInt(datastring.substr(8,2));
    _m = String.ToInt(datastring.substr(10,2));
    _s = String.ToInt(datastring.substr(12,2));
	RefreshStamp();
	return *this;
}
