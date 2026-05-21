#include "AriesDMD.h"
#include "AriesFont5x7.h"
#include "AriesRGB_Font5x7.h"

void AriesDrawChar(
    AriesDMD& dmd,
    int x,
    int y,
    char c,
    uint8_t mode
)
{
    if(
        c < ARIES_FONT_FIRST ||
        c >= (ARIES_FONT_FIRST + 96)
    )
    {
        return;
    }

    uint16_t charIndex =
        c - ARIES_FONT_FIRST;

    uint16_t glyphIndex =
        5 +
        (charIndex * ARIES_FONT_WIDTH);

    for(uint8_t col = 0;
        col < ARIES_FONT_WIDTH;
        col++)
    {
        uint8_t line =
            pgm_read_byte(
                &System5x7[glyphIndex + col]
            );

        for(uint8_t row = 0;
            row < ARIES_FONT_HEIGHT;
            row++)
        {
            if(line & (1 << row))
            {
                dmd.writePixel(
                    x + col,
                    y + row,
                    mode,
                    true
                );
            }
        }
    }
}

void AriesDrawString(
    AriesDMD& dmd,
    int x,
    int y,
    const char* str,
    uint8_t mode
)
{
    while(*str)
    {
        AriesDrawChar(
            dmd,
            x,
            y,
            *str,
            mode
        );

        x += (ARIES_FONT_WIDTH + 1);

        str++;
    }
}
