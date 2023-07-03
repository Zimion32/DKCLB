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

BIT ALLEGRO_IS_SHUTDOWN = false;

void Nullify(BMP volatile& object){
	if(ALLEGRO_IS_SHUTDOWN) return;
	if(object){destroy_bitmap(object); object = NULL;}
}
void Nullify(BMP& object){
	if(ALLEGRO_IS_SHUTDOWN) return;
	if(object){destroy_bitmap(object); object = NULL;}
}
void Nullify(BMP& object, STR what){
	if(ALLEGRO_IS_SHUTDOWN) return;
	if(object){destroy_bitmap(object); object = NULL;}
}
void Nullify(SAMPLE*& object){
	if(ALLEGRO_IS_SHUTDOWN) return;
	if(object){destroy_sample(object); object = NULL;}
}
void Nullify(FONT*& object){
	if(ALLEGRO_IS_SHUTDOWN) return;
	if(object){destroy_font(object); object = NULL;}
}
void Nullify2(BMP object){
	if(ALLEGRO_IS_SHUTDOWN) return;
	if(object){destroy_bitmap(object);}
}
