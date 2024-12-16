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
#include <png.h>
#include <pngstruct.h>
#include "include.h"
extern "C" {BITMAP *load_memory_png(AL_CONST void* buffer, int bufsize, RGB *pal);}
extern "C" {BITMAP *really_load_png(PACKFILE *fp, RGB *pal);}
int really_save_png(CFileIO* IOFILE, BMP bmp, AL_CONST RGB *pal);

//##############################################################################
//##############################################################################
//##############################################################################
//##############################################################################
//##############################################################################
//##############################################################################
//##############################################################################
//##############################################################################
/* Overkill :-) */
#define LOADPNG_VERSION		1
#define LOADPNG_SUBVERSION	5
#define LOADPNG_VERSIONSTR	"1.5"
/* _png_screen_gamma is slightly overloaded (sorry):
 *
 * A value of 0.0 means: Don't do any gamma correction in load_png()
 * and load_memory_png().  This meaning was introduced in v1.4.
 *
 * A value of -1.0 means: Use the value from the environment variable
 * SCREEN_GAMMA (if available), otherwise fallback to a value of 2.2
 * (a good guess for PC monitors, and the value for sRGB colourspace).
 * This is the default.
 *
 * Otherwise, the value of _png_screen_gamma is taken as-is.
 */
double _png_screen_gamma = -1.0;
/* Choose zlib compression level for saving file.
 * Default is Z_BEST_COMPRESSION.
 */
int _png_compression_level = 9;///Z_BEST_COMPRESSION;



