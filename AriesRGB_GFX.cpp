#include "AriesRGB_GFX.h"
#include "AriesRGB_Font5x7_Vert.h" 

#ifdef __AVR__
  #include <avr/pgmspace.h>
#endif

// Safer pointer reading using compiler built-ins to completely silence warnings
#ifndef pgm_read_pointer
  static inline void* safe_read_pointer(const void* addr) {
      void* ptr;
      __builtin_memcpy(&ptr, addr, sizeof(void*));
      return ptr;
  }
  #define pgm_read_pointer(addr) safe_read_pointer(addr)
#endif

void AriesDrawCharGFX(AriesRGB_32x16& display, int x, int y, char c, bool r, bool g, bool b, const GFXfont *gfxFont) 
{
    uint16_t first = pgm_read_word(&gfxFont->first);
    uint16_t last  = pgm_read_word(&gfxFont->last);

    if(c < first || c > last) return;
    
    uint8_t c_offset = (uint8_t)(c - first);

    GFXglyph *glyph  = &(((GFXglyph *)pgm_read_pointer(&gfxFont->glyph))[c_offset]);
    uint8_t  *bitmap = (uint8_t *)pgm_read_pointer(&gfxFont->bitmap);

    uint16_t bo = pgm_read_word(&glyph->bitmapOffset);
    uint8_t  w  = pgm_read_byte(&glyph->width);
    uint8_t  h  = pgm_read_byte(&glyph->height);
    int8_t   xo = pgm_read_byte(&glyph->xOffset);
    int8_t   yo = pgm_read_byte(&glyph->yOffset);

    uint8_t  bit = 0, bits = 0;

    for(uint8_t yy = 0; yy < h; yy++) {
        for(uint8_t xx = 0; xx < w; xx++) {
            if(!(bit++ & 7)) {
                bits = pgm_read_byte(&bitmap[bo++]);
            }
            if(bits & 0x80) {
                // THE FIX: Standard X rendering!
                // We removed the (w - 1 - xx) mirror so it draws left-to-right normally.
                int drawX = x + xo + xx; 
                
                // Y remains flipped to properly push the typography up from your bottom-up baseline
                int drawY = y - yo - yy; 

                AriesDrawPixelVert(display, drawX, drawY, r, g, b);
            }
            bits <<= 1;
        }
    }
}

int AriesDrawStringGFX(AriesRGB_32x16& display, int x, int y, const char* str, bool r, bool g, bool b, const GFXfont *gfxFont) 
{
    uint16_t first = pgm_read_word(&gfxFont->first);
    uint16_t last  = pgm_read_word(&gfxFont->last);

    while(*str) {
        char c = *str++;
        if(c >= first && c <= last) {
            uint8_t c_offset = (uint8_t)(c - first);
            GFXglyph *glyph = &(((GFXglyph *)pgm_read_pointer(&gfxFont->glyph))[c_offset]);
            uint8_t xAdvance = pgm_read_byte(&glyph->xAdvance);
            
            AriesDrawCharGFX(display, x, y, c, r, g, b, gfxFont);
            
            // Advance the cursor normally
            x += xAdvance; 
        }
    }
    return x; 
}
