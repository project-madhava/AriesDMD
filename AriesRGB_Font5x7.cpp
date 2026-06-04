#include "AriesRGB_Font5x7.h"
#include "AriesRGB_32x16.h"
#include <avr/pgmspace.h>

#define FONT_HEADER_SIZE 6

void AriesDrawChar(
    AriesRGB_32x16& display,
    int x, int y,
    char c,
    bool r, bool g, bool b
)
{
    if (c < ARIES_FONT_FIRST || c >= (ARIES_FONT_FIRST + 96))
        return;

    uint16_t charIndex  = c - ARIES_FONT_FIRST;
    uint16_t glyphIndex = FONT_HEADER_SIZE + (charIndex * ARIES_FONT_WIDTH);

    for (uint8_t col = 0; col < ARIES_FONT_WIDTH; col++)
    {
        uint8_t line = pgm_read_byte(&System5x7[glyphIndex + col]);

        for (uint8_t row = 0; row < ARIES_FONT_HEIGHT; row++)
        {
            if (line & (1 << row))
            {
                display.drawPixel(x + col, y + row, r, g, b);
            }
        }
    }
}

void AriesDrawString(
    AriesRGB_32x16& display,
    int x, int y,
    const char* str,
    bool r, bool g, bool b
)
{
    while (*str)
    {
        AriesDrawChar(display, x, y, *str, r, g, b);

        x += (ARIES_FONT_WIDTH + 1);

        str++;
    }
}
