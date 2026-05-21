/*
 * WIRING
 * ──────
 *   GPIO  3  → R0     GPIO  4  → G0     GPIO  5  → B0    
 *   GPIO  6  → R1     GPIO  7  → G1     GPIO  8  → B1    
 *   GPIO 14  → CLK    GPIO 15  → STB    GPIO 16  → OE
 *   GPIO 10  → A      GPIO 11  → B      GPIO 12  → C
 *   GPIO 13  → D      GPIO  9  → E
 *   GPIO 26  → Left button  (other pin → GND)
 *   GPIO 27  → Right button (other pin → GND)
 *
 * CONTROLS
 * ────────
 *   Left button                  → move piece left  (auto-repeat)
 *   Right button                 → move piece right (auto-repeat)
 *   Any button (game over screen) → restart
 *
 */

#include <AriesRGB.h>

// ── Hardware ──────────────────────────────────────────────────────────────
AriesRGB panel(
    3, 4, 5,
    6, 7, 8,
    14, 15, 16,
    10, 11, 12, 13, 9,
    2,    // panelsWide
    2     // panelsHigh
);

#define BTN_LEFT   26
#define BTN_RIGHT  27

// ── Board & display constants ─────────────────────────────────────────────
#define CELL         4  
#define BOARD_COLS  10
#define BOARD_ROWS  20
#define BOARD_X     60   
#define BOARD_Y      0   

//   I=cyan  O=yellow  T=magenta  S=green  Z=red  L=white  J=blue
const uint8_t PCOLOR[7][3] = {
    {0,1,1}, {1,1,0}, {1,0,1}, {0,1,0}, {1,0,0}, {1,1,1}, {0,0,1}
};

// ── Piece shapes [type][rotation][cell] = {row_offset, col_offset} ─────
const int8_t SHAPE[7][4][4][2] = {
    // I (cyan)
    {{{0,0},{0,1},{0,2},{0,3}}, {{0,2},{1,2},{2,2},{3,2}},
     {{2,0},{2,1},{2,2},{2,3}}, {{0,1},{1,1},{2,1},{3,1}}},
    // O (yellow)
    {{{0,0},{0,1},{1,0},{1,1}}, {{0,0},{0,1},{1,0},{1,1}},
     {{0,0},{0,1},{1,0},{1,1}}, {{0,0},{0,1},{1,0},{1,1}}},
    // T (magenta)
    {{{0,1},{1,0},{1,1},{1,2}}, {{0,0},{1,0},{1,1},{2,0}},
     {{0,0},{0,1},{0,2},{1,1}}, {{0,1},{1,0},{1,1},{2,1}}},
    // S (green)
    {{{0,1},{0,2},{1,0},{1,1}}, {{0,0},{1,0},{1,1},{2,1}},
     {{0,1},{0,2},{1,0},{1,1}}, {{0,0},{1,0},{1,1},{2,1}}},
    // Z (red)
    {{{0,0},{0,1},{1,1},{1,2}}, {{0,1},{1,0},{1,1},{2,0}},
     {{0,0},{0,1},{1,1},{1,2}}, {{0,1},{1,0},{1,1},{2,0}}},
    // L (white)
    {{{0,2},{1,0},{1,1},{1,2}}, {{0,0},{1,0},{2,0},{2,1}},
     {{0,0},{0,1},{0,2},{1,0}}, {{0,0},{0,1},{1,1},{2,1}}},
    // J (blue)
    {{{0,0},{1,0},{1,1},{1,2}}, {{0,0},{0,1},{1,0},{2,0}},
     {{0,0},{0,1},{0,2},{1,2}}, {{0,2},{1,2},{2,1},{2,2}}},
};

// ── Game state ────────────────────────────────────────────────────────────
bool    board[BOARD_ROWS][BOARD_COLS];
uint8_t boardColor[BOARD_ROWS][BOARD_COLS];

int  pieceType, pieceRot, pieceX, pieceY;
int  nextType;
int  level, linesTotal;
bool gameOver;

unsigned long lastDropTime, dropInterval;