static void write_data(png_structp png_ptr, png_bytep data, png_uint_32 length)
{
    CFileIO *IOFILE = (CFileIO*)png_get_io_ptr(png_ptr);
    IOFILE->Append((U08*)data, (U32)length);
}
static void flush_data(png_structp png_ptr)
{
    (void)png_ptr;
}
static int save_indexed(png_structp png_ptr, BITMAP *bmp)
{
    ASSERT(bitmap_color_depth(bmp) == 8);
    if(is_memory_bitmap(bmp))   /* fast path */
    {
        for(IND y = 0; y < bmp->h; y++)
        {
            png_write_row(png_ptr, bmp->line[y]);
        }
        return 1;
    }
    else  /* generic case */
    {
        U08* rowdata;

        rowdata = (U08*) malloc(bmp->w * 3);
        if(!rowdata) return 0;

        for(IND y = 0; y < bmp->h; y++)
        {
            U08* p = rowdata;
            for(IND x = 0; x < bmp->w; x++)
            {
                *p++ = getpixel(bmp, x, y);
            }
            png_write_row(png_ptr, rowdata);
        }
        free(rowdata);
        return 1;
    }
}
static int save_rgb(png_structp png_ptr, BITMAP *bmp)
{
    AL_CONST int depth = bitmap_color_depth(bmp);
    U08* rowdata;
    int y, x;

    ASSERT(depth == 15 || depth == 16 || depth == 24);

    rowdata = (U08*)malloc(bmp->w * 3);
    if(!rowdata) return 0;

    for(y = 0; y < bmp->h; y++)
    {
        U08* p = rowdata;
        if(depth == 15)
        {
            for (x = 0; x < bmp->w; x++)
            {
                int c = getpixel(bmp, x, y);
                *p++ = getr15(c);
                *p++ = getg15(c);
                *p++ = getb15(c);
            }
        }
        else if(depth == 16)
        {
            for (x = 0; x < bmp->w; x++)
            {
                int c = getpixel(bmp, x, y);
                *p++ = getr16(c);
                *p++ = getg16(c);
                *p++ = getb16(c);
            }
        }
        else   /* depth == 24 */
        {
            for (x = 0; x < bmp->w; x++)
            {
                int c = getpixel(bmp, x, y);
                *p++ = getr24(c);
                *p++ = getg24(c);
                *p++ = getb24(c);
            }
        }
        png_write_row(png_ptr, rowdata);
    }
    free(rowdata);
    return 1;
}
static int save_rgba(png_structp png_ptr, BITMAP *bmp)
{
    U08* rowdata;
    int x, y;

    ASSERT(bitmap_color_depth(bmp) == 32);

    rowdata = (U08*)malloc(bmp->w * 4);
    if(!rowdata) return 0;

    for(y = 0; y < bmp->h; y++)
    {
        U08* p = rowdata;
		for(x = 0; x < bmp->w; x++)
        {
            int c = getpixel(bmp, x, y);
            *p++ = getr32(c);
            *p++ = getg32(c);
            *p++ = getb32(c);
            *p++ = geta32(c);
        }
        png_write_row(png_ptr, rowdata);
    }
    free(rowdata);
    return 1;
}
int really_save_png(CFileIO* IOFILE, BMP bmp, AL_CONST RGB *pal)
{
    png_structp png_ptr = NULL;
    png_infop info_ptr = NULL;
    int depth;
    int colour_type;

    depth = bitmap_color_depth(bmp);
    if(depth == 8 && !pal) return true;

    /* Create and initialize the png_struct with the
     * desired error handler functions.
     */
    png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, (void* )NULL, NULL, NULL);
    if(!png_ptr) goto Error;

    /* Allocate/initialize the image information data. */
    info_ptr = png_create_info_struct(png_ptr);
    if(!info_ptr) goto Error;

    /* Set error handling. */
    if(setjmp(png_ptr->jmp_buf_local)) goto Error; /* If we get here, we had a problem reading the file. */

    png_set_write_fn(png_ptr, (PTR)IOFILE, (png_rw_ptr)write_data, flush_data);

    /* Set the image information here.  Width and height are up to 2^31,
     * bit_depth is one of 1, 2, 4, 8, or 16, but valid values also depend on
     * the color_type selected. color_type is one of PNG_COLOR_TYPE_GRAY,
     * PNG_COLOR_TYPE_GRAY_ALPHA, PNG_COLOR_TYPE_PALETTE, PNG_COLOR_TYPE_RGB,
     * or PNG_COLOR_TYPE_RGB_ALPHA.  interlace is either PNG_INTERLACE_NONE or
     * PNG_INTERLACE_ADAM7, and the compression_type and filter_type MUST
     * currently be PNG_COMPRESSION_TYPE_BASE and PNG_FILTER_TYPE_BASE.
     */
    if(depth == 8)		 colour_type = PNG_COLOR_TYPE_PALETTE;
    else if(depth == 32) colour_type = PNG_COLOR_TYPE_RGB_ALPHA;
    else				 colour_type = PNG_COLOR_TYPE_RGB;

    /* Set compression level. */
    png_set_compression_level(png_ptr, _png_compression_level);

    png_set_IHDR(png_ptr, info_ptr, bmp->w, bmp->h, 8, colour_type,
		PNG_INTERLACE_NONE, PNG_COMPRESSION_TYPE_BASE, PNG_FILTER_TYPE_BASE);

    /* Set the palette if there is one.  Required for indexed-color images. */
    if(colour_type == PNG_COLOR_TYPE_PALETTE)
    {
        png_color palette[256];
        for(IND i = 0; i < 256; i++)
        {
            palette[i].red   = pal[i].r; //NO - DO NOT CONVERT. KEEP 256. -Simion32
            palette[i].green = pal[i].g;
            palette[i].blue  = pal[i].b;
        }
        /* Set palette colors. */
        png_set_PLTE(png_ptr, info_ptr, palette, 256);
    }

    /* Optionally write comments into the image ... Nah. */

    /* Write the file header information. */
    png_write_info(png_ptr, info_ptr);

    /* Once we write out the header, the compression type on the text
     * chunks gets changed to PNG_TEXT_COMPRESSION_NONE_WR or
     * PNG_TEXT_COMPRESSION_zTXt_WR, so it doesn't get written out again
     * at the end.
     */

    /* Save the data. */
    switch(depth)
    {
		case 8:
			if(!save_indexed(png_ptr, bmp)) goto Error;
		break;
		case 15:
		case 16:
		case 24:
			if(!save_rgb(png_ptr, bmp)) goto Error;
		break;
		case 32:
			if(!save_rgba(png_ptr, bmp)) goto Error;
		break;
		default:
			ASSERT(FALSE);
			goto Error;
    }
    png_write_end(png_ptr, info_ptr);
    png_destroy_write_struct(&png_ptr, &info_ptr);
    return false;

	Error:
	if(png_ptr)
    {
        if(info_ptr) png_destroy_write_struct(&png_ptr, &info_ptr);
        else		 png_destroy_write_struct(&png_ptr, NULL);
    }
    return true;
}
//##############################################################################
//##############################################################################
//##############################################################################
//##############################################################################
//##############################################################################
//##############################################################################
//##############################################################################
//##############################################################################

