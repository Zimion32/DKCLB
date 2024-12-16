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
#pragma pack(push, 1)

struct DGEID;
struct FILES;
struct DLTID;
struct MUSIC;
struct SOUND;
struct OBJID;
struct LAYER;
struct FTYPE;
struct DSXID;
struct TXTID;
struct PTHID;
class  LUCID;

struct ByteCode
{
    //DEV: should hold a small index offset array and then data bytes.
    //the byte structure must make it past the EXE/DLL boundary.
    //DESIGN: Generally only up to 8 arguments are allowed in the index array.
    //DESIGN: The bytecode is passed to the EXE as a pointer to the struct.
    //        This struct may be a temporary object but should exist for the duration of the call.
    unsigned char data_[4096];
    
    ByteCode();
    ByteCode(NUM a, NUM b);
    
    operator TXT ();
    operator NUM ();
    operator FLO ();
    operator Vx<NUM> ();
    operator Vx<FLO> ();
    operator Vx<DSXID> ();
    operator Vx<OBJID> ();
    operator DGEID ();
    operator FILES ();
    operator DLTID ();
    operator MUSIC ();
    operator SOUND ();
    operator OBJID ();
    operator LAYER ();
    operator FTYPE ();
    operator DSXID ();
    operator TXTID ();
    operator PTHID ();
    operator LUCID ();
    
    //ByteCode Access Functions
    NUM i(NUM index);
    FLO f(NUM index);
    const char* t(NUM index);
    Vx<NUM> ii(NUM index);
    Vx<FLO> ff(NUM index);
    Vx<DSXID> dd(NUM index);
    Vx<OBJID> oo(NUM index);
    
    //ByteCode Return Functions
    ByteCode(TXT data);
    ByteCode(NUM data);
    ByteCode(FLO data);
    ByteCode(Vx<NUM> data);
    ByteCode(Vx<FLO> data);
    ByteCode(DGEID data);
    ByteCode(FILES data);
    ByteCode(DLTID data);
    ByteCode(MUSIC data);
    ByteCode(SOUND data);
    ByteCode(OBJID data);
    ByteCode(LAYER data);
    ByteCode(FTYPE data);
    ByteCode(DSXID data);
    ByteCode(TXTID data);
    ByteCode(PTHID data);
    ByteCode(LUCID data);
    ByteCode(Vx<OBJID> da);
    
    //ByteCode Builder Functions
    ByteCode& operator()();
    ByteCode& operator()(int data);
    ByteCode& operator()(NUM a, NUM b);
    ByteCode& operator()(TXT data);
    ByteCode& operator()(NUM data);
    ByteCode& operator()(FLO data);
    ByteCode& operator()(Vx<NUM> data);
    ByteCode& operator()(Vx<FLO> data);
    ByteCode& operator()(Vx<OBJID> da);
    ByteCode& operator()(DGEID data);
    ByteCode& operator()(FILES data);
    ByteCode& operator()(DLTID data);
    ByteCode& operator()(MUSIC data);
    ByteCode& operator()(SOUND data);
    ByteCode& operator()(OBJID data);
    ByteCode& operator()(LAYER data);
    ByteCode& operator()(FTYPE data);
    ByteCode& operator()(DSXID data);
    ByteCode& operator()(TXTID data);
    ByteCode& operator()(PTHID data);
    ByteCode& operator()(LUCID data);
    ByteCode& operator()(ByteCode& r);
    U32& Table(NUM index);
    U32 Set(U32 where, TXT text);
    U32 Set(U32 where, NUM var);
    U32 Set(U32 where, FLO var);
    U32 Set(U32 where, Vx<NUM> var);
    U32 Set(U32 where, Vx<FLO> var);
    U32 Set(U32 where, Vx<OBJID> var);
    NUM GetA();
    NUM GetB();
    
};
class LUCID
{
	public:
		union{
			struct{
				U08 pData[24];
			};
			struct{
				U32 dw5;
				U32 dw4;
				U32 dw3;
				U32 dw2;
				U32 dw1;
				U32 dw0;
			};
		};
		
