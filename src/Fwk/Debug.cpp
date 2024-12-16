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
#include <stdio.h>
/*#include <execinfo.h>
#include <signal.h>
#include <stdlib.h>
#include <unistd.h>*/
#include <exception>
#include "include.h"
#include "CMain.h"
#ifdef DEBUG_BUILD
BIT nvwaDebugLoggingEnabled = true;
#else
BIT nvwaDebugLoggingEnabled = false;
#endif

U32 abort_called_status = 0;
extern TXT abort_reason;

static void custom_terminate()
{
    abort_reason += "Unhandled C++ Exception Encountered!!";
    abort(); //will do pointless crashdump (triggers call to OnSigAbrt)
}

void DebugAppendCallTrace(TXT& errorMessage, PCONTEXT context);
void KDebug::OnSigAbrt(int signal)
{
    abort_called_status = 1;
    
    // Get processor information for the current thread:
    CONTEXT context; memset(&context, 0, sizeof(CONTEXT));
    context.ContextFlags = CONTEXT_FULL;
    // Load the RTLCapture context function:
    HINSTANCE kernel32 = LoadLibrary(L"Kernel32.dll");
    typedef void (*RtlCaptureContextFunc)(CONTEXT* ContextRecord);
    RtlCaptureContextFunc rtlCaptureContext = (RtlCaptureContextFunc) GetProcAddress(kernel32, "RtlCaptureContext");
    // Capture the thread context
    rtlCaptureContext(&context);
    
    CrashDump(&context);
}
void KDebug::CrashDump(PCONTEXT context)
{
    Debug.Log("\n");
	TXT error_message;
	error_message += ("################################################################\n");
	error_message += ("\nPROGRAM CRASH!! Please help by sending Simion32 this file!!\n");
	if(abort_called_status){
        error_message += "\nINFO: ";
        error_message += abort_reason;
        error_message += "\n";
	}
	DebugAppendCallTrace(error_message, context);
	error_message += "\n\nProgram ID:        ";
	error_message += String.FromHex(PROGRAM_ID, 8);
	error_message += "\nProgram Title:     ";
	error_message += PROGRAM_TITLEBAR;
	error_message += "\nProgram Version:   ";
	error_message += PROGRAM_VERSION;
	error_message += ("\n\n################################################################\n");
	Debug.Log(error_message);
	
	//reopen the file and dump the crash info into a specific file
	SetCurrentDirectory(String.ToWideC(FileSystem.ProgramDirectory()));
    TXT fn = FileSystem.FileName(FileSystem.ProgramFile())+".crash.log";
    //FileSystem.DeleteFile(fn); //DON'T DO THIS, WE MIGHT HAVE MULTIPLE CRASHES.
    freopen(fn.c_str(),"ab",stderr); ///Append Mode
    Debug.Log(error_message);
	fn = FileSystem.FileName(FileSystem.ProgramFile())+".log";
    freopen(fn.c_str(),"ab",stderr); ///temporarily reopen the main debug log
	
	DialogBox(GetModuleHandle(NULL), MAKEINTRESOURCE(55555), NULL, CMain::CrashDialog);
}
KDebug::KDebug()
{
    SetCurrentDirectory(String.ToWideC(FileSystem.ProgramDirectory()));
    TXT fn = FileSystem.FileName(FileSystem.ProgramFile())+".log";
    FileSystem.DeleteFile(fn);
    freopen(fn.c_str(),"wb",stderr);
    
    signal(SIGABRT, OnSigAbrt);
    std::set_terminate(custom_terminate);
}
KDebug::~KDebug()
{

}
void KDebug::Abort()
{
    abort();
}
void KDebug::Abort(TXT information)
{
    BegAbortProtect(information);
    abort();
    EndAbortProtect();
}
void KDebug::AbortBox(TXT information)
{
    BegAbortProtect(information);
    MessageBox(System.MainHWND, String.ToWideC(information), L"CRITICAL ERROR", MB_OK | MB_ICONERROR);
    abort();
    EndAbortProtect();
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
void KDebug::Error(INT what)
{
    TXT title = "Something went wrong!!";
    TXT message = "There was an error trying to perform some operation.\r\n";
    message += "The following information should describe the problem:";
    message += "\r\n\r\nERROR:\t";
    message += ErrorDescriptions[what].text;
    message += "\r\nDescription:\r\n";
    message += ErrorDescriptions[what].desc;
    message += "\r\n\r\nGetLastError():\r\n";
    message += GetLastErrorAsString();
    message += "\r\n\r\nallegro_error:\r\n";
    message += allegro_error;
    fputs("\n\n",stderr);//BUGFIX: use fputs not fprINTf.
    fputs(message.c_str(),stderr);//BUGFIX: use fputs not fprINTf.
    fputs("\n\n",stderr);//BUGFIX: use fputs not fprINTf.
    fflush(stderr);
    Box(message, title);
}


