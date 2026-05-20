#include "AriesRGB_32x16.h"
#include "AriesRGB_Font5x7.h"       
#include "AriesRGB_Font5x7_Vert.h"  

AriesRGB_32x16 display(
    3, 4, 5,     // R1  G1  B1
    6, 7, 8,     // R2  G2  B2
    9, 10, 11,   // CLK LAT OE
    12, 13, 14,  // A   B   C
    2, 2         // panelsWide=1, panelsHigh=1
);

const char* msg = " HELLO Vertical";

int scrollX;

// Vertically centre the text row:
//   logical Y = (32 - 7) / 2 = 12
const int TEXT_Y = (ARIES_VERT_LOG_HEIGHT - ARIES_FONT_HEIGHT) / 2;

// Number of refresh() calls per frame — increase to slow down, decrease to speed up.
#define REFRESHES_PER_FRAME  400

void setup()
{
    display.begin();

    // Text starts just off the right edge of the logical display (x = 16).
    scrollX = ARIES_VERT_LOG_WIDTH;
}

void loop()
{
    display.clear();

    AriesDrawStringVert(display, scrollX, TEXT_Y, msg, 1, 1, 0);

    for (int i = 0; i < REFRESHES_PER_FRAME; i++)
    {
        display.refresh();
    }

    scrollX--;

    int textWidth = (int)strlen(msg) * (ARIES_FONT_WIDTH + 1);

    if (scrollX < -textWidth)
    {
        scrollX = ARIES_VERT_LOG_WIDTH;
    }
}
