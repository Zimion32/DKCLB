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
#include "include.h"
#include "CMain.h"

BIT ENABLE_LOGGING_DELTAERRORS = false;

static void SetupLogging()
{
    INT everything_value         = INI.GetSetInt("Log","All", 1);
    ENABLE_LOGGING_DELTAERRORS   = INI.GetSetInt("Log","DeltaErrors",   0) || everything_value;
    // (...)
    
    DeltaError.Configure(ENABLE_LOGGING_DELTAERRORS, true);
}
extern U32 abort_called_status;
LONG CMain::DebugCrashFunc(_EXCEPTION_POINTERS *exception_info)
{
    abort_called_status = 0;
    Debug.CrashDump(exception_info->ContextRecord);
	return EXCEPTION_CONTINUE_SEARCH;
	//return EXCEPTION_CONTINUE_EXECUTION;
}
LONG CMain::CrashFunc(LPEXCEPTION_POINTERS p)
{
	DialogBox(GetModuleHandle(NULL), MAKEINTRESOURCE(55555), NULL, CrashDialog);
	return EXCEPTION_EXECUTE_HANDLER;
}
BOOL CALLBACK CMain::CrashDialog(HWND hwnd, UINT Message, WPARAM wParam, LPARAM lParam)
{
	switch(Message)
	{
		case WM_INITDIALOG:	{PostMessage(hwnd, WM_MOUSEMOVE, 0, 0); return true;} break;
		case WM_MOUSEMOVE:	{SetWindowPos(hwnd, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE|SWP_NOSIZE);} break;
		case WM_COMMAND:    {if(LOWORD(wParam) == IDOK){TerminateProcess(GetCurrentProcess(), 0);}} break;
		default:            return false; break;
	}
	return true;
}
void WinExit()
{
	Debug.Log("EXIT SUCCESSFUL");
	///RC::CleanUp();
}
INT WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, INT nCmdShow)
{
	atexit(&WinExit);
	Debug.Log("EXECUTABLE RUNNING");
	Debug.Log(TXT("STARTED \"DKC Level Builder Toolkit\" [NOTE: Check the INI file for extra log file options!!]"));
	INT err = CMain::Init(hInstance, hPrevInstance, lpCmdLine, nCmdShow);
	if(err == 0){
        CMain::Exit();
	}
    Debug.Log("EXITING");
    return err;
}
INT CMain::Init(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, INT nCmdShow)
{
    INT err = 0;
	
	T16 wide_args = String.ToWide(lpCmdLine);
	INT num_args = 0;
	C16** argv = NULL;
	argv = CommandLineToArgvW(wide_args.c_str(), &num_args);
	Vx<TXT> args;
	for(INT i = 0; i < num_args; i++) args.push_back(String.FromWide(argv[i]));
	
	Debug.Log("CMain::Init()");
	if(!System.OneInstanceOnly()) return Error::NONE;
	SetCurrentDirectory(String.ToWideC(FileSystem.ProgramDirectory()));
	ConvertThreadToFiber(NULL);
    SetErrorMode(SEM_FAILCRITICALERRORS | SEM_NOGPFAULTERRORBOX);
	//SetUnhandledExceptionFilter((LPTOP_LEVEL_EXCEPTION_FILTER)&CrashFunc);
    AddVectoredExceptionHandler(1, (PVECTORED_EXCEPTION_HANDLER)&DebugCrashFunc);

	INI.Open(FileSystem.FileName(FileSystem.ProgramFile())+".ini");
	SetupLogging();
    
	// WARNING: MUST INIT ALLEGRO MANUALLY AND REPO MANUALLY TO PREVENT HANGS ON EXIT.
	///win_set_window(GetConsoleWindow()); //COMMENTED OUT: CAUSES ALLEGRO TO NOT INIT
	if(install_allegro(SYSTEM_AUTODETECT, &errno, NULL)){
	    err = 1;
	    Fail();
	    return err;
    }
	three_finger_flag = false; // Prevents a crash upon activating CTRL+ALT+DEL
	Debug.Log("CMain::Init() SUCCESSFUL");
	
	CMain::Program(args);
	
	return 0;
}
void CMain::Fail()
{
	Debug.Log("CMain::Fail()");
	Exit(true);
}
extern BIT ALLEGRO_IS_SHUTDOWN;
void CMain::Exit(INT failure)
{
	Debug.Log("CMain::Exit()");
	allegro_exit(); // WARNING: MUST INIT ALLEGRO MANUALLY AND REPO MANUALLY TO PREVENT HANGS ON EXIT.
	ALLEGRO_IS_SHUTDOWN = true;
	if(!failure) INI.Save();
	INI.Exit();
	Debug.Log("CMain::Exit() COMPLETED");
}
//##############################################################################
//##############################################################################
//##############################################################################
//##############################################################################
//##############################################################################
//##############################################################################
//##############################################################################
//##############################################################################
//##############################################################################
//##############################################################################









