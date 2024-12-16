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

static U32 BtwColPct(U32 start, U32 end, F64 percent_fromstart)
{
	F64 blu1 = F64(start & 0xFF);
	F64 grn1 = F64((start >> 8) & 0xFF);
	F64 red1 = F64((start >> 16) & 0xFF);
	F64 blu2 = F64(end & 0xFF);
	F64 grn2 = F64((end >> 8) & 0xFF);
	F64 red2 = F64((end >> 16) & 0xFF);
	F64 redV, grnV, bluV;
	if(red2 >= red1){
		redV = red1 + ((red2 - red1) * 0.01 * percent_fromstart);
	}else{
		redV = red2 + ((red1 - red2) * 0.01 * (100.0 - percent_fromstart));
	}
	if(grn2 >= grn1){
		grnV = grn1 + ((grn2 - grn1) * 0.01 * percent_fromstart);
	}else{
		grnV = grn2 + ((grn1 - grn2) * 0.01 * (100.0 - percent_fromstart));
	}
	if(blu2 >= blu1){
		bluV = blu1 + ((blu2 - blu1) * 0.01 * percent_fromstart);
	}else{
		bluV = blu2 + ((blu1 - blu2) * 0.01 * (100.0 - percent_fromstart));
	}
	return (0xFF000000 | ((INT(redV) & 0xFF) << 16) | ((INT(grnV) & 0xFF) << 8) | (INT(bluV) & 0xFF));
}
U32 GeneratePercentageColor(F64 p)
{
	static U32 c[13] = {
		0xFF003EFF, //00-05
		0xFF0094FF, //05-10
		0xFF00FFFF, //10-20
		0xFF00FF90, //20-30
		0xFF00FF21, //30-40
		0xFF4CFF00, //40-50
		0xFFB6FF00, //50-60
		0xFFFFD800, //60-70
		0xFFFF6A00, //70-80
		0xFFFF2F00, //80-90
		0xFFFF0000, //90-95
		0xFFFF006E, //95-99
		0xFFA71F00};//100+
	if((p >= 100.0)              ) return c[12];
	if(               (p <   5.0)) return BtwColPct(c[0], c[1], (p           * 20.0));
	if((p >=  5.0) && (p <  10.0)) return BtwColPct(c[1], c[2], ((p -   5.0) * 20.0));
	if((p >= 10.0) && (p <  20.0)) return BtwColPct(c[2], c[3], ((p -  10.0) * 10.0));
	if((p >= 20.0) && (p <  30.0)) return BtwColPct(c[3], c[4], ((p -  20.0) * 10.0));
	if((p >= 30.0) && (p <  40.0)) return BtwColPct(c[4], c[5], ((p -  30.0) * 10.0));
	if((p >= 40.0) && (p <  50.0)) return BtwColPct(c[5], c[6], ((p -  40.0) * 10.0));
	if((p >= 50.0) && (p <  60.0)) return BtwColPct(c[6], c[7], ((p -  50.0) * 10.0));
	if((p >= 60.0) && (p <  70.0)) return BtwColPct(c[7], c[8], ((p -  60.0) * 10.0));
	if((p >= 70.0) && (p <  80.0)) return BtwColPct(c[8], c[9], ((p -  70.0) * 10.0));
	if((p >= 80.0) && (p <  90.0)) return BtwColPct(c[9], c[10],((p -  80.0) * 10.0));
	if((p >= 90.0) && (p <  95.0)) return BtwColPct(c[10],c[11],((p -  90.0) * 20.0));
	if((p >= 95.0) && (p < 100.0)) return BtwColPct(c[11],0xFFFD80B5,((p -  95.0) * 20.0));
	return 0xFFFFFFFF;
}
U32 GenerateFpsColor(F64 hz, F64 fps)
{
    static U32 c[5] = {
		0xFF4CFF00, //full fps
		0xFFFFD800, //half
		0xFFFF6A00, //third
		0xFFFF0000, //fourth
		0xFF7F0000};//fifth+
    if(hz <= 0.0) hz = 60.0;
    F64 f = (fps / hz);
    if((f >= 1.0) || (hz == fps)) return 0xFF003EFF;
    if((f < (      1.0)) && (f >= (1.0 / 2.0))) return BtwColPct(c[0], c[1], ((f - (1.0 / 2.0)) / ((1.0      ) - (1.0 / 2.0))) * 100.0);
    if((f < (1.0 / 2.0)) && (f >= (1.0 / 3.0))) return BtwColPct(c[1], c[2], ((f - (1.0 / 3.0)) / ((1.0 / 2.0) - (1.0 / 3.0))) * 100.0);
    if((f < (1.0 / 3.0)) && (f >= (1.0 / 4.0))) return BtwColPct(c[2], c[3], ((f - (1.0 / 4.0)) / ((1.0 / 3.0) - (1.0 / 4.0))) * 100.0);
    if((f < (1.0 / 4.0)) && (f >= (1.0 / 5.0))) return BtwColPct(c[3], c[4], ((f - (1.0 / 5.0)) / ((1.0 / 4.0) - (1.0 / 5.0))) * 100.0);
    if(f < (1.0 / 5.0)) return c[4];
    return 0xFFFF0000;
}
BIT PixelIsMagic(U32 pixel)
{
    return (((pixel & 0xFF000000) == 0) || ((pixel & 0x00FFFFFF) == 0x00FF00FF));
}
SRect GetBitmapBounds(BMP input)
{
    INT l = 0, t = 0, r = (input->w - 1), b = (input->h - 1);
    BIT exitloop = false;
    for(; l < (input->w-1); l++){
        for(IND yy = 0; yy < input->h; yy++){
            U32 pixel = _getpixel32(input, l, yy);
            if(!PixelIsMagic(pixel)){
                exitloop = true;
            }
            if(exitloop) break;
        }
        if(exitloop) break;
    }
    exitloop = false;
    for(; r > 0; r--){
        for(IND yy = 0; yy < input->h; yy++){
            U32 pixel = _getpixel32(input, r, yy);
            if(!PixelIsMagic(pixel)){
                exitloop = true;
            }
            if(exitloop) break;
        }
        if(exitloop) break;
    }
    exitloop = false;
    for(; t < (input->h-1); t++){
        for(IND xx = 0; xx < input->w; xx++){
            U32 pixel = _getpixel32(input, xx, t);
            if(!PixelIsMagic(pixel)){
                exitloop = true;
            }
            if(exitloop) break;
        }
        if(exitloop) break;
    }
    exitloop = false;
    for(; b > 0; b--){
        for(IND xx = 0; xx < input->w; xx++){
            U32 pixel = _getpixel32(input, xx, b);
            if(!PixelIsMagic(pixel)){
                exitloop = true;
            }
            if(exitloop) break;
        }
        if(exitloop) break;
    }
    if((r <= l) || (b <= t)) return SRect(0, 0, 0, 0);
    return SRect(l, t, r+1, b+1);
}
SRect GetBitmapBoundsQuick(BMP input)
{
    INT l = 0, t = 0, r = (input->w - 1), b = (input->h - 1);
    if((r <= l) || (b <= t)) return SRect(0, 0, 0, 0);
    return SRect(l, t, r+1, b+1);
}
void PrepareBitmapForFastAlpha(BMP bmp)
{
    for(IND yy = 0; yy < bmp->h; yy++){
        for(IND xx = 0; xx < bmp->w; xx++){
            U32 color = _getpixel32(bmp, xx, yy);
            U32 a = ((color >> 24) & 0xFF);
            U32 r = ((color >> 16) & 0xFF);
            U32 g = ((color >>  8) & 0xFF);
            U32 b = ((color >>  0) & 0xFF);
            r = ((r * (a+((a>>7)&1))) >> 8);
            g = ((g * (a+((a>>7)&1))) >> 8);
            b = ((b * (a+((a>>7)&1))) >> 8);
            a = ((a ^ 0xFF) >> 1);
            if(a == 0x7F) a = 0x7E;
            U32 rr = ((a << 24)|(r << 16)|(g << 8)|b);
            if(((color & 0x00FFFFFF) == 0x00FF00FF) 
            || ((color & 0xFF000000) == 0x00000000)){
                rr = 0x7E000000;
            }
            _putpixel32(bmp, xx, yy, rr);
        }
    }
}
void PrepareBitmapForFastAlphaWithColor(BMP bmp, U32 rgbcolor)
{
    for(IND yy = 0; yy < bmp->h; yy++){
        for(IND xx = 0; xx < bmp->w; xx++){
            U32 color = _getpixel32(bmp, xx, yy);
            color = ((color & 0xFF000000) | (rgbcolor & 0x00FFFFFF));
            U32 a = ((color >> 24) & 0xFF);
            U32 r = ((color >> 16) & 0xFF);
            U32 g = ((color >>  8) & 0xFF);
            U32 b = ((color >>  0) & 0xFF);
            r = ((r * (a+((a>>7)&1))) >> 8);
            g = ((g * (a+((a>>7)&1))) >> 8);
            b = ((b * (a+((a>>7)&1))) >> 8);
            a = ((a ^ 0xFF) >> 1);
            if(a == 0x7F) a = 0x7E;
            U32 rr = ((a << 24)|(r << 16)|(g << 8)|b);
            if(((color & 0x00FFFFFF) == 0x00FF00FF) 
            || ((color & 0xFF000000) == 0x00000000)){
                rr = 0x7E000000;
            }
            _putpixel32(bmp, xx, yy, rr);
        }
    }
}
static U32 MultiplyColors2(U32 color0, U32 color1, U32 mult0)
{
    //WARNING: Extremely sensitive operation. 
    //Loss of any precision can cause "griding".
    U32 a0 = ((color0 >> 24) & 0xFF);
    U32 r0 = ((color0 >> 16) & 0xFF);
    U32 g0 = ((color0 >>  8) & 0xFF);
    U32 b0 = ((color0 >>  0) & 0xFF);
    U32 a1 = ((color1 >> 24) & 0xFF);
    U32 r1 = ((color1 >> 16) & 0xFF);
    U32 g1 = ((color1 >>  8) & 0xFF);
    U32 b1 = ((color1 >>  0) & 0xFF);
    mult0    += ((mult0 >> 7) & 1);
    U32 mult1 = (256 - mult0);
    a0 = (((a0 * mult0) + (a1 * mult1)) >> 8);
    r0 = (((r0 * mult0) + (r1 * mult1)) >> 8);
    g0 = (((g0 * mult0) + (g1 * mult1)) >> 8);
    b0 = (((b0 * mult0) + (b1 * mult1)) >> 8);
    if(a0 > 0xFF) a0 = 0xFF;
    if(r0 > 0xFF) r0 = 0xFF;
    if(g0 > 0xFF) g0 = 0xFF;
    if(b0 > 0xFF) b0 = 0xFF;
    return ((a0 << 24)|(r0 << 16)|(g0 << 8)|b0);
}
void PixelUpScale(BMP src, BMP dst, U32 transparent)
{
    F64 ww_src = (1.0 / F64(src->w));
    F64 hh_src = (1.0 / F64(src->h));
    F64 ww_dst = (1.0 / F64(dst->w));
    F64 hh_dst = (1.0 / F64(dst->h));
    for(IND yy = 0; yy < dst->h; yy++){
        for(IND xx = 0; xx < dst->w; xx++){
            F64 fxx0 = (F64(xx) * ww_dst);
            F64 fyy0 = (F64(yy) * hh_dst);
            F64 fxx1 = (F64(xx+1) * ww_dst);
            F64 fyy1 = (F64(yy+1) * hh_dst);
            IND uu = INT((fxx0 / ww_src) + 0.001);
            IND vv = INT((fyy0 / hh_src) + 0.001);
            if(uu < 0) uu = 0;
            if(vv < 0) vv = 0;
            if(uu >= src->w) uu = (src->w - 1);
            if(vv >= src->h) vv = (src->h - 1);
            F64 fuu1 = (F64(uu+1) * ww_src);
            F64 fvv1 = (F64(vv+1) * hh_src);
            F64 fxx_lerp = 0.0;
            F64 fyy_lerp = 0.0;
            if((fuu1 >= fxx0) && (fuu1 < fxx1)) fxx_lerp = ((fxx1 - fuu1) / ww_dst);
            if((fvv1 >= fyy0) && (fvv1 < fyy1)) fyy_lerp = ((fyy1 - fvv1) / hh_dst);
            INT xx_lerp = INT(fxx_lerp * 255.0);
            INT yy_lerp = INT(fyy_lerp * 255.0);
            if(xx_lerp < 0) xx_lerp = 0;
            if(yy_lerp < 0) yy_lerp = 0;
            if(xx_lerp >= 256) xx_lerp = 255;
            if(yy_lerp >= 256) yy_lerp = 255;
            IND uu1 = uu+1;
            IND vv1 = vv+1;
            if(uu1 >= src->w) uu1 = (src->w - 1);
            if(vv1 >= src->h) vv1 = (src->h - 1);
            U32 color0 = _getpixel32(src, uu , vv );
            U32 color1 = _getpixel32(src, uu1, vv );
            U32 color2 = _getpixel32(src, uu , vv1);
            U32 color3 = _getpixel32(src, uu1, vv1);
            if(!(color0 & 0xFF000000)||((color0 & 0x00FFFFFF)==0x00FF00FF)) color0 = transparent;
            if(!(color1 & 0xFF000000)||((color1 & 0x00FFFFFF)==0x00FF00FF)) color1 = transparent;
            if(!(color2 & 0xFF000000)||((color2 & 0x00FFFFFF)==0x00FF00FF)) color2 = transparent;
            if(!(color3 & 0xFF000000)||((color3 & 0x00FFFFFF)==0x00FF00FF)) color3 = transparent;
            U32 top = MultiplyColors2(color0, color1, xx_lerp^0xFF);
            U32 btm = MultiplyColors2(color2, color3, xx_lerp^0xFF);
            U32 res = MultiplyColors2(top,    btm,    yy_lerp^0xFF);
            _putpixel32(dst, xx, yy, res);
        }
    }
}
void PixelUpScaleWithRatio(BMP src, BMP dst, S32 bw, S32 bh, U32 transparent)
{
    if(bw <= 0) return;
    if(bh <= 0) return;
    F64 w_given_h = F64(bh)*(F64(src->w)/F64(src->h));
    F64 h_given_w = F64(bw)*(F64(src->h)/F64(src->w));
    F64 use_w, use_h;
    if(w_given_h < bw){
        use_w = w_given_h;
        use_h = F64(bh);
    }else{
        use_w = F64(bw);
        use_h = h_given_w;
    }
    INT final_w = use_w;
    INT final_h = use_h;
    INT x_offs = ((bw - final_w) >> 1);
    INT y_offs = ((bh - final_h) >> 1);
    if(x_offs < 0) x_offs = 0;
    if(y_offs < 0) y_offs = 0;
    //----------------------------
    F64 ww_src = (1.0 / F64(src->w));
    F64 hh_src = (1.0 / F64(src->h));
    F64 ww_dst = (1.0 / use_w);
    F64 hh_dst = (1.0 / use_h);
    for(IND yy = 0; yy < final_h; yy++){
        for(IND xx = 0; xx < final_w; xx++){
            F64 fxx0 = (F64(xx) * ww_dst);
            F64 fyy0 = (F64(yy) * hh_dst);
            F64 fxx1 = (F64(xx+1) * ww_dst);
            F64 fyy1 = (F64(yy+1) * hh_dst);
            IND uu = INT((fxx0 / ww_src) + 0.001);
            IND vv = INT((fyy0 / hh_src) + 0.001);
            if(uu < 0) uu = 0;
            if(vv < 0) vv = 0;
            if(uu >= src->w) uu = (src->w - 1);
            if(vv >= src->h) vv = (src->h - 1);
            F64 fuu1 = (F64(uu+1) * ww_src);
            F64 fvv1 = (F64(vv+1) * hh_src);
            F64 fxx_lerp = 0.0;
            F64 fyy_lerp = 0.0;
            if((fuu1 >= fxx0) && (fuu1 < fxx1)) fxx_lerp = ((fxx1 - fuu1) / ww_dst);
            if((fvv1 >= fyy0) && (fvv1 < fyy1)) fyy_lerp = ((fyy1 - fvv1) / hh_dst);
            INT xx_lerp = INT(fxx_lerp * 255.0);
            INT yy_lerp = INT(fyy_lerp * 255.0);
            if(xx_lerp < 0) xx_lerp = 0;
            if(yy_lerp < 0) yy_lerp = 0;
            if(xx_lerp >= 256) xx_lerp = 255;
            if(yy_lerp >= 256) yy_lerp = 255;
            IND uu1 = uu+1;
            IND vv1 = vv+1;
            if(uu1 >= src->w) uu1 = (src->w - 1);
            if(vv1 >= src->h) vv1 = (src->h - 1);
            U32 color0 = _getpixel32(src, uu , vv );
            U32 color1 = _getpixel32(src, uu1, vv );
            U32 color2 = _getpixel32(src, uu , vv1);
            U32 color3 = _getpixel32(src, uu1, vv1);
            if(!(color0 & 0xFF000000)||((color0 & 0x00FFFFFF)==0x00FF00FF)) color0 = transparent;
            if(!(color1 & 0xFF000000)||((color1 & 0x00FFFFFF)==0x00FF00FF)) color1 = transparent;
            if(!(color2 & 0xFF000000)||((color2 & 0x00FFFFFF)==0x00FF00FF)) color2 = transparent;
            if(!(color3 & 0xFF000000)||((color3 & 0x00FFFFFF)==0x00FF00FF)) color3 = transparent;
            U32 top = MultiplyColors2(color0, color1, xx_lerp^0xFF);
            U32 btm = MultiplyColors2(color2, color3, xx_lerp^0xFF);
            U32 res = MultiplyColors2(top,    btm,    yy_lerp^0xFF);
            _putpixel32(dst, x_offs+xx, y_offs+yy, res);
        }
    }
}
U32 GetUnboundedPixel(BMP src, S32 x, S32 y, U32 palette)
{
    if(x <       0) return 0x00FF00FF;
    if(x >= src->w) return 0x00FF00FF;
    if(y <       0) return 0x00FF00FF;
    if(y >= src->h) return 0x00FF00FF;
    U32 color = _getpixel32(src, x, y);
    //BUGFIX: The color can come back with zero alpha and any arbitrary color, or even 0x00FF00FF.
    if(!(color & 0xFF000000)||((color & 0x00FFFFFF)==0x00FF00FF)) return 0x00FF00FF;
    //Now for the real palette lookup.
    if(palette) return ((color & 0xFF000000) | (((U32*)palette)[color & 0xFFFF]));
    return color;
}

