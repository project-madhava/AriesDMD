#pragma once

#include <Arduino.h>
#include "AriesRGB_32x16.h"

// SCALED VERTICAL FONT RENDERER

void AriesDrawCharVertScaled(
    AriesRGB_32x16& display,
    int x,
    int y,
    char c,
    bool r,
    bool g,
    bool b,
    int scale
);

void AriesDrawStringVertScaled(
    AriesRGB_32x16& display,
    int x,
    int y,
    const char* str,
    bool r,
    bool g,
    bool b,
    int scale
);
