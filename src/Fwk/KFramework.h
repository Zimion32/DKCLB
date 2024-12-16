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
#include "Types\dtime.h"
class CFileType;
struct SFileInfo
{
	TXT folderpath, file;
	Vx< BITMAP* >* icons;
	U64 filesize;
	U32 attributes;
	dtime created;
	dtime modified;
	CFileType*  typeinfo;
    SFileInfo();
	SFileInfo(TXT folderpath, TXT file, U64 filesize, U32 attributes, dtime& created, dtime& modified);
};
class KDeltaError
{
    public:
                KDeltaError();
                ~KDeltaError();
        void    ToggleBoxes();
        void    Configure(BIT log_errors, BIT box_errors);
        TXT		GetLastError();
        Vx<TXT> GetErrorList();
        void	AddError(TXT error_string);
        
    private:
        Vx<TXT> error_list_;
        INT     log_errors_;
        INT     box_errors_;
};
class KDebug
{
    public:
        void    Abort();
        void    Abort(TXT information);
        void    AbortBox(TXT information);
        void    Log(TXT text);
        void    Box();
        void    Box(TXT text);
        void    Box(TXT text, TXT title);
        void    Box(TXT text1, TXT text2, TXT title);
        void    Box(INT value);
        void    Box(INT value1, INT value2);
        void    Box(INT value1, TXT title);
        void    Box(INT value1, INT value2, TXT title);
        void    Box(INT value1, INT value2, INT value3, TXT title);
        void    Box(INT value1, INT value2, INT value3, INT value4, TXT title);
        TXT		GetLastErrorAsString();
        void    Error(INT what);
static  void    OnSigAbrt(int signal);
static  void    CrashDump(PCONTEXT context);
                KDebug();
                ~KDebug();
};
class KClipboard
{
    public:
        void    SetBinary(Vx<U08>& data, U32 id);
        Vx<U08> GetBinary(U32 id);
        void    SetText(TXT clipText);
        TXT     GetText();
        BIT     GetOperation();
        void    SetOperation(BIT is_cutting);
        BIT     HasFiles();
        void    GetFilesList(Vx<TXT>& path_list);
        void    CopyFiles(Vx<TXT>& path_list);
        void    CutFiles(Vx<TXT>& path_list);
        void    PasteFiles(TXT where, Vx<TXT>& error_list);
        
        BIT     SetBitmapData(BMP bmp);
        
    private:
        void    CutCopyFilesList_(Vx<TXT>& path_list, BIT is_cutting);
};
class KSystem
{
    public:
static  HWND    MainHWND;
static HINSTANCE MainHINST;
static  HANDLE  MainHTHREAD;
static  DWORD   MainTHREADID;
static  HANDLE  MainHFIBER;
static  INT		MainCursorIndex;
        
        void    RequestClose();
        BIT     HandleCloseRequest();
        void    PostCloseMessage();
        U64     GetPerformanceFrequency();
        U64     GetPerformanceCounter();
        U32     GetStackAvailable();
        U64     GetMACAddress();
        Vx<U64> GetMACAddressList();
        B16     GetSystemIdentityLUCID();
        BIT     OneInstanceOnly();
        INT     GetTaskBarHeight();
        void    LoadCursors();
        void    CheckAndResetCursor(INT c);
        void    ChangeCursor(INT c);
        void    FixFullScreenCursor();
        void    PlayRegistrySound(TXT app, TXT event);
        
    private:
static  U32     close_was_requested_;
static  HCURSOR cursors_[16];
static  INT     cursor_reset_delay_;
};
typedef void (*TXT_callback_t)(TXT item);
class KFileSystem
{
    public:
        TXT		ParsePathReparsePoints(TXT path);
        TXT		UncDoPrefix(TXT path);
        T16		UncDoPrefix(T16 path);
        TXT		UncUnPrefix(TXT path);
        T16		UncUnPrefix(T16 path);
        
