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
#include "types.h"
#define alwaysinline __attribute__((always_inline))
#define GET_DW3(x) (*(((U32*)(&(x)))+0))
#define GET_DW2(x) (*(((U32*)(&(x)))+1))
#define GET_DW1(x) (*(((U32*)(&(x)))+2))
#define GET_DW0(x) (*(((U32*)(&(x)))+3))
class Hex128;
class Vsx128
{public:
	U32 dw3, dw2, dw1, dw0;
	Vsx128() alwaysinline:
		dw3(0), dw2(0), dw1(0), dw0(0) {};
	/////////////////////////////////////////////////
	Vsx128(const Hex128& h) alwaysinline:
		dw3(GET_DW3(h)), dw2(GET_DW2(h)), dw1(GET_DW1(h)), dw0(GET_DW0(h)) {};
	Vsx128(const U64& qw0) alwaysinline:
		dw3(0), dw2(0), dw1(qw0 >> 32), dw0(qw0 & 0xFFFFFFFF) {};
	Vsx128(const U64& qw1, const U64& qw0) alwaysinline:
		dw3(qw1 >> 32), dw2(qw1 & 0xFFFFFFFF), dw1(qw0 >> 32), dw0(qw0 & 0xFFFFFFFF) {};
	Vsx128(const U32& dw0) alwaysinline:
		dw3(0), dw2(0), dw1(0), dw0(dw0) {};
	Vsx128(const U32& dw1, const U32& dw0) alwaysinline:
		dw3(0), dw2(0), dw1(dw1), dw0(dw0) {};
	Vsx128(const U32& dw2, const U32& dw1, const U32& dw0) alwaysinline:
		dw3(0), dw2(dw2), dw1(dw1), dw0(dw0) {};
	Vsx128(const U32& dw3, const U32& dw2, const U32& dw1, const U32& dw0) alwaysinline:
		dw3(dw3), dw2(dw2), dw1(dw1), dw0(dw0) {};
	/////////////////////////////////////////////////
	Vsx128(const Vsx128& h) alwaysinline:
		dw3(h.dw3), dw2(h.dw2), dw1(h.dw1), dw0(h.dw0) {};
	Vsx128(const S64& qw0) alwaysinline:
		dw3(qw0 >> 63), dw2(qw0 >> 63), dw1(qw0 >> 32), dw0(qw0 & 0xFFFFFFFF) {};
	Vsx128(const S32& _dw0) alwaysinline:
		dw3(_dw0 >> 31), dw2(_dw0 >> 31), dw1(_dw0 >> 31), dw0(_dw0) {};

	operator U32() const	 alwaysinline{return dw0;}
	operator S32() const	 alwaysinline
	{
		if(*this >= U32(0x80000000)) return S32(0x7FFFFFFF);
		if(*this <= S32(0x80000000)) return S32(0x80000000);
		return ((dw0 & 0x7FFFFFFF) | (dw3 & 0x80000000));
	}
	operator U64() const	 alwaysinline{return U64(U64(dw0) | (U64(dw1) << 32));}
	operator S64() const	 alwaysinline
	{
		if(*this >= U64(0x8000000000000000LL)) return S64(0x7FFFFFFFFFFFFFFFLL);
		if(*this <= S64(0x8000000000000000LL)) return S64(0x8000000000000000LL);
		return S64(U64(dw0) | (U64(dw1 & 0x7FFFFFFF) << 32) | (U64(dw3 & 0x80000000) << 32));
	}
	
	//##########################################################################
	// Assignment Operators
	Vsx128& operator=(const Hex128&  rhs) alwaysinline{
		dw3 = GET_DW3(rhs); dw2 = GET_DW2(rhs); dw1 = GET_DW1(rhs); dw0 = GET_DW0(rhs); return *this;}
	Vsx128& operator=(const U64&  rhs) alwaysinline{
		dw3 = dw2 = 0; dw1 = (rhs >> 32); dw0 = (rhs & 0xFFFFFFFF); return *this;}
	Vsx128& operator=(const U32&  rhs) alwaysinline{
		dw3 = dw2 = dw1 = 0; dw0 = rhs; return *this;}
	Vsx128& operator=(const Vsx128&  rhs) alwaysinline{
		dw3 = rhs.dw3; dw2 = rhs.dw2; dw1 = rhs.dw1; dw0 = rhs.dw0; return *this;}
	Vsx128& operator=(const S64&  rhs) alwaysinline{
		dw3 = dw2 = (rhs >> 63); dw1 = (rhs >> 32); dw0 = (rhs & 0xFFFFFFFF); return *this;}
	Vsx128& operator=(const S32&  rhs) alwaysinline{
		dw3 = dw2 = dw1 = (rhs >> 31); dw0 = rhs; return *this;}

