#include <stdio.h>
#include <windows.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>

#pragma region DATA_DECL
// ��������
#define GAME_VERSION "������ v0.1 | 202327005 �赿��" 
#define WAIT_TICK 1000 / 30
#define SCREEN_MIN_X 2
#define SCREEN_MAX_X 84
#define SCREEN_MIN_Y 1
#define SCREEN_MAX_Y 30

//ȭ�� ������ ���� enum
enum GameState{
    EXIT = 0,
    MENU,
    HELP,
    GAME,
    DINO,
    MAZE
};

//�ؽ�Ʈ �� ��� ���� ������ ���� enum
enum ConsoleColor{
    BLACK,
    DARK_BLUE,
    DARK_GREEN,
    DARK_SKYBLUE,
    DARK_RED,
    DARK_VOILET,
    DAKR_YELLOW,
    GRAY,
    DARK_GRAY,
    BLUE,
    GREEN,
    CYAN,
    RED,
    VIOLET,
    YELLOW,
    WHITE,
    DEFAULT_BACKGROUND = BLACK,
    DEFAULT_TEXT = GRAY,
};
#pragma endregion

#pragma region KEY
// Ű
#define KEY_UP 0x26
#define KEY_LEFT 0x25
#define KEY_DOWN 0x28
#define KEY_RIGHT 0x27

#define KEY_W 0x57
#define KEY_A 0x41
#define KEY_S 0x53
#define KEY_D 0x44

#define KEY_SPACE 0x20

#define KEY_Y 0x59
#define KEY_N 0x4E
#define KEY_ESCAPE 0x1B

#define INPUT_SENSITIVITY 100 // �Է� ����
#pragma endregion

#pragma region SCREEN_DECL
void InitScreen(void);
void InitScreenFade(void);
void InitScreenFadeColor(int, int);
void InitBackGround(void);
#pragma endregion

#pragma region RENDER_DECL
void RenderTitle(void);
void RenderAuthor(void);
void RenderTitle(void);
enum GameState GameMenu(void);
enum GameState MainMenu(void);
enum GameState HelpMenu(void);
#pragma endregion

#pragma region UTILL_DECL
void GotoXY(int, int);
void CursorHide(void);
void SetColor(int);
void SetAllColor(int, int);
int WriteCenter(const char*, int );
int WriteLineCenter(const char*, int );
int GetCenter(const char*);
void ClearLine(int);
void ClearLineColor(int, int, int);
int euc_kr_strlen(const char*);
#pragma endregion

#pragma region GAME_DECL
void PrintPos(int, int);
void PrintLog(const char*);

enum GameState Dino_Game();
void Dino_InitGame();
void Dino_PrintPlayer(int, int, int);
int Dino_ScoreScreen(int);
int Dino_GamePause();

void PrintScore(int);

enum GameState Maze_Game();
void Maze_InitGame();
void Maze_PrintPlayer(int, int, int);
int Maze_GamePause();


#define PLAYER_SPEED 1000 / 10
#define PHYSICS_TICK 1000 / 30
#define GROUND_HEIGHT 28
#define MAX_JUMP_HEIGHT GROUND_HEIGHT - 10

enum TileType{
    EMPTY,
    WALL,
    TREE,
    PLAYER,
};

enum ConsoleColor GetTileColor(enum TileType);

struct Player{
    int posX;
    int posY;
    int hp;
    int direction;
    bool isGrounded;
    bool isJumping;
};

#pragma endregion

void Init(void);
void InitDebug(void);

int main(){
    enum GameState gameState;
    int lastTick = 0;
    bool DebugMode;

    system("cls");
    system("mode con cols=88 lines=39 | title ����� ��� | Color 70");
    CursorHide();
    
    InitBackGround();
    SetAllColor(BLACK, RED);
    int xPos = WriteLineCenter("����� ���� �����մϱ�? (Y/N)", 14);
    while(1){
        if(GetAsyncKeyState(KEY_Y) & 0x8000){
            ClearLine(14);
            WriteLineCenter("����� ���� �����մϴ�.", 14);
            DebugMode = true;
            break;
        }
        else if(GetAsyncKeyState(KEY_N) & 0x8000){
            ClearLine(14);
            WriteLineCenter("�Ϲ� ���� �����մϴ�.", 14);
            DebugMode = false;
            break;
        }
    }
    Sleep(500);
    system("title ������ 0.1 | Color 70");


    if(DebugMode){
        InitDebug();   
        gameState = DINO;
    }else{
        Init();
        RenderTitle();
        gameState = MENU;
    }

    //���� ����
    while(1){
        switch(gameState){
        case EXIT:
            system("cls");
            return 0;
        case MENU:
            gameState = MainMenu();
            break;
        case GAME:
            gameState = GameMenu();
            break;
        case HELP:
            gameState = HelpMenu();
            break;
        case DINO:
            gameState = Dino_Game();
            break;
        case MAZE:
            gameState = Maze_Game();
            break;
        }
    }
}

void Init(){
    InitBackGround();
    RenderAuthor();
    InitScreenFade();
}

void InitDebug(){
    InitBackGround();
}

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
    printf("����: %s", str);
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

void RenderAuthor(){
    InitScreenFadeColor(DARK_GRAY, BLACK);

    SetAllColor(DARK_GRAY, GREEN);
    WriteLineCenter(" __ __ _____ _____ _____ _____    _____ _____ _____ _____ ", 13);
    WriteLineCenter("|  |  |  |  |  |  |  _  |   | |  |   __|  _  |     |   __|", 14);
    WriteLineCenter("|_   _|  |  |     |     | | | |  |  |  |     | | | |   __|", 15);
    WriteLineCenter("  |_| |_____|__|__|__|__|_|___|  |_____|__|__|_|_|_|_____|", 16);
    WriteLineCenter("                                                          ", 17);
    SetAllColor(DARK_GRAY, BLACK);
    Sleep(1000);
    SetAllColor(DEFAULT_BACKGROUND, DEFAULT_TEXT);
}

