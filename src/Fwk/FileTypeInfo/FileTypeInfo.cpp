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
#include "include.h"
#include "Shellapi.h"
#include "math.h"

// In the following block, the (required!!) pragmas force correct data alignment.
#pragma pack( push, 1 ) 
typedef struct
{
    BYTE        		bWidth;			// Width, in pixels, of the image
    BYTE        		bHeight;		// Height, in pixels, of the image
    BYTE        		bColorCount;	// Number of colors in image (0 if >=8bpp)
    BYTE        		bReserved;		// Reserved ( must be 0)
    WORD        		wPlanes;		// Color Planes
    WORD        		wBitCount;		// Bits per pixel
    DWORD       		dwBytesInRes;	// How many bytes in this resource?
    DWORD       		dwImageOffset;	// Where in the file is this image?
} ICONDIRENTRY, *LPICONDIRENTRY;
typedef struct
{
    WORD           		idReserved;		// Reserved (must be 0)
    WORD           		idType;			// Resource Type (1 for icons)
    WORD           		idCount;		// How many images?
    ICONDIRENTRY   		idEntries[1];	// An entry for each image (idCount of 'em)
} ICONDIR, *LPICONDIR;
typedef struct
{
   BITMAPINFOHEADER   	icHeader;		// DIB header
   RGBQUAD         		icColors[1];	// Color table
   BYTE            		icXOR[1];		// DIB bits for XOR mask
   BYTE            		icAND[1];		// DIB bits for AND mask
} ICONIMAGE, *LPICONIMAGE;
#pragma pack( pop)
#pragma pack( push, 2 )
typedef struct
{
   BYTE   				bWidth;         // Width, in pixels, of the image
   BYTE   				bHeight;        // Height, in pixels, of the image
   BYTE   				bColorCount;    // Number of colors in image (0 if >=8bpp)
   BYTE   				bReserved;      // Reserved
   WORD   				wPlanes;        // Color Planes
   WORD   				wBitCount;      // Bits per pixel
   DWORD  				dwBytesInRes;   // total size of the RT_ICON resource referenced by the nID member.
   WORD   				nID;            // resourceID of RT_ICON (LockResource to obtain a pointer to its ICONIMAGE)
} GRPICONDIRENTRY, *LPGRPICONDIRENTRY;
typedef struct 
{
   WORD            		idReserved;		// Reserved (must be 0)
   WORD            		idType;			// Resource type (1 for icons)
   WORD            		idCount;		// How many images?
   GRPICONDIRENTRY   	idEntries[1];	// The entries for each image
} GRPICONDIR, *LPGRPICONDIR;
#pragma pack( pop )

