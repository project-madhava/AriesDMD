#include "AriesRGB_32x16.h"
#include "AriesRGB_Font5x7_Vert.h"
#include "AriesRGB_Font5x7_VertScaled.h"

// Initialize display for 20 wide, 5 high setup
AriesRGB_32x16 display(3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 20, 5);

// Start scrolling from the right edge
int scrollX = 320; 
#define REFRESHES_PER_FRAME 20 

// --- FLIGHT DATA SETUP ---
const int NUM_LINES = 6;

const char* flights[NUM_LINES] = {
    "FLIGHT                GATE  ",   
    "AA102                 G12   ",   
    "BA405                 G04   ",  
    "DL992                 G18   ",   
    "AF320                 G07   ",   
    "EK202                 G22   "    
};

// The colors for each row {R, G, B}
const bool colors[NUM_LINES][3] = {
    {0, 1, 1}, // Cyan    
    {0, 1, 0}, // Green   
    {1, 1, 0}, // Yellow  
    {1, 0, 0}, // Red     
    {1, 0, 1}, // Magenta 
    {0, 1, 0}  // Green   
};

// Y-Coordinates for each row
// Scale 2 char height is 14px. Spaced 26 pixels apart vertically.
const int yCoords[NUM_LINES] = {8, 34, 60, 86, 112, 138};

void setup() { 
    display.begin(); 
}

void loop() {
    display.clear();
    
    // Draw all 6 lines of text
    for(int i = 0; i < NUM_LINES; i++) {
        AriesDrawStringVertScaled(
            display, 
            scrollX, 
            yCoords[i], 
            flights[i], 
            colors[i][0], colors[i][1], colors[i][2], 
            2 // Scale 2
        );
    }

    // Refresh the hardware
    for (int i = 0; i < REFRESHES_PER_FRAME; i++) {
        display.refresh();
    }

    // Move text to the left
    scrollX--; 
    
    // Reset scroll when the string goes off screen.
    // 28 chars * 12 pixels per char = 336 pixels wide.
    if (scrollX < -350) { 
        scrollX = 320; 
    }
}
