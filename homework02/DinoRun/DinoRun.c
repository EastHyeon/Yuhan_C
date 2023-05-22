#include <stdio.h>
#include <windows.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>

#pragma region DATA_DECL
// 게임정보
#define GAME_VERSION "디노게임 v0.1 | 202327005 김동현" 
#define WAIT_TICK 1000 / 30
#define SCREEN_MIN_X 2
#define SCREEN_MAX_X 84
#define SCREEN_MIN_Y 1
#define SCREEN_MAX_Y 30

//화면 선택을 위한 enum
enum GameState{
    EXIT = 0,
    MENU,
    HELP,
    GAME,
    DINO,
    MAZE
};

//텍스트 및 배경 색상 지정을 위한 enum
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
// 키
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

#define INPUT_SENSITIVITY 100 // 입력 감도
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
    system("mode con cols=88 lines=39 | title 디버그 모드 | Color 70");
    CursorHide();
    
    InitBackGround();
    SetAllColor(BLACK, RED);
    int xPos = WriteLineCenter("디버그 모드로 실행합니까? (Y/N)", 14);
    while(1){
        if(GetAsyncKeyState(KEY_Y) & 0x8000){
            ClearLine(14);
            WriteLineCenter("디버그 모드로 실행합니다.", 14);
            DebugMode = true;
            break;
        }
        else if(GetAsyncKeyState(KEY_N) & 0x8000){
            ClearLine(14);
            WriteLineCenter("일반 모드로 실행합니다.", 14);
            DebugMode = false;
            break;
        }
    }
    Sleep(500);
    system("title 디노게임 0.1 | Color 70");


    if(DebugMode){
        InitDebug();   
        gameState = DINO;
    }else{
        Init();
        RenderTitle();
        gameState = MENU;
    }

    //메인 루프
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
    printf("상태: %s", str);
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