	//##########################################################################
	// Basic Arithmetic Operators
	Vsx128& operator+=(const Vsx128&  rhs) alwaysinline
	{
		asm volatile("addl 12(%%esi),%%eax \n adcl 8(%%esi),%%ebx \n adcl 4(%%esi),%%ecx \n adcl (%%esi),%%edx\n"
		:"=d"(dw3),"=c"(dw2),"=b"(dw1),"=a"(dw0):"d"(dw3),"c"(dw2),"b"(dw1),"a"(dw0),"S"(&rhs):"memory");
		return *this;
	}
	Vsx128& operator+=(const U64&  rhs) alwaysinline
	{
		asm volatile("addl %%esi,%%eax \n adcl %%edi,%%ebx \n adcl $0x00,%%ecx \n adcl $0x00,%%edx\n"
		:"=d"(dw3),"=c"(dw2),"=b"(dw1),"=a"(dw0):"d"(dw3),"c"(dw2),"b"(dw1),"a"(dw0),"S"(U32(rhs & 0xFFFFFFFF)),"D"(U32(rhs >> 32)):);
		return *this;
	}
	Vsx128& operator+=(const U32&  rhs) alwaysinline
	{
		asm volatile("addl %%esi,%%eax \n adcl $0x00,%%ebx \n adcl $0x00,%%ecx \n adcl $0x00,%%edx\n"
		:"=d"(dw3),"=c"(dw2),"=b"(dw1),"=a"(dw0):"d"(dw3),"c"(dw2),"b"(dw1),"a"(dw0),"S"(rhs):);
		return *this;
	}
	Vsx128& operator+=(const S64&  rhs) alwaysinline
	{
		asm volatile("addl %%esi,%%eax \n adcl %%edi,%%ebx \n\
					  sarl $0x1F,%%edi #MUST Shift to get sign extension! \n\
					  adcl %%edi,%%ecx \n adcl %%edi,%%edx \n"
		:"=d"(dw3),"=c"(dw2),"=b"(dw1),"=a"(dw0):"d"(dw3),"c"(dw2),"b"(dw1),"a"(dw0),"S"(U32(rhs & 0xFFFFFFFF)),"D"(U32(rhs >> 32)):);
		return *this;
	}
	Vsx128& operator+=(const S32&  rhs) alwaysinline
	{
		asm volatile("addl %%esi,%%eax \n\
					  sarl $0x1F,%%esi #MUST Shift to get sign extension! \n\
					  adcl %%esi,%%ebx \n adcl %%esi,%%ecx \n adcl %%esi,%%edx\n"
		:"=d"(dw3),"=c"(dw2),"=b"(dw1),"=a"(dw0):"d"(dw3),"c"(dw2),"b"(dw1),"a"(dw0),"S"(rhs):);
		return *this;
	}
	Vsx128& operator-=(const Vsx128&  rhs) alwaysinline
	{
		asm volatile("subl 12(%%esi),%%eax \n sbbl 8(%%esi),%%ebx \n sbbl 4(%%esi),%%ecx \n sbbl (%%esi),%%edx\n"
		:"=d"(dw3),"=c"(dw2),"=b"(dw1),"=a"(dw0):"d"(dw3),"c"(dw2),"b"(dw1),"a"(dw0),"S"(&rhs):"memory");
		return *this;
	}
	Vsx128& operator-=(const U64&  rhs) alwaysinline
	{
		asm volatile("subl %%esi,%%eax \n sbbl %%edi,%%ebx \n sbbl $0x00,%%ecx \n sbbl $0x00,%%edx\n"
		:"=d"(dw3),"=c"(dw2),"=b"(dw1),"=a"(dw0):"d"(dw3),"c"(dw2),"b"(dw1),"a"(dw0),"S"(U32(rhs & 0xFFFFFFFF)),"D"(U32(rhs >> 32)):);
		return *this;
	}
	Vsx128& operator-=(const U32&  rhs) alwaysinline
	{
		asm volatile("subl %%esi,%%eax \n sbbl $0x00,%%ebx \n sbbl $0x00,%%ecx \n sbbl $0x00,%%edx\n"
		:"=d"(dw3),"=c"(dw2),"=b"(dw1),"=a"(dw0):"d"(dw3),"c"(dw2),"b"(dw1),"a"(dw0),"S"(rhs):);
		return *this;
	}
	Vsx128& operator-=(const S64&  rhs) alwaysinline
	{
		asm volatile("subl %%esi,%%eax \n sbbl %%edi,%%ebx \n\
					  sarl $0x1F,%%edi #MUST Shift to get sign extension! \n\
					  sbbl %%edi,%%ecx \n sbbl %%edi,%%edx\n"
		:"=d"(dw3),"=c"(dw2),"=b"(dw1),"=a"(dw0):"d"(dw3),"c"(dw2),"b"(dw1),"a"(dw0),"S"(U32(rhs & 0xFFFFFFFF)),"D"(U32(rhs >> 32)):);
		return *this;
	}
	Vsx128& operator-=(const S32&  rhs) alwaysinline
	{
		asm volatile("subl %%esi,%%eax \n\
					  sarl $0x1F,%%esi #MUST Shift to get sign extension! \n\
					  sbbl %%esi,%%ebx \n sbbl %%esi,%%ecx \n sbbl %%esi,%%edx\n"
		:"=d"(dw3),"=c"(dw2),"=b"(dw1),"=a"(dw0):"d"(dw3),"c"(dw2),"b"(dw1),"a"(dw0),"S"(rhs):);
		return *this;
	}
	Vsx128 mul32x32(const Vsx128&  rhs) const alwaysinline
	{
		Vsx128 result(*this);
		Vsx128 Urhs(rhs);
		U32 lhs_xor_mask = (S32(result.dw3) >> 31);
		U32 rhs_xor_mask = (S32(Urhs.dw3) >> 31);
		U32 result_xor_mask = (lhs_xor_mask ^ rhs_xor_mask);
		result.dw0 ^= lhs_xor_mask;
		result.dw1 ^= lhs_xor_mask;
		result.dw2 ^= lhs_xor_mask;
		result.dw3 ^= lhs_xor_mask;
		Urhs.dw0 ^= rhs_xor_mask;
		Urhs.dw1 ^= rhs_xor_mask;
		Urhs.dw2 ^= rhs_xor_mask;
		Urhs.dw3 ^= rhs_xor_mask;
		result += (lhs_xor_mask & 1);
		Urhs += (rhs_xor_mask & 1);
		asm volatile("mull %%edx":"=d"(result.dw1),"=a"(result.dw0):"d"(Urhs.dw0),"a"(result.dw0):);
		result.dw0 ^= result_xor_mask;
		result.dw1 ^= result_xor_mask;
		result.dw2 ^= result_xor_mask;
		result.dw3 ^= result_xor_mask;
		return (result += (result_xor_mask & 1));
	}
	Vsx128& operator*=(const Vsx128&  rhs) alwaysinline //__attribute__((optimize(0)))
	{
		Vsx128 Urhs(rhs);
		U32 lhs_xor_mask = (S32(dw3) >> 31);
		U32 rhs_xor_mask = (S32(Urhs.dw3) >> 31);
		U32 result_xor_mask = (lhs_xor_mask ^ rhs_xor_mask);
		dw0 ^= lhs_xor_mask;
		dw1 ^= lhs_xor_mask;
		dw2 ^= lhs_xor_mask;
		dw3 ^= lhs_xor_mask;
		Urhs.dw0 ^= rhs_xor_mask;
		Urhs.dw1 ^= rhs_xor_mask;
		Urhs.dw2 ^= rhs_xor_mask;
		Urhs.dw3 ^= rhs_xor_mask;
		*this += (lhs_xor_mask & 1);
		Urhs += (rhs_xor_mask & 1);
		struct mul128_t
		{
			Vsx128 dqw1, dqw0;
			mul128_t(const Vsx128& dqw1, const Vsx128& dqw0): dqw1(dqw1), dqw0(dqw0){}
		};
		mul128_t data(Urhs,*this);
		asm volatile(
		"push      %%ebp							\n\
		movl       %%eax,   %%ebp					\n\
		movl       $0x00,   %%ebx					\n\
		movl       $0x00,   %%ecx					\n\
		movl       $0x00,   %%esi					\n\
		movl       $0x00,   %%edi					\n\
		movl   28(%%ebp),   %%eax #Calc: (dw0*dw0)	\n\
		mull             12(%%ebp)					\n\
		addl       %%eax,   %%ebx					\n\
		adcl       %%edx,   %%ecx					\n\
		adcl       $0x00,   %%esi					\n\
		adcl       $0x00,   %%edi					\n\
		movl   24(%%ebp),   %%eax #Calc: (dw1*dw0)	\n\
		mull             12(%%ebp)					\n\
		addl       %%eax,   %%ecx					\n\
		adcl       %%edx,   %%esi					\n\
		adcl       $0x00,   %%edi					\n\
		movl   20(%%ebp),   %%eax #Calc: (dw2*dw0)	\n\
		mull             12(%%ebp)					\n\
		addl       %%eax,   %%esi					\n\
		adcl       %%edx,   %%edi					\n\
		movl   16(%%ebp),   %%eax #Calc: (dw3*dw0)	\n\
		mull             12(%%ebp)					\n\
		addl       %%eax,   %%edi					\n\
		movl   28(%%ebp),   %%eax #Calc: (dw0*dw1)	\n\
		mull              8(%%ebp)					\n\
		addl       %%eax,   %%ecx					\n\
		adcl       %%edx,   %%esi					\n\
		adcl       $0x00,   %%edi					\n\
		movl   24(%%ebp),   %%eax #Calc: (dw1*dw1)	\n\
		mull              8(%%ebp)					\n\
		addl       %%eax,   %%esi					\n\
		adcl       %%edx,   %%edi					\n\
		movl   20(%%ebp),   %%eax #Calc: (dw2*dw1)	\n\
		mull              8(%%ebp)					\n\
		addl       %%eax,   %%edi					\n\
		movl   28(%%ebp),   %%eax #Calc: (dw0*dw2)	\n\
		mull              4(%%ebp)					\n\
		addl       %%eax,   %%esi					\n\
		adcl       %%edx,   %%edi					\n\
		movl   24(%%ebp),  %%eax #Calc: (dw1*dw2)	\n\
		mull              4(%%ebp)					\n\
		addl       %%eax,   %%edi					\n\
		movl   28(%%ebp),   %%eax #Calc: (dw0*dw3)	\n\
		mull               (%%ebp)					\n\
		addl       %%eax,   %%edi					\n\
		pop        %%ebp							\n"
		:"=b"(this->dw0),"=c"(this->dw1),"=S"(this->dw2),"=D"(this->dw3)
		:"a"(&data):);
		dw0 ^= result_xor_mask;
		dw1 ^= result_xor_mask;
		dw2 ^= result_xor_mask;
		dw3 ^= result_xor_mask;
		return (*this += (result_xor_mask & 1));
	}
	Vsx128& operator*=(const S64&  rhs) alwaysinline
	{
		S64 Urhs = rhs;
		U32 lhs_sign = (dw3 & 0x80000000);
		U32 rhs_sign = ((Urhs >> 32) & 0x80000000);
		U32 result_sign = (lhs_sign ^ rhs_sign);
		U32 lhs_xor_mask = (S32(lhs_sign) >> 31);
		S64 rhs_xor_mask = (Urhs >> 63);
		U32 result_xor_mask = (S32(result_sign) >> 31);
		dw0 ^= lhs_xor_mask;
		dw1 ^= lhs_xor_mask;
		dw2 ^= lhs_xor_mask;
		dw3 ^= lhs_xor_mask;
		Urhs ^= rhs_xor_mask;
		*this += (lhs_sign >> 31);
		Urhs += (rhs_sign >> 31);
		struct mul128_t
		{
			Vsx128 dqw1, dqw0;
			mul128_t(const Vsx128& dqw1, const Vsx128& dqw0): dqw1(dqw1), dqw0(dqw0){}
		};
		mul128_t data(Urhs,*this);
		asm volatile(
		"push      %%ebp							\n\
		movl       %%eax,   %%ebp					\n\
		movl       $0x00,   %%ebx					\n\
		movl       $0x00,   %%ecx					\n\
		movl       $0x00,   %%esi					\n\
		movl       $0x00,   %%edi					\n\
		movl   28(%%ebp),   %%eax #Calc: (dw0*dw0)	\n\
		mull             12(%%ebp)					\n\
		addl       %%eax,   %%ebx					\n\
		adcl       %%edx,   %%ecx					\n\
		adcl       $0x00,   %%esi					\n\
		adcl       $0x00,   %%edi					\n\
		movl   24(%%ebp),   %%eax #Calc: (dw1*dw0)	\n\
		mull             12(%%ebp)					\n\
		addl       %%eax,   %%ecx					\n\
		adcl       %%edx,   %%esi					\n\
		adcl       $0x00,   %%edi					\n\
		movl   20(%%ebp),   %%eax #Calc: (dw2*dw0)	\n\
		mull             12(%%ebp)					\n\
		addl       %%eax,   %%esi					\n\
		adcl       %%edx,   %%edi					\n\
		movl   16(%%ebp),   %%eax #Calc: (dw3*dw0)	\n\
		mull             12(%%ebp)					\n\
		addl       %%eax,   %%edi					\n\
		movl   28(%%ebp),   %%eax #Calc: (dw0*dw1)	\n\
		mull              8(%%ebp)					\n\
		addl       %%eax,   %%ecx					\n\
		adcl       %%edx,   %%esi					\n\
		adcl       $0x00,   %%edi					\n\
		movl   24(%%ebp),   %%eax #Calc: (dw1*dw1)	\n\
		mull              8(%%ebp)					\n\
		addl       %%eax,   %%esi					\n\
		adcl       %%edx,   %%edi					\n\
		movl   20(%%ebp),   %%eax #Calc: (dw2*dw1)	\n\
		mull              8(%%ebp)					\n\
		addl       %%eax,   %%edi					\n\
		pop        %%ebp							\n"
		:"=b"(this->dw0),"=c"(this->dw1),"=S"(this->dw2),"=D"(this->dw3)
		:"a"(&data):);
		dw0 ^= result_xor_mask;
		dw1 ^= result_xor_mask;
		dw2 ^= result_xor_mask;
		dw3 ^= result_xor_mask;
		return (*this += (result_sign >> 31));
	}
	Vsx128& operator*=(const S32&  rhs) alwaysinline
	{
		S32 Urhs = rhs;
		U32 lhs_sign = (dw3 & 0x80000000);
		U32 rhs_sign = (Urhs & 0x80000000);
		U32 result_sign = (lhs_sign ^ rhs_sign);
		U32 lhs_xor_mask = (S32(lhs_sign) >> 31);
		S32 rhs_xor_mask = (Urhs >> 31);
		U32 result_xor_mask = (S32(result_sign) >> 31);
		dw0 ^= lhs_xor_mask;
		dw1 ^= lhs_xor_mask;
		dw2 ^= lhs_xor_mask;
		dw3 ^= lhs_xor_mask;
		Urhs ^= rhs_xor_mask;
		*this += (lhs_sign >> 31);
		Urhs += (rhs_sign >> 31);
		struct mul128_t
		{
			Vsx128 dqw1, dqw0;
			mul128_t(const Vsx128& dqw1, const Vsx128& dqw0): dqw1(dqw1), dqw0(dqw0){}
		};
		mul128_t data(Urhs,*this);
		asm volatile(
		"push      %%ebp							\n\
		movl       %%eax,   %%ebp					\n\
		movl       $0x00,   %%ebx					\n\
		movl       $0x00,   %%ecx					\n\
		movl       $0x00,   %%esi					\n\
		movl       $0x00,   %%edi					\n\
		movl   28(%%ebp),   %%eax #Calc: (dw0*dw0)	\n\
		mull             12(%%ebp)					\n\
		addl       %%eax,   %%ebx					\n\
		adcl       %%edx,   %%ecx					\n\
		adcl       $0x00,   %%esi					\n\
		adcl       $0x00,   %%edi					\n\
		movl   24(%%ebp),   %%eax #Calc: (dw1*dw0)	\n\
		mull             12(%%ebp)					\n\
		addl       %%eax,   %%ecx					\n\
		adcl       %%edx,   %%esi					\n\
		adcl       $0x00,   %%edi					\n\
		movl   20(%%ebp),   %%eax #Calc: (dw2*dw0)	\n\
		mull             12(%%ebp)					\n\
		addl       %%eax,   %%esi					\n\
		adcl       %%edx,   %%edi					\n\
		movl   16(%%ebp),   %%eax #Calc: (dw3*dw0)	\n\
		mull             12(%%ebp)					\n\
		addl       %%eax,   %%edi					\n\
		pop        %%ebp							\n"
		:"=b"(this->dw0),"=c"(this->dw1),"=S"(this->dw2),"=D"(this->dw3)
		:"a"(&data):);
		dw0 ^= result_xor_mask;
		dw1 ^= result_xor_mask;
		dw2 ^= result_xor_mask;
		dw3 ^= result_xor_mask;
		return (*this += (result_sign >> 31));
	}
	Vsx128& DivideBy128(const Vsx128& div, BIT do_modulus) alwaysinline
	{
		#ifdef DEBUG_WTFs
			//DEBUG_VALUE0 = *this;
			//DEBUG_VALUE1 = div;
		#endif
		struct div128_t
		{
			Vsx128 value;
			Vsx128 dummy;
			div128_t(const Vsx128& value): value(value), dummy(0){}
		};
		div128_t divisor(div);
		U32& ddw0 = divisor.value.dw0;
		U32& ddw1 = divisor.value.dw1;
		U32& ddw2 = divisor.value.dw2;
		U32& ddw3 = divisor.value.dw3;
		
		INT num_bit_count = bsr();
		INT den_bit_count = divisor.value.bsr();
		INT shift = (num_bit_count - den_bit_count);
		if(shift < 0)
		{ // Set self to 0 and return - nothing fits!
		  // BUGFIX: But keep number there if we want the modulus.
			if(!do_modulus){
				dw0 = dw1 = dw2 = dw3 = 0;
			}
			return *this;
		}
		divisor.value <<= shift;
		Vsx128 quotient;
		U32* qdwa = (((U32*)&quotient)+3);
		for(IND i = shift; i >= 0; --i)
		{ // Really crazy "conditional subtract" hack removes the impending if-statement!
			INT ptrplus = ((dw3 > ddw3) || ((dw3 == ddw3) && (dw2 > ddw2))
				|| ((dw3 == ddw3) && (dw2 == ddw2) && (dw1 > ddw1))
				|| ((dw3 == ddw3) && (dw2 == ddw2) && (dw1 == ddw1) && (dw0 >= ddw0))); 
			asm volatile("subl 12(%%esi),%%eax \n sbbl 8(%%esi),%%ebx \n sbbl 4(%%esi),%%ecx \n sbbl (%%esi),%%edx\n"
			:"=d"(dw3),"=c"(dw2),"=b"(dw1),"=a"(dw0):"d"(dw3),"c"(dw2),"b"(dw1),"a"(dw0),"S"(((Vsx128*)(&divisor))+((~ptrplus)&1)):"memory");
			*(qdwa-(i>>5)) |= (ptrplus << (i & 0x1F));
			ddw0 >>= 1;
			asm("rorl $0x01, %%eax": "=a"(ddw1): "a"(ddw1):);
			asm("rorl $0x01, %%eax": "=a"(ddw2): "a"(ddw2):);
			asm("rorl $0x01, %%eax": "=a"(ddw3): "a"(ddw3):);
			ddw0 |= (ddw1 & 0x80000000); ddw1 &= 0x7FFFFFFF;
			ddw1 |= (ddw2 & 0x80000000); ddw2 &= 0x7FFFFFFF;
			ddw2 |= (ddw3 & 0x80000000); ddw3 &= 0x7FFFFFFF;
		}
		#ifdef DEBUG_WTFs
			//DEBUG_VALUE2 = quotient;
			//DEBUG_VALUE3 = *this;
		#endif
		if(do_modulus) return *this; // Dividend becomes the remainder, return self.
		return (*this = quotient); // Assign quotient value, and return self.
	}
	Vsx128& operator/=(const Vsx128&  rhs) alwaysinline
	{
		Vsx128 Urhs(rhs);
		U32 lhs_xor_mask = (S32(dw3) >> 31);
		U32 rhs_xor_mask = (S32(Urhs.dw3) >> 31);
		U32 result_xor_mask = (lhs_xor_mask ^ rhs_xor_mask);
		dw0 ^= lhs_xor_mask;
		dw1 ^= lhs_xor_mask;
		dw2 ^= lhs_xor_mask;
		dw3 ^= lhs_xor_mask;
		Urhs.dw0 ^= rhs_xor_mask;
		Urhs.dw1 ^= rhs_xor_mask;
		Urhs.dw2 ^= rhs_xor_mask;
		Urhs.dw3 ^= rhs_xor_mask;
		*this += (lhs_xor_mask & 1);
		Urhs += (rhs_xor_mask & 1);
		DivideBy128(Urhs,false);
		dw0 ^= result_xor_mask;
		dw1 ^= result_xor_mask;
		dw2 ^= result_xor_mask;
		dw3 ^= result_xor_mask;
		return (*this += (result_xor_mask & 1));
	}
	Vsx128& operator/=(const S64&  rhs) alwaysinline
	{
		S64 Urhs = rhs;
		U32 lhs_sign = (dw3 & 0x80000000);
		U32 rhs_sign = ((Urhs >> 32) & 0x80000000);
		U32 result_sign = (lhs_sign ^ rhs_sign);
		U32 lhs_xor_mask = (S32(lhs_sign) >> 31);
		S64 rhs_xor_mask = (Urhs >> 63);
		U32 result_xor_mask = (S32(result_sign) >> 31);
		dw0 ^= lhs_xor_mask;
		dw1 ^= lhs_xor_mask;
		dw2 ^= lhs_xor_mask;
		dw3 ^= lhs_xor_mask;
		Urhs ^= rhs_xor_mask;
		*this += (lhs_sign >> 31);
		Urhs += (rhs_sign >> 31);
		DivideBy128(Vsx128(Urhs),false);
		dw0 ^= result_xor_mask;
		dw1 ^= result_xor_mask;
		dw2 ^= result_xor_mask;
		dw3 ^= result_xor_mask;
		return (*this += (result_sign >> 31));
	}
	Vsx128& operator/=(const S32&  rhs) alwaysinline
	{
		S32 Urhs = rhs;
		U32 lhs_sign = (dw3 & 0x80000000);
		U32 rhs_sign = (Urhs & 0x80000000);
		U32 result_sign = (lhs_sign ^ rhs_sign);
		U32 lhs_xor_mask = (S32(lhs_sign) >> 31);
		S32 rhs_xor_mask = (Urhs >> 31);
		U32 result_xor_mask = (S32(result_sign) >> 31);
		dw0 ^= lhs_xor_mask;
		dw1 ^= lhs_xor_mask;
		dw2 ^= lhs_xor_mask;
		dw3 ^= lhs_xor_mask;
		Urhs ^= rhs_xor_mask;
		*this += (lhs_sign >> 31);
		Urhs += (rhs_sign >> 31);
		DivideBy128(Vsx128(Urhs),false);
		dw0 ^= result_xor_mask;
		dw1 ^= result_xor_mask;
		dw2 ^= result_xor_mask;
		dw3 ^= result_xor_mask;
		return (*this += (result_sign >> 31));
	}
	Vsx128& operator%=(const Vsx128&  rhs) alwaysinline
	{
		Vsx128 Urhs(rhs);
		U32 lhs_sign = (dw3 & 0x80000000);
		U32 rhs_sign = (Urhs.dw3 & 0x80000000);
		U32 result_sign = (lhs_sign ^ rhs_sign);
		U32 lhs_xor_mask = (S32(lhs_sign) >> 31);
		U32 rhs_xor_mask = (S32(rhs_sign) >> 31);
		U32 result_xor_mask = (S32(result_sign) >> 31);
		dw0 ^= lhs_xor_mask;
		dw1 ^= lhs_xor_mask;
		dw2 ^= lhs_xor_mask;
		dw3 ^= lhs_xor_mask;
		Urhs.dw0 ^= rhs_xor_mask;
		Urhs.dw1 ^= rhs_xor_mask;
		Urhs.dw2 ^= rhs_xor_mask;
		Urhs.dw3 ^= rhs_xor_mask;
		*this += (lhs_sign >> 31);
		Urhs += (rhs_sign >> 31);
		DivideBy128(Urhs,true);
		dw0 ^= result_xor_mask;
		dw1 ^= result_xor_mask;
		dw2 ^= result_xor_mask;
		dw3 ^= result_xor_mask;
		return (*this += (result_sign >> 31));
	}
	Vsx128& operator%=(const S64&  rhs) alwaysinline
	{
		S64 Urhs = rhs;
		U32 lhs_sign = (dw3 & 0x80000000);
		U32 rhs_sign = ((Urhs >> 32) & 0x80000000);
		U32 result_sign = (lhs_sign ^ rhs_sign);
		U32 lhs_xor_mask = (S32(lhs_sign) >> 31);
		S64 rhs_xor_mask = (Urhs >> 63);
		U32 result_xor_mask = (S32(result_sign) >> 31);
		dw0 ^= lhs_xor_mask;
		dw1 ^= lhs_xor_mask;
		dw2 ^= lhs_xor_mask;
		dw3 ^= lhs_xor_mask;
		Urhs ^= rhs_xor_mask;
		*this += (lhs_sign >> 31);
		Urhs += (rhs_sign >> 31);
		DivideBy128(Vsx128(Urhs),true);
		dw0 ^= result_xor_mask;
		dw1 ^= result_xor_mask;
		dw2 ^= result_xor_mask;
		dw3 ^= result_xor_mask;
		return (*this += (result_sign >> 31));
	}
	Vsx128& operator%=(const S32&  rhs) alwaysinline
	{
		S32 Urhs = rhs;
		U32 lhs_sign = (dw3 & 0x80000000);
		U32 rhs_sign = (Urhs & 0x80000000);
		U32 result_sign = (lhs_sign ^ rhs_sign);
		U32 lhs_xor_mask = (S32(lhs_sign) >> 31);
		S32 rhs_xor_mask = (Urhs >> 31);
		U32 result_xor_mask = (S32(result_sign) >> 31);
		dw0 ^= lhs_xor_mask;
		dw1 ^= lhs_xor_mask;
		dw2 ^= lhs_xor_mask;
		dw3 ^= lhs_xor_mask;
		Urhs ^= rhs_xor_mask;
		*this += (lhs_sign >> 31);
		Urhs += (rhs_sign >> 31);
		DivideBy128(Vsx128(Urhs),true);
		dw0 ^= result_xor_mask;
		dw1 ^= result_xor_mask;
		dw2 ^= result_xor_mask;
		dw3 ^= result_xor_mask;
		return (*this += (result_sign >> 31));
	} 
	Vsx128  operator+(const Vsx128&  rhs) const alwaysinline{return Vsx128(*this) += rhs;}
	Vsx128  operator+(const S64&  rhs)  const alwaysinline{return Vsx128(*this) += rhs;}
	Vsx128  operator+(const S32&  rhs)  const alwaysinline{return Vsx128(*this) += rhs;}
	Vsx128  operator-(const Vsx128&  rhs) const alwaysinline{return Vsx128(*this) -= rhs;}
	Vsx128  operator-(const S64&  rhs)  const alwaysinline{return Vsx128(*this) -= rhs;}
	Vsx128  operator-(const S32&  rhs)  const alwaysinline{return Vsx128(*this) -= rhs;}
	Vsx128  operator*(const Vsx128&  rhs) const alwaysinline{return Vsx128(*this) *= rhs;}
	Vsx128  operator*(const S64&  rhs)  const alwaysinline{return Vsx128(*this) *= rhs;}
	Vsx128  operator*(const S32&  rhs)  const alwaysinline{return Vsx128(*this) *= rhs;}
	Vsx128  operator/(const Vsx128&  rhs) const alwaysinline{return Vsx128(*this) /= rhs;}
	Vsx128  operator/(const S64&  rhs)  const alwaysinline{return Vsx128(*this) /= rhs;}
	Vsx128  operator/(const S32&  rhs)  const alwaysinline{return Vsx128(*this) /= rhs;}
	Vsx128  operator%(const Vsx128&  rhs) const alwaysinline{return Vsx128(*this) %= rhs;}
	Vsx128  operator%(const S64&  rhs)  const alwaysinline{return Vsx128(*this) %= rhs;}
	Vsx128  operator%(const S32&  rhs)  const alwaysinline{return Vsx128(*this) %= rhs;}
	Vsx128& operator++() alwaysinline
	{
		asm volatile("addl %%esi,%%eax \n adcl $0x00,%%ebx \n adcl $0x00,%%ecx \n adcl $0x00,%%edx\n"
		:"=d"(dw3),"=c"(dw2),"=b"(dw1),"=a"(dw0):"d"(dw3),"c"(dw2),"b"(dw1),"a"(dw0),"S"(1):"memory");
		return *this;
	}
	Vsx128 operator++(INT) alwaysinline
	{
		Vsx128 ret(*this);
		asm volatile("addl %%esi,%%eax \n adcl $0x00,%%ebx \n adcl $0x00,%%ecx \n adcl $0x00,%%edx\n"
		:"=d"(dw3),"=c"(dw2),"=b"(dw1),"=a"(dw0):"d"(dw3),"c"(dw2),"b"(dw1),"a"(dw0),"S"(1):"memory");
		return ret;
	}
	Vsx128& operator--() alwaysinline
	{
		asm volatile("subl %%esi,%%eax \n sbbl $0x00,%%ebx \n sbbl $0x00,%%ecx \n sbbl $0x00,%%edx\n"
		:"=d"(dw3),"=c"(dw2),"=b"(dw1),"=a"(dw0):"d"(dw3),"c"(dw2),"b"(dw1),"a"(dw0),"S"(1):"memory");
		return *this;
	}
	Vsx128 operator--(INT) alwaysinline
	{
		Vsx128 ret(*this);
		asm volatile("subl %%esi,%%eax \n sbbl $0x00,%%ebx \n sbbl $0x00,%%ecx \n sbbl $0x00,%%edx\n"
		:"=d"(dw3),"=c"(dw2),"=b"(dw1),"=a"(dw0):"d"(dw3),"c"(dw2),"b"(dw1),"a"(dw0),"S"(1):"memory");
		return ret;
	}
	Vsx128 operator-() const alwaysinline{
		return (Vsx128(~dw3, ~dw2, ~dw1, ~dw0) += 1);}
	Vsx128 operator+() const alwaysinline{
		return ((dw3 & 0x80000000)?(Vsx128(~dw3, ~dw2, ~dw1, ~dw0) += 1):(Vsx128(*this)));}
	