// ó�� Ÿ��Ʋ ȭ�� ���
void RenderTitle(void){
    InitScreenFade();
    SetAllColor(DEFAULT_BACKGROUND, WHITE);
    //�ΰ� ���
    //������ ���� õõ�� ��µǴ� ȿ���� ���� Sleep() �Լ� ���
    WriteLineCenter(":::::::::::::::::::::::::::::::::::::::::", 3); Sleep(40);
    WriteLineCenter("::'########::'####:'##::: ##::'#######:::", 4); Sleep(40);
    WriteLineCenter(":: ##:::: ##:: ##:: ####: ##: ##:::: ##::", 5); Sleep(40);
    WriteLineCenter(":: ##:::: ##:: ##:: ## ## ##: ##:::: ##::", 6); Sleep(40);
    WriteLineCenter(":: ##:::: ##:: ##:: ##. ####: ##:::: ##::", 7); Sleep(40);
    WriteLineCenter(":: ##:::: ##:: ##:: ##:. ###: ##:::: ##::", 8); Sleep(40);
    WriteLineCenter(":: ########::'####: ##::. ##:. #######:::", 9); Sleep(40);
    WriteLineCenter("::........:::....::..::::..:::.......::::", 10); Sleep(40);
    WriteLineCenter(":::::::::::::::::::::::::::::::::::::::::", 11); Sleep(40);
    WriteLineCenter("_________________________________________", 12); Sleep(40);
    WriteLineCenter("~ C��� ������ ���� ~", 14); Sleep(40);
    SetAllColor(DEFAULT_BACKGROUND, DEFAULT_TEXT);

    int count = 0;
    while(1){
        //�����̽��ٸ� ������ �ݺ����� ��������
        if(GetAsyncKeyState(VK_SPACE) & 0x8000){
            Sleep(200);
            break;
        } 

        //�����Ÿ��� ȿ��
        count++;
        if(count>40){
            SetColor(GREEN);
            WriteLineCenter(">> �����̽��ٸ� ������ ���� <<", 21);
            SetColor(DEFAULT_TEXT);
            count = 0;
        }

        count++;
        if(count>40){
            WriteLineCenter("                              ", 21);
            count = 0;
        }
        Sleep(17);
    }
}

enum GameState MainMenu(){
    InitScreen();
    InitBackGround();
    SetAllColor(DEFAULT_BACKGROUND, WHITE);
    //�ΰ� ���
    WriteLineCenter(":::::::::::::::::::::::::::::::::::::::::", 3);
    WriteLineCenter("::'########::'####:'##::: ##::'#######:::", 4);
    WriteLineCenter(":: ##:::: ##:: ##:: ####: ##: ##:::: ##::", 5);
    WriteLineCenter(":: ##:::: ##:: ##:: ## ## ##: ##:::: ##::", 6);
    WriteLineCenter(":: ##:::: ##:: ##:: ##. ####: ##:::: ##::", 7);
    WriteLineCenter(":: ##:::: ##:: ##:: ##:. ###: ##:::: ##::", 8);
    WriteLineCenter(":: ########::'####: ##::. ##:. #######:::", 9);
    WriteLineCenter("::........:::....::..::::..:::.......::::", 10);
    WriteLineCenter(":::::::::::::::::::::::::::::::::::::::::", 11);
    WriteLineCenter("_________________________________________", 12);
    SetAllColor(DEFAULT_BACKGROUND, DEFAULT_TEXT);
    
    static int choose = 1;
    int xPos = GetCenter("����") - 2;

    //���� �޴� ��ȣ ���
    WriteCenter("���� : ", 14);
    printf("%d", choose);

    //�޴� ���
    GotoXY(xPos, 20);
    printf("����");
    GotoXY(xPos, 21);
    printf("����");
    GotoXY(xPos, 22);
    printf("����");

    //������ ������ ���� ����
    int lastTick = 0;
    //�ΰ��� ������ ���� ����
    int lastInputTick = 0;
    int sensitivity = INPUT_SENSITIVITY + 50;
    bool processingInput = false;

