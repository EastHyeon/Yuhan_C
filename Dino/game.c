#include "main.h"

void PrintPos(int x, int y){
    SetAllColor(DARK_GRAY, WHITE);
    char buffer[256];
    sprintf_s(buffer,sizeof(buffer), "(%d, %d)", x, y);
    char* msg = buffer;
    int PosX = SCREEN_MAX_X - euc_kr_strlen(msg) + 1;
    GotoXY(78, 35);
    printf("     ");
    GotoXY(73, 35);
    SetAllColor(DARK_GRAY, WHITE);
    printf("Pos: ");
    GotoXY(PosX, 35);
    printf(msg);
    SetAllColor(DEFAULT_BACKGROUND, DEFAULT_TEXT);
}

void PrintLog(const char* str){
    SetAllColor(DARK_GRAY, WHITE);
    GotoXY(SCREEN_MIN_X, 35);
    printf("                                                                      ");
    GotoXY(SCREEN_MIN_X, 35);
    printf("ป๓ลย: %s", str);
    SetAllColor(DEFAULT_BACKGROUND, DEFAULT_TEXT);
}

void PrintScore(int score) {
    SetAllColor(DARK_GRAY, WHITE);
    char buffer[256];
    sprintf_s(buffer, sizeof(buffer), "Score: %d", score);
    char* msg = buffer;
    SetAllColor(DARK_GRAY, WHITE);
    GotoXY(SCREEN_MIN_X, 33);
    printf("               ");
    GotoXY(SCREEN_MIN_X, 33);
    printf(msg);
    SetAllColor(DEFAULT_BACKGROUND, DEFAULT_TEXT);
}

enum ConsoleColor GetTileColor(enum TileType type) {
    switch (type) {
    case EMPTY:
        return CYAN;
    case WALL:
        return DARK_GREEN;
    case TREE:
        return GRAY;
    case PLAYER:
        return GREEN;
    default:
        return DEFAULT_BACKGROUND;
    }
}