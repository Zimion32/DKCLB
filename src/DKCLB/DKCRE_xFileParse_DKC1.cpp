#include "include.h"
#include "DKCRE.h"
#include "CGameData.h"
#include "CLevelView.h"
#include "CLevelData.h"
#include "CObjectData.h"
#include "CTileMap.h"
#include "CSelectorObject.h"
#include "CAnimation.h"
#include "CFrame.h"
#include "STreeEntryIndex.h"
#include "SLevelDigitRelations.h"

/* Header Information Block:
6 BYTES: $DKCRE
1 BYTE: RESERVED (0x00 if not used)
4 BITS: RESERVED (keep as 0s)
4 BITS: DKC1 ROM VERSION:
	0 = ORIGINAL ROM WAS HACKED OR NOT ORIGINAL, USER FORCED LOAD.
	1 = U v1.0
	2 = U v1.1
	3 = U v1.2
	4 = U Competition Cart
	5 = E v1.0
	6 = E v1.1
	7 = J v1.0
	8 = J v1.1
*/
static void ApplyOldHacksFixesCheck(CBitwiseFileAccess& dkc1_rom)
{
	dkc1_rom.SetR(0xFFE0 << 3);
	if(string(dkc1_rom.GvText(6)) == "$DKCRE")
	{
		//This is an older DKCRE-based hack, must be v1.0
		//Apply some fixes to the ROM data to undo damage
		//caused from the buggy past versions of DKCRE.
		CBitwiseFileAccess temp;
		temp.Enable();
		temp.Fill(GetResourceID("BIN_DKC1_V10_OLDHACK_FIXER"));
		for(int i = 0; i < 0x40; i++){
			dkc1_rom[0xFFC0+i] ^= temp[i];
		}
		temp.Disable();
		//Go ahead and apply the new ROM header data.
		dkc1_rom.SetW(HackHeaderDKC1 << 3);
		dkc1_rom.SvText("$DKCRE");
		dkc1_rom.SvHex(0,8);//reserved
		dkc1_rom.SvHex(0,4);//reserved
		dkc1_rom.SvHex(1,4);//original version, here it's v1.0 U
	}
}
static bool FirstLoadCheckDKC1(CBitwiseFileAccess& dkc1_rom)
{
	dkc1_rom.SetR(HackHeaderDKC1 << 3);
	return (string(dkc1_rom.GvText(6)) != "$DKCRE");
}
static int HandleFirstLoadDKC1(CBitwiseFileAccess& dkc1_rom)
{
	//returns -1 if this is not DKC at all.
	CBitwiseFileAccess temp;
	Crv256 hash = dkc1_rom.SHA256();
	temp.Enable();
	temp.Fill(GetResourceID("BIN_DKC1_DATA_SHA256_HASHES"));
	Crv256* hashes = ((Crv256*)(&temp.Vector()[0]));
	int which = 0;
	for(int i = 0; i < 8; i++){
		if(hash == hashes[i]){
			which = i+1;
		}
	}
	temp.Disable();
	dkc1_rom.SetR(0xFFDC << 3);
	//Last resort ROM ID checker and etc.
	switch(dkc1_rom.GvHex(32)){
		case 0x7F1080EF:{if(which == -1){which = 1;}}break;
		case 0x832E7CD1:{if(which == -1){which = 2;}}break;
		case 0x33D4CC2B:{if(which == -1){which = 3;}}break;
		case 0x65AE9A51:{if(which == -1){which = 4;}}break;
		case 0xF65E09A1:{if(which == -1){which = 5;}}break;
		case 0x48ACB753:{if(which == -1){which = 6;}}break;
		case 0xE8F3170C:{if(which == -1){which = 7;}}break;
		case 0xA67F5980:{if(which == -1){which = 8;}}break;
		default:{return -1;}//this sure isn't a DKC1 rom file. error.
	}
	return which;
}
static int FirstLoadHeaderSetupDKC1(CBitwiseFileAccess& dkc1_rom, int which)
{
	dkc1_rom.SetW(HackHeaderDKC1 << 3);
	dkc1_rom.SvText("$DKCRE");
	dkc1_rom.SvHex(0,8);//reserved
	dkc1_rom.SvHex(0,4);//reserved
	dkc1_rom.SvHex(which,4);//original version
	return which;
}
static int GetVersionDKC1(CBitwiseFileAccess& dkc1_rom)
{
	dkc1_rom.SetR((HackHeaderDKC1 << 3) + 60);
	return dkc1_rom.GvHex(4);
}
static string GetIniNameDKC1(int which)
{
	switch(which){
		case 1: return "GameConfig\\DKC1v10U.INI";
		case 2: return "GameConfig\\DKC1v11U.INI";
		case 3: return "GameConfig\\DKC1v12U.INI";
		case 4: return "GameConfig\\DKC1vCCU.INI";
		case 5: return "GameConfig\\DKC1v10E.INI";
		case 6: return "GameConfig\\DKC1v11E.INI";
		case 7: return "GameConfig\\DKC1v10J.INI";
		case 8: return "GameConfig\\DKC1v11J.INI";
	}
	return "GameConfig\\DKC1v10U.INI";
}
string GetOriginalRomNameDKC1(int which)
{
	switch(which){
		case 1: return "DKC1v10U.smc";
		case 2: return "DKC1v11U.smc";
		case 3: return "DKC1v12U.smc";
		case 4: return "DKC1vCCU.smc";
		case 5: return "DKC1v10E.smc";
		case 6: return "DKC1v11E.smc";
		case 7: return "DKC1v10J.smc";
		case 8: return "DKC1v11J.smc";
	}
	return "DKC1v10U.smc";
}
string GetOriginalRomFriendlyNameDKC1(int which)
{
	switch(which){
		case 1: return "Donkey Kong Country (U) (v1.0) [!]";
		case 2: return "Donkey Kong Country (U) (v1.1) [!]";
		case 3: return "Donkey Kong Country (U) (v1.2) [!]";
		case 4: return "Donkey Kong Country - Competition Cartridge (U)";
		case 5: return "Donkey Kong Country (E) (v1.0) [!]";
		case 6: return "Donkey Kong Country (E) (v1.1) [!]";
		case 7: return "Donkey Kong Country (J) (v1.0) [!]";
		case 8: return "Donkey Kong Country (J) (v1.1) [!]";
	}
	return "Donkey Kong Country (U) (v1.0) [!]";
}
bool IsKnownOriginalRomFile(vector<Hex08>& file)
{
	//returns false if this is not a known ROM.
	CBitwiseFileAccess temp;
	Crv256 hash = temp.Sha256ButIgnoreRomHeaderSnes(file);
	temp.Enable();
	temp.Fill(GetResourceID("BIN_DKC1_DATA_SHA256_HASHES"));
	Crv256* hashes = ((Crv256*)(&temp.Vector()[0]));
	int which = -1;
	for(int i = 0; i < 8; i++){
		if(hash == hashes[i]){
			which = i+1;
		}
	}
	return (which != -1);
}