    while(1){
        //������ ���� 30���������� ����
        int currentTick = GetTickCount();
        if(currentTick - lastTick < WAIT_TICK)
            continue;
        lastTick = currentTick;

        //Input �н�
        // �� ����Ű�� ������ ��
        if(GetAsyncKeyState(KEY_W) & 0x8000 || GetAsyncKeyState(KEY_UP) & 0x8000){
            if(currentTick - lastInputTick > sensitivity){
                choose--;
                if(choose < 1)
                    choose = 3;
                processingInput = true;
                lastInputTick = currentTick;
            }
        }
        // �Ʒ� ����Ű�� ������ ��
        else if(GetAsyncKeyState(KEY_S) & 0x8000 || GetAsyncKeyState(KEY_DOWN) & 0x8000){
            if(currentTick - lastInputTick > sensitivity){
                choose++;
                if(choose > 3)
                    choose = 1;
                processingInput = true;
                lastInputTick = currentTick;
            }
        }
        // �����̽��ٰ� ������ ��
        else if(GetAsyncKeyState(VK_SPACE) & 0x8000 ){
            if(currentTick - lastInputTick > sensitivity){
                switch (choose)
                {
                case 1:
                    SetColor(GREEN);
                    GotoXY(xPos, 20);
                    printf("����");
                    SetColor(DEFAULT_TEXT);
                    Sleep(500);
                    return GAME;
                case 2:
                    SetColor(GREEN);
                    GotoXY(xPos, 21);
                    printf("����");
                    Sleep(500);
                    SetColor(DEFAULT_TEXT);
                    return HELP;
                case 3:
                    SetColor(GREEN);
                    GotoXY(xPos, 22);
                    printf("���� (Y/N)");                 
                    while(1){
                        if(GetAsyncKeyState(KEY_Y) & 0x8000){
                            GotoXY(xPos, 22);
                            printf("���� ��...");
                            GotoXY(xPos, 22);
                            SetColor(DEFAULT_TEXT);
                            Sleep(500);
                            return EXIT;
                        }
                        else if(GetAsyncKeyState(KEY_N) & 0x8000){
                            break;
                        }
                    }
                    break;
                default: 
                    break;
                }
                lastInputTick = currentTick;
            }
            processingInput = true;
        }else{
            processingInput = false;
        }

        for(int i = 0;  i < 3; i++){
            ClearLine(i + 20);
        }

        //Render �н� 

        //���� �޴� ��ȣ ���
        WriteCenter("���� : ", 14);
        printf("%d", choose);

        //�޴� ���
        GotoXY(xPos, 20);
        printf("����");
        GotoXY(xPos, 21);
        printf("����");
        GotoXY(xPos, 22);
        printf("����");

        for(int i = 0; i < 3; i++){
            SetColor(DARK_GRAY);
            GotoXY(xPos - 4,  i + 20);
            printf("|");
            GotoXY(xPos + 15,  i + 20);
            printf("|");
            SetColor(DEFAULT_TEXT);
        }
        
        //���õǾ��ִ� �޴� ���� �ʷϻ� Ŀ���� ǥ����
        if(choose == 1){
            SetColor(DARK_GREEN);
            GotoXY(xPos - 2, 20);
            printf(">");
            SetColor(BLUE);
            GotoXY(xPos - 4, 20);
            printf("|");
            GotoXY(xPos + 15, 20);
            printf("|");            
            SetColor(DEFAULT_TEXT);
            GotoXY(xPos - 2, 21);
            printf(" ");
            GotoXY(xPos - 2, 22);
            printf(" ");
        }
        else if(choose == 2){
            SetColor(DARK_GREEN);
            GotoXY(xPos - 2, 21);
            printf(">");
            SetColor(BLUE);
            GotoXY(xPos - 4, 21);
            printf("|");         
            GotoXY(xPos + 15,  21);
            printf("|");           
            SetColor(DEFAULT_TEXT);
            GotoXY(xPos - 2, 20);
            printf(" ");
            GotoXY(xPos - 2, 22);
            printf(" ");
        }
        else if(choose == 3){
            SetColor(DARK_GREEN);
            GotoXY(xPos - 2, 22);
            printf(">");
            SetColor(BLUE);
            GotoXY(xPos - 4, 22);
            printf("|");     
            GotoXY(xPos + 15,  22);
            printf("|");                   
            SetColor(DEFAULT_TEXT);
            GotoXY(xPos - 2, 20);
            printf(" ");
            GotoXY(xPos - 2, 21);
            printf(" ");
        }
        
    }
}

enum GameState GameMenu(){
    InitScreen();
    InitBackGround();

    //�ΰ� ���
    SetAllColor(DEFAULT_BACKGROUND, DEFAULT_TEXT);
    WriteLineCenter("�÷����� ������ �����ϼ���.", 3);

    int choose = 1;
    int xPos = GetCenter("DINO") - 2;

    WriteCenter("���� : ", 5);
    printf("%d", choose);

    //�޴� ���
    GotoXY(xPos, 7);
    printf("DINO");
    GotoXY(xPos, 8);
    printf("MAZE");
    GotoXY(xPos, 9);
    printf("�޴���");

    for(int i = 0; i < 3; i++){
        SetColor(DARK_GRAY);
        GotoXY(xPos - 4,  i + 7);
        printf("|");
        GotoXY(xPos + 15,  i + 7);
        printf("|");
        SetColor(DEFAULT_TEXT);
    }