        TXT		Directory(TXT path_string);
        TXT		UpDirectory(TXT path_string);
        TXT		Folder(TXT path_string);
        TXT		File(TXT path_string);
        TXT		FileName(TXT path_string);
        TXT		FileDot(TXT path_string);
        TXT		FileExt(TXT path_string);
        void 	Run(TXT command, BIT wait);
        void 	RunFile(TXT path, BIT wait);
        void 	RunFolder(TXT path, BIT wait);
        TXT 	ProgramDirectory();
        TXT 	ProgramFile();
        BIT     CleanFolderPathText(TXT& text);
        BIT     CleanFolderNameText(TXT& text);
        BIT 	CleanFilenameText(TXT& text, BIT extensive_validation);
        BIT 	CleanFileExtension(TXT& x);
        TXT 	ConvertToFullPath(TXT path);
        BIT 	CreateFolder(TXT folder);
        BIT 	CreateDirectory(TXT folder);
        BIT 	DeleteFile(TXT file);
        BIT 	DeleteDirectory(TXT folder);
        BIT 	RenameFile(TXT directory, TXT old_filename, TXT new_filename);
        BIT 	RenameFolder(TXT folder_path, TXT new_foldername, TXT* new_name_result);
        BIT 	MoveFile(   TXT old_path, TXT new_path);
        BIT 	MoveFile(   TXT old_path, TXT new_path,       Vx<TXT>& error_list);
        BIT 	MoveFileClipboard(  TXT old_path, TXT new_directory,  Vx<TXT>& error_list);
        BIT 	CopyFile(   TXT old_path, TXT new_path);
        BIT 	CopyFile(   TXT old_path, TXT new_path,       Vx<TXT>& error_list);
        BIT 	CopyFileClipboard(  TXT old_path, TXT new_directory,  Vx<TXT>& error_list);
        BIT 	MoveFolder( TXT old_path, TXT new_path,       Vx<TXT>& error_list);
        BIT 	MoveFolderClipboard(TXT old_path, TXT new_directory,  Vx<TXT>& error_list);
        BIT 	CopyFolder( TXT old_path, TXT new_path,       Vx<TXT>& error_list);
        BIT 	CopyFolderClipboard(TXT old_path, TXT new_directory,  Vx<TXT>& error_list);
        BIT 	FileExists(TXT path);
        BIT 	FolderExists(TXT path);
        U64 	FileDate(TXT path);
        S32 	CompareFileDates(TXT path1, TXT path2);
        void 	FolderGetSubDirectoriesList(TXT path, Vx<TXT>& list, BIT add_dir_root);
        void 	FolderGetSubFilesList(TXT path, Vx<TXT>& list);
        void 	FolderGetSubDirectoriesAndFilesLists(TXT path, Vx<TXT>& files_list, Vx<TXT>& folders_list, BIT add_dir_root);
        void 	FolderGetFileNamesList(TXT path, Vx<TXT>& names_list);
        void	FolderGetFolderNamesList(TXT path, Vx<TXT>& names_list);
        void 	FolderGetObjectNamesList(TXT path, Vx<TXT>& names_list);
        void 	FolderGetSubDirectoriesList(TXT path, TXT_callback_t cb, BIT add_dir_root);
        void 	FolderGetSubFilesList(TXT path, TXT_callback_t cb);
        void 	FolderGetSubDirectoriesAndFilesLists(TXT path, TXT_callback_t cb_files, TXT_callback_t cb_folders, BIT add_dir_root);
        void 	FolderGetFileNamesList(TXT path, TXT_callback_t cb);
        void 	FolderGetFolderNamesList(TXT path, TXT_callback_t cb);
        void 	FolderGetObjectNamesList(TXT path, TXT_callback_t cb);
        //============================================
        void    FolderGetFileNamesListEx(TXT path, Vx<SFileInfo>& list);
        void    FolderGetFolderNamesListEx(TXT path, Vx<SFileInfo>& list);
        TXT     GetDriveLabel(TXT drive);
        void    GetFileInfo(TXT filepath, SFileInfo& info);
        
