#include "AriesRGB.h"

AriesRGB::AriesRGB(
    uint8_t r1, uint8_t g1, uint8_t b1,
    uint8_t r2, uint8_t g2, uint8_t b2,
    uint8_t clk, uint8_t lat, uint8_t oe,
    uint8_t a, uint8_t b, uint8_t c, uint8_t d, uint8_t e,
    uint8_t pw, uint8_t ph)
{
    panelsWide  = constrain(pw, 1, MAX_PANELS_X);
    panelsHigh  = constrain(ph, 1, MAX_PANELS_Y);

    totalWidth  = panelsWide  * PANEL_WIDTH;
    totalHeight = panelsHigh  * PANEL_HEIGHT;

    PIN_R1 = r1; PIN_G1 = g1; PIN_B1 = b1;
    PIN_R2 = r2; PIN_G2 = g2; PIN_B2 = b2;
    PIN_CLK = clk; PIN_LAT = lat; PIN_OE = oe;
    PIN_A = a; PIN_B = b; PIN_C = c; PIN_D = d; PIN_E = e;

    scanRow = 0;
    memset(framebuffer, 0, sizeof(framebuffer));
}

void AriesRGB::begin()
{
    pinMode(PIN_R1, OUTPUT); pinMode(PIN_G1, OUTPUT); pinMode(PIN_B1, OUTPUT);
    pinMode(PIN_R2, OUTPUT); pinMode(PIN_G2, OUTPUT); pinMode(PIN_B2, OUTPUT);
    pinMode(PIN_CLK, OUTPUT);
    pinMode(PIN_LAT, OUTPUT);
    pinMode(PIN_OE,  OUTPUT);
    pinMode(PIN_A, OUTPUT); pinMode(PIN_B, OUTPUT); pinMode(PIN_C, OUTPUT);
    pinMode(PIN_D, OUTPUT); pinMode(PIN_E, OUTPUT);

    clear();
    initDriver();
}

void AriesRGB::initDriver()
{
    digitalWrite(PIN_OE, HIGH);

    for (int i = 0; i < totalWidth; i++)
    {
        digitalWrite(PIN_CLK, HIGH);
        digitalWrite(PIN_CLK, LOW);
    }

    digitalWrite(PIN_LAT, HIGH);
    digitalWrite(PIN_LAT, LOW);
    digitalWrite(PIN_OE, LOW);
}

void AriesRGB::clear()
{
    memset(framebuffer, 0, sizeof(framebuffer));
}

void AriesRGB::drawPixel(int x, int y, uint8_t r, uint8_t g, uint8_t b)
{
    if (x < 0 || x >= totalWidth) return;
    if (y < 0 || y >= totalHeight) return;

    framebuffer[y][x][0] = r;
    framebuffer[y][x][1] = g;
    framebuffer[y][x][2] = b;
}

void AriesRGB::refresh()
{
    digitalWrite(PIN_OE, HIGH);

    digitalWrite(PIN_A,  scanRow & 1);
    digitalWrite(PIN_B, (scanRow >> 1) & 1);
    digitalWrite(PIN_C, (scanRow >> 2) & 1);
    digitalWrite(PIN_D, (scanRow >> 3) & 1);
    digitalWrite(PIN_E, (scanRow >> 4) & 1);

    // iterate vertically stacked panels
    for (int py = 0; py < panelsHigh; py++)
    {
        int y1 = py * PANEL_HEIGHT + scanRow;
        int y2 = y1 + 20;

        // horizontal panels
        for (int p = 0; p < panelsWide; p++)
        {
            int x0 = p * PANEL_WIDTH;

            for (int col = 0; col < PANEL_WIDTH; col++)
            {
                int x = x0 + col;

                digitalWrite(PIN_R1, framebuffer[y1][x][0]);
                digitalWrite(PIN_G1, framebuffer[y1][x][1]);
                digitalWrite(PIN_B1, framebuffer[y1][x][2]);

                digitalWrite(PIN_R2, framebuffer[y2][x][0]);
                digitalWrite(PIN_G2, framebuffer[y2][x][1]);
                digitalWrite(PIN_B2, framebuffer[y2][x][2]);

                digitalWrite(PIN_CLK, HIGH);
                digitalWrite(PIN_CLK, LOW);
            }
        }
    }

    digitalWrite(PIN_LAT, HIGH);
    digitalWrite(PIN_LAT, LOW);

    digitalWrite(PIN_OE, LOW);
    delayMicroseconds(50);
    digitalWrite(PIN_OE, HIGH);

    if (++scanRow >= 20) scanRow = 0;
}

// ================= TEXT =================

void AriesRGB::setCursor(int x, int y) {
    cursor_x = x;
    cursor_y = y;
}

void AriesRGB::setTextColor(uint8_t r, uint8_t g, uint8_t b) {
    text_r = r;
    text_g = g;
    text_b = b;
}

void AriesRGB::setTextSize(uint8_t s) {
    text_size = s;
}

void AriesRGB::setFont(const uint8_t* f) {
    font.setFont(f);
}

void AriesRGB::print(const char* str)
{
    while (*str)
    {
        char c = *str++;

        if (!font.isCharValid(c)) continue;

        uint8_t w = font.getCharWidth(c);
        uint8_t h = font.getHeight();

        for (uint8_t i = 0; i < w; i++)
        {
            uint8_t col = font.getColumn(c, i);

            for (uint8_t j = 0; j < h; j++)
            {
                if (col & (1 << j))
                {
                    for (uint8_t sx = 0; sx < text_size; sx++)
                    {
                        for (uint8_t sy = 0; sy < text_size; sy++)
                        {
                            drawPixel(
                                cursor_x + i * text_size + sx,
                                cursor_y + j * text_size + sy,
                                text_r, text_g, text_b
                            );
                        }
                    }
                }
            }
        }

        cursor_x += (w + 1) * text_size;
    }
}
