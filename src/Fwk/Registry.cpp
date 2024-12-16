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
#include <TChar.H>
#include "include.h"
#define chk(x,z) {U32 _r; if((_r = (x)) != ERROR_SUCCESS){SetLastError(_r); RegCloseKey(hk); z}}
#define MAX_KEY_LENGTH 255
#define MAX_VALUE_NAME 16383
static const C16* esn(const C16* x){
	return ((x[0] == L'\0')?(NULL):(x));
}
void KRegistry::GetKeyList(HKEY root, TXT path, Vx<TXT>& key_list) 
{ 
	key_list.clear();
	HKEY hk;
	chk(RegOpenKeyEx(root, esn(String.ToWideC(path)), 0, KEY_READ, &hk),								return; );
	
    C16  achKey[MAX_KEY_LENGTH];   // buffer for subkey name
    DWORD    cbName;                   // size of name TXT 
    C16  achClass[MAX_PATH];  // buffer for class name 
    DWORD    cchClassName = MAX_PATH;  // size of class TXT 
    DWORD    cSubKeys=0;               // number of subkeys 
    DWORD    cbMaxSubKey;              // longest subkey size 
    DWORD    cchMaxClass;              // longest class TXT 
    DWORD    cValues;              // number of values for key 
    DWORD    cchMaxValue;          // longest value name 
    DWORD    cbMaxValueData;       // longest value data 
    DWORD    cbSecurityDescriptor; // size of security descriptor 
    FILETIME ftLastWriteTime;      // last write time 
 
    DWORD retCode; 
 
    //C16  achValue[MAX_VALUE_NAME]; 
    //DWORD cchValue = MAX_VALUE_NAME; 
 
    // Get the class name and the value count. 
    retCode = RegQueryInfoKey(
        hk,                    // key handle 
        achClass,                // buffer for class name 
        &cchClassName,           // size of class TXT 
        NULL,                    // reserved 
        &cSubKeys,               // number of subkeys 
        &cbMaxSubKey,            // longest subkey size 
        &cchMaxClass,            // longest class TXT 
        &cValues,                // number of values for this key 
        &cchMaxValue,            // longest value name 
        &cbMaxValueData,         // longest value data 
        &cbSecurityDescriptor,   // security descriptor 
        &ftLastWriteTime);       // last write time 
 
    // Enumerate the subkeys, until RegEnumKeyEx fails.
    if(cSubKeys)
    {
		for(IND i = 0; i < cSubKeys; i++) 
        { 
            cbName = MAX_KEY_LENGTH;
            retCode = RegEnumKeyEx(hk, i, achKey, &cbName, NULL, NULL, NULL, &ftLastWriteTime); 
            if(retCode == ERROR_SUCCESS) 
            {
                key_list.push_back(String.FromWide(achKey));
            }
        }
    }
    RegCloseKey(hk);
}
void KRegistry::GetKeyListSW(HKEY root, TXT path, Vx<TXT>& key_list, CHR starting_chr)
{
	key_list.clear();
	HKEY hk;
	chk(RegOpenKeyEx(root, esn(String.ToWideC(path)), 0, KEY_READ, &hk),								return; );
	
    C16  achKey[MAX_KEY_LENGTH];   // buffer for subkey name
    DWORD    cbName;                   // size of name TXT 
    C16  achClass[MAX_PATH];  // buffer for class name 
    DWORD    cchClassName = MAX_PATH;  // size of class TXT 
    DWORD    cSubKeys=0;               // number of subkeys 
    DWORD    cbMaxSubKey;              // longest subkey size 
    DWORD    cchMaxClass;              // longest class TXT 
    DWORD    cValues;              // number of values for key 
    DWORD    cchMaxValue;          // longest value name 
    DWORD    cbMaxValueData;       // longest value data 
    DWORD    cbSecurityDescriptor; // size of security descriptor 
    FILETIME ftLastWriteTime;      // last write time 
 
    DWORD retCode; 
 
    //C16  achValue[MAX_VALUE_NAME]; 
    //DWORD cchValue = MAX_VALUE_NAME; 
 
    // Get the class name and the value count. 
    retCode = RegQueryInfoKey(
        hk,                    // key handle 
        achClass,                // buffer for class name 
        &cchClassName,           // size of class TXT 
        NULL,                    // reserved 
        &cSubKeys,               // number of subkeys 
        &cbMaxSubKey,            // longest subkey size 
        &cchMaxClass,            // longest class TXT 
        &cValues,                // number of values for this key 
        &cchMaxValue,            // longest value name 
        &cbMaxValueData,         // longest value data 
        &cbSecurityDescriptor,   // security descriptor 
        &ftLastWriteTime);       // last write time 
 
    // Enumerate the subkeys, until RegEnumKeyEx fails.
    if(cSubKeys)
    {
		for(IND i = 0; i < cSubKeys; i++) 
        { 
            cbName = MAX_KEY_LENGTH;
            retCode = RegEnumKeyEx(hk, i, achKey, &cbName, NULL, NULL, NULL, &ftLastWriteTime); 
            if((retCode == ERROR_SUCCESS) && (achKey[0] == starting_chr))
            {
                key_list.push_back(String.FromWide(achKey));
            }
        }
    }
    RegCloseKey(hk);
}
void KRegistry::GetList(HKEY root, TXT path, Vx<TXT>& list) 
{ 
	list.clear();
	HKEY hk;
	chk(RegOpenKeyEx(root, esn(String.ToWideC(path)), 0, KEY_READ, &hk),								return; );
	
    //C16  achKey[MAX_KEY_LENGTH];   // buffer for subkey name
    //DWORD    cbName;                   // size of name TXT 
    C16  achClass[MAX_PATH] = TEXT("");  // buffer for class name 
    DWORD    cchClassName = MAX_PATH;  // size of class TXT 
    DWORD    cSubKeys=0;               // number of subkeys 
    DWORD    cbMaxSubKey;              // longest subkey size 
    DWORD    cchMaxClass;              // longest class TXT 
    DWORD    cValues;              // number of values for key 
    DWORD    cchMaxValue;          // longest value name 
    DWORD    cbMaxValueData;       // longest value data 
    DWORD    cbSecurityDescriptor; // size of security descriptor 
    FILETIME ftLastWriteTime;      // last write time 
 
    DWORD retCode; 
 
    C16  achValue[MAX_VALUE_NAME]; 
    DWORD cchValue = MAX_VALUE_NAME; 
 
    // Get the class name and the value count. 
    retCode = RegQueryInfoKey(
        hk,                    // key handle 
        achClass,                // buffer for class name 
        &cchClassName,           // size of class TXT 
        NULL,                    // reserved 
        &cSubKeys,               // number of subkeys 
        &cbMaxSubKey,            // longest subkey size 
        &cchMaxClass,            // longest class TXT 
        &cValues,                // number of values for this key 
        &cchMaxValue,            // longest value name 
        &cbMaxValueData,         // longest value data 
        &cbSecurityDescriptor,   // security descriptor 
        &ftLastWriteTime);       // last write time 
 
    // Enumerate the key values. 
    if(cValues) 
    {
        list.resize(cValues,"");
        for(IND i = 0, retCode = ERROR_SUCCESS; i < cValues; i++) 
        { 
            cchValue = MAX_VALUE_NAME; 
            achValue[0] = '\0'; 
            retCode = RegEnumValue(hk, i, achValue, &cchValue, NULL, NULL, NULL, NULL);
            if(retCode == ERROR_SUCCESS) 
            { 
				list[i] = String.FromWide(achValue);
            } 
        }
    }
    RegCloseKey(hk);
}
INT KRegistry::CreateTree(HKEY hk, TXT path)
{
    CHR sDir[MAX_PATH];
	INT iNameSz = (INT)path.size(), iCount = 0, iPos = 0;
	for(iPos = 0; iPos < iNameSz; iPos++)
    {
        if(path[iPos] == '\\' || path[iPos] == '/')
        {
            sDir[iPos] = '\0';
            if(Create(hk, sDir))
            {
                iCount++;
            }
        }
        sDir[iPos] = path[iPos];
    }
    sDir[iPos] = '\0';
    if(Create(hk, sDir))
    {
        iCount++;
    }
    return iCount;
}
BIT KRegistry::Create(HKEY root, TXT path)
{
    HKEY hk; DWORD dwFunc;
    //NOTE: you can't create keys directly under HKEY_LOCAL_MACHINE in Windows NT or 2000
    //------------------------------------------------------------------------------
    SECURITY_DESCRIPTOR SD;
    SECURITY_ATTRIBUTES SA;
    if(!InitializeSecurityDescriptor(&SD, SECURITY_DESCRIPTOR_REVISION)) return true;
    if(!SetSecurityDescriptorDacl(&SD, true, 0, false)) return true;
    SA.nLength				= sizeof(SA);
    SA.lpSecurityDescriptor	= &SD;
    SA.bInheritHandle		= false;
    //------------------------------------------------------------------------------
    chk(RegCreateKeyEx(root, esn(String.ToWideC(path)), 0, NULL, REG_OPTION_NON_VOLATILE, KEY_WRITE, &SA, &hk, &dwFunc), return true; );
    return false;
}
BIT KRegistry::KeyExists(HKEY root, TXT path)
{
	HKEY hk = NULL;
	LONG result = RegOpenKeyEx(root, esn(String.ToWideC(path)), 0, KEY_READ, &hk);
	RegCloseKey(hk);
	return (result == ERROR_SUCCESS);
}
//##############################################################################
BIT KRegistry::SetHex(HKEY root, TXT path, TXT variable, U32 value)
{
    HKEY hk;
    chk(RegOpenKeyEx(root, esn(String.ToWideC(path)), 0, KEY_WRITE, &hk), 							return true; );
    chk(RegSetValueEx(hk, esn(String.ToWideC(variable)), 0, REG_DWORD,(U08*)&value,sizeof(value)), return true; );
	RegCloseKey(hk);
    return false;
}
BIT KRegistry::SetU64(HKEY root, TXT path, TXT variable, U64 value)
{
    HKEY hk;
    chk(RegOpenKeyEx(root, esn(String.ToWideC(path)), 0, KEY_WRITE, &hk),							return true; );
	chk(RegSetValueEx(hk, esn(String.ToWideC(variable)), 0, REG_QWORD,(LPBYTE)&value,sizeof(value)), return true; );
	RegCloseKey(hk);
	return false;
}
BIT KRegistry::SetStr(HKEY root, TXT path, TXT variable, TXT value)
{
    HKEY hk; 
    T16 wvalue = String.ToWide(value);
    U32 sz = ((wvalue.size() + 1) * 2);//WARNING: reg TXT must be null terminated
    chk(RegOpenKeyEx(root, esn(String.ToWideC(path)), 0, KEY_WRITE, &hk),							return true; );
    chk(RegSetValueEx(hk, esn(String.ToWideC(variable)), 0, REG_SZ, (U08*)(wvalue.c_str()), sz),	return true; );
    RegCloseKey(hk);
	return false;
}
BIT KRegistry::SetBool(HKEY root, TXT path, TXT variable, BIT value)
{
    U32 actual = (value?1:0);
    return SetHex(root, path, variable, actual);
}
BIT KRegistry::SetData(HKEY root, TXT path, TXT variable, U08* data, U32 bytes)
{
    HKEY hk;
    chk(RegOpenKeyEx(root, esn(String.ToWideC(path)), 0, KEY_WRITE, &hk),							return true; );
	chk(RegSetValueEx(hk, esn(String.ToWideC(variable)), 0, REG_BINARY, (U08*)data, bytes),		return true; );
	RegCloseKey(hk);
    return false;
}
BIT KRegistry::SetData(HKEY root, TXT path, TXT variable, Vx<U08>& data)
{
	return SetData(root, path, variable, (U08*)(&data[0]), data.size());
}
//##############################################################################
U32 KRegistry::GetHex(HKEY root, TXT path, TXT variable, U32 def)
{
    U32 ulBuff = def;
    HKEY hk; DWORD dwType = REG_BINARY, sz = sizeof(ulBuff);
    chk(RegOpenKeyEx(root, esn(String.ToWideC(path)), 0, KEY_READ, &hk),							return def; );
    chk(RegQueryValueEx(hk, esn(String.ToWideC(variable)), 0, &dwType, (LPBYTE)&ulBuff, &sz),		return def; );
    RegCloseKey(hk);
    return ulBuff;
}
U64 KRegistry::GetU64(HKEY root, TXT path, TXT variable, U64 def)
{
    U64 u64Buff = 0;
    HKEY hk; DWORD dwType = REG_BINARY, sz = sizeof(u64Buff);
    chk(RegOpenKeyEx(root, esn(String.ToWideC(path)), 0, KEY_READ, &hk),							return def; );
    chk(RegQueryValueEx(hk, esn(String.ToWideC(variable)), 0, &dwType, (LPBYTE)&u64Buff, &sz),		return def; );
    RegCloseKey(hk);
    return u64Buff;
}
TXT KRegistry::GetStr(HKEY root, TXT path, TXT variable, TXT def)
{
    HKEY hk; DWORD dwType = KEY_ALL_ACCESS, sz = 0;
    chk(RegOpenKeyEx(root, esn(String.ToWideC(path)), 0, KEY_READ, &hk), 							return def; );
    chk(RegQueryValueEx(hk, esn(String.ToWideC(variable)), NULL, &dwType, NULL, &sz),				return def; );
	C16* buffer = new C16[sz+1]; sz *= 2; sz += 2;
	ZeroMemory((PTR)buffer, sz);
	chk(RegQueryValueEx(hk, esn(String.ToWideC(variable)), NULL, &dwType, (U08*)buffer, &sz),		delete[] buffer; return def; );
    RegCloseKey(hk);
    TXT result = String.FromWide(buffer);
	delete[] buffer;
	return result;
}
BIT KRegistry::GetBool(HKEY root, TXT path, TXT variable, BIT def)
{
    U32 value = GetHex(root, path, variable, 0x80000000);
    if(value != 0x80000000) return (value != 0);
    return def;
}
Vx<U08> KRegistry::GetData(HKEY root, TXT path, TXT variable)
{
	Vx<U08> buffer; buffer.clear();
    HKEY hk; DWORD dwType = REG_BINARY, sz = 0;
	chk(RegOpenKeyEx(root, esn(String.ToWideC(path)), 0, KEY_READ, &hk),								return buffer; );
	chk(RegQueryValueEx(hk, esn(String.ToWideC(variable)), 0, &dwType, NULL, &sz),					return buffer; );
    buffer.resize(sz);
    chk(RegQueryValueEx(hk, esn(String.ToWideC(variable)), 0, &dwType, (LPBYTE)(&buffer[0]), &sz),	buffer.clear(); return buffer; );
	RegCloseKey(hk);
    return buffer;
}