    private:
        BIT 	MoveFolder_(TXT old_path, TXT new_path, Vx<TXT>& error_list, BIT toplevel);
        BIT 	CopyFolder_(TXT old_path, TXT new_path, Vx<TXT>& error_list, BIT toplevel);
};
class KUtils
{
    public:
        void    SerializeVector(Vx< Vx<U08> >& in, Vx<U08>& out);
        void    SerializeVector(Vx< Vx<U16> >& in, Vx<U08>& out);
        void    SerializeVector(Vx< Vx<U32> >& in, Vx<U08>& out);
        void    SerializeVector(Vx< Vx<S08> >& in, Vx<U08>& out);
        void    SerializeVector(Vx< Vx<S16> >& in, Vx<U08>& out);
        void    SerializeVector(Vx< Vx<S32> >& in, Vx<U08>& out);
        void    SerializeVector(Vx< Vx< Vx<U08> > >& in, Vx<U08>& out);
        void    SerializeVector(Vx< Vx< Vx<U16> > >& in, Vx<U08>& out);
        void    SerializeVector(Vx< Vx< Vx<U32> > >& in, Vx<U08>& out);
        void    SerializeVector(Vx< Vx< Vx<S08> > >& in, Vx<U08>& out);
        void    SerializeVector(Vx< Vx< Vx<S16> > >& in, Vx<U08>& out);
        void    SerializeVector(Vx< Vx< Vx<S32> > >& in, Vx<U08>& out);
        void    UnserializeVector(Vx<U08>& in, Vx< Vx<U08> >& out);
        void    UnserializeVector(Vx<U08>& in, Vx< Vx<U16> >& out);
        void    UnserializeVector(Vx<U08>& in, Vx< Vx<U32> >& out);
        void    UnserializeVector(Vx<U08>& in, Vx< Vx<S08> >& out);
        void    UnserializeVector(Vx<U08>& in, Vx< Vx<S16> >& out);
        void    UnserializeVector(Vx<U08>& in, Vx< Vx<S32> >& out);
        void    UnserializeVector(Vx<U08>& in, Vx< Vx< Vx<U08> > >& out);
        void    UnserializeVector(Vx<U08>& in, Vx< Vx< Vx<U16> > >& out);
        void    UnserializeVector(Vx<U08>& in, Vx< Vx< Vx<U32> > >& out);
        void    UnserializeVector(Vx<U08>& in, Vx< Vx< Vx<S08> > >& out);
        void    UnserializeVector(Vx<U08>& in, Vx< Vx< Vx<S16> > >& out);
        void    UnserializeVector(Vx<U08>& in, Vx< Vx< Vx<S32> > >& out);
        U64     SuperRandom();
        TXT     ConvertToUrlBase64(U08* data, U32 size);
        Vx<U08> ConvertFromUrlBase64(TXT str);
};
class KString
{
    public:
static  Vx<T16> w_persist_;
static  INT     w_persist_index_;
        BIT     IsHex(TXT str);
        BIT     IsInt(TXT str);
        BIT     IsFloat(TXT str);
        TXT		FromInt(S32 value, INT digits, BIT zeroes = true);
        TXT		FromHex(U64 value, INT digits);
        U64		ToHex(TXT str);
        S64		ToInt(TXT str);
        F64     ToFloat(TXT str);
        Vx<TXT> Split(const TXT &s, CHR delim);
        Vx<TXT> Split(const TXT &s, TXT delims);
        Vx<TXT> SplitWithAll(const TXT &s, TXT delims);
        Vx<TXT> SplitWithAll_WithEachDelimiter(const TXT &s, TXT delims);
        TXT     RemoveComments(const TXT &s);
        TXT     EscapeStringWhiteSpace(const TXT &s, CHR space, CHR tab);
        T16		ToWide(STR str);
        T16		ToWide(TXT str);
  const C16*	ToWideC(STR str);
  const C16*	ToWideC(TXT str);
        TXT		FromWide(T16 wstr);
        TXT		ToUpper(TXT x);
        TXT		ToLower(TXT x);
        TXT     FileSizeXP(U64 size);
        Vx<TXT> InfixToPostfix(TXT infix);
};
class KRegistry
{
    public:
        void	GetKeyList(HKEY root, TXT path, Vx<TXT>& key_list);
        void	GetKeyListSW(HKEY root, TXT path, Vx<TXT>& key_list, CHR starting_chr);
        void	GetList(HKEY root, TXT path, Vx<TXT>& list);
        INT		CreateTree(HKEY hk, TXT path);
        BIT	    Create(HKEY root, TXT path);
        BIT	    KeyExists(HKEY root, TXT path);
        BIT	    SetHex(HKEY root, TXT path, TXT variable, U32 value);
        BIT	    SetU64(HKEY root, TXT path, TXT variable, U64 value);
        BIT	    SetStr(HKEY root, TXT path, TXT variable, TXT value);
        BIT	    SetBool(HKEY root, TXT path, TXT variable, BIT value);
        BIT		SetData(HKEY root, TXT path, TXT variable, U08* data, U32 bytes);
        BIT		SetData(HKEY root, TXT path, TXT variable, Vx<U08>& data);
        U32	    GetHex(HKEY root, TXT path, TXT variable, U32 def);
        U64	    GetU64(HKEY root, TXT path, TXT variable, U64 def);
        TXT	    GetStr(HKEY root, TXT path, TXT variable, TXT def = "");
        BIT	    GetBool(HKEY root, TXT path, TXT variable, BIT def);
        Vx<U08>	GetData(HKEY root, TXT path, TXT variable);
};
#include "CConfig.h"
#include "CConfigManager.h"
class KConfig
{
    public:
 CConfigManager config_manager_;
        TXT     ini_main_file;
        BIT     ini_last_load_failure;
        BIT	    Open(TXT file); // Call with NULL to close the INI.
        BIT	    OpenSecondary(TXT file, INT secondary_id); // Call with NULL to close a secondary INI.
        void    Exit();
        BIT	    Save(); // Saves non-secondary INI.
        TXT	    SetStr(TXT section, TXT variable, TXT str);
        U64	    SetHex64(TXT section, TXT variable, U64 value, INT index = 0);
        S64	    SetVsx64(TXT section, TXT variable, S64 value, INT index = 0);
        U32	    SetHex(TXT section, TXT variable, U32 value, INT index = 0);
        S32	    SetVsx(TXT section, TXT variable, S32 value, INT index = 0);
        S32	    SetInt(TXT section, TXT variable, S32 value);
        void	SetWP(TXT section, U32 window_id, S16 x, S16 y, S16 w, S16 h, BIT m);
        S16	    SetWPx(TXT section, U32 window_id, S16 x);
        S16	    SetWPy(TXT section, U32 window_id, S16 y);
        S16	    SetWPw(TXT section, U32 window_id, S16 w);
        S16	    SetWPh(TXT section, U32 window_id, S16 h);
        S16	    SetWPm(TXT section, U32 window_id, BIT m);
        TXT	    GetStr(TXT section, TXT variable, TXT def = "");
        U64	    GetHex64(TXT section, TXT variable, U64 def, INT index = 0);
        S64	    GetVsx64(TXT section, TXT variable, S64 def, INT index = 0);
        U32	    GetHex(TXT section, TXT variable, U32 def, INT index = 0);
        S32	    GetVsx(TXT section, TXT variable, S32 def, INT index = 0);
        S32	    GetInt(TXT section, TXT variable, S32 def);
        void	GetWP(TXT section, U32 window_id, S16& x, S16& y, S16& w, S16& h, BIT& m);
        S16	    GetWPx(TXT section, U32 window_id, S16 def);
        S16	    GetWPy(TXT section, U32 window_id, S16 def);
        S16	    GetWPw(TXT section, U32 window_id, S16 def);
        S16	    GetWPh(TXT section, U32 window_id, S16 def);
        S16	    GetWPm(TXT section, U32 window_id, BIT def);
        INT		GetList(TXT section, CConfigList& list); // Returns proxy object, acts as array of entry names.
        void	Move(TXT src_section, TXT dst_section);
        void	Copy(TXT src_section, TXT dst_section);
        void	Delete(TXT section);
        S32	    GetSetInt(TXT section, TXT variable, S32 value);
        U32     GetSetHex(TXT section, TXT variable, U32 value);
        U64     GetSetHex64(TXT section, TXT variable, U64 value);
        S32     GetSetVsx(TXT section, TXT variable, S32 value);
        S64     GetSetVsx64(TXT section, TXT variable, S64 value);
        TXT     GetSetStr(TXT section, TXT variable, TXT value);
};

