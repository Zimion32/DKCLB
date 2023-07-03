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

static U32 GetLucidUserID()
{
	TXT path = "SOFTWARE\\SubwareSoft\\DELTA\\LUCID Generator Data\\";
	if(!Registry.KeyExists(HKEY_CURRENT_USER, path)){
        Registry.CreateTree(HKEY_CURRENT_USER, path);
	}
	U32 r = Registry.GetHex(HKEY_CURRENT_USER, path, "UserID", 0xFFFFFFFF);
	if(r == 0xFFFFFFFF){
        r = (Utils.SuperRandom() & 0xFFFF);
        Registry.SetHex(HKEY_CURRENT_USER, path, "UserID", r);
	}
	return (r & 0xFFFF);
}
static U08 GetLucidCreateCount()
{
	TXT path = "SOFTWARE\\SubwareSoft\\DELTA\\LUCID Generator Data\\";
	if(!Registry.KeyExists(HKEY_CURRENT_USER, path)){
        Registry.CreateTree(HKEY_CURRENT_USER, path);
	}
	U32 r = Registry.GetHex(HKEY_CURRENT_USER, path, "CreationCount", 0xFFFFFFFF);
	r = ((r + 1) & 0xFF);
	Registry.SetHex(HKEY_CURRENT_USER, path, "CreationCount", r);
	return r;
}
LUCID& LUCID::Calculate()
{
	dtime dt;
	dt.SetToCurrentTime();
	U64 stamp = dt;
	U32 userid = GetLucidUserID();
	pData[0] = U08((stamp >> 32) & 0xFF);
	pData[1] = U08((stamp >> 24) & 0xFF);
	pData[2] = U08((stamp >> 16) & 0xFF);
	pData[3] = U08((stamp >>  8) & 0xFF);
	pData[4] = U08((stamp >>  0) & 0xFF);
	pData[5] = GetLucidCreateCount();
	pData[6] = U08((userid >> 8) & 0xFF);
	pData[7] = U08((userid >> 0) & 0xFF);
	*((B16*)(&pData[8])) = System.GetSystemIdentityLUCID();
	return *this;
}
U64 LUCID::GetTimeStamp()
{
    U64 result = 0LL;
    result |= (((U64)pData[0]) << 32);
    result |= (((U64)pData[1]) << 24);
    result |= (((U64)pData[2]) << 16);
    result |= (((U64)pData[3]) <<  8);
    result |= (((U64)pData[4]) <<  0);
    return result;
}
TXT LUCID::ToString()
{
    return Utils.ConvertToUrlBase64(&pData[0], 24);
}
TXT LUCID::ToHexString()
{
    TXT result;
    for(IND i = 0; i < 24; i++){result += String.FromHex(pData[i],2);} 
    return result;
}
LUCID::LUCID(TXT urlbase64)
{
    Vx<U08> x = Utils.ConvertFromUrlBase64(urlbase64);
    for(IND i = 0; i < 24; i++){pData[i] = x[i];}
}
