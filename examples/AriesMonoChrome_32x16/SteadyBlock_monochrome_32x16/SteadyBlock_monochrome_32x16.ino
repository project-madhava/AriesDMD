#include "AriesDMD.h"

// panelsWide = 1  → number of P10 panels horizontally
// panelsHigh = 1  → number of P10 panels vertically
// pinOE  = 9  → Output Enable
// pinA   = 6  → Row select A
// pinB   = 7  → Row select B
// pinLAT = 8  → Latch (STB)
//pinClk → SPI2_SCLK
//pinData → SPI2_MOSI
AriesDMD dmd(1, 1, 9, 6, 7, 8);

void setup() {
  dmd.begin();
  for(int i = 0; i < 4; i++){
    for (int j = 0; j < 4; j++){
      dmd.writePixel(i, j, GRAPHICS_NORMAL, true); // A block of size 4*4 will be created on top left corner.
    }
  }
}

void loop() {
  dmd.scanDisplay();
}
