#include "AriesRGB_32x16.h"
#include "AriesRGB_Font5x7.h"

AriesRGB_32x16 display(3, 4, 5, // R1, B1, G1
                       6, 7, 8, // R2, B2, G2
                       9, 10, 11, // Clk, Lat, OE
                       12, 13, 14, // A, B, C
                       2, 1); // Panel Width, Panel Height 


void setup()
{
    display.begin();
}

void loop()
{
    display.clear();

    AriesDrawString(display, 1, 0, "Project", 1, 1, 0);
    AriesDrawString(display, 0, 8, "Madhava", 1, 0, 1);

    for(int i=0;i<150;i++)
    {
        display.refresh();
    }
}
