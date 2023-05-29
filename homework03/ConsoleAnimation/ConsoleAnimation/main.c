#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <Windows.h>

#include "main.h"
#include "asciimedia.h"
#include "input.h"

enum GameState {
	EXIT = 0,
	EXITMENU,
	MAINMENU,
	VIDEOPLAY,
	HELPMENU
};

FMOD_SYSTEM* FMODSystem;
int fontSize = 4;

int main() {
	enum GameState gameState = MAINMENU;
	DWORD currentTick = 0;
	DWORD lastTick, lastInputTick = 0;

	int temp = 0;


	// Idol 영상 문제있음
	char screenBuffer[ARRAY_SIZE] = "@";
	InitializeMenu();

	SetCursorHide();

	while (true)
	{
		PrintTitleScreen();

		Sleep(500);

		InitializeMenu();

		printf("Title");

		Sleep(500);

		PlayASCIIVideo("Felix Colgrave - Donks", FMODSystem, SCREEN_HEIGHT, SCREEN_WIDTH, &fontSize);
	}

	FMOD_System_Close(FMODSystem);
	FMOD_System_Release(FMODSystem);
	return 0;
}

void InitializeMenu() {
	system("title Console Animation");
	SetConsoleTextColor(White);
	char commandMessage[90];
	SetConsoleFontSize(fontSize * FONT_RATIO);
	sprintf_s(commandMessage, sizeof(commandMessage), "mode con cols=%d lines=%d | title Playing", SCREEN_WIDTH / FONT_RATIO, SCREEN_HEIGHT / FONT_RATIO);
	system(commandMessage);
}

void PrintTitleScreen() {
	char commandMessage[90];
	SetConsoleFontSize(fontSize);
	sprintf_s(commandMessage, sizeof(commandMessage), "mode con cols=%d lines=%d | title Playing", SCREEN_WIDTH, SCREEN_HEIGHT);
	system(commandMessage);

	char* buffer = (char*)malloc((SCREEN_WIDTH) * (SCREEN_HEIGHT) + SCREEN_HEIGHT);
	int height = WriteImageByWidth("Resources\\Icons\\Title.png", buffer, SCREEN_WIDTH, 0);
	SetCursorPosition(0, SCREEN_HEIGHT / 2 - height / 2);

	puts(buffer);
}