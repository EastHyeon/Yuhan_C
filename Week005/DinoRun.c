#include <stdio.h>
#include <windows.h>
#include <stdbool.h>
#include <conio.h>
#include <stdlib.h>

struct Player{
    bool isDead;
    bool isGrounded;
    int x;
    int y;

};

void init();
void render_title();
void render_help();
void render_background();
void gotoxy(int, int);

int keyCode = 0;

int main(){
    int game_state = 1;

    init();

    while (game_state != 0)
    {
        //ȭ�� ����
        system("cls");
        // input ������ �Է� ���� �޴´�.
        // game_state�� ���� ����� �޴��� �����Ѵ�.
        // game_state == 1 : Ÿ��Ʋ �޴�
        // game_state == 2 : ���� �޴�
        // game_state == 3 : ����
        int input = 0;
        if(game_state == 1){
            render_title();
            gotoxy(0, 10);
            printf("�޴��� �����ϼ��� : ");
            scanf("%d", &input);
            if (input == 1){
                continue;
            }
            else if (input == 2){
                game_state = 2;
                continue;
            }
            else if (input == 3){
                game_state = 3;
                continue;
            }
            else{
                printf("�����մϴ�.");
                Sleep(1000);
                continue;
            }
        }
        else if(game_state == 2){
            render_help();
            gotoxy(0, 10);
            printf("�޴��� ���ư��ϱ�? (1.Yes / 2. NO) : ");
            scanf("%d", &input);
            if (input == 1){
                game_state = 1;
                continue;
            }
            else if (input == 2){
                game_state = 2;
                continue;
            }
            else{
                printf("�߸��� �Է��Դϴ�.");
                Sleep(1000);
                continue;
            }
        }
        else if(game_state == 3){
            printf("������ �����մϱ�? (1.Yes / 2. NO) : ");
            scanf("%d", &input);
            if (input == 1){
                break;
            }
            else if (input == 2){
                game_state = 1;
                continue;
            }
            else{
                printf("�߸��� �Է��Դϴ�.");
                Sleep(1000);
                continue;
            }
        }
    }
}

void init(){
    system("mode con cols=83 lines=30 | title ������ 0.1");
}

void render_title(){
    render_background();
    gotoxy(3, 2);
    printf("�� DINO GAME v0.1");
    gotoxy(3, 5);
    printf("1.���ӽ���");
    gotoxy(3, 6);
    printf("2.�÷��� ���");
    gotoxy(3, 7);
    printf("3.����");
}

void render_help(){
    render_background();
    gotoxy(3, 2);
    printf("�� ����");
    gotoxy(3, 5);
    printf("�����̽� �ٸ� ������ ����");
    gotoxy(3, 6);
    printf("�Ʒ� ����Ű�� ������ ���̱�");
    gotoxy(3, 7);
    printf("�ٰ����� ��ֹ��� ���ϼ���!");
}

void render_background(){
    puts("����������������������������������������������������������������������������������������������������������������������������������������������������������������");
    puts("��                                                                              ��");
    puts("��                                                                              ��");
    puts("��                                                                              ��");
    puts("��                                                                              ��");
    puts("��                                                                              ��");
    puts("��                                                                              ��");
    puts("��                                                                              ��");
    puts("��                                                                              ��");
    puts("����������������������������������������������������������������������������������������������������������������������������������������������������������������");
}

void gotoxy(int x, int y){
    HANDLE consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
    COORD pos = {x, y};
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
}
