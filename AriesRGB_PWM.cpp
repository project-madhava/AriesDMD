#include "AriesRGB_PWM.h"

// Hardware Register base for THEJAS32 (GPIO 0)
#define VEGA_GPIO_0_BASE 0x10080000UL

// Inline helper for raw register access
inline void fastWrite(uint8_t pin, bool val) {
    volatile unsigned short* port = (volatile unsigned short*)(VEGA_GPIO_0_BASE + ((1 << pin) << 2));
    if (val) *port = (1 << pin);
    else     *port = 0;
    __asm__ __volatile__ ("fence");
}

AriesRGB_PWM::AriesRGB_PWM(uint8_t r1, uint8_t g1, uint8_t b1, uint8_t r2, uint8_t g2, uint8_t b2, uint8_t clk, uint8_t lat, uint8_t oe, uint8_t a, uint8_t b, uint8_t c, uint8_t pw, uint8_t ph) {
    panelsWide = constrain(pw, 1, MAX_PANELS_X);
    panelsHigh = constrain(ph, 1, MAX_PANELS_Y);
    totalWidth = panelsWide * PANEL_WIDTH;
    totalHeight = panelsHigh * PANEL_HEIGHT;
    PIN_R1=r1; PIN_G1=g1; PIN_B1=b1; PIN_R2=r2; PIN_G2=g2; PIN_B2=b2;
    PIN_CLK=clk; PIN_LAT=lat; PIN_OE=oe; PIN_A=a; PIN_B=b; PIN_C=c;
    scanRow = 0;
    clear();
}

void AriesRGB_PWM::begin() {
    uint8_t pins[] = {PIN_R1, PIN_G1, PIN_B1, PIN_R2, PIN_G2, PIN_B2, PIN_CLK, PIN_LAT, PIN_OE, PIN_A, PIN_B, PIN_C};
    for(int i=0; i<12; i++) pinMode(pins[i], OUTPUT);
    clear();
}

void AriesRGB_PWM::clear() { memset(framebuffer, 0, sizeof(framebuffer)); }

void AriesRGB_PWM::drawPixel(int x, int y, uint8_t r, uint8_t g, uint8_t b) {
    if(x < 0 || x >= totalWidth || y < 0 || y >= totalHeight) return;
    framebuffer[y][x] = {r, g, b};
}

void AriesRGB_PWM::refresh() {
    for(uint8_t bit = 0; bit < 4; bit++) {
        fastWrite(PIN_OE, HIGH);
        fastWrite(PIN_A, (scanRow >> 0) & 1);
        fastWrite(PIN_B, (scanRow >> 1) & 1);
        fastWrite(PIN_C, (scanRow >> 2) & 1);

        for(int py = 0; py < panelsHigh; py++) {
            for(int px = 0; px < panelsWide; px++) {
                for(int col = 0; col < PANEL_WIDTH; col++) {
                    int x = (px * PANEL_WIDTH) + col;
                    RGBPixel p1 = framebuffer[py * PANEL_HEIGHT + scanRow][x];
                    RGBPixel p2 = framebuffer[py * PANEL_HEIGHT + scanRow + 8][x];

                    fastWrite(PIN_R1, (p1.r >> (bit + 4)) & 1);
                    fastWrite(PIN_G1, (p1.g >> (bit + 4)) & 1);
                    fastWrite(PIN_B1, (p1.b >> (bit + 4)) & 1);
                    fastWrite(PIN_R2, (p2.r >> (bit + 4)) & 1);
                    fastWrite(PIN_G2, (p2.g >> (bit + 4)) & 1);
                    fastWrite(PIN_B2, (p2.b >> (bit + 4)) & 1);

                    fastWrite(PIN_CLK, HIGH);
                    fastWrite(PIN_CLK, LOW);
                }
            }
        }
        fastWrite(PIN_LAT, HIGH);
        fastWrite(PIN_LAT, LOW);
        fastWrite(PIN_OE, LOW);
        delayMicroseconds(50 * (1 << bit));
        fastWrite(PIN_OE, HIGH);
    }
    scanRow = (scanRow + 1) % 8;
}
