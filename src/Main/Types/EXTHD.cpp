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

dtime EXTHD::GetDateLastSaved()
{
    U64 result = (
        (((U64)(pData[0])) << 32)|
        (((U64)(pData[1])) << 24)|
        (((U64)(pData[2])) << 16)|
        (((U64)(pData[3])) <<  8)|
        (((U64)(pData[4]))      ));
    return result;
}
dtime EXTHD::GetDateFinalized()
{
    U64 result = (
        (((U64)(pData[5])) << 32)|
        (((U64)(pData[6])) << 24)|
        (((U64)(pData[7])) << 16)|
        (((U64)(pData[8])) <<  8)|
        (((U64)(pData[9]))      ));
    return result;
}
dtime EXTHD::GetDateBugFixed()
{
    U64 result = (
        (((U64)(pData[10])) << 32)|
        (((U64)(pData[11])) << 24)|
        (((U64)(pData[12])) << 16)|
        (((U64)(pData[13])) <<  8)|
        (((U64)(pData[14]))      ));
    return result;
}
U64 EXTHD::GetMajorVersion()
{
    U64 result = (
        (((U64)(pData[15])) << 32)|
        (((U64)(pData[16])) << 24)|
        (((U64)(pData[17])) << 16)|
        (((U64)(pData[18])) <<  8)|
        (((U64)(pData[19]))      ));
    return result;
}
U64 EXTHD::GetSaveCountBF()
{
    U32 result = (
        (((U32)(pData[20])) << 24)|
        (((U32)(pData[21])) << 16)|
        (((U32)(pData[22])) <<  8)|
        (((U32)(pData[23]))      ));
    return result;
}
U64 EXTHD::GetSaveCountAF()
{
    U32 result = (
        (((U32)(pData[24])) << 24)|
        (((U32)(pData[25])) << 16)|
        (((U32)(pData[26])) <<  8)|
        (((U32)(pData[27]))      ));
    return result;
}
U32 EXTHD::GetSaveFlags()
{
    U32 result = (
        (((U32)(pData[28])) << 24)|
        (((U32)(pData[29])) << 16)|
        (((U32)(pData[30])) <<  8)|
        (((U32)(pData[31]))      ));
    return result;
}
//======================================================
//======================================================
void EXTHD::SetDateLastSaved(dtime t)
{
    pData[0] = ((((U64)t) >> 32) & 0xFF);
    pData[1] = ((((U64)t) >> 24) & 0xFF);
    pData[2] = ((((U64)t) >> 16) & 0xFF);
    pData[3] = ((((U64)t) >>  8) & 0xFF);
    pData[4] = ((((U64)t)      ) & 0xFF);
}
void EXTHD::SetDateFinalized(dtime t)
{
    pData[5] = ((((U64)t) >> 32) & 0xFF);
    pData[6] = ((((U64)t) >> 24) & 0xFF);
    pData[7] = ((((U64)t) >> 16) & 0xFF);
    pData[8] = ((((U64)t) >>  8) & 0xFF);
    pData[9] = ((((U64)t)      ) & 0xFF);
}
void EXTHD::SetDateBugFixed(dtime t)
{
    pData[10] = ((((U64)t) >> 32) & 0xFF);
    pData[11] = ((((U64)t) >> 24) & 0xFF);
    pData[12] = ((((U64)t) >> 16) & 0xFF);
    pData[13] = ((((U64)t) >>  8) & 0xFF);
    pData[14] = ((((U64)t)      ) & 0xFF);
}
void EXTHD::SetMajorVersion(U64 v)
{
    pData[15] = ((v >> 32) & 0xFF);
    pData[16] = ((v >> 24) & 0xFF);
    pData[17] = ((v >> 16) & 0xFF);
    pData[18] = ((v >>  8) & 0xFF);
    pData[19] = ((v      ) & 0xFF);
}
void EXTHD::SetSaveCountBF(U64 c)
{
    pData[20] = ((c >> 24) & 0xFF);
    pData[21] = ((c >> 16) & 0xFF);
    pData[22] = ((c >>  8) & 0xFF);
    pData[23] = ((c      ) & 0xFF);
}
void EXTHD::SetSaveCountAF(U64 c)
{
    pData[24] = ((c >> 24) & 0xFF);
    pData[25] = ((c >> 16) & 0xFF);
    pData[26] = ((c >>  8) & 0xFF);
    pData[27] = ((c      ) & 0xFF);
}
void EXTHD::SetSaveFlags(U32 f)
{
    pData[28] = ((f >> 24) & 0xFF);
    pData[29] = ((f >> 16) & 0xFF);
    pData[30] = ((f >>  8) & 0xFF);
    pData[31] = ((f      ) & 0xFF);
}
