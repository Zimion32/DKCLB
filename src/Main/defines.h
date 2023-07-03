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
#define __CCID__ ((CONSTHASH(__FILE__) & 0xFFFFFF80) + __COUNTER__) //This provides a unique ID for the current file line.
#define LINE_STRING_STRINGIZE(x) LINE_STRING_STRINGIZE2(x)
#define LINE_STRING_STRINGIZE2(x) #x
#define __LINESTR__ LINE_STRING_STRINGIZE(__LINE__) //Provides the current line as a C TXT.

//This complicated thing allocates and returns a static variable 
//which is unique to the current invocation of this macro!!!
//This allows you to make functions that save time by remembering
//lookup'd pointers like expensive TXT value lookups.
#define GetStaticPtr() ((void**)(([](U32 a){__asm__ __volatile__(".section .data\n0: .long 0x00000000\n.section .text\nleal 0b,%0":"=r"(a)::"memory"); return a;})(0x1234)))
