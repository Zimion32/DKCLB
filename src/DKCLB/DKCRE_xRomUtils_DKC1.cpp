#include "include.h"
#include "DKCRE.h"
#include "CGameData.h"
#include "CLevelData.h"
bool RE_DKC1_GetCGFX(vector<Hex08>& out_cgfx, Hex32 offset, Hex32 archetype)
{
	Hex32 result = 0;
    if((archetype & 0x0F) != 0x0C)
    {
        Hex32 a = 0x00000000;
        while(true)
        {
            out_cgfx.push_back(DKC1[a + 0x80 + offset] & 0xFF);
            int v = DKC1[a + 0x80 + offset];
            if((v & 0xFF) == 0x00) break;
            a++;
            switch(v & 0xC0)
            {
                case 0x00:
                {
                    for(int i = (v & 0xFF); i != 0; --i)
                    {
                    	out_cgfx.push_back(DKC1[a + 0x80 + offset] & 0xFF);
                    	a++;
					}
					result += (v & 0xFF);
                }
                break;
                case 0x80:
                {
                    out_cgfx.push_back(DKC1[a + 0x80 + offset + 0] & 0xFF);
					out_cgfx.push_back(DKC1[a + 0x80 + offset + 1] & 0xFF);
					a += 2;
					result += (v & 0x3F);
                }break;
                case 0xC0:
                {
                    result += 2;
                }
                break;
                case 0x40:
                {
                	out_cgfx.push_back(DKC1[a + 0x80 + offset] & 0xFF);
                    a++;
                    result += (v & 0x3F);
                }
                break;
            }
        }
    }
    return result;
}
void RE_DKC1_GetGFX(vector<Hex08>& out_gfx, Hex32 offset, Hex32 archetype)
{
    Hex32 cgfx = offset;
    if((archetype & 0x0F) == 0x0C)
    {
    	// The Gangplank Galleon data is not compressed, copy verbatim.
    	for(int i = 0; i != 0x84A0; i++)
    	{
    		out_gfx.push_back(DKC1[(i + cgfx)] & 0xFF);
		}
	}
    else
    {
        Hex32 a = 0x00000000;
        while(true)
        {
            int v = DKC1[a + 0x80 + cgfx];
            if((v & 0xFF) == 0x00) break;
            a++;
            switch(v & 0xC0)
            {
                case 0x00:
                {
                    for(int i = (v & 0xFF); i != 0; --i)
                    {
                    	out_gfx.push_back(DKC1[a + 0x80 + cgfx] & 0xFF);
                    	a++;
					}
                }
                break;
                case 0x80:
                {
                    for(int i = (v & 0x3F), 
							j = ( (DKC1[a + 0x80 + cgfx] & 0xFF) + 
								 ((DKC1[a + 0x80 + cgfx + 1] & 0xFF) << 8))
						; i != 0; --i, ++j)
					{
                    	out_gfx.push_back(out_gfx[j]);
					}
                    a += 2;
                }break;
                case 0xC0:
                {
                    out_gfx.push_back(DKC1[((v & 0x3F) << 1) + cgfx    ] & 0xFF);
                    out_gfx.push_back(DKC1[((v & 0x3F) << 1) + cgfx + 1] & 0xFF);
                }
                break;
                case 0x40:
                {
                    for(int i = (v & 0x3F); i != 0; --i)
                    {
                    	out_gfx.push_back(DKC1[a + 0x80 + cgfx] & 0xFF);
					}
                    a++;
                }
                break;
            }
        }
    }
}
BITMAP* RE_DKC1_16x16(int offset, int pallete, int flip)
{
	safe_array(int, objectpallete, 0x00FF00FF,
	snespal2rgba(DKC1(pallete+0x00,2)),
	snespal2rgba(DKC1(pallete+0x02,2)),
	snespal2rgba(DKC1(pallete+0x04,2)),
	snespal2rgba(DKC1(pallete+0x06,2)),
	snespal2rgba(DKC1(pallete+0x08,2)),
	snespal2rgba(DKC1(pallete+0x0A,2)),
	snespal2rgba(DKC1(pallete+0x0C,2)),
	snespal2rgba(DKC1(pallete+0x0E,2)),
	snespal2rgba(DKC1(pallete+0x10,2)),
	snespal2rgba(DKC1(pallete+0x12,2)),
	snespal2rgba(DKC1(pallete+0x14,2)),
	snespal2rgba(DKC1(pallete+0x16,2)),
	snespal2rgba(DKC1(pallete+0x18,2)),
	snespal2rgba(DKC1(pallete+0x1A,2)),
	snespal2rgba(DKC1(pallete+0x1C,2)));
	BITMAP* rend_target = create_bitmap_ex(32, 16, 16);
	clear_to_color(rend_target, 0x00FF00FF);
	for(int row = 0;row != 8;row++)
	{
		int row_offset = offset + (row << 1);
		Hex32 qPixelRow = (
		(DKC1[row_offset +  0] << 24)|
		(DKC1[row_offset +  1] << 16)|
		(DKC1[row_offset + 16] <<  8)|
		(DKC1[row_offset + 17] <<  0));
		qPixelRow = ((qPixelRow & 0x00200400)|
		((qPixelRow & 0x00400801) <<  3)|
		((qPixelRow & 0x80100200) >>  3)|
		((qPixelRow & 0x00801002) <<  6)|
		((qPixelRow & 0x40080100) >>  6)|
		((qPixelRow & 0x00002004) <<  9)|
		((qPixelRow & 0x20040000) >>  9)|
		((qPixelRow & 0x00004008) << 12)|
		((qPixelRow & 0x10020000) >> 12)|
		((qPixelRow & 0x00008010) << 15)|
		((qPixelRow & 0x08010000) >> 15)|
		((qPixelRow & 0x00000020) << 18)|
		((qPixelRow & 0x04000000) >> 18)|
		((qPixelRow & 0x00000040) << 21)|
		((qPixelRow & 0x02000000) >> 21)|
		((qPixelRow & 0x00000080) << 24)|
		((qPixelRow & 0x01000000) >> 24));
		_putpixel32(rend_target, 7, row, objectpallete[qPixelRow & 0x0000000F]); qPixelRow >>= 4;
		_putpixel32(rend_target, 6, row, objectpallete[qPixelRow & 0x0000000F]); qPixelRow >>= 4;
		_putpixel32(rend_target, 5, row, objectpallete[qPixelRow & 0x0000000F]); qPixelRow >>= 4;
		_putpixel32(rend_target, 4, row, objectpallete[qPixelRow & 0x0000000F]); qPixelRow >>= 4;
		_putpixel32(rend_target, 3, row, objectpallete[qPixelRow & 0x0000000F]); qPixelRow >>= 4;
		_putpixel32(rend_target, 2, row, objectpallete[qPixelRow & 0x0000000F]); qPixelRow >>= 4;
		_putpixel32(rend_target, 1, row, objectpallete[qPixelRow & 0x0000000F]); qPixelRow >>= 4;
		_putpixel32(rend_target, 0, row, objectpallete[qPixelRow & 0x0000000F]);
		row_offset += 0x20;
		qPixelRow = (
		(DKC1[row_offset +  0] << 24)|
		(DKC1[row_offset +  1] << 16)|
		(DKC1[row_offset + 16] <<  8)|
		(DKC1[row_offset + 17] <<  0));
		qPixelRow = ((qPixelRow & 0x00200400)|
		((qPixelRow & 0x00400801) <<  3)|
		((qPixelRow & 0x80100200) >>  3)|
		((qPixelRow & 0x00801002) <<  6)|
		((qPixelRow & 0x40080100) >>  6)|
		((qPixelRow & 0x00002004) <<  9)|
		((qPixelRow & 0x20040000) >>  9)|
		((qPixelRow & 0x00004008) << 12)|
		((qPixelRow & 0x10020000) >> 12)|
		((qPixelRow & 0x00008010) << 15)|
		((qPixelRow & 0x08010000) >> 15)|
		((qPixelRow & 0x00000020) << 18)|
		((qPixelRow & 0x04000000) >> 18)|
		((qPixelRow & 0x00000040) << 21)|
		((qPixelRow & 0x02000000) >> 21)|
		((qPixelRow & 0x00000080) << 24)|
		((qPixelRow & 0x01000000) >> 24));
		_putpixel32(rend_target, 8+7, row, objectpallete[qPixelRow & 0x0000000F]); qPixelRow >>= 4;
		_putpixel32(rend_target, 8+6, row, objectpallete[qPixelRow & 0x0000000F]); qPixelRow >>= 4;
		_putpixel32(rend_target, 8+5, row, objectpallete[qPixelRow & 0x0000000F]); qPixelRow >>= 4;
		_putpixel32(rend_target, 8+4, row, objectpallete[qPixelRow & 0x0000000F]); qPixelRow >>= 4;
		_putpixel32(rend_target, 8+3, row, objectpallete[qPixelRow & 0x0000000F]); qPixelRow >>= 4;
		_putpixel32(rend_target, 8+2, row, objectpallete[qPixelRow & 0x0000000F]); qPixelRow >>= 4;
		_putpixel32(rend_target, 8+1, row, objectpallete[qPixelRow & 0x0000000F]); qPixelRow >>= 4;
		_putpixel32(rend_target, 8+0, row, objectpallete[qPixelRow & 0x0000000F]);
		row_offset -= 0x20;
		row_offset += 0x200;
		qPixelRow = (
		(DKC1[row_offset +  0] << 24)|
		(DKC1[row_offset +  1] << 16)|
		(DKC1[row_offset + 16] <<  8)|
		(DKC1[row_offset + 17] <<  0));
		qPixelRow = ((qPixelRow & 0x00200400)|
		((qPixelRow & 0x00400801) <<  3)|
		((qPixelRow & 0x80100200) >>  3)|
		((qPixelRow & 0x00801002) <<  6)|
		((qPixelRow & 0x40080100) >>  6)|
		((qPixelRow & 0x00002004) <<  9)|
		((qPixelRow & 0x20040000) >>  9)|
		((qPixelRow & 0x00004008) << 12)|
		((qPixelRow & 0x10020000) >> 12)|
		((qPixelRow & 0x00008010) << 15)|
		((qPixelRow & 0x08010000) >> 15)|
		((qPixelRow & 0x00000020) << 18)|
		((qPixelRow & 0x04000000) >> 18)|
		((qPixelRow & 0x00000040) << 21)|
		((qPixelRow & 0x02000000) >> 21)|
		((qPixelRow & 0x00000080) << 24)|
		((qPixelRow & 0x01000000) >> 24));
		_putpixel32(rend_target, 7, row+8, objectpallete[qPixelRow & 0x0000000F]); qPixelRow >>= 4;
		_putpixel32(rend_target, 6, row+8, objectpallete[qPixelRow & 0x0000000F]); qPixelRow >>= 4;
		_putpixel32(rend_target, 5, row+8, objectpallete[qPixelRow & 0x0000000F]); qPixelRow >>= 4;
		_putpixel32(rend_target, 4, row+8, objectpallete[qPixelRow & 0x0000000F]); qPixelRow >>= 4;
		_putpixel32(rend_target, 3, row+8, objectpallete[qPixelRow & 0x0000000F]); qPixelRow >>= 4;
		_putpixel32(rend_target, 2, row+8, objectpallete[qPixelRow & 0x0000000F]); qPixelRow >>= 4;
		_putpixel32(rend_target, 1, row+8, objectpallete[qPixelRow & 0x0000000F]); qPixelRow >>= 4;
		_putpixel32(rend_target, 0, row+8, objectpallete[qPixelRow & 0x0000000F]);
		row_offset += 0x20;
		qPixelRow = (
		(DKC1[row_offset +  0] << 24)|
		(DKC1[row_offset +  1] << 16)|
		(DKC1[row_offset + 16] <<  8)|
		(DKC1[row_offset + 17] <<  0));
		qPixelRow = ((qPixelRow & 0x00200400)|
		((qPixelRow & 0x00400801) <<  3)|
		((qPixelRow & 0x80100200) >>  3)|
		((qPixelRow & 0x00801002) <<  6)|
		((qPixelRow & 0x40080100) >>  6)|
		((qPixelRow & 0x00002004) <<  9)|
		((qPixelRow & 0x20040000) >>  9)|
		((qPixelRow & 0x00004008) << 12)|
		((qPixelRow & 0x10020000) >> 12)|
		((qPixelRow & 0x00008010) << 15)|
		((qPixelRow & 0x08010000) >> 15)|
		((qPixelRow & 0x00000020) << 18)|
		((qPixelRow & 0x04000000) >> 18)|
		((qPixelRow & 0x00000040) << 21)|
		((qPixelRow & 0x02000000) >> 21)|
		((qPixelRow & 0x00000080) << 24)|
		((qPixelRow & 0x01000000) >> 24));
		_putpixel32(rend_target, 8+7, row+8, objectpallete[qPixelRow & 0x0000000F]); qPixelRow >>= 4;
		_putpixel32(rend_target, 8+6, row+8, objectpallete[qPixelRow & 0x0000000F]); qPixelRow >>= 4;
		_putpixel32(rend_target, 8+5, row+8, objectpallete[qPixelRow & 0x0000000F]); qPixelRow >>= 4;
		_putpixel32(rend_target, 8+4, row+8, objectpallete[qPixelRow & 0x0000000F]); qPixelRow >>= 4;
		_putpixel32(rend_target, 8+3, row+8, objectpallete[qPixelRow & 0x0000000F]); qPixelRow >>= 4;
		_putpixel32(rend_target, 8+2, row+8, objectpallete[qPixelRow & 0x0000000F]); qPixelRow >>= 4;
		_putpixel32(rend_target, 8+1, row+8, objectpallete[qPixelRow & 0x0000000F]); qPixelRow >>= 4;
		_putpixel32(rend_target, 8+0, row+8, objectpallete[qPixelRow & 0x0000000F]);
	}
    BITMAP* final = create_bitmap_ex(32, 16, 16);
    clear_to_color(final, 0x00FF00FF);
    TzBMP_draw(rend_target, final, 0, 0, flip, TzModeBwNRM);
    Nullify(rend_target);
    return final;
}
BITMAP* RE_DKC1_Sprite(int param_spritenumber, int object_pallete, SRect* rect)
{
	safe_array(int, objectpallete, 0x00FF00FF,
	snespal2rgba(DKC1(object_pallete+0x00,2)),
	snespal2rgba(DKC1(object_pallete+0x02,2)),
	snespal2rgba(DKC1(object_pallete+0x04,2)),
	snespal2rgba(DKC1(object_pallete+0x06,2)),
	snespal2rgba(DKC1(object_pallete+0x08,2)),
	snespal2rgba(DKC1(object_pallete+0x0A,2)),
	snespal2rgba(DKC1(object_pallete+0x0C,2)),
	snespal2rgba(DKC1(object_pallete+0x0E,2)),
	snespal2rgba(DKC1(object_pallete+0x10,2)),
	snespal2rgba(DKC1(object_pallete+0x12,2)),
	snespal2rgba(DKC1(object_pallete+0x14,2)),
	snespal2rgba(DKC1(object_pallete+0x16,2)),
	snespal2rgba(DKC1(object_pallete+0x18,2)),
	snespal2rgba(DKC1(object_pallete+0x1A,2)),
	snespal2rgba(DKC1(object_pallete+0x1C,2)));
	
	const Hex32 SPRITE_PTR_TABLE = DKCRE::gbl->SpriteBank + DKCRE::gbl->SpriteTable;
    const Hex32 SPRITE_DATA_OFFSET = (DKC1(SPRITE_PTR_TABLE + param_spritenumber,4) & 0x003FFFFF);
    if(!SPRITE_DATA_OFFSET)
    {
    	BITMAP* error = create_bitmap_ex(32, 8, 8);
		clear_to_color(error, 0xFFFF0000);
		return error;
    }
    const int TilesGroupA = (DKC1[SPRITE_DATA_OFFSET + 0] & 0xFF); // Number of 16� tiles in Group A
    const int TilesGroupB = (DKC1[SPRITE_DATA_OFFSET + 1] & 0xFF); // Number of 8� tiles in Group B
    const int StartGroupB = (DKC1[SPRITE_DATA_OFFSET + 2] & 0xFF); // Start of Group B 8� tiles relative to start of graphics (Measured in 8� tiles)
    const int TilesGroupC = (DKC1[SPRITE_DATA_OFFSET + 3] & 0xFF); // Number of 8� tiles in Group C
    const int StartGroupC = (DKC1[SPRITE_DATA_OFFSET + 4] & 0xFF); // Start of Group C 8� tiles relative to start of graphics (Measured in 8� tiles)
    const int PositionDataSize = ((TilesGroupA + TilesGroupB + TilesGroupC) << 1);
	const int a_offset = (SPRITE_DATA_OFFSET + 8);
	const int b_offset = ((SPRITE_DATA_OFFSET + 8) + (TilesGroupA << 1));
	const int c_offset = ((SPRITE_DATA_OFFSET + 8) + (TilesGroupA << 1) + (TilesGroupB << 1));
	
	Vsx32 translate_x = 0, translate_y = 0;
	BITMAP* rend_target = NULL;
	if(rect)
	{
		rect->L = 0x7FFFFFFF;
		rect->T = 0x7FFFFFFF;
		rect->R = 0x80000001;
		rect->B = 0x80000001;
		for(int curtile = 0; curtile < TilesGroupA; curtile++)
		{
			int rend_x = (128 + (Vsx08(-DKC1[a_offset+(curtile << 1)+0] ^ 0x7F)));
			int rend_y = (128 + (-Vsx08(DKC1[a_offset+(curtile << 1)+1] ^ 0x7F)));
			if(rend_x < rect->L) rect->L = rend_x;
			if(rend_y < rect->T) rect->T = rend_y;
			if((rend_x+16) > rect->R) rect->R = (rend_x+16);
			if((rend_y+16) > rect->B) rect->B = (rend_y+16);
		}
		for(int curtile = 0; curtile < TilesGroupB; curtile++)
		{
			int rend_x = (128 + (Vsx08(-DKC1[b_offset+(curtile << 1)+0] ^ 0x7F)));
			int rend_y = (128 + (-Vsx08(DKC1[b_offset+(curtile << 1)+1] ^ 0x7F)));
			if(rend_x < rect->L) rect->L = rend_x;
			if(rend_y < rect->T) rect->T = rend_y;
			if((rend_x+8) > rect->R) rect->R = (rend_x+8);
			if((rend_y+8) > rect->B) rect->B = (rend_y+8);
		}
		for(int curtile = 0; curtile < TilesGroupC; curtile++)
		{
			int rend_x = (128 + (Vsx08(-DKC1[c_offset+(curtile << 1)+0] ^ 0x7F)));
			int rend_y = (128 + (-Vsx08(DKC1[c_offset+(curtile << 1)+1] ^ 0x7F)));
			if(rend_x < rect->L) rect->L = rend_x;
			if(rend_y < rect->T) rect->T = rend_y;
			if((rend_x+8) > rect->R) rect->R = (rend_x+8);
			if((rend_y+8) > rect->B) rect->B = (rend_y+8);
		}
		rend_target = create_bitmap_ex(32, (rect->R - rect->L), (rect->B - rect->T));
		clear_to_color(rend_target, 0x00FF00FF);
		translate_x = rect->L;
		translate_y = rect->T;
	}
	else
	{
		rend_target = create_bitmap_ex(32, 256, 256);
		clear_to_color(rend_target, 0x00FF00FF);
	}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//Cycle through group A 16x16 BMP's.
	int StartOfGraphics = (SPRITE_DATA_OFFSET + PositionDataSize + 8);
	for(int curtile = 0; curtile < TilesGroupA; curtile++)
	{
		int rend_x = (128 + (Vsx08(-DKC1[a_offset+(curtile << 1)+0] ^ 0x7F))) - translate_x;
		int rend_y = (128 + (-Vsx08(DKC1[a_offset+(curtile << 1)+1] ^ 0x7F))) - translate_y;
		for(int row = 0;row != 8;row++)
		{
			int row_offset = StartOfGraphics + ((curtile >> 3) << 10) + ((curtile & 7) << 6) + 0x00 + (row << 1);
			Hex32 qPixelRow = (
			(DKC1[row_offset +  0] << 24)|
			(DKC1[row_offset +  1] << 16)|
			(DKC1[row_offset + 16] <<  8)|
			(DKC1[row_offset + 17] <<  0));
			qPixelRow = ((qPixelRow & 0x00200400)|
			((qPixelRow & 0x00400801) <<  3)|
			((qPixelRow & 0x80100200) >>  3)|
			((qPixelRow & 0x00801002) <<  6)|
			((qPixelRow & 0x40080100) >>  6)|
			((qPixelRow & 0x00002004) <<  9)|
			((qPixelRow & 0x20040000) >>  9)|
			((qPixelRow & 0x00004008) << 12)|
			((qPixelRow & 0x10020000) >> 12)|
			((qPixelRow & 0x00008010) << 15)|
			((qPixelRow & 0x08010000) >> 15)|
			((qPixelRow & 0x00000020) << 18)|
			((qPixelRow & 0x04000000) >> 18)|
			((qPixelRow & 0x00000040) << 21)|
			((qPixelRow & 0x02000000) >> 21)|
			((qPixelRow & 0x00000080) << 24)|
			((qPixelRow & 0x01000000) >> 24));
			_putpixel32(rend_target, rend_x+7, rend_y+row, objectpallete[qPixelRow & 0x0000000F]); qPixelRow >>= 4;
			_putpixel32(rend_target, rend_x+6, rend_y+row, objectpallete[qPixelRow & 0x0000000F]); qPixelRow >>= 4;
			_putpixel32(rend_target, rend_x+5, rend_y+row, objectpallete[qPixelRow & 0x0000000F]); qPixelRow >>= 4;
			_putpixel32(rend_target, rend_x+4, rend_y+row, objectpallete[qPixelRow & 0x0000000F]); qPixelRow >>= 4;
			_putpixel32(rend_target, rend_x+3, rend_y+row, objectpallete[qPixelRow & 0x0000000F]); qPixelRow >>= 4;
			_putpixel32(rend_target, rend_x+2, rend_y+row, objectpallete[qPixelRow & 0x0000000F]); qPixelRow >>= 4;
			_putpixel32(rend_target, rend_x+1, rend_y+row, objectpallete[qPixelRow & 0x0000000F]); qPixelRow >>= 4;
			_putpixel32(rend_target, rend_x+0, rend_y+row, objectpallete[qPixelRow & 0x0000000F]);
			row_offset += 0x20;
			qPixelRow = (
			(DKC1[row_offset +  0] << 24)|
			(DKC1[row_offset +  1] << 16)|
			(DKC1[row_offset + 16] <<  8)|
			(DKC1[row_offset + 17] <<  0));
			qPixelRow = ((qPixelRow & 0x00200400)|
			((qPixelRow & 0x00400801) <<  3)|
			((qPixelRow & 0x80100200) >>  3)|
			((qPixelRow & 0x00801002) <<  6)|
			((qPixelRow & 0x40080100) >>  6)|
			((qPixelRow & 0x00002004) <<  9)|
			((qPixelRow & 0x20040000) >>  9)|
			((qPixelRow & 0x00004008) << 12)|
			((qPixelRow & 0x10020000) >> 12)|
			((qPixelRow & 0x00008010) << 15)|
			((qPixelRow & 0x08010000) >> 15)|
			((qPixelRow & 0x00000020) << 18)|
			((qPixelRow & 0x04000000) >> 18)|
			((qPixelRow & 0x00000040) << 21)|
			((qPixelRow & 0x02000000) >> 21)|
			((qPixelRow & 0x00000080) << 24)|
			((qPixelRow & 0x01000000) >> 24));
			_putpixel32(rend_target, rend_x+8+7, rend_y+row, objectpallete[qPixelRow & 0x0000000F]); qPixelRow >>= 4;
			_putpixel32(rend_target, rend_x+8+6, rend_y+row, objectpallete[qPixelRow & 0x0000000F]); qPixelRow >>= 4;
			_putpixel32(rend_target, rend_x+8+5, rend_y+row, objectpallete[qPixelRow & 0x0000000F]); qPixelRow >>= 4;
			_putpixel32(rend_target, rend_x+8+4, rend_y+row, objectpallete[qPixelRow & 0x0000000F]); qPixelRow >>= 4;
			_putpixel32(rend_target, rend_x+8+3, rend_y+row, objectpallete[qPixelRow & 0x0000000F]); qPixelRow >>= 4;
			_putpixel32(rend_target, rend_x+8+2, rend_y+row, objectpallete[qPixelRow & 0x0000000F]); qPixelRow >>= 4;
			_putpixel32(rend_target, rend_x+8+1, rend_y+row, objectpallete[qPixelRow & 0x0000000F]); qPixelRow >>= 4;
			_putpixel32(rend_target, rend_x+8+0, rend_y+row, objectpallete[qPixelRow & 0x0000000F]);
			row_offset -= 0x20;
			row_offset += (((curtile >> 3) == (TilesGroupA >> 3))?(((TilesGroupA & 7) << 6) + (TilesGroupB << 5)):512);
			qPixelRow = (
			(DKC1[row_offset +  0] << 24)|
			(DKC1[row_offset +  1] << 16)|
			(DKC1[row_offset + 16] <<  8)|
			(DKC1[row_offset + 17] <<  0));
			qPixelRow = ((qPixelRow & 0x00200400)|
			((qPixelRow & 0x00400801) <<  3)|
			((qPixelRow & 0x80100200) >>  3)|
			((qPixelRow & 0x00801002) <<  6)|
			((qPixelRow & 0x40080100) >>  6)|
			((qPixelRow & 0x00002004) <<  9)|
			((qPixelRow & 0x20040000) >>  9)|
			((qPixelRow & 0x00004008) << 12)|
			((qPixelRow & 0x10020000) >> 12)|
			((qPixelRow & 0x00008010) << 15)|
			((qPixelRow & 0x08010000) >> 15)|
			((qPixelRow & 0x00000020) << 18)|
			((qPixelRow & 0x04000000) >> 18)|
			((qPixelRow & 0x00000040) << 21)|
			((qPixelRow & 0x02000000) >> 21)|
			((qPixelRow & 0x00000080) << 24)|
			((qPixelRow & 0x01000000) >> 24));
			_putpixel32(rend_target, rend_x+7, rend_y+row+8, objectpallete[qPixelRow & 0x0000000F]); qPixelRow >>= 4;
			_putpixel32(rend_target, rend_x+6, rend_y+row+8, objectpallete[qPixelRow & 0x0000000F]); qPixelRow >>= 4;
			_putpixel32(rend_target, rend_x+5, rend_y+row+8, objectpallete[qPixelRow & 0x0000000F]); qPixelRow >>= 4;
			_putpixel32(rend_target, rend_x+4, rend_y+row+8, objectpallete[qPixelRow & 0x0000000F]); qPixelRow >>= 4;
			_putpixel32(rend_target, rend_x+3, rend_y+row+8, objectpallete[qPixelRow & 0x0000000F]); qPixelRow >>= 4;
			_putpixel32(rend_target, rend_x+2, rend_y+row+8, objectpallete[qPixelRow & 0x0000000F]); qPixelRow >>= 4;
			_putpixel32(rend_target, rend_x+1, rend_y+row+8, objectpallete[qPixelRow & 0x0000000F]); qPixelRow >>= 4;
			_putpixel32(rend_target, rend_x+0, rend_y+row+8, objectpallete[qPixelRow & 0x0000000F]);
			row_offset += 0x20;
			qPixelRow = (
			(DKC1[row_offset +  0] << 24)|
			(DKC1[row_offset +  1] << 16)|
			(DKC1[row_offset + 16] <<  8)|
			(DKC1[row_offset + 17] <<  0));
			qPixelRow = ((qPixelRow & 0x00200400)|
			((qPixelRow & 0x00400801) <<  3)|
			((qPixelRow & 0x80100200) >>  3)|
			((qPixelRow & 0x00801002) <<  6)|
			((qPixelRow & 0x40080100) >>  6)|
			((qPixelRow & 0x00002004) <<  9)|
			((qPixelRow & 0x20040000) >>  9)|
			((qPixelRow & 0x00004008) << 12)|
			((qPixelRow & 0x10020000) >> 12)|
			((qPixelRow & 0x00008010) << 15)|
			((qPixelRow & 0x08010000) >> 15)|
			((qPixelRow & 0x00000020) << 18)|
			((qPixelRow & 0x04000000) >> 18)|
			((qPixelRow & 0x00000040) << 21)|
			((qPixelRow & 0x02000000) >> 21)|
			((qPixelRow & 0x00000080) << 24)|
			((qPixelRow & 0x01000000) >> 24));
			_putpixel32(rend_target, rend_x+8+7, rend_y+row+8, objectpallete[qPixelRow & 0x0000000F]); qPixelRow >>= 4;
			_putpixel32(rend_target, rend_x+8+6, rend_y+row+8, objectpallete[qPixelRow & 0x0000000F]); qPixelRow >>= 4;
			_putpixel32(rend_target, rend_x+8+5, rend_y+row+8, objectpallete[qPixelRow & 0x0000000F]); qPixelRow >>= 4;
			_putpixel32(rend_target, rend_x+8+4, rend_y+row+8, objectpallete[qPixelRow & 0x0000000F]); qPixelRow >>= 4;
			_putpixel32(rend_target, rend_x+8+3, rend_y+row+8, objectpallete[qPixelRow & 0x0000000F]); qPixelRow >>= 4;
			_putpixel32(rend_target, rend_x+8+2, rend_y+row+8, objectpallete[qPixelRow & 0x0000000F]); qPixelRow >>= 4;
			_putpixel32(rend_target, rend_x+8+1, rend_y+row+8, objectpallete[qPixelRow & 0x0000000F]); qPixelRow >>= 4;
			_putpixel32(rend_target, rend_x+8+0, rend_y+row+8, objectpallete[qPixelRow & 0x0000000F]);
		}
	}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//Cycle through group B 8x8 BMP's.
	for(int curtile = 0; curtile < TilesGroupB; curtile++)
	{
		int rend_x = (128 + (Vsx08(-DKC1[b_offset+(curtile << 1)+0] ^ 0x7F))) - translate_x;
		int rend_y = (128 + (-Vsx08(DKC1[b_offset+(curtile << 1)+1] ^ 0x7F))) - translate_y;
		for(int row = 0;row != 8;row++)
		{
			int row_offset = (StartOfGraphics + (StartGroupB << 5) + (curtile << 5) + (row << 1));
			Hex32 qPixelRow = (
			(DKC1[row_offset +  0] << 24)|
			(DKC1[row_offset +  1] << 16)|
			(DKC1[row_offset + 16] <<  8)|
			(DKC1[row_offset + 17] <<  0));
			qPixelRow = ((qPixelRow & 0x00200400)|
			((qPixelRow & 0x00400801) <<  3)|
			((qPixelRow & 0x80100200) >>  3)|
			((qPixelRow & 0x00801002) <<  6)|
			((qPixelRow & 0x40080100) >>  6)|
			((qPixelRow & 0x00002004) <<  9)|
			((qPixelRow & 0x20040000) >>  9)|
			((qPixelRow & 0x00004008) << 12)|
			((qPixelRow & 0x10020000) >> 12)|
			((qPixelRow & 0x00008010) << 15)|
			((qPixelRow & 0x08010000) >> 15)|
			((qPixelRow & 0x00000020) << 18)|
			((qPixelRow & 0x04000000) >> 18)|
			((qPixelRow & 0x00000040) << 21)|
			((qPixelRow & 0x02000000) >> 21)|
			((qPixelRow & 0x00000080) << 24)|
			((qPixelRow & 0x01000000) >> 24));
			_putpixel32(rend_target, rend_x+7, rend_y+row, objectpallete[qPixelRow & 0x0000000F]); qPixelRow >>= 4;
			_putpixel32(rend_target, rend_x+6, rend_y+row, objectpallete[qPixelRow & 0x0000000F]); qPixelRow >>= 4;
			_putpixel32(rend_target, rend_x+5, rend_y+row, objectpallete[qPixelRow & 0x0000000F]); qPixelRow >>= 4;
			_putpixel32(rend_target, rend_x+4, rend_y+row, objectpallete[qPixelRow & 0x0000000F]); qPixelRow >>= 4;
			_putpixel32(rend_target, rend_x+3, rend_y+row, objectpallete[qPixelRow & 0x0000000F]); qPixelRow >>= 4;
			_putpixel32(rend_target, rend_x+2, rend_y+row, objectpallete[qPixelRow & 0x0000000F]); qPixelRow >>= 4;
			_putpixel32(rend_target, rend_x+1, rend_y+row, objectpallete[qPixelRow & 0x0000000F]); qPixelRow >>= 4;
			_putpixel32(rend_target, rend_x+0, rend_y+row, objectpallete[qPixelRow & 0x0000000F]);
		}
	}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//Cycle through group C 8x8 BMP's.
	for(int curtile = 0; curtile < TilesGroupC; curtile++)
	{
		//DebugLog((string("......loopC ")+int2hexstr(curtile,3)));
		//DebugLog((string(".........address is ")+int2hexstr(c_offset+(curtile << 1)+0,8)));
		//DebugLog((string(".........address is ")+int2hexstr(c_offset+(curtile << 1)+1,8)));
		int rend_x = (128 + (Vsx08(-DKC1[c_offset+(curtile << 1)+0] ^ 0x7F))) - translate_x;
		//DebugLog((string(".........rend_x is ")+int2hexstr(rend_x,8)));
		int rend_y = (128 + (-Vsx08(DKC1[c_offset+(curtile << 1)+1] ^ 0x7F))) - translate_y;
		//DebugLog((string(".........rend_y is ")+int2hexstr(rend_y,8)));
		for(int row = 0;row != 8;row++)
		{
			int row_offset = (StartOfGraphics + (StartGroupC << 5) + (curtile << 5) + (row << 1));
			Hex32 qPixelRow = (
			(DKC1[row_offset +  0] << 24)|
			(DKC1[row_offset +  1] << 16)|
			(DKC1[row_offset + 16] <<  8)|
			(DKC1[row_offset + 17] <<  0));
			qPixelRow = ((qPixelRow & 0x00200400)|
			((qPixelRow & 0x00400801) <<  3)|
			((qPixelRow & 0x80100200) >>  3)|
			((qPixelRow & 0x00801002) <<  6)|
			((qPixelRow & 0x40080100) >>  6)|
			((qPixelRow & 0x00002004) <<  9)|
			((qPixelRow & 0x20040000) >>  9)|
			((qPixelRow & 0x00004008) << 12)|
			((qPixelRow & 0x10020000) >> 12)|
			((qPixelRow & 0x00008010) << 15)|
			((qPixelRow & 0x08010000) >> 15)|
			((qPixelRow & 0x00000020) << 18)|
			((qPixelRow & 0x04000000) >> 18)|
			((qPixelRow & 0x00000040) << 21)|
			((qPixelRow & 0x02000000) >> 21)|
			((qPixelRow & 0x00000080) << 24)|
			((qPixelRow & 0x01000000) >> 24));
			_putpixel32(rend_target, rend_x+7, rend_y+row, objectpallete[qPixelRow & 0x0000000F]); qPixelRow >>= 4;
			_putpixel32(rend_target, rend_x+6, rend_y+row, objectpallete[qPixelRow & 0x0000000F]); qPixelRow >>= 4;
			_putpixel32(rend_target, rend_x+5, rend_y+row, objectpallete[qPixelRow & 0x0000000F]); qPixelRow >>= 4;
			_putpixel32(rend_target, rend_x+4, rend_y+row, objectpallete[qPixelRow & 0x0000000F]); qPixelRow >>= 4;
			_putpixel32(rend_target, rend_x+3, rend_y+row, objectpallete[qPixelRow & 0x0000000F]); qPixelRow >>= 4;
			_putpixel32(rend_target, rend_x+2, rend_y+row, objectpallete[qPixelRow & 0x0000000F]); qPixelRow >>= 4;
			_putpixel32(rend_target, rend_x+1, rend_y+row, objectpallete[qPixelRow & 0x0000000F]); qPixelRow >>= 4;
			_putpixel32(rend_target, rend_x+0, rend_y+row, objectpallete[qPixelRow & 0x0000000F]);
		}
	}
    //BFA.SavePNG(rend_target,(string("Data\\")+int2hexstr(param_spritenumber,6)+"-"+int2hexstr(object_pallete,6)+".png"));
	return rend_target;
}
BITMAP* RE_DKC1_DualSprite(int param_spritenumber, int param_spritenumber2, int object_pallete, int object_pallete2, SRect* rect, bool invert)
{
	if(invert)
	{//We want to handle inverting the draw order by swapping params here.
		int temp = param_spritenumber;
		param_spritenumber = param_spritenumber2;
		param_spritenumber2 = temp;
		temp = object_pallete;
		object_pallete = object_pallete2;
		object_pallete2 = temp;
	}
	safe_array(int, objectpallete, 0x00FF00FF,
	snespal2rgba(DKC1(object_pallete+0x00,2)),
	snespal2rgba(DKC1(object_pallete+0x02,2)),
	snespal2rgba(DKC1(object_pallete+0x04,2)),
	snespal2rgba(DKC1(object_pallete+0x06,2)),
	snespal2rgba(DKC1(object_pallete+0x08,2)),
	snespal2rgba(DKC1(object_pallete+0x0A,2)),
	snespal2rgba(DKC1(object_pallete+0x0C,2)),
	snespal2rgba(DKC1(object_pallete+0x0E,2)),
	snespal2rgba(DKC1(object_pallete+0x10,2)),
	snespal2rgba(DKC1(object_pallete+0x12,2)),
	snespal2rgba(DKC1(object_pallete+0x14,2)),
	snespal2rgba(DKC1(object_pallete+0x16,2)),
	snespal2rgba(DKC1(object_pallete+0x18,2)),
	snespal2rgba(DKC1(object_pallete+0x1A,2)),
	snespal2rgba(DKC1(object_pallete+0x1C,2)));
	safe_array(int, objectpallete2, 0x00FF00FF,
	snespal2rgba(DKC1(object_pallete2+0x00,2)),
	snespal2rgba(DKC1(object_pallete2+0x02,2)),
	snespal2rgba(DKC1(object_pallete2+0x04,2)),
	snespal2rgba(DKC1(object_pallete2+0x06,2)),
	snespal2rgba(DKC1(object_pallete2+0x08,2)),
	snespal2rgba(DKC1(object_pallete2+0x0A,2)),
	snespal2rgba(DKC1(object_pallete2+0x0C,2)),
	snespal2rgba(DKC1(object_pallete2+0x0E,2)),
	snespal2rgba(DKC1(object_pallete2+0x10,2)),
	snespal2rgba(DKC1(object_pallete2+0x12,2)),
	snespal2rgba(DKC1(object_pallete2+0x14,2)),
	snespal2rgba(DKC1(object_pallete2+0x16,2)),
	snespal2rgba(DKC1(object_pallete2+0x18,2)),
	snespal2rgba(DKC1(object_pallete2+0x1A,2)),
	snespal2rgba(DKC1(object_pallete2+0x1C,2)));
	
	const Hex32 SPRITE_PTR_TABLE = DKCRE::gbl->SpriteBank + DKCRE::gbl->SpriteTable;
    const Hex32 SPRITE_DATA_OFFSET = (DKC1(SPRITE_PTR_TABLE + param_spritenumber,4) & 0x003FFFFF);
    const Hex32 SPRITE_DATA_OFFSET2 = (DKC1(SPRITE_PTR_TABLE + param_spritenumber2,4) & 0x003FFFFF);
    if(!SPRITE_DATA_OFFSET || !SPRITE_DATA_OFFSET2)
    {
    	BITMAP* error = create_bitmap_ex(32, 8, 8);
		clear_to_color(error, 0xFFFF0000);
		return error;
    }
    const int TilesGroupA = (DKC1[SPRITE_DATA_OFFSET + 0] & 0xFF); // Number of 16� tiles in Group A
    const int TilesGroupB = (DKC1[SPRITE_DATA_OFFSET + 1] & 0xFF); // Number of 8� tiles in Group B
    const int StartGroupB = (DKC1[SPRITE_DATA_OFFSET + 2] & 0xFF); // Start of Group B 8� tiles relative to start of graphics (Measured in 8� tiles)
    const int TilesGroupC = (DKC1[SPRITE_DATA_OFFSET + 3] & 0xFF); // Number of 8� tiles in Group C
    const int StartGroupC = (DKC1[SPRITE_DATA_OFFSET + 4] & 0xFF); // Start of Group C 8� tiles relative to start of graphics (Measured in 8� tiles)
    const int PositionDataSize = ((TilesGroupA + TilesGroupB + TilesGroupC) << 1);
	const int a_offset = (SPRITE_DATA_OFFSET + 8);
	const int b_offset = ((SPRITE_DATA_OFFSET + 8) + (TilesGroupA << 1));
	const int c_offset = ((SPRITE_DATA_OFFSET + 8) + (TilesGroupA << 1) + (TilesGroupB << 1));
	const int TilesGroupA2 = (DKC1[SPRITE_DATA_OFFSET2 + 0] & 0xFF); // Number of 16� tiles in Group A
    const int TilesGroupB2 = (DKC1[SPRITE_DATA_OFFSET2 + 1] & 0xFF); // Number of 8� tiles in Group B
    const int StartGroupB2 = (DKC1[SPRITE_DATA_OFFSET2 + 2] & 0xFF); // Start of Group B 8� tiles relative to start of graphics (Measured in 8� tiles)
    const int TilesGroupC2 = (DKC1[SPRITE_DATA_OFFSET2 + 3] & 0xFF); // Number of 8� tiles in Group C
    const int StartGroupC2 = (DKC1[SPRITE_DATA_OFFSET2 + 4] & 0xFF); // Start of Group C 8� tiles relative to start of graphics (Measured in 8� tiles)
    const int PositionDataSize2 = ((TilesGroupA2 + TilesGroupB2 + TilesGroupC2) << 1);
	const int a_offset2 = (SPRITE_DATA_OFFSET2 + 8);
	const int b_offset2 = ((SPRITE_DATA_OFFSET2 + 8) + (TilesGroupA2 << 1));
	const int c_offset2 = ((SPRITE_DATA_OFFSET2 + 8) + (TilesGroupA2 << 1) + (TilesGroupB2 << 1));
	
	int translate_x = 0, translate_y = 0;
	BITMAP* rend_target = NULL;
	if(rect)
	{
		rect->L = 0x7FFFFFFF;
		rect->T = 0x7FFFFFFF;
		rect->R = 0x80000001;
		rect->B = 0x80000001;
		for(int curtile = 0; curtile != TilesGroupA; curtile++)
		{
			int rend_x = (128 + (Vsx08(-DKC1[a_offset+(curtile << 1)+0] ^ 0x7F)));
			int rend_y = (128 + (-Vsx08(DKC1[a_offset+(curtile << 1)+1] ^ 0x7F)));
			if(rend_x < rect->L) rect->L = rend_x;
			if(rend_y < rect->T) rect->T = rend_y;
			if((rend_x+16) > rect->R) rect->R = (rend_x+16);
			if((rend_y+16) > rect->B) rect->B = (rend_y+16);
		}
		for(int curtile = 0; curtile != TilesGroupB; curtile++)
		{
			int rend_x = (128 + (Vsx08(-DKC1[b_offset+(curtile << 1)+0] ^ 0x7F)));
			int rend_y = (128 + (-Vsx08(DKC1[b_offset+(curtile << 1)+1] ^ 0x7F)));
			if(rend_x < rect->L) rect->L = rend_x;
			if(rend_y < rect->T) rect->T = rend_y;
			if((rend_x+8) > rect->R) rect->R = (rend_x+8);
			if((rend_y+8) > rect->B) rect->B = (rend_y+8);
		}
		for(int curtile = 0; curtile != TilesGroupC; curtile++)
		{
			int rend_x = (128 + (Vsx08(-DKC1[c_offset+(curtile << 1)+0] ^ 0x7F)));
			int rend_y = (128 + (-Vsx08(DKC1[c_offset+(curtile << 1)+1] ^ 0x7F)));
			if(rend_x < rect->L) rect->L = rend_x;
			if(rend_y < rect->T) rect->T = rend_y;
			if((rend_x+8) > rect->R) rect->R = (rend_x+8);
			if((rend_y+8) > rect->B) rect->B = (rend_y+8);
		}
		for(int curtile = 0; curtile != TilesGroupA2; curtile++)
		{
			int rend_x = (128 + (Vsx08(-DKC1[a_offset2+(curtile << 1)+0] ^ 0x7F)));
			int rend_y = (128 + (-Vsx08(DKC1[a_offset2+(curtile << 1)+1] ^ 0x7F)));
			if(rend_x < rect->L) rect->L = rend_x;
			if(rend_y < rect->T) rect->T = rend_y;
			if((rend_x+16) > rect->R) rect->R = (rend_x+16);
			if((rend_y+16) > rect->B) rect->B = (rend_y+16);
		}
		for(int curtile = 0; curtile != TilesGroupB2; curtile++)
		{
			int rend_x = (128 + (Vsx08(-DKC1[b_offset2+(curtile << 1)+0] ^ 0x7F)));
			int rend_y = (128 + (-Vsx08(DKC1[b_offset2+(curtile << 1)+1] ^ 0x7F)));
			if(rend_x < rect->L) rect->L = rend_x;
			if(rend_y < rect->T) rect->T = rend_y;
			if((rend_x+8) > rect->R) rect->R = (rend_x+8);
			if((rend_y+8) > rect->B) rect->B = (rend_y+8);
		}
		for(int curtile = 0; curtile != TilesGroupC2; curtile++)
		{
			int rend_x = (128 + (Vsx08(-DKC1[c_offset2+(curtile << 1)+0] ^ 0x7F)));
			int rend_y = (128 + (-Vsx08(DKC1[c_offset2+(curtile << 1)+1] ^ 0x7F)));
			if(rend_x < rect->L) rect->L = rend_x;
			if(rend_y < rect->T) rect->T = rend_y;
			if((rend_x+8) > rect->R) rect->R = (rend_x+8);
			if((rend_y+8) > rect->B) rect->B = (rend_y+8);
		}
		rend_target = create_bitmap_ex(32, (rect->R - rect->L), (rect->B - rect->T));
		clear_to_color(rend_target, 0x00FF00FF);
		translate_x = rect->L;
		translate_y = rect->T;
	}
	else
	{
		rend_target = create_bitmap_ex(32, 256, 256);
		clear_to_color(rend_target, 0x00FF00FF);
	}
	
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//Cycle through group A 16x16 BMP's.
	int StartOfGraphics = (SPRITE_DATA_OFFSET + PositionDataSize + 8);
	for(int curtile = 0; curtile != TilesGroupA; curtile++)
	{
		int rend_x = (128 + (Vsx08(-DKC1[a_offset+(curtile << 1)+0] ^ 0x7F))) - translate_x;
		int rend_y = (128 + (-Vsx08(DKC1[a_offset+(curtile << 1)+1] ^ 0x7F))) - translate_y;
		for(int row = 0;row != 8;row++)
		{
			int row_offset = StartOfGraphics + ((curtile >> 3) << 10) + ((curtile & 7) << 6) + 0x00 + (row << 1);
			Hex32 qPixelRow = (
			(DKC1[row_offset +  0] << 24)|
			(DKC1[row_offset +  1] << 16)|
			(DKC1[row_offset + 16] <<  8)|
			(DKC1[row_offset + 17] <<  0));
			qPixelRow = ((qPixelRow & 0x00200400)|
			((qPixelRow & 0x00400801) <<  3)|
			((qPixelRow & 0x80100200) >>  3)|
			((qPixelRow & 0x00801002) <<  6)|
			((qPixelRow & 0x40080100) >>  6)|
			((qPixelRow & 0x00002004) <<  9)|
			((qPixelRow & 0x20040000) >>  9)|
			((qPixelRow & 0x00004008) << 12)|
			((qPixelRow & 0x10020000) >> 12)|
			((qPixelRow & 0x00008010) << 15)|
			((qPixelRow & 0x08010000) >> 15)|
			((qPixelRow & 0x00000020) << 18)|
			((qPixelRow & 0x04000000) >> 18)|
			((qPixelRow & 0x00000040) << 21)|
			((qPixelRow & 0x02000000) >> 21)|
			((qPixelRow & 0x00000080) << 24)|
			((qPixelRow & 0x01000000) >> 24));
			_putpixel32(rend_target, rend_x+7, rend_y+row, objectpallete[qPixelRow & 0x0000000F]); qPixelRow >>= 4;
			_putpixel32(rend_target, rend_x+6, rend_y+row, objectpallete[qPixelRow & 0x0000000F]); qPixelRow >>= 4;
			_putpixel32(rend_target, rend_x+5, rend_y+row, objectpallete[qPixelRow & 0x0000000F]); qPixelRow >>= 4;
			_putpixel32(rend_target, rend_x+4, rend_y+row, objectpallete[qPixelRow & 0x0000000F]); qPixelRow >>= 4;
			_putpixel32(rend_target, rend_x+3, rend_y+row, objectpallete[qPixelRow & 0x0000000F]); qPixelRow >>= 4;
			_putpixel32(rend_target, rend_x+2, rend_y+row, objectpallete[qPixelRow & 0x0000000F]); qPixelRow >>= 4;
			_putpixel32(rend_target, rend_x+1, rend_y+row, objectpallete[qPixelRow & 0x0000000F]); qPixelRow >>= 4;
			_putpixel32(rend_target, rend_x+0, rend_y+row, objectpallete[qPixelRow & 0x0000000F]);
			row_offset += 0x20;
			qPixelRow = (
			(DKC1[row_offset +  0] << 24)|
			(DKC1[row_offset +  1] << 16)|
			(DKC1[row_offset + 16] <<  8)|
			(DKC1[row_offset + 17] <<  0));
			qPixelRow = ((qPixelRow & 0x00200400)|
			((qPixelRow & 0x00400801) <<  3)|
			((qPixelRow & 0x80100200) >>  3)|
			((qPixelRow & 0x00801002) <<  6)|
			((qPixelRow & 0x40080100) >>  6)|
			((qPixelRow & 0x00002004) <<  9)|
			((qPixelRow & 0x20040000) >>  9)|
			((qPixelRow & 0x00004008) << 12)|
			((qPixelRow & 0x10020000) >> 12)|
			((qPixelRow & 0x00008010) << 15)|
			((qPixelRow & 0x08010000) >> 15)|
			((qPixelRow & 0x00000020) << 18)|
			((qPixelRow & 0x04000000) >> 18)|
			((qPixelRow & 0x00000040) << 21)|
			((qPixelRow & 0x02000000) >> 21)|
			((qPixelRow & 0x00000080) << 24)|
			((qPixelRow & 0x01000000) >> 24));
			_putpixel32(rend_target, rend_x+8+7, rend_y+row, objectpallete[qPixelRow & 0x0000000F]); qPixelRow >>= 4;
			_putpixel32(rend_target, rend_x+8+6, rend_y+row, objectpallete[qPixelRow & 0x0000000F]); qPixelRow >>= 4;
			_putpixel32(rend_target, rend_x+8+5, rend_y+row, objectpallete[qPixelRow & 0x0000000F]); qPixelRow >>= 4;
			_putpixel32(rend_target, rend_x+8+4, rend_y+row, objectpallete[qPixelRow & 0x0000000F]); qPixelRow >>= 4;
			_putpixel32(rend_target, rend_x+8+3, rend_y+row, objectpallete[qPixelRow & 0x0000000F]); qPixelRow >>= 4;
			_putpixel32(rend_target, rend_x+8+2, rend_y+row, objectpallete[qPixelRow & 0x0000000F]); qPixelRow >>= 4;
			_putpixel32(rend_target, rend_x+8+1, rend_y+row, objectpallete[qPixelRow & 0x0000000F]); qPixelRow >>= 4;
			_putpixel32(rend_target, rend_x+8+0, rend_y+row, objectpallete[qPixelRow & 0x0000000F]);
			row_offset -= 0x20;
			row_offset += (((curtile >> 3) == (TilesGroupA >> 3))?(((TilesGroupA & 7) << 6) + (TilesGroupB << 5)):512);
			qPixelRow = (
			(DKC1[row_offset +  0] << 24)|
			(DKC1[row_offset +  1] << 16)|
			(DKC1[row_offset + 16] <<  8)|
			(DKC1[row_offset + 17] <<  0));
			qPixelRow = ((qPixelRow & 0x00200400)|
			((qPixelRow & 0x00400801) <<  3)|
			((qPixelRow & 0x80100200) >>  3)|
			((qPixelRow & 0x00801002) <<  6)|
			((qPixelRow & 0x40080100) >>  6)|
			((qPixelRow & 0x00002004) <<  9)|
			((qPixelRow & 0x20040000) >>  9)|
			((qPixelRow & 0x00004008) << 12)|
			((qPixelRow & 0x10020000) >> 12)|
			((qPixelRow & 0x00008010) << 15)|
			((qPixelRow & 0x08010000) >> 15)|
			((qPixelRow & 0x00000020) << 18)|
			((qPixelRow & 0x04000000) >> 18)|
			((qPixelRow & 0x00000040) << 21)|
			((qPixelRow & 0x02000000) >> 21)|
			((qPixelRow & 0x00000080) << 24)|
			((qPixelRow & 0x01000000) >> 24));
			_putpixel32(rend_target, rend_x+7, rend_y+row+8, objectpallete[qPixelRow & 0x0000000F]); qPixelRow >>= 4;
			_putpixel32(rend_target, rend_x+6, rend_y+row+8, objectpallete[qPixelRow & 0x0000000F]); qPixelRow >>= 4;
			_putpixel32(rend_target, rend_x+5, rend_y+row+8, objectpallete[qPixelRow & 0x0000000F]); qPixelRow >>= 4;
			_putpixel32(rend_target, rend_x+4, rend_y+row+8, objectpallete[qPixelRow & 0x0000000F]); qPixelRow >>= 4;
			_putpixel32(rend_target, rend_x+3, rend_y+row+8, objectpallete[qPixelRow & 0x0000000F]); qPixelRow >>= 4;
			_putpixel32(rend_target, rend_x+2, rend_y+row+8, objectpallete[qPixelRow & 0x0000000F]); qPixelRow >>= 4;
			_putpixel32(rend_target, rend_x+1, rend_y+row+8, objectpallete[qPixelRow & 0x0000000F]); qPixelRow >>= 4;
			_putpixel32(rend_target, rend_x+0, rend_y+row+8, objectpallete[qPixelRow & 0x0000000F]);
			row_offset += 0x20;
			qPixelRow = (
			(DKC1[row_offset +  0] << 24)|
			(DKC1[row_offset +  1] << 16)|
			(DKC1[row_offset + 16] <<  8)|
			(DKC1[row_offset + 17] <<  0));
			qPixelRow = ((qPixelRow & 0x00200400)|
			((qPixelRow & 0x00400801) <<  3)|
			((qPixelRow & 0x80100200) >>  3)|
			((qPixelRow & 0x00801002) <<  6)|
			((qPixelRow & 0x40080100) >>  6)|
			((qPixelRow & 0x00002004) <<  9)|
			((qPixelRow & 0x20040000) >>  9)|
			((qPixelRow & 0x00004008) << 12)|
			((qPixelRow & 0x10020000) >> 12)|
			((qPixelRow & 0x00008010) << 15)|
			((qPixelRow & 0x08010000) >> 15)|
			((qPixelRow & 0x00000020) << 18)|
			((qPixelRow & 0x04000000) >> 18)|
			((qPixelRow & 0x00000040) << 21)|
			((qPixelRow & 0x02000000) >> 21)|
			((qPixelRow & 0x00000080) << 24)|
			((qPixelRow & 0x01000000) >> 24));
			_putpixel32(rend_target, rend_x+8+7, rend_y+row+8, objectpallete[qPixelRow & 0x0000000F]); qPixelRow >>= 4;
			_putpixel32(rend_target, rend_x+8+6, rend_y+row+8, objectpallete[qPixelRow & 0x0000000F]); qPixelRow >>= 4;
			_putpixel32(rend_target, rend_x+8+5, rend_y+row+8, objectpallete[qPixelRow & 0x0000000F]); qPixelRow >>= 4;
			_putpixel32(rend_target, rend_x+8+4, rend_y+row+8, objectpallete[qPixelRow & 0x0000000F]); qPixelRow >>= 4;
			_putpixel32(rend_target, rend_x+8+3, rend_y+row+8, objectpallete[qPixelRow & 0x0000000F]); qPixelRow >>= 4;
			_putpixel32(rend_target, rend_x+8+2, rend_y+row+8, objectpallete[qPixelRow & 0x0000000F]); qPixelRow >>= 4;
			_putpixel32(rend_target, rend_x+8+1, rend_y+row+8, objectpallete[qPixelRow & 0x0000000F]); qPixelRow >>= 4;
			_putpixel32(rend_target, rend_x+8+0, rend_y+row+8, objectpallete[qPixelRow & 0x0000000F]);
		}
	}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//Cycle through group B 8x8 BMP's.
	for(int curtile = 0; curtile != TilesGroupB; curtile++)
	{
		int rend_x = (128 + (Vsx08(-DKC1[b_offset+(curtile << 1)+0] ^ 0x7F))) - translate_x;
		int rend_y = (128 + (-Vsx08(DKC1[b_offset+(curtile << 1)+1] ^ 0x7F))) - translate_y;
		for(int row = 0;row != 8;row++)
		{
			int row_offset = (StartOfGraphics + (StartGroupB << 5) + (curtile << 5) + (row << 1));
			Hex32 qPixelRow = (
			(DKC1[row_offset +  0] << 24)|
			(DKC1[row_offset +  1] << 16)|
			(DKC1[row_offset + 16] <<  8)|
			(DKC1[row_offset + 17] <<  0));
			qPixelRow = ((qPixelRow & 0x00200400)|
			((qPixelRow & 0x00400801) <<  3)|
			((qPixelRow & 0x80100200) >>  3)|
			((qPixelRow & 0x00801002) <<  6)|
			((qPixelRow & 0x40080100) >>  6)|
			((qPixelRow & 0x00002004) <<  9)|
			((qPixelRow & 0x20040000) >>  9)|
			((qPixelRow & 0x00004008) << 12)|
			((qPixelRow & 0x10020000) >> 12)|
			((qPixelRow & 0x00008010) << 15)|
			((qPixelRow & 0x08010000) >> 15)|
			((qPixelRow & 0x00000020) << 18)|
			((qPixelRow & 0x04000000) >> 18)|
			((qPixelRow & 0x00000040) << 21)|
			((qPixelRow & 0x02000000) >> 21)|
			((qPixelRow & 0x00000080) << 24)|
			((qPixelRow & 0x01000000) >> 24));
			_putpixel32(rend_target, rend_x+7, rend_y+row, objectpallete[qPixelRow & 0x0000000F]); qPixelRow >>= 4;
			_putpixel32(rend_target, rend_x+6, rend_y+row, objectpallete[qPixelRow & 0x0000000F]); qPixelRow >>= 4;
			_putpixel32(rend_target, rend_x+5, rend_y+row, objectpallete[qPixelRow & 0x0000000F]); qPixelRow >>= 4;
			_putpixel32(rend_target, rend_x+4, rend_y+row, objectpallete[qPixelRow & 0x0000000F]); qPixelRow >>= 4;
			_putpixel32(rend_target, rend_x+3, rend_y+row, objectpallete[qPixelRow & 0x0000000F]); qPixelRow >>= 4;
			_putpixel32(rend_target, rend_x+2, rend_y+row, objectpallete[qPixelRow & 0x0000000F]); qPixelRow >>= 4;
			_putpixel32(rend_target, rend_x+1, rend_y+row, objectpallete[qPixelRow & 0x0000000F]); qPixelRow >>= 4;
			_putpixel32(rend_target, rend_x+0, rend_y+row, objectpallete[qPixelRow & 0x0000000F]);
		}
	}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//Cycle through group C 8x8 BMP's.
	for(int curtile = 0; curtile != TilesGroupC; curtile++)
	{
		int rend_x = (128 + (Vsx08(-DKC1[c_offset+(curtile << 1)+0] ^ 0x7F))) - translate_x;
		int rend_y = (128 + (-Vsx08(DKC1[c_offset+(curtile << 1)+1] ^ 0x7F))) - translate_y;
		for(int row = 0;row != 8;row++)
		{
			int row_offset = (StartOfGraphics + (StartGroupC << 5) + (curtile << 5) + (row << 1));
			Hex32 qPixelRow = (
			(DKC1[row_offset +  0] << 24)|
			(DKC1[row_offset +  1] << 16)|
			(DKC1[row_offset + 16] <<  8)|
			(DKC1[row_offset + 17] <<  0));
			qPixelRow = ((qPixelRow & 0x00200400)|
			((qPixelRow & 0x00400801) <<  3)|
			((qPixelRow & 0x80100200) >>  3)|
			((qPixelRow & 0x00801002) <<  6)|
			((qPixelRow & 0x40080100) >>  6)|
			((qPixelRow & 0x00002004) <<  9)|
			((qPixelRow & 0x20040000) >>  9)|
			((qPixelRow & 0x00004008) << 12)|
			((qPixelRow & 0x10020000) >> 12)|
			((qPixelRow & 0x00008010) << 15)|
			((qPixelRow & 0x08010000) >> 15)|
			((qPixelRow & 0x00000020) << 18)|
			((qPixelRow & 0x04000000) >> 18)|
			((qPixelRow & 0x00000040) << 21)|
			((qPixelRow & 0x02000000) >> 21)|
			((qPixelRow & 0x00000080) << 24)|
			((qPixelRow & 0x01000000) >> 24));
			_putpixel32(rend_target, rend_x+7, rend_y+row, objectpallete[qPixelRow & 0x0000000F]); qPixelRow >>= 4;
			_putpixel32(rend_target, rend_x+6, rend_y+row, objectpallete[qPixelRow & 0x0000000F]); qPixelRow >>= 4;
			_putpixel32(rend_target, rend_x+5, rend_y+row, objectpallete[qPixelRow & 0x0000000F]); qPixelRow >>= 4;
			_putpixel32(rend_target, rend_x+4, rend_y+row, objectpallete[qPixelRow & 0x0000000F]); qPixelRow >>= 4;
			_putpixel32(rend_target, rend_x+3, rend_y+row, objectpallete[qPixelRow & 0x0000000F]); qPixelRow >>= 4;
			_putpixel32(rend_target, rend_x+2, rend_y+row, objectpallete[qPixelRow & 0x0000000F]); qPixelRow >>= 4;
			_putpixel32(rend_target, rend_x+1, rend_y+row, objectpallete[qPixelRow & 0x0000000F]); qPixelRow >>= 4;
			_putpixel32(rend_target, rend_x+0, rend_y+row, objectpallete[qPixelRow & 0x0000000F]);
		}
	}
//$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
//$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
//$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
//$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
//$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
//$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
//$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
//$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
	int StartOfGraphics2 = (SPRITE_DATA_OFFSET2 + PositionDataSize2 + 8);
	for(int curtile = 0; curtile != TilesGroupA2; curtile++)
	{
		int rend_x = (128 + (Vsx08(-DKC1[a_offset2+(curtile << 1)+0] ^ 0x7F))) - translate_x;
		int rend_y = (128 + (-Vsx08(DKC1[a_offset2+(curtile << 1)+1] ^ 0x7F))) - translate_y;
		for(int row = 0;row != 8;row++)
		{
			int row_offset = StartOfGraphics2 + ((curtile >> 3) << 10) + ((curtile & 7) << 6) + 0x00 + (row << 1);
			Hex32 qPixelRow = (
			(DKC1[row_offset +  0] << 24)|
			(DKC1[row_offset +  1] << 16)|
			(DKC1[row_offset + 16] <<  8)|
			(DKC1[row_offset + 17] <<  0));
			qPixelRow = ((qPixelRow & 0x00200400)|
			((qPixelRow & 0x00400801) <<  3)|
			((qPixelRow & 0x80100200) >>  3)|
			((qPixelRow & 0x00801002) <<  6)|
			((qPixelRow & 0x40080100) >>  6)|
			((qPixelRow & 0x00002004) <<  9)|
			((qPixelRow & 0x20040000) >>  9)|
			((qPixelRow & 0x00004008) << 12)|
			((qPixelRow & 0x10020000) >> 12)|
			((qPixelRow & 0x00008010) << 15)|
			((qPixelRow & 0x08010000) >> 15)|
			((qPixelRow & 0x00000020) << 18)|
			((qPixelRow & 0x04000000) >> 18)|
			((qPixelRow & 0x00000040) << 21)|
			((qPixelRow & 0x02000000) >> 21)|
			((qPixelRow & 0x00000080) << 24)|
			((qPixelRow & 0x01000000) >> 24));
			if(qPixelRow & 0x0000000F)_putpixel32(rend_target, rend_x+7, rend_y+row, objectpallete2[qPixelRow & 0x0000000F]); qPixelRow >>= 4;
			if(qPixelRow & 0x0000000F)_putpixel32(rend_target, rend_x+6, rend_y+row, objectpallete2[qPixelRow & 0x0000000F]); qPixelRow >>= 4;
			if(qPixelRow & 0x0000000F)_putpixel32(rend_target, rend_x+5, rend_y+row, objectpallete2[qPixelRow & 0x0000000F]); qPixelRow >>= 4;
			if(qPixelRow & 0x0000000F)_putpixel32(rend_target, rend_x+4, rend_y+row, objectpallete2[qPixelRow & 0x0000000F]); qPixelRow >>= 4;
			if(qPixelRow & 0x0000000F)_putpixel32(rend_target, rend_x+3, rend_y+row, objectpallete2[qPixelRow & 0x0000000F]); qPixelRow >>= 4;
			if(qPixelRow & 0x0000000F)_putpixel32(rend_target, rend_x+2, rend_y+row, objectpallete2[qPixelRow & 0x0000000F]); qPixelRow >>= 4;
			if(qPixelRow & 0x0000000F)_putpixel32(rend_target, rend_x+1, rend_y+row, objectpallete2[qPixelRow & 0x0000000F]); qPixelRow >>= 4;
			if(qPixelRow & 0x0000000F)_putpixel32(rend_target, rend_x+0, rend_y+row, objectpallete2[qPixelRow & 0x0000000F]);
			row_offset += 0x20;
			qPixelRow = (
			(DKC1[row_offset +  0] << 24)|
			(DKC1[row_offset +  1] << 16)|
			(DKC1[row_offset + 16] <<  8)|
			(DKC1[row_offset + 17] <<  0));
			qPixelRow = ((qPixelRow & 0x00200400)|
			((qPixelRow & 0x00400801) <<  3)|
			((qPixelRow & 0x80100200) >>  3)|
			((qPixelRow & 0x00801002) <<  6)|
			((qPixelRow & 0x40080100) >>  6)|
			((qPixelRow & 0x00002004) <<  9)|
			((qPixelRow & 0x20040000) >>  9)|
			((qPixelRow & 0x00004008) << 12)|
			((qPixelRow & 0x10020000) >> 12)|
			((qPixelRow & 0x00008010) << 15)|
			((qPixelRow & 0x08010000) >> 15)|
			((qPixelRow & 0x00000020) << 18)|
			((qPixelRow & 0x04000000) >> 18)|
			((qPixelRow & 0x00000040) << 21)|
			((qPixelRow & 0x02000000) >> 21)|
			((qPixelRow & 0x00000080) << 24)|
			((qPixelRow & 0x01000000) >> 24));
			if(qPixelRow & 0x0000000F)_putpixel32(rend_target, rend_x+8+7, rend_y+row, objectpallete2[qPixelRow & 0x0000000F]); qPixelRow >>= 4;
			if(qPixelRow & 0x0000000F)_putpixel32(rend_target, rend_x+8+6, rend_y+row, objectpallete2[qPixelRow & 0x0000000F]); qPixelRow >>= 4;
			if(qPixelRow & 0x0000000F)_putpixel32(rend_target, rend_x+8+5, rend_y+row, objectpallete2[qPixelRow & 0x0000000F]); qPixelRow >>= 4;
			if(qPixelRow & 0x0000000F)_putpixel32(rend_target, rend_x+8+4, rend_y+row, objectpallete2[qPixelRow & 0x0000000F]); qPixelRow >>= 4;
			if(qPixelRow & 0x0000000F)_putpixel32(rend_target, rend_x+8+3, rend_y+row, objectpallete2[qPixelRow & 0x0000000F]); qPixelRow >>= 4;
			if(qPixelRow & 0x0000000F)_putpixel32(rend_target, rend_x+8+2, rend_y+row, objectpallete2[qPixelRow & 0x0000000F]); qPixelRow >>= 4;
			if(qPixelRow & 0x0000000F)_putpixel32(rend_target, rend_x+8+1, rend_y+row, objectpallete2[qPixelRow & 0x0000000F]); qPixelRow >>= 4;
			if(qPixelRow & 0x0000000F)_putpixel32(rend_target, rend_x+8+0, rend_y+row, objectpallete2[qPixelRow & 0x0000000F]);
			row_offset -= 0x20;
			row_offset += (((curtile >> 3) == (TilesGroupA2 >> 3))?(((TilesGroupA2 & 7) << 6) + (TilesGroupB2 << 5)):512);
			qPixelRow = (
			(DKC1[row_offset +  0] << 24)|
			(DKC1[row_offset +  1] << 16)|
			(DKC1[row_offset + 16] <<  8)|
			(DKC1[row_offset + 17] <<  0));
			qPixelRow = ((qPixelRow & 0x00200400)|
			((qPixelRow & 0x00400801) <<  3)|
			((qPixelRow & 0x80100200) >>  3)|
			((qPixelRow & 0x00801002) <<  6)|
			((qPixelRow & 0x40080100) >>  6)|
			((qPixelRow & 0x00002004) <<  9)|
			((qPixelRow & 0x20040000) >>  9)|
			((qPixelRow & 0x00004008) << 12)|
			((qPixelRow & 0x10020000) >> 12)|
			((qPixelRow & 0x00008010) << 15)|
			((qPixelRow & 0x08010000) >> 15)|
			((qPixelRow & 0x00000020) << 18)|
			((qPixelRow & 0x04000000) >> 18)|
			((qPixelRow & 0x00000040) << 21)|
			((qPixelRow & 0x02000000) >> 21)|
			((qPixelRow & 0x00000080) << 24)|
			((qPixelRow & 0x01000000) >> 24));
			if(qPixelRow & 0x0000000F)_putpixel32(rend_target, rend_x+7, rend_y+row+8, objectpallete2[qPixelRow & 0x0000000F]); qPixelRow >>= 4;
			if(qPixelRow & 0x0000000F)_putpixel32(rend_target, rend_x+6, rend_y+row+8, objectpallete2[qPixelRow & 0x0000000F]); qPixelRow >>= 4;
			if(qPixelRow & 0x0000000F)_putpixel32(rend_target, rend_x+5, rend_y+row+8, objectpallete2[qPixelRow & 0x0000000F]); qPixelRow >>= 4;
			if(qPixelRow & 0x0000000F)_putpixel32(rend_target, rend_x+4, rend_y+row+8, objectpallete2[qPixelRow & 0x0000000F]); qPixelRow >>= 4;
			if(qPixelRow & 0x0000000F)_putpixel32(rend_target, rend_x+3, rend_y+row+8, objectpallete2[qPixelRow & 0x0000000F]); qPixelRow >>= 4;
			if(qPixelRow & 0x0000000F)_putpixel32(rend_target, rend_x+2, rend_y+row+8, objectpallete2[qPixelRow & 0x0000000F]); qPixelRow >>= 4;
			if(qPixelRow & 0x0000000F)_putpixel32(rend_target, rend_x+1, rend_y+row+8, objectpallete2[qPixelRow & 0x0000000F]); qPixelRow >>= 4;
			if(qPixelRow & 0x0000000F)_putpixel32(rend_target, rend_x+0, rend_y+row+8, objectpallete2[qPixelRow & 0x0000000F]);
			row_offset += 0x20;
			qPixelRow = (
			(DKC1[row_offset +  0] << 24)|
			(DKC1[row_offset +  1] << 16)|
			(DKC1[row_offset + 16] <<  8)|
			(DKC1[row_offset + 17] <<  0));
			qPixelRow = ((qPixelRow & 0x00200400)|
			((qPixelRow & 0x00400801) <<  3)|
			((qPixelRow & 0x80100200) >>  3)|
			((qPixelRow & 0x00801002) <<  6)|
			((qPixelRow & 0x40080100) >>  6)|
			((qPixelRow & 0x00002004) <<  9)|
			((qPixelRow & 0x20040000) >>  9)|
			((qPixelRow & 0x00004008) << 12)|
			((qPixelRow & 0x10020000) >> 12)|
			((qPixelRow & 0x00008010) << 15)|
			((qPixelRow & 0x08010000) >> 15)|
			((qPixelRow & 0x00000020) << 18)|
			((qPixelRow & 0x04000000) >> 18)|
			((qPixelRow & 0x00000040) << 21)|
			((qPixelRow & 0x02000000) >> 21)|
			((qPixelRow & 0x00000080) << 24)|
			((qPixelRow & 0x01000000) >> 24));
			if(qPixelRow & 0x0000000F)_putpixel32(rend_target, rend_x+8+7, rend_y+row+8, objectpallete2[qPixelRow & 0x0000000F]); qPixelRow >>= 4;
			if(qPixelRow & 0x0000000F)_putpixel32(rend_target, rend_x+8+6, rend_y+row+8, objectpallete2[qPixelRow & 0x0000000F]); qPixelRow >>= 4;
			if(qPixelRow & 0x0000000F)_putpixel32(rend_target, rend_x+8+5, rend_y+row+8, objectpallete2[qPixelRow & 0x0000000F]); qPixelRow >>= 4;
			if(qPixelRow & 0x0000000F)_putpixel32(rend_target, rend_x+8+4, rend_y+row+8, objectpallete2[qPixelRow & 0x0000000F]); qPixelRow >>= 4;
			if(qPixelRow & 0x0000000F)_putpixel32(rend_target, rend_x+8+3, rend_y+row+8, objectpallete2[qPixelRow & 0x0000000F]); qPixelRow >>= 4;
			if(qPixelRow & 0x0000000F)_putpixel32(rend_target, rend_x+8+2, rend_y+row+8, objectpallete2[qPixelRow & 0x0000000F]); qPixelRow >>= 4;
			if(qPixelRow & 0x0000000F)_putpixel32(rend_target, rend_x+8+1, rend_y+row+8, objectpallete2[qPixelRow & 0x0000000F]); qPixelRow >>= 4;
			if(qPixelRow & 0x0000000F)_putpixel32(rend_target, rend_x+8+0, rend_y+row+8, objectpallete2[qPixelRow & 0x0000000F]);
		}
	}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//Cycle through group B 8x8 BMP's.
	for(int curtile = 0; curtile != TilesGroupB2; curtile++)
	{
		int rend_x = (128 + (Vsx08(-DKC1[b_offset2+(curtile << 1)+0] ^ 0x7F))) - translate_x;
		int rend_y = (128 + (-Vsx08(DKC1[b_offset2+(curtile << 1)+1] ^ 0x7F))) - translate_y;
		for(int row = 0;row != 8;row++)
		{
			int row_offset = (StartOfGraphics2 + (StartGroupB2 << 5) + (curtile << 5) + (row << 1));
			Hex32 qPixelRow = (
			(DKC1[row_offset +  0] << 24)|
			(DKC1[row_offset +  1] << 16)|
			(DKC1[row_offset + 16] <<  8)|
			(DKC1[row_offset + 17] <<  0));
			qPixelRow = ((qPixelRow & 0x00200400)|
			((qPixelRow & 0x00400801) <<  3)|
			((qPixelRow & 0x80100200) >>  3)|
			((qPixelRow & 0x00801002) <<  6)|
			((qPixelRow & 0x40080100) >>  6)|
			((qPixelRow & 0x00002004) <<  9)|
			((qPixelRow & 0x20040000) >>  9)|
			((qPixelRow & 0x00004008) << 12)|
			((qPixelRow & 0x10020000) >> 12)|
			((qPixelRow & 0x00008010) << 15)|
			((qPixelRow & 0x08010000) >> 15)|
			((qPixelRow & 0x00000020) << 18)|
			((qPixelRow & 0x04000000) >> 18)|
			((qPixelRow & 0x00000040) << 21)|
			((qPixelRow & 0x02000000) >> 21)|
			((qPixelRow & 0x00000080) << 24)|
			((qPixelRow & 0x01000000) >> 24));
			if(qPixelRow & 0x0000000F)_putpixel32(rend_target, rend_x+7, rend_y+row, objectpallete2[qPixelRow & 0x0000000F]); qPixelRow >>= 4;
			if(qPixelRow & 0x0000000F)_putpixel32(rend_target, rend_x+6, rend_y+row, objectpallete2[qPixelRow & 0x0000000F]); qPixelRow >>= 4;
			if(qPixelRow & 0x0000000F)_putpixel32(rend_target, rend_x+5, rend_y+row, objectpallete2[qPixelRow & 0x0000000F]); qPixelRow >>= 4;
			if(qPixelRow & 0x0000000F)_putpixel32(rend_target, rend_x+4, rend_y+row, objectpallete2[qPixelRow & 0x0000000F]); qPixelRow >>= 4;
			if(qPixelRow & 0x0000000F)_putpixel32(rend_target, rend_x+3, rend_y+row, objectpallete2[qPixelRow & 0x0000000F]); qPixelRow >>= 4;
			if(qPixelRow & 0x0000000F)_putpixel32(rend_target, rend_x+2, rend_y+row, objectpallete2[qPixelRow & 0x0000000F]); qPixelRow >>= 4;
			if(qPixelRow & 0x0000000F)_putpixel32(rend_target, rend_x+1, rend_y+row, objectpallete2[qPixelRow & 0x0000000F]); qPixelRow >>= 4;
			if(qPixelRow & 0x0000000F)_putpixel32(rend_target, rend_x+0, rend_y+row, objectpallete2[qPixelRow & 0x0000000F]);
		}
	}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//Cycle through group C 8x8 BMP's.
	for(int curtile = 0; curtile != TilesGroupC2; curtile++)
	{
		int rend_x = (128 + (Vsx08(-DKC1[c_offset2+(curtile << 1)+0] ^ 0x7F))) - translate_x;
		int rend_y = (128 + (-Vsx08(DKC1[c_offset2+(curtile << 1)+1] ^ 0x7F))) - translate_y;
		for(int row = 0;row != 8;row++)
		{
			int row_offset = (StartOfGraphics2 + (StartGroupC2 << 5) + (curtile << 5) + (row << 1));
			Hex32 qPixelRow = (
			(DKC1[row_offset +  0] << 24)|
			(DKC1[row_offset +  1] << 16)|
			(DKC1[row_offset + 16] <<  8)|
			(DKC1[row_offset + 17] <<  0));
			qPixelRow = ((qPixelRow & 0x00200400)|
			((qPixelRow & 0x00400801) <<  3)|
			((qPixelRow & 0x80100200) >>  3)|
			((qPixelRow & 0x00801002) <<  6)|
			((qPixelRow & 0x40080100) >>  6)|
			((qPixelRow & 0x00002004) <<  9)|
			((qPixelRow & 0x20040000) >>  9)|
			((qPixelRow & 0x00004008) << 12)|
			((qPixelRow & 0x10020000) >> 12)|
			((qPixelRow & 0x00008010) << 15)|
			((qPixelRow & 0x08010000) >> 15)|
			((qPixelRow & 0x00000020) << 18)|
			((qPixelRow & 0x04000000) >> 18)|
			((qPixelRow & 0x00000040) << 21)|
			((qPixelRow & 0x02000000) >> 21)|
			((qPixelRow & 0x00000080) << 24)|
			((qPixelRow & 0x01000000) >> 24));
			if(qPixelRow & 0x0000000F)_putpixel32(rend_target, rend_x+7, rend_y+row, objectpallete2[qPixelRow & 0x0000000F]); qPixelRow >>= 4;
			if(qPixelRow & 0x0000000F)_putpixel32(rend_target, rend_x+6, rend_y+row, objectpallete2[qPixelRow & 0x0000000F]); qPixelRow >>= 4;
			if(qPixelRow & 0x0000000F)_putpixel32(rend_target, rend_x+5, rend_y+row, objectpallete2[qPixelRow & 0x0000000F]); qPixelRow >>= 4;
			if(qPixelRow & 0x0000000F)_putpixel32(rend_target, rend_x+4, rend_y+row, objectpallete2[qPixelRow & 0x0000000F]); qPixelRow >>= 4;
			if(qPixelRow & 0x0000000F)_putpixel32(rend_target, rend_x+3, rend_y+row, objectpallete2[qPixelRow & 0x0000000F]); qPixelRow >>= 4;
			if(qPixelRow & 0x0000000F)_putpixel32(rend_target, rend_x+2, rend_y+row, objectpallete2[qPixelRow & 0x0000000F]); qPixelRow >>= 4;
			if(qPixelRow & 0x0000000F)_putpixel32(rend_target, rend_x+1, rend_y+row, objectpallete2[qPixelRow & 0x0000000F]); qPixelRow >>= 4;
			if(qPixelRow & 0x0000000F)_putpixel32(rend_target, rend_x+0, rend_y+row, objectpallete2[qPixelRow & 0x0000000F]);
		}
	}
	/*BFA.SavePNG(rend_target,(string("Data\\Dual-")
	+int2hexstr(param_spritenumber,6)+"-"+int2hexstr(object_pallete,6)+"_"
	+int2hexstr(param_spritenumber2,6)+"-"+int2hexstr(object_pallete2,6)+".png"));*/
	return rend_target;
}
BITMAP* RE_CropSpriteBitmap(BITMAP* input)
{
	int adj_l = 0, adj_t = 0, adj_r = 0, adj_b = 0;
    bool runloop = true;
    for(int yy = 0; runloop && (yy != input->h); yy++)
    {
        for(int xx = 0; runloop && (xx != input->w); xx++)
        {
            if(_getpixel32(input,xx,yy) != 0x00FF00FF)
            {
            	runloop = false;
            	adj_t = yy;
			}
        }
    }
    runloop = true;
    for(int xx = 0; runloop && (xx != input->w); xx++)
    {
        for(int yy = 0; runloop && (yy != input->h); yy++)
        {
            if(_getpixel32(input,xx,yy) != 0x00FF00FF)
            {
            	runloop = false;
            	adj_l = xx;
			}
        }
    }
    runloop = true;
    for(int yy = 0; runloop && (yy != input->h); yy++)
    {
        for(int xx = 0; runloop && (xx != input->w); xx++)
        {
            if(_getpixel32(input,xx,input->h-1-yy) != 0x00FF00FF)
            {
            	runloop = false;
            	adj_b = yy;
			}
        }
    }
    runloop = true;
    for(int xx = 0; runloop && (xx != input->w); xx++)
    {
        for(int yy = 0; runloop && (yy != input->h); yy++)
        {
            if(_getpixel32(input,input->w-1-xx,yy) != 0x00FF00FF)
            {
            	runloop = false;
            	adj_r = xx;
			}
        }
    }
    int cropedge = 
     ((((adj_l < adj_r)?(adj_l):(adj_r)) < ((adj_t < adj_b)?(adj_t):(adj_b)))?
       ((adj_l < adj_r)?(adj_l):(adj_r)):((adj_t < adj_b)?(adj_t):(adj_b)));
    BITMAP* newbmp = create_bitmap_ex(32,input->w-(cropedge*2),input->h-(cropedge*2));
    TzBMP_blit(input, newbmp, cropedge, cropedge, 0, 0, newbmp->w, newbmp->h, 0, TzModeBwNRM);
    Nullify(input);
    return newbmp;
}
