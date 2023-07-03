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

Vx<T16> KString::w_persist_(111);
INT             KString::w_persist_index_ = 0;

BIT KString::IsHex(TXT str)
{
    INT i = 0;
    if((str.size() >= 2) && (str[0] == '0') && (str[1] == 'x')){
        i += 2;
    }else if((str.size() >= 1) && (str[0] == '$')){
        i++;
    }else{
        return false;
    }
    BIT is_hex = true;
    for(; i < str.size(); i++){
        CHR c = str[i];
        is_hex &= (((c >= '0') && (c <= '9'))
                || ((c >= 'A') && (c <= 'F'))
                || ((c >= 'a') && (c <= 'f')));
    }
    return is_hex;
}
BIT KString::IsInt(TXT str)
{
    INT i = 0;
    if((str.size() >= 1) && (str[0] == '-')) i++;
    if((str.size() >= 1) && (str[0] == '+')) i++;
    BIT is_INT = true;
    for(; i < str.size(); i++){
        CHR c = str[i];
        is_INT &= ((c >= '0') && (c <= '9'));
    }
    return is_INT;
}
BIT KString::IsFloat(TXT str)
{
    INT i = 0;
    if((str.size() >= 1) && (str[0] == '-')) i++;
    if((str.size() >= 1) && (str[0] == '+')) i++;
    BIT is_flo = true;
    INT dotcount = 0;
    for(; i < str.size(); i++){
        CHR c = str[i];
        BIT is_dot = (c == '.');
        is_flo &= (((c >= '0') && (c <= '9'))
                || (is_dot && (dotcount == 0)));
        if(is_dot) dotcount++;
    }
    return is_flo;
}
TXT KString::FromInt(S32 value, INT digits, BIT zeroes)
{
	TXT result;
    TXT s;
    S32 n = value;
    INT i = 0;
    if(n < 0) n = ((~n) + 1);
    do{
        s += ((CHR)((n % 10) + '0')); ++i;
    }while((n /= 10) > 0);
    if(zeroes){while(i < digits){s += '0'; ++i;}}
    if(value & 0x80000000){
        s += '-';
    }
    for(IND j = s.size() - 1; j >= 0; --j)
    {
        result += s[j];
    }
	return result;
}
TXT KString::FromHex(U64 value, INT digits)
{
	static TXT hex = "0123456789ABCDEF";
	TXT result = "";
	for(IND i = digits - 1; i >= 0; --i){
		result += hex[((value >> (i << 2)) & 0x0F)];
	}
	return result;
}
U64 KString::ToHex(TXT str)
{
	if(str.size() >= 1){
        if(str[0] == '$'){
            str = str.substr(1);
        }
	}
	if(str.size() >= 2){
        if(str.substr(0,2) == "0x"){
            str = str.substr(2);
        }
	}
	U64 result = 0x00000000;
	for(IND i = str.size() - 1; i >= 0; --i){
		INT c = (str[str.size()-1-i] & 0xFF);
		if((c >= '0') && (c <= '9')){
			c -= '0';
		}else if((c >= 'A') && (c <= 'F')){
			c = c - 'A' + 0x0A;
		}else if((c >= 'a') && (c <= 'f')){
			c = c - 'a' + 0x0A;
		}
		result |= (((U64)c) << (i << 2));
	}
	return result;
}
S64 KString::ToInt(TXT str)
{
	S64 result = 0x00000000;
	INT i = 0;
	BIT is_negative = (str[i] == '-');
	if((is_negative) || (str[i] == '+')) ++i;
	for(; i < str.size(); ++i){
		result *= 10; result += (str[i] - '0');
	}
	if(is_negative) result = ((~result) + 1);
	return result;
}
F64 KString::ToFloat(TXT str)
{
    F64 result = 0.0;
    F64 fraction = 1.0;
	INT i = 0;
	BIT is_negative = (str[i] == '-');
	if((is_negative) || (str[i] == '+')) ++i;
	BIT is_decpointed = false;
	for(; i < str.size(); ++i){
        if(str[i] == '.'){
            is_decpointed = true;
            continue;
        }
        if(is_decpointed) fraction *= 10.0;
		result *= 10.0; result += F64(str[i] - '0');
	}
	result /= fraction; //shift number back by (10^p).
	if(is_negative) result *= -1.0;
	return result;
}
Vx<TXT> KString::Split(const TXT &s, CHR delim)
{
    TXT delims = "";
    delims += delim;
    return Split(s, delims);
}
Vx<TXT> KString::Split(const TXT &s, TXT delims)
{
    Vx<TXT> elems;
    size_t lwhere = 0;
    size_t where = 0;
    while(true){
        lwhere = where;
        where = s.find_first_of(delims, where);
        if(where != TXT::npos){
            TXT item = s.substr(lwhere, (where - lwhere));
            if(item != "") elems.push_back(item); //only add the item to the list if it's not empty
            where++; //skip over the found character.
        }else{
            TXT item = s.substr(lwhere, TXT::npos);
            if(item != "") elems.push_back(item); //only add the item to the list if it's not empty
            break;
        }
    }
    return elems;
}
Vx<TXT> KString::SplitWithAll(const TXT &s, TXT delims)
{
    Vx<TXT> elems;
    INT current = 0;
    TXT buffer;
    BIT current_isdelims = false;
    for(IND i = 0; i < delims.size(); i++){
        current_isdelims |= (s[current] == delims[i]);
    }
    while(current < s.size()){
        BIT is_equal_delim = false;
        for(IND i = 0; i < delims.size(); i++){
            is_equal_delim |= (s[current] == delims[i]);
        }
        if(current_isdelims ^ is_equal_delim){
            elems.push_back(buffer);
            buffer = "";
            current_isdelims = is_equal_delim;
        }
        buffer += s[current];
        current++;
    }
    if(buffer != "") elems.push_back(buffer);
    return elems;
}
Vx<TXT> KString::SplitWithAll_WithEachDelimiter(const TXT &s, TXT delims)
{
    Vx<TXT> elems;
    INT current = 0;
    TXT buffer;
    BIT current_isdelims = false;
    for(IND i = 0; i < delims.size(); i++){
        current_isdelims |= (s[current] == delims[i]);
    }
    while(current < s.size()){
        BIT is_equal_delim = false;
        for(IND i = 0; i < delims.size(); i++){
            is_equal_delim |= (s[current] == delims[i]);
        }
        if(current_isdelims ^ is_equal_delim){
            elems.push_back(buffer);
            buffer = "";
            current_isdelims = is_equal_delim;
        }else if(current_isdelims){
            elems.push_back(buffer);
            buffer = "";
        }
        buffer += s[current];
        current++;
    }
    if(buffer != "") elems.push_back(buffer);
    return elems;
}
TXT KString::RemoveComments(const TXT &s)
{
    TXT buffer = "";
    BIT is_comment = false;
    for(IND i = 0; i < s.size(); i++){
        if(is_comment){
            if(s[i] == '\n'){
                is_comment = false;
                buffer += s[i];
            }else{
                //Do Nothing
            }
        }else{
            if(s[i] != '/'){
                buffer += s[i];
            }else{
                if(((i+1) < s.size()) && (s[i+1] == '/')){
                    is_comment = true;
                }else{
                    buffer += s[i];
                }
            }
        }
    }
    return buffer;
}
TXT KString::EscapeStringWhiteSpace(const TXT &s, CHR space, CHR tab)
{
    TXT buffer = "";
    BIT is_TXT = false;
    for(IND i = 0; i < s.size(); i++){
        if(is_TXT){
            //A newline means quit parsing the TXT (NO MULTILINE STRINGS).
            if((s[i] == '\"') || (s[i] == '\n')){
                is_TXT = false;
                if(s[i] == '\n') buffer += '\"';
                buffer += s[i];
            }else{
                if(s[i] == ' '){
                    buffer += space;
                }else if(s[i] == '\t'){
                    buffer += tab;
                }else{
                    buffer += s[i];
                }
            }
        }else{
            if(s[i] != '\"'){
                buffer += s[i];
            }else{
                is_TXT = true;
                buffer += s[i];
            }
        }
    }
    return buffer;
}
T16 KString::ToWide(STR str)
{
	if(str == NULL) return T16(L"");
	return ToWide((TXT)str);
}
T16 KString::ToWide(TXT str)
{
	size_t utf16_sz = 
	MultiByteToWideChar(CP_UTF8,0,
		str.c_str(),str.size(),
		NULL,0);
	C16 utf16str[utf16_sz+1];
	utf16str[utf16_sz] = (C16)0x0000;
	MultiByteToWideChar(CP_UTF8,0,
		str.c_str(), str.size(),
		utf16str, utf16_sz);
    T16 ws = utf16str;
	return ws;
}
const C16* KString::ToWideC(STR str)
{
    w_persist_index_ %= 111;
    w_persist_.at(w_persist_index_) = ToWide(str);
    return ((str == NULL) ? NULL : (w_persist_.at(w_persist_index_++).c_str()));
}
const C16* KString::ToWideC(TXT str)
{
    w_persist_index_ %= 111;
    w_persist_.at(w_persist_index_) = ToWide(str);
    return (w_persist_.at(w_persist_index_++).c_str());
}
TXT KString::FromWide(T16 wstr)
{
	size_t utf8_sz = 
	WideCharToMultiByte(CP_UTF8,0,
		wstr.c_str(), wstr.size(),
		NULL, 0,
		NULL,NULL);
	CHR utf8str[utf8_sz+1];
	utf8str[utf8_sz] = '\0';
	WideCharToMultiByte(CP_UTF8,0,
		wstr.c_str(), wstr.size(),
		utf8str, utf8_sz,
		NULL,NULL);
	TXT result = utf8str;
	for(IND i = result.size()-1; i >= 0; --i)
	{
		U08 theValue = ((U08)result[i]);
		if(!(theValue == 0x7F)
		&& !(theValue == 192)
		&& !(theValue == 193)
		&& !(theValue < 0x20)){
			continue;
		}
		result.erase(result.begin()+i);
	}
	return result;
}
TXT KString::ToUpper(TXT x)
{
	for(IND i = 0; i < x.size(); ++i) x[i] = toupper(x[i]);
	return x;
}
TXT KString::ToLower(TXT x)
{
	for(IND i = 0; i < x.size(); ++i) x[i] = tolower(x[i]);
	return x;
}
TXT KString::FileSizeXP(U64 size)
{
    TXT postfix = " Bytes";
    if(size >= (1024LL*1024LL*1024LL*1024LL)){
        size /= (1024LL*1024LL*1024LL*1024LL);
        postfix = " TB";
    }else if(size >= (1024LL*1024LL*1024LL)){
        size /= (1024LL*1024LL*1024LL);
        postfix = " GB";
    }else if(size >= (1024LL*1024LL)){
        size /= (1024LL*1024LL);
        postfix = " MB";
    }else if(size >= (1024LL)){
        size /= (1024LL);
        postfix = " KB";
    }else{
        postfix = " Bytes";
    }
    return String.FromInt(size, 10, false) + postfix;
}
