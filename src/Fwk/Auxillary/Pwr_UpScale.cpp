//Header is graphics.h
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
static F64 DepthInsideUnitCircle(F64 cx, F64 cy, F64 cxscale, F64 cyscale, F64 x, F64 y)
{
	return sqrtf((((x - cx) * (x - cx))/cxscale) + (((y - cy) * (y - cy))/cyscale));
}
static F64 sindeg(F64 angle)
{
    F64 angleradians = angle * M_PI / 180.0f;
    return sin(angleradians) * M_PI / 180.0f;
}
static F64 WieghtFunction(F64 depth) 
{
	if((depth < 0.0) || (depth >= 1.0)) return 0.0;
	return (1.0 - depth);
}
#pragma pack(push, 1)
struct ARGB
{
	union{
		struct{
			U08 b, g, r, a;
		};
		U32 value;
	};
	ARGB(U32 value): value(value){}
	ARGB(F64 a, F64 r, F64 g, F64 b): a(a), r(r), g(g), b(b){}
};
#pragma pack(pop)
void PwrUpScale(BMP src, BMP dst, F64 power)
{
	if(power < 0.0) power = 0.0;
	INT sw = src->w;
	INT sh = src->h;
	INT dw = dst->w;
	INT dh = dst->h;
	F64 _sw = (sw - 1);
	F64 _sh = (sh - 1);
	F64 _dw = (dw - 1);
	F64 _dh = (dh - 1);
	F64 sxi = (1.0 / _sw);
	F64 syi = (1.0 / _sh);
	F64 dxi = (1.0 / _dw);
	F64 dyi = (1.0 / _dh);
	for(IND yy = 0; yy < dh; yy++)
	{
		for(IND xx = 0; xx < dw; xx++)
		{
			F64 dst_px = (dxi * F64(xx));
			F64 dst_py = (dyi * F64(yy));
			INT src_x0 = INT(dst_px / sxi);
			INT src_y0 = INT(dst_py / syi);
			INT src_x1 = src_x0 + 1;
			INT src_y1 = src_y0;
			INT src_x2 = src_x0;
			INT src_y2 = src_y0 + 1;
			INT src_x3 = src_x0 + 1;
			INT src_y3 = src_y0 + 1;
			if(src_x1 >= sw) src_x1 = (sw - 1);
			if(src_x3 >= sw) src_x3 = (sw - 1);
			if(src_y2 >= sh) src_y2 = (sh - 1);
			if(src_y3 >= sh) src_y3 = (sh - 1);
			if(src_x0 < 0) src_x0 = 0;
			if(src_x2 < 0) src_x2 = 0;
			if(src_y0 < 0) src_y0 = 0;
			if(src_y1 < 0) src_y1 = 0;
			F64 src_px0 = (sxi * src_x0);
			F64 src_py0 = (syi * src_y0);
			F64 src_px1 = (sxi * src_x1);
			F64 src_py1 = (syi * src_y1);
			F64 src_px2 = (sxi * src_x2);
			F64 src_py2 = (syi * src_y2);
			F64 src_px3 = (sxi * src_x3);
			F64 src_py3 = (syi * src_y3);
			F64 cd0 = DepthInsideUnitCircle(src_px0, src_py0, sxi, syi, dst_px, dst_py);
			F64 cd1 = DepthInsideUnitCircle(src_px1, src_py1, sxi, syi, dst_px, dst_py);
			F64 cd2 = DepthInsideUnitCircle(src_px2, src_py2, sxi, syi, dst_px, dst_py);
			F64 cd3 = DepthInsideUnitCircle(src_px3, src_py3, sxi, syi, dst_px, dst_py);
			F64 w0 = WieghtFunction(cd0);
			F64 w1 = WieghtFunction(cd1);
			F64 w2 = WieghtFunction(cd2);
			F64 w3 = WieghtFunction(cd3);
			w0 *= pow(w0,power);
			w1 *= pow(w1,power);
			w2 *= pow(w2,power);
			w3 *= pow(w3,power);
			ARGB src_p0(_getpixel32(src, src_x0, src_y0));
			ARGB src_p1(_getpixel32(src, src_x1, src_y1));
			ARGB src_p2(_getpixel32(src, src_x2, src_y2));
			ARGB src_p3(_getpixel32(src, src_x3, src_y3));
			F64 total_wieght = (w0 + w1 + w2 + w3);
			w0 /= total_wieght;
			w1 /= total_wieght;
			w2 /= total_wieght;
			w3 /= total_wieght;
			F64 a = ((w0*src_p0.a)+(w1*src_p1.a)+(w2*src_p2.a)+(w3*src_p3.a));
			F64 r = ((w0*src_p0.r)+(w1*src_p1.r)+(w2*src_p2.r)+(w3*src_p3.r));
			F64 g = ((w0*src_p0.g)+(w1*src_p1.g)+(w2*src_p2.g)+(w3*src_p3.g));
			F64 b = ((w0*src_p0.b)+(w1*src_p1.b)+(w2*src_p2.b)+(w3*src_p3.b));
			ARGB dst_p(a,r,g,b);
			_putpixel32(dst, xx, yy, dst_p.value);
		}
	}
}