static BIT HB(S32 ll, S32 rr, S32 tt, S32 xx, S32 yy)
{
    return ((xx >= ll) && (xx <= rr) && (yy == tt));
}
static BIT HB2(S32 xx, S32 yy, S32 ll, S32 tt, S32 rr, S32 bb)
{
    return HB(ll, rr, tt, xx, yy) 
        || HB(ll, rr, bb, xx, yy) 
        || HB(tt, bb, ll, yy, xx) 
        || HB(tt, bb, rr, yy, xx);
}
SPoint PixelUpScaleEx(BMP src, BMP dst, S32 bw, S32 bh, SRect hitbox,
    SPoint src_center, SRect src_abounds, S32 src_border, U32 transparent, U32 palette)
{
    //First adjust the bounds according to the center position at 0,0
    if(src_abounds.l > 0) src_abounds.l = 0;
    if(src_abounds.t > 0) src_abounds.t = 0;
    if(src_abounds.r < 0) src_abounds.r = 0;
    if(src_abounds.b < 0) src_abounds.b = 0;
    //Now add a border around this new bounding rectangle
    src_abounds.l -= src_border;
    src_abounds.t -= src_border;
    src_abounds.r += src_border;
    src_abounds.b += src_border;
    //Calculate the offsets required to process the source bitmap
    S32 gfx_dx =  ((-src_abounds.l) - src_center.x);
    S32 gfx_dy =  ((-src_abounds.t) - src_center.y);
    S32 src_dw =  ((-src_abounds.l) + src_abounds.r);
    S32 src_dh =  ((-src_abounds.t) + src_abounds.b);
    //Do the ratio calculation and generate offsets.
    if(bw <= 0) return SPoint(0,0);
    if(bh <= 0) return SPoint(0,0);
    F64 w_given_h = F64(bh)*(F64(src_dw)/F64(src_dh));
    F64 h_given_w = F64(bw)*(F64(src_dh)/F64(src_dw));
    F64 use_w, use_h;
    if(w_given_h < bw){
        use_w = w_given_h;
        use_h = F64(bh);
    }else{
        use_w = F64(bw);
        use_h = h_given_w;
    }
    INT final_w = use_w;
    INT final_h = use_h;
    INT x_offs = ((bw - final_w) >> 1);
    INT y_offs = ((bh - final_h) >> 1);
    if(x_offs < 0) x_offs = 0;
    if(y_offs < 0) y_offs = 0;
    //----------------------------
    S32 px = (x_offs + S32((((F64)final_w) / ((F64)src_dw)) * ((F64)(-src_abounds.l))));
    S32 py = (y_offs + S32((((F64)final_h) / ((F64)src_dh)) * ((F64)(-src_abounds.t))));
    //----------------------------
    F64 ww_src = (1.0 / F64(src_dw));
    F64 hh_src = (1.0 / F64(src_dh));
    F64 ww_dst = (1.0 / use_w);
    F64 hh_dst = (1.0 / use_h);
    for(IND yy = 0; yy < final_h; yy++){
        for(IND xx = 0; xx < final_w; xx++){
            F64 fxx0 = (F64(xx) * ww_dst);
            F64 fyy0 = (F64(yy) * hh_dst);
            F64 fxx1 = (F64(xx+1) * ww_dst);
            F64 fyy1 = (F64(yy+1) * hh_dst);
            IND uu = INT((fxx0 / ww_src) + 0.001);
            IND vv = INT((fyy0 / hh_src) + 0.001);
            F64 fuu1 = (F64(uu+1) * ww_src);
            F64 fvv1 = (F64(vv+1) * hh_src);
            F64 fxx_lerp = 0.0;
            F64 fyy_lerp = 0.0;
            if((fuu1 >= fxx0) && (fuu1 < fxx1)) fxx_lerp = ((fxx1 - fuu1) / ww_dst);
            if((fvv1 >= fyy0) && (fvv1 < fyy1)) fyy_lerp = ((fyy1 - fvv1) / hh_dst);
            INT xx_lerp = INT(fxx_lerp * 255.0);
            INT yy_lerp = INT(fyy_lerp * 255.0);
            if(xx_lerp < 0) xx_lerp = 0;
            if(yy_lerp < 0) yy_lerp = 0;
            if(xx_lerp >= 256) xx_lerp = 255;
            if(yy_lerp >= 256) yy_lerp = 255;
            IND uu1 = uu+1;
            IND vv1 = vv+1;
            U32 color0 = GetUnboundedPixel(src, uu  - gfx_dx, vv  - gfx_dy, palette);
            U32 color1 = GetUnboundedPixel(src, uu1 - gfx_dx, vv  - gfx_dy, palette);
            U32 color2 = GetUnboundedPixel(src, uu  - gfx_dx, vv1 - gfx_dy, palette);
            U32 color3 = GetUnboundedPixel(src, uu1 - gfx_dx, vv1 - gfx_dy, palette);
            if(!(color0 & 0xFF000000)||((color0 & 0x00FFFFFF)==0x00FF00FF)) color0 = transparent;
            if(!(color1 & 0xFF000000)||((color1 & 0x00FFFFFF)==0x00FF00FF)) color1 = transparent;
            if(!(color2 & 0xFF000000)||((color2 & 0x00FFFFFF)==0x00FF00FF)) color2 = transparent;
            if(!(color3 & 0xFF000000)||((color3 & 0x00FFFFFF)==0x00FF00FF)) color3 = transparent;
            if(HB2(uu  - gfx_dx, vv  - gfx_dy, hitbox.l, hitbox.t, hitbox.r, hitbox.b)) color0 = 0xFFFFFFFF;
            if(HB2(uu1  - gfx_dx, vv  - gfx_dy, hitbox.l, hitbox.t, hitbox.r, hitbox.b)) color1 = 0xFFFFFFFF;
            if(HB2(uu  - gfx_dx, vv1  - gfx_dy, hitbox.l, hitbox.t, hitbox.r, hitbox.b)) color2 = 0xFFFFFFFF;
            if(HB2(uu1  - gfx_dx, vv1  - gfx_dy, hitbox.l, hitbox.t, hitbox.r, hitbox.b)) color3 = 0xFFFFFFFF;
            U32 top = MultiplyColors2(color0, color1, xx_lerp^0xFF);
            U32 btm = MultiplyColors2(color2, color3, xx_lerp^0xFF);
            U32 res = MultiplyColors2(top,    btm,    yy_lerp^0xFF);
            _putpixel32(dst, x_offs+xx, y_offs+yy, res);
        }
    }
    return SPoint(px,py);
}
SPoint PixelUpScaleEx_CalcNewCenter(S32 bx, S32 by, S32 bw, S32 bh, 
    SPoint src_center, SRect src_abounds, S32 src_border, S32 mouse_x, S32 mouse_y)
{
    //First adjust the bounds according to the center position at 0,0
    if(src_abounds.l > 0) src_abounds.l = 0;
    if(src_abounds.t > 0) src_abounds.t = 0;
    if(src_abounds.r < 0) src_abounds.r = 0;
    if(src_abounds.b < 0) src_abounds.b = 0;
    //Now add a border around this new bounding rectangle
    src_abounds.l -= src_border;
    src_abounds.t -= src_border;
    src_abounds.r += src_border;
    src_abounds.b += src_border;
    //Calculate the offsets required to process the source bitmap
    S32 src_dw =  ((-src_abounds.l) + src_abounds.r);
    S32 src_dh =  ((-src_abounds.t) + src_abounds.b);
    //Do the ratio calculation and generate offsets.
    if(bw <= 0) return SPoint(0,0);
    if(bh <= 0) return SPoint(0,0);
    F64 w_given_h = F64(bh)*(F64(src_dw)/F64(src_dh));
    F64 h_given_w = F64(bw)*(F64(src_dh)/F64(src_dw));
    F64 use_w, use_h;
    if(w_given_h < bw){
        use_w = w_given_h;
        use_h = F64(bh);
    }else{
        use_w = F64(bw);
        use_h = h_given_w;
    }
    INT final_w = use_w;
    INT final_h = use_h;
    INT x_offs = ((bw - final_w) >> 1);
    INT y_offs = ((bh - final_h) >> 1);
    if(x_offs < 0) x_offs = 0;
    if(y_offs < 0) y_offs = 0;
    //----------------------------
    mouse_x -= bx;
    mouse_y -= by;
    mouse_x -= x_offs;
    mouse_y -= y_offs;
    S32 px = (((S32)((((F64)mouse_x) / (((F64)final_w) / ((F64)src_dw))) + 0.5)) - (-src_abounds.l) + src_center.x);
    S32 py = (((S32)((((F64)mouse_y) / (((F64)final_h) / ((F64)src_dh))) + 0.5)) - (-src_abounds.t) + src_center.y);
    return SPoint(px,py);
}






