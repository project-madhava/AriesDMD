#pragma once

#include <Arduino.h>

#define ARIES_FONT_WIDTH   5
#define ARIES_FONT_HEIGHT  7
#define ARIES_FONT_FIRST   32

#ifndef GRAPHICS_NORMAL
#define GRAPHICS_NORMAL 0
#endif

#ifndef GRAPHICS_INVERT
#define GRAPHICS_INVERT 1
#endif

extern const uint8_t System5x7[];

class AriesDMD;

void AriesDrawChar(
    AriesDMD& dmd,
    int x,
    int y,
    char c,
    uint8_t mode = GRAPHICS_NORMAL
);

void AriesDrawString(
    AriesDMD& dmd,
    int x,
    int y,
    const char* str,
    uint8_t mode = GRAPHICS_NORMAL
);
