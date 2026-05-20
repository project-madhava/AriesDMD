#include <AriesRGB.h>

AriesRGB panel(
    3, 4, 5, // R0 G0 B0
    6, 7, 8, // R1 G1 B1 
    14, 15, 16, //CLK LAT OE 
    10, 11, 12, 13, 9, // A B C D E 
    2,   // panelsWide
    2    // panelsHigh 
);

int x = 0;
int y = 0;

int dx = 1;
int dy = 0;

const int SIZE = 6;

void drawBlock(int x, int y, uint8_t r, uint8_t g, uint8_t b)
{
    for (int i = 0; i < SIZE; i++)
        for (int j = 0; j < SIZE; j++)
            panel.drawPixel(x + i, y + j, r, g, b);
}

void setup()
{
    panel.begin();
}

void loop()
{
    panel.clear();

    // draw moving block
    drawBlock(x, y, 1, 0, 0);

    // movement logic (rectangle border)
    int maxX = panel.width()  - SIZE;
    int maxY = panel.height() - SIZE;

    if (dx == 1 && x >= maxX) { dx = 0; dy = 1; }
    else if (dy == 1 && y >= maxY) { dx = -1; dy = 0; }
    else if (dx == -1 && x <= 0) { dx = 0; dy = -1; }
    else if (dy == -1 && y <= 0) { dx = 1; dy = 0; }

    x += dx;
    y += dy;

    for (int i = 0; i < 200; i++)
        panel.refresh();
}
