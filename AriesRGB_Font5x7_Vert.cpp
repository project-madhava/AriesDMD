
#include "AriesRGB_Font5x7_Vert.h"
#include "AriesRGB_32x16.h"
#include "AriesRGB_Font5x7.h"
#include <avr/pgmspace.h>

// =======================================================
// THE CYLINDER MAPPING - 100% UNTOUCHED
// =======================================================
void AriesDrawPixelVert(
    AriesRGB_32x16& display,
    int lx, int ly,
    bool r, bool g, bool b)
{
    int logWidth = (display.width() / 32) * 16;
    int logHeight = (display.height() / 16) * 32;

    // Seamless horizontal wrap-around
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

// =======================================================
// FONT RENDERING
// =======================================================
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

// =======================================================
// IMAGE RENDERING: 8-Color (3-bit) Bitmap Support
// Format: 1 byte per pixel -> 0b00000RGB
// =======================================================
void AriesDrawBitmapVert(
    AriesRGB_32x16& display,
    int x, int y,
    const uint8_t *bitmap,
    int w, int h)
{
    int byteIndex = 0;
    
    // Loop through every row of the image
    for(int j = 0; j < h; j++) 
    {
        // Loop through every column of the image
        for(int i = 0; i < w; i++) 
        {
            uint8_t pixel = bitmap[byteIndex++];
            
            // Extract the RGB bits from the byte
            bool r = (pixel >> 2) & 0x01; // Bit 2
            bool g = (pixel >> 1) & 0x01; // Bit 1
            bool b = (pixel >> 0) & 0x01; // Bit 0
            
            // Only draw the pixel if it is NOT completely black (0,0,0)
            // This creates natural transparency around logos!
            if (r || g || b) 
            {
                // Push it through the cylinder mapping engine
                AriesDrawPixelVert(display, x + i, y + j, r, g, b);
            }
        }
    }
}
