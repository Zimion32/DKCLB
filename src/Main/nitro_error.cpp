#include "include.h"
#include <csignal>
safe_array(SErrorDesc, ErrorDescriptions,
{"No Error",                        "No error was reported."},
{"INIT_RegisterClassEx",            "Failed to register the window class."},
{"INIT_CreateWindowEx",             "Failed to create the main window."},
{"INIT_install_allegro",            "There was a problem initializing allegro!!"},
{"INIT_InputInit",                  "There was a problem initializing the input routines!!"},
{"VIDEO_set_gfx_mode",              "The graphics mode that has been requested is not available."},
{"VIDEO_create_bitmap_ex",          "Out of memory trying to allocate the GUI framebuffer!!"},
{"VIDEO_create_video_bitmap",       "Out of video memory trying to allocate video pages!!"},
{"VIDEO_show_video_bitmap",         "Failed to show the current video page."},
{"VIDEO_get_refresh_rate",          "An unknown rate (zero) was reported to the video routines."},
{"VIDEO_bitmap_color_depth",        "The screen bitmap doesn't have the proper 32-bit color depth."},
{"NGUI_failed_to_load",             "There was a problem loading NitroGUI!!"},
{"LAST",                            "No error was reported."}
);

TXT abort_reason = "An unknown part of the program called abort().";
void BegAbortProtect(TXT why)
{
    abort_reason = why;
}
void EndAbortProtect()
{
    abort_reason = "An unknown part of the program called abort().";
}