// ── Button state ──────────────────────────────────────────────────────────
bool  bLprev, bRprev;
bool  bothPressed, bothActed;
unsigned long bLtime, bRtime, bLrepeat, bRrepeat, bothPressTime;

// ── Core game helpers ─────────────────────────────────────────────────────

bool pieceCanPlace(int type, int rot, int bx, int by) {
    for (int i = 0; i < 4; i++) {
        int r = by + SHAPE[type][rot][i][0];
        int c = bx + SHAPE[type][rot][i][1];
        if (c < 0 || c >= BOARD_COLS || r >= BOARD_ROWS) return false;
        if (r >= 0 && board[r][c])                       return false;
    }
    return true;
}

void pieceLock() {
    for (int i = 0; i < 4; i++) {
        int r = pieceY + SHAPE[pieceType][pieceRot][i][0];
        int c = pieceX + SHAPE[pieceType][pieceRot][i][1];
        if (r >= 0 && r < BOARD_ROWS && c >= 0 && c < BOARD_COLS) {
            board[r][c]      = true;
            boardColor[r][c] = pieceType;
        }
    }
}

// Finds full rows; returns count; fills out[] with which rows are full
int findFullRows(bool out[BOARD_ROWS]) {
    int n = 0;
    for (int r = 0; r < BOARD_ROWS; r++) {
        out[r] = true;
        for (int c = 0; c < BOARD_COLS; c++) if (!board[r][c]) { out[r]=false; break; }
        if (out[r]) n++;
    }
    return n;
}

int clearLines() {
    int n = 0;
    for (int r = BOARD_ROWS - 1; r >= 0; ) {
        bool full = true;
        for (int c = 0; c < BOARD_COLS; c++) if (!board[r][c]) { full=false; break; }
        if (full) {
            n++;
            for (int rr = r; rr > 0; rr--)
                for (int c = 0; c < BOARD_COLS; c++) {
                    board[rr][c]      = board[rr-1][c];
                    boardColor[rr][c] = boardColor[rr-1][c];
                }
            for (int c = 0; c < BOARD_COLS; c++) board[0][c] = false;
        } else r--;
    }
    return n;
}

// CW rotation with 5-offset wall kick (centre, left 1, right 1, left 2, right 2)
void pieceRotate() {
    int nr = (pieceRot + 1) % 4;
    const int KICKS[5] = {0, -1, 1, -2, 2};
    for (int k = 0; k < 5; k++) {
        if (pieceCanPlace(pieceType, nr, pieceX + KICKS[k], pieceY)) {
            pieceX  += KICKS[k];
            pieceRot = nr;
            return;
        }
    }
}

// Move current piece to its lowest valid position; force lock on next gravity tick
void hardDrop() {
    while (pieceCanPlace(pieceType, pieceRot, pieceX, pieceY + 1))
        pieceY++;
    lastDropTime = 0;   // triggers immediate lock on next loop() gravity check
}

void spawnPiece() {
    pieceType = nextType;
    nextType  = random(7);
    pieceRot  = 0;
    pieceX    = 3;
    pieceY    = 0;
    if (!pieceCanPlace(pieceType, pieceRot, pieceX, pieceY))
        gameOver = true;
}

void updateLevel() {
    int newLevel = linesTotal / 5 + 1;
    if (newLevel != level) {
        level       = newLevel;
        dropInterval = (unsigned long)max(80, 800 - (level - 1) * 38);
    }
}

void initGame() {
    memset(board,      0, sizeof(board));
    memset(boardColor, 0, sizeof(boardColor));
    level        = 1;
    linesTotal   = 0;
    dropInterval = 800;
    gameOver     = false;
    nextType     = random(7);
    bLprev = bRprev = bothPressed = bothActed = false;
    spawnPiece();
    lastDropTime = millis();
}

void drawCell(int px, int py, uint8_t r, uint8_t g, uint8_t b) {
    for (int dy = 0; dy < 3; dy++)
        for (int dx = 0; dx < 3; dx++)
            panel.drawPixel(px+dx, py+dy, r, g, b);
}

void eraseCell(int px, int py) {
    drawCell(px, py, 0, 0, 0);
}

void hline(int x0, int x1, int y) {
    for (int x = x0; x <= x1; x++) panel.drawPixel(x, y, 1, 1, 1);
}

