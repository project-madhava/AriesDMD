#include "AriesRGB_32x16.h"
#include "AriesRGB_Font5x7_Vert.h"
#include "AriesRGB_Font5x7_VertScaled.h"

AriesRGB_32x16 display(3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 20, 5);

int scrollX = 320; 
const int TEXT_Y = 73; // Centered at Scale 2
#define REFRESHES_PER_FRAME 20 

void setup() { display.begin(); }

void loop() {
    display.clear();
    const char* flightMsg = "Aries P10 Cylindrical Setup";
    
    // Scale 2 
    AriesDrawStringVertScaled(display, scrollX, TEXT_Y, flightMsg, 0, 1, 1, 2);

    for (int i = 0; i < REFRESHES_PER_FRAME; i++) {
        display.refresh();
    }

    scrollX--; 
    // 12 pixels wide per char at scale 2
    if (scrollX < -( (int)strlen(flightMsg) * 12 )) {
        scrollX = 320; 
    }
}

