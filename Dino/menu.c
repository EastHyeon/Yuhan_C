#include "main.h"

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
void RenderTitle(){
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
        int currentTick = GetTickCount64();
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
        int currentTick = GetTickCount64();
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