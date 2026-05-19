#include "AriesDMD.h"
#include "AriesFont5x7.h"

AriesDMD dmd(2, 1, 9, 6, 7, 8);   // 2 panels wide, 1 panel high, OE, A, B, Lat
// Clk --> SPI2_SCLK
// Data --> SPI2_MOSI
// GND --> GND

const char text[] = "NISHIL PATEL";
int scrollX;

void setup() {
  dmd.begin();
  scrollX = dmd.width();
}

// Right to Left
/* 
void loop() {
  dmd.scanDisplay();

  static unsigned long lastMove = 0;
  if (millis() - lastMove > 70) {
    lastMove = millis();

    dmd.clearScreen(true);
    AriesDrawString(dmd, scrollX, 4, text);

    scrollX--;
    int textWidth = strlen(text) * (ARIES_FONT_WIDTH + 1);

    if (scrollX < -textWidth) {
      scrollX = dmd.width();
    }
  }
}
*/ 

// Left to Right 
void loop() {
    dmd.scanDisplay();   

    static unsigned long lastMove = 0;
    if (millis() - lastMove > 40) {
        lastMove = millis();

        dmd.clearScreen(true);
        AriesDrawString(dmd, scrollX, 4, text);

        scrollX++;   

        int textWidth = strlen(text) * (ARIES_FONT_WIDTH + 1);
        if (scrollX > dmd.width()) {
            scrollX = -textWidth;   
        }
    }
}