	//##########################################################################
	// Bit Manipulation Operators
	Vsx128 operator~() const alwaysinline{
		return Vsx128(~dw3, ~dw2, ~dw1, ~dw0);}
	Vsx128& operator&=(const Hex128&  rhs) alwaysinline{
		dw3 &= GET_DW3(rhs); dw2 &= GET_DW2(rhs); dw1 &= GET_DW1(rhs); dw0 &= GET_DW0(rhs); return *this;}
	Vsx128& operator&=(const U64&  rhs) alwaysinline{
		dw3 &= 0; dw2 &= 0; dw1 &= (rhs >> 32); dw0 &= (rhs & 0xFFFFFFFF); return *this;}
	Vsx128& operator&=(const U32&  rhs) alwaysinline{
		dw3 &= 0; dw2 &= 0; dw1 &= 0; dw0 &= rhs; return *this;}
	Vsx128& operator&=(const Vsx128&  rhs) alwaysinline{
		dw3 &= rhs.dw3; dw2 &= rhs.dw2; dw1 &= rhs.dw1; dw0 &= rhs.dw0; return *this;}
	Vsx128& operator&=(const S64&  rhs) alwaysinline{
		dw3 &= (rhs >> 63); dw2 &= (rhs >> 63); dw1 &= (rhs >> 32); dw0 &= (rhs & 0xFFFFFFFF); return *this;}
	Vsx128& operator&=(const S32&  rhs) alwaysinline{
		dw3 &= (rhs >> 31); dw2 &= (rhs >> 31); dw1 &= (rhs >> 31); dw0 &= rhs; return *this;}
	Vsx128& operator|=(const Hex128&  rhs) alwaysinline{
		dw3 |= GET_DW3(rhs); dw2 |= GET_DW2(rhs); dw1 |= GET_DW1(rhs); dw0 |= GET_DW0(rhs); return *this;}
	Vsx128& operator|=(const U64&  rhs) alwaysinline{
		dw3 |= 0; dw2 |= 0; dw1 |= (rhs >> 32); dw0 |= (rhs & 0xFFFFFFFF); return *this;}
	Vsx128& operator|=(const U32&  rhs) alwaysinline{
		dw3 |= 0; dw2 |= 0; dw1 |= 0; dw0 |= rhs; return *this;}
	Vsx128& operator|=(const Vsx128&  rhs) alwaysinline{
		dw3 |= rhs.dw3; dw2 |= rhs.dw2; dw1 |= rhs.dw1; dw0 |= rhs.dw0; return *this;}
	Vsx128& operator|=(const S64&  rhs) alwaysinline{
		dw3 |= (rhs >> 63); dw2 |= (rhs >> 63); dw1 |= (rhs >> 32); dw0 |= (rhs & 0xFFFFFFFF); return *this;}
	Vsx128& operator|=(const S32&  rhs) alwaysinline{
		dw3 |= (rhs >> 31); dw2 |= (rhs >> 31); dw1 |= (rhs >> 31); dw0 |= rhs; return *this;}
	Vsx128& operator^=(const Hex128&  rhs) alwaysinline{
		dw3 ^= GET_DW3(rhs); dw2 ^= GET_DW2(rhs); dw1 ^= GET_DW1(rhs); dw0 ^= GET_DW0(rhs); return *this;}
	Vsx128& operator^=(const U64&  rhs) alwaysinline{
		dw3 ^= 0; dw2 ^= 0; dw1 ^= (rhs >> 32); dw0 ^= (rhs & 0xFFFFFFFF); return *this;}
	Vsx128& operator^=(const U32&  rhs) alwaysinline{
		dw3 ^= 0; dw2 ^= 0; dw1 ^= 0; dw0 ^= rhs; return *this;}
	Vsx128& operator^=(const Vsx128&  rhs) alwaysinline{
		dw3 ^= rhs.dw3; dw2 ^= rhs.dw2; dw1 ^= rhs.dw1; dw0 ^= rhs.dw0; return *this;}
	Vsx128& operator^=(const S64&  rhs) alwaysinline{
		dw3 ^= (rhs >> 63); dw2 ^= (rhs >> 63); dw1 ^= (rhs >> 32); dw0 ^= (rhs & 0xFFFFFFFF); return *this;}
	Vsx128& operator^=(const S32&  rhs) alwaysinline{
		dw3 ^= (rhs >> 31); dw2 ^= (rhs >> 31); dw1 ^= (rhs >> 31); dw0 ^= rhs; return *this;}
	Vsx128& operator&(const Hex128&  rhs) const alwaysinline{return (Vsx128(*this) &= rhs);}
	Vsx128& operator&(const U64&  rhs)  const alwaysinline{return (Vsx128(*this) &= rhs);}
	Vsx128& operator&(const U32&  rhs)  const alwaysinline{return (Vsx128(*this) &= rhs);}
	Vsx128& operator&(const Vsx128&  rhs) const alwaysinline{return (Vsx128(*this) &= rhs);}
	Vsx128& operator&(const S64&  rhs)  const alwaysinline{return (Vsx128(*this) &= rhs);}
	Vsx128& operator&(const S32&  rhs)  const alwaysinline{return (Vsx128(*this) &= rhs);}
	Vsx128& operator|(const Hex128&  rhs) const alwaysinline{return (Vsx128(*this) |= rhs);}
	Vsx128& operator|(const U64&  rhs)  const alwaysinline{return (Vsx128(*this) |= rhs);}
	Vsx128& operator|(const U32&  rhs)  const alwaysinline{return (Vsx128(*this) |= rhs);}
	Vsx128& operator|(const Vsx128&  rhs) const alwaysinline{return (Vsx128(*this) |= rhs);}
	Vsx128& operator|(const S64&  rhs)  const alwaysinline{return (Vsx128(*this) |= rhs);}
	Vsx128& operator|(const S32&  rhs)  const alwaysinline{return (Vsx128(*this) |= rhs);}
	Vsx128& operator^(const Hex128&  rhs) const alwaysinline{return (Vsx128(*this) ^= rhs);}
	Vsx128& operator^(const U64&  rhs)  const alwaysinline{return (Vsx128(*this) ^= rhs);}
	Vsx128& operator^(const U32&  rhs)  const alwaysinline{return (Vsx128(*this) ^= rhs);}
	Vsx128& operator^(const Vsx128&  rhs) const alwaysinline{return (Vsx128(*this) ^= rhs);}
	Vsx128& operator^(const S64&  rhs)  const alwaysinline{return (Vsx128(*this) ^= rhs);}
	Vsx128& operator^(const S32&  rhs)  const alwaysinline{return (Vsx128(*this) ^= rhs);}

