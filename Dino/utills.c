#include "main.h"

void GotoXY(int x, int y){
    HANDLE consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
    COORD pos = {x, y};
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
}

void CursorHide(void){
    CONSOLE_CURSOR_INFO cursorInfo = { 0, };
    cursorInfo.dwSize = 1;
    cursorInfo.bVisible = FALSE;
    SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cursorInfo);
}

void SetColor(int textColor){
    HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(handle, (DEFAULT_BACKGROUND<<4) + textColor);
}

void SetAllColor(int backGroundColor, int textColor){
    HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(handle, (backGroundColor<<4) + textColor);
}

int WriteCenter(const char *str, int y){
    int screenWidth = SCREEN_MAX_X;
    int len = euc_kr_strlen(str);
    int xPos = (screenWidth - len) / 2 + SCREEN_MIN_X;

    if(xPos <2){
        xPos = 2;
    }

    GotoXY(xPos, y);
    printf("%s", str);
    return xPos;
}

int WriteLineCenter(const char *str, int y){
    int screenWidth = SCREEN_MAX_X;
    int len = euc_kr_strlen(str);
    int xPos = (screenWidth - len) / 2 + SCREEN_MIN_X;

    if(xPos < 2){
        xPos = 2;
    }

    GotoXY(xPos, y);
    printf("%s\n", str);
    return xPos;
}

int GetCenter(const char *str){
    int screenWidth = SCREEN_MAX_X;
    int len = euc_kr_strlen(str);
    int xPos = (screenWidth - len) / 2 + SCREEN_MIN_X;

    if(xPos < 2){
        xPos = 2;
    }
    return xPos;
}

void ClearLine(int y){
    GotoXY(SCREEN_MIN_X, y);
    printf("                                                                                    ");
}

void ClearLineColor(int y, int backGroundColor, int textColor){
    SetAllColor(backGroundColor, textColor);
    GotoXY(SCREEN_MIN_X, y);
    printf("                                                                                    ");
}

int euc_kr_strlen(const char *str) {
    int length = 0;
    while (*str) {
        if ((*str & 0x80) != 0) { // 상위 비트가 1인 경우
            if ((unsigned char)*str >= 0xA1 && (unsigned char)*str <= 0xFE) {
                // EUC-KR 인코딩에서 한글 문자 또는 특수 문자 범위인 경우
                str++; // 다음 바이트로 건너뜁니다.
                length++;
            }
        }
        length++;
        str++;
    }
    return length;
}