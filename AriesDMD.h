#ifndef ARIES_DMD_H
#define ARIES_DMD_H

#include <Arduino.h>
#include <SPI.h>

#define PANEL_WIDTH   32
#define PANEL_HEIGHT  16

#define GRAPHICS_NORMAL   0
#define GRAPHICS_INVERSE  1
#define GRAPHICS_TOGGLE   2

class AriesDMD
{
public:

  AriesDMD(
      uint8_t panelsWide,
      uint8_t panelsHigh,
      uint8_t pinOE,
      uint8_t pinA,
      uint8_t pinB,
      uint8_t pinLAT
  );

  ~AriesDMD();

  void begin();

  void clearScreen(bool off = true);

  void writePixel(
      uint16_t x,
      uint16_t y,
      uint8_t mode,
      bool pixel
  );

  void scanDisplay();

  uint16_t width()  const { return displayWidth;  }
  uint16_t height() const { return displayHeight; }

private:

  uint8_t panelsWide;
  uint8_t panelsHigh;

  uint16_t displayWidth;
  uint16_t displayHeight;

  uint16_t rowWidthBytes;
  uint16_t frameBytes;

  uint8_t *frameBuffer;

  uint8_t pinOE;
  uint8_t pinA;
  uint8_t pinB;
  uint8_t pinLAT;

  volatile uint8_t scanRow;
};

#endif
