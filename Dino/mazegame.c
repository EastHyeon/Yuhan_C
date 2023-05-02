#include "main.h"

enum GameState Maze_Game() {
    struct Player player = {SCREEN_MIN_X, SCREEN_MIN_Y, 100, 0, .isJumping = false};
    enum TileType Tile[SCREEN_MAX_Y][(SCREEN_MAX_X) / 2 + 1] = {EMPTY};

    int lastInputTick = 0;
    int lastFrameTick = 0;
    int lastAnimationTick = 0;
    int RenderX = 0;

    bool isGenerated = false;
    
    int score = 0;

    int DestX = 23;
    int DestY = 23;

    player.posX = 1;
    player.posY = 1;

    Maze_InitGame();

    //render
    while(1){
        int currentTick = GetTickCount();
        if(currentTick - lastFrameTick < WAIT_TICK)
            continue;
        lastFrameTick = currentTick;

        if(currentTick - lastInputTick > INPUT_SENSITIVITY){
            PrintLog("�Է� ��� ��");
            if (GetAsyncKeyState(KEY_W) & 0x8000 || GetAsyncKeyState(KEY_UP) & 0x8000){
                PrintLog("�� ����Ű ����");
                player.direction = 0;
                    if(player.posY > SCREEN_MIN_Y  && Tile[player.posY - 1][player.posX] == EMPTY)
                        player.posY--;
            }
            else if (GetAsyncKeyState(KEY_A) & 0x8000 || GetAsyncKeyState(KEY_LEFT) & 0x8000){
                PrintLog("���� ����Ű ����");
                player.direction = 1;
                if(player.posX > 0  && Tile[player.posY][player.posX - 1] == EMPTY)
                    player.posX--;      
            }
            else if (GetAsyncKeyState(KEY_S) & 0x8001  || GetAsyncKeyState(KEY_DOWN) & 0x8000 ){
                PrintLog("�Ʒ� ����Ű ����");
                player.direction = 2;
                    if(player.posY < SCREEN_MAX_Y  && Tile[player.posY + 1][player.posX] == EMPTY)
                        player.posY++;
            }
            else if (GetAsyncKeyState(KEY_D) & 0x8000 || GetAsyncKeyState(KEY_RIGHT) & 0x8000){
                PrintLog("������ ����Ű ����");
                player.direction = 3;
                if(player.posX < SCREEN_MAX_X / 2 - 4 && Tile[player.posY][player.posX + 1] == EMPTY)
                    player.posX++;            
            }
            if (GetAsyncKeyState(VK_ESCAPE) & 0x8000){
                if(MENU == Maze_GamePause())
                    return MENU;
            }
            lastInputTick = currentTick;
        }
        //logics
        //Generate Maze

        if(!isGenerated){
            for(int y = 0; y < 29; y++){
                for(int x = 0; x < 29; x++){
                    if(x % 2 == 0 || y % 2 == 0)
                        Tile[y][x] = WALL;
                    else
                        Tile[y][x] = EMPTY;
                }
            }

            for(int y = 0; y < 29; y++){
                int count = 1;
                for(int x = 0; x < 29; x++){
                    if(x % 2 == 0 || y % 2 == 0)
                        continue;

                    if(y == 29 - 2 && x == 29 - 2)
                        continue;

                    if(y == 29 - 2){
                        Tile[y][x + 1] = EMPTY;
                        continue;
                    }

                    if(x == 29 - 2){
                        Tile[y + 1][x] = EMPTY;
                        continue;
                    }
                    
                    if (rand() % 2 == 0){
                        Tile[y][x + 1] = EMPTY;
                        count++;
                    }
                    else{
                        int randIndex = rand() % count;
                        Tile[y + 1][x - randIndex * 2] = EMPTY;
                        count = 1;
                    }
                }
            }
            DestX = 29 - 2;
            DestY = 29 - 2;
            isGenerated = true;
        }
       
       if(player.posX == DestX && player.posY == DestY){
            player.posX = 1;
            player.posY = 1;
            isGenerated = false;
            score++;
       }
        
        PrintScore(score);
        PrintPos(player.posX, player.posY);
        RenderX = 0;
        for(int y = 0; y < SCREEN_MAX_Y; y++){
            for(int x = 0; x < SCREEN_MAX_X; x++){
                // X��ǥ�� ¦���� ���� ��� (��µǴ� ����� ���簢������ ���߱� ����)
                if(x == 0 || x % 2 == 0){
                    if(y == player.posY && RenderX == player.posX){
                        SetAllColor(GREEN, DEFAULT_TEXT);
                    }
                    else if(y == DestY && RenderX == DestX){
                        SetAllColor(YELLOW, DEFAULT_TEXT);
                    } 
                    else {
                        GotoXY(x + SCREEN_MIN_X, y + SCREEN_MIN_Y);
                        SetAllColor(GetTileColor(Tile[y][RenderX]), DEFAULT_TEXT);
                    }
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
    for(int i = 0; i < 3; i++){
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
        //������ ���� 29���������� ����
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

