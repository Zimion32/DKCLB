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
#include "Sha256.h"
#include "Aes.h"

B32 SHA256(Vx<U08>& data)
{
	return SHA256(&data[0], data.size());
}
B32 SHA256(Vx<U08>& data, U64 salt)
{
    CSha256 shaobject;
	B32 hash;
	Sha256_Init(&shaobject);
	Sha256_Update(&shaobject, reinterpret_cast<const Byte*>(&data[0]), (size_t)data.size());
	Sha256_Update(&shaobject, reinterpret_cast<const Byte*>(&salt), (size_t)8);
	Sha256_Final(&shaobject, reinterpret_cast<Byte*>(&hash));
	return hash;
}
B32 SHA256(U08* data, U32 size)
{
	CSha256 shaobject;
	B32 hash;
	Sha256_Init(&shaobject);
	Sha256_Update(&shaobject, reinterpret_cast<const Byte*>(data), (size_t)size);
	Sha256_Final(&shaobject, reinterpret_cast<Byte*>(&hash));
	return hash;
}



static bool pNeedsToInitAES = true;
void AES_Encrypt(Vx<U08>& data, B32* key, B16* iv)
{
    AES_Encrypt(&data[0], data.size(), key, iv);
}
void AES_Encrypt(U08* data, U32 size, B32* key, B16* iv)
{
	if(pNeedsToInitAES)
	{
		AesGenTables();
		pNeedsToInitAES = false;
	}
	if(size < 16) return;
	U32 blocks = (size / 16);
	U32 recryptoffset = (size - (blocks * 16));
	CAesCbc* aesobject = new CAesCbc;
	Aes_SetKeyEncode(&aesobject->aes, reinterpret_cast<const Byte*>(key), 32);
	AesCbc_Init(aesobject, reinterpret_cast<const Byte*>(iv));
	AesCbc_Encode(aesobject, (Byte*)data, blocks * 16);
	if(recryptoffset != 0)
	{
		AesCbc_Init(aesobject, reinterpret_cast<const Byte*>(iv));
		AesCbc_Encode(aesobject, (Byte*)&data[recryptoffset], blocks * 16);
	}
	delete aesobject;
	
}
void AES_Decrypt(Vx<U08>& data, B32* key, B16* iv)
{
    AES_Decrypt(&data[0], data.size(), key, iv);
}
void AES_Decrypt(U08* data, U32 size, B32* key, B16* iv)
{
	if(pNeedsToInitAES)
	{
		AesGenTables();
		pNeedsToInitAES = false;
	}
	if(size < 16) return;
	U32 blocks = (size / 16);
	U32 recryptoffset = (size - (blocks * 16));
	CAesCbc* aesobject = new CAesCbc;
	Aes_SetKeyDecode(&aesobject->aes, reinterpret_cast<const Byte*>(key), 32);
	if(recryptoffset != 0)
	{
		AesCbc_Init(aesobject, reinterpret_cast<const Byte*>(iv));
		AesCbc_Decode(aesobject, (Byte*)&data[recryptoffset], blocks * 16);
	}
	AesCbc_Init(aesobject, reinterpret_cast<const Byte*>(iv));
	AesCbc_Decode(aesobject, (Byte*)data, blocks * 16);
	delete aesobject;
}

