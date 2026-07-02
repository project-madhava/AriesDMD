#pragma once
#include <Arduino.h>
#include "AriesRGB_32x16.h"

// Define Adafruit GFX structures natively
typedef struct {
    uint16_t bitmapOffset;     // Pointer into GFXfont->bitmap
    uint8_t  width, height;    // Bitmap dimensions in pixels
    uint8_t  xAdvance;         // Distance to advance cursor (x axis)
    int8_t   xOffset, yOffset; // Distance from cursor pos to upper-left corner
} GFXglyph;

typedef struct {
    uint8_t  *bitmap;      // Glyph bitmaps, concatenated
    GFXglyph *glyph;       // Glyph array
    uint16_t  first, last; // ASCII extents
    uint8_t   yAdvance;    // Newline distance (y axis)
} GFXfont;

// Function declarations for the new engine
void AriesDrawCharGFX(AriesRGB_32x16& display, int x, int y, char c, bool r, bool g, bool b, const GFXfont *gfxFont);
int AriesDrawStringGFX(AriesRGB_32x16& display, int x, int y, const char* str, bool r, bool g, bool b, const GFXfont *gfxFont);
