#include "AriesRGB_32x16.h"

AriesRGB_32x16 display(3, 4, 5, // R1, B1, G1
                       6, 7, 8, // R2, B2, G2
                       9, 10, 11, // Clk, Lat, OE
                       12, 13, 14, // A, B, C
                       2, 1); // Panel Width, Panel Height 

int x = 0;
int y = 4;

void setup()
{
    display.begin();
}

void loop()
{
    display.clear();

 // Adjust Size of block from here. Currently Block size is 10*10
    for (int i = 0; i < 10; i++)
    {
        for (int j = 0; j < 10; j++)
        {
            display.drawPixel(x + i, y + j, 1, 1, 0);
        }
    }

    for (int i = 0; i < 100; i++) // Adjust Movement Speed from here
    {
        display.refresh();
    }

    x++;
    if (x > display.width() - 4) x = 0;
}
