#include <algorithm>
#include "include.h"

Vx<SVirtualData> CFileIO::VirtualFileArray;

static TXT GetTruncatedPath(TXT path)
{
    if((path.size() >= 2) && (path[1] != ':')){
		path = FileSystem.ProgramDirectory() + path;
	}
	TXT pd = FileSystem.ProgramDirectory();
    return path.substr(pd.size(), TXT::npos);
}
static BIT VirtualCompare(const SVirtualData& a, const SVirtualData& b)
{
	return a.path < b.path;
}
void CFileIO::InitVirtual(SResData res)
{
    if(res.size_ < 4) return;
    VirtualFileArray.clear();
    CFileIO f;
    try{
        f.LoadSectionEx("", 0, 0xFFFFFFFF, (U08*)res.data_, res.size_);
        INT num_files = f.GvHex(4);
        for(IND i = 0; i < num_files; i++){
            TXT path = f.GvString();
            U32 size = f.GvHex(4);
            U08* data = (((U08*)res.data_) + f.GetR()); //must use the resource data pointer
            f.GvSkipBytes(size);
            VirtualFileArray.push_back(SVirtualData(path, size, data));
        }
        f.Close();
    }catch(const exception& e){
    }endtry;
	sort(VirtualFileArray.begin(), VirtualFileArray.end(), VirtualCompare);
}
void CFileIO::RepoVirtual()
{
    VirtualFileArray.clear();
}

INT CFileIO::VirtualLookup(TXT path)
{
    if((path.size() >= 2) && (path[1] != ':')){
		path = FileSystem.ProgramDirectory() + path;
	}
	TXT pd = FileSystem.ProgramDirectory();
    if(path.substr(0, pd.size()) != pd){
        return -1;
    }else{
        TXT key = path.substr(pd.size(), TXT::npos);
        
        
        
        /*Debug.Box(key, "VirtualLookup()");
        TXT debug = "";
        BIT exists = false;
        for(IND i = 0; i < VirtualFileArray.size(); i++){
            debug += VirtualFileArray[i].path;
            debug += "\r\n";
            exists |= (key == VirtualFileArray[i].path);
        }
        if(!exists) Debug.Box(debug, key);*/
        
        
        
        
        // returns ID of the matching element if it finds key, otherwise -1.
        INT first = 0, last = VirtualFileArray.size()-1;
        while(first <= last)
        {
            INT mid = (first + last) / 2;
            if(key > VirtualFileArray[mid].path)
                first = mid + 1;
            else if(key < VirtualFileArray[mid].path)
                last = mid - 1;
            else
                return (INT)mid;
        }
        return -1;
    }
    return -1;
}
can_throw_t CFileIO::VirtualLoad(TXT filename)
{
    TXT virtual_path = filename;
    INT lookup = -1;
    if((lookup = VirtualLookup(virtual_path)) != -1){
        Fill(VirtualFileArray[lookup].data, VirtualFileArray[lookup].size);
    }else{
        last_error_ = "Could not open \"";
        last_error_ += filename;
        last_error_ += "\".\nPlease make sure that the file exists and is not in use by another program.";
        throw exception(last_error_);
    }
    SetupChunkValues_();
    return can_throw_t();
}
can_throw_tt<BMP> CFileIO::VirtualLoadPNG(TXT filename)
{
    TXT virtual_path = filename;
    INT lookup = -1;
    if((lookup = VirtualLookup(virtual_path)) != -1){
        SResData pngres((PTR)VirtualFileArray[lookup].data, VirtualFileArray[lookup].size);
        return LoadPNG(pngres);
    }
    return NULL;
}
void FileIO_BuildVirtualFiles()
{
    if(!FileSystem.FolderExists("NitroGUI")) return;
    Vx<TXT> files_list;
    FileSystem.FolderGetSubFilesList("NitroGUI", files_list);
    CFileIO f;
    try{
        f.SvHex(files_list.size(), 4);
        for(IND i = 0; i < files_list.size(); i++){
            TXT path = GetTruncatedPath(files_list[i]);
            f.SvString(path);
            f.SvFile(files_list[i]);
        }
        #ifdef IS_EDITOR_VERSION
        f.Save("..\\..\\src\\ResData\\NitroGUI.files");
        #else
        f.Save("..\\..\\src\\ResData\\NitroGUI_GE.files");
        #endif
        f.Close();
    }catch(const exception& e){
    }endtry;
}









