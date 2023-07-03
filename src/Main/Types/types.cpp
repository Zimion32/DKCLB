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
#include <limits>
#include "include.h"
char* checked_string_zero = "\0\0\0\0\0\0\0\0";
volatile U32  __attribute__((used)) ___global_throwcheck = 0;
static void __attribute__((used)) __BUILDTIME_TYPE_CHECKS__()
{ // IF A LINE IN THIS SECTION FAILS, THAT TYPE HAS AN INCORRECT TYPE SIZE!!
	BUILD_BUG_ON((((signed int)-1)>>1) != ((signed int)-1)); // Must have Arithmetic Signed Right Shifts.
	BUILD_BUG_ON(sizeof(PTR) != 4); // 32-bit pointers only
	BUILD_BUG_ON(sizeof(U08) != 1); // U08 must represent an unsigned  8 bit INTeger.
	BUILD_BUG_ON(sizeof(U16) != 2); // U16 must represent an unsigned 16 bit INTeger.
	BUILD_BUG_ON(sizeof(U32) != 4); // U32 must represent an unsigned 32 bit INTeger.
	BUILD_BUG_ON(sizeof(U64) != 8); // U64 must represent an unsigned 64 bit INTeger.
	BUILD_BUG_ON(sizeof(S08) != 1); // S08 must represent a signed  8 bit INTeger.
	BUILD_BUG_ON(sizeof(S16) != 2); // S16 must represent a signed 16 bit INTeger.
	BUILD_BUG_ON(sizeof(S32) != 4); // S32 must represent a signed 32 bit INTeger.
	BUILD_BUG_ON(sizeof(S64) != 8); // S64 must represent a signed 64 bit INTeger.
	BUILD_BUG_ON(sizeof(B16) != 16); // B16 must represent 128 bit cryptographic value.
	BUILD_BUG_ON(sizeof(B32) != 32); // B32 must represent 256 bit cryptographic value.
	BUILD_BUG_ON(sizeof(F32) != 4);
	BUILD_BUG_ON(sizeof(F64) != 8);
	BUILD_BUG_ON(sizeof(INT) < 4);  //at least 32 bits INT size
	BUILD_BUG_ON(sizeof(LUCID) != 24);//LUCID must be 24 bytes!!
	BUILD_BUG_ON(sizeof(EXTHD) != 32);//EXTHD must be 32 bytes!!
}
void __attribute__((used)) ___global_IncrementThrowCheck()
{
	___global_throwcheck++;
}
void __attribute__((used)) ___global_DecrementThrowCheck()
{
	___global_throwcheck--;
}

