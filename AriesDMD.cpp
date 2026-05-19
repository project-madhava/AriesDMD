#include "AriesDMD.h"

// VEGA SPI INSTANCE
SPIClass SPI(2); // You can change this section if you are using another SPI pins on the Aries v3 baord. There are total three SPI Pins: 0,1,2

static SPISettings dmdSPI(
    2000000,
    MSBFIRST,
    SPI_MODE0
);

static const uint8_t pixelMask[8] =
{
    0x80,
    0x40,
    0x20,
    0x10,
    0x08,
    0x04,
    0x02,
    0x01
};

AriesDMD::AriesDMD(
    uint8_t pw,
    uint8_t ph,
    uint8_t oe,
    uint8_t a,
    uint8_t b,
    uint8_t lat
)
{
    panelsWide  = pw;
    panelsHigh  = ph;

    pinOE  = oe;
    pinA   = a;
    pinB   = b;
    pinLAT = lat;

    displayWidth  = panelsWide  * PANEL_WIDTH;
    displayHeight = panelsHigh  * PANEL_HEIGHT;

    rowWidthBytes = panelsWide * 4;

    frameBytes = rowWidthBytes * displayHeight;

    frameBuffer = (uint8_t*)malloc(frameBytes);

    if(!frameBuffer)
    {
        while(1);
    }

    scanRow = 0;
}

AriesDMD::~AriesDMD()
{
    if(frameBuffer)
    {
        free(frameBuffer);
    }
}

void AriesDMD::begin()
{
    pinMode(pinOE, OUTPUT);
    pinMode(pinA,  OUTPUT);
    pinMode(pinB,  OUTPUT);
    pinMode(pinLAT,OUTPUT);

    digitalWrite(pinOE, HIGH);
    digitalWrite(pinLAT, LOW);

    SPI.begin();

    SPI.beginTransaction(dmdSPI);

    clearScreen(true);
}

void AriesDMD::clearScreen(bool off)
{
    memset(
        frameBuffer,
        off ? 0xFF : 0x00,
        frameBytes
    );
}

void AriesDMD::writePixel(
    uint16_t x,
    uint16_t y,
    uint8_t mode,
    bool pixel
)
{
    if(x >= displayWidth || y >= displayHeight)
    {
        return;
    }

    uint16_t panelX = x / PANEL_WIDTH;

    uint8_t localX = x % PANEL_WIDTH;

    uint16_t byteCol =
        panelX * 4 + (localX >> 3);

    uint8_t mask =
        pixelMask[localX & 7];

    uint32_t index =
        y * rowWidthBytes + byteCol;

    switch(mode)
    {
        case GRAPHICS_NORMAL:

            pixel ?
            (frameBuffer[index] &= ~mask) :
            (frameBuffer[index] |=  mask);

            break;

        case GRAPHICS_INVERSE:

            pixel ?
            (frameBuffer[index] |=  mask) :
            (frameBuffer[index] &= ~mask);

            break;

        case GRAPHICS_TOGGLE:

            if(pixel)
            {
                frameBuffer[index] ^= mask;
            }

            break;
    }
}

void AriesDMD::scanDisplay()
{
    digitalWrite(pinOE, HIGH);

    uint8_t r0 = scanRow;
    uint8_t r1 = scanRow + 4;
    uint8_t r2 = scanRow + 8;
    uint8_t r3 = scanRow + 12;

    for(uint16_t col = 0;
        col < rowWidthBytes;
        col++)
    {
        SPI.transfer(
            frameBuffer[
                r3 * rowWidthBytes + col
            ]
        );

        SPI.transfer(
            frameBuffer[
                r2 * rowWidthBytes + col
            ]
        );

        SPI.transfer(
            frameBuffer[
                r1 * rowWidthBytes + col
            ]
        );

        SPI.transfer(
            frameBuffer[
                r0 * rowWidthBytes + col
            ]
        );
    }

    digitalWrite(pinA, scanRow & 1);

    digitalWrite(
        pinB,
        (scanRow >> 1) & 1
    );

    digitalWrite(pinLAT, HIGH);
    digitalWrite(pinLAT, LOW);

    digitalWrite(pinOE, LOW);

    scanRow =
        (scanRow + 1) & 3;
}
