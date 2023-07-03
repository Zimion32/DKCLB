//-----------------------------------------------------------------------------
// MurmurHash2A, by Austin Appleby

// This is a variant of MurmurHash2 modified to use the Merkle-Damgard
// construction. Bulk speed should be identical to Murmur2, small-key speed
// will be 10%-20% slower due to the added overhead at the end of the hash.

// This variant fixes a minor issue where null keys were more likely to
// collide with each other than expected, and also makes the algorithm
// more amenable to incremental implementations. All other caveats from
// MurmurHash2 still apply.

#include "MurmurHash2A.h"

const unsigned int CMurmurHash2A::m = 0x5bd1e995;
const int          CMurmurHash2A::r = 24;

#define mmix(h,k) { k *= m; k ^= k >> r; k *= m; h *= m; h ^= k; }

unsigned int MurmurHash2A_8bit(const void * key, int len, unsigned int seed)
{
    unsigned int x = MurmurHash2A(key, len, seed);
    x = (((x >> 24) ^ (x >> 16) ^ (x >> 8) ^ x) & 0xFF);
    return x;
}
unsigned int MurmurHash2A_16bit(const void * key, int len, unsigned int seed)
{
    unsigned int x = MurmurHash2A(key, len, seed);
    x = (((x >> 16) ^ x) & 0xFFFF);
    return x;
}
unsigned int MurmurHash2A_24bit(const void * key, int len, unsigned int seed)
{
    unsigned int x = MurmurHash2A(key, len, seed);
    x = (x & 0xFFFFFF);
    return x;
}
unsigned int MurmurHash2A(const void * key, int len, unsigned int seed)
{
	const unsigned int m = 0x5bd1e995;
	const int r = 24;
	unsigned int l = len;

	const unsigned char * data = (const unsigned char *)key;

	unsigned int h = seed;

	while(len >= 4)
	{
		unsigned int k = *(unsigned int*)data;

		mmix(h,k);

		data += 4;
		len -= 4;
	}

	unsigned int t = 0;

	switch(len)
	{
	case 3: t ^= data[2] << 16;
	case 2: t ^= data[1] << 8;
	case 1: t ^= data[0];
	};

	mmix(h,t);
	mmix(h,l);

	h ^= h >> 13;
	h *= m;
	h ^= h >> 15;

	return h;
}

//-----------------------------------------------------------------------------
// CMurmurHash2A, by Austin Appleby

// This is a sample implementation of MurmurHash2A designed to work
// incrementally.

// Usage -

// CMurmurHash2A hasher
// hasher.Begin(seed);
// hasher.Add(data1,size1);
// hasher.Add(data2,size2);
// ...
// hasher.Add(dataN,sizeN);
// unsigned int hash = hasher.End()

void CMurmurHash2A::Begin(unsigned int seed)
{
    m_hash  = seed;
    m_tail  = 0;
    m_count = 0;
    m_size  = 0;
}
void CMurmurHash2A::Add(const unsigned char * data, int len)
{
    m_size += len;
    
    MixTail(data,len);
    
    while(len >= 4)
    {
        unsigned int k = *(unsigned int*)data;

        mmix(m_hash,k);

        data += 4;
        len -= 4;
    }
    
    MixTail(data,len);
}
unsigned int CMurmurHash2A::End()
{
    mmix(m_hash,m_tail);
    mmix(m_hash,m_size);
    
    m_hash ^= m_hash >> 13;
    m_hash *= m;
    m_hash ^= m_hash >> 15;
    
    return m_hash;
}
void CMurmurHash2A::MixTail(const unsigned char * & data, int & len)
{
    while(len && ((len<4) || m_count))
    {
        m_tail |= (*data++) << (m_count * 8);
        
        m_count++;
        len--;
        
        if(m_count == 4){
            mmix(m_hash,m_tail);
            m_tail = 0;
            m_count = 0;
        }
    }
}