    //������ ������ ���� ����
    int lastTick = 0;
    //�ΰ��� ������ ���� ����
    int lastInputTick = 0;
    int sensitivity = INPUT_SENSITIVITY + 50;
    bool processingInput = false;
    while(1){
        //������ ���� 30���������� ����
        int currentTick = GetTickCount();
        if(currentTick - lastTick < WAIT_TICK)
            continue;
        lastTick = currentTick;
        
        //Input �н�
        // �� ����Ű�� ������ ��
        if(GetAsyncKeyState(KEY_W) & 0x8000 || GetAsyncKeyState(KEY_UP) & 0x8000){
            if(currentTick - lastInputTick > sensitivity){
                choose--;
                if(choose < 1)
                    choose = 3;
                processingInput = true;
                lastInputTick = currentTick;
            }
        }
        // �Ʒ� ����Ű�� ������ ��
        else if(GetAsyncKeyState(KEY_S) & 0x8000 || GetAsyncKeyState(KEY_DOWN) & 0x8000){
            if(currentTick - lastInputTick > sensitivity){
                choose++;
                if(choose > 3)
                    choose = 1;
                processingInput = true;
                lastInputTick = currentTick;
            }
        }
        // �����̽��ٰ� ������ ��
        else if(GetAsyncKeyState(VK_SPACE) & 0x8000 ){
                if(currentTick - lastInputTick > sensitivity){
                    switch (choose)
                    {
                    case 1:
                        SetColor(GREEN);
                        GotoXY(xPos, 7);
                        printf("���� ��");
                        SetColor(DEFAULT_TEXT);
                        Sleep(500);
                        return DINO;
                    case 2:
                        SetColor(GREEN);
                        GotoXY(xPos, 8);
                        printf("���� ��");
                        Sleep(500);
                        SetColor(DEFAULT_TEXT);
                        return MAZE;
                    case 3:
                        SetColor(GREEN);
                        GotoXY(xPos, 9);
                        printf("�޴���");
                        Sleep(500);
                        return MENU;
                }
                processingInput = true;
            }else{
                processingInput = false;
            }
        }

        for(int i = 7;  i < 10; i++){
            ClearLine(i);
        }

        //Render �н� 

        //���� �޴� ��ȣ ���
        WriteCenter("���� : ", 5);
        printf("%d", choose);

        //�޴� ���
        GotoXY(xPos, 7);
        printf("DINO");
        GotoXY(xPos, 8);
        printf("MAZE");
        GotoXY(xPos, 9);
        printf("�޴���");

        for(int i = 0; i < 3; i++){
            SetColor(DARK_GRAY);
            GotoXY(xPos - 4,  i + 7);
            printf("|");
            GotoXY(xPos + 15,  i + 7);
            printf("|");
            SetColor(DEFAULT_TEXT);
        }

        //���õǾ��ִ� �޴� ���� �ʷϻ� Ŀ���� ǥ����
        if(choose == 1){
            SetColor(DARK_GREEN);
            GotoXY(xPos - 2, 7);
            printf(">");
            SetColor(BLUE);
            GotoXY(xPos - 4, 7);
            printf("|");
            GotoXY(xPos + 15, 7);
            printf("|");              
            SetColor(DEFAULT_TEXT);
            GotoXY(xPos - 2, 8);
            printf(" ");
            GotoXY(xPos - 2, 9);
            printf(" ");
        }
        else if(choose == 2){
            SetColor(DARK_GREEN);
            GotoXY(xPos - 2, 8);
            printf(">");
            SetColor(BLUE);
            GotoXY(xPos - 4, 8);
            printf("|");
            GotoXY(xPos + 15, 8);
            printf("|");               
            SetColor(DEFAULT_TEXT);
            GotoXY(xPos - 2, 7);
            printf(" ");
            GotoXY(xPos - 2, 9);
            printf(" ");
        }
        else if(choose == 3){
            SetColor(DARK_GREEN);
            GotoXY(xPos - 2, 9);
            printf(">");
            SetColor(BLUE);
            GotoXY(xPos - 4, 9);
            printf("|");
            GotoXY(xPos + 15, 9);
            printf("|");               
            SetColor(DEFAULT_TEXT);
            GotoXY(xPos - 2, 7);
            printf(" ");
            GotoXY(xPos - 2, 8);
            printf(" ");
        }
    }
}

// �������� ��� �Լ�
enum GameState HelpMenu(void){
    InitScreen();
    int xPos = 9;
    WriteLineCenter("����", 3);
    GotoXY(xPos,6);
    printf("�Ʒ� ����Ű�� ������ ���̱�");
    GotoXY(xPos,7);
    printf("�Ʒ� ����Ű�� ������ ���̱�");
    GotoXY(xPos,8);
    printf("�ٰ����� ��ֹ��� ���ϼ���!");

    GotoXY(xPos,9);
    printf("C��� ������ ���� ���۵� �����Դϴ�.");
    GotoXY(xPos,10);
    printf("CHROME DINO ������ ������� �ϰ��ֽ��ϴ�.");
    xPos = GetCenter("��                                                    ��");
    GotoXY(xPos,24);
    puts("������������������������������������������������������������������������������������������������������������");
    WriteLineCenter("��                                                    ��", 25);
    GotoXY(xPos,26);
    puts("������������������������������������������������������������������������������������������������������������");
    WriteLineCenter("���ư����� �����̽��ٸ� �����ʽÿ�.",25);
    while(1){
        if(GetAsyncKeyState(VK_SPACE) & 1){
            ClearLine(25);
            GotoXY(xPos,24);
            SetColor(DEFAULT_TEXT);
            puts("������������������������������������������������������������������������������������������������������������");
            WriteLineCenter("��                                                    ��", 25);
            GotoXY(xPos,26);
            puts("������������������������������������������������������������������������������������������������������������");
            SetColor(GREEN);
            WriteLineCenter("�޴��� ���ư��ϴ�.", 25); 
            SetColor(DEFAULT_TEXT);
            Sleep(500);
            break;
        } 
        GotoXY(3, 2);
    }
    return MENU;
}

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

enum GameState Dino_Game(){
    struct Player player = {SCREEN_MIN_X, SCREEN_MIN_Y, 100, 0, .isJumping = false};
    enum TileType Tile[SCREEN_MAX_Y][(SCREEN_MAX_X) / 2 + 1] = {EMPTY};

    player.posX = 0;
    player.posY = GROUND_HEIGHT;

    int Score = 0;

    int TreeX = 0;

    int lastInputTick = 0;
    int lastPyisicsTick = 0;
    int lastFrameTick = 0;
    int lastAnimationTick = 0;
    int lastScoreTick = 0;

    bool legFlag = true;
    bool isDown = false;

    int random = 0;

    Dino_InitGame();

    SetAllColor(DEFAULT_BACKGROUND, DEFAULT_TEXT);

    bool playerIsAtk = 0;
    player.direction = 3;

