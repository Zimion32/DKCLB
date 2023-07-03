#pragma once
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
class CFileInfo
{
    public:
                CFileInfo();
                
        //These are the header info loaders. If already loaded there is no effect.
        void    WriteHeaderTo(CFileIO& file, INT filecount);
        void    WriteHeaderTo(CFileIO& file);
        void    WriteSubHeaderTo(CFileIO& file);
        void    WriteDescriptionTo(CFileIO& file);
        void    ReadDescriptionFrom(CFileIO& file);
        void    LoadDescription();
        
    private:
        U32     type_;   //8-bit file type designation as defined by DELTA header.
        LUCID   lucid_;//24 bytes
        EXTHD   exthd_;//DATA PART 2: DELTA Extended Header. 32 Bytes.
        //=========================
        // This data has to be part of the CFileInfo due to INTerface stuff (showing thumbnails, etc).
        // The BITMAPs are NULL until the INTerface explicitly calls the loader function for them.
        BMP     icon_;   // 16x16 display icon for list/tree views
        BMP     icon32_; // 32x32 display icon
        BMP     thumb_;  // Large resize-able custom thumbnail. Will have empty space dropped from the edges.
        BMP     photo_;  // Intended for a resized screenshot of the entire level/object/etc.
        TXT     what_;
        TXT     author_;
        TXT     description_;
        TXT     notes_;
};
#define FILE_FORMAT_DLT 0x00 //DELTA Project File
#define FILE_FORMAT_CDL 0x01 //Compiled DELTA Game Data
#define FILE_FORMAT_DGS 0x02 //Game Settings
#define FILE_FORMAT_DLV 0x03 //Level
#define FILE_FORMAT_DPX 0x04 //Graphics
#define FILE_FORMAT_DMS 0x05 //Music Set Script
#define FILE_FORMAT_DSS 0x06 //Sound Effects Set Script
#define FILE_FORMAT_DPH 0x07 //Path
#define FILE_FORMAT_DPT 0x08 //Physics Tileset
#define FILE_FORMAT_DLO 0x09 //Level Object Engine (DLL Type)
#define FILE_FORMAT_DAS 0x0A //Internal "Embedded Audio Stream" Format for CDLs
#define FILE_FORMAT_DAP 0x0B //Audio Plugin
#define FILE_FORMAT_DTS 0x0C //Tileset              (DPX duplicate)
#define FILE_FORMAT_DBG 0x0D //Background           (DPX duplicate)
#define FILE_FORMAT_DFT 0x0E //Bitmap Font          (DPX duplicate)
#define FILE_FORMAT_DGL 0x0F //Games Library Mode   (DLV duplicate)
#define FILE_FORMAT_DSC	0x10 //Screen (General)     (DLV duplicate)
#define FILE_FORMAT_DLS	0x11 //Loading Screen       (DLV duplicate)
#define FILE_FORMAT_DWM	0x12 //World Map            (DLV duplicate)
#define FILE_FORMAT_DDT	0x13 //Physics Drift Table
#define FILE_FORMAT_DPB	0x14 //Polygon Bounding Box
#define FILE_FORMAT_DXO	0x15 //Clipboard - Objects
#define FILE_FORMAT_DXT	0x16 //Clipboard - Tiles
#define FILE_FORMAT_DXP	0x17 //Clipboard - Physics Tiles
#define FILE_FORMAT_DSX 0x18 //DELTA Easy Script File
#define FILE_FORMAT_DLI 0x19 //Level Object Script
#define FILE_FORMAT_DXH 0x1A //Clipboard - Paths
#define FILE_FORMAT_MAXIMUM 0x1B
#include "file_chunk_id.h"