U32 Convert16BitToARGB(U16 value)
{
	return (0xFF000000|((value >>  7) & 0x0000F8)|((value <<  6) & 0x00F800)|((value << 19) & 0xF80000));
}
U32 GetMaskBit(U08* data, INT x, INT y, INT w, INT h)
{
	U32 mask_data_rowsize = (((w+31)>>5) * 4);
	return ((~(data[(mask_data_rowsize * ((h-1)-y)) + (x >> 3)] >> (0x07 - (x & 0x07))) & 1) * 0xFFFFFFFF);
}
U32 GetColorMonochrome(U08* xordata, U08* anddata, INT x, INT y, INT w, INT h, U32* pal)
{
	U32 mask_data_rowsize = (((w+31)>>5) * 4);
	U32 xor_bit = (((xordata[(mask_data_rowsize * ((h-1)-y)) + (x >> 3)] >> (0x07 - (x & 0x07))) << 1) & 2);
	U32 and_bit = (((anddata[(mask_data_rowsize * ((h-1)-y)) + (x >> 3)] >> (0x07 - (x & 0x07)))     ) & 1);
	U32 value = (xor_bit | and_bit);
	return pal[value];
}
U32 NumberOfColorsIn8bppIconResData(PTR data, INT size, INT w, INT h, INT colors)
{
	U32 ignore_size = ((BITMAPINFOHEADER*)(data))->biSize;
	//U32 color_data_size = ((((((w * 8)+7) >> 3) +3) & ~3) * h);
	if(colors > 256) colors = 256; //Color Count must be restricted to 256 entries at the most.
	if(colors <=  0) colors = 256; //Color Count might be reported as zero. This means 256.
	
	BIT usedColors[colors];
	INT numUsedColors = 0;
	for(IND i = colors-1; i >= 0; i--) usedColors[i]=0;
	
	U08* src = (((U08*)data) + ignore_size + (colors << 2));
	U32* pal = ((U32*)(((U08*)data) + ignore_size));
	INT padding_checker = 0;
	for(IND yy = h-1; yy >= 0; --yy){
		for(IND xx = 0; xx < w; ++xx){
			U08 color = src[0];
			if(color < colors){
				numUsedColors += (!usedColors[color]);
				usedColors[color] |= true;
			}
			src++;
			padding_checker++;
			padding_checker &= 3;
		}
		//This loop jumps over any padding bytes.
		while(padding_checker)
		{
			src++;
			padding_checker++;
			padding_checker &= 3;
		}
	}
	for(IND cc = 0; cc < 255; ++cc){
		if(usedColors[cc]){
			U32 thecolorvalue = pal[cc];
			for(IND dd = 0; dd < 255; ++dd){
				if((dd != cc) && usedColors[dd] && (thecolorvalue == pal[dd])){
					usedColors[dd] = false;
				}
			}
		}
	}
	U32 really_used_colors = 0;
	for(IND cc = 0; cc < 255; ++cc){
		really_used_colors += usedColors[cc];
	}
	return really_used_colors;
}
BIT IconResDataIsPNG(PTR data, INT size)
{
	///Simion32: USAGE OF STR APPROVED.
	STR pngheader = "\211PNG\r\n\032\n";
	STR cpd = (CHR*)data;
	if(size < 8) return false;
	return    ((cpd[0]==pngheader[0])
			&& (cpd[1]==pngheader[1])
			&& (cpd[2]==pngheader[2])
			&& (cpd[3]==pngheader[3])
			&& (cpd[4]==pngheader[4])
			&& (cpd[5]==pngheader[5])
			&& (cpd[6]==pngheader[6])
			&& (cpd[7]==pngheader[7]));
}
BMP CreateBmp32bppFromIconResData(PTR data, INT size, INT depth, INT w, INT h, INT colors)
{
	///Simion32: USAGE OF STR APPROVED.
	STR pngheader = "\211PNG\r\n\032\n";
	STR cpd = (CHR*)data;
	if(size < 8) return NULL;
	BIT is_png = ((cpd[0]==pngheader[0])
				&& (cpd[1]==pngheader[1])
				&& (cpd[2]==pngheader[2])
				&& (cpd[3]==pngheader[3])
				&& (cpd[4]==pngheader[4])
				&& (cpd[5]==pngheader[5])
				&& (cpd[6]==pngheader[6])
				&& (cpd[7]==pngheader[7]));
	if(is_png)
	{
		try{
            CFileIO PNG;
            PNG.SvHex(size, 4);
            PNG.Append((U08*)data, size);
            BMP result = PNG.GvPNG();
            PNG.Close();
            return result;
		}catch(const exception& e){
            return NULL;
		}endtry;
	}
	else
	{
		U32 ignore_size = ((BITMAPINFOHEADER*)(data))->biSize;
		BMP bmp = create_bitmap_ex(32,w,h);
		//U32 pixel_count = (w * h);
		U32 color_data_size = ((((((w * depth)+7) >> 3) +3) & ~3) * h);
		switch(depth)
		{
			default: return bmp; break;
			case 32:
			{
				U32* src = (U32*)(((U08*)data) + ignore_size);
				for(IND yy = h-1; yy >= 0; --yy){
					for(IND xx = 0; xx < w; ++xx){
						_putpixel32(bmp,xx,yy,src[0]);
						src++;
					}
					//There should never be any padding to jump over here.
				}
				return bmp;
			}
			break;
			case 24:
			{
				U32* src = (U32*)(((U08*)data) + ignore_size);
				U08* bitmask = (U08*)(((U08*)data) + ignore_size + color_data_size);
				INT padding_checker = 0;
				for(IND yy = h-1; yy >= 0; --yy){
					for(IND xx = 0; xx < w; ++xx){
						_putpixel32(bmp,xx,yy,((src[0] & 0x00FFFFFF) | 0xFF000000) & GetMaskBit(bitmask, xx, yy, w, h));
						src++;
						src = (U32*)(((U08*)src)-1); //go back a byte due to packing
						padding_checker += 3;
						padding_checker &= 3;
					}
					//This loop jumps over any padding bytes.
					while(padding_checker)
					{
						src = (U32*)(((U08*)src)+1);
						padding_checker++;
						padding_checker &= 3;
					}
				}
				return bmp;
			}
			break;
			case 16:
			{
				//Note: there might be a color table present! ignore it.
				U16* src = (U16*)(((U08*)data) + ignore_size + (colors << 2));
				U08* bitmask = (U08*)(((U08*)data) + ignore_size + (colors << 2) + color_data_size);
				INT padding_checker = 0;
				for(IND yy = h-1; yy >= 0; --yy){
					for(IND xx = 0; xx < w; ++xx){
						_putpixel32(bmp,xx,yy,Convert16BitToARGB(src[0]) & GetMaskBit(bitmask, xx, yy, w, h));
						src++;
						padding_checker += 2;
						padding_checker &= 3;
					}
					//This loop jumps over any padding bytes.
					while(padding_checker)
					{
						src = (U16*)(((U08*)src)+1);
						padding_checker++;
						padding_checker &= 3;
					}
				}
				return bmp;
			}
			break;
			case 8:
			{
				if(colors > 256) colors = 256; //Color Count must be restricted to 256 entries at the most.
				if(colors <=  0) colors = 256; //Color Count might be reported as zero. This means 256.
				U08* src = (((U08*)data) + ignore_size + (colors << 2));
				U32* pal = ((U32*)(((U08*)data) + ignore_size));
				U08* bitmask = (U08*)(((U08*)data) + ignore_size + (colors << 2) + color_data_size);
				INT padding_checker = 0;
				for(IND yy = h-1; yy >= 0; --yy){
					for(IND xx = 0; xx < w; ++xx){
						U08 color = src[0];
						if(color < colors){
							_putpixel32(bmp,xx,yy,(pal[color] | 0xFF000000) & GetMaskBit(bitmask, xx, yy, w, h));
						}else{
							_putpixel32(bmp,xx,yy,0x00FF00FF);
						}
						src++;
						padding_checker++;
						padding_checker &= 3;
					}
					//This loop jumps over any padding bytes.
					while(padding_checker)
					{
						src++;
						padding_checker++;
						padding_checker &= 3;
					}
				}
				return bmp;
			}
			break;
			case 4:
			{
				if(colors > 16) colors = 16; //Color Count must be restricted to 16 entries at the most.
				if(colors <= 0) colors = 16; //Color Count might be reported as zero. This means 16.
				U08* src = (((U08*)data) + ignore_size + (colors << 2));
				U32* pal = ((U32*)(((U08*)data) + ignore_size));
				U08* bitmask = (U08*)(((U08*)data) + ignore_size + (colors << 2) + color_data_size);
				INT padding_checker = 0;
				for(IND yy = h-1; yy >= 0; --yy){
					for(IND xx = 0; xx < w; ++xx){
						U08 color = src[0];
						if(xx & 1) color = ( color       & 0x0F);
						else	   color = ((color >> 4) & 0x0F);
						if(color < colors){
							_putpixel32(bmp,xx,yy,(pal[color] | 0xFF000000) & GetMaskBit(bitmask, xx, yy, w, h));
						}else{
							_putpixel32(bmp,xx,yy,0x00FF00FF);
						}
						if(xx & 1)
						{
							src++;
							padding_checker++;
							padding_checker &= 3;
						}
					}
					//if the pointer hasn't incremented to the next byte yet, do so.
					if(w & 1) //odd width
					{
						src++;
						padding_checker++;
						padding_checker &= 3;
					}
					//This loop jumps over any padding bytes.
					while(padding_checker)
					{
						src++;
						padding_checker++;
						padding_checker &= 3;
					}
				}
				return bmp;
			}
			break;
			case 1:
			{
				if(colors >  2) colors = 2; //Color Count must be restricted to 2 entries at the most.
				if(colors <= 0) colors = 2; //Color Count might be reported as zero. This means 2.
				U32* pal        = (U32*)(((U08*)data) + ignore_size);
				U08* bitmaskXOR = (U08*)(((U08*)data) + ignore_size + (colors << 2));
				U08* bitmaskAND = (U08*)(((U08*)data) + ignore_size + (colors << 2) + color_data_size);
				U32  ret_colors[4] = {pal[0]|0xFF000000, 0x00FF00FF, pal[1]|0xFF000000, 0x0000FF00};
				for(IND yy = h-1; yy >= 0; --yy){
					for(IND xx = 0; xx < w; ++xx){
						_putpixel32(bmp,xx,yy,GetColorMonochrome(bitmaskXOR, bitmaskAND, xx, yy, w, h, ret_colors));
					}
				}
				return bmp;
			}
			break;
		}
		return bmp;
	}
	return NULL;
}


