/**********************************************/
/*                                            */
/*     HomeWork Project                       */
/*     202327005 Kim Dong Hyeon               */
/*                                            */
/*     Changes                                */
/*     +WriteStringToBufferWithPointer()      */
/*     +WriteStringToBuffer() (Use 2D Array)  */
/*     -Delete Game Logic                     */
/*                                            */
/**********************************************/

#include <stdio.h>
#include <windows.h>
#include <string.h>
#include <stdbool.h>

#define SCREEN_MAX_Y 30
#define SCREEN_MAX_X 80
#define ARRAY_SIZE SCREEN_MAX_Y * SCREEN_MAX_X + SCREEN_MAX_Y
#define GROUND_HEIGHT SCREEN_MAX_Y - 2
#define MAX_JUMP_HEIGHT SCREEN_MAX_Y - 12

#define VK_W 0x57
#define VK_A 0x41
#define VK_S 0x53
#define VK_D 0x44

enum GameState MainMenu(void);
enum GameState HelpMenu(void);
enum GameState ExitMenu(void);
enum GameState Game(void);

void InitializeScreen(void);
void CursorHide(void);
void GotoXYZero(void);
int InputCenter(const char*, int);
void InputYX(const char*, int, int);

//Homework02 Function with pointer
int WriteStringToBufferWithPointer(const char*, char*, int , int);

//Homework02 Function
int WriteStringToBuffer(const char*, int, int);

enum GameState {
    EXIT = 0,
    EXITMENU,
    MAINMENU,
    GAME,
    HELPMENU
};

enum CurrentInput {
    KEY_Wait,
    KEY_W,
    KEY_A,
    KEY_S,
    KEY_D,
    KEY_UpArrow,
    KEY_LeftArrow,
    KEY_DownArrow,
    KEY_RightArrow,
    KEY_Spacebar,
    KEY_Enter
} currentInput;

struct Player
{
    int posX;
    int posY;
    int hp;
    bool isGrounded;
    bool isJumping;
    bool isRuning;
};

char screen2D[SCREEN_MAX_Y][SCREEN_MAX_X];

int inputValue = 0;

DWORD currentTick = 0;
DWORD lastTick, lastInputTick = 0;

int main() {
    enum GameState gameState = MAINMENU;

    system("cls");
    system("mode con cols=81 lines=31 | title Sukhee the Castaway");
    CursorHide();

    while (1) {
        InitializeScreen();

        // Limit input rate to 1 / 60 second
        // Limit frame rate to 60 frame
        currentTick = GetTickCount();
        if (currentTick - lastTick < 1000/60)
            continue;
        lastTick = currentTick;

        // Input PASS
        // Limit input rate to 1 / 15 second
        if(currentTick - lastInputTick > 1000/10){
            if (GetAsyncKeyState(VK_W) & 0x8000 || GetAsyncKeyState(VK_UP) & 0x8000) {
                currentInput = KEY_W;
            }
            if (GetAsyncKeyState(VK_S) & 0x8000 || GetAsyncKeyState(VK_DOWN) & 0x8000) {
                currentInput = KEY_S;
            }
            if (GetAsyncKeyState(VK_SPACE) & 0x8001 || GetAsyncKeyState(VK_RETURN) & 0x8001) {
                currentInput = KEY_Spacebar;
            }
            lastInputTick = currentTick;
        }

#pragma region Logic_PASS
        //Logic PASS

        switch (gameState) {
        case EXIT:
            system("cls");
            return 0;
        case EXITMENU:
            gameState = ExitMenu();
            break;
        case MAINMENU:
            gameState = MainMenu();
            break;
        case GAME:
            gameState = Game();
            break;
        case HELPMENU:
            gameState = HelpMenu();
            break;
        default:
            break;
        }
#pragma endregion

        //Render PASS
        char screen[ARRAY_SIZE];
    
        WriteStringToBuffer("WriteStringToBuffer() (2D Array Version) (2, 25)", 2, 25);

        for (int i = 0; i < ARRAY_SIZE; i++) {
            int y = i / (SCREEN_MAX_X + 1);
            int x = i % (SCREEN_MAX_X + 1);

            if (i != 0 && x == SCREEN_MAX_X)
                screen[i] = '\n';
            else
                screen[i] = screen2D[y][x];
        }
        WriteStringToBufferWithPointer("WriteStringToBufferWithPointer() Function Test (2, 26)", screen, 2, 26);
        WriteStringToBufferWithPointer("Movement to WASD or Arrow Key | Select to Spacebar or Enter", screen, 2, 28);
        

        screen[ARRAY_SIZE - 1] = '\0';
        printf("%s\n", screen);
    }
}

