#pragma once
template <class T>
class RadixSort
{
    private:
        U32 radix_count_;
        Vx<T*> buff_;
        Vx<T* > sxbuff_;
        
    public:
        void Init(U32 reserve_size)
        {//required seperately so that memory thrashing doesn't happen.
            if(reserve_size <= 0) reserve_size = 1024*1024;
            buff_.resize(reserve_size);
            radix_count_ = T::RadixCount();
            if(radix_count_ & 1) abort(); ///WARNING: COUNT MUST BE MULTIPLE OF 2.
        }
        void Sort(Vx<T*>& input, U32 option)
        {
            Vx<T*>* arrays_[2] = {&buff_, &input};
            if(input.size() > buff_.size()){
                buff_.resize(input.size());
            }
            U32 counters_[0x100];
            U32 offsets_[0x100];
            for(U32 k = 0; k < radix_count_; k++){
                for(U32 i = 0; i < 0x100; i++){
                    counters_[i] = 0;
                    offsets_[i] = 0;
                }
                for(U32 i = 0; i < input.size(); i++){
                    counters_[(*(arrays_[(k^1)&1]))[i]->Radix(option, k) & 0xFF]++;
                }
                for(U32 i = 1; i < 0x100; i++){
                    offsets_[i] = offsets_[i-1] + counters_[i-1];
                }
                for(U32 i = 0; i < input.size(); i++){
                    T* ptr = (*(arrays_[(k^1)&1]))[i];
                    (*(arrays_[k&1]))[offsets_[ptr->Radix(option, k) & 0xFF]++] = ptr;
                }
            }
        }
        void Sort(Vx<T* >& input, U32 option)
        {
            Vx<T* >* arrays_[2] = {&sxbuff_, &input};
            if(input.size() > sxbuff_.size()){
                sxbuff_.resize(input.size());
            }
            U32 counters_[0x100];
            U32 offsets_[0x100];
            for(U32 k = 0; k < radix_count_; k++){
                for(U32 i = 0; i < 0x100; i++){
                    counters_[i] = 0;
                    offsets_[i] = 0;
                }
                for(U32 i = 0; i < input.size(); i++){
                    counters_[(*(arrays_[(k^1)&1]))[i]->Radix(option, k) & 0xFF]++;
                }
                for(U32 i = 1; i < 0x100; i++){
                    offsets_[i] = offsets_[i-1] + counters_[i-1];
                }
                for(U32 i = 0; i < input.size(); i++){
                    T* ptr = (*(arrays_[(k^1)&1]))[i];
                    (*(arrays_[k&1]))[offsets_[ptr->Radix(option, k) & 0xFF]++] = ptr;
                }
            }
        }
};
static U64 RadixFloat64(F64 value)
{
    U64 data0 = *((U64*)&value);
    U64 mask = (((data0 >> 63) & 1LL) * 0x7FFFFFFFFFFFFFFFLL);
    return (data0 ^ 0x8000000000000000LL ^ mask);
}
static U32 RadixFloat32(F32 value)
{
    U32 data0 = *((U32*)&value);
    U32 mask = (((data0 >> 31) & 1) * 0x7FFFFFFF);
    return (data0 ^ 0x80000000 ^ mask);
}