can_throw_t CFileIO::FillPNG(BITMAP *bmp)
{
	ASSERT(bmp);
	acquire_bitmap(bmp);
	buffer_.clear(); // EMPTY
	if(really_save_png(this, bmp, NULL))
	{
		release_bitmap(bmp);
		last_error_ = "Could not fill PNG from [Internal Data].";
		throw exception(last_error_);
	}
	release_bitmap(bmp);
	return can_throw_t();
}
can_throw_t CFileIO::AppendPNG(BITMAP *bmp)
{
	ASSERT(bmp);
	acquire_bitmap(bmp);
	if(really_save_png(this, bmp, NULL))
	{
		release_bitmap(bmp);
		last_error_ = "Could not append PNG from [Internal Data].";
		throw exception(last_error_);
	}
	release_bitmap(bmp);
	return can_throw_t();
}
can_throw_t CFileIO::SavePNG(BMP bmp, TXT filename)
{
    CFileIO x;
    x.FillPNG(bmp);
    x.Save(filename);
    x.Close();
	return can_throw_t();
}
can_throw_t CFileIO::SaveOpaquePNG(BMP bmp, TXT filename)
{
	BMP saving = create_bitmap_ex(32,bmp->w,bmp->h);
	for(IND y = bmp->h-1; y >= 0; --y){
		for(IND x = bmp->w-1; x >= 0; --x){
			U32 color = _getpixel32(bmp,x,y);
			_putpixel32(saving, x, y, (color | 0xFF000000));
		}
	}
	SavePNG(saving, filename);
	NullifyF(saving);
	return can_throw_t();
}
//##############################################################################
//##############################################################################
//##############################################################################
//##############################################################################
//##############################################################################
//##############################################################################
//##############################################################################
//##############################################################################
can_throw_tt<BMP> CFileIO::LoadPNG(TXT filename)
{
	BegAbortProtect("Tried to load a malformed PNG. LIBPNG called abort().");
	PACKFILE *fp = NULL;
    BITMAP *bmp = NULL;
    fp = pack_fopen(filename.c_str(), "r");
    if(!fp){
    	last_error_  = "Could not LoadPNG() file \"";
    	last_error_ += filename;
    	last_error_ += "\".\n\nMake sure the directory and the file both actually exist.";
    	throw exception(last_error_);
    }
    bmp = really_load_png(fp, NULL);
    pack_fclose(fp);
    if(!bmp){
		last_error_  = "Could not LoadPNG() file \"";
    	last_error_ += filename;
    	last_error_ += "\".\n\nThere was an error processing the file.";
    	throw exception(last_error_);
    }
    EndAbortProtect();
    return bmp;
}
can_throw_tt<BMP> CFileIO::GvPNG()
{
	BegAbortProtect("Tried to load a malformed PNG. LIBPNG called abort().");
	U32 size = GvHex(4); //this can fail if out of data
	if(size == 0x00000000){
        return NULL;
	}
	U32 start = get_position_; //obtain byte offset
	GvSkipBytes(size); //skip bits (pointer used below to load it!). this can fail if out of data
	PALLETE pal;
    BMP temp_abmp = load_memory_png((PTR)(&buffer_[start]), size, pal);
    if(!temp_abmp){
		last_error_  = "Could not GvPNG() from memory buffer.\n\nThere was an error processing the data.";
    	throw exception(last_error_);
    }
    int org_depth = bitmap_color_depth(temp_abmp);
    BMP BmpPtr = create_bitmap_ex(32, temp_abmp->w, temp_abmp->h);
    if(org_depth == 8){
		// We must draw the bitmap manually because Allegro uses
		// the fucked-up VGA hardware format - which only allows
		// 6 bits per color intensity.
		for(IND yy = (BmpPtr->h - 1); yy >= 0; --yy){
			for(IND xx = (BmpPtr->w - 1); xx >= 0; --xx){
				int index = getpixel(temp_abmp, xx, yy);
				int color = (pal[index].b | (pal[index].g << 8) | (pal[index].r << 16));
				_putpixel32(BmpPtr, xx, yy, color);
			}
		}
    }else{
		/*color convert*/blit(temp_abmp, BmpPtr, 0, 0, 0, 0, temp_abmp->w, temp_abmp->h);
	}
	NullifyF(temp_abmp);
    for(IND yy = (BmpPtr->h - 1); yy >= 0; --yy)
    {
        for(IND xx = (BmpPtr->w - 1); xx >= 0; --xx)
        {
            int color = _getpixel32(BmpPtr, xx, yy);//Load pixel.
            if(org_depth != 32) color |= 0xFF000000;
            if((!(color & 0xFF000000)) || ((color & 0x00FFFFFF) == 0x00FF00FF)){
                color = 0x00FF00FF;
            }
            _putpixel32(BmpPtr, xx, yy, color);//Put the pixel back.
        }
    }
    EndAbortProtect();
	return BmpPtr;
}
can_throw_t CFileIO::GvSkipPNG()
{
	U64 skip = GvHex(4);
    return GvSkipBytes(skip);
}
can_throw_t CFileIO::SvPNG(BITMAP *bmp)
{
	if(bmp == NULL){
        SvHex(0x00000000,4);
        return can_throw_t();
	}
	U32 size = 0xFFFFFFFF;
	U32 start = set_position_; //obtain offset
	SvHex(size,4); //put dummy size value of 0xFFFFFFFF
    ASSERT(bmp);
	acquire_bitmap(bmp);
	if(really_save_png(this, bmp, NULL))
	{
		release_bitmap(bmp);
		last_error_ = "Could not append PNG from [Internal Data].";
		throw exception(last_error_);
	}
	U32 after_png = set_position_;
	size = (after_png - (start + 4));
	SetW(start); //go back to the size value address
	SvHex(size,4); //fill in real size value!
	SetW(after_png); //put pointer back
	release_bitmap(bmp);
	return can_throw_t();
}