	//##########################################################################
	// Bitshift Operators
	
	INT bsr() const alwaysinline
	{
		INT resul3 = 0;
		U32 count3 = (dw3 | (dw3 >>  1));
		count3 |= (count3 >>  2);
		count3 |= (count3 >>  4);
		count3 |= (count3 >>  8);
		count3 |= (count3 >> 16);
		count3 = ((count3 >> 1) + (1 & count3)); // create power of 2, only if not zero
		resul3 |= (((count3 & 0xFFFF0000) != 0) << 4);
		resul3 |= (((count3 & 0xFF00FF00) != 0) << 3);
		resul3 |= (((count3 & 0xF0F0F0F0) != 0) << 2);
		resul3 |= (((count3 & 0xCCCCCCCC) != 0) << 1);
		resul3 |=  ((count3 & 0xAAAAAAAA) != 0);
		if(resul3 || ((count3 & 0x55555555) != 0)) return resul3 + 96;
		INT resul2 = 0;
		U32 count2 = (dw2 | (dw2 >>  1));
		count2 = (count2 | (count2 >>  2));
		count2 = (count2 | (count2 >>  4));
		count2 = (count2 | (count2 >>  8));
		count2 = (count2 | (count2 >> 16));
		count2 = ((count2 >> 1) + (1 & count2)); // create power of 2, only if not zero
		resul2 |= (((count2 & 0xFFFF0000) != 0) << 4);
		resul2 |= (((count2 & 0xFF00FF00) != 0) << 3);
		resul2 |= (((count2 & 0xF0F0F0F0) != 0) << 2);
		resul2 |= (((count2 & 0xCCCCCCCC) != 0) << 1);
		resul2 |=  ((count2 & 0xAAAAAAAA) != 0);
		if(resul2 || ((count2 & 0x55555555) != 0)) return resul2 + 64;
		INT resul1 = 0;
		U32 count1 = (dw1 | (dw1 >>  1));
		count1 = (count1 | (count1 >>  2));
		count1 = (count1 | (count1 >>  4));
		count1 = (count1 | (count1 >>  8));
		count1 = (count1 | (count1 >> 16));
		count1 = ((count1 >> 1) + (1 & count1)); // create power of 2, only if not zero
		resul1 |= (((count1 & 0xFFFF0000) != 0) << 4);
		resul1 |= (((count1 & 0xFF00FF00) != 0) << 3);
		resul1 |= (((count1 & 0xF0F0F0F0) != 0) << 2);
		resul1 |= (((count1 & 0xCCCCCCCC) != 0) << 1);
		resul1 |=  ((count1 & 0xAAAAAAAA) != 0);
		if(resul1 || ((count1 & 0x55555555) != 0)) return resul1 + 32;
		INT resul0 = 0;
		U32 count0 = (dw0 | (dw0 >>  1));
		count0 = (count0 | (count0 >>  2));
		count0 = (count0 | (count0 >>  4));
		count0 = (count0 | (count0 >>  8));
		count0 = (count0 | (count0 >> 16));
		count0 = ((count0 >> 1) + (1 & count0)); // create power of 2, only if not zero
		resul0 |= (((count0 & 0xFFFF0000) != 0) << 4);
		resul0 |= (((count0 & 0xFF00FF00) != 0) << 3);
		resul0 |= (((count0 & 0xF0F0F0F0) != 0) << 2);
		resul0 |= (((count0 & 0xCCCCCCCC) != 0) << 1);
		resul0 |=  ((count0 & 0xAAAAAAAA) != 0);
		return resul0;
	}
	Vsx128 shr(const INT& rhs) const alwaysinline
	{
		Vsx128 result(*this);
		if(rhs == 0) return result;
		if(rhs < 0)
		{ // Attempt at sane behavior
			U32 right_mask = (0xFFFFFFFF << (-rhs & 0x1F));
			U32 left_mask = ~right_mask;
			U32 rot_by = (-rhs & 0x1F);
			switch(rhs >> 5)
			{
				case 0:{
					result.dw3 <<= rot_by;
					asm("roll %%cl, %%eax": "=a"(result.dw2): "a"(result.dw2), "c"(rot_by):);
					asm("roll %%cl, %%eax": "=a"(result.dw1): "a"(result.dw1), "c"(rot_by):);
					asm("roll %%cl, %%eax": "=a"(result.dw0): "a"(result.dw0), "c"(rot_by):);
					result.dw3 |= (result.dw2 & left_mask); result.dw2 &= right_mask;
					result.dw2 |= (result.dw1 & left_mask); result.dw1 &= right_mask;
					result.dw1 |= (result.dw0 & left_mask); result.dw0 &= right_mask;
				}break;
				case 1:{
					result.dw3 = (result.dw2 << rot_by);
					asm("roll %%cl, %%eax": "=a"(result.dw1): "a"(result.dw1), "c"(rot_by):);
					asm("roll %%cl, %%eax": "=a"(result.dw0): "a"(result.dw0), "c"(rot_by):);
					result.dw3 |= (result.dw1 & left_mask); result.dw2 = (result.dw1 & right_mask);
					result.dw2 |= (result.dw0 & left_mask); result.dw1 = (result.dw0 & right_mask);
					result.dw0 = 0;
				}break;
				case 2:{
					result.dw3 = (result.dw1 << rot_by);
					asm("roll %%cl, %%eax": "=a"(result.dw0): "a"(result.dw0), "c"(rot_by):);
					result.dw3 |= (result.dw0 & left_mask); result.dw2 = (result.dw0 & right_mask);
					result.dw1 = result.dw0 = 0;
				}break;
				case 3:{
					result.dw3 = (result.dw0 << rot_by);
					result.dw2 = result.dw1 = result.dw0 = 0;
				}break;
			}
			return result;
		}
		if((rhs >> 5) >= 4)
		{ // Zero clear and return
			result.dw0 = result.dw1 = result.dw2 = result.dw3 = 0;
			return result;
		}
		U32 right_mask = (0xFFFFFFFF << (-rhs & 0x1F));
		U32 left_mask = ~right_mask;
		U32 rot_by = (rhs & 0x1F);
		switch(rhs >> 5)
		{
			case 0:{
				result.dw0 >>= rot_by;
				asm("rorl %%cl, %%eax": "=a"(result.dw1): "a"(result.dw1), "c"(rot_by):);
				asm("rorl %%cl, %%eax": "=a"(result.dw2): "a"(result.dw2), "c"(rot_by):);
				asm("rorl %%cl, %%eax": "=a"(result.dw3): "a"(result.dw3), "c"(rot_by):);
				result.dw0 |= (result.dw1 & right_mask); result.dw1 &= left_mask;
				result.dw1 |= (result.dw2 & right_mask); result.dw2 &= left_mask;
				result.dw2 |= (result.dw3 & right_mask); result.dw3 &= left_mask;
			}break;
			case 1:{
				result.dw0 = (result.dw1 >> rot_by);
				asm("rorl %%cl, %%eax": "=a"(result.dw2): "a"(result.dw2), "c"(rot_by):);
				asm("rorl %%cl, %%eax": "=a"(result.dw3): "a"(result.dw3), "c"(rot_by):);
				result.dw0 |= (result.dw2 & right_mask); result.dw1 = (result.dw2 & left_mask);
				result.dw1 |= (result.dw3 & right_mask); result.dw2 = (result.dw3 & left_mask);
				result.dw3 = 0;
			}break;
			case 2:{
				result.dw0 = (result.dw2 >> rot_by);
				asm("rorl %%cl, %%eax": "=a"(result.dw3): "a"(result.dw3), "c"(rot_by):);
				result.dw0 |= (result.dw3 & right_mask); result.dw1 = (result.dw3 & left_mask);
				result.dw2 = result.dw3 = 0;
			}break;
			case 3:{
				result.dw0 = (result.dw3 >> rot_by);
				result.dw1 = result.dw2 = result.dw3 = 0;
			}break;
		}
		return result;
	}
	Vsx128 usar(const INT& rhs) const alwaysinline
	{
		Vsx128 result(*this);
		U32 lhs_xor_mask = (S32(result.dw3) >> 31);
		result.dw0 ^= lhs_xor_mask;
		result.dw1 ^= lhs_xor_mask;
		result.dw2 ^= lhs_xor_mask;
		result.dw3 ^= lhs_xor_mask;
		result += (lhs_xor_mask & 1);
		result >>= rhs;
		result.dw0 ^= lhs_xor_mask;
		result.dw1 ^= lhs_xor_mask;
		result.dw2 ^= lhs_xor_mask;
		result.dw3 ^= lhs_xor_mask;
		return (result += (lhs_xor_mask & 1));
	}
	Vsx128 usarr(const INT& rhs) const alwaysinline
	{
		Vsx128 result(*this);
		U32 lhs_xor_mask = (S32(result.dw3) >> 31);
		result.dw0 ^= lhs_xor_mask;
		result.dw1 ^= lhs_xor_mask;
		result.dw2 ^= lhs_xor_mask;
		result.dw3 ^= lhs_xor_mask;
		result += (lhs_xor_mask & 1);
		result = ((result >> rhs)+((result >> (rhs-1)) & 1));
		result.dw0 ^= lhs_xor_mask;
		result.dw1 ^= lhs_xor_mask;
		result.dw2 ^= lhs_xor_mask;
		result.dw3 ^= lhs_xor_mask;
		return (result += (lhs_xor_mask & 1));
	}
	Vsx128& operator>>=(const INT& rhs) alwaysinline
	{
		if(rhs == 0) return *this;
		if(rhs < 0)
		{ // Attempt at sane behavior
			U32 right_mask = (0xFFFFFFFF << (-rhs & 0x1F));
			U32 left_mask = ~right_mask;
			U32 rot_by = (-rhs & 0x1F);
			switch(rhs >> 5)
			{
				case 0:{
					dw3 <<= rot_by;
					asm("roll %%cl, %%eax": "=a"(dw2): "a"(dw2), "c"(rot_by):);
					asm("roll %%cl, %%eax": "=a"(dw1): "a"(dw1), "c"(rot_by):);
					asm("roll %%cl, %%eax": "=a"(dw0): "a"(dw0), "c"(rot_by):);
					dw3 |= (dw2 & left_mask); dw2 &= right_mask;
					dw2 |= (dw1 & left_mask); dw1 &= right_mask;
					dw1 |= (dw0 & left_mask); dw0 &= right_mask;
				}break;
				case 1:{
					dw3 = (dw2 << rot_by);
					asm("roll %%cl, %%eax": "=a"(dw1): "a"(dw1), "c"(rot_by):);
					asm("roll %%cl, %%eax": "=a"(dw0): "a"(dw0), "c"(rot_by):);
					dw3 |= (dw1 & left_mask); dw2 = (dw1 & right_mask);
					dw2 |= (dw0 & left_mask); dw1 = (dw0 & right_mask);
					dw0 = 0;
				}break;
				case 2:{
					dw3 = (dw1 << rot_by);
					asm("roll %%cl, %%eax": "=a"(dw0): "a"(dw0), "c"(rot_by):);
					dw3 |= (dw0 & left_mask); dw2 = (dw0 & right_mask);
					dw1 = dw0 = 0;
				}break;
				case 3:{
					dw3 = (dw0 << rot_by);
					dw2 = dw1 = dw0 = 0;
				}break;
			}
			return *this;
		}
		if((rhs >> 5) >= 4)
		{ // Zero clear and return
			dw0 = dw1 = dw2 = dw3 = 0;
			return *this;
		}
		U32 right_mask = (0xFFFFFFFF << (-rhs & 0x1F));
		U32 left_mask = ~right_mask;
		U32 rot_by = (rhs & 0x1F);
		U32 signbits = (S32(dw3) >> 31);
		switch(rhs >> 5)
		{
			case 0:{
				dw0 >>= rot_by;
				asm("rorl %%cl, %%eax": "=a"(dw1): "a"(dw1), "c"(rot_by):);
				asm("rorl %%cl, %%eax": "=a"(dw2): "a"(dw2), "c"(rot_by):);
				asm("rorl %%cl, %%eax": "=a"(dw3): "a"(dw3), "c"(rot_by):);
				dw0 |= (dw1 & right_mask); dw1 &= left_mask;
				dw1 |= (dw2 & right_mask); dw2 &= left_mask;
				dw2 |= (dw3 & right_mask); dw3 &= left_mask;
				dw3 |= (signbits & right_mask);
			}break;
			case 1:{
				dw0 = (dw1 >> rot_by);
				asm("rorl %%cl, %%eax": "=a"(dw2): "a"(dw2), "c"(rot_by):);
				asm("rorl %%cl, %%eax": "=a"(dw3): "a"(dw3), "c"(rot_by):);
				dw0 |= (dw2 & right_mask); dw1 = (dw2 & left_mask);
				dw1 |= (dw3 & right_mask); dw2 = (dw3 & left_mask);
				dw2 |= (signbits & right_mask);
				dw3 = signbits;
			}break;
			case 2:{
				dw0 = (dw2 >> rot_by);
				asm("rorl %%cl, %%eax": "=a"(dw3): "a"(dw3), "c"(rot_by):);
				dw0 |= (dw3 & right_mask); dw1 = (dw3 & left_mask);
				dw1 |= (signbits & right_mask);
				dw2 = dw3 = signbits;
			}break;
			case 3:{
				dw0 = (dw3 >> rot_by);
				dw0 |= (signbits & right_mask);
				dw1 = dw2 = dw3 = signbits;
			}break;
		}
		return *this;
	}
	Vsx128& operator<<=(const INT& rhs) alwaysinline
	{
		if(rhs == 0) return *this;
		if(rhs < 0)
		{ // Attempt at sane behavior
			U32 right_mask = (0xFFFFFFFF << (rhs & 0x1F));
			U32 left_mask = ~right_mask;
			U32 rot_by = (-rhs & 0x1F);
			U32 signbits = (S32(dw3) >> 31);
			switch(rhs >> 5)
			{
				case 0:{
					dw0 >>= rot_by;
					asm("rorl %%cl, %%eax": "=a"(dw1): "a"(dw1), "c"(rot_by):);
					asm("rorl %%cl, %%eax": "=a"(dw2): "a"(dw2), "c"(rot_by):);
					asm("rorl %%cl, %%eax": "=a"(dw3): "a"(dw3), "c"(rot_by):);
					dw0 |= (dw1 & right_mask); dw1 &= left_mask;
					dw1 |= (dw2 & right_mask); dw2 &= left_mask;
					dw2 |= (dw3 & right_mask); dw3 &= left_mask;
					dw3 |= (signbits & right_mask);
				}break;
				case 1:{
					dw0 = (dw1 >> rot_by);
					asm("rorl %%cl, %%eax": "=a"(dw2): "a"(dw2), "c"(rot_by):);
					asm("rorl %%cl, %%eax": "=a"(dw3): "a"(dw3), "c"(rot_by):);
					dw0 |= (dw2 & right_mask); dw1 = (dw2 & left_mask);
					dw1 |= (dw3 & right_mask); dw2 = (dw3 & left_mask);
					dw2 |= (signbits & right_mask);
					dw3 = signbits;
				}break;
				case 2:{
					dw0 = (dw2 >> rot_by);
					asm("rorl %%cl, %%eax": "=a"(dw3): "a"(dw3), "c"(rot_by):);
					dw0 |= (dw3 & right_mask); dw1 = (dw3 & left_mask);
					dw1 |= (signbits & right_mask);
					dw2 = dw3 = signbits;
				}break;
				case 3:{
					dw0 = (dw3 >> rot_by);
					dw0 |= (signbits & right_mask);
					dw1 = dw2 = dw3 = signbits;
				}break;
			}
			return *this;
		}
		if((rhs >> 5) >= 4)
		{ // Zero clear and return
			dw0 = dw1 = dw2 = dw3 = 0;
			return *this;
		}
		U32 right_mask = (0xFFFFFFFF << (rhs & 0x1F));
		U32 left_mask = ~right_mask;
		U32 rot_by = (rhs & 0x1F);
		switch(rhs >> 5)
		{
			case 0:{
				dw3 <<= rot_by;
				asm("roll %%cl, %%eax": "=a"(dw2): "a"(dw2), "c"(rot_by):);
				asm("roll %%cl, %%eax": "=a"(dw1): "a"(dw1), "c"(rot_by):);
				asm("roll %%cl, %%eax": "=a"(dw0): "a"(dw0), "c"(rot_by):);
				dw3 |= (dw2 & left_mask); dw2 &= right_mask;
				dw2 |= (dw1 & left_mask); dw1 &= right_mask;
				dw1 |= (dw0 & left_mask); dw0 &= right_mask;
			}break;
			case 1:{
				dw3 = (dw2 << rot_by);
				asm("roll %%cl, %%eax": "=a"(dw1): "a"(dw1), "c"(rot_by):);
				asm("roll %%cl, %%eax": "=a"(dw0): "a"(dw0), "c"(rot_by):);
				dw3 |= (dw1 & left_mask); dw2 = (dw1 & right_mask);
				dw2 |= (dw0 & left_mask); dw1 = (dw0 & right_mask);
				dw0 = 0;
			}break;
			case 2:{
				dw3 = (dw1 << rot_by);
				asm("roll %%cl, %%eax": "=a"(dw0): "a"(dw0), "c"(rot_by):);
				dw3 |= (dw0 & left_mask); dw2 = (dw0 & right_mask);
				dw1 = dw0 = 0;
			}break;
			case 3:{
				dw3 = (dw0 << rot_by);
				dw2 = dw1 = dw0 = 0;
			}break;
		}
		return *this;
	}
	Vsx128& operator>>(const INT& rhs) const alwaysinline{return (Vsx128(*this) >>= rhs);}
	Vsx128& operator<<(const INT& rhs) const alwaysinline{return (Vsx128(*this) <<= rhs);}

