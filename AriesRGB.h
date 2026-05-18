/*
#ifndef ARIES_RGB_H
#define ARIES_RGB_H

#include <Arduino.h>

#define PANEL_WIDTH  80
#define PANEL_HEIGHT 40

#ifndef MAX_PANELS_X
  #define MAX_PANELS_X 5
#endif

#ifndef MAX_PANELS_Y
  #define MAX_PANELS_Y 3
#endif

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

    void begin();
    void clear();
    void drawPixel(int x, int y, uint8_t r, uint8_t g, uint8_t b);
    void refresh();

    uint16_t width()  const { return totalWidth;  }
    uint16_t height() const { return totalHeight; }

private:
    uint8_t panelsWide;
    uint8_t panelsHigh;

    uint16_t totalWidth;
    uint16_t totalHeight;

    uint8_t PIN_R1, PIN_G1, PIN_B1;
    uint8_t PIN_R2, PIN_G2, PIN_B2;
    uint8_t PIN_CLK, PIN_LAT, PIN_OE;
    uint8_t PIN_A, PIN_B, PIN_C, PIN_D, PIN_E;

    uint8_t framebuffer[MAX_PANELS_Y * PANEL_HEIGHT]
                        [MAX_PANELS_X * PANEL_WIDTH][3];

    uint8_t scanRow;

    void initDriver();
};

#endif
*/

#ifndef ARIES_RGB_H
#define ARIES_RGB_H

#include <Arduino.h>
#include "AriesRGBFont.h"

#define PANEL_WIDTH  80
#define PANEL_HEIGHT 40

#ifndef MAX_PANELS_X
  #define MAX_PANELS_X 20
#endif

#ifndef MAX_PANELS_Y
  #define MAX_PANELS_Y 5
#endif

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

    void begin();
    void clear();
    void drawPixel(int x, int y, uint8_t r, uint8_t g, uint8_t b);
    void refresh();

    uint16_t width()  const { return totalWidth;  }
    uint16_t height() const { return totalHeight; }

    // TEXT SYSTEM 
    void setCursor(int x, int y);
    void setTextColor(uint8_t r, uint8_t g, uint8_t b);
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
    uint8_t PIN_CLK, PIN_LAT, PIN_OE;
    uint8_t PIN_A, PIN_B, PIN_C, PIN_D, PIN_E;

    uint8_t framebuffer[MAX_PANELS_Y * PANEL_HEIGHT]
                        [MAX_PANELS_X * PANEL_WIDTH][3];

    uint8_t scanRow;

    void initDriver();

    // TEXT
    int cursor_x = 0;
    int cursor_y = 0;
    uint8_t text_r = 1, text_g = 1, text_b = 1;
    uint8_t text_size = 1;
    AriesRGBFont font;
};

#endif