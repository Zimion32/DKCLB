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
#pragma pack(push, 1)
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
};
struct FILES {
    U32 v;
    operator U32(){return v;}
    FILES(U32 vv){v = vv;}
    FILES(){v = 0xFFFFFFFF;}
};
struct DLTID {
    U32 v;
    operator U32(){return v;}
    DLTID(U32 vv){v = vv;}
    DLTID(){v = 0xFFFFFFFF;}
};
struct MUSIC {
    U32 v;
    operator U32(){return v;}
    MUSIC(U32 vv){v = vv;}
    MUSIC(){v = 0xFFFFFFFF;}
};
struct SOUND {
    U32 v;
    operator U32(){return v;}
    SOUND(U32 vv){v = vv;}
    SOUND(){v = 0xFFFFFFFF;}
};
struct OBJID {
    U32 v;
    operator U32(){return v;}
    OBJID(U32 vv){v = vv;}
    OBJID(){v = 0xFFFFFFFF;}
};
struct LAYER {
    U32 v;
    operator U32(){return v;}
    LAYER(U32 vv){v = vv;}
    LAYER(){v = 0xFFFFFFFF;}
};
struct FTYPE {
    U32 v;
    operator U32(){return v;}
    FTYPE(U32 vv){v = vv;}
    FTYPE(){v = 0xFFFFFFFF;}
};
struct DSXID {
    U32 v;
    operator U32(){return v;}
    DSXID(U32 vv){v = vv;}
    DSXID(){v = 0xFFFFFFFF;}
};
struct TXTID {
    U32 v;
    operator U32(){return v;}
    TXTID(U32 vv){v = vv;}
    TXTID(){v = 0xFFFFFFFF;}
};
struct PTHID {
    U32 v;
    operator U32(){return v;}
    PTHID(U32 vv){v = vv;}
    PTHID(){v = 0xFFFFFFFF;}
};