    while(1){
        int currentTick = GetTickCount();
        if(currentTick - lastFrameTick < WAIT_TICK)
            continue;
        lastFrameTick = currentTick;
#pragma region INPUT
        //Input && Logics �н�
        if(currentTick - lastInputTick > INPUT_SENSITIVITY){
            PrintLog("�Է� ��� ��");
            if (GetAsyncKeyState(KEY_W) & 0x8000 || GetAsyncKeyState(KEY_UP) & 0x8000){
                // PrintLog("�� ����Ű ����");
                // player.direction = 0;
                //     if(player.posY > SCREEN_MIN_Y)
                //         player.posY--;
            }
            if (GetAsyncKeyState(KEY_A) & 0x8000 || GetAsyncKeyState(KEY_LEFT) & 0x8000){
                PrintLog("���� ����Ű ����");
                player.direction = 1;
                if(player.posX > 0)
                    player.posX--;      
            }
            if (GetAsyncKeyState(KEY_S) & 0x8001  || GetAsyncKeyState(KEY_DOWN) & 0x8000 ){
                PrintLog("�Ʒ��� ����Ű ����");
                isDown = true;
            }
            else{
                isDown = false;
            }
            if (GetAsyncKeyState(KEY_D) & 0x8000 || GetAsyncKeyState(KEY_RIGHT) & 0x8000){
                PrintLog("������ ����Ű ����");
                player.direction = 3;
                if(player.posX < SCREEN_MAX_X / 2 - 4)
                    player.posX++;            
            }
            if (GetAsyncKeyState(VK_ESCAPE) & 0x8000){
                int choose = Dino_GamePause();
                if(MENU == choose)
                    return MENU;
            }
            lastInputTick = currentTick;
        }
        if (GetAsyncKeyState(KEY_SPACE) & 0x8000) {
            PrintLog("�����̽� �� ����");
            if (player.isGrounded) {
                player.isJumping = true;
                player.isGrounded = false;
            }
        }
#pragma endregion

        if(currentTick - lastPyisicsTick > PHYSICS_TICK){
            if(player.isJumping){
                player.posY -= 1;
            }
            else{
                player.posY += 1;
            }

            if(player.posY >= GROUND_HEIGHT){
                player.posY = GROUND_HEIGHT;
                player.isGrounded = true;
            }

            if(player.posY <= MAX_JUMP_HEIGHT){
                player.isJumping = false;
            }

            TreeX -= 1;
            if (TreeX < 0)
            {
                random = rand() % 5 + 1;
                TreeX = SCREEN_MAX_X / 2 + 20;
            }

            lastPyisicsTick = currentTick;
        }

        int RenderX = 0;
        for(int y = 0; y < SCREEN_MAX_Y; y++){
            for(int x = 0; x < SCREEN_MAX_X / 2; x++){
                Tile[y][x] = EMPTY;
            }
        }

        for (int x = 0; x < SCREEN_MAX_X / 2; x++) {
            Tile[29][x] = WALL;
        }


        if(isDown){
            Tile[player.posY - 2][player.posX] = PLAYER;
            Tile[player.posY - 1][player.posX + 1] = PLAYER;
            Tile[player.posY - 1][player.posX + 2] = PLAYER;
            Tile[player.posY - 1][player.posX + 3] = PLAYER;
            Tile[player.posY - 2][player.posX + 1] = PLAYER;
            Tile[player.posY - 2][player.posX + 2] = PLAYER;
            Tile[player.posY - 2][player.posX + 3] = PLAYER;
            Tile[player.posY - 2][player.posX + 4] = PLAYER;
            Tile[player.posY - 3][player.posX + 3] = PLAYER;
            Tile[player.posY - 3][player.posX + 4] = PLAYER;
        }else{
            Tile[player.posY - 1][player.posX] = PLAYER;
            Tile[player.posY - 1][player.posX + 1] = PLAYER;
            Tile[player.posY - 1][player.posX + 2] = PLAYER;
            Tile[player.posY - 2][player.posX + 1] = PLAYER;
            Tile[player.posY - 2][player.posX + 2] = PLAYER;
            Tile[player.posY - 3][player.posX + 1] = PLAYER;
            Tile[player.posY - 3][player.posX + 2] = PLAYER;
            Tile[player.posY - 3][player.posX + 3] = PLAYER;
            Tile[player.posY - 4][player.posX + 2] = PLAYER;
            Tile[player.posY - 4][player.posX + 3] = PLAYER;
        }


        if (currentTick - lastAnimationTick > 1000 / 4) {
            if (legFlag) {
                legFlag = false;
            }
            else {
                legFlag = true;
            }
            lastAnimationTick = currentTick;
        }

        if (legFlag == 0) {
            Tile[player.posY][player.posX + 2] = PLAYER;
        }
        else {
            Tile[player.posY][player.posX + 1] = PLAYER;
        }

        //�浹ó��
        if (TreeX < SCREEN_MAX_X / 2) {
            if (random == 1) {
                for (int i = 0; i < 4; i++) {
                    if (Tile[GROUND_HEIGHT - i][TreeX] == PLAYER) {
                        return Dino_ScoreScreen(Score);
                    }
                    else {
                        Tile[GROUND_HEIGHT - i][TreeX] = TREE;
                    }
                }
            }
            else if (random == 2) {
                for (int i = 0; i < 6; i++) {
                    if (Tile[GROUND_HEIGHT - i][TreeX] == PLAYER) {
                        return Dino_ScoreScreen(Score);
                    }
                    else {
                        Tile[GROUND_HEIGHT - i][TreeX] = TREE;
                    }
                }
            }
            else if (random == 3) {
                for (int i = 0; i < 2; i++) {
                    for (int x = 0; x < 3; x++) {
                        if (Tile[GROUND_HEIGHT - i][TreeX + x] == PLAYER) {
                            return Dino_ScoreScreen(Score);
                        }
                        else {
                            if(TreeX + x >= 0)
                                Tile[GROUND_HEIGHT - i][TreeX + x] = TREE;
                        }
                    }
                }
            }
            else if (random == 4) {
                for (int i = 0; i < 6; i++) {
                    for (int x = 0; x < 3; x++) {
                        if (Tile[GROUND_HEIGHT - i][TreeX + x] == PLAYER) {
                            return Dino_ScoreScreen(Score);
                        } 
                        else {
                            if(TreeX + x >= 0)
                                Tile[GROUND_HEIGHT - i][TreeX + x] = TREE;
                        }
                    }
                }
            }
            else if (random == 5) {
                for (int i = 0; i < 6; i++) {
                    for (int x = 0; x < 3; x++) {
                        if (Tile[GROUND_HEIGHT - i - 4][TreeX + x] == PLAYER) {
                            return Dino_ScoreScreen(Score);
                        }
                        else {
                            if(TreeX + x >= 0)
                                Tile[GROUND_HEIGHT - i - 4][TreeX + x] = TREE;
                        }
                    }
                }
            }
        }

        if (currentTick - lastScoreTick > 2000) {
            Score++;
            PrintScore(Score);
            lastScoreTick = currentTick;
        }

        PrintPos(player.posX, player.posY);

        SetAllColor(DEFAULT_BACKGROUND, DEFAULT_TEXT);
        //Render �н�
        RenderX = 0;
        for(int y = 0; y < SCREEN_MAX_Y; y++){
            for(int x = 0; x < SCREEN_MAX_X; x++){
                // X��ǥ�� ¦���� ���� ��� (��µǴ� ����� ���簢������ ���߱� ����)
                if(x == 0 || x % 2 == 0){
                    GotoXY(x + SCREEN_MIN_X, y + SCREEN_MIN_Y);
                    SetAllColor(GetTileColor(Tile[y][RenderX]), DEFAULT_TEXT);
                    printf("  "); 
                    RenderX++;
                }
            }
            RenderX = 0;
        }
    }
    return 1;
}