		/// BUGFIX: DO NOT ADD operator TXT() AS THIS CAUSES CHANGES IN WHICH OVERLOAD OF A FUNCTION IS CALLED. 
		LUCID(TXT urlbase64);
        LUCID(ByteCode data);
        LUCID& Calculate();
		U64 GetTimeStamp();
		TXT ToString();
		TXT ToHexString();
		BIT IsZero(){
            BIT is_zero = true;
            for(IND d = 0; d < 24; ++d){
                is_zero &= (pData[d] == 0x00);
            }
            return is_zero;
		}
		
		LUCID(){for(IND d = 0; d < 24; ++d){pData[d] = 0x00;}}
		BIT operator==(const LUCID&  rhs) const
		{
			return ((dw5==rhs.dw5)&&(dw4==rhs.dw4)&&(dw3==rhs.dw3)&&(dw2==rhs.dw2)&&(dw1==rhs.dw1)&&(dw0==rhs.dw0));
		}
		BIT operator!=(const LUCID&  rhs) const
		{
			return ((dw5!=rhs.dw5)||(dw4!=rhs.dw4)||(dw3!=rhs.dw3)||(dw2!=rhs.dw2)||(dw1!=rhs.dw1)||(dw0!=rhs.dw0));
		}
		BIT operator>(const LUCID&  rhs) const
		{
			return(((dw5>rhs.dw5))
				|| ((dw5==rhs.dw5)&&(dw4>rhs.dw4))
				|| ((dw5==rhs.dw5)&&(dw4==rhs.dw4)&&(dw3>rhs.dw3))
				|| ((dw5==rhs.dw5)&&(dw4==rhs.dw4)&&(dw3==rhs.dw3)&&(dw2>rhs.dw2))
				|| ((dw5==rhs.dw5)&&(dw4==rhs.dw4)&&(dw3==rhs.dw3)&&(dw2==rhs.dw2)&&(dw1>rhs.dw1))
				|| ((dw5==rhs.dw5)&&(dw4==rhs.dw4)&&(dw3==rhs.dw3)&&(dw2==rhs.dw2)&&(dw1==rhs.dw1)&&(dw0>rhs.dw0)));
		}
		BIT operator<(const LUCID&  rhs) const
		{
			return(((dw5<rhs.dw5))
				|| ((dw5==rhs.dw5)&&(dw4<rhs.dw4))
				|| ((dw5==rhs.dw5)&&(dw4==rhs.dw4)&&(dw3<rhs.dw3))
				|| ((dw5==rhs.dw5)&&(dw4==rhs.dw4)&&(dw3==rhs.dw3)&&(dw2<rhs.dw2))
				|| ((dw5==rhs.dw5)&&(dw4==rhs.dw4)&&(dw3==rhs.dw3)&&(dw2==rhs.dw2)&&(dw1<rhs.dw1))
				|| ((dw5==rhs.dw5)&&(dw4==rhs.dw4)&&(dw3==rhs.dw3)&&(dw2==rhs.dw2)&&(dw1==rhs.dw1)&&(dw0<rhs.dw0)));
		}
		BIT operator>=(const LUCID&  rhs) const
		{
			return(((dw5>rhs.dw5))
				|| ((dw5==rhs.dw5)&&(dw4>rhs.dw4))
				|| ((dw5==rhs.dw5)&&(dw4==rhs.dw4)&&(dw3>rhs.dw3))
				|| ((dw5==rhs.dw5)&&(dw4==rhs.dw4)&&(dw3==rhs.dw3)&&(dw2>rhs.dw2))
				|| ((dw5==rhs.dw5)&&(dw4==rhs.dw4)&&(dw3==rhs.dw3)&&(dw2==rhs.dw2)&&(dw1>rhs.dw1))
				|| ((dw5==rhs.dw5)&&(dw4==rhs.dw4)&&(dw3==rhs.dw3)&&(dw2==rhs.dw2)&&(dw1==rhs.dw1)&&(dw0>=rhs.dw0)));
		}
		BIT operator<=(const LUCID&  rhs) const
		{
			return(((dw5<rhs.dw5))
				|| ((dw5==rhs.dw5)&&(dw4<rhs.dw4))
				|| ((dw5==rhs.dw5)&&(dw4==rhs.dw4)&&(dw3<rhs.dw3))
				|| ((dw5==rhs.dw5)&&(dw4==rhs.dw4)&&(dw3==rhs.dw3)&&(dw2<rhs.dw2))
				|| ((dw5==rhs.dw5)&&(dw4==rhs.dw4)&&(dw3==rhs.dw3)&&(dw2==rhs.dw2)&&(dw1<rhs.dw1))
				|| ((dw5==rhs.dw5)&&(dw4==rhs.dw4)&&(dw3==rhs.dw3)&&(dw2==rhs.dw2)&&(dw1==rhs.dw1)&&(dw0<=rhs.dw0)));
		}
};
#pragma pack(pop)

