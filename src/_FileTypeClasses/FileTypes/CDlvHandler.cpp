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
#define FILEIO_CHUNK_LEVEL_SETTINGS 16

BIT CDlvHandler::Save(Sx<CFileInfo> info, Sx<IGameData> data)
{
    CFileIO file;
    try{
        info->WriteHeaderTo(file);
        info->WriteDescriptionTo(file);
        
        Sx<BLevel> level = (Sx<BLevel>)data;
        
        file.SvBeginSection();
        file.SvChunkID(FILEIO_CHUNK_LEVEL_1);
        file.SvHex(level->GetX(),4);
        file.SvHex(level->GetY(),4);
        file.SvHex(level->GetW(),4);
        file.SvHex(level->GetH(),4);
        file.SvHex(level->paths_.size(),4);
        for(IND i = 0; i < level->paths_.size(); i++){
            if(level->paths_[i]){
                level->paths_[i]->Serialize(file);
            }else{
                file.SvHex(0xFFFFFFFF,4); //deleted path
            }
        }
        
        file.SvChunkID(FILEIO_CHUNK_LEVEL_SETTINGS);
        file.SvLUCID(level->pause_screen_lucid_);
        file.SvHex(level->start_script_.size(),4);
        for(IND i = 0; i < level->start_script_.size(); i++){
            file.SvHex(level->start_script_[i],8);
        }
        file.SvHex(level->start_script_constants_table_.size(),4);
        for(IND i = 0; i < level->start_script_constants_table_.size(); i++){
            file.SvHex(level->start_script_constants_table_[i],8);
        }
        file.SvHex(level->stop_script_.size(),4);
        for(IND i = 0; i < level->stop_script_.size(); i++){
            file.SvHex(level->stop_script_[i],8);
        }
        file.SvHex(level->stop_script_constants_table_.size(),4);
        for(IND i = 0; i < level->stop_script_constants_table_.size(); i++){
            file.SvHex(level->stop_script_constants_table_[i],8);
        }
        file.SvHex(level->drift_table_.size(),4);
        for(IND i = 0; i < level->drift_table_.size(); i++){
            file.SvDouble(level->drift_table_[i].slopeAX);
            file.SvDouble(level->drift_table_[i].slopeAY);
            file.SvDouble(level->drift_table_[i].slopeBX);
            file.SvDouble(level->drift_table_[i].slopeBY);
            file.SvDouble(level->drift_table_[i].speed_mul);
            file.SvDouble(level->drift_table_[i].drift);
            file.SvDouble(level->drift_table_[i].accel_div);
            file.SvHex(level->drift_table_[i].flags,8);
        }
        file.SvHex(level->music_track_id_,4);
        file.SvHex(level->music_dynamics_,4);
        file.SvDouble(level->camera_start_x_);
        file.SvDouble(level->camera_start_y_);
        file.SvDouble(level->camera_smoothing_);
        file.SvHex(level->event_names_.size(),4);
        for(IND i = 0; i < level->event_names_.size(); i++){
            file.SvString(level->event_names_[i]);
        }
        file.SvHex(0,4);
        file.SvHex(level->variables_.size(),4);
        for(IND i = 0; i < level->variables_.size(); i++){
            file.SvString(level->variables_[i].name);
            file.SvHex(level->variables_[i].flags, 8);
            file.SvHex(level->variables_[i].type, 4);
            file.SvVsx(level->variables_[i].data_num, 8);
            file.SvDouble(level->variables_[i].data_flo);
            file.SvString(level->variables_[i].data_txt);
        }
        
        file.SvEndSection();
        
        file.SvBeginSectionArray();
        for(IND i = 0; i < level->GetLayerCount(); i++){
            Sx<ILevelLayer> layer0 = level->GetLayer(i);
            
            file.SvBeginSection();
            switch(layer0->LayerType()){
                case 0:{
                    Sx<BLevelObject> layer = (Sx<BLevelObject>)layer0;
                    file.SvChunkID(FILEIO_CHUNK_LAYER_OBJECT);
                    Serialize(file, layer);
                }break;
                case 1:{
                    Sx<BLevelTileMap> layer = (Sx<BLevelTileMap>)layer0;
                    file.SvChunkID(FILEIO_CHUNK_LAYER_TILEMAP);
                    Serialize(file, layer);
                }break;
                case 2:{
                    Sx<BLevelBackground> layer = (Sx<BLevelBackground>)layer0;
                    file.SvChunkID(FILEIO_CHUNK_LAYER_BACKGROUND);
                    Serialize(file, layer);
                }break;
                case 3:{
                    Sx<BLevelBaseBacking> layer = (Sx<BLevelBaseBacking>)layer0;
                    file.SvChunkID(FILEIO_CHUNK_LAYER_BASEBACKING);
                    Serialize(file, layer);
                }break;
                case 4:{
                    Sx<BLevelPhysMap> layer = (Sx<BLevelPhysMap>)layer0;
                    file.SvChunkID(FILEIO_CHUNK_LAYER_PHYSMAP);
                    Serialize(file, layer);
                }break;
                case 5:{
                    Sx<BLevelComposition> layer = (Sx<BLevelComposition>)layer0;
                    file.SvChunkID(FILEIO_CHUNK_LAYER_COMPOSITION);
                    Serialize(file, layer);
                }break;
            }
            file.SvEndSection();
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