void Dino_InitGame(){
    InitScreen();

    for(int i = 0; i < 3; i++){
        ClearLineColor(i + 33, GRAY, BLACK);
    }
    PrintPos(0, 0);
    PrintLog("�Է� ��� ��");
    PrintScore(0);
}

int Dino_ScoreScreen(int score) {
    PrintLog("�Ͻ�����");
    int choose = 1;
    int xPos = GetCenter("�ٽ� ����") - 2;

    static int highestScore = 0;


    //������ ������ ���� ����
    int lastTick = 0;
    //�ΰ��� ������ ���� ����
    int lastInputTick = 0;
    int sensitivity = INPUT_SENSITIVITY + 50;
    bool processingInput = false;
    int boxCenter = GetCenter("                                        ");
    SetAllColor(DARK_RED, DEFAULT_TEXT);
    for (int i = 0; i < 11; i++) {
        GotoXY(boxCenter, i + 10);
        printf("                                        "); Sleep(30);
    }
    SetAllColor(DARK_RED, WHITE);
    WriteLineCenter("GAME OVER", 11); Sleep(30);

    SetAllColor(DARK_RED, YELLOW);
    if (highestScore < score){
        highestScore = score;
        WriteLineCenter("�ְ��� ����!", 16); Sleep(30);
    }

    char buffer[256];
    sprintf_s(buffer, sizeof(buffer), "Highest Score: %d", highestScore);
    char* msg = buffer;
    if (highestScore < 10){
        GotoXY(GetCenter(msg) - 1, 13);
        printf(msg);
    }
    else
        WriteLineCenter(msg, 13);

    Sleep(30);

    sprintf_s(buffer, sizeof(buffer), "Score: %d", score);
    msg = buffer;
    if (score < 10) {
        GotoXY(GetCenter(msg) - 1, 14);
        printf(msg);
    }
    else
        WriteLineCenter(msg, 14);

    Sleep(30);

    SetAllColor(DEFAULT_BACKGROUND, DEFAULT_TEXT);
    while (1) {
        //������ ���� 30���������� ����
        int currentTick = GetTickCount();
        if (currentTick - lastTick < WAIT_TICK)
            continue;
        lastTick = currentTick;

        //Input �н�
        // �� ����Ű�� ������ ��
        if (GetAsyncKeyState(KEY_W) & 0x8000 || GetAsyncKeyState(KEY_UP) & 0x8000) {
            if (currentTick - lastInputTick > sensitivity) {
                choose--;
                if (choose < 1)
                    choose = 2;
                lastInputTick = currentTick;
            }
        }
        // �Ʒ� ����Ű�� ������ ��
        else if (GetAsyncKeyState(KEY_S) & 0x8000 || GetAsyncKeyState(KEY_DOWN) & 0x8000) {
            if (currentTick - lastInputTick > sensitivity) {
                choose++;
                if (choose > 2)
                    choose = 1;
                lastInputTick = currentTick;
            }
        }
        // �����̽��ٰ� ������ ��
        else if (GetAsyncKeyState(VK_SPACE) & 0x8000) {
            if (currentTick - lastInputTick > sensitivity) {
                switch (choose)
                {
                case 1:
                    SetAllColor(DARK_RED, GREEN);
                    WriteLineCenter("�������� ���ư��ϴ�.", 18);
                    PrintLog("�ٽ� ����");
                    SetAllColor(DEFAULT_BACKGROUND, DEFAULT_TEXT);
                    Sleep(500);
                    return DINO;
                case 2:
                    SetAllColor(DARK_RED, GREEN);
                    WriteLineCenter("���� �޴��� ���ư��ϱ�? (Y/N)", 19);
                    while (1) {
                        if (GetAsyncKeyState(KEY_Y) & 0x8000) {
                            WriteLineCenter("                              ", 19);
                            WriteLineCenter("�޴��� ���ư��ϴ�.", 19);
                            PrintLog("�޴��� �̵�");
                            Sleep(500);
                            SetAllColor(DEFAULT_BACKGROUND, DEFAULT_TEXT);
                            return MENU;
                        }
                        else if (GetAsyncKeyState(KEY_N) & 0x8000) {
                            WriteLineCenter("                              ", 19);
                            break;
                        }
                    }
                    break;
                default:
                    break;
                }
                lastInputTick = currentTick;
            }
            processingInput = true;
        }
        else {
            processingInput = false;
        }

        //Render �н� 

        //���� �޴� ��ȣ ���

        //�޴� ���
        SetAllColor(DARK_RED, WHITE);
        GotoXY(xPos, 18);
        printf("�ٽ� ����");
        GotoXY(xPos, 19);
        printf("�޴��� ���ư���");

        //���õǾ��ִ� �޴� ���� �ʷϻ� Ŀ���� ǥ����
        if (choose == 1) {
            SetAllColor(DARK_RED, GREEN);
            GotoXY(xPos - 2, 18);
            printf(">");
            SetAllColor(DARK_RED, WHITE);
            GotoXY(xPos - 2, 19);
            printf(" ");
        }
        else if (choose == 2) {
            SetAllColor(DARK_RED, GREEN);
            GotoXY(xPos - 2, 19);
            printf(">");
            SetAllColor(DARK_RED, WHITE);
            GotoXY(xPos - 2, 18);
            printf(" ");
        }
    }
}

