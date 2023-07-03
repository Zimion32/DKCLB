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
#define UNICODE
#define _UNICODE
#define __UNICODE

#include <stdio.h>
#include <fcntl.h>
#include <io.h>
#include <iostream>
#include <fstream>

#define cout std::cout
#define cin std::cin
#define endl std::endl

#include "types.h"
#include "alleg_win.h"
#include "Libs\Nvwa\debug_new.h"
#include "Libs\7zCrypto\7zCrypto.h"
#include "Libs\CrcFn\CRC.h"
#include "Libs\MurmurHash2A.h"
#define M_PI 3.14159265258
#include "math.h"
#include "defines.h"
//==========================================
// Framework Includes
#include "KFramework.h"
//==========================================
// Class Type Includes
#include "Types\dtime.h"
#include "Types\LUCID.h"
#include "Types\EXTHD.h"
//==========================================
// Class Includes
#include "CFileIO.h"
//==========================================
// Includes
#include "auxillary.h"


