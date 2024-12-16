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

BIT ALLEGRO_IS_INSTALLED = false;
BIT ALLEGRO_IS_SHUTDOWN = false;

static Vx<BMP>     ALLEGRO_NULLIFY_BMPs;
static Vx<SAMPLE*> ALLEGRO_NULLIFY_SAMPLEs;
static Vx<FONT*>   ALLEGRO_NULLIFY_FONTs;

void NullifyF(BMP& object){
	if(ALLEGRO_IS_SHUTDOWN) return;
	if(object){
        destroy_bitmap(object);
        object = NULL;
    }
}
void Nullify(BMP& object){
	if(ALLEGRO_IS_SHUTDOWN) return;
	if(object){
        for(U32 i = 0; i < ALLEGRO_NULLIFY_BMPs.size(); i++){
            if(ALLEGRO_NULLIFY_BMPs[i] == object){object = NULL; return;}
        }
        ALLEGRO_NULLIFY_BMPs.push_back(object); 
        object = NULL;
    }
}
void Nullify(BMP& object, STR what){
	if(ALLEGRO_IS_SHUTDOWN) return;
	if(object){
        for(U32 i = 0; i < ALLEGRO_NULLIFY_BMPs.size(); i++){
            if(ALLEGRO_NULLIFY_BMPs[i] == object){object = NULL; return;}
        }
        ALLEGRO_NULLIFY_BMPs.push_back(object);
        object = NULL;}
}
void Nullify(SAMPLE*& object){
	if(ALLEGRO_IS_SHUTDOWN) return;
	if(object){
        for(U32 i = 0; i < ALLEGRO_NULLIFY_SAMPLEs.size(); i++){
            if(ALLEGRO_NULLIFY_SAMPLEs[i] == object){object = NULL; return;}
        }
	    ALLEGRO_NULLIFY_SAMPLEs.push_back(object);
	    object = NULL;}
}
void Nullify(FONT*& object){
	if(ALLEGRO_IS_SHUTDOWN) return;
	if(object){
        for(U32 i = 0; i < ALLEGRO_NULLIFY_FONTs.size(); i++){
            if(ALLEGRO_NULLIFY_FONTs[i] == object){object = NULL; return;}
        }
        ALLEGRO_NULLIFY_FONTs.push_back(object);
        object = NULL;}
}
void Nullify2(BMP object){
	if(ALLEGRO_IS_SHUTDOWN) return;
	if(object){
        for(U32 i = 0; i < ALLEGRO_NULLIFY_BMPs.size(); i++){
            if(ALLEGRO_NULLIFY_BMPs[i] == object){return;}
        }
        ALLEGRO_NULLIFY_BMPs.push_back(object);
    }
}
void AllegroCompleteNullifications()
{
    if(ALLEGRO_IS_SHUTDOWN) return;
    for(U32 i = 0; i < ALLEGRO_NULLIFY_BMPs.size(); i++){
        destroy_bitmap(ALLEGRO_NULLIFY_BMPs[i]);
    }
    for(U32 i = 0; i < ALLEGRO_NULLIFY_SAMPLEs.size(); i++){
        destroy_sample(ALLEGRO_NULLIFY_SAMPLEs[i]);
    }
    for(U32 i = 0; i < ALLEGRO_NULLIFY_FONTs.size(); i++){
        destroy_font(ALLEGRO_NULLIFY_FONTs[i]);
    }
    ALLEGRO_NULLIFY_BMPs.clear();
    ALLEGRO_NULLIFY_SAMPLEs.clear();
    ALLEGRO_NULLIFY_FONTs.clear();
}
