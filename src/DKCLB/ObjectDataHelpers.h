#pragma once
class CAnimation;
struct SPreRenderCommand;
Vsx32 FindObjectPaletteDKC1(Hex16 word);
Vsx32 FindObjectAnimationDKC1(Hex16 word);
CAnimation* PreRenderAnimationWithPaletteDKC1(Hex32 object_ani, Hex32 object_pal, Hex16 word);
CAnimation* PreRenderSpriteWithPaletteDKC1(Hex32 object_spr, Hex32 object_pal, Hex16 word);
void Execute_PreRenderCommand(SPreRenderCommand* cmd);
