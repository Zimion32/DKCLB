#pragma once
unsigned int MurmurHash2A(const void * key, int len, unsigned int seed);
unsigned int MurmurHash2A_8bit(const void * key, int len, unsigned int seed);
unsigned int MurmurHash2A_16bit(const void * key, int len, unsigned int seed);
unsigned int MurmurHash2A_24bit(const void * key, int len, unsigned int seed);
class CMurmurHash2A
{
    public:
        void Begin(unsigned int seed = 0);
        void Add(const unsigned char * data, int len);
        unsigned int End();
        
    private:
        static const unsigned int m;
        static const int r;
        void MixTail (const unsigned char * & data, int & len);
        unsigned int m_hash;
        unsigned int m_tail;
        unsigned int m_count;
        unsigned int m_size;
};
