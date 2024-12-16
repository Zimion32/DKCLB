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
TXT __ngui_textf(TXT fmt, ...);
#define Tx(...) __ngui_textf(__VA_ARGS__)

void PwrUpScale(BMP src, BMP dst, F64 power = 20.0);
void PixelUpScale(BMP src, BMP dst, U32 transparent);
void PixelUpScaleWithRatio(BMP src, BMP dst, S32 bw, S32 bh, U32 transparent);
SPoint PixelUpScaleEx(BMP src, BMP dst, S32 bw, S32 bh, SRect hitbox, SPoint src_center, SRect src_abounds, S32 src_aborder, U32 transparent, U32 palette);
SPoint PixelUpScaleEx_CalcNewCenter(S32 bx, S32 by, S32 bw, S32 bh, 
    SPoint src_center, SRect src_abounds, S32 src_border, S32 mouse_x, S32 mouse_y);

U32 GeneratePercentageColor(F64 p);
U32 GenerateFpsColor(F64 hz, F64 fps);

BIT PixelIsMagic(U32 pixel);
SRect GetBitmapBounds(BMP input);
SRect GetBitmapBoundsQuick(BMP input);

void PrepareBitmapForFastAlpha(BMP bmp);
void PrepareBitmapForFastAlphaWithColor(BMP bmp, U32 rgbcolor);
