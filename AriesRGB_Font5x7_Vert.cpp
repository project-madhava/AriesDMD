#include <Arduino.h>

//  Vertical-panel text rendering for AriesRGB_32x16.
//
//  PHYSICAL MOUNTING
//  ─────────────────
//  Rotate the 32×16 panel 90° COUNTER-CLOCKWISE so that:
//    • The long (32-pixel) edge becomes the vertical axis  → logical HEIGHT = 32
//    • The short (16-pixel) edge becomes the horizontal axis → logical WIDTH  = 16
//    • The DATA connector (normally at the bottom) now faces to the RIGHT.
//
//  Coordinate transform used internally (logical → physical):
//      phys_x  =  logical_y
//      phys_y  =  (PANEL_HEIGHT - 1) - logical_x   =  15 - logical_x
//
//  LOGICAL SPACE for a single 32×16 panel mounted vertically:
//      x : 0 … 15   (left → right as you look at it)
//      y : 0 … 31   (top  → bottom)
//
//  For multi-panel arrays (panelsWide > 1 etc.) scale accordingly.
// ─────────────────────────────────────────────────────────────────────────────

#define ARIES_VERT_LOG_WIDTH   16   
#define ARIES_VERT_LOG_HEIGHT  32   

class AriesRGB_32x16;   
void AriesDrawPixelVert(
    AriesRGB_32x16& display,
    int lx, int ly,
    bool r, bool g, bool b
);

void AriesDrawCharVert(
    AriesRGB_32x16& display,
    int x, int y,
    char c,
    bool r, bool g, bool b
);

void AriesDrawStringVert(
    AriesRGB_32x16& display,
    int x, int y,
    const char* str,
    bool r, bool g, bool b
);
