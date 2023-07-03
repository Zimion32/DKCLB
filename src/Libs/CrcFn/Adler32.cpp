#include "include.h"
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
const U32 MOD_ADLER = 65521;
U32 Adler32(U08* data, U32 size)
{
    //Implementation came from Wikipedia!!
    U32 a = 1, b = 0;
    for(U32 index = 0; index < size; index++){
        a = ((a + data[index]) % MOD_ADLER);
        b = ((b + a) % MOD_ADLER);
    }
    return ((b << 16) | a);
}
U32 Adler32(Vx<U08>& data)
{
	return Adler32((U08*)&data[0], data.size());
}
