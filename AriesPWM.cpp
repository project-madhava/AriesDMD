#include "AriesPWM.h"

#define VEGA_GPIO_0_BASE 0x10080000UL
#define VEGA_GPIO_1_BASE 0x10180000UL

volatile unsigned short* pwmPort[32];
unsigned short pwmMask[32];

inline void pwmDigitalWrite(uint8_t pin, bool val) {
    if (val) *(pwmPort[pin]) = pwmMask[pin];
    else     *(pwmPort[pin]) = 0;
    __asm__ __volatile__ ("fence"); 
}

AriesPWM_Matrix::AriesPWM_Matrix(uint8_t r1, uint8_t g1, uint8_t b1, uint8_t r2, uint8_t g2, uint8_t b2,
                                 uint8_t clk, uint8_t lat, uint8_t oe, uint8_t a, uint8_t b, uint8_t c) {
    PIN_R1 = r1; PIN_G1 = g1; PIN_B1 = b1; PIN_R2 = r2; PIN_G2 = g2; PIN_B2 = b2;
    PIN_CLK = clk; PIN_LAT = lat; PIN_OE = oe; PIN_A = a; PIN_B = b; PIN_C = c;
    scanRow = 0;
}

void AriesPWM_Matrix::begin() {
    uint8_t pins[] = {PIN_R1, PIN_G1, PIN_B1, PIN_R2, PIN_G2, PIN_B2, PIN_CLK, PIN_LAT, PIN_OE, PIN_A, PIN_B, PIN_C};
    for(int i=0; i<12; i++) pinMode(pins[i], OUTPUT);
    for(int p=0; p<32; p++) {
        if (p <= 15) { pwmPort[p] = (volatile unsigned short*)(VEGA_GPIO_0_BASE + ((1<<p)<<2)); pwmMask[p] = (1<<p); } 
        else         { pwmPort[p] = (volatile unsigned short*)(VEGA_GPIO_1_BASE + ((1<<(p-16))<<2)); pwmMask[p] = (1<<(p-16)); }
    }
    clear();
}

void AriesPWM_Matrix::clear() { memset(framebuffer, 0, sizeof(framebuffer)); }

void AriesPWM_Matrix::drawPixelVert(int lx, int ly, uint8_t r4, uint8_t g4, uint8_t b4) {
    while(lx < 0) lx += 320; lx = lx % 320;
    if (ly < 0 || ly >= 160) return;
    int px = ((19 - (lx / 16)) * 32) + (31 - (ly % 32));
    int py = ((ly / 32) * 16) + (15 - (lx % 16));
    framebuffer[py][px] = (r4 << 8) | (g4 << 4) | b4;
}

void AriesPWM_Matrix::refreshBAM() {
    pwmDigitalWrite(PIN_OE, HIGH);
    pwmDigitalWrite(PIN_A, (scanRow >> 0) & 1);
    pwmDigitalWrite(PIN_B, (scanRow >> 1) & 1);
    pwmDigitalWrite(PIN_C, (scanRow >> 2) & 1);

    for(uint8_t bit = 0; bit < 4; bit++) {
        for(int pCol = 19; pCol >= 0; pCol--) {
            int panelBaseX = pCol * 32;
            for(int pRow = 4; pRow >= 0; pRow--) {
                int y1 = (pRow * 16) + scanRow;
                int y2 = y1 + 8;
                for(int col = 0; col < 32; col++) {
                    uint16_t c1 = framebuffer[y1][panelBaseX + col];
                    uint16_t c2 = framebuffer[y2][panelBaseX + col];
                    pwmDigitalWrite(PIN_R1, (c1 >> (8 + bit)) & 1); pwmDigitalWrite(PIN_G1, (c1 >> (4 + bit)) & 1); pwmDigitalWrite(PIN_B1, (c1 >> (0 + bit)) & 1);
                    pwmDigitalWrite(PIN_R2, (c2 >> (8 + bit)) & 1); pwmDigitalWrite(PIN_G2, (c2 >> (4 + bit)) & 1); pwmDigitalWrite(PIN_B2, (c2 >> (0 + bit)) & 1);
                    pwmDigitalWrite(PIN_CLK, HIGH); pwmDigitalWrite(PIN_CLK, LOW);
                }
            }
        }
        pwmDigitalWrite(PIN_LAT, HIGH); pwmDigitalWrite(PIN_LAT, LOW);
        pwmDigitalWrite(PIN_OE, LOW);
        delayMicroseconds(1 << bit);
        pwmDigitalWrite(PIN_OE, HIGH);
    }
    scanRow = (scanRow + 1) % 8;
}
