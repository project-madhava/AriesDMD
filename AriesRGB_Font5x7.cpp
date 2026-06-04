#include "AriesRGB_Font5x7_Vert.h"
#include "AriesRGB_32x16.h"
#include "AriesRGB_Font5x7.h"
#include <avr/pgmspace.h>

void AriesDrawPixelVert(
    AriesRGB_32x16& display,
    int lx, int ly,
    bool r, bool g, bool b)
{
    int logWidth = (display.width() / 32) * 16;
    int logHeight = (display.height() / 16) * 32;

    while(lx < 0) lx += logWidth;
    lx = lx % logWidth;

    if (ly < 0 || ly >= logHeight) return;

    int maxCol = (logWidth / 16) - 1;
    int panelCol = maxCol - (lx / 16); 
    
    int panelRow = ly / 32;     

    int localX = lx % 16;
    int localY = ly % 32;

    int px = (panelCol * 32) + (31 - localY);
    int py = (panelRow * 16) + (15 - localX);

    display.drawPixel(px, py, r, g, b);
}

#define FONT_HEADER_SIZE 6

void AriesDrawCharVert(
    AriesRGB_32x16& display,
    int x, int y, char c,
    bool r, bool g, bool b)
{
    if (c < ARIES_FONT_FIRST || c >= (ARIES_FONT_FIRST + 96)) return;

    uint16_t charIndex = (uint16_t)(c - ARIES_FONT_FIRST);
    uint16_t glyphIndex = FONT_HEADER_SIZE + (charIndex * ARIES_FONT_WIDTH);

    for(uint8_t col = 0; col < ARIES_FONT_WIDTH; col++)
    {
        uint8_t line = pgm_read_byte(&System5x7[glyphIndex + col]);
        for(uint8_t row = 0; row < ARIES_FONT_HEIGHT; row++)
        {
            if(line & (1 << row))
            {
                // UN-MIRROR X, KEEP Y UPRIGHT
                int drawX = x + col;
                int drawY = y + (ARIES_FONT_HEIGHT - 1 - row);
                
                AriesDrawPixelVert(display, drawX, drawY, r, g, b);
            }
        }
    }
}

void AriesDrawStringVert(
    AriesRGB_32x16& display,
    int x, int y, const char* str,
    bool r, bool g, bool b)
{
    while(*str)
    {
        AriesDrawCharVert(display, x, y, *str, r, g, b);
        x += (ARIES_FONT_WIDTH + 1);
        str++;
    }
}
