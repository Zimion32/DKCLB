#include "include.h"
void veccpy(Vx<F32>& dst, U32 dstoff, Vx<F32>& src, U32 srcoff, U32 bytesize)
{
    if(dstoff >= dst.size())                                    {Debug.Log("out of range copy!! A"); throw exception("out of range copy!!");}
    if((dstoff*sizeof(F32)+bytesize) > (dst.size()*sizeof(F32))){Debug.Log("out of range copy!! B"); throw exception("out of range copy!!");}
    if(srcoff >= src.size())                                    {Debug.Log("out of range copy!! C"); throw exception("out of range copy!!");}
    if((srcoff*sizeof(F32)+bytesize) > (src.size()*sizeof(F32))){Debug.Log("out of range copy!! D"); throw exception("out of range copy!!");}
    memcpy(&dst[dstoff], &src[srcoff], bytesize);
}
void veccpy(Vx<U08>& dst, U32 dstoff, Vx<U08>& src, U32 srcoff, U32 bytesize)
{
    if(dstoff >= dst.size())          {Debug.Log("out of range copy!! A"); throw exception("out of range copy!!");}
    if((dstoff+bytesize) > dst.size()){Debug.Log("out of range copy!! B"); throw exception("out of range copy!!");}
    if(srcoff >= src.size())          {Debug.Log("out of range copy!! C"); throw exception("out of range copy!!");}
    if((srcoff+bytesize) > src.size()){Debug.Log("out of range copy!! D"); throw exception("out of range copy!!");}
    memcpy(&dst[dstoff], &src[srcoff], bytesize);
}
void veccpy_str2u64s(Vx<U64>& dst, TXT& src, U32 bytesize)
{
    if(bytesize > (dst.size()*8)){throw exception("out of range copy 2!!");}
    if(bytesize > src.size())    {throw exception("out of range copy 2!!");}
    memcpy(((void*)&dst[0]), &src[0], bytesize);
}
