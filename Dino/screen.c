#include "main.h"

//��ũ�� ������ �ʱ�ȭ
void InitScreen(){
    GotoXY(0, 0);
    SetAllColor(DEFAULT_BACKGROUND, DEFAULT_TEXT);
    for(int i = SCREEN_MIN_Y; i <= SCREEN_MAX_Y; i++){
        GotoXY(SCREEN_MIN_X,i);
        printf("                                                                                    ");
    }
}

//��ũ�� ������ ������ �Ʒ��� ������� ȿ���� �ָ鼭 �ʱ�ȭ
void InitScreenFade(){
    SetAllColor(DEFAULT_BACKGROUND, DEFAULT_TEXT);
    GotoXY(0, 0);
    for(int i = SCREEN_MIN_Y; i <= SCREEN_MAX_Y; i++){
        GotoXY(SCREEN_MIN_X,i);
        printf("                                                                                    ");
        Sleep(20);
    }
}
//��ũ�� ������ ������ �ٲٰ� ������ �Ʒ��� ������� ȿ���� �ָ鼭 �ʱ�ȭ
void InitScreenFadeColor(int backGroundColor, int textColor){
    GotoXY(0, 0);
    SetAllColor(backGroundColor, textColor);
    for(int i = SCREEN_MIN_Y; i <= SCREEN_MAX_Y; i++){
        GotoXY(SCREEN_MIN_X,i);
        printf("                                                                                    ");
        Sleep(20);
    }
}

//�⺻ ����� �ʱ�ȭ
void InitBackGround(){
    GotoXY(0, 0);
    SetAllColor(GRAY, BLACK);
    GotoXY(0, SCREEN_MIN_Y - 1);
    puts("������������������������������������������������������������������������������������������������������������������������������������������������������������������������������");
    for(int i = SCREEN_MIN_Y; i <= SCREEN_MAX_Y; i++){
        puts("��                                                                                     ��");
    }
    puts("������������������������������������������������������������������������������������������������������������������������������������������������������������������������������");
    puts("������������������������������������������������������������������������������������������������������������������������������������������������������������������������������");
    for(int i = 0; i < 3; i++){
        puts("��                                                                                     ��");
    }
    puts("������������������������������������������������������������������������������������������������������������������������������������������������������������������������������");
    WriteLineCenter(GAME_VERSION, 34);
    SetAllColor(DEFAULT_BACKGROUND, DEFAULT_TEXT);
    InitScreen();
}