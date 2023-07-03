#include "include.h"
#include "ObjectDataHelpers.h"
#include "CObjectData.h"
#include "CGameData.h"
#include "DKCRE.h"
#include "DKCRE_Graphics.h"
BITMAP* RE_DKC1_Sprite(int param_spritenumber, int objectpallete, SRect* rect);
BITMAP* RE_DKC1_DualSprite(int param_spritenumber, int param_spritenumber2, int object_pallete, int object_pallete2, SRect* rect, bool invert);
Vsx32 FindObjectPaletteDKC1(Hex16 word)
{
	//return 0xFFFFFFFF;
	Hex32 result = 0xFFFFFFFF;
	Hex32 base = (0x350000 | word);
	vector<Vsx32> address_stack;
	for(Vsx32 i = 0, exit = 0; exit != -1;)
	{
		switch(DKC1(base+i,2))
		{
			case 0x8000: // Nothing! Bail Out.
			{
				if(address_stack.size())
				{
					i = address_stack.back();
					address_stack.erase(address_stack.end()-1);
				}
				else
				{
					exit--;
				}
			}
			break; 
			case 0x8200: // Jump
			{
				address_stack.push_back(i+4);
				i = ((0x350000 | DKC1(base+i+2,2)) - base);		
			}
			break; 
			case 0x8800: // Found! Exit Loop.
			{
				result = (0x3C0000 | DKC1(base+i+2,2));
				exit--;
			}
			break; 
			default: // Do Nothing
			{
				i += 4;
			}
			break; 
		}
	}
	return result;
}
Vsx32 FindObjectAnimationDKC1(Hex16 word)
{
	//return 0xFFFFFFFF;
	Hex32 result = 0xFFFFFFFF;
	Hex32 base = (0x350000 | word);
	vector<Vsx32> address_stack;
	for(Vsx32 i = 0, exit = 0; exit != -1;)
	{
		switch(DKC1(base+i,2))
		{
			case 0x8000: // Nothing! Bail Out.
			{
				if(address_stack.size())
				{
					i = address_stack.back();
					address_stack.erase(address_stack.end()-1);
				}
				else
				{
					exit--;
				}
			}
			break; 
			case 0x8200: // Jump
			{
				address_stack.push_back(i+4);
				i = ((0x350000 | DKC1(base+i+2,2)) - base);		
			}
			break; 
			case 0x8100: // Found! Exit Loop.
			{
				result = DKC1(base+i+2,2);
				exit--;
			}
			break; 
			default: // Do Nothing
			{
				i += 4;
			}
			break; 
		}
	}
	return result;
}
int DEBUGX_generated = 0;
CAnimation* PreRenderAnimationWithPaletteDKC1(Hex32 object_ani, Hex32 object_pal, Hex16 word)
{
	safe_array(Hex08,AnimID,0x01, 0x00, 
	((object_ani >> 16) & 0xFF), ((object_ani >> 8) & 0xFF), (object_ani & 0xFF),
	((object_pal >> 16) & 0xFF), ((object_pal >> 8) & 0xFF), (object_pal & 0xFF));
	
	CAnimation* anim = Animation(AnimID);
	if(anim->GetLength()) return anim; //If the animation already has frames, we had a cached copy!
	
	DEBUGX_generated++;
	
	Hex32 addr = ((DKCRE::gbl->AnimationScriptBank | DKCRE::gbl->AnimationScriptTable) + (object_ani * 2));
	Hex32 ani_offset = (DKC1(addr,2) | DKCRE::gbl->AnimationScriptBank);
	
	vector<Vsx32> address_stack;
	for(int i = 0, exit = 0; exit != -1;)
	{
		int current_op = (DKC1[ani_offset+i] & 0xFF);
		if((current_op & 0x80) != 0x80)
		{
			Hex32 object_dly = DKC1(ani_offset+i+0,1);
			Hex32 object_spr = DKC1(ani_offset+i+1,2);
			if((object_spr > 0x2AF0)||(object_spr < 0x8C)) return anim; // Abort if out of range. Returns empty animation!
			safe_array(Hex08,GfxID, 0x01, 0x01, 
				((object_spr >> 16) & 0xFF), ((object_spr >> 8) & 0xFF), (object_spr & 0xFF),
				((object_pal >> 16) & 0xFF), ((object_pal >> 8) & 0xFF), (object_pal & 0xFF));
			Hex32 dualdisplaysprite = ini_gethex("SpriteDualFrames", int2hexstr(object_spr,4),0);
			Hex32 object_ddpal = ini_gethex("ObjectDualPalette", int2hexstr(word,4),0);
			if((dualdisplaysprite & 0xFFFF) && (object_ddpal != 0))
			{
				GfxID[1] |= 0x02; //Keep value range for dual sprites seperated!
				SRect rect;//gets rect border - passed to graphic to accelerate image cropping. order: L, T, R, B
				BITMAP* image = RE_DKC1_DualSprite(
					object_spr, (dualdisplaysprite & 0xFFFF), object_pal, (object_ddpal?object_ddpal:object_pal), 
					&rect, (dualdisplaysprite & ~0xFFFF));
				anim->AddFrame(Frame(Graphic(image,&rect,GfxID), 128, 128), object_dly);
			}
			else
			{
				SRect rect;//gets rect border - passed to graphic to accelerate image cropping. order: L, T, R, B
				BITMAP* image = RE_DKC1_Sprite(object_spr, object_pal, &rect);
				anim->AddFrame(Frame(Graphic(image,&rect,GfxID), 128, 128), object_dly);
			}
			i += 3;
		}
		else
		{
			switch(current_op)
			{
				case 0x80:
				{
					if(address_stack.size())
					{
						i = address_stack.back();
						address_stack.erase(address_stack.end()-1);
					}
					else
					{
						exit--;
					}
				} break; //Bail out.
				case 0x81:
				{
					i += 4;
				} break;
				case 0x82:
				{
					address_stack.push_back(i+4);
					int adress = (DKCRE::gbl->AnimationScriptBank | DKC1(ani_offset+i+2,2));
					i = (adress - ani_offset);
				}
				break;
				case 0x83:
				{
					i += 3;
				} break;
				case 0x84:
				{
					i += 4;
				} break;
				case 0x85:
				{
					/*string dbgout = (int2hexstr(vWord,4) + ": " + vDescription);
					DebugBox(dbgout,"This object has an animation with script 0x85:");
					i += 6;*/
				} break;
				case 0x86:
				{
					/*string dbgout = (int2hexstr(vWord,4) + ": " + vDescription);
					DebugBox(dbgout,"This object has an animation with script 0x86:");
					i += 10;*/
				} break;
				case 0x87:
				{
					/*string dbgout = (int2hexstr(vWord,4) + ": " + vDescription);
					DebugBox(dbgout,"This object has an animation with script 0x87:");
					i += 8;*/
				} break;
				case 0x88:
				{
					/*string dbgout = (int2hexstr(vWord,4) + ": " + vDescription);
					DebugBox(dbgout,"This object has an animation with script 0x88:");
					i += 5;
				} break;
				case 0x8B:
				{
					string dbgout = (int2hexstr(vWord,4) + ": " + vDescription);
					DebugBox(dbgout,"This object has an animation with script 0x8B:");
					i += 8;*/
				} break;
				case 0x8C:
				{
					/*string dbgout = (int2hexstr(vWord,4) + ": " + vDescription);
					DebugBox(dbgout,"This object has an animation with script 0x8C:");
					i += 3;*/
				} break;
				case 0x8D:
				{
					/*string dbgout = (int2hexstr(vWord,4) + ": " + vDescription);
					DebugBox(dbgout,"This object has an animation with script 0x8D:");
					i += 5;*/
				} break;
				case 0x8E:
				{
					i += 2;
				} break;
				case 0x8F:
				{
					i += 2;
				} break;
				case 0x90:
				{
					i += 2;
				} break;
				default:
				{
					//ErrorBox("While searching for an object's first sprite's\nID, an unknown script opcode was encountered.\n\nCannot continue search.","ObjectMAP::AnimationScriptSearcher");
					exit--;
				}
			}
		}
	}
	/*Hex32 delfrms = ini_gethex("AnimationPopBackFrames", int2hexstr(object_ani,4),0);
	for(Hex32 i = 0; i < delfrms; ++i)
	{
		if(qAnimations[object_ani]->GetNumFrames())
		{
			qAnimations[object_ani]->DelFrame(
				qAnimations[object_ani]->GetNumFrames() - 1);
		}
	}
	Hex32 revfrms = ini_gethex("AnimationShowReverseFrames", int2hexstr(object_ani,4),0);
	Hex32 numframes = qAnimations[object_ani]->GetNumFrames();
	for(Hex32 i = 0; i < revfrms; ++i)
	{
		if(qAnimations[object_ani]->HasDualFrames())
		{
			qAnimations[object_ani]->AddFrameDual(
				qAnimations[object_ani]->GetFrame(numframes - 1 - i), 
				qAnimations[object_ani]->GetFrame2(numframes - 1 - i), 
				qAnimations[object_ani]->GetDelay(numframes - 1 - i),
				qAnimations[object_ani]->DualOrderInvert());
		}
		else
		{
			qAnimations[object_ani]->AddFrame(
				qAnimations[object_ani]->GetFrame(numframes - 1 - i), 
				qAnimations[object_ani]->GetDelay(numframes - 1 - i));
		}
	}*/
	return anim; 
}
CAnimation* PreRenderSpriteWithPaletteDKC1(Hex32 object_spr, Hex32 object_pal, Hex16 word)
{
	safe_array(Hex08,GfxID,0x01, 0x01, 
	((object_spr >> 16) & 0xFF), ((object_spr >> 8) & 0xFF), (object_spr & 0xFF),
	((object_pal >> 16) & 0xFF), ((object_pal >> 8) & 0xFF), (object_pal & 0xFF));
	
	CAnimation* anim = Animation(GfxID);
	if(anim->GetLength()) return anim; //If the animation already has frames, we had a cached copy!
	
	Hex32 object_dly = 0x7FFFFFFE;
	if((object_spr > 0x2AF0)||(object_spr < 0x8C)) return anim; // Abort if out of range. Returns empty animation!
	Hex32 dualdisplaysprite = ini_gethex("SpriteDualFrames", int2hexstr(object_spr,4),0);
	Hex32 object_ddpal = ini_gethex("ObjectDualPalette", int2hexstr(word,4),0);
	if((dualdisplaysprite & 0xFFFF) && (object_ddpal != 0))
	{
		GfxID[1] |= 0x02; //Keep value range for dual sprites seperated!
		SRect rect;//gets rect border - passed to graphic to accelerate image cropping. order: L, T, R, B
		BITMAP* image = RE_DKC1_DualSprite(
			object_spr, (dualdisplaysprite & 0xFFFF), object_pal, (object_ddpal?object_ddpal:object_pal), 
			&rect, (dualdisplaysprite & ~0xFFFF));
		anim->AddFrame(Frame(Graphic(image,&rect,GfxID), 128, 128), object_dly);
	}
	else
	{
		SRect rect;//gets rect border - passed to graphic to accelerate image cropping. order: L, T, R, B
		BITMAP* image = RE_DKC1_Sprite(object_spr, object_pal, &rect);
		anim->AddFrame(Frame(Graphic(image,&rect,GfxID), 128, 128), object_dly);
	}
	return anim; 
}