int Dino_GamePause(){
    PrintLog("�Ͻ�����");
    int choose = 1;
    int xPos = GetCenter("���� �簳") - 2;

    //������ ������ ���� ����
    int lastTick = 0;
    //�ΰ��� ������ ���� ����
    int lastInputTick = 0;
    int sensitivity = INPUT_SENSITIVITY + 50;
    bool processingInput = false;
    int boxCenter = GetCenter("                                        ");
    SetAllColor(DARK_RED, DEFAULT_TEXT);
    for(int i = 13; i < 17; i++){
        GotoXY(boxCenter, i);
        printf("                                        ");
    }
    SetAllColor(DEFAULT_BACKGROUND, DEFAULT_TEXT);
    while(1){
        //������ ���� 30���������� ����
        int currentTick = GetTickCount();
        if(currentTick - lastTick < WAIT_TICK)
            continue;
        lastTick = currentTick;

        //Input �н�
        // �� ����Ű�� ������ ��
        if(GetAsyncKeyState(KEY_W) & 0x8000 || GetAsyncKeyState(KEY_UP) & 0x8000){
            if(currentTick - lastInputTick > sensitivity){
                choose--;
                if(choose < 1)
                    choose = 2;
                processingInput = true;
                lastInputTick = currentTick;
            }
        }
        // �Ʒ� ����Ű�� ������ ��
        else if(GetAsyncKeyState(KEY_S) & 0x8000 || GetAsyncKeyState(KEY_DOWN) & 0x8000){
            if(currentTick - lastInputTick > sensitivity){
                choose++;
                if(choose > 2)
                    choose = 1;
                processingInput = true;
                lastInputTick = currentTick;
            }
        }
        // �����̽��ٰ� ������ ��
        else if(GetAsyncKeyState(VK_SPACE) & 0x8000 ){
            if(currentTick - lastInputTick > sensitivity){
                switch (choose)
                {
                case 1:
                    SetAllColor(DARK_RED, GREEN);
                    WriteLineCenter("�������� ���ư��ϴ�.",14);
                    PrintLog("���� �簳");
                    SetAllColor(DEFAULT_BACKGROUND, DEFAULT_TEXT);
                    Sleep(500);
                    return 0;
                case 2:
                    SetAllColor(DARK_RED, GREEN);
                    WriteLineCenter("���� �޴��� ���ư��ϱ�? (Y/N)",15);
                    while(1){
                        if(GetAsyncKeyState(KEY_Y) & 0x8000){
                            WriteLineCenter("                              ",15);
                            WriteLineCenter("�޴��� ���ư��ϴ�.",15);
                            PrintLog("�޴��� �̵�");
                            Sleep(500);
                            SetAllColor(DEFAULT_BACKGROUND, DEFAULT_TEXT);
                            return MENU;
                        }
                        else if(GetAsyncKeyState(KEY_N) & 0x8000){
                            WriteLineCenter("                              ",15);
                            break;
                        }
                    }
                    break;
                default: 
                    break;
                }
                lastInputTick = currentTick;
            }
            processingInput = true;
        }else{
            processingInput = false;
        }
 
        //Render �н� 

        //���� �޴� ��ȣ ���

        //�޴� ���
        SetAllColor(DARK_RED, WHITE);
        GotoXY(xPos, 14);
        printf("�����簳");
        GotoXY(xPos, 15);
        printf("�޴��� ���ư���");
        
        //���õǾ��ִ� �޴� ���� �ʷϻ� Ŀ���� ǥ����
        if(choose == 1){
            SetAllColor(DARK_RED, GREEN);
            GotoXY(xPos - 2, 14);
            printf(">");
            SetAllColor(DARK_RED, WHITE);
            GotoXY(xPos - 2, 15);
            printf(" ");
        }
        else if(choose == 2){
            SetAllColor(DARK_RED, GREEN);
            GotoXY(xPos - 2, 15);
            printf(">");
            SetAllColor(DARK_RED, WHITE);
            GotoXY(xPos - 2, 14);
            printf(" ");
        }
    }
}

void Dino_PrintPlayer(int x, int y, int dir){
    SetAllColor(DARK_GRAY, GREEN);
    GotoXY(x, y);
    printf("  ");
    GotoXY(x, y);
    switch(dir){
    case 0:
        printf("%s", "��");
        break;
    case 1:
        printf("%s", "��");
        break;
    case 2:
        printf("%s", "��");
        break;
    case 3:
        printf("%s", "��");
        break;
    }
    SetAllColor(DEFAULT_BACKGROUND, DEFAULT_TEXT);
}

enum GameState Maze_Game() {
    enum TileType Tile[SCREEN_MAX_Y][(SCREEN_MAX_X) / 2 + 1] = {EMPTY};

    int RenderX = 0;
    for(int y = 0; y < 25; y++){
        for(int x = 0; x < 25; x++){
            if(x % 2 == 0 && y % 2 == 0)
                Tile[y][x] = WALL;
            else
                Tile[y][x] = EMPTY;
        }
    }

