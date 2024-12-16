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
class CFileTypeManager;
class CFileType
{
	// A matching set of animations is defined by both animations
	// having the same ID bytes.
	friend class CFileTypeManager;
    public:
       Vx<BMP>* GetImages();
        TXT	    GetExtension();
        TXT*	GetDescription();
        U32	    GetDominantHue();
        void	LoadIcons();
                ~CFileType(); // Only friends can delete.
        
	protected:
		U08&	operator[](INT i);
		U32		GetDataSz() const;
                CFileType(TXT ext, TXT percent1_filepath, BIT delayload, BIT searchcopy);
        
	private:
static  U08		static_zero_;
static  Vx<BMP> delay_load_images_;
static  Vx<BMP> default_images_;
        Vx<BMP> images_;
		U32		dominant_hue_;
		TXT		percent1_file_path_, percent1_id_;
		TXT		extension_;
		TXT		type_;
		TXT		description_;
		TXT		icon_location_string_;
		TXT		icon_file_;
		U32		icon_index_;
		BIT		self_refrence_;
		BIT		is_particular_file_icons_;
		BIT		should_delay_load_icons_;
		BIT		has_loaded_icons_;
                CFileType(); // Only friends can create.
                CFileType(const CFileType&); // No copy-construction
                CFileType& operator=(const CFileType&); // No assignment
                void CalculateDominantHue_(BMP dest);
};
CFileType* 	FileType(TXT ext, TXT percent1_filepath="", BIT delay_icon_load=false);
BIT		    FileType_DelayLoadAnIcon();

