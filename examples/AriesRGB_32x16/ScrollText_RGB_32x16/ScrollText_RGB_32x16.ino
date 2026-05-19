#include "AriesRGB_32x16.h"
#include "AriesRGB_Font5x7.h"

AriesRGB_32x16 display(
    3,4,5,  // R1, B1, G1
    6,7,8,  // R2, B2, G2
    9,10,11, // CLK, LAT, OE
    12,13,14, // A, B, C
    2,1 // PanelWidth, PanelHeight
);

const char* msg = "Jay Shree Ram";

int scrollX;

void setup()
{
    display.begin();

    scrollX = display.width();
}

void loop()
{
    display.clear();

    AriesDrawString(display, scrollX, 4, msg, 1, 1, 0);

    for(int i=0;i<120;i++)
    {
        display.refresh();
    }

    scrollX--;

    int textWidth = strlen(msg) * 6;

    if(scrollX < -textWidth)
    {
        scrollX = display.width();
    }
}
