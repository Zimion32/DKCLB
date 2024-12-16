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

KDeltaError DeltaError;
KDebug Debug;
KClipboard Clipboard;
KSystem System;
KFileSystem FileSystem;
KUtils Utils;
KString String;
KRegistry Registry;
KConfig INI;

INT TileModulus(INT x, INT tilesize)
{
    if(x >= 0){
        return (x % tilesize);
    }else{
        //Say the tilesize is 100...
        //   100 - (abs(0) % 100)  = 100 //becomes zero
        //   100 - (abs(-1) % 100)  = 99
        //   100 - (abs(-99) % 100) = 1
        //   100 - (abs(-100) % 100) = 100 //becomes zero
        //   100 - (abs(-101) % 100) = 99
        return ((tilesize - (abs(x) % tilesize)) % tilesize);
    }
}
INT InBox(INT xx, INT yy, INT l, INT t, INT r, INT b)
{
    if((xx >= l) && (xx < r) && (yy >= t) && (yy < b)) return true;
    return false;
}
INT InBoxClipped(INT xx, INT yy, INT l, INT t, INT r, INT b, SRect clip)
{
    if((xx >= l) && (xx < r) && (yy >= t) && (yy < b)
    && (xx >= clip.l) && (xx < clip.r) && (yy >= clip.t) && (yy < clip.b)) return true;
    return false;
}
INT BoxOverlap(INT l1, INT t1, INT r1, INT b1, INT l2, INT t2, INT r2, INT b2)
{
    if((l1 < r2) && (r1 > l2) && (t1 < b2) && (b1 > t2)) return true;
    return false;
}
