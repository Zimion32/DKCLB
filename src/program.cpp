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
#include "CMain.h"
static void DisplayLegalNotice()
{
    cout << "--------------------------------------------------------------------------------" << endl;
    cout << "  DKC Level Builder Toolkit - Copyright (C) 2023 Simion32" << endl;
    cout << "--------------------------------------------------------------------------------" << endl;
    cout << "  This program comes with ABSOLUTELY NO WARRANTY." << endl;
    cout << "  This is free software, and you are welcome to redistribute it" << endl;
    cout << "  under certain conditions. See the included file \"COPYING.txt\"." << endl;
    cout << "--------------------------------------------------------------------------------" << endl;
}
void CMain::Program(Vx<TXT> args)
{
    DisplayLegalNotice();
    cout << endl;
    cout << endl;
    cout << "Processing Complete, Press ENTER to Exit." << endl;
    cin.get();
}


