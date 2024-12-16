#pragma once
namespace Error{
enum Enum{
    NONE = 0,
    INIT_RegisterClassEx,
    INIT_CreateWindowEx,
    INIT_install_allegro,
    INIT_InputInit,
    VIDEO_set_gfx_mode,
    VIDEO_create_bitmap_ex,
    VIDEO_create_video_bitmap,
    VIDEO_show_video_bitmap,
    VIDEO_get_refresh_rate,
    VIDEO_bitmap_color_depth,
    NGUI_failed_to_load
};}
struct SErrorDesc
{
    STR text;
    STR desc;
};
extern Vx<SErrorDesc> ErrorDescriptions;
void WinAbort(int signum);
void BegAbortProtect(TXT why);
void EndAbortProtect();
void SetWinAbort();
#define ERR Error::Enum
