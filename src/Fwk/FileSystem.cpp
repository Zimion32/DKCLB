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
TXT KFileSystem::ParsePathReparsePoints(TXT path)
{
	U32 wantsz = ExpandEnvironmentStrings(String.ToWideC(path), NULL, 0);
	C16 buffer2[wantsz+2];
	ExpandEnvironmentStrings(String.ToWideC(path), buffer2, wantsz);
	return String.FromWide(buffer2);
}
TXT KFileSystem::UncDoPrefix(TXT path)
{
	if(path.substr(0,4) != "\\\\?\\"){path = TXT("\\\\?\\")+path;}
	return path;
}
TXT KFileSystem::UncUnPrefix(TXT path)
{
	if(path.substr(0,4) == "\\\\?\\"){path = path.substr(4,TXT::npos);}
	return path;
}
T16 KFileSystem::UncDoPrefix(T16 path)
{
	if(path.substr(0,4) != L"\\\\?\\"){path = T16(L"\\\\?\\")+path;}
	return path;
}
T16 KFileSystem::UncUnPrefix(T16 path)
{
	if(path.substr(0,4) == L"\\\\?\\"){path = path.substr(4,T16::npos);}
	return path;
}
//####################################################################################
//####################################################################################
//####################################################################################
//####################################################################################
TXT	KFileSystem::Directory(TXT path_string)
{
	size_t found = path_string.find_last_of("/\\");
	if(found == TXT::npos) return path_string;
	return path_string.substr(0,found+1);
}
TXT	KFileSystem::UpDirectory(TXT path_string)
{
	size_t found = path_string.find_last_of("/\\");
	if(found == TXT::npos) return "My Computer";
	TXT tempf = path_string.substr(0,found);
	if((tempf.size() == 2) && (tempf[1] == ':')){
        return "My Computer";
	}
	return Directory(tempf);
}
TXT	KFileSystem::Folder(TXT path_string)
{
	size_t found = path_string.find_last_of("/\\");
	if(found == TXT::npos) return path_string;
	TXT tempf = path_string.substr(0,found);
	size_t found2 = tempf.find_last_of("/\\");
	if(found2 == TXT::npos) return tempf;
	return tempf.substr(found2+1);
}
TXT	KFileSystem::File(TXT path_string)
{
	size_t found = path_string.find_last_of("/\\");
	if(found == TXT::npos) return path_string;
	return path_string.substr(found+1);
}
TXT	KFileSystem::FileName(TXT path_string)
{
	size_t found = path_string.find_last_of("/\\");
	TXT tempf;
	if(found != TXT::npos){
		tempf = path_string.substr(found+1);
	}else{
		tempf = path_string;
	}
	size_t found2 = tempf.find_last_of(".");
	if(found2 == TXT::npos) return tempf;
	return tempf.substr(0,found2);
}
TXT	KFileSystem::FileDot(TXT path_string)
{
	size_t found = path_string.find_last_of("/\\");
	TXT tempf;
	if(found != TXT::npos){
		tempf = path_string.substr(found+1);
	}else{
		tempf = path_string;
	}
	size_t found2 = tempf.find_last_of(".");
	if(found2 == tempf.npos) return "";
	return ".";
}
TXT	KFileSystem::FileExt(TXT path_string)
{
	size_t found = path_string.find_last_of("/\\");
	TXT tempf;
	if(found != TXT::npos){
		tempf = path_string.substr(found+1);
	}else{
		tempf = path_string;
	}
	size_t found2 = tempf.find_last_of(".");
	if(found2 == tempf.npos) return TXT("");
	return tempf.substr(found2+1);
}
TXT KFileSystem::ProgramDirectory()
{
	C16 programcpath[MAX_PATH+1]; programcpath[MAX_PATH] = L'\0';
	//Windows XP:  The TXT is truncated to nSize characters and is not null-terminated.
	GetModuleFileName(NULL, programcpath, MAX_PATH);
	T16 theProgramPath = programcpath;
	size_t found = theProgramPath.find_last_of(L"/\\");
	return String.FromWide(theProgramPath.substr(0,found+1));
}
TXT KFileSystem::ProgramFile()
{
	C16 programcpath[MAX_PATH+1]; programcpath[MAX_PATH] = L'\0';
	//Windows XP:  The TXT is truncated to nSize characters and is not null-terminated.
	GetModuleFileName(NULL, programcpath, MAX_PATH);
	return String.FromWide(programcpath);
}
BIT KFileSystem::CleanFolderNameText(TXT& text)
{
	TXT& x = text;
	BIT input_error = false;
	size_t found = 0;
	while((found = x.find_first_not_of(
	" !#$%&'()+,-;=@[]^_`{}~0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ")) != TXT::npos){
		x.erase(x.begin()+found);
		found = 0;
		input_error = true;
	}
	while(x.size() && (x[0] == ' ')){
        x.erase(x.begin()+0);
		input_error = true;
	}
	TXT u = String.ToUpper(x);
	if((u.find("CON" ,0) == 0) || (u.find("PRN" ,0) == 0) || (u.find("AUX" ,0) == 0) || (u.find("NUL" ,0) == 0)
    || (u.find("COM1",0) == 0) || (u.find("COM2",0) == 0) || (u.find("COM3",0) == 0) || (u.find("COM4",0) == 0)
    || (u.find("COM5",0) == 0) || (u.find("COM6",0) == 0) || (u.find("COM7",0) == 0) || (u.find("COM8",0) == 0)
    || (u.find("COM9",0) == 0) || (u.find("COM0",0) == 0) || (u.find("LPT1",0) == 0) || (u.find("LPT2",0) == 0)
    || (u.find("LPT3",0) == 0) || (u.find("LPT4",0) == 0) || (u.find("LPT5",0) == 0) || (u.find("LPT6",0) == 0)
    || (u.find("LPT7",0) == 0) || (u.find("LPT8",0) == 0) || (u.find("LPT9",0) == 0) || (u.find("LPT0",0) == 0)){
        x = "INVALID";
		input_error = true;
	}
	return input_error;
}
BIT KFileSystem::CleanFilenameText(TXT& text, BIT extensive_validation)
{
	BIT input_error = false;
	TXT x = FileName(text);
	TXT dot = FileDot(text);
	TXT ext = FileExt(text);
	
	input_error |= CleanFileExtension(ext);
	
	size_t found = 0;
	while((found = x.find_first_not_of(
	" !#$%&'()+,-;=@[]^_`{}~0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ")) != TXT::npos){
		x.erase(x.begin()+found);
		found = 0;
		input_error = true;
	}
	while(x.size() && (x[0] == ' ')){
        x.erase(x.begin()+0);
		input_error = true;
	}
	TXT u = String.ToUpper(x);
	if((u.find("CON" ,0) == 0) || (u.find("PRN" ,0) == 0) || (u.find("AUX" ,0) == 0) || (u.find("NUL" ,0) == 0)
    || (u.find("COM1",0) == 0) || (u.find("COM2",0) == 0) || (u.find("COM3",0) == 0) || (u.find("COM4",0) == 0)
    || (u.find("COM5",0) == 0) || (u.find("COM6",0) == 0) || (u.find("COM7",0) == 0) || (u.find("COM8",0) == 0)
    || (u.find("COM9",0) == 0) || (u.find("COM0",0) == 0) || (u.find("LPT1",0) == 0) || (u.find("LPT2",0) == 0)
    || (u.find("LPT3",0) == 0) || (u.find("LPT4",0) == 0) || (u.find("LPT5",0) == 0) || (u.find("LPT6",0) == 0)
    || (u.find("LPT7",0) == 0) || (u.find("LPT8",0) == 0) || (u.find("LPT9",0) == 0) || (u.find("LPT0",0) == 0)){
        x = "INVALID";
		input_error = true;
	}
	text = x + dot + ext;
	while(dot.size() && text.size() && (text[text.size()-1] == ' ')){
        text.erase(text.begin()+0);
		input_error = true;
	}
	return input_error;
}
BIT KFileSystem::CleanFileExtension(TXT& x)
{
	x = String.ToLower(x);
	BIT input_error = false;
	size_t found = 0;
	while((found = x.find_first_not_of(
	"!#$%&'()+,-;=@[]^_`{}~0123456789abcdefghijklmnopqrstuvwxyz")) != TXT::npos){
		x.erase(x.begin()+found);
		found = 0;
		input_error = true;
	}
	return input_error;
}
TXT KFileSystem::ConvertToFullPath(TXT path)
{
	///THIS FUNCTION FOLLOWS PATHBOX RULES.
	size_t colonslash = path.find(":\\");
	if(colonslash != TXT::npos)
	{
		path[colonslash+1] = '\\';
		if(colonslash == 0) return ""; //nope, can't use this TXT.
		if(!(unsigned(path[colonslash-1] - 'A') <= ('Z' - 'A'))
		&& !(unsigned(path[colonslash-1] - 'a') <= ('z' - 'a'))) return ""; //no drive letter
		
		//Remove everything that's junk before drive letter
		path.erase(path.begin(), path.begin()+(colonslash-1));
	}
	else
	{
		//no drive letter, so it's a relative path from the current folder
		//note: checks are preformed at same time due to TXT modification after the checks
		BIT is_from_exedir = ((path.size() >= 2) && (path[0] == '.') && (path[1] == '\\'));
		BIT is_upfolder = ((path.size() >= 3) && (path[0] == '.') && (path[1] == '.') && (path[2] == '\\'));
		if(is_upfolder)
		{
			path = (UpDirectory(ProgramDirectory()) + path); //vPath should always end in a backslash
		}
		else if(is_from_exedir)
		{
			path.erase(path.begin(), path.begin()+2);
			path = (ProgramDirectory() + path);
		}
	}
	return path;
}
//####################################################################################
//####################################################################################
//####################################################################################
//####################################################################################
BIT KFileSystem::CreateFolder(TXT folder)
{
	/// ERROR_ALREADY_EXISTS means that the lpPathName is in use.
	///So don't expect that you can use the lpPathName as a directory. It can be also an ordinary file!
	if((folder.size() >= 2) && (folder[1] != ':')){
		folder = ProgramDirectory() + folder;
	}
	return !CreateDirectoryW(String.ToWideC(ParsePathReparsePoints(folder)),NULL);
}
BIT KFileSystem::CreateDirectory(TXT folder)
{
	if((folder.size() >= 2) && (folder[1] != ':')){
		folder = ProgramDirectory() + folder;
	}
	Vx<TXT> folders;
	TXT processing = folder;
	//First remove the leading slash.
	size_t found = processing.find_last_of("/\\");
	if(found == (processing.size()-1)) processing = processing.substr(0,found);
	//Now, process each folder name
	while(processing.size()){
        size_t found2 = processing.find_last_of("/\\");
        if(found2 == TXT::npos){
            folders.push_back(processing);
            processing = "";
        }else{
            folders.push_back(processing.substr(found2+1));
            processing = processing.substr(0, found2);
        }
	}
	TXT current = "";
	BIT status = false;
	for(IND i = folders.size()-1; i >= 0; i--){//loop must run in reverse here
        current += folders[i];
        current += "\\";
        status |= !CreateDirectoryW(String.ToWideC(ParsePathReparsePoints(current)),NULL);
	}
	return status;
}
BIT KFileSystem::DeleteFile(TXT file)
{
	if((file.size() >= 2) && (file[1] != ':')){
		file = ProgramDirectory() + file;
	}
	return DeleteFileW(String.ToWideC(ParsePathReparsePoints(file)));
}
static BOOL IsDots(const C16* str)
{
    if(lstrcmp(str,L".") && lstrcmp(str,L"..")) return FALSE;
    return TRUE;
}
BIT KFileSystem::DeleteDirectory(TXT folder)
{
    if((folder.size() >= 2) && (folder[1] != ':')){
		folder = ProgramDirectory() + folder;
	}
	T16 wfolder = String.ToWide(folder);
    HANDLE hFind;  // file handle

    WIN32_FIND_DATA FindFileData;

    C16 DirPath[MAX_PATH];
    C16 FileName[MAX_PATH];

    lstrcpy(DirPath,wfolder.c_str());
    lstrcat(DirPath,L"\\*");    // searching all files

    lstrcpy(FileName,wfolder.c_str());
    lstrcat(FileName,L"\\");

    hFind = FindFirstFile(DirPath,&FindFileData); // find the first file
    if(hFind == INVALID_HANDLE_VALUE) return FALSE;
    lstrcpy(DirPath,FileName);
        
    BIT bSearch = true;
    while(bSearch) { // until we finds an entry
        if(FindNextFile(hFind,&FindFileData)) {
            if(IsDots(FindFileData.cFileName)) continue;
            lstrcat(FileName,FindFileData.cFileName);
            if((FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)) {

                // we have found a directory, recurse
                if(!DeleteDirectory(String.FromWide(FileName))) { 
                    FindClose(hFind); 
                    return FALSE; // directory couldn't be deleted
                }
                RemoveDirectoryW(FileName); // remove the empty directory
                lstrcpy(FileName,DirPath);
            }
            else {
                if(FindFileData.dwFileAttributes & FILE_ATTRIBUTE_READONLY)
                    SetFileAttributes(FileName,FILE_ATTRIBUTE_NORMAL); // change read-only file mode
                if(!DeleteFileW(FileName)) {  // delete the file
                    FindClose(hFind);
                    return FALSE; 
                }                 
                lstrcpy(FileName,DirPath);
            }
        }
        else {
            if(GetLastError() == ERROR_NO_MORE_FILES){// no more files there
            bSearch = false;
            }else {
                // some error occured, close the handle and return FALSE
                FindClose(hFind); 
                return FALSE;
            }

        }

    }
    FindClose(hFind);  // closing file handle
    return RemoveDirectoryW(wfolder.c_str()); // remove the empty directory
}
//IMPORTANT: The one caveat is that the MoveFile function will fail
// on directory moves when the destination is on a different volume.
//CopyFile will not create the directory if it doesn't exist. 
// if it doesn't exist, the error code will indicate: ERROR_FILE_NOT_FOUND.
BIT KFileSystem::RenameFile(TXT directory, TXT old_filename, TXT new_filename)
{
	if(directory.find_last_of("\\/") != (directory.size()-1)) directory += "\\";
	TXT old_path = (directory + old_filename);
	TXT new_path = (directory + new_filename);
	if(!MoveFileExW(String.ToWideC(UncDoPrefix(old_path)), String.ToWideC(UncDoPrefix(new_path)), 0)) return true;
	return false;
}
BIT KFileSystem::RenameFolder(TXT folder_path, TXT new_foldername, TXT* new_name_result)
{
	if(folder_path.find_last_of("\\/") == (folder_path.size()-1)) folder_path.erase(folder_path.size()-1);
	TXT SurObjfolder = Directory(folder_path);//because Directory() expects slash at end of folder paths
	TXT new_path = (SurObjfolder + new_foldername);
	if(new_name_result){(*new_name_result) = new_path;}
	if(!MoveFileExW(String.ToWideC(UncDoPrefix(folder_path)), String.ToWideC(UncDoPrefix(new_path)), 0)) return true;
	return false;
}
BIT KFileSystem::MoveFile(TXT old_path, TXT new_path)
{
	//Version for moving a single file; eliminates the need to pass in an error list.
	Vx<TXT> error_list;
	return MoveFile(old_path, new_path, error_list);
}
BIT KFileSystem::MoveFile(TXT old_path, TXT new_path, Vx<TXT>& error_list)
{
	if(!MoveFileExW(String.ToWideC(UncDoPrefix(old_path)), String.ToWideC(UncDoPrefix(new_path)), MOVEFILE_COPY_ALLOWED | MOVEFILE_WRITE_THROUGH)){//Allow HDD<--->HDD moves.
		error_list.push_back(old_path);
		return true;
	}
	return false;
}
BIT KFileSystem::MoveFileClipboard(TXT old_path, TXT new_directory, Vx<TXT>& error_list)
{
	//Version for clipboard routines, which accept full file path and the target directory.
	if(new_directory.find_last_of("\\/") != (new_directory.size()-1)) new_directory += "\\";
	TXT new_path = (new_directory + File(old_path));
	return MoveFile(old_path, new_path, error_list);
}
BIT KFileSystem::CopyFile(TXT old_path, TXT new_path)
{
	//Version for copying a single file; eliminates the need to pass in an error list.
	Vx<TXT> error_list;
	return CopyFile(old_path, new_path, error_list);
}
BIT KFileSystem::CopyFile(TXT old_path, TXT new_path, Vx<TXT>& error_list)
{
	if(!CopyFileW(String.ToWideC(UncDoPrefix(old_path)), String.ToWideC(UncDoPrefix(new_path)), FALSE)){
		error_list.push_back(old_path);
		return true;
	}
	return false;
}
BIT KFileSystem::CopyFileClipboard(TXT old_path, TXT new_directory, Vx<TXT>& error_list)
{
	//Version for clipboard routines, which accept full file path and the target directory.
	if(new_directory.find_last_of("\\/") != (new_directory.size()-1)) new_directory += "\\";
	TXT new_path = (new_directory + File(old_path));
	return CopyFile(old_path, new_path, error_list);
}
BIT KFileSystem::MoveFolder_(TXT old_path, TXT new_path, Vx<TXT>& error_list, BIT toplevel)
{
	//REMINDER: MoveFile function will fail on HDD<--->HDD directory moves.
	//Therefore, we need to:
	// (1) manually create the "moved" directory structures
	// (2) move just the files themselves
	// (3) delete the moved-from directory structures, BUT only if they are empty
	// (3b) if a move failure occurs the ENTIRE structure must remain. DO NOT remove any empty folders
	if(old_path[old_path.size()-1] != '\\') old_path += "\\";
	if(new_path[new_path.size()-1] != '\\') new_path += "\\";
	HANDLE hFind;
	WIN32_FIND_DATA FindData;
	CreateDirectory(new_path);
	if((hFind = FindFirstFile(String.ToWideC(old_path+"*"), &FindData)) != INVALID_HANDLE_VALUE){
		do{ 
			TXT fname = String.FromWide(FindData.cFileName);
			if(FindData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY){
				if((fname != ".") && (fname != "..")){
					if(~FindData.dwFileAttributes & FILE_ATTRIBUTE_REPARSE_POINT){
						MoveFolder_(old_path+fname+"\\", new_path+fname+"\\", error_list, false);
					}
				}
			}else{
				MoveFile(old_path+fname, new_path+fname, error_list);
			}
		}while(FindNextFile(hFind, &FindData));
		FindClose(hFind);
	}
	if(error_list.size()){
		return true;
	}else{
		if(toplevel){
			//No errors, it's OK to delete the old folder structure!
			old_path.erase(old_path.size()-1);//need a non-slashed folder name?
			DeleteDirectory(old_path);
		}
	}
	return false;
}
BIT KFileSystem::MoveFolder(TXT old_path, TXT new_path, Vx<TXT>& error_list)
{
	return MoveFolder_(UncDoPrefix(old_path), UncDoPrefix(new_path), error_list, true);
}
BIT KFileSystem::MoveFolderClipboard(TXT old_path, TXT new_directory, Vx<TXT>& error_list)
{
	if(old_path.find_last_of("\\/") != (old_path.size()-1)) old_path += "\\";
	if(new_directory.find_last_of("\\/") != (new_directory.size()-1)) new_directory += "\\";
	TXT new_path = (new_directory + Folder(old_path) + "\\");
	return MoveFolder_(UncDoPrefix(old_path), UncDoPrefix(new_path), error_list, true);
}
BIT KFileSystem::CopyFolder_(TXT old_path, TXT new_path, Vx<TXT>& error_list, BIT toplevel)
{
	//REMINDER: CopyFile will not create the directory if it doesn't exist. 
	//if it doesn't exist, the error code will indicate: ERROR_FILE_NOT_FOUND.
	if(old_path[old_path.size()-1] != '\\') old_path += "\\";
	if(new_path[new_path.size()-1] != '\\') new_path += "\\";
	HANDLE hFind;
	WIN32_FIND_DATA FindData;
	CreateDirectory(new_path);
	if((hFind = FindFirstFile(String.ToWideC(old_path+"*"), &FindData)) != INVALID_HANDLE_VALUE){
		do{ 
			TXT fname = String.FromWide(FindData.cFileName);
			if(FindData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY){
				if((fname != ".") && (fname != "..")){
					if(~FindData.dwFileAttributes & FILE_ATTRIBUTE_REPARSE_POINT){
						CopyFolder_(old_path+fname+"\\", new_path+fname+"\\", error_list, false);
					}
				}
			}else{
				CopyFile(old_path+fname, new_path+fname, error_list);
			}
		}while(FindNextFile(hFind, &FindData));
		FindClose(hFind);
	}
	if(error_list.size()){
		return true;
	}
	return false;
}
BIT KFileSystem::CopyFolder(TXT old_path, TXT new_path, Vx<TXT>& error_list)
{
	return CopyFolder_(UncDoPrefix(old_path), UncDoPrefix(new_path), error_list, true);
}
BIT KFileSystem::CopyFolderClipboard(TXT old_path, TXT new_directory, Vx<TXT>& error_list)
{
	if(old_path.find_last_of("\\/") != (old_path.size()-1)) old_path += "\\";
	if(new_directory.find_last_of("\\/") != (new_directory.size()-1)) new_directory += "\\";
	TXT new_path = (new_directory + Folder(old_path) + "\\");
	return CopyFolder_(UncDoPrefix(old_path), UncDoPrefix(new_path), error_list, true);
}
BIT KFileSystem::FileExists(TXT path)
{
	if((path.size() >= 2) && (path[1] != ':')){
		path = ProgramDirectory() + path;
	}
	U32 attr = GetFileAttributes(String.ToWideC(UncDoPrefix(ParsePathReparsePoints(path))));
	return ((attr != INVALID_FILE_ATTRIBUTES) && (~attr & FILE_ATTRIBUTE_DIRECTORY));
}
BIT KFileSystem::FolderExists(TXT path)
{
	if((path.size() >= 2) && (path[1] != ':')){
		path = ProgramDirectory() + path;
	}
	U32 attr = GetFileAttributes(String.ToWideC(UncDoPrefix(ParsePathReparsePoints(path))));
	return ((attr != INVALID_FILE_ATTRIBUTES) && (attr & FILE_ATTRIBUTE_DIRECTORY));
}
U64 KFileSystem::FileDate(TXT path)
{
	FILETIME ftime; HANDLE hFile = CreateFile(String.ToWideC(path), 0, 
		FILE_SHARE_DELETE | FILE_SHARE_WRITE | FILE_SHARE_READ, 
		NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
		GetFileTime(hFile, NULL, NULL, &ftime);
	return ((((U64)ftime.dwHighDateTime) << 32LL)|((U64)ftime.dwLowDateTime));
}
S32 KFileSystem::CompareFileDates(TXT path1, TXT path2)
{
	HANDLE hFile1, hFile2;
	FILETIME ftime1, ftime2;
	hFile1 = CreateFile(String.ToWideC(path1), 0, 
		FILE_SHARE_DELETE | FILE_SHARE_WRITE | FILE_SHARE_READ, 
		NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
		GetFileTime(hFile1, NULL, NULL, &ftime1);
	hFile2 = CreateFile(String.ToWideC(path2), 0, 
		FILE_SHARE_DELETE | FILE_SHARE_WRITE | FILE_SHARE_READ, 
		NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
		GetFileTime(hFile2, NULL, NULL, &ftime2);
	return CompareFileTime(&ftime1, &ftime2);
}
void KFileSystem::FolderGetSubDirectoriesList(TXT path, Vx<TXT>& list, BIT add_dir_root)
{
	if((path.size() >= 2) && (path[1] != ':')){
		path = ProgramDirectory() + path;
	}
	if(path[path.size()-1] != '\\') path += "\\";
	if(add_dir_root) list.push_back(path);
	HANDLE hFind;
	WIN32_FIND_DATA FindData;
	if((hFind = FindFirstFile(String.ToWideC(path+"*"), &FindData)) != INVALID_HANDLE_VALUE){
		do{ 
			TXT fname = String.FromWide(FindData.cFileName);
			if(FindData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY){
				if((fname != ".") && (fname != "..")){
					if(~FindData.dwFileAttributes & FILE_ATTRIBUTE_REPARSE_POINT){
						FolderGetSubDirectoriesList(path+fname+"\\", list, true);
					}
				}
			}
		}while(FindNextFile(hFind, &FindData));
		FindClose(hFind);
	}
}
void KFileSystem::FolderGetSubFilesList(TXT path, Vx<TXT>& list)
{
	if((path.size() >= 2) && (path[1] != ':')){
		path = ProgramDirectory() + path;
	}
	if(path[path.size()-1] != '\\') path += "\\";
	HANDLE hFind;
	WIN32_FIND_DATA FindData;
	if((hFind = FindFirstFile(String.ToWideC(path+"*"), &FindData)) != INVALID_HANDLE_VALUE){
		do{ 
			TXT fname = String.FromWide(FindData.cFileName);
			if(FindData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY){
				if((fname != ".") && (fname != "..")){
					if(~FindData.dwFileAttributes & FILE_ATTRIBUTE_REPARSE_POINT){
						FolderGetSubFilesList(path+fname+"\\", list);
					}
				}
			}else{
				list.push_back(path+fname);
			}
		}while(FindNextFile(hFind, &FindData));
		FindClose(hFind);
	}
}
void KFileSystem::FolderGetSubDirectoriesAndFilesLists(
TXT path, Vx<TXT>& files_list, Vx<TXT>& folders_list, BIT add_dir_root)
{
	if((path.size() >= 2) && (path[1] != ':')){
		path = ProgramDirectory() + path;
	}
	if(path[path.size()-1] != '\\') path += "\\";
	if(add_dir_root) folders_list.push_back(path);
	HANDLE hFind;
	WIN32_FIND_DATA FindData;
	if((hFind = FindFirstFile(String.ToWideC(path+"*"), &FindData)) != INVALID_HANDLE_VALUE){
		do{ 
			TXT fname = String.FromWide(FindData.cFileName);
			if(FindData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY){
				if((fname != ".") && (fname != "..")){
					if(~FindData.dwFileAttributes & FILE_ATTRIBUTE_REPARSE_POINT){
						FolderGetSubDirectoriesAndFilesLists(path+fname+"\\", files_list, folders_list, true);
					}
				}
			}else{
				files_list.push_back(path+fname);
			}
		}while(FindNextFile(hFind, &FindData));
		FindClose(hFind);
	}
}
void KFileSystem::FolderGetFileNamesList(TXT path, Vx<TXT>& names_list)
{
	if((path.size() >= 2) && (path[1] != ':')){
		path = ProgramDirectory() + path;
	}
	if(path[path.size()-1] != '\\') path += "\\";
	HANDLE hFind;
	WIN32_FIND_DATA FindData;
	if((hFind = FindFirstFile(String.ToWideC(path+"*"), &FindData)) != INVALID_HANDLE_VALUE){
		do{ 
			TXT fname = String.FromWide(FindData.cFileName);
			if(~FindData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY){
				names_list.push_back(fname);
			}
		}while(FindNextFile(hFind, &FindData));
		FindClose(hFind);
	}
}
void KFileSystem::FolderGetFolderNamesList(TXT path, Vx<TXT>& names_list)
{
	if((path.size() >= 2) && (path[1] != ':')){
		path = ProgramDirectory() + path;
	}
	if(path[path.size()-1] != '\\') path += "\\";
	HANDLE hFind;
	WIN32_FIND_DATA FindData;
	if((hFind = FindFirstFile(String.ToWideC(path+"*"), &FindData)) != INVALID_HANDLE_VALUE){
		do{ 
			TXT fname = String.FromWide(FindData.cFileName);
			if(FindData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY){
				if((fname != ".") && (fname != "..")){
					if(~FindData.dwFileAttributes & FILE_ATTRIBUTE_REPARSE_POINT){
						names_list.push_back(fname);
					}
				}
			}
		}while(FindNextFile(hFind, &FindData));
		FindClose(hFind);
	}
}
void KFileSystem::FolderGetObjectNamesList(TXT path, Vx<TXT>& names_list)
{
	if((path.size() >= 2) && (path[1] != ':')){
		path = ProgramDirectory() + path;
	}
	if(path[path.size()-1] != '\\') path += "\\";
	HANDLE hFind;
	WIN32_FIND_DATA FindData;
	if((hFind = FindFirstFile(String.ToWideC(path+"*"), &FindData)) != INVALID_HANDLE_VALUE){
		do{ 
			TXT fname = String.FromWide(FindData.cFileName);
			if(FindData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY){
				if((fname != ".") && (fname != "..")){
					if(~FindData.dwFileAttributes & FILE_ATTRIBUTE_REPARSE_POINT){
						names_list.push_back(fname);
					}
				}
			}else{
				names_list.push_back(fname);
			}
		}while(FindNextFile(hFind, &FindData));
		FindClose(hFind);
	}
}
void KFileSystem::FolderGetSubDirectoriesList(
TXT path, TXT_callback_t cb, BIT add_dir_root)
{
    if((path.size() >= 2) && (path[1] != ':')){
		path = ProgramDirectory() + path;
	}
	if(path[path.size()-1] != '\\') path += "\\";
	if(add_dir_root){cb(path);}
	HANDLE hFind;
	WIN32_FIND_DATA FindData;
	if((hFind = FindFirstFile(String.ToWideC(path+"*"), &FindData)) != INVALID_HANDLE_VALUE){
		do{ 
			TXT fname = String.FromWide(FindData.cFileName);
			if(FindData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY){
				if((fname != ".") && (fname != "..")){
					if(~FindData.dwFileAttributes & FILE_ATTRIBUTE_REPARSE_POINT){
						FolderGetSubDirectoriesList(path+fname+"\\", cb, true);
					}
				}
			}
		}while(FindNextFile(hFind, &FindData));
		FindClose(hFind);
	}
}
void KFileSystem::FolderGetSubFilesList(TXT path, TXT_callback_t cb)
{
    if((path.size() >= 2) && (path[1] != ':')){
		path = ProgramDirectory() + path;
	}
	if(path[path.size()-1] != '\\') path += "\\";
	HANDLE hFind;
	WIN32_FIND_DATA FindData;
	if((hFind = FindFirstFile(String.ToWideC(path+"*"), &FindData)) != INVALID_HANDLE_VALUE){
		do{ 
			TXT fname = String.FromWide(FindData.cFileName);
			if(FindData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY){
				if((fname != ".") && (fname != "..")){
					if(~FindData.dwFileAttributes & FILE_ATTRIBUTE_REPARSE_POINT){
						FolderGetSubFilesList(path+fname+"\\", cb);
					}
				}
			}else{
			    cb(path+fname);
			}
		}while(FindNextFile(hFind, &FindData));
		FindClose(hFind);
	}
}
void KFileSystem::FolderGetSubDirectoriesAndFilesLists(
TXT path, TXT_callback_t cb_files, TXT_callback_t cb_folders, BIT add_dir_root)
{
    if((path.size() >= 2) && (path[1] != ':')){
		path = ProgramDirectory() + path;
	}
	if(path[path.size()-1] != '\\') path += "\\";
	if(add_dir_root){(*cb_folders)(path);}
	HANDLE hFind;
	WIN32_FIND_DATA FindData;
	if((hFind = FindFirstFile(String.ToWideC(path+"*"), &FindData)) != INVALID_HANDLE_VALUE){
		do{ 
			TXT fname = String.FromWide(FindData.cFileName);
			if(FindData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY){
				if((fname != ".") && (fname != "..")){
					if(~FindData.dwFileAttributes & FILE_ATTRIBUTE_REPARSE_POINT){
						FolderGetSubDirectoriesAndFilesLists(path+fname+"\\", cb_files, cb_folders, true);
					}
				}
			}else{
			    (*cb_files)(path+fname);
			}
		}while(FindNextFile(hFind, &FindData));
		FindClose(hFind);
	}
}
void KFileSystem::FolderGetFileNamesList(TXT path, TXT_callback_t cb)
{
    if((path.size() >= 2) && (path[1] != ':')){
		path = ProgramDirectory() + path;
	}
	if(path[path.size()-1] != '\\') path += "\\";
	HANDLE hFind;
	WIN32_FIND_DATA FindData;
	if((hFind = FindFirstFile(String.ToWideC(path+"*"), &FindData)) != INVALID_HANDLE_VALUE){
		do{ 
			TXT fname = String.FromWide(FindData.cFileName);
			if(~FindData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY){
			    cb(fname);
			}
		}while(FindNextFile(hFind, &FindData));
		FindClose(hFind);
	}
}
void KFileSystem::FolderGetFolderNamesList(TXT path, TXT_callback_t cb)
{
    if((path.size() >= 2) && (path[1] != ':')){
		path = ProgramDirectory() + path;
	}
	if(path[path.size()-1] != '\\') path += "\\";
	HANDLE hFind;
	WIN32_FIND_DATA FindData;
	if((hFind = FindFirstFile(String.ToWideC(path+"*"), &FindData)) != INVALID_HANDLE_VALUE){
		do{ 
			TXT fname = String.FromWide(FindData.cFileName);
			if(FindData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY){
				if((fname != ".") && (fname != "..")){
					if(~FindData.dwFileAttributes & FILE_ATTRIBUTE_REPARSE_POINT){
                        cb(fname);
					}
				}
			}
		}while(FindNextFile(hFind, &FindData));
		FindClose(hFind);
	}
}
void KFileSystem::FolderGetObjectNamesList(TXT path, TXT_callback_t cb)
{
	if((path.size() >= 2) && (path[1] != ':')){
		path = ProgramDirectory() + path;
	}
	if(path[path.size()-1] != '\\') path += "\\";
	HANDLE hFind;
	WIN32_FIND_DATA FindData;
	if((hFind = FindFirstFile(String.ToWideC(path+"*"), &FindData)) != INVALID_HANDLE_VALUE){
		do{ 
			TXT fname = String.FromWide(FindData.cFileName);
			if(FindData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY){
				if((fname != ".") && (fname != "..")){
					if(~FindData.dwFileAttributes & FILE_ATTRIBUTE_REPARSE_POINT){
						cb(fname);
					}
				}
			}else{
			    cb(fname);
			}
		}while(FindNextFile(hFind, &FindData));
		FindClose(hFind);
	}
}
//####################################################################################
//####################################################################################
//####################################################################################
//####################################################################################
TXT KFileSystem::GetDriveLabel(TXT drive)
{
	C16 label[MAX_PATH];
	T16 wdrive = String.ToWide(drive);
	DWORD dwDontNeedThis;
	if(!GetVolumeInformation(wdrive.c_str(), label, sizeof(label), NULL, &dwDontNeedThis, &dwDontNeedThis, NULL, 0)){
		return String.FromWide(T16(L"Local Disk"));
	}else{
		if(T16(label) == L"") return String.FromWide(T16(L"Local Disk"));
		return String.FromWide(T16(label));
	}
}