void vline(int x, int y0, int y1) {
    for (int y = y0; y <= y1; y++) panel.drawPixel(x, y, 1, 1, 1);
}

// ── UI drawing ────────────────────────────────────────────────────────────

void drawBoardBorders() {
    vline(BOARD_X - 1,              0, 79);   // left border
    vline(BOARD_X + BOARD_COLS*CELL, 0, 79);  // right border
}

void drawNextBox() {
    vline(102, 1, 18);  vline(119, 1, 18);
    hline(102, 119, 1); hline(102, 119, 18);

    for (int y = 2; y <= 17; y++)
        for (int x = 103; x <= 118; x++)
            panel.drawPixel(x, y, 0, 0, 0);

    int minR=3, maxR=0, minC=3, maxC=0;
    for (int i = 0; i < 4; i++) {
        int r = SHAPE[nextType][0][i][0];
        int c = SHAPE[nextType][0][i][1];
        if (r < minR) minR=r;  if (r > maxR) maxR=r;
        if (c < minC) minC=c;  if (c > maxC) maxC=c;
    }
    int pieceW = (maxC - minC + 1) * CELL;
    int pieceH = (maxR - minR + 1) * CELL;
    int offX   = (16 - pieceW) / 2;
    int offY   = (16 - pieceH) / 2;

    for (int i = 0; i < 4; i++) {
        int dr = SHAPE[nextType][0][i][0] - minR;
        int dc = SHAPE[nextType][0][i][1] - minC;
        drawCell(103 + offX + dc*CELL,
                 2   + offY + dr*CELL,
                 PCOLOR[nextType][0],
                 PCOLOR[nextType][1],
                 PCOLOR[nextType][2]);
    }
}

void drawLevelBar() {
    vline(102, 22, 77);  vline(119, 22, 77);
    hline(102, 119, 22); hline(102, 119, 77);

    const int  BAR_H  = 54;
    const int  prog   = linesTotal % 5;
    const int  filled = prog * BAR_H / 5;

    uint8_t barR, barG, barB;
    if      (level <= 3)  { barR=0; barG=1; barB=0; }  // green   — easy
    else if (level <= 7)  { barR=1; barG=1; barB=0; }  // yellow  — medium
    else if (level <= 12) { barR=1; barG=0; barB=1; }  // magenta — hard
    else                  { barR=1; barG=0; barB=0; }  // red     — insane

    for (int y = 0; y < BAR_H; y++) {
        bool lit = (y >= BAR_H - filled);
        for (int x = 103; x <= 118; x++)
            panel.drawPixel(x, 23 + y, lit ? barR : 0, lit ? barG : 0, lit ? barB : 0);
    }
}


void doRefresh(int frames = 1) {
    for (int f = 0; f < frames; f++)
        for (int s = 0; s < 20; s++)
            panel.refresh();
}


void flashClearedLines(bool fullRows[BOARD_ROWS]) {
    for (int flash = 0; flash < 4; flash++) {
        uint8_t on = (flash % 2 == 0) ? 1 : 0;
        for (int r = 0; r < BOARD_ROWS; r++) {
            if (!fullRows[r]) continue;
            for (int c = 0; c < BOARD_COLS; c++)
                drawCell(BOARD_X + c*CELL, BOARD_Y + r*CELL, on, on, on);
        }
        doRefresh(1);
    }
}


