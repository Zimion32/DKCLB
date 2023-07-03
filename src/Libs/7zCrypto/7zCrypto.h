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
B32 SHA256(Vx<U08>& data);
B32 SHA256(Vx<U08>& data, U64 salt);
B32 SHA256(U08* data, U32 size);
void AES_Encrypt(    Vx<U08>& data, B32* key, B16* iv);
void AES_Encrypt(U08* data, U32 size, B32* key, B16* iv);
void AES_Decrypt(    Vx<U08>& data, B32* key, B16* iv);
void AES_Decrypt(U08* data, U32 size, B32* key, B16* iv);
