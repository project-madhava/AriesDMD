#pragma once
#include <Arduino.h>

#define ARIES_FONT_WIDTH   5
#define ARIES_FONT_HEIGHT  7
#define ARIES_FONT_FIRST   32

extern const uint8_t System5x7[];

class AriesRGB_32x16;

void AriesDrawChar(
    AriesRGB_32x16& display,
    int x, int y,
    char c,
    bool r, bool g, bool b
);

void AriesDrawString(
    AriesRGB_32x16& display,
    int x, int y,
    const char* str,
    bool r, bool g, bool b
);