void renderBoard() {
    
    for (int r = 0; r < BOARD_ROWS; r++) {
        for (int c = 0; c < BOARD_COLS; c++) {
            int px = BOARD_X + c*CELL;
            int py = BOARD_Y + r*CELL;
            if (board[r][c]) {
                uint8_t t = boardColor[r][c];
                drawCell(px, py, PCOLOR[t][0], PCOLOR[t][1], PCOLOR[t][2]);
            } else {
                eraseCell(px, py);
            }
        }
    }
  
    for (int i = 0; i < 4; i++) {
        int r = pieceY + SHAPE[pieceType][pieceRot][i][0];
        int c = pieceX + SHAPE[pieceType][pieceRot][i][1];
        if (r >= 0 && r < BOARD_ROWS && c >= 0 && c < BOARD_COLS)
            drawCell(BOARD_X + c*CELL, BOARD_Y + r*CELL,
                     PCOLOR[pieceType][0],
                     PCOLOR[pieceType][1],
                     PCOLOR[pieceType][2]);
    }
}
void handleInput() {
    bool L   = (digitalRead(BTN_LEFT)  == LOW);
    bool R   = (digitalRead(BTN_RIGHT) == LOW);
    unsigned long now = millis();

    // ── Both buttons ──────────────────────────────────────────────────────
    if (L && R) {
        if (!bothPressed) {
            bothPressed   = true;
            bothActed     = false;
            bothPressTime = now;
        } else if (!bothActed && (now - bothPressTime >= 350)) {
            hardDrop();               // held ≥ 350 ms → hard drop
            bothActed = true;
        }
        bLprev = bRprev = true;       
        return;
    }

    if (bothPressed) {
        if (!bothActed) pieceRotate(); // brief press → rotate
        bothPressed = bothActed = false;
        bLprev = bRprev = false;      
        return;
    }

    // ── Left button only ──────────────────────────────────────────────────
    if (L && !R) {
        if (!bLprev) {
            if (pieceCanPlace(pieceType, pieceRot, pieceX-1, pieceY)) pieceX--;
            bLprev = true;  bLtime = bLrepeat = now;
        } else if (now - bLtime > 230 && now - bLrepeat > 75) {
            if (pieceCanPlace(pieceType, pieceRot, pieceX-1, pieceY)) pieceX--;
            bLrepeat = now;
        }
    } else {
        bLprev = false;
    }

    // ── Right button only ─────────────────────────────────────────────────
    if (R && !L) {
        if (!bRprev) {
            if (pieceCanPlace(pieceType, pieceRot, pieceX+1, pieceY)) pieceX++;
            bRprev = true;  bRtime = bRrepeat = now;
        } else if (now - bRtime > 230 && now - bRrepeat > 75) {
            if (pieceCanPlace(pieceType, pieceRot, pieceX+1, pieceY)) pieceX++;
            bRrepeat = now;
        }
    } else {
        bRprev = false;
    }
}

// ── Setup ─────────────────────────────────────────────────────────────────

void setup() {
    pinMode(BTN_LEFT,  INPUT_PULLUP);
    pinMode(BTN_RIGHT, INPUT_PULLUP);

    panel.begin();
    randomSeed(micros());

    panel.clear();
    drawBoardBorders();
    initGame();
    drawNextBox();
    drawLevelBar();
}

// ── Main loop ─────────────────────────────────────────────────────────────

void loop() {

    // ── Game-over state ───────────────────────────────────────────────────
    if (gameOver) {
        // Alternate red / black over the board at ~2 Hz
        bool flash = ((millis() / 400) % 2 == 0);
        for (int r = 0; r < BOARD_ROWS; r++)
            for (int c = 0; c < BOARD_COLS; c++)
                drawCell(BOARD_X + c*CELL, BOARD_Y + r*CELL, flash ? 1 : 0, 0, 0);
        doRefresh(1);

        if (digitalRead(BTN_LEFT) == LOW || digitalRead(BTN_RIGHT) == LOW) {
            delay(400);                
            panel.clear();
            drawBoardBorders();
            initGame();
            drawNextBox();
            drawLevelBar();
        }
        return;
    }

    // ── Input ─────────────────────────────────────────────────────────────
    handleInput();

    if (millis() - lastDropTime >= dropInterval) {
        lastDropTime = millis();

        if (pieceCanPlace(pieceType, pieceRot, pieceX, pieceY + 1)) {
        
            pieceY++;
        } else {
           

            pieceLock();

            bool fullRows[BOARD_ROWS] = {};
            int  cleared = findFullRows(fullRows);

            if (cleared > 0) {
                flashClearedLines(fullRows);  
                clearLines();                 
                linesTotal += cleared;
                updateLevel();
                drawLevelBar();
            }

            spawnPiece();                     
            if (!gameOver) drawNextBox();
        }
    }

    renderBoard();
    doRefresh(1);   
}
