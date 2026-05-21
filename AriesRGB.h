#ifndef ARIES_RGB_H
#define ARIES_RGB_H

#include <Arduino.h>
#include "AriesRGBFont.h"

#define PANEL_WIDTH  80
#define PANEL_HEIGHT 40

class AriesRGB
{
public:

    AriesRGB(
        uint8_t r1, uint8_t g1, uint8_t b1,
        uint8_t r2, uint8_t g2, uint8_t b2,
        uint8_t clk, uint8_t lat, uint8_t oe,
        uint8_t a, uint8_t b, uint8_t c,
        uint8_t d, uint8_t e,
        uint8_t panelsWide = 1,
        uint8_t panelsHigh = 1
    );

    ~AriesRGB();

    void begin();

    void clear();

    void drawPixel(
        int x,
        int y,
        uint8_t r,
        uint8_t g,
        uint8_t b
    );

    void refresh();

    uint16_t width()  const { return totalWidth;  }
    uint16_t height() const { return totalHeight; }

    // TEXT
    void setCursor(int x, int y);

    void setTextColor(
        uint8_t r,
        uint8_t g,
        uint8_t b
    );

    void setTextSize(uint8_t s);

    void setFont(const uint8_t* f);

    void print(const char* str);

private:

    uint8_t panelsWide;
    uint8_t panelsHigh;

    uint16_t totalWidth;
    uint16_t totalHeight;

    uint8_t PIN_R1, PIN_G1, PIN_B1;
    uint8_t PIN_R2, PIN_G2, PIN_B2;

    uint8_t PIN_CLK;
    uint8_t PIN_LAT;
    uint8_t PIN_OE;

    uint8_t PIN_A;
    uint8_t PIN_B;
    uint8_t PIN_C;
    uint8_t PIN_D;
    uint8_t PIN_E;

    // DYNAMIC FRAMEBUFFER
    uint8_t* framebuffer;

    uint8_t scanRow;

    void initDriver();

    inline uint32_t pixelIndex(
        int x,
        int y,
        int color
    ) const
    {
        return ((y * totalWidth) + x) * 3 + color;
    }

    // TEXT
    int cursor_x = 0;
    int cursor_y = 0;

    uint8_t text_r = 1;
    uint8_t text_g = 1;
    uint8_t text_b = 1;

    uint8_t text_size = 1;

    AriesRGBFont font;
};

#endif
