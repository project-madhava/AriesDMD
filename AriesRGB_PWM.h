#ifndef ARIES_RGB_PWM_H
#define ARIES_RGB_PWM_H

#include <Arduino.h>

#define PANEL_WIDTH   32
#define PANEL_HEIGHT  16
#define MAX_PANELS_X  20
#define MAX_PANELS_Y  5

struct RGBPixel { uint8_t r, g, b; };

class AriesRGB_PWM {
public:
    AriesRGB_PWM(uint8_t r1, uint8_t g1, uint8_t b1, uint8_t r2, uint8_t g2, uint8_t b2, uint8_t clk, uint8_t lat, uint8_t oe, uint8_t a, uint8_t b, uint8_t c, uint8_t pw, uint8_t ph);
    void begin();
    void clear();
    void drawPixel(int x, int y, uint8_t r, uint8_t g, uint8_t b);
    void refresh();
    uint16_t width();
    uint16_t height();
private:
    uint8_t PIN_R1, PIN_G1, PIN_B1, PIN_R2, PIN_G2, PIN_B2, PIN_CLK, PIN_LAT, PIN_OE, PIN_A, PIN_B, PIN_C;
    uint8_t panelsWide, panelsHigh;
    uint16_t totalWidth, totalHeight;
    uint8_t scanRow;
    RGBPixel framebuffer[MAX_PANELS_Y * PANEL_HEIGHT][MAX_PANELS_X * PANEL_WIDTH];
};
#endif
