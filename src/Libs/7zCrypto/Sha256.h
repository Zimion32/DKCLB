/* Crypto/Sha256.h -- SHA-256 Hash function
2008-10-04 : Igor Pavlov : Public domain */

#ifndef __CRYPTO_SHA256_H
#define __CRYPTO_SHA256_H

#include "Types7z.h"

#define SHA256_DIGEST_SIZE 32

typedef struct
{
  UInt32 state[8];
  UInt64 count;
  Byte buffer[64];
} CSha256;

void Sha256_Init(CSha256 *p);
void Sha256_Update(CSha256 *p, const Byte *data, size_t size);
void Sha256_Final(CSha256 *p, Byte *digest);

#endif