    while(1){
        RenderX = 0;
        for(int y = 0; y < SCREEN_MAX_Y; y++){
            for(int x = 0; x < SCREEN_MAX_X; x++){
                // X��ǥ�� ¦���� ���� ��� (��µǴ� ����� ���簢������ ���߱� ����)
                if(x == 0 || x % 2 == 0){
                    GotoXY(x + SCREEN_MIN_X, y + SCREEN_MIN_Y);
                    SetAllColor(GetTileColor(Tile[y][RenderX]), DEFAULT_TEXT);
                    printf("  "); 
                    RenderX++;
                }
            }
            RenderX = 0;
        }
    }
}

void Maze_InitGame() {
    InitScreen();
    for (int i = 0; i < 3; i++) {
        ClearLineColor(i + 33, GRAY, BLACK);
    }
    PrintPos(0, 0);
    PrintLog("�Է� ��� ��");
    PrintScore(0);
}

int Maze_ScoreScreen(int score) {
}

int Maze_GamePause() {
    PrintLog("�Ͻ�����");
    int choose = 1;
    int xPos = GetCenter("���� �簳") - 2;

    //������ ������ ���� ����
    int lastTick = 0;
    //�ΰ��� ������ ���� ����
    int lastInputTick = 0;
    int sensitivity = INPUT_SENSITIVITY + 50;
    bool processingInput = false;
    int boxCenter = GetCenter("                                        ");
    SetAllColor(DARK_RED, DEFAULT_TEXT);
    for (int i = 13; i < 17; i++) {
        GotoXY(boxCenter, i);
        printf("                                        ");
    }
    SetAllColor(DEFAULT_BACKGROUND, DEFAULT_TEXT);
    while (1) {
        //������ ���� 30���������� ����
        int currentTick = GetTickCount();
        if (currentTick - lastTick < WAIT_TICK)
            continue;
        lastTick = currentTick;

        //Input �н�
        // �� ����Ű�� ������ ��
        if (GetAsyncKeyState(KEY_W) & 0x8000 || GetAsyncKeyState(KEY_UP) & 0x8000) {
            if (currentTick - lastInputTick > sensitivity) {
                choose--;
                if (choose < 1)
                    choose = 2;
                processingInput = true;
                lastInputTick = currentTick;
            }
        }
        // �Ʒ� ����Ű�� ������ ��
        else if (GetAsyncKeyState(KEY_S) & 0x8000 || GetAsyncKeyState(KEY_DOWN) & 0x8000) {
            if (currentTick - lastInputTick > sensitivity) {
                choose++;
                if (choose > 2)
                    choose = 1;
                processingInput = true;
                lastInputTick = currentTick;
            }
        }
        // �����̽��ٰ� ������ ��
        else if (GetAsyncKeyState(VK_SPACE) & 0x8000) {
            if (currentTick - lastInputTick > sensitivity) {
                switch (choose)
                {
                case 1:
                    SetAllColor(DARK_RED, GREEN);
                    WriteLineCenter("�������� ���ư��ϴ�.", 14);
                    PrintLog("���� �簳");
                    SetAllColor(DEFAULT_BACKGROUND, DEFAULT_TEXT);
                    Sleep(500);
                    return 0;
                case 2:
                    SetAllColor(DARK_RED, GREEN);
                    WriteLineCenter("���� �޴��� ���ư��ϱ�? (Y/N)", 15);
                    while (1) {
                        if (GetAsyncKeyState(KEY_Y) & 0x8000) {
                            WriteLineCenter("                              ", 15);
                            WriteLineCenter("�޴��� ���ư��ϴ�.", 15);
                            PrintLog("�޴��� �̵�");
                            Sleep(500);
                            SetAllColor(DEFAULT_BACKGROUND, DEFAULT_TEXT);
                            return MENU;
                        }
                        else if (GetAsyncKeyState(KEY_N) & 0x8000) {
                            WriteLineCenter("                              ", 15);
                            break;
                        }
                    }
                    break;
                default:
                    break;
                }
                lastInputTick = currentTick;
            }
            processingInput = true;
        }
        else {
            processingInput = false;
        }

        //Render �н� 

        //���� �޴� ��ȣ ���

        //�޴� ���
        SetAllColor(DARK_RED, WHITE);
        GotoXY(xPos, 14);
        printf("�����簳");
        GotoXY(xPos, 15);
        printf("�޴��� ���ư���");

        //���õǾ��ִ� �޴� ���� �ʷϻ� Ŀ���� ǥ����
        if (choose == 1) {
            SetAllColor(DARK_RED, GREEN);
            GotoXY(xPos - 2, 14);
            printf(">");
            SetAllColor(DARK_RED, WHITE);
            GotoXY(xPos - 2, 15);
            printf(" ");
        }
        else if (choose == 2) {
            SetAllColor(DARK_RED, GREEN);
            GotoXY(xPos - 2, 15);
            printf(">");
            SetAllColor(DARK_RED, WHITE);
            GotoXY(xPos - 2, 14);
            printf(" ");
        }
    }
}

void Maze_PrintPlayer(int x, int y, int dir) {
    SetAllColor(DARK_GRAY, GREEN);
    GotoXY(x, y);
    printf("  ");
    GotoXY(x, y);
    switch (dir) {
    case 0:
        printf("%s", "��");
        break;
    case 1:
        printf("%s", "��");
        break;
    case 2:
        printf("%s", "��");
        break;
    case 3:
        printf("%s", "��");
        break;
    }
    SetAllColor(DEFAULT_BACKGROUND, DEFAULT_TEXT);
}

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
        if ((*str & 0x80) != 0) { // ���� ��Ʈ�� 1�� ���
            if ((unsigned char)*str >= 0xA1 && (unsigned char)*str <= 0xFE) {
                // EUC-KR ���ڵ����� �ѱ� ���� �Ǵ� Ư�� ���� ������ ���
                str++; // ���� ����Ʈ�� �ǳʶݴϴ�.
                length++;
            }
        }
        length++;
        str++;
    }
    return length;
}