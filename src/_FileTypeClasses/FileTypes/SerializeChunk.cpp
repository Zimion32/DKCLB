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
#include "SerializeChunk.h"

/**
void Serialize(CFileIO& file, Sx<BLevelObject> layer)
{
    file.SvHex(layer->GetID(), 4);
    file.SvString(layer->GetDesc());
    file.SvHex(layer->GetBlendingTargets(), 4);
    file.SvHex(layer->GetBlendingMode(), 4);
    file.SvHex(layer->GetBlendingFlags(), 4);
    file.SvHex(layer->GetBlendingColor(), 4);
    file.SvHex(layer->GetFlags(), 4); //flags!!!
    file.SvHex(layer->GetLinkColor(), 4);
    file.SvHex(layer->GetExtraUnit()[0], 8);
    file.SvHex(layer->GetExtraUnit()[1], 8);
    file.SvHex(layer->GetVisible(), 1);
    file.SvHex(layer->GetEnabled(), 1);
    file.SvHex(layer->GetSticky(), 1);
    file.SvHex(layer->GetIsPayload(), 1);
    //------------
    for(IND v = 0; v < 256; v++){
        file.SvHex(layer->GetVarInt(v), 8);
    }
    for(IND v = 0; v < 256; v++){
        file.SvDouble(layer->GetVarFlo(v));
    }
    for(IND v = 0; v < 256; v++){
        file.SvHex(layer->GetNamedVarInt(v), 8);
    }
    for(IND v = 0; v < 256; v++){
        file.SvDouble(layer->GetNamedVarFlo(v));
    }
    file.SvString(layer->GetName());
    file.SvLUCID(CFileInfoManager::GetInfo(layer->GetSprite()->GetInfo())->GetLucid());
    file.SvHex(layer->GetAnimID(), 4);
    
    file.SvLUCID(CFileInfoManager::GetInfo(layer->GetEngineBase()->GetInfo())->GetLucid());
    file.SvLUCID(CFileInfoManager::GetInfo(layer->GetEngineDefault()->GetInfo())->GetLucid());
    file.SvLUCID(CFileInfoManager::GetInfo(layer->GetEnginePhysics()->GetInfo())->GetLucid());
    for(IND v = 0; v < 16; v++){
        Sx<BLevelObjectEngine> eng = layer->GetEngineInherit(v);
        if(eng){
            file.SvLUCID(CFileInfoManager::GetInfo(eng->GetInfo())->GetLucid());
        }else{
            file.SvLUCID(LUCID());
        }
    }
    file.SvHex(layer->GetPayload(), 4); //OBJID save
    for(IND v = 0; v < 8; v++){
        file.SvHex(layer->GetEventFlags(v), 4);
    }
    
    file.SvHex(0, 4); //hitbox count always zero due to hitboxes being part of DGraphic's instead
    
    file.SvHex(layer->GetHitBoxIndexSelected(), 4);
    file.SvHex(layer->GetHitBoxIndexSelectedBlock(), 4);
    
    file.SvHex(layer->GetPhysLineCount(), 4);
    for(IND v = 0; v < layer->GetPhysLineCount(); v++){
        BLevelLine pl = layer->GetPhysLine(v);
        file.SvHex(pl.line_.flags, 8);
        file.SvDouble(pl.line_.ax);
        file.SvDouble(pl.line_.ay);
        file.SvDouble(pl.line_.bx);
        file.SvDouble(pl.line_.by);
        file.SvHex(pl.line_.events[0],4);
        file.SvHex(pl.line_.events[1],4);
        file.SvHex(pl.line_.events[2],4);
        file.SvHex(pl.line_.events[3],4);
    }
    file.SvHex(layer->easy_script_.size(), 4);
    for(IND v = 0; v < layer->easy_script_.size(); v++){
        file.SvHex(layer->easy_script_[v], 8);
    }
    file.SvHex(layer->easy_script_constants_table_.size(), 4);
    for(IND v = 0; v < layer->easy_script_constants_table_.size(); v++){
        file.SvHex(layer->easy_script_constants_table_[v], 8);
    }
}
void Serialize(CFileIO& file, Sx<BLevelTileMap> layer)
{
    file.SvHex(layer->GetID(), 4);
    file.SvString(layer->GetDesc());
    file.SvHex(layer->GetBlendingTargets(), 4);
    file.SvHex(layer->GetBlendingMode(), 4);
    file.SvHex(layer->GetBlendingFlags(), 4);
    file.SvHex(layer->GetBlendingColor(), 4);
    file.SvHex(layer->GetFlags(), 4); //flags!!!
    file.SvHex(layer->GetLinkColor(), 4);
    file.SvHex(layer->GetExtraUnit()[0], 8);
    file.SvHex(layer->GetExtraUnit()[1], 8);
    file.SvVsx(layer->GetX(), 4);
    file.SvVsx(layer->GetY(), 4);
    file.SvDouble(layer->GetXSpeed());
    file.SvDouble(layer->GetYSpeed());
    //------------
    file.SvLUCID(CFileInfoManager::GetInfo(layer->set_->data_->GetInfo())->GetLucid());
    file.SvHex(layer->GetW(), 4);
    file.SvHex(layer->GetH(), 4);
    for(IND yy = 0; yy < layer->GetH(); yy++){
        for(IND xx = 0; xx < layer->GetW(); xx++){
            BLevelTileSlot* slot = (*layer)(xx, yy);
            file.SvHex(slot->tile_->index_, 3);
            file.SvHex(slot->flip_, 1);
        }
    }
}
void Serialize(CFileIO& file, Sx<BLevelBackground> layer)
{
    file.SvHex(layer->GetID(), 4);
    file.SvString(layer->GetDesc());
    file.SvHex(layer->GetBlendingTargets(), 4);
    file.SvHex(layer->GetBlendingMode(), 4);
    file.SvHex(layer->GetBlendingFlags(), 4);
    file.SvHex(layer->GetBlendingColor(), 4);
    file.SvHex(layer->GetFlags(), 4); //flags!!!
    file.SvHex(layer->GetLinkColor(), 4);
    file.SvHex(layer->GetExtraUnit()[0], 8);
    file.SvHex(layer->GetExtraUnit()[1], 8);
    file.SvVsx(layer->GetX(), 4);
    file.SvVsx(layer->GetY(), 4);
    file.SvDouble(layer->GetXSpeed());
    file.SvDouble(layer->GetYSpeed());
    //------------
    file.SvLUCID(CFileInfoManager::GetInfo(layer->sprite_->GetInfo())->GetLucid());
}
void Serialize(CFileIO& file, Sx<BLevelBaseBacking> layer)
{
    file.SvHex(layer->GetID(), 4);
    file.SvString(layer->GetDesc());
    file.SvHex(layer->GetBlendingTargets(), 4);
    file.SvHex(layer->GetBlendingMode(), 4);
    file.SvHex(layer->GetBlendingFlags(), 4);
    file.SvHex(layer->GetBlendingColor(), 4);
    file.SvHex(layer->GetFlags(), 4); //flags!!!
    file.SvHex(layer->GetLinkColor(), 4);
    file.SvHex(layer->GetExtraUnit()[0], 8);
    file.SvHex(layer->GetExtraUnit()[1], 8);
    file.SvVsx(layer->GetX(), 4);
    file.SvVsx(layer->GetY(), 4);
    //------------
    file.SvHex(layer->w_, 4);
    file.SvHex(layer->h_, 4);
    file.SvHex(layer->iw_, 4);
    file.SvHex(layer->ih_, 4);
}
void Serialize(CFileIO& file, Sx<BLevelPhysMap> layer)
{
    file.SvHex(layer->GetID(), 4);
    file.SvString(layer->GetDesc());
    file.SvHex(layer->GetBlendingTargets(), 4);
    file.SvHex(layer->GetBlendingMode(), 4);
    file.SvHex(layer->GetBlendingFlags(), 4);
    file.SvHex(layer->GetBlendingColor(), 4);
    file.SvHex(layer->GetFlags(), 4); //flags!!!
    file.SvHex(layer->GetLinkColor(), 4);
    file.SvHex(layer->GetExtraUnit()[0], 8);
    file.SvHex(layer->GetExtraUnit()[1], 8);
    file.SvVsx(layer->GetX(), 4);
    file.SvVsx(layer->GetY(), 4);
    //------------
    file.SvLUCID(CFileInfoManager::GetInfo(layer->set_->data_->GetInfo())->GetLucid());
    file.SvHex(layer->GetW(), 4);
    file.SvHex(layer->GetH(), 4);
    for(IND yy = 0; yy < layer->GetH(); yy++){
        for(IND xx = 0; xx < layer->GetW(); xx++){
            BLevelPhysSlot* slot = (*layer)(xx, yy);
            file.SvHex(slot->tile_->index_, 3);
            file.SvHex(slot->flip_, 1);
        }
    }
}
void Serialize(CFileIO& file, Sx<BLevelComposition> layer)
{
    file.SvHex(layer->GetID(), 4);
    file.SvString(layer->GetDesc());
    file.SvHex(layer->GetBlendingTargets(), 4);
    file.SvHex(layer->GetBlendingMode(), 4);
    file.SvHex(layer->GetBlendingFlags(), 4);
    file.SvHex(layer->GetBlendingColor(), 4);
    file.SvHex(layer->GetFlags(), 4); //flags!!!
    file.SvHex(layer->GetLinkColor(), 4);
    file.SvHex(layer->GetExtraUnit()[0], 8);
    file.SvHex(layer->GetExtraUnit()[1], 8);
    file.SvVsx(layer->GetX(), 4);
    file.SvVsx(layer->GetY(), 4);
    //------------
    file.SvHex(layer->w_, 4);
    file.SvHex(layer->h_, 4);
    file.SvHex(layer->iw_, 4);
    file.SvHex(layer->ih_, 4);
}
*/

