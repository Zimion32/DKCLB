#include "include.h"
#ifndef CFSTR_PREFERREDDROPEFFECT
#define CFSTR_PREFERREDDROPEFFECT L"Preferred DropEffect"
#endif

/// OpenClipboard() Requires valid HWND (not NULL), otherwise EmptyClipboard() 
/// will set owner to NULL and SetClipboardData() would fail.

void KClipboard::SetBinary(Vx<U08>& data, U32 id)
{
	if(id == 0) id = 0xFFFFFFFF;
	UINT GENERIC_BINARY_DATA_FORMAT = RegisterClipboardFormat(String.ToWideC("SubwareSoftCFzGENERICBINARYDATAv00000000"));
	HGLOBAL hText = GlobalAlloc(GMEM_MOVEABLE, data.size()+4);
	PTR theData = GlobalLock(hText);
	memcpy(theData, ((PTR)&data[0]), data.size());
	memcpy( (PTR)(((U08*)theData)+data.size()), (PTR)&id, 4);
	GlobalUnlock(hText);
	OpenClipboard(System.MainHWND);
	EmptyClipboard();
	SetClipboardData(GENERIC_BINARY_DATA_FORMAT, hText);
	CloseClipboard();
}
Vx<U08> KClipboard::GetBinary(U32 id)
{
	Vx<U08> result;
	if(id == 0) id = 0xFFFFFFFF;
	UINT GENERIC_BINARY_DATA_FORMAT = RegisterClipboardFormat(String.ToWideC("SubwareSoftCFzGENERICBINARYDATAv00000000"));
	if(IsClipboardFormatAvailable(GENERIC_BINARY_DATA_FORMAT)){
		OpenClipboard(System.MainHWND);
		HGLOBAL hglb = GetClipboardData(GENERIC_BINARY_DATA_FORMAT);
		U08* theData = (U08*)GlobalLock(hglb);
		U32 theSize = GlobalSize(hglb);
		U32 theId = 0;
		memcpy((PTR)&theId, (PTR)(theData+theSize-4), 4);
		if(theId != id){
			GlobalUnlock(hglb);
			CloseClipboard();
			return result;
		}
		result.clear();
		result.resize(theSize-4,0x00);
		memcpy((PTR)&result[0], (PTR)theData, theSize-4);
		GlobalUnlock(hglb);
		CloseClipboard();
	}
	return result;
}
void KClipboard::SetText(TXT clipText)
{
	T16 wClipText = String.ToWide(clipText);
	const C16* strbuf = wClipText.APPROVED_CONVERT_TO_C_STR();
	HGLOBAL hText = GlobalAlloc(GMEM_MOVEABLE, (wClipText.size()*2 + 2));
	lstrcpy((C16*)GlobalLock(hText), strbuf);
	GlobalUnlock(hText);
	OpenClipboard(System.MainHWND);
	EmptyClipboard();
	SetClipboardData(CF_UNICODETEXT, hText);
	CloseClipboard();
}
TXT KClipboard::GetText()
{
	TXT clipText = "[NULL]";
	if(IsClipboardFormatAvailable(CF_UNICODETEXT))
	{
		OpenClipboard(System.MainHWND);
		HGLOBAL hglb = GetClipboardData(CF_UNICODETEXT);
		T16 wClipText = (C16*)GlobalLock(hglb);
		clipText = String.FromWide(wClipText);
		GlobalUnlock(hglb);
		CloseClipboard();
	}
	return clipText;
}
//#include <locale.h>
//#include <codecvt.h>
BIT KClipboard::GetOperation()
{
	UINT PDE_DATA_FORMAT = RegisterClipboardFormat((CFSTR_PREFERREDDROPEFFECT));
	if(IsClipboardFormatAvailable(PDE_DATA_FORMAT))
	{
		//WARNING: First pointer is NOT to an STGMEDIUM
		//         It's a pointer to the actual raw DWORD!!
		OpenClipboard(System.MainHWND);
		HGLOBAL hglb = GetClipboardData(PDE_DATA_FORMAT);
		DWORD theEffect = *((DWORD*)GlobalLock(hglb));
		GlobalUnlock(hglb);
		CloseClipboard();
		return (theEffect & DROPEFFECT_MOVE);
	}
	return 0; //by default, copy
}
void KClipboard::SetOperation(BIT is_cutting)
{
	HGLOBAL hTheDword = GlobalAlloc(GMEM_FIXED, sizeof(DWORD));
	(*(DWORD*)GlobalLock(hTheDword)) = (is_cutting ? DROPEFFECT_MOVE : DROPEFFECT_COPY);
	GlobalUnlock(hTheDword);
	UINT PDE_DATA_FORMAT = RegisterClipboardFormat((CFSTR_PREFERREDDROPEFFECT));
	OpenClipboard(System.MainHWND);
	SetClipboardData(PDE_DATA_FORMAT, hTheDword);
	CloseClipboard();
}
BIT KClipboard::HasFiles()
{
	return IsClipboardFormatAvailable(CF_HDROP);
}
void KClipboard::GetFilesList(Vx<TXT>& path_list)
{
	if(IsClipboardFormatAvailable(CF_HDROP))
	{
		//WARNING: First pointer is NOT to an STGMEDIUM
		//         It's a pointer to the actual raw data!!
		OpenClipboard(System.MainHWND);
		HGLOBAL hglb = GetClipboardData(CF_HDROP);
		DROPFILES* theFiles = ((DROPFILES*)GlobalLock(hglb));
		C16* currentfile = ((C16*)(((CHR*)(theFiles))+(theFiles->pFiles)));
		while(true){
			T16 w_filename = currentfile;
			if(w_filename.size() == 0){break;}
			TXT filename = String.FromWide(w_filename);
			//Watch out! We might get \\?\ before the actual path! [UNC Names]
			filename = FileSystem.UncUnPrefix(filename);
			path_list.push_back(filename);
			currentfile += (w_filename.size()+1);
		}
		GlobalUnlock(hglb);
		CloseClipboard();
	}
}
void KClipboard::CutCopyFilesList_(Vx<TXT>& path_list, BIT is_cutting)
{
	//SET THE DROP FILES LIST
	Vx<T16> w_path_list;
	U32 byte_size = 0;
	for(IND i = 0; i < path_list.size(); i++){
		//WATCH OUT: Must add \\?\ to the file path! [UNC Names]
		w_path_list.push_back(String.ToWide(FileSystem.UncDoPrefix(FileSystem.ParsePathReparsePoints(path_list[i]))));
		byte_size += (w_path_list[i].size()*2)+2;
	}
	byte_size += 2;//terminating null TXT!
	HGLOBAL hFilesList = GlobalAlloc(GMEM_MOVEABLE, byte_size+sizeof(DROPFILES));
	DROPFILES* files_list = (DROPFILES*)GlobalLock(hFilesList);
	files_list->pFiles = sizeof(DROPFILES);//ABSOLUTELY MUST always equal sizeof(DROPFILES), else will glitch.
	files_list->pt.x = 0;
	files_list->pt.y = 0;
	files_list->fNC = TRUE; //note: win32 TRUE
	files_list->fWide = TRUE; //note: win32 TRUE
	C16* currentfile = ((C16*)(((CHR*)(files_list))+(files_list->pFiles)));
	for(IND i = 0; i < w_path_list.size(); i++){
		currentfile[w_path_list[i].size()] = L'\0';
		lstrcpy((WCHAR*)currentfile, (WCHAR*)w_path_list[i].APPROVED_CONVERT_TO_C_STR());
		currentfile += (w_path_list[i].size()+1);
	}
	currentfile[0] = L'\0';//terminating null TXT!
	GlobalUnlock(hFilesList);
	
	//SET THE DROP EFFECT TYPE
	UINT PDE_DATA_FORMAT = RegisterClipboardFormat((CFSTR_PREFERREDDROPEFFECT));
	HGLOBAL hTheDword = GlobalAlloc(GMEM_FIXED, sizeof(DWORD));
	(*(DWORD*)GlobalLock(hTheDword)) = (is_cutting ? DROPEFFECT_MOVE : DROPEFFECT_COPY);
	GlobalUnlock(hTheDword);
	
	//PUT THIS STUFF ONTO THE CLIPBOARD
	//Notice that this must all be done within this 
	//clipboard "open" action, you can't seperate them.
	OpenClipboard(System.MainHWND);
	EmptyClipboard();
	SetClipboardData(CF_HDROP, hFilesList);
	SetClipboardData(PDE_DATA_FORMAT, hTheDword);
	CloseClipboard();
}
void KClipboard::CopyFiles(Vx<TXT>& path_list)
{
	CutCopyFilesList_(path_list, false);
}
void KClipboard::CutFiles(Vx<TXT>& path_list)
{
	CutCopyFilesList_(path_list, true);
}
void KClipboard::PasteFiles(TXT where, Vx<TXT>& error_list)
{
	if(IsClipboardFormatAvailable(CF_HDROP))
	{
		if(where.find_last_of("\\/") != (where.size()-1)) where += "\\";
		where = FileSystem.UncDoPrefix(FileSystem.ParsePathReparsePoints(where));
		FileSystem.CreateDirectory(where); //make sure that actually exists before doing work
		BIT is_cutting = false;
		OpenClipboard(System.MainHWND);
		UINT PDE_DATA_FORMAT = RegisterClipboardFormat((CFSTR_PREFERREDDROPEFFECT));
		if(IsClipboardFormatAvailable(PDE_DATA_FORMAT))
		{
			HGLOBAL hglb = GetClipboardData(PDE_DATA_FORMAT);
			DWORD theEffect = *((DWORD*)GlobalLock(hglb));
			GlobalUnlock(hglb);
			is_cutting = (theEffect & DROPEFFECT_MOVE);
		}
		HGLOBAL hglb = GetClipboardData(CF_HDROP);
		DROPFILES* theFiles = ((DROPFILES*)GlobalLock(hglb));
		C16* currentfile = ((C16*)(((CHR*)(theFiles))+(theFiles->pFiles)));
		Vx<TXT> files_list;
		while(true){
			T16 w_filename = currentfile;
			if(w_filename.size() == 0) break;
			files_list.push_back(FileSystem.UncDoPrefix(FileSystem.ParsePathReparsePoints(String.FromWide(w_filename))));
			currentfile += (w_filename.size()+1);
		}
		INT files_count = 0;
		for(IND i = 0; i < files_list.size(); i++){
			if(FileSystem.FolderExists(files_list[i])){
				Vx<TXT> count_list;
				FileSystem.FolderGetSubFilesList(files_list[i], count_list);
				files_count += count_list.size();
			}else{
				files_count++;
			}
		}
		for(IND i = 0; i < files_list.size(); i++)
		{
			if(is_cutting){
				if(FileSystem.FolderExists(files_list[i])){
					FileSystem.MoveFolderClipboard(files_list[i], where, error_list);
				}else{
					FileSystem.MoveFileClipboard(files_list[i], where, error_list);
				}
			}else{
				if(FileSystem.FolderExists(files_list[i])){
					FileSystem.CopyFolderClipboard(files_list[i], where, error_list);
				}else{
					FileSystem.CopyFileClipboard(files_list[i], where, error_list);
				}
			}
		}
		GlobalUnlock(hglb);
		EmptyClipboard();
		CloseClipboard();
	}
}
BIT KClipboard::SetBitmapData(BMP bmp)
{
	if(!OpenClipboard(System.MainHWND)) return true;
	if(!EmptyClipboard()) return true;
	HBITMAP hBmp = convert_bitmap_to_hbitmap(bmp);
	if(SetClipboardData(CF_BITMAP, hBmp) == NULL)
	{
		CloseClipboard();
		return true;
	}
	CloseClipboard();
	DeleteObject(hBmp);
	return false;
}