void GotoXYZero() {
    COORD pos = { 0, 0 };
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
}

enum GameState MainMenu() {
    InputCenter("Dino Run Game", 3);
    int xPos = InputCenter("START", 5);
    InputYX("HELP", 6, xPos);
    InputYX("EXIT", 7, xPos);

    xPos -= 2;

    int yPos = inputValue + 5;

    if (currentInput == KEY_W && yPos > 5){
        inputValue--;
    }
    if (currentInput == KEY_S && yPos < 7){
        inputValue++;
    }

    screen2D[yPos][xPos] = '>';

    if (currentInput == KEY_Spacebar) {
        int temp = inputValue;
        inputValue = 0;
        switch (temp) {
        case 0:
            return GAME;
        case 1:
            return HELPMENU;
        case 2:
            return EXITMENU;
        default:
            break;
        }
    }

    return MAINMENU;
}

enum GameState HelpMenu() {
    InputCenter("Help", 3);
    InputCenter("Created for a C language class assignment", 5);
    InputCenter("Both WASD and ArrowKeys are available", 6);
    InputCenter("A, D or Arrow Key to Move", 7);
    InputCenter("Space Key to Jump", 8);
    InputCenter("Escape Key to Game Exit", 9);

    InputCenter("Spacebar to return to the menu", 11);


    if (currentInput == KEY_Spacebar) {
        return MAINMENU;
    }

    return HELPMENU;
}

enum GameState ExitMenu() {
    InputCenter("Are you sure you want to exit?", 3);
    int xPos = InputCenter("Yes", 5);
    InputYX("No", 6, xPos);

    xPos -= 2;
    int yPos = inputValue + 5;
    if (currentInput == KEY_W && yPos > 5)
        inputValue--;
    if (currentInput == KEY_S && yPos < 6)
        inputValue++;

    screen2D[yPos][xPos] = '>';

    if (currentInput == KEY_Spacebar) {
        int temp = inputValue;
        inputValue = 0;
        switch (temp) {
        case 0:
            return EXIT;
        case 1:
            return MAINMENU;
        default:
            break;
        }
    }

    return EXITMENU;
}

enum GameState Game() {
    InputCenter("Game is not ready", 3);
    InputCenter("Press Spacebar to return to the menu", 5);
    if (currentInput == KEY_Spacebar) {
        return MAINMENU;
    }

    return GAME;
}

void InitializeScreen() {
    currentInput = KEY_Wait;
    GotoXYZero();
    for (int i = 0; i < SCREEN_MAX_Y * SCREEN_MAX_X; i++) {
        int y = i / SCREEN_MAX_X;
        int x = i % SCREEN_MAX_X;
        //draw corner
        if (y == 0 && x == 0 || // 1
            x == SCREEN_MAX_X - 1 && y == 0 || // 2
            x == 0 && y == SCREEN_MAX_Y - 1 || // 3
            x == SCREEN_MAX_X - 1 && y == SCREEN_MAX_Y - 1)  // 4
            screen2D[y][x] = '#';
        else if (y == 0 || y == SCREEN_MAX_Y - 1)
            screen2D[y][x] = '-';
        else if (x == 0 || x == SCREEN_MAX_X - 1)
            screen2D[y][x] = '|';
        else
            screen2D[y][x] = ' ';
    }
    InputCenter("| 202327005 Kim Dong Hyeon |", SCREEN_MAX_Y - 1);
}

void CursorHide() {
    CONSOLE_CURSOR_INFO cursorInfo = { 0, };
    cursorInfo.dwSize = 1;
    cursorInfo.bVisible = false;
    SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cursorInfo);
}

int InputCenter(const char* msg, int y) {
    int xPos = (SCREEN_MAX_X / 2) - (strlen(msg) / 2);

    for (int i = 0; i < strlen(msg); i++) {
        screen2D[y][xPos + i] = msg[i];
    }
    return xPos;
}

void InputYX(const char* msg, int y, int x) {
    for (int i = 0; i < strlen(msg); i++) {
        screen2D[y][x + i] = msg[i];
    }
}

//Homework02 Function
int WriteStringToBufferWithPointer(const char* string, char* buffer, int x, int y){
    for (int i = 0; i < strlen(string); i++){
        buffer[i+(y*(SCREEN_MAX_X+1)+x)] = string[i];
    }
    return 0;
}

//Homework02 Function
int WriteStringToBuffer(const char* string, int x, int y){
    for (int i = 0; i < strlen(string); i++) {
        screen2D[y][x + i] = string[i];
    }
    return 0;
}