//Objects
extern KDeltaError DeltaError;
extern KDebug Debug;
extern KClipboard Clipboard;
extern KSystem System;
extern KFileSystem FileSystem;
extern KUtils Utils;
extern KString String;
extern KRegistry Registry;
extern KConfig INI;

template<typename T> T Derr(TXT function_name, T value, TXT error_string){
    DeltaError.AddError(function_name + ":\r\n" + error_string);
    return value;
}
void Derr(TXT function_name, TXT error_string);

#define De(...) Derr(__PRETTY_FUNCTION__, __VA_ARGS__)


INT TileModulus(INT x, INT tilesize);
INT InBox(INT xx, INT yy, INT l, INT t, INT r, INT b);
INT InBoxClipped(INT xx, INT yy, INT l, INT t, INT r, INT b, SRect clip);

#define CURSOR_ARROW		 0
#define CURSOR_WAIT			 1
#define CURSOR_WAITSMALL	 2
#define CURSOR_TEXT			 3
#define CURSOR_MOVE			 4
#define CURSOR_MOVE_NS		 5
#define CURSOR_MOVE_WE		 6
#define CURSOR_MOVE_NESW	 7
#define CURSOR_MOVE_NWSE	 8
#define CURSOR_DRAGDROP		 9
#define CURSOR_HAND			10
#define CURSOR_CROSSHAIR	11
#define CURSOR_NOTALLOWED	12
