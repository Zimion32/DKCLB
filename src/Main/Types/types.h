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
#define BUILD_BUG_ON(condition) ((void)sizeof(char[1 - 2*!!(condition)]))
#include <cstddef> //Standard Definitions: ptrdiff_t, size_t, NULL

#include "checked.h" //checked vector and string types

typedef unsigned long long          uint64_t;
typedef unsigned int                uint32_t;
typedef unsigned short int          uint16_t;
typedef unsigned char                uint8_t;
typedef long long                    int64_t;
typedef signed int                   int32_t;
typedef signed short int             int16_t;
typedef signed char                   int8_t;

/// (sizeof(char) == 1) ALWAYS.
/// C spec 6.5.3.4 says "When applied to an operand that has type char, 
/// unsigned char, or signed char, (or a qualified version thereof) the result is 1."
/// This guarantee is used to ensure binary compatibility in some of DELTA's code,
/// especially the object DLL interfaces and APIs.

typedef    uint64_t  U64; //__attribute__((aligned(8))) //alignment requirements removed.
typedef    uint32_t  U32;
typedef    uint16_t  U16;
typedef     uint8_t  U08;
typedef     int64_t  S64;
typedef     int32_t  S32;
typedef     int16_t  S16;
typedef      int8_t  S08;
typedef       float  F32;
typedef      double  F64;
typedef long double  F80;
typedef       void*  PTR;
typedef        char  C08;
typedef     wchar_t  C16;
typedef         S32  INT;
typedef         F64  FLO;
typedef         C08  CHR;
typedef        bool  BIT;
typedef const char*  STR;
typedef         int  IND; //for loop indexes

//DELTA API TYPES
typedef     int64_t  NUM;
typedef     int64_t  BLL;
typedef        void  VOI;
typedef      double  POS;


struct SPoint
{
    S32 x, y;
    SPoint(): x(0), y(0) {}
    SPoint(S32 x, S32 y): x(x), y(y) {}
    SPoint(const SPoint& other): x(other.x), y(other.y) {}
    SPoint(SPoint* other): x(other->x), y(other->y) {}
    SPoint& Translate(S32 xx, S32 yy){x+=xx; y+=yy; return *this;}
};
struct SRect
{
    S32 l, t, r, b;
    SRect(): l(0), t(0), r(0), b(0) {}
    SRect(S32 l, S32 t, S32 r, S32 b): l(l), t(t), r(r), b(b) {}
    SRect(const SRect& other): l(other.l), t(other.t), r(other.r), b(other.b) {}
    SRect(SRect* other): l(other->l), t(other->t), r(other->r), b(other->b) {}
    SRect& Translate(S32 xx, S32 yy){l+=xx; t+=yy; r+=xx; b+=yy; return *this;}
    SRect& ClipWith(SRect other){
        if(other.l > l) l = other.l;
        if(other.t > t) t = other.t;
        if(other.r < r) r = other.r;
        if(other.b < b) b = other.b;
        return *this;
    }
    SRect& ClipWith(SRect* other){
        if(other->l > l) l = other->l;
        if(other->t > t) t = other->t;
        if(other->r < r) r = other->r;
        if(other->b < b) b = other->b;
        return *this;
    }
};
struct SFPoint
{
    FLO x, y;
    INT flags, side;
    SFPoint(): x(0), y(0) {}
    SFPoint(FLO x, FLO y): x(x), y(y) {}
    SFPoint(const SFPoint& other) {*this = other;}
    SFPoint(SFPoint* other) {*this = *other;}
    SFPoint& Translate(FLO xx, FLO yy){x+=xx; y+=yy; return *this;}
};
struct SFRect
{
    F64 l, t, r, b;
    SFRect(): l(0), t(0), r(0), b(0) {}
    SFRect(F64 l, F64 t, F64 r, F64 b): l(l), t(t), r(r), b(b) {}
    SFRect(const SFRect& other): l(other.l), t(other.t), r(other.r), b(other.b) {}
    SFRect(SFRect* other): l(other->l), t(other->t), r(other->r), b(other->b) {}
    SFRect& Translate(F64 xx, F64 yy){l+=xx; t+=yy; r+=xx; b+=yy; return *this;}
    SFRect& ClipWith(SFRect other){
        if(other.l > l) l = other.l;
        if(other.t > t) t = other.t;
        if(other.r < r) r = other.r;
        if(other.b < b) b = other.b;
        return *this;
    }
    SFRect& ClipWith(SFRect* other){
        if(other->l > l) l = other->l;
        if(other->t > t) t = other->t;
        if(other->r < r) r = other->r;
        if(other->b < b) b = other->b;
        return *this;
    }
};
struct SFLine
{
    union{
        F64 x0;
        F64 ax;
    };
    union{
        F64 y0;
        F64 ay;
    };
    union{
        F64 x1;
        F64 bx;
    };
    union{
        F64 y1;
        F64 by;
    };
    INT flags, side;
    U32 events[4]; //extra data, used in DELTA's physics engine
    SFLine(): x0(0), y0(0), x1(0), y1(0), flags(0), side(0) {}
    SFLine(F64 x0, F64 y0, F64 x1, F64 y1): x0(x0), y0(y0), x1(x1), y1(y1), flags(0), side(0) {}
    SFLine(const SFLine& other) {*this = other;}
    SFLine(SFLine* other) {*this = *other;}
    SFLine& Translate(F64 xx, F64 yy){x0+=xx; y0+=yy; x1+=xx; y1+=yy; return *this;}
    BIT OneWayCheck(const SFLine& B);
    BIT Intersect(SFPoint& I, const SFLine& B, BIT all_edges = true);
    INT Tolerance(SFPoint& I, const SFLine& B, FLO tolerance);
    INT CalcSide(INT wall);
};



