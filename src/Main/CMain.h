#pragma once
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
class CMain
{
	public:
static LONG             DebugCrashFunc(_EXCEPTION_POINTERS *exception_info);
static LONG             CrashFunc(LPEXCEPTION_POINTERS p);
static BOOL CALLBACK    CrashDialog(HWND hwnd, UINT Message, WPARAM wParam, LPARAM lParam);
static INT              Init(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, INT nCmdShow);
static void             Fail();
static void             Exit(INT failure = 0);
static void             Program(Vx<TXT> args);

};
