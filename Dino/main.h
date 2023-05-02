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