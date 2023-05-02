#include "main.h"

//스크린 영역을 초기화
void InitScreen(){
    GotoXY(0, 0);
    SetAllColor(DEFAULT_BACKGROUND, DEFAULT_TEXT);
    for(int i = SCREEN_MIN_Y; i <= SCREEN_MAX_Y; i++){
        GotoXY(SCREEN_MIN_X,i);
        printf("                                                                                    ");
    }
}

//스크린 영역을 위에서 아래로 사라지는 효과를 주면서 초기화
void InitScreenFade(){
    SetAllColor(DEFAULT_BACKGROUND, DEFAULT_TEXT);
    GotoXY(0, 0);
    for(int i = SCREEN_MIN_Y; i <= SCREEN_MAX_Y; i++){
        GotoXY(SCREEN_MIN_X,i);
        printf("                                                                                    ");
        Sleep(20);
    }
}
//스크린 영역의 색상을 바꾸고 위에서 아래로 사라지는 효과를 주면서 초기화
void InitScreenFadeColor(int backGroundColor, int textColor){
    GotoXY(0, 0);
    SetAllColor(backGroundColor, textColor);
    for(int i = SCREEN_MIN_Y; i <= SCREEN_MAX_Y; i++){
        GotoXY(SCREEN_MIN_X,i);
        printf("                                                                                    ");
        Sleep(20);
    }
}

//기본 배경을 초기화
void InitBackGround(){
    GotoXY(0, 0);
    SetAllColor(GRAY, BLACK);
    GotoXY(0, SCREEN_MIN_Y - 1);
    puts("┌─────────────────────────────────────────────────────────────────────────────────────┐");
    for(int i = SCREEN_MIN_Y; i <= SCREEN_MAX_Y; i++){
        puts("│                                                                                     │");
    }
    puts("└─────────────────────────────────────────────────────────────────────────────────────┘");
    puts("┌─────────────────────────────────────────────────────────────────────────────────────┐");
    for(int i = 0; i < 3; i++){
        puts("│                                                                                     │");
    }
    puts("└─────────────────────────────────────────────────────────────────────────────────────┘");
    WriteLineCenter(GAME_VERSION, 34);
    SetAllColor(DEFAULT_BACKGROUND, DEFAULT_TEXT);
    InitScreen();
}