#pragma pack(push, 1)
struct B16
{
	U32 dw3, dw2, dw1, dw0;
	B16(){dw0 = dw1 = dw2 = dw3 = 0x000000000;}
	B16(U32 dw0, U32 dw1, U32 dw2, U32 dw3):
		dw0(dw0), dw1(dw1), dw2(dw2), dw3(dw3){}
	inline BIT operator==(const B16& rhs) const
	{
		return((this->dw0 == rhs.dw0)
			&& (this->dw1 == rhs.dw1)
			&& (this->dw2 == rhs.dw2)
			&& (this->dw3 == rhs.dw3));
	}
	inline BIT operator!=(const B16& rhs) const
	{
		return !(*this == rhs);
	}
};
struct B32
{
	U32 dw7, dw6, dw5, dw4, dw3, dw2, dw1, dw0;
	B32(){dw0 = dw1 = dw2 = dw3 = dw4 = dw5 = dw6 = dw7 = 0x000000000;}
	B32(U32 dw0, U32 dw1, U32 dw2, U32 dw3, U32 dw4, U32 dw5, U32 dw6, U32 dw7):
		dw0(dw0), dw1(dw1), dw2(dw2), dw3(dw3), dw4(dw4), dw5(dw5), dw6(dw6), dw7(dw7){}
	inline BIT operator==(const B32& rhs) const
	{
		return((this->dw0 == rhs.dw0)
			&& (this->dw1 == rhs.dw1)
			&& (this->dw2 == rhs.dw2)
			&& (this->dw3 == rhs.dw3)
			&& (this->dw4 == rhs.dw4)
			&& (this->dw5 == rhs.dw5)
			&& (this->dw6 == rhs.dw6)
			&& (this->dw7 == rhs.dw7));
	}
	inline BIT operator!=(const B32& rhs) const
	{
		return !(*this == rhs);
	}
	B16 To128()
	{
	    B16 x;
	    x.dw0 = (dw0 ^ dw4);
	    x.dw1 = (dw1 ^ dw5);
	    x.dw2 = (dw2 ^ dw6);
	    x.dw3 = (dw3 ^ dw7);
	    return x;
	}
};
class UVL
{
	friend class CFileIO;
	protected:
		void* data;
		U32 length;
		UVL();
		UVL(const UVL& o);
		UVL(void* qData, U32 qLength)
		{
			data = qData;
			length = ((qLength-1) & 3);
		}
		
