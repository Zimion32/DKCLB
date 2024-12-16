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
#include <list>
#include "include.h"
#include "dbghelp.h"

void DebugAppendCallTrace(TXT& errorMessage, PCONTEXT context)
{
    // Store the names of all the functions called here:
    std::list<TXT> functionNames;

    // Initialise the symbol table to get function names:
    SymInitialize(
        GetCurrentProcess(),        // Process to get symbol table for
        0,                          // Where to search for symbol files: dont care
        true                        // Get symbols for all dll's etc. attached to process
    );

    // Store the current stack frame here:
    STACKFRAME frame;
    ZeroMemory(&frame,sizeof(STACKFRAME));
    
    // Use this to fill the frame structure:
    frame.AddrPC.Offset         = context->Eip;      // Current location in program
    frame.AddrPC.Mode           = AddrModeFlat;     // Address mode for this pointer: flat 32 bit addressing
    frame.AddrStack.Offset      = context->Esp;      // Stack pointers current value
    frame.AddrStack.Mode        = AddrModeFlat;     // Address mode for this pointer: flat 32 bit addressing
    frame.AddrFrame.Offset      = context->Ebp;      // Value of register used to access local function variables.
    frame.AddrFrame.Mode        = AddrModeFlat;     // Address mode for this pointer: flat 32 bit addressing

    // Keep getting stack frames from windows till there are no more left:
    while(
        StackWalk(
            IMAGE_FILE_MACHINE_I386 ,       // MachineType - x86
            GetCurrentProcess()     ,	    // Process to get stack trace for
            GetCurrentThread()      ,	    // Thread to get stack trace for
            & frame                 ,	    // Where to store next stack frame
             context               ,	    // Pointer to processor context record
            0                       ,	    // Routine to read process memory: use the default ReadProcessMemory
            SymFunctionTableAccess  ,	    // Routine to access the modules symbol table
            SymGetModuleBase        ,       // Routine to access the modules base address
            0                               // Something to do with 16-bit code. Not needed.
        )
    ){
            //------------------------------------------------------------------
            // Declare an image help symbol structure to hold symbol info and
            // name up to 256 chars This struct is of variable lenght though so
            // it must be declared as a raw byte buffer.
            //------------------------------------------------------------------
            static char symbolBuffer[ sizeof(IMAGEHLP_SYMBOL) + 255 ];
            memset( symbolBuffer , 0 , sizeof(IMAGEHLP_SYMBOL) + 255 );

            // Cast it to a symbol struct:
            IMAGEHLP_SYMBOL * symbol = (IMAGEHLP_SYMBOL*) symbolBuffer;

            // Need to set the first two fields of this symbol before obtaining name info:
            symbol->SizeOfStruct    = sizeof(IMAGEHLP_SYMBOL) + 255;
            symbol->MaxNameLength   = 254;

            // The displacement from the beginning of the symbol is stored here: pretty useless
            unsigned displacement = 0;
            
            // Get the symbol information from the address of the instruction pointer register:
            if(
                SymGetSymFromAddr(
                    GetCurrentProcess()     ,   // Process to get symbol information for
                    frame.AddrPC.Offset     ,   // Address to get symbol for: instruction pointer register
                    (DWORD*) & displacement ,   // Displacement from the beginning of the symbol: whats this for ?
                    symbol                      // Where to save the symbol
                )
            ){
                // Add the name of the function to the function list:
                char buffer[2048]; sprintf( buffer , "0x%08x %s" ,  frame.AddrPC.Offset , symbol->Name );
                functionNames.push_back(buffer);
            }else{
                // Print an unknown location:
                // functionNames.push_back("unknown location");
                char buffer[64]; sprintf( buffer , "0x%08x" ,  frame.AddrPC.Offset );
                functionNames.push_back(buffer);
            }
    }

    // Cleanup the symbol table:
    SymCleanup( GetCurrentProcess() );

    //--------------------------------------------------------------------------
    // Print the names of all the functions called
    //--------------------------------------------------------------------------
    {
        errorMessage += "\nBack Trace:\n--------------------------------";
        
        // Store the number of functions printed here:
        unsigned count = 1;
        std::list<TXT>::iterator i = functionNames.begin();
        std::list<TXT>::iterator e = functionNames.end();
        while((i != e) && (count < 64)){
            TXT& name = *i; i++;
            errorMessage += "\n";
            errorMessage += String.FromInt(count, 4, true);
            errorMessage += " - ";
            errorMessage += name;
            count++;
        }
    }

}

