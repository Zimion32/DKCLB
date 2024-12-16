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
	BUILD_BUG_ON(sizeof(FLO) != 8);
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

Vx<DeferredDeleteFn> DeferredDelete::values;
Vx<PTR>              DeferredDelete::pointers;
DeferredDelete       defdel;

BIT SFLine::OneWayCheck(const SFLine& B)
{
    const SFLine& A = *this;
    return ((((A.y1 - A.y0) * (B.x1 - B.x0)) - ((B.y1 - B.y0) * (A.x1 - A.x0))) < 0.0);
}
BIT SFLine::Intersect(SFPoint& I, const SFLine& B, BIT all_edges)
{
    const SFLine& A = *this;
    if((B.ax == B.bx) //B is an infinite vertical line:
    && (fabs(B.ay) == std::numeric_limits<FLO>::infinity()) 
    && (fabs(B.by) == std::numeric_limits<FLO>::infinity())){
        if(A.bx == A.ax){
            //A is horizontal and does NOT intersect
            I.x = 0.0;
            I.y = 0.0;
            I.flags = 0;
            return false;
        }
        FLO am = ((A.by - A.ay) / (A.bx - A.ax));
        FLO ab = A.ay - (A.ax * am);
        I.x = B.ax;
        I.y = (am * B.ax) + ab;
        I.flags = 0;
        return true;
    }
    if((B.ay == B.by) //B is an infinite horizontal line:
    && (fabs(B.ax) == std::numeric_limits<FLO>::infinity()) 
    && (fabs(B.bx) == std::numeric_limits<FLO>::infinity())){
        if(A.bx == A.ax){
            //A is vertical and DOES intersect
            I.x = A.ax;
            I.y = B.ay;
            I.flags = 0;
            return true;
        }
        FLO am = ((A.by - A.ay) / (A.bx - A.ax));
        FLO ab = A.ay - (A.ax * am);
        I.x = ((ab - B.ay) / -am);
        I.y = B.ay;
        I.flags = 0;
        return true;
    }
    FLO dd = ((A.ax - A.bx)*(B.ay - B.by) - (A.ay - A.by)*(B.ax - B.bx));
    if(dd == 0.0){
        //lines are parallel or coincident. handle this special case!!
        if(A.side == B.side){
            //if lines are end-to-end, then they technically still intersect
            if((A.bx == B.ax) && (A.by == B.ay)){
                I.x = A.bx;
                I.y = A.by;
                I.flags |= 2; //continuation happens in positive direction
                return true;
            }
            if((A.ax == B.bx) && (A.ay == B.by)){
                I.x = A.ax;
                I.y = A.ay;
                I.flags |= 1; //continuation happens in negative direction
                return true;
            }
        }
        //else there is no intersection
        I.x = 0.0;
        I.y = 0.0;
        I.flags = 0;
        return false;
    }
    I.x = ((A.ax*A.by - A.ay*A.bx)*(B.ax - B.bx) - (A.ax - A.bx)*(B.ax*B.by - B.ay*B.bx)) / dd;
    I.y = ((A.ax*A.by - A.ay*A.bx)*(B.ay - B.by) - (A.ay - A.by)*(B.ax*B.by - B.ay*B.bx)) / dd;
    I.flags = 0;
    I.side = 0;
    if(A.side == B.side){
        //if same sided lines are end-to-end, this is a special case handler
        if((A.bx == B.ax) && (A.by == B.ay)){
            I.x = A.bx;
            I.y = A.by;
            I.flags |= 2; //continuation happens in positive direction
            return true;
        }
        if((A.ax == B.bx) && (A.ay == B.by)){
            I.x = A.ax;
            I.y = A.ay;
            I.flags |= 1; //continuation happens in negative direction
            return true;
        }
    }
    //now for the main check
    if(((((A.bx >= A.ax) && (I.x >= A.ax) && (I.x <= A.bx))
      || ((A.bx <= A.ax) && (I.x >= A.bx) && (I.x <= A.ax))) &&
        (((A.by >= A.ay) && (I.y >= A.ay) && (I.y <= A.by))
      || ((A.by <= A.ay) && (I.y >= A.by) && (I.y <= A.ay))) &&
        (((B.bx >= B.ax) && (I.x >= B.ax) && (I.x <= B.bx))
      || ((B.bx <= B.ax) && (I.x >= B.bx) && (I.x <= B.ax))) &&
        (((B.by >= B.ay) && (I.y >= B.ay) && (I.y <= B.by))
      || ((B.by <= B.ay) && (I.y >= B.by) && (I.y <= B.ay)))) 
       && (all_edges || !((B.side != A.side) && 
        ( ((A.side == 0) && (B.ay >= I.y) && (B.by >= I.y))
       || ((A.side == 1) && (B.ax <= I.x) && (B.bx <= I.x))
       || ((A.side == 2) && (B.ay <= I.y) && (B.by <= I.y))
       || ((A.side == 3) && (B.ax >= I.x) && (B.bx >= I.x)) )))){
        if(all_edges) return true;
        //when processing traversal intersections we require some extra steps...
        FLO angleA = atan2((A.by - A.ay),(A.bx - A.ax));
        FLO angleB = atan2((B.by - B.ay),(B.bx - B.ax));
        //for traversal towards (A.bx,A.by) if angle C is positive
        //for traversal towards (A.ax,A.ay) if angle C is negative
        FLO angleC = fmod((angleA - angleB), M_PI);
        BIT direction = (angleC > 0.0) ^ (A.side >= 2);
        if(B.side == A.side){
            //continuation on next line
            if(!direction){
                //continuation happens in negative direction
                I.flags |= 1;
            }else{
                //continuation happens in positive direction
                I.flags |= 2;
            }
        }else{
            //stop point on line intersection
            if(!direction){
                //stop happens in negative direction
                I.flags |= 4;
            }else{
                //stop happens in positive direction
                I.flags |= 8;
            }
        }
        return true;
    }
    I.x = 0.0;
    I.y = 0.0;
    return false;
}
INT SFLine::Tolerance(SFPoint& I, const SFLine& B, FLO tolerance)
{
    const FLO e = (1 / 256.0);
    I.x = 0.0;
    I.y = 0.0;
    I.flags = 0;
    if(side != B.side) return 0;
    switch(side){
        case 0:{
            if((fabs(B.bx - ax) <= e) && (fabs(B.by - ay) <= tolerance)){
                I.flags |= 1;
                I.x = ax;
                I.y = ay;
                return -1;
            }
            if((fabs(B.ax - bx) <= e) && (fabs(B.ay - by) <= tolerance)){
                I.flags |= 2;
                I.x = bx;
                I.y = by;
                return +1;
            }
        }break;
        case 1:{
            if((fabs(B.by - ay) <= e) && (fabs(B.bx - ax) <= tolerance)){
                I.flags |= 1;
                I.x = ax;
                I.y = ay;
                return -1;
            }
            if((fabs(B.ay - by) <= e) && (fabs(B.ax - bx) <= tolerance)){
                I.flags |= 2;
                I.x = bx;
                I.y = by;
                return +1;
            }
        }break;
        case 2:{
            if((fabs(B.ax - bx) <= e) && (fabs(B.ay - by) <= tolerance)){
                I.flags |= 1;
                I.x = bx;
                I.y = by;
                return -1;
            }
            if((fabs(B.bx - ax) <= e) && (fabs(B.by - ay) <= tolerance)){
                I.flags |= 2;
                I.x = ax;
                I.y = ay;
                return +1;
            }
        }break;
        case 3:{
            if((fabs(B.ay - by) <= e) && (fabs(B.ax - bx) <= tolerance)){
                I.flags |= 1;
                I.x = bx;
                I.y = by;
                return -1;
            }
            if((fabs(B.by - ay) <= e) && (fabs(B.bx - ax) <= tolerance)){
                I.flags |= 2;
                I.x = ax;
                I.y = ay;
                return +1;
            }
        }break;
    }
    return 0;
}
INT SFLine::CalcSide(INT wall)
{
    flags = (wall & 1);
    if(!wall && (ax <  bx)) side = 0;
    if( wall && (ay <  by)) side = 1;
    if(!wall && (ax >= bx)) side = 2;
    if( wall && (ay >= by)) side = 3;
    return side;
}
SFPoint::operator SPoint()
{
    return SPoint((*this).x+0.5, (*this).y+0.5);
}
SPoint::operator SFPoint()
{
    return SFPoint((*this).x, (*this).y);
}
SFRect::operator SRect()
{
    return SRect(floor((*this).l), floor((*this).t), ceil((*this).r), ceil((*this).b));
}
SRect::operator SFRect()
{
    return SFRect((*this).l, (*this).t, (*this).r, (*this).b);
}
SPoint::SPoint(const SFPoint& other)
{
    x = other.x+0.5;
    y = other.y+0.5;
}
SPoint::SPoint(SFPoint* other)
{
    x = other->x+0.5;
    y = other->y+0.5;
}
SFPoint::SFPoint(const SPoint& other)
{
    x = other.x;
    y = other.y;
}
SFPoint::SFPoint(SPoint* other)
{
    x = other->x;
    y = other->y;
}
SRect::SRect(const SFRect& other)
{
    l = floor(other.l);
    t = floor(other.t);
    r = ceil(other.r);
    b = ceil(other.b);
}
SRect::SRect(SFRect* other)
{
    l = floor(other->l);
    t = floor(other->t);
    r = ceil(other->r);
    b = ceil(other->b);
}
SFRect::SFRect(const SRect& other)
{
    l = other.l;
    t = other.t;
    r = other.r;
    b = other.b;
}
SFRect::SFRect(SRect* other)
{
    l = other->l;
    t = other->t;
    r = other->r;
    b = other->b;
}
SPoint& SPoint::ScaleByInverseOf(SPoint scale){
    if(abs(scale.x) != 0) x /= scale.x; else x = 0x7FFFFFFF;
    if(abs(scale.y) != 0) y /= scale.y; else y = 0x7FFFFFFF;
    return *this;
}
SRect& SRect::ScaleByInverseOf(SPoint scale){
    if(abs(scale.x) != 0) l /= scale.x; else l = 0x7FFFFFFF;
    if(abs(scale.y) != 0) t /= scale.y; else t = 0x7FFFFFFF;
    if(abs(scale.x) != 0) r /= scale.x; else r = 0x7FFFFFFF;
    if(abs(scale.y) != 0) b /= scale.y; else b = 0x7FFFFFFF;
    return *this;
}
SFPoint& SFPoint::ScaleByInverseOf(SFPoint scale){
    if(fabs(scale.x) < std::numeric_limits<F64>::epsilon()) scale.x = std::numeric_limits<F64>::epsilon();
    if(fabs(scale.y) < std::numeric_limits<F64>::epsilon()) scale.y = std::numeric_limits<F64>::epsilon();
    x /= scale.x;
    y /= scale.y;
    return *this;
}
SFRect& SFRect::ScaleByInverseOf(SFPoint scale){
    if(fabs(scale.x) < std::numeric_limits<F64>::epsilon()) scale.x = std::numeric_limits<F64>::epsilon();
    if(fabs(scale.y) < std::numeric_limits<F64>::epsilon()) scale.y = std::numeric_limits<F64>::epsilon();
    l /= scale.x;
    t /= scale.y;
    r /= scale.x;
    b /= scale.y;
    return *this;
}
