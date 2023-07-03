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
#include "include.h"
/**
#define FILEIO_CHUNK_ALTERNATE_PALETTES  8
#define FILEIO_CHUNK_HITBOXES  9
#define FILEIO_CHUNK_DEFAULT_BEHAVIOR  10

BIT CDpxHandler::Save(Sx<CFileInfo> info, Sx<IGameData> data)
{
    CFileIO file;
    try{
        info->WriteHeaderTo(file);
        info->WriteDescriptionTo(file);
        
        Sx<DSprite> sprite = (Sx<DSprite>)data;
        
        file.SvBeginSection();
        
        file.SvChunkID(FILEIO_CHUNK_LEVEL_1);
        file.SvVsx(sprite->GetDrawX(), 4);
        file.SvVsx(sprite->GetDrawY(), 4);
        file.SvString(sprite->GetGameId());
        
        file.SvChunkID(FILEIO_CHUNK_DEFAULT_BEHAVIOR);
        file.SvLUCID(sprite->GetEngine());
        file.SvHex(sprite->GetScript().size(),4);
        for(IND i = 0; i < sprite->GetScript().size(); i++){
            file.SvHex(sprite->GetScript()[i],8);
        }
        file.SvHex(sprite->GetScriptData().size(),4);
        for(IND i = 0; i < sprite->GetScriptData().size(); i++){
            file.SvHex(sprite->GetScriptData()[i],8);
        }
        
        file.SvEndSection();
        
        file.SvBeginSectionArray();
        for(IND i = 0; i < sprite->GetAnimationCount(); i++){
            Sx<DAnimation> anim = sprite->GetAnimation(i);
            
            file.SvBeginSection();
            file.SvChunkID(FILEIO_CHUNK_LEVEL_2);
            file.SvVsx(anim->GetDrawX(), 4);
            file.SvVsx(anim->GetDrawY(), 4);
            file.SvString(anim->GetGameId());
            file.SvEndSection();
            
            file.SvBeginSectionArray();
            for(IND j = 0; j < anim->GetGraphicCount(); j++){
                Sx<DGraphic> gfx = anim->GetGraphic(j);
                
                file.SvBeginSection();
                
                file.SvChunkID(FILEIO_CHUNK_LEVEL_3);
                file.SvPNG(gfx->GetBitmap());
                file.SvVsx(gfx->GetCenterX(), 4);
                file.SvVsx(gfx->GetCenterY(), 4);
                file.SvHex(gfx->GetBlendingFlags(), 4);
                file.SvDouble(gfx->GetDuration());
                file.SvHex(gfx->GetBlendingMode(), 4);
                file.SvHex(gfx->GetBlendingColor(), 4);
                file.SvString(gfx->GetGameId());
                file.SvHex(gfx->GetPaletteSize(), 4);
                Vx<U32> pal = gfx->GetPalette(0);
                for(IND k = 0; k < pal.size(); k++){
                    file.SvHex(pal[k], 4);
                }
                
                file.SvChunkID(FILEIO_CHUNK_ALTERNATE_PALETTES);
                file.SvHex(gfx->GetPaletteAltCount(), 4);
                for(IND k = 0; k < gfx->GetPaletteAltCount(); k++){
                    Vx<U32> pal = gfx->GetPalette(k+1);
                    file.SvHex(pal.size(), 4);
                    for(IND l = 0; l < pal.size(); l++){
                        file.SvHex(pal[l], 4);
                    }
                }
                
                file.SvChunkID(FILEIO_CHUNK_HITBOXES);
                file.SvHex(gfx->GetHitBoxCount(), 4);
                for(IND v = 0; v < gfx->GetHitBoxCount(); v++){
                    SHitBox hb = gfx->GetHitBox(v);
                    file.SvHex(hb.flags, 4);
                    file.SvDouble(hb.l);
                    file.SvDouble(hb.t);
                    file.SvDouble(hb.r);
                    file.SvDouble(hb.b);
                }
                
                file.SvEndSection();
            }
            file.SvEndSectionArray();
        }
        file.SvEndSectionArray();
        file.SvFooter();
        file.Save(info->GetFilePath());
        file.Close();
    }catch(const exception& e){
        file.Close();
        return De(true, TXT("In file \"") + info->GetFilePath() + "\":\r\n\r\n" + e.what());
    }endtry;
    return false;
}
*/