	//##########################################################################
	// Comparision Functions (Constant)
	BIT operator==(const Hex128&  rhs) const alwaysinline{
		return ((~GET_DW3(rhs) & 0x80000000) && (dw3 == GET_DW3(rhs)) && (dw2 == GET_DW2(rhs)) && (dw1 == GET_DW1(rhs)) && (dw0 == GET_DW0(rhs)));}
	BIT operator==(const Vsx128&  rhs) const alwaysinline{
		return ((dw3 == rhs.dw3) && (dw2 == rhs.dw2) && (dw1 == rhs.dw1) && (dw0 == rhs.dw0));}
	BIT operator==(const U64&  rhs) const alwaysinline{
		return ((rhs < 0x8000000000000000LL) && (dw3 == 0) && (dw2 == 0) && (dw1 == U32(rhs >> 32)) && (dw0 == U32(rhs & 0xFFFFFFFF)));}
	BIT operator==(const S64&  rhs) const alwaysinline{
		return ((dw3 == (rhs >> 63)) && (dw2 == (rhs >> 63)) && (dw1 == U32(rhs >> 32)) && (dw0 == U32(rhs & 0xFFFFFFFF)));}
	BIT operator==(const U32&  rhs) const alwaysinline{
		return ((rhs < 0x80000000L) && (dw3 == 0) && (dw2 == 0) && (dw1 == 0) && (rhs == dw0));}
	BIT operator==(const S32&  rhs) const alwaysinline{
		return ((dw3 == (rhs >> 31)) && (dw2 == (rhs >> 31)) && (dw1 == (rhs >> 31)) && (U32(rhs) == dw0));}
	BIT operator!=(const Hex128&  rhs) const alwaysinline{
		return ((GET_DW3(rhs) & 0x80000000) || (dw3 != GET_DW3(rhs)) || (dw2 != GET_DW2(rhs)) || (dw1 != GET_DW1(rhs)) || (dw0 != GET_DW0(rhs)));}
	BIT operator!=(const Vsx128&  rhs) const alwaysinline{
		return ((dw3 != rhs.dw3) || (dw2 != rhs.dw2) || (dw1 != rhs.dw1) || (dw0 != rhs.dw0));}
	BIT operator!=(const U64&  rhs) const alwaysinline{
		return ((rhs >= 0x8000000000000000LL) || (dw3 != 0) || (dw2 != 0) || (dw1 != U32(rhs >> 32)) || (dw0 != U32(rhs & 0xFFFFFFFF)));}
	BIT operator!=(const S64&  rhs) const alwaysinline{
		return ((dw3 != (rhs >> 63)) || (dw2 != (rhs >> 63)) || (dw1 != U32(rhs >> 32)) || (dw0 != U32(rhs & 0xFFFFFFFF)));}
	BIT operator!=(const U32&  rhs) const alwaysinline{
		return ((rhs >= 0x80000000L) || (dw3 != 0) || (dw2 != 0) || (dw1 != 0) || (rhs != dw0));}
	BIT operator!=(const S32&  rhs) const alwaysinline{
		return ((dw3 != (rhs >> 31)) || (dw2 != (rhs >> 31)) || (dw1 != (rhs >> 31)) || (U32(rhs) != dw0));}
	BIT operator>(const Hex128&  rhs) const alwaysinline{
		if((GET_DW3(rhs) | dw3) & 0x80000000) return false;
		return ((dw3 > GET_DW3(rhs)) 
			|| ((dw3 == GET_DW3(rhs)) && (dw2 > GET_DW2(rhs)))
			|| ((dw3 == GET_DW3(rhs)) && (dw2 == GET_DW2(rhs)) && (dw1 > GET_DW1(rhs)))
			|| ((dw3 == GET_DW3(rhs)) && (dw2 == GET_DW2(rhs)) && (dw1 == GET_DW1(rhs)) && (dw0 > GET_DW0(rhs))));}
	BIT operator>(const Vsx128&  rhs) const alwaysinline{
		return (((dw3^0x80000000) > (rhs.dw3^0x80000000)) 
			|| (((dw3^0x80000000) == (rhs.dw3^0x80000000)) && (dw2 > rhs.dw2))
			|| (((dw3^0x80000000) == (rhs.dw3^0x80000000)) && (dw2 == rhs.dw2) && (dw1 > rhs.dw1))
			|| (((dw3^0x80000000) == (rhs.dw3^0x80000000)) && (dw2 == rhs.dw2) && (dw1 == rhs.dw1) && (dw0 > rhs.dw0)));}
	BIT operator>(const U64&  rhs) const alwaysinline{
		if(dw3 & 0x80000000) return false;
		return ((dw3 > 0) 
			|| ((dw3 == 0) && (dw2 > 0))
			|| ((dw3 == 0) && (dw2 == 0) && (dw1 > U32(rhs >> 32)))
			|| ((dw3 == 0) && (dw2 == 0) && (dw1 == U32(rhs >> 32)) && (dw0 > U32(rhs & 0xFFFFFFFF))));}
	BIT operator>(const S64&  rhs) const alwaysinline{
		if((dw3 & 0x80000000) //need to rule out that this Vsx128 is too negative
		&& (((dw3 & 0x7FFFFFFF) != 0x7FFFFFFF) || (dw2 != 0xFFFFFFFF) 
			|| ((dw1 & 0x80000000) != 0x80000000))) return false;
		S64 rrhs = (rhs^0x8000000000000000LL);
		return ((((dw1^0x80000000) > U32(rrhs >> 32)))
			 || (((dw1^0x80000000) == U32(rrhs >> 32)) && (dw0 > U32(rrhs & 0xFFFFFFFF))));}
	BIT operator>(const U32&  rhs) const alwaysinline{
		if(dw3 & 0x80000000) return false;
		return ((dw3 > 0) 
			|| ((dw3 == 0) && (dw2 > 0))
			|| ((dw3 == 0) && (dw2 == 0) && (dw1 > 0))
			|| ((dw3 == 0) && (dw2 == 0) && (dw1 == 0) && (dw0 > rhs)));}
	BIT operator>(const S32&  rhs) const alwaysinline{
		if((dw3 & 0x80000000) //need to rule out that this Vsx128 is too negative
		&& (((dw3 & 0x7FFFFFFF) != 0x7FFFFFFF) || (dw2 != 0xFFFFFFFF) || (dw1 != 0xFFFFFFFF) 
			|| ((dw0 & 0x80000000) != 0x80000000))) return false;
		S32 rrhs = (rhs^0x80000000L);
		return ((dw0^0x80000000) > U32(rrhs));}
	BIT operator<(const Hex128&  rhs) const alwaysinline{
		if((GET_DW3(rhs) | dw3) & 0x80000000) return true;
		return ((dw3 < GET_DW3(rhs)) 
			|| ((dw3 == GET_DW3(rhs)) && (dw2 < GET_DW2(rhs)))
			|| ((dw3 == GET_DW3(rhs)) && (dw2 == GET_DW2(rhs)) && (dw1 < GET_DW1(rhs)))
			|| ((dw3 == GET_DW3(rhs)) && (dw2 == GET_DW2(rhs)) && (dw1 == GET_DW1(rhs)) && (dw0 < GET_DW0(rhs))));}
	BIT operator<(const Vsx128&  rhs) const alwaysinline{
		return (((dw3^0x80000000) < (rhs.dw3^0x80000000)) 
			|| (((dw3^0x80000000) == (rhs.dw3^0x80000000)) && (dw2 < rhs.dw2))
			|| (((dw3^0x80000000) == (rhs.dw3^0x80000000)) && (dw2 == rhs.dw2) && (dw1 < rhs.dw1))
			|| (((dw3^0x80000000) == (rhs.dw3^0x80000000)) && (dw2 == rhs.dw2) && (dw1 == rhs.dw1) && (dw0 < rhs.dw0)));}
	BIT operator<(const U64&  rhs) const alwaysinline{
		if(dw3 & 0x80000000) return true;
		return (((dw3 == 0) && (dw2 == 0) && (dw1 < U32(rhs >> 32)))
			 || ((dw3 == 0) && (dw2 == 0) && (dw1 == U32(rhs >> 32)) && (dw0 < U32(rhs & 0xFFFFFFFF))));}
	BIT operator<(const S64&  rhs) const alwaysinline{
		if((~dw3 & 0x80000000) //need to rule out that this Vsx128 is too positive
		&& (((dw3 & 0x7FFFFFFF) != 0x00000000) || (dw2 != 0x00000000) 
			|| ((dw1 & 0x80000000) != 0x00000000))) return false;
		S64 rrhs = (rhs^0x8000000000000000LL);
		return ((((dw1^0x80000000) < U32(rrhs >> 32)))
			 || (((dw1^0x80000000) == U32(rrhs >> 32)) && (dw0 < U32(rrhs & 0xFFFFFFFF))));}
	BIT operator<(const U32&  rhs) const alwaysinline{
		if(dw3 & 0x80000000) return true;
		return (((dw3 == 0) && (dw2 == 0) && (dw1 == 0) && (dw0 < rhs)));}
	BIT operator<(const S32&  rhs) const alwaysinline{
		if((~dw3 & 0x80000000) //need to rule out that this Vsx128 is too positive
		&& (((dw3 & 0x7FFFFFFF) != 0x00000000) || (dw2 != 0x00000000) || (dw1 != 0x00000000) 
			|| ((dw0 & 0x80000000) != 0x00000000))) return false;
		S32 rrhs = (rhs^0x80000000L);
		return ((dw0^0x80000000) < U32(rrhs));}
	//!/////////////////////////////////////////////////////////////////////////
	BIT operator<=(const Hex128&  rhs) const alwaysinline{
		if((GET_DW3(rhs) | dw3) & 0x80000000) return false;
		return !((dw3 > GET_DW3(rhs)) 
			|| ((dw3 == GET_DW3(rhs)) && (dw2 > GET_DW2(rhs)))
			|| ((dw3 == GET_DW3(rhs)) && (dw2 == GET_DW2(rhs)) && (dw1 > GET_DW1(rhs)))
			|| ((dw3 == GET_DW3(rhs)) && (dw2 == GET_DW2(rhs)) && (dw1 == GET_DW1(rhs)) && (dw0 > GET_DW0(rhs))));}
	BIT operator<=(const Vsx128&  rhs) const alwaysinline{
		return !(((dw3^0x80000000) > (rhs.dw3^0x80000000)) 
			|| (((dw3^0x80000000) == (rhs.dw3^0x80000000)) && (dw2 > rhs.dw2))
			|| (((dw3^0x80000000) == (rhs.dw3^0x80000000)) && (dw2 == rhs.dw2) && (dw1 > rhs.dw1))
			|| (((dw3^0x80000000) == (rhs.dw3^0x80000000)) && (dw2 == rhs.dw2) && (dw1 == rhs.dw1) && (dw0 > rhs.dw0)));}
	BIT operator<=(const U64&  rhs) const alwaysinline{
		if(dw3 & 0x80000000) return false;
		return !((dw3 > 0) 
			|| ((dw3 == 0) && (dw2 > 0))
			|| ((dw3 == 0) && (dw2 == 0) && (dw1 > U32(rhs >> 32)))
			|| ((dw3 == 0) && (dw2 == 0) && (dw1 == U32(rhs >> 32)) && (dw0 > U32(rhs & 0xFFFFFFFF))));}
	BIT operator<=(const S64&  rhs) const alwaysinline{
		if((dw3 & 0x80000000) //need to rule out that this Vsx128 is too negative
		&& (((dw3 & 0x7FFFFFFF) != 0x7FFFFFFF) || (dw2 != 0xFFFFFFFF) 
			|| ((dw1 & 0x80000000) != 0x80000000))) return false;
		S64 rrhs = (rhs^0x8000000000000000LL);
		return !((((dw1^0x80000000) > U32(rrhs >> 32)))
			 || (((dw1^0x80000000) == U32(rrhs >> 32)) && (dw0 > U32(rrhs & 0xFFFFFFFF))));}
	BIT operator<=(const U32&  rhs) const alwaysinline{
		if(dw3 & 0x80000000) return false;
		return !((dw3 > 0) 
			|| ((dw3 == 0) && (dw2 > 0))
			|| ((dw3 == 0) && (dw2 == 0) && (dw1 > 0))
			|| ((dw3 == 0) && (dw2 == 0) && (dw1 == 0) && (dw0 > rhs)));}
	BIT operator<=(const S32&  rhs) const alwaysinline{
		if((dw3 & 0x80000000) //need to rule out that this Vsx128 is too negative
		&& (((dw3 & 0x7FFFFFFF) != 0x7FFFFFFF) || (dw2 != 0xFFFFFFFF) || (dw1 != 0xFFFFFFFF) 
			|| ((dw0 & 0x80000000) != 0x80000000))) return false;
		S32 rrhs = (rhs^0x80000000L);
		return !((dw0^0x80000000) > U32(rrhs));}
	BIT operator>=(const Hex128&  rhs) const alwaysinline{
		if((GET_DW3(rhs) | dw3) & 0x80000000) return false;
		return !((dw3 < GET_DW3(rhs)) 
			|| ((dw3 == GET_DW3(rhs)) && (dw2 < GET_DW2(rhs)))
			|| ((dw3 == GET_DW3(rhs)) && (dw2 == GET_DW2(rhs)) && (dw1 < GET_DW1(rhs)))
			|| ((dw3 == GET_DW3(rhs)) && (dw2 == GET_DW2(rhs)) && (dw1 == GET_DW1(rhs)) && (dw0 < GET_DW0(rhs))));}
	BIT operator>=(const Vsx128&  rhs) const alwaysinline{
		return !(((dw3^0x80000000) < (rhs.dw3^0x80000000)) 
			|| (((dw3^0x80000000) == (rhs.dw3^0x80000000)) && (dw2 < rhs.dw2))
			|| (((dw3^0x80000000) == (rhs.dw3^0x80000000)) && (dw2 == rhs.dw2) && (dw1 < rhs.dw1))
			|| (((dw3^0x80000000) == (rhs.dw3^0x80000000)) && (dw2 == rhs.dw2) && (dw1 == rhs.dw1) && (dw0 < rhs.dw0)));}
	BIT operator>=(const U64&  rhs) const alwaysinline{
		if(dw3 & 0x80000000) return false;
		return !(((dw3 == 0) && (dw2 == 0) && (dw1 < U32(rhs >> 32)))
			 || ((dw3 == 0) && (dw2 == 0) && (dw1 == U32(rhs >> 32)) && (dw0 < U32(rhs & 0xFFFFFFFF))));}
	BIT operator>=(const S64&  rhs) const alwaysinline{
		if((~dw3 & 0x80000000) //need to rule out that this Vsx128 is too positive
		&& (((dw3 & 0x7FFFFFFF) != 0x00000000) || (dw2 != 0x00000000) 
			|| ((dw1 & 0x80000000) != 0x00000000))) return false;
		S64 rrhs = (rhs^0x8000000000000000LL);
		return !((((dw1^0x80000000) < U32(rrhs >> 32)))
			 || (((dw1^0x80000000) == U32(rrhs >> 32)) && (dw0 < U32(rrhs & 0xFFFFFFFF))));}
	BIT operator>=(const U32&  rhs) const alwaysinline{
		if(dw3 & 0x80000000) return false;
		return !(((dw3 == 0) && (dw2 == 0) && (dw1 == 0) && (dw0 < rhs)));}
	BIT operator>=(const S32&  rhs) const alwaysinline{
		if((~dw3 & 0x80000000) //need to rule out that this Vsx128 is too positive
		&& (((dw3 & 0x7FFFFFFF) != 0x00000000) || (dw2 != 0x00000000) || (dw1 != 0x00000000) 
			|| ((dw0 & 0x80000000) != 0x00000000))) return false;
		S32 rrhs = (rhs^0x80000000L);
		return !((dw0^0x80000000) < U32(rrhs));}
};
typedef Vsx128 Fix128;
static Vsx128 operator+(const U64&  lhs, const Vsx128&  rhs){return (Vsx128(lhs) += rhs);}
static Vsx128 operator+(const U32&  lhs, const Vsx128&  rhs){return (Vsx128(lhs) += rhs);}
static Vsx128 operator+(const S64&  lhs, const Vsx128&  rhs){return (Vsx128(lhs) += rhs);}
static Vsx128 operator+(const S32&  lhs, const Vsx128&  rhs){return (Vsx128(lhs) += rhs);}
static Vsx128 operator-(const U64&  lhs, const Vsx128&  rhs){return (Vsx128(lhs) -= rhs);}
static Vsx128 operator-(const U32&  lhs, const Vsx128&  rhs){return (Vsx128(lhs) -= rhs);}
static Vsx128 operator-(const S64&  lhs, const Vsx128&  rhs){return (Vsx128(lhs) -= rhs);}
static Vsx128 operator-(const S32&  lhs, const Vsx128&  rhs){return (Vsx128(lhs) -= rhs);}
static Vsx128 operator*(const S64&  lhs, const Vsx128&  rhs){return (Vsx128(lhs) *= rhs);}
static Vsx128 operator*(const S32&  lhs, const Vsx128&  rhs){return (Vsx128(lhs) *= rhs);}
static Vsx128 operator/(const S64&  lhs, const Vsx128&  rhs){return (Vsx128(lhs) /= rhs);}
static Vsx128 operator/(const S32&  lhs, const Vsx128&  rhs){return (Vsx128(lhs) /= rhs);}
static Vsx128 operator%(const S64&  lhs, const Vsx128&  rhs){return (Vsx128(lhs) %= rhs);}
static Vsx128 operator%(const S32&  lhs, const Vsx128&  rhs){return (Vsx128(lhs) %= rhs);}
static Vsx128 operator&(const U64&  lhs, const Vsx128&  rhs){return (Vsx128(lhs) &= rhs);}
static Vsx128 operator&(const U32&  lhs, const Vsx128&  rhs){return (Vsx128(lhs) &= rhs);}
static Vsx128 operator&(const S64&  lhs, const Vsx128&  rhs){return (Vsx128(lhs) &= rhs);}
static Vsx128 operator&(const S32&  lhs, const Vsx128&  rhs){return (Vsx128(lhs) &= rhs);}
static Vsx128 operator|(const U64&  lhs, const Vsx128&  rhs){return (Vsx128(lhs) |= rhs);}
static Vsx128 operator|(const U32&  lhs, const Vsx128&  rhs){return (Vsx128(lhs) |= rhs);}
static Vsx128 operator|(const S64&  lhs, const Vsx128&  rhs){return (Vsx128(lhs) |= rhs);}
static Vsx128 operator|(const S32&  lhs, const Vsx128&  rhs){return (Vsx128(lhs) |= rhs);}
static Vsx128 operator^(const U64&  lhs, const Vsx128&  rhs){return (Vsx128(lhs) ^= rhs);}
static Vsx128 operator^(const U32&  lhs, const Vsx128&  rhs){return (Vsx128(lhs) ^= rhs);}
static Vsx128 operator^(const S64&  lhs, const Vsx128&  rhs){return (Vsx128(lhs) ^= rhs);}
static Vsx128 operator^(const S32&  lhs, const Vsx128&  rhs){return (Vsx128(lhs) ^= rhs);}
static BIT operator==(const U64&  lhs, const Vsx128&  rhs){return (Vsx128(lhs) == rhs);}
static BIT operator==(const U32&  lhs, const Vsx128&  rhs){return (Vsx128(lhs) == rhs);}
static BIT operator==(const S64&  lhs, const Vsx128&  rhs){return (Vsx128(lhs) == rhs);}
static BIT operator==(const S32&  lhs, const Vsx128&  rhs){return (Vsx128(lhs) == rhs);}
static BIT operator!=(const U64&  lhs, const Vsx128&  rhs){return (Vsx128(lhs) != rhs);}
static BIT operator!=(const U32&  lhs, const Vsx128&  rhs){return (Vsx128(lhs) != rhs);}
static BIT operator!=(const S64&  lhs, const Vsx128&  rhs){return (Vsx128(lhs) != rhs);}
static BIT operator!=(const S32&  lhs, const Vsx128&  rhs){return (Vsx128(lhs) != rhs);}
static BIT operator>=(const U64&  lhs, const Vsx128&  rhs){return (Vsx128(lhs) >= rhs);}
static BIT operator>=(const U32&  lhs, const Vsx128&  rhs){return (Vsx128(lhs) >= rhs);}
static BIT operator>=(const S64&  lhs, const Vsx128&  rhs){return (Vsx128(lhs) >= rhs);}
static BIT operator>=(const S32&  lhs, const Vsx128&  rhs){return (Vsx128(lhs) >= rhs);}
static BIT operator<=(const U64&  lhs, const Vsx128&  rhs){return (Vsx128(lhs) <= rhs);}
static BIT operator<=(const U32&  lhs, const Vsx128&  rhs){return (Vsx128(lhs) <= rhs);}
static BIT operator<=(const S64&  lhs, const Vsx128&  rhs){return (Vsx128(lhs) <= rhs);}
static BIT operator<=(const S32&  lhs, const Vsx128&  rhs){return (Vsx128(lhs) <= rhs);}
static BIT operator>(const U64&  lhs, const Vsx128&  rhs){return (Vsx128(lhs) > rhs);}
static BIT operator>(const U32&  lhs, const Vsx128&  rhs){return (Vsx128(lhs) > rhs);}
static BIT operator>(const S64&  lhs, const Vsx128&  rhs){return (Vsx128(lhs) > rhs);}
static BIT operator>(const S32&  lhs, const Vsx128&  rhs){return (Vsx128(lhs) > rhs);}
static BIT operator<(const U64&  lhs, const Vsx128&  rhs){return (Vsx128(lhs) < rhs);}
static BIT operator<(const U32&  lhs, const Vsx128&  rhs){return (Vsx128(lhs) < rhs);}
static BIT operator<(const S64&  lhs, const Vsx128&  rhs){return (Vsx128(lhs) < rhs);}
static BIT operator<(const S32&  lhs, const Vsx128&  rhs){return (Vsx128(lhs) < rhs);}
#undef alwaysinline
#undef GET_DW3
#undef GET_DW2
#undef GET_DW1
#undef GET_DW0
#include "Hex128.h"