static INT DepthTier(INT depth)
{
	if(depth >= 8) return 0;
	if(depth >= 4) return 1;
	return 2;
}
static INT DepthRating(INT depth)
{
	if(depth == 4) return 1;
	if(depth < 8) return 0;
	if(depth == 32) return 0;
	if(depth == 24) return 1;
	if(depth ==  8) return 2;
	if(depth == 16) return 3;
	return 42;//failed? well who cares
}
static INT SizeTier(INT w, INT h, INT tw, INT th)
{
	INT distLimit = ((w * w) + (h * h));
	INT distThis  = ((tw * tw) + (th * th));
	return (distThis < distLimit);
}
static INT SizeRating(INT w, INT h, INT tw, INT th)
{
	INT diffw = w - tw;
	INT diffh = h - th;
	return ((diffw * diffw) + (diffh * diffh));
}
struct SIconDims
{
	INT ID, c;
	PTR data; DWORD size;
	INT d, w, h;
	INT depth_tier, depth_rating;
	INT size_tier,  size_rating;
	SIconDims(INT ID, INT c, PTR data, DWORD size, INT d, INT w, INT h, INT target_w, INT target_h):
		ID(ID), d(d), c(c), data(data), size(size), w(w), h(h),
		depth_tier(DepthTier(d)),                   depth_rating(DepthRating(d)),
		size_tier(SizeTier(w,h,target_w,target_h)), size_rating(SizeRating(w,h,target_w,target_h))
	{
		if(IconResDataIsPNG(data, size)){
			depth_tier = 0;
			depth_rating = 0;
		}else{
			if(d == 8){
				U32 really_used_colors = NumberOfColorsIn8bppIconResData(data, size, w, h, c);
				if(really_used_colors < 24){
					depth_tier = 1;
					depth_rating = 0;
				}
			}
		}
	};
	SIconDims():
		ID(-1), c(0), data(NULL), size(0), w(0), h(0), d(-1), 
		depth_tier(4), depth_rating(4),
		size_tier(-1), size_rating(0x70000000) {};
	
