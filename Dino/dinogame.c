#include "main.h"

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

        //print Player
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

        // 1000/4 Tick 마다 다리 애니메이션 갱신
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