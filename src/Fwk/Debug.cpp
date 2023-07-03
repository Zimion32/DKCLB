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
#ifdef DEBUG_BUILD
BIT nvwaDebugLoggingEnabled = true;
#else
BIT nvwaDebugLoggingEnabled = false;
#endif
KDebug::KDebug()
{
    SetCurrentDirectory(String.ToWideC(FileSystem.ProgramDirectory()));
    TXT fn = FileSystem.FileName(FileSystem.ProgramFile())+".log";
    FileSystem.DeleteFile(fn);
    freopen(fn.c_str(),"wb",stderr);
}
KDebug::~KDebug()
{

}
void KDebug::Log(TXT text)
{
	fputs(text.c_str(),stderr);//BUGFIX: Use fputs instead of fprINTf.
    fputs("\n", stderr);//BUGFIX: Use fputs instead of fprINTf.
    fflush(stderr);
}
void KDebug::Box()
{
	MessageBox(System.MainHWND, L"DebugBox was called without parameter.", L"Debug Message", MB_OK | MB_ICONERROR);
}
void KDebug::Box(TXT text)
{
	MessageBox(System.MainHWND, String.ToWideC(text), L"Debug Message", MB_OK | MB_ICONERROR);
}
void KDebug::Box(TXT text, TXT title)
{
	MessageBox(System.MainHWND, String.ToWideC(text), String.ToWideC(title), MB_OK | MB_ICONERROR);
}
void KDebug::Box(TXT text1, TXT text2, TXT title)
{
	MessageBox(System.MainHWND, String.ToWideC(text1 + "\r\n\r\n" + text2), String.ToWideC(title), MB_OK | MB_ICONERROR);
}
void KDebug::Box(INT value)
{
	TXT dbgstr = "0x"; dbgstr += String.FromHex(value , 8);
	MessageBox(System.MainHWND, String.ToWideC(dbgstr), L"Debug Message", MB_OK | MB_ICONERROR);
}
void KDebug::Box(INT value1, INT value2)
{
	TXT dbgstr = "0x"; dbgstr += String.FromHex(value1, 8);
	dbgstr += ", 0x"; dbgstr += String.FromHex(value2, 8);
	MessageBox(System.MainHWND, String.ToWideC(dbgstr), L"Debug Message", MB_OK | MB_ICONERROR);
}
void KDebug::Box(INT value1, TXT title)
{
	TXT dbgstr = "0x"; dbgstr += String.FromHex(value1, 8);
	MessageBox(System.MainHWND, String.ToWideC(dbgstr), String.ToWideC(title), MB_OK | MB_ICONERROR);
}
void KDebug::Box(INT value1, INT value2, TXT title)
{
	TXT dbgstr = "0x"; dbgstr += String.FromHex(value1, 8);
	dbgstr += ", 0x"; dbgstr += String.FromHex(value2, 8);
	MessageBox(System.MainHWND, String.ToWideC(dbgstr), String.ToWideC(title), MB_OK | MB_ICONERROR);
}
void KDebug::Box(INT value1, INT value2, INT value3, TXT title)
{
	TXT dbgstr = "0x"; dbgstr += String.FromHex(value1, 8);
	dbgstr += ", 0x"; dbgstr += String.FromHex(value2, 8);
	dbgstr += ", 0x"; dbgstr += String.FromHex(value3, 8);
	MessageBox(System.MainHWND, String.ToWideC(dbgstr), String.ToWideC(title), MB_OK | MB_ICONERROR);
}
void KDebug::Box(INT value1, INT value2, INT value3, INT value4, TXT title)
{
	TXT dbgstr = "0x"; dbgstr += String.FromHex(value1, 8);
	dbgstr += ", 0x"; dbgstr += String.FromHex(value2, 8);
	dbgstr += ", 0x"; dbgstr += String.FromHex(value3, 8);
	dbgstr += ", 0x"; dbgstr += String.FromHex(value4, 8);
	MessageBox(System.MainHWND, String.ToWideC(dbgstr), String.ToWideC(title), MB_OK | MB_ICONERROR);
}
TXT KDebug::GetLastErrorAsString()
{
    DWORD errorMessageID = GetLastError();
    if(errorMessageID == 0) return "No Error";
    C16* messageBuffer = NULL;
    size_t size = FormatMessageW(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
                                 NULL, errorMessageID, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), ((WCHAR*)(&messageBuffer)), 0, NULL);
    T16 message(messageBuffer, size);
    LocalFree(messageBuffer);
    return String.FromWide(message);
}