	// exceptions:
	// if the image actually doesn't have 256 colors defined in it, 
	// or only 16 are used or something, then drop it down a tier.
	// I guess it could be a sub-depth within tier 1
	// so the number of real colors would act as the sub-depth.
	
	/*	the point is that an image that has low colors, regardless 
		of its actual depth, should get demoted to the proper tier
		if it looks like low color we treat it as such
		this is a heuristic, unfortunately.
		usually, the high color versions of an icon are only low color
		copies if that's the only thing it is supposed to look like,
		in which case depth choice doesn't matter
		but for 256-colors, it has a tendency to want to copy the 16
		color icon (maybe with a few extra colors?)
		monochrome icons are exceedingly rare, but they do exist*/
	
	BIT operator>(const SIconDims&  rhs)
	{
		if(depth_tier < rhs.depth_tier){
			return true;
		}else if(depth_tier == rhs.depth_tier){
			if(size_tier < rhs.size_tier){
				return true;
			}else if(size_tier == rhs.size_tier){
				if(size_rating < rhs.size_rating){
					return true;
				}else if(size_rating == rhs.size_rating){
					if(depth_rating < rhs.depth_rating){
						return true;
					}
				}
			}
		}
		return false;
	}
};
Vx<BMP> ResourceToBitmapVector(HMODULE hm, HRSRC hr, BIT is_group_icon)
{
	Vx<BMP> results;
	results.push_back(create_bitmap_ex(32,16,16));
	results.push_back(create_bitmap_ex(32,32,32));
	results.push_back(create_bitmap_ex(32,48,48));
	clear_to_color(results[0],0x00FF00FF);
	clear_to_color(results[1],0x00FF00FF);
	clear_to_color(results[2],0x00FF00FF);
	if(is_group_icon)
	{
		HGLOBAL hg = LoadResource(hm,hr);
		GRPICONDIR* gd = (GRPICONDIR*)LockResource(hg);
		if(gd->idType == 1)
		{
			SIconDims best16;
			SIconDims best32;
			SIconDims best48;
			for(IND i = 0; i < gd->idCount; ++i)
			{
				//WARNING: The GRPICONDIRENTRY's data might be wrong!
				GRPICONDIRENTRY* ie = (GRPICONDIRENTRY*)&(gd->idEntries[i]);
				HRSRC ihr = FindResource(hm,MAKEINTRESOURCE(ie->nID),RT_ICON);
				if(ihr != NULL)
				{
					HGLOBAL ihg = LoadResource(hm,ihr);
					PTR data = (PTR)LockResource(ihg);
					DWORD size = SizeofResource(hm,ihr);
					U32 b = ((BITMAPINFOHEADER*)(data))->biBitCount;
					U32 w = ((BITMAPINFOHEADER*)(data))->biWidth;
					U32 h = (((BITMAPINFOHEADER*)(data))->biHeight >> 1); //icons have doubled height value.
					U32 c = ((BITMAPINFOHEADER*)(data))->biClrUsed;
					SIconDims i16(i, c, data, size, b, w, h, 16, 16);
					SIconDims i32(i, c, data, size, b, w, h, 32, 32);
					SIconDims i48(i, c, data, size, b, w, h, 48, 48);
					if(i16 > best16) best16 = i16;
					if(i32 > best32) best32 = i32;
					if(i48 > best48) best48 = i48;
				}
			}
			if(gd->idCount)
			{
				BMP b16 = CreateBmp32bppFromIconResData(best16.data, best16.size, best16.d, best16.w, best16.h, best16.c);
				BMP b32 = CreateBmp32bppFromIconResData(best32.data, best32.size, best32.d, best32.w, best32.h, best32.c);
				BMP b48 = CreateBmp32bppFromIconResData(best48.data, best48.size, best48.d, best48.w, best48.h, best48.c);
				PwrUpScale(b16, results[0]);
				PwrUpScale(b32, results[1]);
				PwrUpScale(b48, results[2]);
				NullifyF(b16);
				NullifyF(b32);
				NullifyF(b48);
			}
		}
	}
	else
	{
		HGLOBAL ihg = LoadResource(hm,hr);
		PTR data = (PTR)LockResource(ihg);
		DWORD size = SizeofResource(hm,hr);
		U32 b = ((BITMAPINFOHEADER*)(data))->biBitCount;
		U32 w = ((BITMAPINFOHEADER*)(data))->biWidth;
		U32 h = (((BITMAPINFOHEADER*)(data))->biHeight >> 1); //icons have doubled height value.
		U32 c = ((BITMAPINFOHEADER*)(data))->biClrUsed;
		BMP bALL = CreateBmp32bppFromIconResData(data, size, b, w, h, c);
		PwrUpScale(bALL, results[0]);
		PwrUpScale(bALL, results[1]);
		PwrUpScale(bALL, results[2]);
		NullifyF(bALL);
	}
	return results;
}
Vx<BMP> IconFileToBitmapVector(PTR icon_data, U32 icon_size)
{
	Vx<BMP> results;
	results.push_back(create_bitmap_ex(32,16,16));
	results.push_back(create_bitmap_ex(32,32,32));
	results.push_back(create_bitmap_ex(32,48,48));
	clear_to_color(results[0],0x00FF00FF);
	clear_to_color(results[1],0x00FF00FF);
	clear_to_color(results[2],0x00FF00FF);
	ICONDIR* gd = (ICONDIR*)icon_data;
	if(gd->idType == 1) 
	{
		SIconDims best16;
		SIconDims best32;
		SIconDims best48;
		for(IND i = 0; i < gd->idCount; ++i)
		{
			//WARNING: The ICONDIRENTRY's data might be wrong!
			DWORD offset = gd->idEntries[i].dwImageOffset;
			DWORD size = gd->idEntries[i].dwBytesInRes;
			PTR data = (PTR)(((U08*)icon_data) + ((U32)offset));
			U32 b = ((BITMAPINFOHEADER*)(data))->biBitCount;
			U32 w = ((BITMAPINFOHEADER*)(data))->biWidth;
			U32 h = (((BITMAPINFOHEADER*)(data))->biHeight >> 1); //icons have doubled height value.
			U32 c = ((BITMAPINFOHEADER*)(data))->biClrUsed;
			SIconDims i16(i, c, data, size, b, w, h, 16, 16);
			SIconDims i32(i, c, data, size, b, w, h, 32, 32);
			SIconDims i48(i, c, data, size, b, w, h, 48, 48);
			if(i16 > best16) best16 = i16;
			if(i32 > best32) best32 = i32;
			if(i48 > best48) best48 = i48;
		}
		if(gd->idCount)
		{
			BMP b16 = CreateBmp32bppFromIconResData(best16.data, best16.size, best16.d, best16.w, best16.h, best16.c);
			BMP b32 = CreateBmp32bppFromIconResData(best32.data, best32.size, best32.d, best32.w, best32.h, best32.c);
			BMP b48 = CreateBmp32bppFromIconResData(best48.data, best48.size, best48.d, best48.w, best48.h, best48.c);
			PwrUpScale(b16, results[0]);
			PwrUpScale(b32, results[1]);
			PwrUpScale(b48, results[2]);
			NullifyF(b16);
			NullifyF(b32);
			NullifyF(b48);
		}
	}
	return results;
}
static INT EnumResNameProc_nicon = 0;
static INT EnumResNameProc_index = 0;
BOOL CALLBACK EnumResNameProc(HMODULE hm, LPCTSTR lpszType, LPTSTR lpszName, LONG_PTR lParam)
{
	Vx<BMP>& results = *((Vx<BMP>*)lParam);
	HRSRC hr = FindResource(hm,lpszName,lpszType);
	if(hr != NULL)
	{
		if(EnumResNameProc_nicon == (EnumResNameProc_index & 0x7FFFFFFF))
		{
			results = ResourceToBitmapVector(hm, hr, true);
			return false;
		}
	}
	EnumResNameProc_nicon++;
	return true;
}
Vx<BMP> GetFileTypeImages(TXT& file, BIT self_refrence, U32 index)
{
    #define LOAD_IGNORE_CODE_AUTHZ_LEVEL 0x00000010
    #define LLEX_PARAMS (DONT_RESOLVE_DLL_REFERENCES | LOAD_IGNORE_CODE_AUTHZ_LEVEL | LOAD_LIBRARY_AS_DATAFILE)
    Vx<BMP> results;
	TXT ext = FileSystem.FileExt(file);
    for(IND i = 0; i < ext.size(); ++i) ext[i] = tolower(ext[i]);
    if(ext != "ico")
    {
    	// Portable Executable Resource (works for both DLL and EXE)
		// 32bit Icon Library (Microangelo Studio?)
    	HMODULE hm = LoadLibraryEx(String.ToWideC(file), NULL, LLEX_PARAMS);
		if(hm != NULL)
		{
			BIT is_single_icon;
			INT which_to_try_first = !(index & 0x80000000);
			for(IND xyz = 1; xyz >= 0; --xyz, which_to_try_first=!which_to_try_first)
			{
				switch(which_to_try_first)
				{
					case 0:{
						is_single_icon = false;
						HRSRC hr = FindResource(hm,MAKEINTRESOURCE(index & 0x7FFFFFFF),RT_GROUP_ICON);
						if(hr == NULL)
						{
							hr = FindResource(hm,MAKEINTRESOURCE(index & 0x7FFFFFFF),RT_ICON);
							is_single_icon = (hr != NULL);
						}
						if(hr != NULL)
						{
							results = ResourceToBitmapVector(hm, hr, !is_single_icon);
						}
						if(results.size()) xyz = -1;
					}break;
					case 1:{
						is_single_icon = false;
						EnumResNameProc_nicon = 0;
						EnumResNameProc_index = index;
						EnumResourceNames(hm,RT_GROUP_ICON,&EnumResNameProc,(LONG_PTR)&results);
						if(results.size()) xyz = -1;
					}break;
				}
			}
			FreeLibrary(hm);
		}
    	else
    	{
    		// Failed, insert generic blank image.
    		results.push_back(create_bitmap_ex(32,16,16));
    		results.push_back(create_bitmap_ex(32,32,32));
    		results.push_back(create_bitmap_ex(32,48,48));
    		clear_to_color(results[0],0x00FF00FF);
    		clear_to_color(results[1],0x00FF00FF);
    		clear_to_color(results[2],0x00FF00FF);
    	}
    }
    else
    {
    	//Single Icon File
    	CFileIO ICO_FILE;
    	BIT load_failure = false;
    	try{
			ICO_FILE.Load(file);
		}catch(const exception& e){ 
			load_failure = true;
		}endtry;
    	if(!load_failure && ICO_FILE.ByteSize())
    	{
    		results = IconFileToBitmapVector((PTR)(&(ICO_FILE.Vector()[0])), ICO_FILE.Vector().size());
    	}
    	else
    	{
    		// Failed, insert generic blank image.
    		results.push_back(create_bitmap_ex(32,16,16));
    		results.push_back(create_bitmap_ex(32,32,32));
    		results.push_back(create_bitmap_ex(32,48,48));
    		clear_to_color(results[0],0x00FF00FF);
    		clear_to_color(results[1],0x00FF00FF);
    		clear_to_color(results[2],0x00FF00FF);
    	}
    	ICO_FILE.Close();
    }
    return results;
}
//###########################################################################
//###########################################################################
//###########################################################################
//###########################################################################
//###########################################################################
//###########################################################################
//###########################################################################
//###########################################################################
//###########################################################################
//###########################################################################
//###########################################################################
//###########################################################################
//###########################################################################
typedef struct RgbColor
{
    U08 r;
    U08 g;
    U08 b;
} RgbColor;