	public:
		operator U32(){
			switch(length){
				case 0x00: return (((U32)(*((U08*)data))) & ((U32)0x000000FF));
				case 0x01: return (((U32)(*((U16*)data))) & ((U32)0x0000FFFF));
				case 0x02: return ((((U32)(*((U16*)data))) & ((U32)0x0000FFFF)) | ((((U32)(*(((U08*)data)+2))) << 16) & ((U32)0x00FF0000)));
				case 0x03: return (((U32)(*((U32*)data))) & ((U32)0xFFFFFFFF));
			} return 0; }
		UVL& operator=(const U32& i){
			switch(length){
				case 0x00:	(*((U08*)data)) = ((U08)(i & ((U32)0x000000FF)));		break;
				case 0x01:	(*((U16*)data)) = ((U16)(i & ((U32)0x0000FFFF)));		break;
				case 0x02:	(*((U16*)data)) = ((U16)(i & ((U32)0x0000FFFF)));
						(*(((U08*)data)+2)) = ((U08)((i >> 16) & ((U32)0x000000FF)));	break;
				case 0x03:	(*((U32*)data)) = ((U32)(i & ((U32)0xFFFFFFFF)));		break;
			} return *this; }
		UVL& operator+=(const U32& i){
			switch(length){
				case 0x00:	(*((U08*)data)) += ((U08)(i & ((U32)0x000000FF)));	break;
				case 0x01:	(*((U16*)data)) += ((U16)(i & ((U32)0x0000FFFF)));	break;
				case 0x02:{	U32 value = ((((U32)(*((U16*)data))) & ((U32)0x0000FFFF)) | ((((U32)(*(((U08*)data)+2))) << 16) & 0x00FF0000));
							value			  += 		 (i & ((U32)0x00FFFFFF));
							(*((U16*)data)) = ((U16)(value & ((U32)0x0000FFFF)));
							(*(((U08*)data)+2)) = ((U08)((value >> 16) & ((U32)0x000000FF)));}break;
				case 0x03:	(*((U32*)data)) += ((U32)(i & ((U32)0xFFFFFFFF)));	break;
			} return *this; }
		UVL& operator-=(const U32& i){
			switch(length){
				case 0x00:	(*((U08*)data)) -= ((U08)(i & ((U32)0x000000FF)));	break;
				case 0x01:	(*((U16*)data)) -= ((U16)(i & ((U32)0x0000FFFF)));	break;
				case 0x02:{	U32 value = ((((U32)(*((U16*)data))) & ((U32)0x0000FFFF)) | ((((U32)(*(((U08*)data)+2))) << 16) & 0x00FF0000));
							value			  -= 		 (i & ((U32)0x00FFFFFF));
							(*((U16*)data)) = ((U16)(value & ((U32)0x0000FFFF)));
							(*(((U08*)data)+2)) = ((U08)((value >> 16) & ((U32)0x000000FF)));}break;
				case 0x03:	(*((U32*)data)) -= ((U32)(i & ((U32)0xFFFFFFFF)));	break;
			} return *this; }
		UVL& operator*=(const U32& i){
			switch(length){
				case 0x00:	(*((U08*)data)) *= ((U08)(i & ((U32)0x000000FF)));	break;
				case 0x01:	(*((U16*)data)) *= ((U16)(i & ((U32)0x0000FFFF)));	break;
				case 0x02:{	U32 value = ((((U32)(*((U16*)data))) & ((U32)0x0000FFFF)) | ((((U32)(*(((U08*)data)+2))) << 16) & 0x00FF0000));
							value			  *= 		 (i & ((U32)0x00FFFFFF));
							(*((U16*)data)) = ((U16)(value & ((U32)0x0000FFFF)));
							(*(((U08*)data)+2)) = ((U08)((value >> 16) & ((U32)0x000000FF)));}break;
				case 0x03:	(*((U32*)data)) *= ((U32)(i & ((U32)0xFFFFFFFF)));	break;
			} return *this; }
		UVL& operator/=(const U32& i){
			switch(length){
				case 0x00:	(*((U08*)data)) /= ((U08)(i & ((U32)0x000000FF)));	break;
				case 0x01:	(*((U16*)data)) /= ((U16)(i & ((U32)0x0000FFFF)));	break;
				case 0x02:{	U32 value = ((((U32)(*((U16*)data))) & ((U32)0x0000FFFF)) | ((((U32)(*(((U08*)data)+2))) << 16) & 0x00FF0000));
							value			  /= 		 (i & ((U32)0x00FFFFFF));
							(*((U16*)data)) = ((U16)(value & ((U32)0x0000FFFF)));
							(*(((U08*)data)+2)) = ((U08)((value >> 16) & ((U32)0x000000FF)));}break;
				case 0x03:	(*((U32*)data)) /= ((U32)(i & ((U32)0xFFFFFFFF)));	break;
			} return *this; }
		UVL& operator%=(const U32& i){
			switch(length){
				case 0x00:	(*((U08*)data)) %= ((U08)(i & ((U32)0x000000FF)));	break;
				case 0x01:	(*((U16*)data)) %= ((U16)(i & ((U32)0x0000FFFF)));	break;
				case 0x02:{	U32 value = ((((U32)(*((U16*)data))) & ((U32)0x0000FFFF)) | ((((U32)(*(((U08*)data)+2))) << 16) & 0x00FF0000));
							value			  %= 		 (i & ((U32)0x00FFFFFF));
							(*((U16*)data)) = ((U16)(value & ((U32)0x0000FFFF)));
							(*(((U08*)data)+2)) = ((U08)((value >> 16) & ((U32)0x000000FF)));}break;
				case 0x03:	(*((U32*)data)) %= ((U32)(i & ((U32)0xFFFFFFFF)));	break;
			} return *this; }
		UVL& operator&=(const U32& i){
			switch(length){
				case 0x00:	(*((U08*)data)) &= ((U08)(i & ((U32)0x000000FF)));	break;
				case 0x01:	(*((U16*)data)) &= ((U16)(i & ((U32)0x0000FFFF)));	break;
				case 0x02:	(*((U16*)data)) &= ((U16)(i & ((U32)0x0000FFFF)));
						(*(((U08*)data)+2)) &= ((U08)((i >> 16) & ((U32)0x000000FF))); break;
				case 0x03:	(*((U32*)data)) &= ((U32)(i & ((U32)0xFFFFFFFF)));	break;
			} return *this; }
		UVL& operator|=(const U32& i){
			switch(length){
				case 0x00:	(*((U08*)data)) |= ((U08)(i & ((U32)0x000000FF)));	break;
				case 0x01:	(*((U16*)data)) |= ((U16)(i & ((U32)0x0000FFFF)));	break;
				case 0x02:	(*((U16*)data)) |= ((U16)(i & ((U32)0x0000FFFF)));
						(*(((U08*)data)+2)) |= ((U08)((i >> 16) & ((U32)0x000000FF))); break;
				case 0x03:	(*((U32*)data)) |= ((U32)(i & ((U32)0xFFFFFFFF)));	break;
			} return *this; }
		UVL& operator^=(const U32& i){
			switch(length){
				case 0x00:	(*((U08*)data)) ^= ((U08)(i & ((U32)0x000000FF)));	break;
				case 0x01:	(*((U16*)data)) ^= ((U16)(i & ((U32)0x0000FFFF)));	break;
				case 0x02:	(*((U16*)data)) ^= ((U16)(i & ((U32)0x0000FFFF)));
						(*(((U08*)data)+2)) ^= ((U08)((i >> 16) & ((U32)0x000000FF))); break;
				case 0x03:	(*((U32*)data)) ^= ((U32)(i & ((U32)0xFFFFFFFF)));	break;
			} return *this; }
};
#pragma pack(pop)

