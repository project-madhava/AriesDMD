#pragma once
#include <Arduino.h>

class AriesPWM_Matrix {
public:
    AriesPWM_Matrix(uint8_t r1, uint8_t g1, uint8_t b1, uint8_t r2, uint8_t g2, uint8_t b2,
                    uint8_t clk, uint8_t lat, uint8_t oe, uint8_t a, uint8_t b, uint8_t c);
    void begin();
    void clear();
    void drawPixelVert(int lx, int ly, uint8_t r4, uint8_t g4, uint8_t b4);
    void refreshBAM();

private:
    uint8_t PIN_R1, PIN_G1, PIN_B1, PIN_R2, PIN_G2, PIN_B2;
    uint8_t PIN_CLK, PIN_LAT, PIN_OE, PIN_A, PIN_B, PIN_C;
    uint8_t scanRow;
    uint16_t framebuffer[160][320]; 
};