// 처음 타이틀 화면 출력
void RenderTitle(void){
    InitScreenFade();
    SetAllColor(DEFAULT_BACKGROUND, WHITE);
    //로고 출력
    //위에서 부터 천천히 출력되는 효과를 위해 Sleep() 함수 사용
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
    WriteLineCenter("~ C언어 과제로 만듦 ~", 14); Sleep(40);
    SetAllColor(DEFAULT_BACKGROUND, DEFAULT_TEXT);

    int count = 0;
    while(1){
        //스페이스바를 누를시 반복문을 빠져나옴
        if(GetAsyncKeyState(VK_SPACE) & 0x8000){
            Sleep(200);
            break;
        } 

        //깜빡거리는 효과
        count++;
        if(count>40){
            SetColor(GREEN);
            WriteLineCenter(">> 스페이스바를 눌러서 시작 <<", 21);
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
    //로고 출력
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
    int xPos = GetCenter("시작") - 2;

    //선택 메뉴 번호 출력
    WriteCenter("선택 : ", 14);
    printf("%d", choose);

    //메뉴 출력
    GotoXY(xPos, 20);
    printf("시작");
    GotoXY(xPos, 21);
    printf("도움말");
    GotoXY(xPos, 22);
    printf("종료");

    //프레임 제한을 위한 변수
    int lastTick = 0;
    //민감도 설정을 위한 변수
    int lastInputTick = 0;
    int sensitivity = INPUT_SENSITIVITY + 50;
    bool processingInput = false;

    while(1){
        //프레임 관리 30프레임으로 고정
        int currentTick = GetTickCount();
        if(currentTick - lastTick < WAIT_TICK)
            continue;
        lastTick = currentTick;

        //Input 패스
        // 위 방향키가 눌렸을 때
        if(GetAsyncKeyState(KEY_W) & 0x8000 || GetAsyncKeyState(KEY_UP) & 0x8000){
            if(currentTick - lastInputTick > sensitivity){
                choose--;
                if(choose < 1)
                    choose = 3;
                processingInput = true;
                lastInputTick = currentTick;
            }
        }
        // 아래 방향키가 눌렀을 때
        else if(GetAsyncKeyState(KEY_S) & 0x8000 || GetAsyncKeyState(KEY_DOWN) & 0x8000){
            if(currentTick - lastInputTick > sensitivity){
                choose++;
                if(choose > 3)
                    choose = 1;
                processingInput = true;
                lastInputTick = currentTick;
            }
        }
        // 스페이스바가 눌렸을 때
        else if(GetAsyncKeyState(VK_SPACE) & 0x8000 ){
            if(currentTick - lastInputTick > sensitivity){
                switch (choose)
                {
                case 1:
                    SetColor(GREEN);
                    GotoXY(xPos, 20);
                    printf("시작");
                    SetColor(DEFAULT_TEXT);
                    Sleep(500);
                    return GAME;
                case 2:
                    SetColor(GREEN);
                    GotoXY(xPos, 21);
                    printf("도움말");
                    Sleep(500);
                    SetColor(DEFAULT_TEXT);
                    return HELP;
                case 3:
                    SetColor(GREEN);
                    GotoXY(xPos, 22);
                    printf("종료 (Y/N)");                 
                    while(1){
                        if(GetAsyncKeyState(KEY_Y) & 0x8000){
                            GotoXY(xPos, 22);
                            printf("종료 중...");
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

        //Render 패스 

        //선택 메뉴 번호 출력
        WriteCenter("선택 : ", 14);
        printf("%d", choose);

        //메뉴 출력
        GotoXY(xPos, 20);
        printf("시작");
        GotoXY(xPos, 21);
        printf("도움말");
        GotoXY(xPos, 22);
        printf("종료");

        for(int i = 0; i < 3; i++){
            SetColor(DARK_GRAY);
            GotoXY(xPos - 4,  i + 20);
            printf("|");
            GotoXY(xPos + 15,  i + 20);
            printf("|");
            SetColor(DEFAULT_TEXT);
        }
        
        //선택되어있는 메뉴 옆에 초록색 커서를 표시함
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

    //로고 출력
    SetAllColor(DEFAULT_BACKGROUND, DEFAULT_TEXT);
    WriteLineCenter("플레이할 게임을 선택하세요.", 3);

    int choose = 1;
    int xPos = GetCenter("DINO") - 2;

    WriteCenter("선택 : ", 5);
    printf("%d", choose);

    //메뉴 출력
    GotoXY(xPos, 7);
    printf("DINO");
    GotoXY(xPos, 8);
    printf("MAZE");
    GotoXY(xPos, 9);
    printf("메뉴로");

    for(int i = 0; i < 3; i++){
        SetColor(DARK_GRAY);
        GotoXY(xPos - 4,  i + 7);
        printf("|");
        GotoXY(xPos + 15,  i + 7);
        printf("|");
        SetColor(DEFAULT_TEXT);
    }

    //프레임 제한을 위한 변수
    int lastTick = 0;
    //민감도 설정을 위한 변수
    int lastInputTick = 0;
    int sensitivity = INPUT_SENSITIVITY + 50;
    bool processingInput = false;
    while(1){
        //프레임 관리 30프레임으로 고정
        int currentTick = GetTickCount();
        if(currentTick - lastTick < WAIT_TICK)
            continue;
        lastTick = currentTick;
        
        //Input 패스
        // 위 방향키가 눌렸을 때
        if(GetAsyncKeyState(KEY_W) & 0x8000 || GetAsyncKeyState(KEY_UP) & 0x8000){
            if(currentTick - lastInputTick > sensitivity){
                choose--;
                if(choose < 1)
                    choose = 3;
                processingInput = true;
                lastInputTick = currentTick;
            }
        }
        // 아래 방향키가 눌렀을 때
        else if(GetAsyncKeyState(KEY_S) & 0x8000 || GetAsyncKeyState(KEY_DOWN) & 0x8000){
            if(currentTick - lastInputTick > sensitivity){
                choose++;
                if(choose > 3)
                    choose = 1;
                processingInput = true;
                lastInputTick = currentTick;
            }
        }
        // 스페이스바가 눌렸을 때
        else if(GetAsyncKeyState(VK_SPACE) & 0x8000 ){
                if(currentTick - lastInputTick > sensitivity){
                    switch (choose)
                    {
                    case 1:
                        SetColor(GREEN);
                        GotoXY(xPos, 7);
                        printf("개발 중");
                        SetColor(DEFAULT_TEXT);
                        Sleep(500);
                        return DINO;
                    case 2:
                        SetColor(GREEN);
                        GotoXY(xPos, 8);
                        printf("개발 중");
                        Sleep(500);
                        SetColor(DEFAULT_TEXT);
                        return MAZE;
                    case 3:
                        SetColor(GREEN);
                        GotoXY(xPos, 9);
                        printf("메뉴로");
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

        //Render 패스 

        //선택 메뉴 번호 출력
        WriteCenter("선택 : ", 5);
        printf("%d", choose);

        //메뉴 출력
        GotoXY(xPos, 7);
        printf("DINO");
        GotoXY(xPos, 8);
        printf("MAZE");
        GotoXY(xPos, 9);
        printf("메뉴로");

        for(int i = 0; i < 3; i++){
            SetColor(DARK_GRAY);
            GotoXY(xPos - 4,  i + 7);
            printf("|");
            GotoXY(xPos + 15,  i + 7);
            printf("|");
            SetColor(DEFAULT_TEXT);
        }

        //선택되어있는 메뉴 옆에 초록색 커서를 표시함
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

// 게임정보 출력 함수
enum GameState HelpMenu(void){
    InitScreen();
    int xPos = 9;
    WriteLineCenter("도움말", 3);
    GotoXY(xPos,6);
    printf("아래 방향키를 눌러서 숙이기");
    GotoXY(xPos,7);
    printf("아래 방향키를 눌러서 숙이기");
    GotoXY(xPos,8);
    printf("다가오는 장애물을 피하세요!");

    GotoXY(xPos,9);
    printf("C언어 과제를 위해 제작된 게임입니다.");
    GotoXY(xPos,10);
    printf("CHROME DINO 게임을 기반으로 하고있습니다.");
    xPos = GetCenter("│                                                    │");
    GotoXY(xPos,24);
    puts("┌────────────────────────────────────────────────────┐");
    WriteLineCenter("│                                                    │", 25);
    GotoXY(xPos,26);
    puts("└────────────────────────────────────────────────────┘");
    WriteLineCenter("돌아갈려면 스페이스바를 누르십시오.",25);
    while(1){
        if(GetAsyncKeyState(VK_SPACE) & 1){
            ClearLine(25);
            GotoXY(xPos,24);
            SetColor(DEFAULT_TEXT);
            puts("┌────────────────────────────────────────────────────┐");
            WriteLineCenter("│                                                    │", 25);
            GotoXY(xPos,26);
            puts("└────────────────────────────────────────────────────┘");
            SetColor(GREEN);
            WriteLineCenter("메뉴로 돌아갑니다.", 25); 
            SetColor(DEFAULT_TEXT);
            Sleep(500);
            break;
        } 
        GotoXY(3, 2);
    }
    return MENU;
}

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
        //Input && Logics 패스
        if(currentTick - lastInputTick > INPUT_SENSITIVITY){
            PrintLog("입력 대기 중");
            if (GetAsyncKeyState(KEY_W) & 0x8000 || GetAsyncKeyState(KEY_UP) & 0x8000){
                // PrintLog("위 방향키 눌림");
                // player.direction = 0;
                //     if(player.posY > SCREEN_MIN_Y)
                //         player.posY--;
            }
            if (GetAsyncKeyState(KEY_A) & 0x8000 || GetAsyncKeyState(KEY_LEFT) & 0x8000){
                PrintLog("왼쪽 방향키 눌림");
                player.direction = 1;
                if(player.posX > 0)
                    player.posX--;      
            }
            if (GetAsyncKeyState(KEY_S) & 0x8001  || GetAsyncKeyState(KEY_DOWN) & 0x8000 ){
                PrintLog("아래쪽 방향키 눌림");
                isDown = true;
            }
            else{
                isDown = false;
            }
            if (GetAsyncKeyState(KEY_D) & 0x8000 || GetAsyncKeyState(KEY_RIGHT) & 0x8000){
                PrintLog("오른쪽 방향키 눌림");
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
            PrintLog("스페이스 바 눌림");
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

        //충돌처리
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
        //Render 패스
        RenderX = 0;
        for(int y = 0; y < SCREEN_MAX_Y; y++){
            for(int x = 0; x < SCREEN_MAX_X; x++){
                // X좌표가 짝수일 때만 출력 (출력되는 모습을 정사각형으로 맞추기 위해)
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
    PrintLog("입력 대기 중");
    PrintScore(0);
}

int Dino_ScoreScreen(int score) {
    PrintLog("일시정지");
    int choose = 1;
    int xPos = GetCenter("다시 시작") - 2;

    static int highestScore = 0;


    //프레임 제한을 위한 변수
    int lastTick = 0;
    //민감도 설정을 위한 변수
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
        WriteLineCenter("최고기록 갱신!", 16); Sleep(30);
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
        //프레임 관리 30프레임으로 고정
        int currentTick = GetTickCount();
        if (currentTick - lastTick < WAIT_TICK)
            continue;
        lastTick = currentTick;

        //Input 패스
        // 위 방향키가 눌렸을 때
        if (GetAsyncKeyState(KEY_W) & 0x8000 || GetAsyncKeyState(KEY_UP) & 0x8000) {
            if (currentTick - lastInputTick > sensitivity) {
                choose--;
                if (choose < 1)
                    choose = 2;
                lastInputTick = currentTick;
            }
        }
        // 아래 방향키가 눌렀을 때
        else if (GetAsyncKeyState(KEY_S) & 0x8000 || GetAsyncKeyState(KEY_DOWN) & 0x8000) {
            if (currentTick - lastInputTick > sensitivity) {
                choose++;
                if (choose > 2)
                    choose = 1;
                lastInputTick = currentTick;
            }
        }
        // 스페이스바가 눌렸을 때
        else if (GetAsyncKeyState(VK_SPACE) & 0x8000) {
            if (currentTick - lastInputTick > sensitivity) {
                switch (choose)
                {
                case 1:
                    SetAllColor(DARK_RED, GREEN);
                    WriteLineCenter("게임으로 돌아갑니다.", 18);
                    PrintLog("다시 시작");
                    SetAllColor(DEFAULT_BACKGROUND, DEFAULT_TEXT);
                    Sleep(500);
                    return DINO;
                case 2:
                    SetAllColor(DARK_RED, GREEN);
                    WriteLineCenter("정말 메뉴로 돌아갑니까? (Y/N)", 19);
                    while (1) {
                        if (GetAsyncKeyState(KEY_Y) & 0x8000) {
                            WriteLineCenter("                              ", 19);
                            WriteLineCenter("메뉴로 돌아갑니다.", 19);
                            PrintLog("메뉴로 이동");
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

        //Render 패스 

        //선택 메뉴 번호 출력

        //메뉴 출력
        SetAllColor(DARK_RED, WHITE);
        GotoXY(xPos, 18);
        printf("다시 시작");
        GotoXY(xPos, 19);
        printf("메뉴로 돌아가기");

        //선택되어있는 메뉴 옆에 초록색 커서를 표시함
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
    PrintLog("일시정지");
    int choose = 1;
    int xPos = GetCenter("게임 재개") - 2;

    //프레임 제한을 위한 변수
    int lastTick = 0;
    //민감도 설정을 위한 변수
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
        //프레임 관리 30프레임으로 고정
        int currentTick = GetTickCount();
        if(currentTick - lastTick < WAIT_TICK)
            continue;
        lastTick = currentTick;

        //Input 패스
        // 위 방향키가 눌렸을 때
        if(GetAsyncKeyState(KEY_W) & 0x8000 || GetAsyncKeyState(KEY_UP) & 0x8000){
            if(currentTick - lastInputTick > sensitivity){
                choose--;
                if(choose < 1)
                    choose = 2;
                processingInput = true;
                lastInputTick = currentTick;
            }
        }
        // 아래 방향키가 눌렀을 때
        else if(GetAsyncKeyState(KEY_S) & 0x8000 || GetAsyncKeyState(KEY_DOWN) & 0x8000){
            if(currentTick - lastInputTick > sensitivity){
                choose++;
                if(choose > 2)
                    choose = 1;
                processingInput = true;
                lastInputTick = currentTick;
            }
        }
        // 스페이스바가 눌렸을 때
        else if(GetAsyncKeyState(VK_SPACE) & 0x8000 ){
            if(currentTick - lastInputTick > sensitivity){
                switch (choose)
                {
                case 1:
                    SetAllColor(DARK_RED, GREEN);
                    WriteLineCenter("게임으로 돌아갑니다.",14);
                    PrintLog("게임 재개");
                    SetAllColor(DEFAULT_BACKGROUND, DEFAULT_TEXT);
                    Sleep(500);
                    return 0;
                case 2:
                    SetAllColor(DARK_RED, GREEN);
                    WriteLineCenter("정말 메뉴로 돌아갑니까? (Y/N)",15);
                    while(1){
                        if(GetAsyncKeyState(KEY_Y) & 0x8000){
                            WriteLineCenter("                              ",15);
                            WriteLineCenter("메뉴로 돌아갑니다.",15);
                            PrintLog("메뉴로 이동");
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
 
        //Render 패스 

        //선택 메뉴 번호 출력

        //메뉴 출력
        SetAllColor(DARK_RED, WHITE);
        GotoXY(xPos, 14);
        printf("게임재개");
        GotoXY(xPos, 15);
        printf("메뉴로 돌아가기");
        
        //선택되어있는 메뉴 옆에 초록색 커서를 표시함
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
        printf("%s", "▲");
        break;
    case 1:
        printf("%s", "◀");
        break;
    case 2:
        printf("%s", "▼");
        break;
    case 3:
        printf("%s", "▶");
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
                // X좌표가 짝수일 때만 출력 (출력되는 모습을 정사각형으로 맞추기 위해)
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
    PrintLog("입력 대기 중");
    PrintScore(0);
}

int Maze_ScoreScreen(int score) {
}

int Maze_GamePause() {
    PrintLog("일시정지");
    int choose = 1;
    int xPos = GetCenter("게임 재개") - 2;

    //프레임 제한을 위한 변수
    int lastTick = 0;
    //민감도 설정을 위한 변수
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
        //프레임 관리 30프레임으로 고정
        int currentTick = GetTickCount();
        if (currentTick - lastTick < WAIT_TICK)
            continue;
        lastTick = currentTick;

        //Input 패스
        // 위 방향키가 눌렸을 때
        if (GetAsyncKeyState(KEY_W) & 0x8000 || GetAsyncKeyState(KEY_UP) & 0x8000) {
            if (currentTick - lastInputTick > sensitivity) {
                choose--;
                if (choose < 1)
                    choose = 2;
                processingInput = true;
                lastInputTick = currentTick;
            }
        }
        // 아래 방향키가 눌렀을 때
        else if (GetAsyncKeyState(KEY_S) & 0x8000 || GetAsyncKeyState(KEY_DOWN) & 0x8000) {
            if (currentTick - lastInputTick > sensitivity) {
                choose++;
                if (choose > 2)
                    choose = 1;
                processingInput = true;
                lastInputTick = currentTick;
            }
        }
        // 스페이스바가 눌렸을 때
        else if (GetAsyncKeyState(VK_SPACE) & 0x8000) {
            if (currentTick - lastInputTick > sensitivity) {
                switch (choose)
                {
                case 1:
                    SetAllColor(DARK_RED, GREEN);
                    WriteLineCenter("게임으로 돌아갑니다.", 14);
                    PrintLog("게임 재개");
                    SetAllColor(DEFAULT_BACKGROUND, DEFAULT_TEXT);
                    Sleep(500);
                    return 0;
                case 2:
                    SetAllColor(DARK_RED, GREEN);
                    WriteLineCenter("정말 메뉴로 돌아갑니까? (Y/N)", 15);
                    while (1) {
                        if (GetAsyncKeyState(KEY_Y) & 0x8000) {
                            WriteLineCenter("                              ", 15);
                            WriteLineCenter("메뉴로 돌아갑니다.", 15);
                            PrintLog("메뉴로 이동");
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

        //Render 패스 

        //선택 메뉴 번호 출력

        //메뉴 출력
        SetAllColor(DARK_RED, WHITE);
        GotoXY(xPos, 14);
        printf("게임재개");
        GotoXY(xPos, 15);
        printf("메뉴로 돌아가기");

        //선택되어있는 메뉴 옆에 초록색 커서를 표시함
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
        printf("%s", "▲");
        break;
    case 1:
        printf("%s", "◀");
        break;
    case 2:
        printf("%s", "▼");
        break;
    case 3:
        printf("%s", "▶");
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