struct DGEID {
    U32 v;
    operator U32(){return v;}
    DGEID(U32 vv){v = vv;}
    DGEID(){v = 0xFFFFFFFF;}
    DGEID(ByteCode data){v = data.i(0);}
};
struct FILES {
    U32 v;
    operator U32(){return v;}
    FILES(U32 vv){v = vv;}
    FILES(){v = 0xFFFFFFFF;}
    FILES(ByteCode data){v = data.i(0);}
};
struct DLTID {
    U32 v;
    operator U32(){return v;}
    DLTID(U32 vv){v = vv;}
    DLTID(){v = 0xFFFFFFFF;}
    DLTID(ByteCode data){v = data.i(0);}
};
struct MUSIC {
    U32 v;
    operator U32(){return v;}
    MUSIC(U32 vv){v = vv;}
    MUSIC(){v = 0xFFFFFFFF;}
    MUSIC(ByteCode data){v = data.i(0);}
};
struct SOUND {
    U32 v;
    operator U32(){return v;}
    SOUND(U32 vv){v = vv;}
    SOUND(){v = 0xFFFFFFFF;}
    SOUND(ByteCode data){v = data.i(0);}
};
struct OBJID {
    U32 v;
    operator U32(){return v;}
    OBJID(U32 vv){v = vv;}
    OBJID(){v = 0xFFFFFFFF;}
    OBJID(ByteCode data){v = data.i(0);}
};
struct LAYER {
    U32 v;
    operator U32(){return v;}
    LAYER(U32 vv){v = vv;}
    LAYER(){v = 0xFFFFFFFF;}
    LAYER(ByteCode data){v = data.i(0);}
};
struct FTYPE {
    U32 v;
    operator U32(){return v;}
    FTYPE(U32 vv){v = vv;}
    FTYPE(){v = 0xFFFFFFFF;}
    FTYPE(ByteCode data){v = data.i(0);}
};
struct DSXID {
    U32 v;
    operator U32(){return v;}
    DSXID(U32 vv){v = vv;}
    DSXID(){v = 0xFFFFFFFF;}
    DSXID(ByteCode data){v = data.i(0);}
};
struct TXTID {
    U32 v;
    operator U32(){return v;}
    TXTID(U32 vv){v = vv;}
    TXTID(){v = 0xFFFFFFFF;}
    TXTID(ByteCode data){v = data.i(0);}
};
struct PTHID {
    U32 v;
    operator U32(){return v;}
    PTHID(U32 vv){v = vv;}
    PTHID(){v = 0xFFFFFFFF;}
    PTHID(ByteCode data){v = data.i(0);}
};

