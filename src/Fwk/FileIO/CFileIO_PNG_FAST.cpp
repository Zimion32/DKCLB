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
can_throw_t CFileIO::SavePNG_FAST(BMP bmp, TXT filename)
{
    CFileIO w, x, y, z;
    //magic
    w.SvHex(137,1);
    w.SvHex(80,1);
    w.SvHex(78,1);
    w.SvHex(71,1);
    w.SvHex(13,1);
    w.SvHex(10,1);
    w.SvHex(26,1);
    w.SvHex(10,1);
    //##################################################################################
    //chunk "IHDR"
    x.SvText("IHDR");
    x.SvHex(bmp->w,4);
    x.SvHex(bmp->h,4);
    x.SvHex(8,1);//bpp: 8
    x.SvHex(6,1);//type: 32bit ARGB
    x.SvHex(0,1);//comptype: deflate no compression
    x.SvHex(0,1);//filter: nothing
    x.SvHex(0,1);//INTerlacing: nothing
    x.SvHex(CRC32((U08*)(&(x.Vector()[0])),x.ByteSize()),4);
    //##################################################################################
    //first arrange the rows with starting filter bytes of 0x00 (no filter)
    Vx<U08> r;
    r.reserve(((bmp->w * bmp->h) * 4) + bmp->h);
    for(IND yy = 0; yy < bmp->h; yy++){
        r.push_back(0x00);///"No Filter" Row Prefix
        ///Requires RGBA byte order.
        for(IND xx = 0; xx < bmp->w; xx++){
            U32 in = (((U32*)(bmp->line[yy]))[xx]);
            r.push_back((in >> 16) & 0xFF);
            r.push_back((in >>  8) & 0xFF);
            r.push_back((in >>  0) & 0xFF);
            r.push_back((in >> 24) & 0xFF);
        }
    }
    U32 adler32 = Adler32(((U08*)(&r[0])), r.size());
    //##################################################################################
    //chunk "IDAT"
    y.SvText("IDAT");
    y.SvHex(0x78,1);//This stuff is a DEFLATE master header or something.
    y.SvHex(0x01,1);//This stuff is a DEFLATE master header or something.
    U08* source_ptr = ((U08*)(&r[0]));
    for(IND ds = r.size(); ds >= 0; ds -= 65535){
        if(ds <= 65535){
            ///DEFLATE Chunk Header: MethodByte, Size16, SizeComplemented16
            U32 le_size = (((ds >> 8) & 0x00FF) | ((ds << 8) & 0xFF00));
            y.SvHex(1,1);
            y.SvHex(le_size,2);
            y.SvHex(le_size^0xFFFF,2);
            U32 where = y.GetW();
            y.SvSkipBytes(ds);
            U08* theptr = ((U08*)(&y[0]))+where;
            memcpy(theptr, source_ptr, ds);
            source_ptr += ds;
        }else{
            y.SvHex(0,1);
            y.SvHex(0xFFFF,2);
            y.SvHex(0x0000,2);
            U32 where = y.GetW();
            y.SvSkipBytes(65535);
            U08* theptr = ((U08*)(&y[0]))+where;
            memcpy(theptr, source_ptr, 65535);
            source_ptr += 65535;
        }
    }
    y.SvHex(adler32,4);
    y.SvHex(CRC32((U08*)(&(y.Vector()[0])),y.ByteSize()),4);
    //##################################################################################
    //chunk "IEND"
    z.SvText("IEND");
    z.SvHex(0xAE426082,4);//crc32 of "IEND" text.
    //##################################################################################
    w.SvHex(x.ByteSize()-8,4);
    w.Append(x.Vector());
    w.SvHex(y.ByteSize()-8,4);
    w.Append(y.Vector());
    w.SvHex(z.ByteSize()-8,4);
    w.Append(z.Vector());
    w.Save(filename);
    return can_throw_t();
}
