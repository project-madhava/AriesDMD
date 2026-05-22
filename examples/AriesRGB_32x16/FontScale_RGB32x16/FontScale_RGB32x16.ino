#include "AriesRGB_32x16.h"

#include "AriesRGB_Font5x7.h"
#include "AriesRGB_Font5x7_Vert.h"

#include "AriesRGB_Font5x7_VertScaled.h"

// SINGLE 32x16 PANEL TEST
// PANEL ROTATED VERTICALLY

AriesRGB_32x16 display(
    3, 4, 5,      // R1 G1 B1
    6, 7, 8,      // R2 G2 B2
    9, 10, 11,    // CLK LAT OE
    12, 13, 14,   // A B C

    1, 1          // 1 panel wide, 1 panel high
);

void setup()
{
    display.begin();
}

void loop()
{
    display.clear();

    
    // SMALL NORMAL TEXT
    AriesDrawStringVert(
        display,
        0,
        0,
        "S",
        0, 1, 0      // Green
    );

    
    // BIG SCALED TEXT
    AriesDrawStringVertScaled(
        display,
        0,
        12,
        "L",
        1, 0, 0,     // Red
        2             // SCALE = 2
    );

    for(int i = 0; i < 500; i++)
    {
        display.refresh();
    }
}