static void ApplyRomBugFixes(CBitwiseFileAccess& dkc1_rom)
{
	// We need to apply various fixes to the ROM 
	// before it can be properly edited in DKCRE.
	// This section is now powered by the INI.
	CConfigList commands;
	int a = ini_get_list("DKCRE_ROM_BugFixesToApply", commands);
	for(int x = 0; x < a; x++)
	{
		string cmd = commands[x];
		string act = ini_getstr("DKCRE_ROM_BugFixesToApply",cmd,"");
		//BUGFIX: Strip out the trailing and heading (basically all) whitespace of the commands.
		for(int i = 0; i < cmd.size(); i++){
            char c = cmd[i];
            if((c == ' ') || (c < 0x20)){
                cmd.erase(cmd.begin()+i); i--;
            }
		}
		for(int i = 0; i < act.size(); i++){
            char c = act[i];
            if((c == ' ') || (c < 0x20)){
                act.erase(act.begin()+i); i--;
            }
		}
		if(!act.size() || !cmd.size() || !((cmd.size() == 17) || (cmd.size() == 8))){
			continue;
		}
		cmd.erase(cmd.begin()+1);//delete the comma
		if(cmd.size() == 16){
			cmd.erase(cmd.begin()+7);//delete a .
			cmd.erase(cmd.begin()+7);//delete a .
			cmd.erase(cmd.begin()+7);//delete a .
		}
		char command_base = cmd[0]; //extract the base
		cmd.erase(cmd.begin()+0); //delete it
		//next validate the remaining command characters.
		//there should either be 6 or 12 by this point
		if(cmd.find_first_not_of("0123456789abcdefABCDEF") != string::npos){
			continue;
		}
		int cmd_addr0 = (hexstr2int(cmd.substr(0,6),6) & 0x3FFFFF);
		int cmd_addr1 = 0;
		if(cmd.size() == 12){
			cmd_addr1 = (hexstr2int(cmd.substr(6,6),6) & 0x3FFFFF);//lol wut
		}
		int command_base_value = -1;
		int command_base_type = -1;
		switch(command_base){
			case 'C':{
				command_base_type = 0;//copy
				command_base_value = 3; //signifies size of the address instead
			}break;
			case '1': case '2': case '3': case '4':{
				command_base_value = (command_base - '1' + 1);
				command_base_type = 1;//write
			}break;
			case '5': case '6': case '7': case '8':{
				command_base_value = (command_base - '5' + 1);
				command_base_type = 2;//ranged write
			}break;
			default: continue;
		}
		bool has_parenthesis = false;
		if(act[0] == '('){
			has_parenthesis = true;
			if(command_base_type != 1){continue;}//if clause not supported on other modes
			switch(act.size()){
				case 6:{
					if(command_base_value != 1){continue;}
					act.erase(act.begin()+0);
					act.erase(act.begin()+2);
				}break;
				case 10:{
					if(command_base_value != 2){continue;}
					act.erase(act.begin()+0);
					act.erase(act.begin()+4);
				}break;
				case 14:{
					if(command_base_value != 3){continue;}
					act.erase(act.begin()+0);
					act.erase(act.begin()+6);
				}break;
				case 18:{
					if(command_base_value != 4){continue;}
					act.erase(act.begin()+0);
					act.erase(act.begin()+8);
				}break;
				default:{continue;}
			}
		}else{
			switch(act.size()){
				case 2:{if(command_base_value != 1){continue;}}break;
				case 4:{if(command_base_value != 2){continue;}}break;
				case 6:{if(command_base_value != 3){continue;}}break;
				case 8:{if(command_base_value != 4){continue;}}break;
				default:{continue;}
			}
		}
		if(act.find_first_not_of("0123456789abcdefABCDEF") != string::npos){
			continue;
		}
		switch(command_base_type){
			case 0:{
				Hex32 addydest = hexstr2int(act.substr(0,act.size()),act.size());
				for(int i = cmd_addr0, j = addydest; i < cmd_addr1; i++, j++){
					dkc1_rom[j] = dkc1_rom[i];
				}
			}break;
			case 1:{
				if(has_parenthesis){
					Hex32 ifdata = hexstr2int(act.substr(           0,act.size()/2),act.size()/2);
					Hex32 wrdata = hexstr2int(act.substr(act.size()/2,act.size()/2),act.size()/2);
					if(dkc1_rom(cmd_addr0,command_base_value) == ifdata){
						dkc1_rom(cmd_addr0,command_base_value) = wrdata;
					}
				}else{
					Hex32 wrdata = hexstr2int(act.substr(0,act.size()),act.size());
					dkc1_rom(cmd_addr0,command_base_value) = wrdata;
				}
			}break;
			case 2:{
				Hex32 wrdata = hexstr2int(act.substr(0,act.size()),act.size());
				for(int i = cmd_addr0; i < cmd_addr1; i++){
					dkc1_rom(i,command_base_value) = wrdata;
				}
            }break;
		}
	}
}
Vsx32 DKCRE::LoadImage(string rom_file, string patch_file, int load_type, bool qReloadINI)
{
	SuspendUsageStats();
	InputToggle(false);
	bool isAlreadyOpen = DKC1.IsActive();
	bool firstTimeLoaded = false;
	try{
		if(!qReloadINI)
		{
			if(!isAlreadyOpen) DKC1.Enable();
				switch(load_type){
					case 0: DKC1.LoadROM(rom_file); break;
					case 1: DKC1.LoadROM(rom_file); break;
					case 2: DKC1.LoadIPS(rom_file, patch_file); break;
					case 3: DKC1.LoadUPS(rom_file, patch_file); break;
				}
			
			switch(load_type){
				case 0: vLastSavePath = rom_file; break;
				case 1: vLastSavePath = rom_file; break;
				case 2: vLastSavePath = patch_file; break;
				case 3: vLastSavePath = patch_file; break;
			}
		}
		else
		{
			vLevel->WriteData();
			if(!vIsSecondaryAppInstance) ini_save();
			ini_open_secondary("",1);
		}
		PrepareCache(qReloadINI);
		
		// Make sure the ROM is actually DKC-sized.
		if(DKC1.ByteSize() < 0x400000)
		{
			vLastLoadErrorTitle = "Load Failure: Incorrect Filesize";
			vLastLoadError = "This ROM is not \"Donkey Kong Country (U) (V1.0) [!].smc\"";
			DKC1.Disable(); InputToggle(true); 
			ResumeUsageStats();
			return false;
		}
		
		ApplyOldHacksFixesCheck(DKC1);
		
		firstTimeLoaded = FirstLoadCheckDKC1(DKC1);
		
		// If we're loading this image for the first time, do some house cleaning.
		if(firstTimeLoaded){
			vRomVersionID = HandleFirstLoadDKC1(DKC1);
			if(vRE_EnableBackups){
				DKC1.CreateDirectory("C:\\dkcre_temp\\");
				DKC1.CreateDirectory("C:\\dkcre_temp\\rom_backups\\");
				DKC1.FlushROM("C:\\dkcre_temp\\rom_backups\\" + GetOriginalRomNameDKC1(vRomVersionID));
			}
			FirstLoadHeaderSetupDKC1(DKC1, vRomVersionID);
			if(vRomVersionID == -1)
			{
				vLastLoadErrorTitle = "Load Failure: Incorrect ROM";
				vLastLoadError = "This ROM is not \"Donkey Kong Country (U) (V1.0) [!].smc\"";
				DKC1.Disable(); InputToggle(true); return false;
			}
		}else{
			//Since we found DKCRE's header we can assume this ROM is valid.
			vRomVersionID = GetVersionDKC1(DKC1);
		}
		string ini_name = GetIniNameDKC1(vRomVersionID);
		if(!BFA.FileExists(ini_name)){
			vLastLoadErrorTitle = "Load Failure: No Compatible Game Config Found";
			vLastLoadError = string("Problem with \"")+ini_name+"\":\r\n\tINI file is corrupted or missing.";
			DKC1.Disable(); InputToggle(true); return false;
		}
		ini_open_secondary(ini_name,1);
		if(ini_gethex("DKCRE_Parameters", "AppVersion", 0x00000609) != 0x00000609)
		{
			vLastLoadErrorTitle = "Load Failure: Incompatible Game Config";
			vLastLoadError = string("Problem with \"")+ini_name+"\":\r\n\tInvalid INI version.";
			DKC1.Disable(); InputToggle(true); return false;
		}
		if(firstTimeLoaded){
			//INI-based ROM bugfixes patcher.
			ApplyRomBugFixes(DKC1);
		}
		DKC1.SetR(0xFFC0 << 3);
		vGameName = string(DKC1.GvText(21));
	}catch(const exception& e){
		vLastLoadErrorTitle = "Unable to Open Game";
		vLastLoadError = DKC1.LastError();
		if(!isAlreadyOpen) DKC1.Disable();
		InputToggle(true);
		ResumeUsageStats();
		return -1;
	}endtry;
	// Temporary variable, re-used several times.
	CConfigList entries;

	// Create global DKC game data structure.
	gbl = new CGameData();

	// Load ObjectData(s) from the Object Dictionary.
	int m = ini_get_list("ObjectDictionary", entries);
	
	//ini_getstr("PrepareAllObjectsOnLoad", entries[i], "<Description Missing>")
	if(vPrepareAllObjectsOnLoad)
	{
		for(int i = 0; i < m; i++)
		{
			vObjectsData.push_back(
				new CObjectData(gbl, entries[i], ini_getstr("ObjectDictionary", entries[i], "<Description Missing>")));
			
			vObjectsData[i]->Prep();
			vObjectLookup[ vObjectsData[i]->GetWord() ] = i;
		}
	}
	else
	{
		for(int i = 0; i < m; i++)
		{
			/*string checkhack = entries[i];
			if(checkhack.substr(1,4) == "FEED")
			{
				string theanimdigit = int2hexstr(ANIMATION_DIGIT_HACK_TO, 4);
				checkhack[20] = theanimdigit[0];
				checkhack[21] = theanimdigit[1];
				checkhack[22] = theanimdigit[2];
				checkhack[23] = theanimdigit[3];
				vObjectsData.push_back(
					new CObjectData(gbl, checkhack, ini_getstr("ObjectDictionary", entries[i], "<Description Missing>")));
			}
			else
			{*/
				vObjectsData.push_back(new CObjectData(gbl, entries[i], ini_getstr("ObjectDictionary", entries[i], "<Description Missing>")));
			//}
			vObjectLookup[ vObjectsData[i]->GetWord() ] = i;
		}
	}
	
	
	LoadObjectCatalogs();
	
	// Load up the last viewed level's ID.
	Hex32 theLastLevelViewed = ini_gethex("LevelDisplay", "Last", 0x100);
	if(theLastLevelViewed != 0x100)
		vLevelID = theLastLevelViewed;
		
	// Create Level Viewer...
	vLevel = new CLevelView(gbl, vLevelID);

	// ...and load level names into list, excluding where appropriate.
	m = ini_get_list("LevelNameList", entries);
	vector<bool> listed(0xE6,false);
	for(int i = 0; i < m; i++)
	{
		vLevelDigitList.push_back( hexstr2int(entries[i], 2) );
		string name = ini_getstr("LevelNameList", entries[i], "NULL");
		if(name != "NULL")
		{
			vLevelNameList.push_back(name);
		}
		else
		{
			vLevelNameList.push_back( string(vLevel->lvl->GetLevelName(gbl, hexstr2int(entries[i], 2), false)) );
		}
		if(hexstr2int(entries[i], 2) == theLastLevelViewed)
		{
			vLastLevelListID = vLevelListID = i;
		}
		listed[hexstr2int(entries[i], 2)] = true;
	}
	for(int i = 0; i < 0xE6; i++)
	{
		vLevelRelationsTable.push_back( SLevelDigitRelations() );
		vLevelRelationsTable[i].Configure(
			string(ini_getstr("LevelDigitReplationships", int2hexstr(i,2), "--------------------------------------------")));
		
		if(!listed[i])
		{
			string exclude = ini_getstr("LevelNameListExclude", int2hexstr(i,2), "NO_EXCULDE");
			if(exclude != "NO_EXCULDE") continue;
			if(static_cast<unsigned>(i) == theLastLevelViewed)
			{
				vLastLevelListID = vLevelListID = vLevelDigitList.size();
			}
			string ln = "0x" + int2hexstr(i,2) + " - " + vLevel->lvl->GetLevelName(gbl, i, false);
			vLevelDigitList.push_back( i );
			vLevelNameList.push_back( ln );
		}
	}
	GuiBind(MainForm_CtLvlEdit_Tool0_SelectLevel)("List", &vLevelNameList);
	
	for(int i = 0xFF; i >= 0; --i)
	{
		vLevelDigitListLookup.push_back(-1);
	}
	for(int i = (vLevelDigitList.size() - 1); i >= 0; --i)
	{
		vLevelDigitListLookup[vLevelDigitList[i]] = i;
	}
	
	m = ini_get_list("LevelMusicPointers", entries);
	for(int i = 0; i < m; i++)
	{
		vMusicTrackPointers.push_back( hexstr2int(entries[i], 4) );
		vMusicTracks.push_back(string(ini_getstr("LevelMusicPointers", entries[i], "<Undefined Track>")));
	}
	
	vLevel->ReLoad(vLevelID);//Reload level again due to a few things needing refreshed
	
	InitPE();
	InitEX();
	
	vEditMode = (ini_gethex("LevelDisplay", "EditMode", 7) % 8);
	InputToggle(true);
	ResumeUsageStats();
	return (firstTimeLoaded ? 2 : 1);
}
//##############################################################################
//##############################################################################
Vsx32 DKCRE::CloseImage()
{
	SuspendUsageStats();
	InputToggle(false);
	DKC1.Disable();
	PrepareCache(false);
	InputToggle(true);
	ResumeUsageStats();
	return 0;
}
//##############################################################################
//##############################################################################
void DKCRE::PrepareCache(bool qIsIniReload)
{
	vHasChanges = (qIsIniReload?vHasChanges:false);
	if(vLevel) delete vLevel;	vLevel = NULL;
	if(gbl)    delete gbl;		gbl    = NULL;
	for(int i = (vObjectsData.size() - 1); i >= 0; --i)
	{
		delete vObjectsData[i];
	}
	GuiUnbind(MainForm_CtLvlEdit_Tool0_SelectLevel,"List");
	
	vLevelDigitList.clear();
	vLevelDigitListLookup.clear();
	vLevelNameList.clear();
	vLevelRelationsTable.clear();
	vObjectsData.clear();
	ClearObjectCatalogs();
	vMusicTrackPointers.clear();
	vMusicTracks.clear();
	
	// If these tables or buffers need to be allocated, do so only once.
	if(!vObjectLookup.size())
	{
		vObjectLookup.reserve(0x10000);
		for(int i = 0xFFFF; i >= 0; --i){vObjectLookup.push_back(0xFFFF);}
	}
	else
	{
		for(int i = 0xFFFF; i >= 0; --i){vObjectLookup[i] = 0xFFFF;}
	}
	
	if(!vObjectPanelLookup.size())
	{
		vObjectPanelLookup.reserve(0x10000);
		for(int i = 0xFFFF; i >= 0; --i)
		{
			vObjectPanelLookup.push_back( STreeEntryIndex() );
			vObjectPanelLookup[0xFFFF-i].w = -1;
			vObjectPanelLookup[0xFFFF-i].x = 0;
			vObjectPanelLookup[0xFFFF-i].y = 0;
			vObjectPanelLookup[0xFFFF-i].z = 0;
		}
	}
	else
	{
		for(int i = 0xFFFF; i >= 0; --i)
		{
			vObjectPanelLookup[i].w = -1;
			vObjectPanelLookup[i].x = 0;
			vObjectPanelLookup[i].y = 0;
			vObjectPanelLookup[i].z = 0;
		}
	}
	
	/*if(!vFrames.size())
	{
		vFrames.reserve(0x1000);
		for(int i = 0x0FFF; i >= 0; --i){vFrames.push_back(NULL);}
	}
	else
	{
		for(int i = (vFrames.size() - 1); i >= 0; --i)
		{
			delete vFrames[i];
			vFrames[i] = NULL;
		}
	}
	
	if(!vAnimations.size())
	{
		vAnimations.reserve(0x400);
		for(int i = 0x03FF; i >= 0; --i){vAnimations.push_back(NULL);}
	}
	else
	{
		for(int i = (vAnimations.size() - 1); i >= 0; --i)
		{
			delete vAnimations[i];
			vAnimations[i] = NULL;
		}
	}*/
}
//##############################################################################
//##############################################################################
void DKCRE::LoadObjectCatalogs()
{
	CConfigList entries;
	int m = ini_get_list("ItemsPanelListing", entries);
	for(int i = 0; i < m; i++)
	{
		CSelectorObject* cso;
		string s_coords = ini_getstr("ItemsPanelListing", entries[i], "0000");
		Vsx32 coords = hexstr2int(s_coords, 4);
		Vsx32 x_coord = Vsx08(Vsx08(coords >> 8) & 0xFF);
		Vsx32 y_coord = Vsx08(Vsx08(coords) & 0xFF);
		Hex16 theWord = hexstr2int(entries[i], 4);
		cso = new CSelectorObject(theWord, x_coord + 32, y_coord + 32);
		vItemsList.push_back( cso );
		vObjectPanelLookup[theWord].w = 3;
		vObjectPanelLookup[theWord].x = 0;
		vObjectPanelLookup[theWord].y = 0;
		vObjectPanelLookup[theWord].z = i;
	}
	
	CConfigList categories;
	int a = ini_get_list("ObjectCategories", categories);
	for(int x = 0; x < a; x++)
	{
		vOpList.push_back(
			ini_getstr("ObjectCategories", categories[x], "<Description Missing>"));
		vOpSubLists.push_back( vector<string>() );
		vOpSelectorObjects.push_back( vector< vector<CSelectorObject*> >() );
		vOpSizeList.push_back( vector<Hex32>() );
		vOpTextXList.push_back( vector<Hex32>() );
		
		CConfigList subcategories;
		int b = ini_get_list(categories[x], subcategories);
		for(int y = 0; y < b; y++)
		{
			vOpSubLists[x].push_back(
				ini_getstr(categories[x], subcategories[y], "<Description Missing>"));
			vOpSelectorObjects[x].push_back( vector<CSelectorObject*>() );
			string theOffsetString = ini_getstr((string(categories[x]) + "Offsets"), subcategories[y], "00000000");
			Hex32 theOffset = hexstr2int(theOffsetString,8);
			
			vOpSizeList[x].push_back((theOffset >> 24) & 0xFF);
			vOpTextXList[x].push_back(theOffset & 0xFF);
			
			CConfigList subsublistitems;
			int c = ini_get_list(subcategories[y], subsublistitems);
			for(int z = 0; z < c; z++)
			{
				CSelectorObject* cso;
				string s_coords = ini_getstr(subcategories[y], subsublistitems[z], "0000");
				Vsx32 coords = hexstr2int(s_coords, 4);
				Vsx32 x_coord = Vsx08(Vsx08(coords >> 8) & 0xFF);
				Vsx32 y_coord = Vsx08(Vsx08(coords) & 0xFF);
				Hex16 theWord = hexstr2int(subsublistitems[z], 4);
				cso = new CSelectorObject(theWord, x_coord + ((theOffset >> 16) & 0xFF), y_coord + ((theOffset >> 8) & 0xFF));
				vOpSelectorObjects[x][y].push_back( cso );
				vObjectPanelLookup[theWord].w = 2;
				vObjectPanelLookup[theWord].x = x;
				vObjectPanelLookup[theWord].y = y;
				vObjectPanelLookup[theWord].z = z;
			}
		}
	}
	
	a = ini_get_list("EnemyCategories", categories);
	for(int x = 0; x < a; x++)
	{
		vEpList.push_back(
			ini_getstr("EnemyCategories", categories[x], "<Description Missing>"));
		vEpSubLists.push_back( vector<string>() );
		vEpSelectorObjects.push_back( vector< vector<CSelectorObject*> >() );
		vEpSizeList.push_back( vector<Hex32>() );
		vEpTextXList.push_back( vector<Hex32>() );
		
		CConfigList subcategories;
		int b = ini_get_list(categories[x], subcategories);
		for(int y = 0; y < b; y++)
		{
			vEpSubLists[x].push_back(
				ini_getstr(categories[x], subcategories[y], "<Description Missing>"));
			vEpSelectorObjects[x].push_back( vector<CSelectorObject*>() );
			string theOffsetString = ini_getstr((string(categories[x]) + "Offsets"), subcategories[y], "00000000");
			Hex32 theOffset = hexstr2int(theOffsetString,8);
			
			vEpSizeList[x].push_back((theOffset >> 24) & 0xFF);
			vEpTextXList[x].push_back(theOffset & 0xFF);
			
			CConfigList subsublistitems;
			int c = ini_get_list(subcategories[y], subsublistitems);
			for(int z = 0; z < c; z++)
			{
				CSelectorObject* cso;
				string s_coords = ini_getstr(subcategories[y], subsublistitems[z], "0000");
				Vsx32 coords = hexstr2int(s_coords, 4);
				Vsx32 x_coord = Vsx08(Vsx08(coords >> 8) & 0xFF);
				Vsx32 y_coord = Vsx08(Vsx08(coords) & 0xFF);
				Hex16 theWord = hexstr2int(subsublistitems[z], 4);
				cso = new CSelectorObject(theWord, x_coord + ((theOffset >> 16) & 0xFF), y_coord + ((theOffset >> 8) & 0xFF));
				vEpSelectorObjects[x][y].push_back( cso );
				vObjectPanelLookup[theWord].w = 1;
				vObjectPanelLookup[theWord].x = x;
				vObjectPanelLookup[theWord].y = y;
				vObjectPanelLookup[theWord].z = z;
			}
		}
	}
}
//##############################################################################
//##############################################################################
void DKCRE::ClearObjectCatalogs()
{
	for(int i = (vItemsList.size() - 1); i >= 0; --i)
	{
		delete vItemsList[i];
	}
	vItemsList.clear();
	
	vEpList.clear();
	for(int x = (vEpSubLists.size() - 1); x >= 0; --x)
	{
		vEpSizeList[x].clear();
		vEpTextXList[x].clear();
		vEpSubLists[x].clear();
		for(int y = (vEpSelectorObjects[x].size() - 1); y >= 0; --y)
		{
			for(int z = (vEpSelectorObjects[x][y].size() - 1); z >= 0; --z)
			{
				delete vEpSelectorObjects[x][y][z];
			}
			vEpSelectorObjects[x][y].clear();
		}
		vEpSelectorObjects[x].clear();
	}
	vEpSizeList.clear();
	vEpTextXList.clear();
	vEpSubLists.clear();
	vEpSelectorObjects.clear();
	
	vOpList.clear();
	for(int x = (vOpSubLists.size() - 1); x >= 0; --x)
	{
		vOpSizeList[x].clear();
		vOpTextXList[x].clear();
		vOpSubLists[x].clear();
		for(int y = (vOpSelectorObjects[x].size() - 1); y >= 0; --y)
		{
			for(int z = (vOpSelectorObjects[x][y].size() - 1); z >= 0; --z)
			{
				delete vOpSelectorObjects[x][y][z];
			}
			vOpSelectorObjects[x][y].clear();
		}
		vOpSelectorObjects[x].clear();
	}
	vOpSizeList.clear();
	vOpTextXList.clear();
	vOpSubLists.clear();
	vOpSelectorObjects.clear();
}
