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
struct __attribute__((packed)) EXTHD {//32 bytes
    U08 pData[32];//[5][5][5][5][4][4][4]
    
    EXTHD(){for(IND d = 0; d < 32; ++d){pData[d] = 0x00;}}
    
    dtime GetDateLastSaved();
    dtime GetDateFinalized();
    dtime GetDateBugFixed();
    U64 GetMajorVersion();//major version (starts 0 during initial dev)
    U64 GetSaveCountBF();//before finalize date
    U64 GetSaveCountAF();//after finalize date
    U32 GetSaveFlags();
    
    void SetDateLastSaved(dtime x);
    void SetDateFinalized(dtime x);
    void SetDateBugFixed(dtime x);
    void SetMajorVersion(U64 x);
    void SetSaveCountBF(U64 x);
    void SetSaveCountAF(U64 x);
    void SetSaveFlags(U32 x);
};

