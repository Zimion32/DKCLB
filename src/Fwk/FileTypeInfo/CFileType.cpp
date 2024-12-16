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
#include "CFileTypeManager.h"
#include "CFileType.h"

Vx<BMP> GetFileTypeImages(TXT& file, BIT self_refrence, U32 index);
INT ARGB_GetHue0to255(U32 color);
INT ARGB_GetSat0to255(U32 color);
INT ARGB_GetVal0to255(U32 color);
U32 Hue0to255ToARGB(INT hue);

static TXT GetFirstExtensionInFilter(TXT filter)
{
	size_t found = filter.find_first_of("\\/");
	if(found != TXT::npos)
	{
		return filter.substr(0, found);
	}
	return filter;
}
static TXT GetExtAllCaps(TXT ext)
{
	ext = GetFirstExtensionInFilter(ext);
	for(IND i = 0; i < ext.size(); ++i) ext[i] = toupper(ext[i]);
	if(ext == "") return "File";
	if(ext == "*") return "All Files";
	return (ext + " File");
}
static TXT GetExtAllLower(TXT ext)
{
	for(IND i = 0; i < ext.size(); ++i) ext[i] = tolower(ext[i]);
	return ext;
}
static TXT PathForceLongNames(TXT& input)
{
	static C16 buffer[MAX_PATH];
	ZeroMemory((PTR)buffer,MAX_PATH*sizeof(C16));
	GetLongPathName(String.ToWideC(input),buffer,MAX_PATH);
	return String.FromWide(T16(buffer));
}

//#include "CNitroGUI.h"

