#include "include.h"
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
U64 KUtils::SuperRandom()
{
	U64 result; // leave this un-initialized on purpose
	U64 tt;
         tt = (U64(System.GetPerformanceCounter())); 
	result ^= (U64(MurmurHash2A((void*)(&tt), 8, 0xCAFEBABE)));
	     tt = (U64(System.GetPerformanceCounter())); 
	result ^= (U64(MurmurHash2A((void*)(&tt), 8, 0xBADDBEEF)) << 32);
	result ^= (U64(rand()));
	result ^= (U64(rand()) << 15);
	result ^= (U64(rand()) << 30);
	result ^= (U64(rand()) << 45);
	result ^= (U64(rand() & 0x0F) << 60);
	result ^= 0xBADDBEEFCAFEBABELL;
	return result;
}
TXT KUtils::ConvertToUrlBase64(U08* data, U32 size)
{
    static TXT base64 = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz-_";
    TXT result;
    U32 buffer = 0;
    U32 cnt = 0;
    for(IND i = 0; i < size; i++){
        buffer <<= 8;
        buffer |= data[i];
        cnt += 8;
        while(cnt >= 6){
            result += base64[(buffer >> (cnt - 6)) & 0x3F];
            cnt -= 6;
        }
    }
    if(cnt > 0){
        result += base64[(buffer << (6 - cnt)) & 0x3F];
    }
    return result;
}
Vx<U08> KUtils::ConvertFromUrlBase64(TXT str)
{
    Vx<U08> result;
    U32 buffer = 0;
    U32 cnt = 0;
    for(IND i = 0; i < str.size(); i++){
        buffer <<= 6;
        if((str[i] >= '0') && (str[i] <= '9')) buffer |= (str[i] - '0');
        if((str[i] >= 'A') && (str[i] <= 'Z')) buffer |= ((str[i] - 'A') + 10);
        if((str[i] >= 'a') && (str[i] <= 'z')) buffer |= ((str[i] - 'a') + 10 + 26);
        if(str[i] == '-') buffer |= 62;
        if(str[i] == '_') buffer |= 63;
        cnt += 6;
        if(cnt >= 8){
            result.push_back((buffer >> (cnt - 8)) & 0xFF);
            cnt -= 8;
        }
    }
    return result;
}