#define static_safe_array(type, name, ...) \
static type const name##_a[]={__VA_ARGS__}; \
static Vx<type> name(name##_a, name##_a + sizeof(name##_a) / sizeof(*name##_a))

#define safe_array(type, name, ...) \
type const name##_a[]={__VA_ARGS__}; \
Vx<type> name(name##_a, name##_a + sizeof(name##_a) / sizeof(*name##_a))

#define UxDefine(type, class, name, ...) \
static type const class##name##_a[]={__VA_ARGS__}; \
Vx<type> class::name(class##name##_a, class##name##_a + sizeof(class##name##_a) / sizeof(*class##name##_a))

#define sm_empty_def(type, class, name, ...) \
Vx<type> class::name()

#define Ux(type, name) Vx<type> name;

class exception
{
	private:
		TXT details;
	public:
		exception(TXT x){details = x;}
		~exception(){}
		TXT what() const{return details;}
};
class fatalexception
{
	private:
		TXT details;
	public:
		fatalexception(TXT x){details = x;}
		~fatalexception(){}
		TXT what() const{return details;}
};

//Runtime-Checked Try/Catch Tools [PREVENTS UNCAUGHT C++ EXCEPTION BUGS]
extern volatile U32 ___global_throwcheck;
void ___global_IncrementThrowCheck();
void ___global_DecrementThrowCheck();
class ___tryret_glbdecthrowcheck
{
	private:
		BIT ok;
	public:
		void yes(){ok = true;}
		___tryret_glbdecthrowcheck(): ok(false){}
		~___tryret_glbdecthrowcheck(){
			if(!ok) ___global_DecrementThrowCheck();
		}
};
extern void WarnUncheckedCall(STR message);
#define try {BIT ___entered_catch_clause = false; ___global_IncrementThrowCheck(); ___tryret_glbdecthrowcheck ___finished_try_clause; try{
#define catch(exception) }catch(exception){___finished_try_clause.yes(); if(!___entered_catch_clause){___global_DecrementThrowCheck(); ___entered_catch_clause = true;}
#define endtry } ___finished_try_clause.yes(); if(!___entered_catch_clause){___global_DecrementThrowCheck(); ___entered_catch_clause = true;}}
class can_throw_t
{
	public:
		can_throw_t(){}
		~can_throw_t(){
			if(!___global_throwcheck){
                throw fatalexception("Unchecked can_throw_t function call!!");
			}
		}
};
template <typename T>
class can_throw_tt
{
	private:
		T xyz;
	public:
		operator T(){return xyz;}
		can_throw_tt(T xyz): xyz(xyz){}
		can_throw_tt(){}
		~can_throw_tt(){
			if(!___global_throwcheck){
                throw fatalexception("Unchecked can_throw_tt function call!!");
			}
		}
};

///This thing allows constructing an array within a macro statement using [1][2][3] style formatting.
template <typename T> 
class VectorBuilder
{
    public:
		Vx<T> values;
		VectorBuilder& operator[](T value){
			this->values.push_back(value);
			return (*this);
		}
		operator Vx<T>(){
			return values;
		}
		VectorBuilder(){}
		~VectorBuilder(){}
};
