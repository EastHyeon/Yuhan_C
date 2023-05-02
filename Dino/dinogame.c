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

        // 1000/4 Tick ���� �ٸ� �ִϸ��̼� ����
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