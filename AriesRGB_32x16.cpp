#include "AriesRGB_32x16.h"

AriesRGB_32x16::AriesRGB_32x16(
    uint8_t r1, uint8_t g1, uint8_t b1,
    uint8_t r2, uint8_t g2, uint8_t b2,
    uint8_t clk, uint8_t lat, uint8_t oe,
    uint8_t a, uint8_t b, uint8_t c,
    uint8_t pw, uint8_t ph)
{
    panelsWide  = constrain(pw, 1, MAX_PANELS_X);
    panelsHigh  = constrain(ph, 1, MAX_PANELS_Y);

    totalWidth  = panelsWide  * PANEL_WIDTH;
    totalHeight = panelsHigh  * PANEL_HEIGHT;

    PIN_R1=r1; PIN_G1=g1; PIN_B1=b1;
    PIN_R2=r2; PIN_G2=g2; PIN_B2=b2;

    PIN_CLK=clk; PIN_LAT=lat; PIN_OE=oe;
    PIN_A=a; PIN_B=b; PIN_C=c;

    scanRow = 0;
    clear();
}

void AriesRGB_32x16::begin()
{
    pinMode(PIN_R1,OUTPUT); pinMode(PIN_G1,OUTPUT); pinMode(PIN_B1,OUTPUT);
    pinMode(PIN_R2,OUTPUT); pinMode(PIN_G2,OUTPUT); pinMode(PIN_B2,OUTPUT);

    pinMode(PIN_CLK,OUTPUT);
    pinMode(PIN_LAT,OUTPUT);
    pinMode(PIN_OE,OUTPUT);

    pinMode(PIN_A,OUTPUT);
    pinMode(PIN_B,OUTPUT);
    pinMode(PIN_C,OUTPUT);
}

void AriesRGB_32x16::clear()
{
    memset(framebuffer, 0, sizeof(framebuffer));
}

void AriesRGB_32x16::drawPixel(int x,int y,bool r,bool g,bool b)
{
    if(x<0||x>=totalWidth||y<0||y>=totalHeight) return;

    framebuffer[y][x][0]=r;
    framebuffer[y][x][1]=g;
    framebuffer[y][x][2]=b;
}

void AriesRGB_32x16::refresh()
{
    digitalWrite(PIN_OE,HIGH);

    digitalWrite(PIN_A,(scanRow>>0)&1);
    digitalWrite(PIN_B,(scanRow>>1)&1);
    digitalWrite(PIN_C,(scanRow>>2)&1);

    int y1 = scanRow;
    int y2 = scanRow + 8;

    for(int x=0;x<totalWidth;x++)
    {
        digitalWrite(PIN_R1,framebuffer[y1][x][0]);
        digitalWrite(PIN_G1,framebuffer[y1][x][1]);
        digitalWrite(PIN_B1,framebuffer[y1][x][2]);

        digitalWrite(PIN_R2,framebuffer[y2][x][0]);
        digitalWrite(PIN_G2,framebuffer[y2][x][1]);
        digitalWrite(PIN_B2,framebuffer[y2][x][2]);

        digitalWrite(PIN_CLK,HIGH);
        digitalWrite(PIN_CLK,LOW);
    }

    digitalWrite(PIN_LAT,HIGH);
    digitalWrite(PIN_LAT,LOW);

    digitalWrite(PIN_OE,LOW);
    delayMicroseconds(50);
    digitalWrite(PIN_OE,HIGH);

    if(++scanRow>=8) scanRow=0;
}

uint16_t AriesRGB_32x16::width(){ return totalWidth; }
uint16_t AriesRGB_32x16::height(){ return totalHeight; }
