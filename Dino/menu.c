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

// ó�� Ÿ��Ʋ ȭ�� ���
void RenderTitle(){
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
        int currentTick = GetTickCount64();
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
        int currentTick = GetTickCount64();
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