U08 CFileType::static_zero_ = 0x00; // KEEP THIS (see below)
Vx<BMP> CFileType::delay_load_images_;
Vx<BMP> CFileType::default_images_;
CFileType::CFileType(TXT ext, TXT percent1_filepath, BIT delayload, BIT searchcopy):
extension_(GetExtAllLower(ext)), description_(GetExtAllCaps(ext)),
icon_location_string_(""), icon_file_(""), icon_index_(0), dominant_hue_(0xFFFFFFFF),
percent1_file_path_(percent1_filepath), percent1_id_(GetExtAllLower(GetFirstExtensionInFilter(ext)) + "?" + percent1_filepath),
should_delay_load_icons_(delayload), has_loaded_icons_(false)
{
	if(!delay_load_images_.size())
	{
		try{
        delay_load_images_.push_back(create_bitmap_ex(32,16,16));
        delay_load_images_.push_back(create_bitmap_ex(32,32,32));
        delay_load_images_.push_back(create_bitmap_ex(32,48,48));
        delay_load_images_.push_back(create_bitmap_ex(32,16,16));
        delay_load_images_.push_back(create_bitmap_ex(32,32,32));
        delay_load_images_.push_back(create_bitmap_ex(32,48,48));
        default_images_.push_back(create_bitmap_ex(32,16,16));
        default_images_.push_back(create_bitmap_ex(32,32,32));
        default_images_.push_back(create_bitmap_ex(32,48,48));
        default_images_.push_back(create_bitmap_ex(32,16,16));
        default_images_.push_back(create_bitmap_ex(32,32,32));
        default_images_.push_back(create_bitmap_ex(32,48,48));
        default_images_.push_back(create_bitmap_ex(32,16,16));
		}catch(const exception& e){
		}endtry;
	}
	
	is_particular_file_icons_ = false;
	if((extension_ == "exe") || (extension_ == "ico") || (extension_ == "icl")){
	    is_particular_file_icons_ = (percent1_filepath != "");
	}
	
	if(searchcopy) return;
	TXT ext_reg_entry = (TXT(".")+GetFirstExtensionInFilter(extension_));
	type_ = Registry.GetStr(HKEY_CLASSES_ROOT, ext_reg_entry, "", "");
	if(type_ != "")
	{
		TXT d = Registry.GetStr(HKEY_CLASSES_ROOT, (type_), "", "");
		if(d != ""){description_ = d;}
		icon_location_string_ = Registry.GetStr(HKEY_CLASSES_ROOT, (type_+"\\DefaultIcon"), "", "");
		if(icon_location_string_ == ""){
			TXT curver_folder = Registry.GetStr(HKEY_CLASSES_ROOT, (type_+"\\CurVer"), "", "");
			if(curver_folder != ""){
				icon_location_string_ = Registry.GetStr(HKEY_CLASSES_ROOT, (curver_folder+"\\DefaultIcon"), "", "");
			}
		}
	}
	if(percent1_filepath == ""){
        if(extension_ == "*"){
            images_.push_back(default_images_[6]);
            images_.push_back(default_images_[6]);
            images_.push_back(default_images_[6]);
            has_loaded_icons_ = true;
        }
        if(extension_ == "exe"){
            images_.push_back(default_images_[3]);
            images_.push_back(default_images_[4]);
            images_.push_back(default_images_[5]);
            has_loaded_icons_ = true;
        }
        if((extension_ == "ico") || (extension_ == "icl")){
            images_.push_back(default_images_[0]);
            images_.push_back(default_images_[1]);
            images_.push_back(default_images_[2]);
            has_loaded_icons_ = true;
        }
	}
	if(!images_.size()){
        if(!should_delay_load_icons_)
        {
            LoadIcons();
        }
        else
        {	//show user temporary images until real one is loaded in
            if(GetFirstExtensionInFilter(extension_) == "exe")
            {
                images_.push_back(delay_load_images_[3]);
                images_.push_back(delay_load_images_[4]);
                images_.push_back(delay_load_images_[5]);
            }
            else
            {
                images_.push_back(delay_load_images_[0]);
                images_.push_back(delay_load_images_[1]);
                images_.push_back(delay_load_images_[2]);
            }
        }
	}
}
CFileType::~CFileType() {}
U08& CFileType::operator[](INT i)
{
	if(!is_particular_file_icons_)
	{
		if(i >= extension_.size()) return static_zero_; // KEEP THIS: USED FOR DATA BLOCK SORTING PURPOSES
		return ((U08*)(&extension_[0]))[i];
	}
	else
	{
		if(i >= percent1_id_.size()) return static_zero_; // KEEP THIS: USED FOR DATA BLOCK SORTING PURPOSES
		return ((U08*)(&percent1_id_[0]))[i];
	}
}
U32 CFileType::GetDataSz() const
{
	if(!is_particular_file_icons_)
	{
		return extension_.size();
	}
	else
	{
		return percent1_id_.size();
	}
}
Vx<BMP>* CFileType::GetImages()
{
	return &images_;
}
TXT CFileType::GetExtension()
{
	return extension_;
}
TXT* CFileType::GetDescription()
{
	return &description_;
}
U32 CFileType::GetDominantHue()
{
	return dominant_hue_;
}
void CFileType::CalculateDominantHue_(BMP dest)
{
	F64 hue_buckets[32];
	F64 hue_buckets_counts[32];
	for(IND i = 32; i >= 0; --i){
		hue_buckets[i] = 0;
		hue_buckets_counts[i] = 0;
	}
	for(IND yy = dest->h-1; yy >= 0; --yy){
		for(IND xx = 0; xx < dest->w; ++xx){
			U32 color = _getpixel32(dest,xx,yy);
			//avoid reading no-alpha pixels.
			if((color & 0xFF000000) == 0) continue;
			//avoid no-hue pixels!
			if(((color & 0xFF) == ((color >>  8) & 0xFF))
			&& ((color & 0xFF) == ((color >> 16) & 0xFF))) continue;
			//be sure that low saturation is rewarded less, otherwise it might look wierd
			hue_buckets		  [ARGB_GetHue0to255(color) >> 3] += ARGB_GetSat0to255(color);
			hue_buckets_counts[ARGB_GetHue0to255(color) >> 3]++;
		}
	}
	INT best = 0;
	F64 best_value = -1.0;
	for(IND i = 32; i >= 0; --i){
        F64 d = hue_buckets_counts[i];
        if(d < 1.0) d = 1.0;
		hue_buckets[i] /= d;
		if(hue_buckets[i] > best_value){
			best = i; best_value = hue_buckets[i];
		}
	}
	if(best_value != -1.0)
	{
		dominant_hue_ = Hue0to255ToARGB(best << 3);
	}
}
void CFileType::LoadIcons()
{
	if(!has_loaded_icons_)
	{
		images_.clear(); //gets rid of delayed-load icons!
		if(type_ != "")
		{
		    if((icon_location_string_ != "") && !is_particular_file_icons_)
			{
				U32 wantsz = ExpandEnvironmentStrings(String.ToWideC(icon_location_string_), NULL, 0);
				C16 buffer2[wantsz+2];
				ExpandEnvironmentStrings(String.ToWideC(icon_location_string_), buffer2, wantsz);
				icon_location_string_ = String.FromWide(buffer2);
                
				//Removes quotes around the filepath if there are any.
				unsigned found = icon_location_string_.find_first_of("\"");
				while(found != TXT::npos){
					icon_location_string_.erase(icon_location_string_.begin()+found);
					found = icon_location_string_.find_first_of("\"",found+1);
				}
				//Removes duplicated slashes (that's a registry typo).
				found = icon_location_string_.find_first_of("\\");
				while(found != TXT::npos){
					while(((found+1) < icon_location_string_.size()) && (icon_location_string_[found+1] == '\\'))
					{
						icon_location_string_.erase(icon_location_string_.begin()+found+1);
					}
					found = icon_location_string_.find_first_of("\\",found+1);
				}

				TXT IconIndex = "";
				self_refrence_ = (icon_location_string_ == "%1");
				BIT is_negativesign = false;

				found = icon_location_string_.find_first_of("\\/");
				if(found == TXT::npos)
				{//this means the file is within system32 directory. prepend that directory.
					icon_location_string_ = (TXT("C:\\WINDOWS\\System32\\") + icon_location_string_);
				}
				found = icon_location_string_.find_last_of(",");
				if(found != TXT::npos){
					IconIndex = icon_location_string_.substr(found+1,(icon_location_string_.size()-(found+1)));
				}else{
					IconIndex = "*";
				}
				if(IconIndex.size() && (IconIndex[0]=='-')){
					IconIndex.erase(IconIndex.begin());
					is_negativesign = true;
				}
				if(found != TXT::npos){
					icon_file_ = icon_location_string_.substr(0,found);
				}else{
					icon_file_ = icon_location_string_;
				}

				// We can get long filenames now (function call fails if it's not a valid path).
				// The function here will return blank if the path does not exist.
				icon_file_ = PathForceLongNames(icon_file_);
				if(self_refrence_) icon_file_ = "%1";
				if(icon_file_ != "")
				{
					icon_index_ = ((atoi(IconIndex.c_str()) & 0x7FFFFFFF) | (is_negativesign ? 0x80000000 : 0));
					images_ = GetFileTypeImages(icon_file_, self_refrence_, icon_index_);
					if(images_.size() && images_[0]){
						CalculateDominantHue_(images_[0]);
					}
					//BFA.SvText((ext_reg_entry+"\t"+((is_negativesign)?"-":"+")+IconIndex+"\t"+icon_file_+"\r\n"));
				}
			}
		}
		if(is_particular_file_icons_)
		{
			images_ = GetFileTypeImages(percent1_file_path_, true, 0);
			if(images_.size() && images_[0]){
				CalculateDominantHue_(images_[0]);
			}
		}
		else if(icon_location_string_ == ""){
            if(GetFirstExtensionInFilter(extension_) == "exe"){
                images_.push_back(default_images_[3]);
                images_.push_back(default_images_[4]);
                images_.push_back(default_images_[5]);
            }else{
                images_.push_back(default_images_[0]);
                images_.push_back(default_images_[1]);
                images_.push_back(default_images_[2]);
            }
        }
        if(!images_.size()){
            images_.push_back(default_images_[0]);
            images_.push_back(default_images_[1]);
            images_.push_back(default_images_[2]);
        }
		has_loaded_icons_ = true;
		/*try{
            FIO.SavePNG(images_[0], TXT("TESTING\\")+icon_file_+"_00.png");
            FIO.SavePNG(images_[1], TXT("TESTING\\")+icon_file_+"_01.png");
            FIO.SavePNG(images_[2], TXT("TESTING\\")+icon_file_+"_02.png");
		}catch(const exception& e){
		}endtry;*/
		
	}
}
CFileType* 	FileType(TXT ext, TXT percent1_filepath, BIT delay_icon_load)
{
	return CFileTypeManager::CreateFileType_(ext, percent1_filepath, delay_icon_load);
}
BIT		FileType_DelayLoadAnIcon()
{
	return CFileTypeManager::DoDelayedLoadOfOneIcon_();
}

