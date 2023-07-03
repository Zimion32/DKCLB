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
#define NO_WIN32_LEAN_AND_MEAN
#include "include.h"
#undef TXT
#include <winsock2.h>
#include <iphlpapi.h>
#define TXT checked_string<std::string>

U32       KSystem::close_was_requested_ = 0;
HCURSOR     KSystem::cursors_[16];
INT         KSystem::cursor_reset_delay_ = 2;

HWND        KSystem::MainHWND = NULL;
HINSTANCE   KSystem::MainHINST = NULL;
HANDLE      KSystem::MainHTHREAD = NULL;
DWORD       KSystem::MainTHREADID = 0;
HANDLE      KSystem::MainHFIBER = NULL;
INT         KSystem::MainCursorIndex = 0;

void KSystem::RequestClose()
{
	close_was_requested_ = true;
}
BIT KSystem::HandleCloseRequest()
{
	BIT result = close_was_requested_;
	close_was_requested_ = false;
	return result;
}
void KSystem::PostCloseMessage()
{
	PostMessage(System.MainHWND, WM_CLOSE, 0, 0);
}
U64 KSystem::GetPerformanceFrequency()
{
	LARGE_INTEGER k;
	QueryPerformanceFrequency(&k);
	return k.QuadPart;
}
U64 KSystem::GetPerformanceCounter()
{
	LARGE_INTEGER k;
	QueryPerformanceCounter(&k);
	return k.QuadPart;
}
U32 KSystem::GetStackAvailable()
{
    MEMORY_BASIC_INFORMATION stackInfo;
	PTR currentAddr = (PTR)(&stackInfo);
	VirtualQuery(currentAddr, &stackInfo, sizeof(MEMORY_BASIC_INFORMATION));
	U32 stackBytesLeft = (U32)currentAddr - (U32)stackInfo.AllocationBase;
	return stackBytesLeft;
}
U64 KSystem::GetMACAddress()
{
    U64 result = 0x0000000000000000LL;
    Vx<IP_ADAPTER_INFO> AdapterInfo(32);
    DWORD dwBufLen = sizeof(AdapterInfo);
    DWORD dwStatus = GetAdaptersInfo(&AdapterInfo[0], &dwBufLen);

    if(dwStatus != ERROR_SUCCESS) return 0x800000000000LL;

    PIP_ADAPTER_INFO pAdapterInfo = &AdapterInfo[0];
    while(pAdapterInfo)
    {
        if(pAdapterInfo->Type == MIB_IF_TYPE_ETHERNET)
        {
			result |= (U64(pAdapterInfo->Address[0]) << (5*8));
			result |= (U64(pAdapterInfo->Address[1]) << (4*8));
			result |= (U64(pAdapterInfo->Address[2]) << (3*8));
			result |= (U64(pAdapterInfo->Address[3]) << (2*8));
			result |= (U64(pAdapterInfo->Address[4]) << (1*8));
			result |= (U64(pAdapterInfo->Address[5]) << (0*8));
			return result;
        }
        pAdapterInfo = pAdapterInfo->Next;
    }
    return 0x800000000000LL;
}
Vx<U64> KSystem::GetMACAddressList()
{
    Vx<U64> m;
    m.clear();
    Vx<IP_ADAPTER_INFO> AdapterInfo(32);
    DWORD dwBufLen = sizeof(AdapterInfo);
    DWORD dwStatus = GetAdaptersInfo(&AdapterInfo[0], &dwBufLen);

    if(dwStatus != ERROR_SUCCESS) return m;

    PIP_ADAPTER_INFO pAdapterInfo = &AdapterInfo[0];
    TXT szBuffer(1024,'\0');
    while(pAdapterInfo)
    {
        if(pAdapterInfo->Type == MIB_IF_TYPE_ETHERNET)
        {
            U64 result = 0LL;
            result |= (U64(pAdapterInfo->Address[0]) << (5*8));
            result |= (U64(pAdapterInfo->Address[1]) << (4*8));
            result |= (U64(pAdapterInfo->Address[2]) << (3*8));
            result |= (U64(pAdapterInfo->Address[3]) << (2*8));
            result |= (U64(pAdapterInfo->Address[4]) << (1*8));
            result |= (U64(pAdapterInfo->Address[5]) << (0*8));
            m.push_back(result);
        }
        pAdapterInfo = pAdapterInfo->Next;
    }
    return m;
}
B16 KSystem::GetSystemIdentityLUCID()
{
    Vx<U08> WINPID = Registry.GetData(HKEY_LOCAL_MACHINE, "SOFTWARE\\MICROSOFT\\Windows NT\\CurrentVersion", "DigitalProductId");//empty on failure
    U64 mac = GetMACAddress();// 0x800000000000LL when there's no ethernet card
    U64 REGID = 0LL;
    if(!(REGID = Registry.GetU64(HKEY_LOCAL_MACHINE, "SOFTWARE\\SubwareSoft\\DELTA\\LUCID Generator Data\\", "RandomUserID", 0LL)))
    {
        REGID = (Utils.SuperRandom() | 1LL);//Must not be zero!
        Registry.CreateTree(HKEY_LOCAL_MACHINE, "SOFTWARE\\SubwareSoft\\DELTA\\LUCID Generator Data\\");
        Registry.SetU64(HKEY_LOCAL_MACHINE, "SOFTWARE\\SubwareSoft\\DELTA\\LUCID Generator Data\\", "RandomUserID", REGID);
    }
    //Add in the other elements to ensure identity is unique across all machines.
    WINPID.push_back((mac >>   40) & 0xFF);
    WINPID.push_back((mac >>   32) & 0xFF);
    WINPID.push_back((mac >>   24) & 0xFF);
    WINPID.push_back((mac >>   16) & 0xFF);
    WINPID.push_back((mac >>    8) & 0xFF);
    WINPID.push_back((mac >>    0) & 0xFF);
    WINPID.push_back((REGID >> 56) & 0xFF);
    WINPID.push_back((REGID >> 48) & 0xFF);
    WINPID.push_back((REGID >> 40) & 0xFF);
    WINPID.push_back((REGID >> 32) & 0xFF);
    WINPID.push_back((REGID >> 24) & 0xFF);
    WINPID.push_back((REGID >> 16) & 0xFF);
    WINPID.push_back((REGID >>  8) & 0xFF);
    WINPID.push_back((REGID >>  0) & 0xFF);
    return SHA256(WINPID, Utils.SuperRandom()).To128();//Salt -> Multiple identity values per user!!
}
INT KSystem::GetTaskBarHeight()
{
	RECT rect;
	HWND taskBar = FindWindow(L"Shell_traywnd", NULL);
	if(taskBar && GetWindowRect(taskBar, &rect)){
		return rect.bottom - rect.top;
	}else{
		return 0;
	}
}
void KSystem::LoadCursors()
{
    cursors_[ 0] = LoadCursor(NULL, IDC_ARROW);
	cursors_[ 1] = LoadCursor(NULL, IDC_WAIT);
	cursors_[ 2] = LoadCursor(NULL, IDC_APPSTARTING);
	cursors_[ 3] = LoadCursor(NULL, IDC_IBEAM);
	cursors_[ 4] = LoadCursor(NULL, IDC_SIZEALL);
	cursors_[ 5] = LoadCursor(NULL, IDC_SIZENS);
	cursors_[ 6] = LoadCursor(NULL, IDC_SIZEWE);
	cursors_[ 7] = LoadCursor(NULL, IDC_SIZENESW);
	cursors_[ 8] = LoadCursor(NULL, IDC_SIZENWSE);
	cursors_[ 9] = LoadCursor(NULL, IDC_HAND);/// IDC_DRAGDROP????
	cursors_[10] = LoadCursor(NULL, IDC_HAND);
	cursors_[11] = LoadCursor(NULL, IDC_CROSS);
	cursors_[12] = LoadCursor(NULL, IDC_NO);
	MainCursorIndex = 0;
}
void KSystem::CheckAndResetCursor()
{
    if(cursor_reset_delay_ > 0){
        cursor_reset_delay_--;
    }else{
        MainCursorIndex = CURSOR_ARROW;
        SetCursor(cursors_[MainCursorIndex]);
    }
}
void KSystem::ChangeCursor(INT c)
{
	if(c == -1){
        if(MainCursorIndex == CURSOR_ARROW) return;
        cursor_reset_delay_ = 2;
        return;
	}
	//This check ensures that drag drop cursor is not overriden by the text edits in a treeview.
	if((MainCursorIndex == CURSOR_DRAGDROP) && (c == CURSOR_TEXT)) return;
	MainCursorIndex = c;
	cursor_reset_delay_ = 2;
	SetCursor(cursors_[MainCursorIndex]);
}
void KSystem::PlayRegistrySound(TXT app, TXT event)
{
	TXT path = (TXT("AppEvents\\Schemes\\Apps\\") + app + "\\" + event + "\\.Current");
	TXT crap = Registry.GetStr(HKEY_CURRENT_USER, path, "", "");
	T16 file = String.ToWide(crap);
	//in case we have some cryptic crap like %WINDOWS% in there...
	U32 wantsz = ExpandEnvironmentStrings(file.c_str(), NULL, 0); C16 buffer2[wantsz];
	if(ExpandEnvironmentStrings(file.c_str(), buffer2, wantsz)) file = buffer2;
	PlaySound(file.c_str(), NULL, SND_FILENAME | SND_ASYNC | SND_NODEFAULT);
}