typedef struct HsvColor
{
    U08 h;
    U08 s;
    U08 v;
} HsvColor;

RgbColor real_HsvToRgb(HsvColor hsv)
{
    RgbColor rgb;
    U08 region, remainder, p, q, t;

    if (hsv.s == 0)
    {
        rgb.r = hsv.v;
        rgb.g = hsv.v;
        rgb.b = hsv.v;
        return rgb;
    }

    region = hsv.h / 43;
    remainder = (hsv.h - (region * 43)) * 6; 

    p = (hsv.v * (255 - hsv.s)) >> 8;
    q = (hsv.v * (255 - ((hsv.s * remainder) >> 8))) >> 8;
    t = (hsv.v * (255 - ((hsv.s * (255 - remainder)) >> 8))) >> 8;

    switch (region)
    {
        case 0:
            rgb.r = hsv.v; rgb.g = t; rgb.b = p;
            break;
        case 1:
            rgb.r = q; rgb.g = hsv.v; rgb.b = p;
            break;
        case 2:
            rgb.r = p; rgb.g = hsv.v; rgb.b = t;
            break;
        case 3:
            rgb.r = p; rgb.g = q; rgb.b = hsv.v;
            break;
        case 4:
            rgb.r = t; rgb.g = p; rgb.b = hsv.v;
            break;
        default:
            rgb.r = hsv.v; rgb.g = p; rgb.b = q;
            break;
    }

    return rgb;
}

