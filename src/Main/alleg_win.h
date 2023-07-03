#pragma once
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
#include <allegro.h>
#include <winalleg.h>
#define BMP BITMAP*
//Here are some undef's, because windows.h messes with my own function names.
#undef DeleteFile
#undef DeleteFileEx
#undef DeleteFolder //just incase
#undef DeleteDirectory //just incase
#undef CopyFile
#undef CopyFileEx
#undef CopyFolder //just incase
#undef CopyDirectory //just incase
#undef MoveFile
#undef MoveFileEx
#undef MoveFolder //just incase
#undef MoveDirectory //just incase
#undef CreateFolder //just incase
#undef CreateDirectory
void Nullify(BMP volatile& object);
void Nullify(BMP& object);
void Nullify(BMP& object, STR what);
void Nullify(SAMPLE*& object);
void Nullify(FONT*& object);
void Nullify2(BMP object);
