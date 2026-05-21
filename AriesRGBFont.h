#ifndef ARIES_RGB_FONT_H
#define ARIES_RGB_FONT_H

#include <Arduino.h>

class AriesRGBFont
{
public:
    AriesRGBFont();

    void setFont(const uint8_t* f);

    bool isCharValid(char c) const;

    uint8_t getCharWidth(char c) const;

    uint8_t getHeight() const;

    uint8_t getColumn(char c, uint8_t column) const;

private:
    const uint8_t* fontData;

    uint8_t width;
    uint8_t height;
    uint8_t firstChar;
    uint8_t charCount;
};

#endif