HsvColor real_RgbToHsv(RgbColor rgb)
{
    HsvColor hsv;
    U08 rgbMin, rgbMax;

    rgbMin = rgb.r < rgb.g ? (rgb.r < rgb.b ? rgb.r : rgb.b) : (rgb.g < rgb.b ? rgb.g : rgb.b);
    rgbMax = rgb.r > rgb.g ? (rgb.r > rgb.b ? rgb.r : rgb.b) : (rgb.g > rgb.b ? rgb.g : rgb.b);

    hsv.v = rgbMax;
    if (hsv.v == 0)
    {
        hsv.h = 0;
        hsv.s = 0;
        return hsv;
    }

    hsv.s = 255 * long(rgbMax - rgbMin) / hsv.v;
    if (hsv.s == 0)
    {
        hsv.h = 0;
        return hsv;
    }

    if (rgbMax == rgb.r)
        hsv.h = 0 + 43 * (rgb.g - rgb.b) / (rgbMax - rgbMin);
    else if (rgbMax == rgb.g)
        hsv.h = 85 + 43 * (rgb.b - rgb.r) / (rgbMax - rgbMin);
    else
        hsv.h = 171 + 43 * (rgb.r - rgb.g) / (rgbMax - rgbMin);

    return hsv;
}
INT ARGB_GetHue0to255(U32 color)
{
	RgbColor x;
	x.r = ((color >> 16) & 0xFF);
	x.g = ((color >> 8) & 0xFF);
	x.b = (color & 0xFF);
	HsvColor k = real_RgbToHsv(x);
	return k.h;
}
INT ARGB_GetSat0to255(U32 color)
{
	RgbColor x;
	x.r = ((color >> 16) & 0xFF);
	x.g = ((color >> 8) & 0xFF);
	x.b = (color & 0xFF);
	HsvColor k = real_RgbToHsv(x);
	return k.s;
}
INT ARGB_GetVal0to255(U32 color)
{
	RgbColor x;
	x.r = ((color >> 16) & 0xFF);
	x.g = ((color >> 8) & 0xFF);
	x.b = (color & 0xFF);
	HsvColor k = real_RgbToHsv(x);
	return k.v;
}
U32 Hue0to255ToARGB(INT hue)
{
	HsvColor hsv;
	hsv.h = hue;
	hsv.s = 255;
	hsv.v = 255;
	RgbColor x = real_HsvToRgb(hsv);
	return ((x.r << 16) | (x.g << 8) | (x.b) | 0xFF000000);
}
















