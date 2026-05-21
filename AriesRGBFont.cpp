#include "AriesRGBFont.h"

AriesRGBFont::AriesRGBFont()
{
    fontData = nullptr;

    width = 0;
    height = 0;
    firstChar = 0;
    charCount = 0;
}

void AriesRGBFont::setFont(const uint8_t* f)
{
    fontData = f;

    width     = pgm_read_byte(&fontData[1]);
    height    = pgm_read_byte(&fontData[2]);
    firstChar = pgm_read_byte(&fontData[3]);
    charCount = pgm_read_byte(&fontData[4]);
}

bool AriesRGBFont::isCharValid(char c) const
{
    if (!fontData) return false;

    return (c >= firstChar) &&
           (c < (firstChar + charCount));
}

uint8_t AriesRGBFont::getCharWidth(char c) const
{
    if (!isCharValid(c)) return 0;

    return width;
}

uint8_t AriesRGBFont::getHeight() const
{
    return height;
}

uint8_t AriesRGBFont::getColumn(char c, uint8_t column) const
{
    if (!isCharValid(c)) return 0;

    if (column >= width) return 0;

    uint16_t index =
        5 +
        ((c - firstChar) * width) +
        column;

    return pgm_read_byte(&fontData[index]);
}
