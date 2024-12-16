#include "include.h"
/*===========================================================================*\
 *  DKC Level Builder Toolkit
 *  Copyright (C) 2025 Simion32
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
static U32 GetLucidUserID()
{
	TXT path = "SOFTWARE\\SubwareSoft\\DELTA\\LUCID Generator Data\\";
	if(!Registry.KeyExists(HKEY_CURRENT_USER, path)){
        Registry.CreateTree(HKEY_CURRENT_USER, path);
	}
	U32 r = Registry.GetHex(HKEY_CURRENT_USER, path, "UserID", 0xFFFFFFFF);
	if(r == 0xFFFFFFFF){
        r = (Utils.SuperRandom() & 0xFFFF);
        Registry.SetHex(HKEY_CURRENT_USER, path, "UserID", r);
	}
	return (r & 0xFFFF);
}
static U08 GetLucidCreateCount()
{
	TXT path = "SOFTWARE\\SubwareSoft\\DELTA\\LUCID Generator Data\\";
	if(!Registry.KeyExists(HKEY_CURRENT_USER, path)){
        Registry.CreateTree(HKEY_CURRENT_USER, path);
	}
	U32 r = Registry.GetHex(HKEY_CURRENT_USER, path, "CreationCount", 0xFFFFFFFF);
	r = ((r + 1) & 0xFF);
	Registry.SetHex(HKEY_CURRENT_USER, path, "CreationCount", r);
	return r;
}
LUCID& LUCID::Calculate()
{
	dtime dt;
	dt.SetToCurrentTime();
	U64 stamp = dt;
	U32 userid = GetLucidUserID();
	pData[0] = U08((stamp >> 32) & 0xFF);
	pData[1] = U08((stamp >> 24) & 0xFF);
	pData[2] = U08((stamp >> 16) & 0xFF);
	pData[3] = U08((stamp >>  8) & 0xFF);
	pData[4] = U08((stamp >>  0) & 0xFF);
	pData[5] = GetLucidCreateCount();
	pData[6] = U08((userid >> 8) & 0xFF);
	pData[7] = U08((userid >> 0) & 0xFF);
	*((B16*)(&pData[8])) = System.GetSystemIdentityLUCID();
	return *this;
}
U64 LUCID::GetTimeStamp()
{
    U64 result = 0LL;
    result |= (((U64)pData[0]) << 32);
    result |= (((U64)pData[1]) << 24);
    result |= (((U64)pData[2]) << 16);
    result |= (((U64)pData[3]) <<  8);
    result |= (((U64)pData[4]) <<  0);
    return result;
}
TXT LUCID::ToString()
{
    return Utils.ConvertToUrlBase64(&pData[0], 24);
}
TXT LUCID::ToHexString()
{
    TXT result;
    for(IND i = 0; i < 24; i++){result += String.FromHex(pData[i],2);} 
    return result;
}
LUCID::LUCID(TXT urlbase64)
{
    size_t found = 0;
    while((found = urlbase64.find_first_of(" \t\r\n",found)) != TXT::npos){
        urlbase64.erase(urlbase64.begin()+found);
    }
    Vx<U08> x = Utils.ConvertFromUrlBase64(urlbase64);
    for(IND i = 0; i < 24; i++){pData[i] = x[i];}
}
LUCID::LUCID(ByteCode data)
{
    TXT urlbase64 = data.t(0);
    Vx<U08> x = Utils.ConvertFromUrlBase64(urlbase64);
    for(IND i = 0; i < 24; i++){pData[i] = x[i];}
}


ByteCode::ByteCode(){(*this)(0x00,0x00);}//BUGFIX: Required Data Init
ByteCode::ByteCode(NUM a, NUM b){(*this)(a,b);}

ByteCode::operator TXT (){return t(0);}
ByteCode::operator NUM (){return i(0);}
ByteCode::operator FLO (){return f(0);}
ByteCode::operator Vx<NUM> (){return ii(0);}
ByteCode::operator Vx<FLO> (){return ff(0);}
ByteCode::operator Vx<DSXID> (){return dd(0);}
ByteCode::operator Vx<OBJID> (){return oo(0);}
ByteCode::operator DGEID (){return i(0);}
ByteCode::operator FILES (){return i(0);}
ByteCode::operator DLTID (){return i(0);}
ByteCode::operator MUSIC (){return i(0);}
ByteCode::operator SOUND (){return i(0);}
ByteCode::operator OBJID (){return i(0);}
ByteCode::operator LAYER (){return i(0);}
ByteCode::operator FTYPE (){return i(0);}
ByteCode::operator DSXID (){return i(0);}
ByteCode::operator TXTID (){return i(0);}
ByteCode::operator PTHID (){return i(0);}
ByteCode::operator LUCID (){return (TXT)t(0);}

//ByteCode Access Functions
NUM ByteCode::i(NUM index)        {return *((NUM*)       (((U32)this)+Table(index)));}
FLO ByteCode::f(NUM index)        {return *((FLO*)       (((U32)this)+Table(index)));}
const char* ByteCode::t(NUM index){return  ((const char*)(((U32)this)+Table(index)));}
Vx<NUM> ByteCode::ii(NUM index){
    NUM* nums0 = ((NUM*)(((U32)this)+Table(0)));
    NUM* nums1 = ((NUM*)(((U32)this)+Table(data_[4095])));
    return Vx<NUM>(nums0, nums1); //pointer array initialization **BUGFIXED**
}
Vx<FLO> ByteCode::ff(NUM index){
    FLO* flos0 = ((FLO*)(((U32)this)+Table(0)));
    FLO* flos1 = ((FLO*)(((U32)this)+Table(data_[4095])));
    return Vx<FLO>(flos0, flos1); //pointer array initialization **BUGFIXED**
}
Vx<DSXID> ByteCode::dd(NUM index){
    NUM* nums0 = ((NUM*)(((U32)this)+Table(0)));
    NUM* nums1 = ((NUM*)(((U32)this)+Table(data_[4095])));
    return Vx<DSXID>(nums0, nums1); //pointer array initialization
}
Vx<OBJID> ByteCode::oo(NUM index){
    NUM* nums0 = ((NUM*)(((U32)this)+Table(0)));
    NUM* nums1 = ((NUM*)(((U32)this)+Table(data_[4095])));
    return Vx<OBJID>(nums0, nums1); //pointer array initialization
}

//ByteCode Return Functions
ByteCode::ByteCode(TXT data)    {(*this)(0xFF,0xFF)(data);}
ByteCode::ByteCode(NUM data)    {(*this)(0xFF,0xFF)(data);}
ByteCode::ByteCode(FLO data)    {(*this)(0xFF,0xFF)(data);}
ByteCode::ByteCode(Vx<NUM> data){(*this)(0xFF,0xFF); for(IND i = 0; i < data.size(); i++){(*this)(data[i]);}}
ByteCode::ByteCode(Vx<FLO> data){(*this)(0xFF,0xFF); for(IND i = 0; i < data.size(); i++){(*this)(data[i]);}}
ByteCode::ByteCode(DGEID data)  {(*this)(0xFF,0xFF)((NUM)data.v);}
ByteCode::ByteCode(FILES data)  {(*this)(0xFF,0xFF)((NUM)data.v);}
ByteCode::ByteCode(DLTID data)  {(*this)(0xFF,0xFF)((NUM)data.v);}
ByteCode::ByteCode(MUSIC data)  {(*this)(0xFF,0xFF)((NUM)data.v);}
ByteCode::ByteCode(SOUND data)  {(*this)(0xFF,0xFF)((NUM)data.v);}
ByteCode::ByteCode(OBJID data)  {(*this)(0xFF,0xFF)((NUM)data.v);}
ByteCode::ByteCode(LAYER data)  {(*this)(0xFF,0xFF)((NUM)data.v);}
ByteCode::ByteCode(FTYPE data)  {(*this)(0xFF,0xFF)((NUM)data.v);}
ByteCode::ByteCode(DSXID data)  {(*this)(0xFF,0xFF)((NUM)data.v);}
ByteCode::ByteCode(TXTID data)  {(*this)(0xFF,0xFF)((NUM)data.v);}
ByteCode::ByteCode(PTHID data)  {(*this)(0xFF,0xFF)((NUM)data.v);}
ByteCode::ByteCode(LUCID data)  {(*this)(0xFF,0xFF)((TXT)data.ToString());}
ByteCode::ByteCode(Vx<OBJID> da){(*this)(0xFF,0xFF)(da);}

//ByteCode Builder Functions
ByteCode& ByteCode::operator()(){return *this;}
ByteCode& ByteCode::operator()(int data)    {Table(data_[4095]+1) = Set(Table(data_[4095]),(NUM)data); data_[4095]++; return *this;}
ByteCode& ByteCode::operator()(NUM a, NUM b){data_[0]=a; data_[1]=b; Table(0) = (16*sizeof(U32)); data_[4095]=0; return *this;}
ByteCode& ByteCode::operator()(TXT data)    {Table(data_[4095]+1) = Set(Table(data_[4095]),data); data_[4095]++; return *this;}
ByteCode& ByteCode::operator()(NUM data)    {Table(data_[4095]+1) = Set(Table(data_[4095]),data); data_[4095]++; return *this;}
ByteCode& ByteCode::operator()(FLO data)    {Table(data_[4095]+1) = Set(Table(data_[4095]),data); data_[4095]++; return *this;}
ByteCode& ByteCode::operator()(Vx<NUM> data){for(IND i = 0; i < data.size(); i++){(*this)(data[i]);} return *this;}
ByteCode& ByteCode::operator()(Vx<FLO> data){for(IND i = 0; i < data.size(); i++){(*this)(data[i]);} return *this;}
ByteCode& ByteCode::operator()(Vx<OBJID> da){Table(data_[4095]+1) = Set(Table(data_[4095]),da);   data_[4095]++; return *this;}

ByteCode& ByteCode::operator()(DGEID data)  {Table(data_[4095]+1) = Set(Table(data_[4095]),(NUM)data.v); data_[4095]++; return *this;}
ByteCode& ByteCode::operator()(FILES data)  {Table(data_[4095]+1) = Set(Table(data_[4095]),(NUM)data.v); data_[4095]++; return *this;}
ByteCode& ByteCode::operator()(DLTID data)  {Table(data_[4095]+1) = Set(Table(data_[4095]),(NUM)data.v); data_[4095]++; return *this;}
ByteCode& ByteCode::operator()(MUSIC data)  {Table(data_[4095]+1) = Set(Table(data_[4095]),(NUM)data.v); data_[4095]++; return *this;}
ByteCode& ByteCode::operator()(SOUND data)  {Table(data_[4095]+1) = Set(Table(data_[4095]),(NUM)data.v); data_[4095]++; return *this;}
ByteCode& ByteCode::operator()(OBJID data)  {Table(data_[4095]+1) = Set(Table(data_[4095]),(NUM)data.v); data_[4095]++; return *this;}
ByteCode& ByteCode::operator()(LAYER data)  {Table(data_[4095]+1) = Set(Table(data_[4095]),(NUM)data.v); data_[4095]++; return *this;}
ByteCode& ByteCode::operator()(FTYPE data)  {Table(data_[4095]+1) = Set(Table(data_[4095]),(NUM)data.v); data_[4095]++; return *this;}
ByteCode& ByteCode::operator()(DSXID data)  {Table(data_[4095]+1) = Set(Table(data_[4095]),(NUM)data.v); data_[4095]++; return *this;}
ByteCode& ByteCode::operator()(TXTID data)  {Table(data_[4095]+1) = Set(Table(data_[4095]),(NUM)data.v); data_[4095]++; return *this;}
ByteCode& ByteCode::operator()(PTHID data)  {Table(data_[4095]+1) = Set(Table(data_[4095]),(NUM)data.v); data_[4095]++; return *this;}
ByteCode& ByteCode::operator()(LUCID data)  {Table(data_[4095]+1) = Set(Table(data_[4095]),data.ToString()); data_[4095]++; return *this;}

//special builder function for the script runner
//copies returned ByteCode parameters into one ByteCode to be run
ByteCode& ByteCode::operator()(ByteCode& r)
{
    U32 r_numelements = r.data_[4095];
    for(IND i = 0; i < r_numelements; i++){
        U32 r_data_offs = r.Table(i);
        U32 r_data_size = (r.Table(i+1) - r_data_offs);
        memcpy(&data_[Table(data_[4095])], &(r.data_[r_data_offs]), r_data_size);
        Table(data_[4095]+1) = (Table(data_[4095]) + r_data_size); data_[4095]++;
    }
    return *this;
}

U32& ByteCode::Table(NUM index){return *((U32*)&data_[4+(index << 2)]);}

U32 ByteCode::Set(U32 where, TXT text){
    U32 i, j;
    for(i = where, j = 0; ((i < 4094) && (j < text.size())); i++, j++){
        data_[i] = text[j];
    }
    data_[i] = '\0';
    return (i + 1);
}
U32 ByteCode::Set(U32 where, NUM var){
    *((NUM*)(&data_[where])) = var;
    return (where + 8);
}
U32 ByteCode::Set(U32 where, FLO var){
    *((FLO*)(&data_[where])) = var;
    return (where + 8);
}
U32 ByteCode::Set(U32 where, Vx<NUM> var){
    for(U32 i = 0; i < var.size(); i++){
        *((NUM*)(&data_[i*8+where])) = var[i];
    }
    return (where+(var.size()*8));
}
U32 ByteCode::Set(U32 where, Vx<FLO> var){
    for(U32 i = 0; i < var.size(); i++){
        *((FLO*)(&data_[i*8+where])) = var[i];
    }
    return (where+(var.size()*8));
}
U32 ByteCode::Set(U32 where, Vx<OBJID> var){
    for(U32 i = 0; i < var.size(); i++){
        *((NUM*)(&data_[i*8+where])) = var[i];
    }
    return (where+(var.size()*8));
}
NUM ByteCode::GetA(){return data_[0];}
NUM ByteCode::GetB(){return data_[1];}
