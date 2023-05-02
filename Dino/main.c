#include "main.h"

void Init(void);
void InitDebug(void);

int main(){
    enum GameState gameState;
    int lastTick = 0;
    bool DebugMode;

    system("cls");
    system("mode con cols=87 lines=38 | title 디버그 모드 | Color 70");
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
    system("title 디노게임 0.1");


    if(DebugMode){
        InitDebug();   
        gameState = MAZE;
    } else {
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
            default:
                gameState = MainMenu();
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