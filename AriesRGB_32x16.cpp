
#include "AriesRGB_32x16.h"

// DIRECT HARDWARE MEMORY MAP (Derived from VEGA gpio.c)
#define VEGA_GPIO_0_BASE 0x10080000UL
#define VEGA_GPIO_1_BASE 0x10180000UL

volatile unsigned short* fastPort[32];
unsigned short fastMask[32];

inline void fastDigitalWrite(uint8_t pin, bool val) 
{
    if (val) *(fastPort[pin]) = fastMask[pin];
    else     *(fastPort[pin]) = 0;
    
    // Crucial RISC-V instruction to force the memory out to the physical pins
    __asm__ __volatile__ ("fence"); 
}

AriesRGB_32x16::AriesRGB_32x16(
    uint8_t r1, uint8_t g1, uint8_t b1,
    uint8_t r2, uint8_t g2, uint8_t b2,
    uint8_t clk, uint8_t lat, uint8_t oe,
    uint8_t a, uint8_t b, uint8_t c,
    uint8_t pw, uint8_t ph)
{
    panelsWide = constrain(pw, 1, MAX_PANELS_X);
    panelsHigh = constrain(ph, 1, MAX_PANELS_Y);
    totalWidth  = panelsWide  * PANEL_WIDTH;
    totalHeight = panelsHigh * PANEL_HEIGHT;

    PIN_R1 = r1; PIN_G1 = g1; PIN_B1 = b1;
    PIN_R2 = r2; PIN_G2 = g2; PIN_B2 = b2;
    PIN_CLK = clk; PIN_LAT = lat; PIN_OE  = oe;
    PIN_A = a; PIN_B = b; PIN_C = c;

    scanRow = 0;
    clear();
}

void AriesRGB_32x16::begin()
{
    pinMode(PIN_R1, OUTPUT); pinMode(PIN_G1, OUTPUT); pinMode(PIN_B1, OUTPUT);
    pinMode(PIN_R2, OUTPUT); pinMode(PIN_G2, OUTPUT); pinMode(PIN_B2, OUTPUT);
    pinMode(PIN_CLK, OUTPUT); pinMode(PIN_LAT, OUTPUT); pinMode(PIN_OE, OUTPUT);
    pinMode(PIN_A, OUTPUT); pinMode(PIN_B, OUTPUT); pinMode(PIN_C, OUTPUT);

    for(int p = 0; p < 32; p++) 
    {
        if (p <= 15) {
            uint32_t bit_pos = (1 << p);
            fastPort[p] = (volatile unsigned short*)(VEGA_GPIO_0_BASE + (bit_pos << 2));
            fastMask[p] = bit_pos;
        } else {
            uint32_t bit_pos = (1 << (p - 16));
            fastPort[p] = (volatile unsigned short*)(VEGA_GPIO_1_BASE + (bit_pos << 2));
            fastMask[p] = bit_pos;
        }
    }

    clear();
}

void AriesRGB_32x16::clear()
{
    memset(framebuffer, 0, sizeof(framebuffer));
}

void AriesRGB_32x16::drawPixel(int x, int y, bool r, bool g, bool b)
{
    if(x < 0 || x >= totalWidth) return;
    if(y < 0 || y >= totalHeight) return;

    framebuffer[y][x][0] = r;
    framebuffer[y][x][1] = g;
    framebuffer[y][x][2] = b;
}

void AriesRGB_32x16::refresh()
{
    fastDigitalWrite(PIN_OE, HIGH);

    fastDigitalWrite(PIN_A, (scanRow >> 0) & 1);
    fastDigitalWrite(PIN_B, (scanRow >> 1) & 1);
    fastDigitalWrite(PIN_C, (scanRow >> 2) & 1);

    for(int pCol = panelsWide - 1; pCol >= 0; pCol--)
    {
        int panelBaseX = pCol * PANEL_WIDTH;
        
        for(int pRow = panelsHigh - 1; pRow >= 0; pRow--)
        {
            int panelBaseY = pRow * PANEL_HEIGHT;

            int y1 = panelBaseY + scanRow;
            int y2 = panelBaseY + scanRow + 8;

            bool* row1_ptr = &framebuffer[y1][panelBaseX][0];
            bool* row2_ptr = &framebuffer[y2][panelBaseX][0];

            for(int col = 0; col < PANEL_WIDTH; col++)
            {
                fastDigitalWrite(PIN_R1, *(row1_ptr++));
                fastDigitalWrite(PIN_G1, *(row1_ptr++));
                fastDigitalWrite(PIN_B1, *(row1_ptr++));

                fastDigitalWrite(PIN_R2, *(row2_ptr++));
                fastDigitalWrite(PIN_G2, *(row2_ptr++));
                fastDigitalWrite(PIN_B2, *(row2_ptr++));

                fastDigitalWrite(PIN_CLK, HIGH);
                fastDigitalWrite(PIN_CLK, LOW);
            }
        }
    }

    fastDigitalWrite(PIN_LAT, HIGH);
    fastDigitalWrite(PIN_LAT, LOW);

    fastDigitalWrite(PIN_OE, LOW);
    
    // If the screen is dim, increase this to 500 or 800
    delayMicroseconds(300); 
    
    fastDigitalWrite(PIN_OE, HIGH);

    scanRow++;
    if(scanRow >= 8) scanRow = 0;
}

uint16_t AriesRGB_32x16::width() { return totalWidth; }
uint16_t AriesRGB_32x16::height() { return totalHeight; }
