#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <Windows.h>

#include "main.h"
#include "asciimedia.h"
#include "input.h"
#include "utilites.h"

FMOD_SYSTEM* FMODSystem;
int fontSize = 4;

int main() {
	enum GameState gameState = MAINMENU;
	DWORD currentTick = 0;
	DWORD lastTick, lastInputTick = 0;

	int temp = 0;

	// Idol 영상 문제있음
	char screenBuffer[BUFFER_SIZE];
	char videoName[50] = "";
	SetCursorHide();
	PrintTitleScreen();

	Sleep(1000);

	InitializeMenu();
	InitializeBuffer(screenBuffer);

	while (true)
	{
		SetCursorPosition(0, 0);
		
		switch (gameState)
		{
		case MAINMENU:
			gameState = WriteMainMenuScreen(screenBuffer);
			break;
		case VIDEOPLAY:
			break;
		default:
			break;
		}
		
		printf("%s", screenBuffer);
	}

	FMOD_System_Close(FMODSystem);
	FMOD_System_Release(FMODSystem);
	return 0;
}

void InitializeMenu() {
	system("Title C Language ASCII Video Player");
	SetConsoleTextColor(White);
	char commandMessage[90];
	SetConsoleFontSize(fontSize * FONT_RATIO);
	sprintf_s(commandMessage, sizeof(commandMessage), "mode con cols=%d lines=%d", SCREEN_WIDTH / FONT_RATIO, SCREEN_HEIGHT / FONT_RATIO);
	system(commandMessage);
}

void InitializeBuffer(char* targetBuffer) {
	for (int i = 0; i < BUFFER_SIZE; i++)
	{
		int x = i % (SCREEN_WIDTH / FONT_RATIO + 1);
		int y = i / (SCREEN_WIDTH / FONT_RATIO + 1);

		if (x == SCREEN_WIDTH / FONT_RATIO)
			targetBuffer[i] = '\n';
		else if (y == 0 && x == 0 || 
			x == SCREEN_WIDTH / FONT_RATIO - 1 && y == 0 || 
			x == SCREEN_WIDTH / FONT_RATIO - 1 && y == SCREEN_HEIGHT / FONT_RATIO - 1 ||
			x == 0 && y == SCREEN_HEIGHT / FONT_RATIO - 1)
			targetBuffer[i] = '+';
		else if (y == 0 || y == SCREEN_HEIGHT / FONT_RATIO - 1)
			targetBuffer[i] = '-';
		else if (x == 0 || x == SCREEN_WIDTH / FONT_RATIO - 1)
			targetBuffer[i] = '|';
		else
			targetBuffer[i] = ' ';
	}

	targetBuffer[BUFFER_SIZE - 1] = '\0';
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


enum GameState WriteMainMenuScreen(char* targetBuffer) {
	char buffer[60];
	sprintf_s(buffer, sizeof(buffer), "C Language ASCII Video Player");
	WriteStringCenterToBuffer("C Language ASCII Video Player", targetBuffer, 2);
	return MAINMENU;
}

enum GameState PlayVideo(const char* videoName) {
	PlayASCIIVideo("Felix Colgrave - Donks", FMODSystem, SCREEN_HEIGHT, SCREEN_WIDTH, &fontSize);
	InitializeMenu();
	return MAINMENU;
}

int WriteStringCenterToBuffer(const char* string, char* targetBuffer, int y) {
	int x = SCREEN_WIDTH / FONT_RATIO / 2 - strlen(string) / 2;

	for (int i = 0; i < strlen(string); i++) {
		targetBuffer[i + (y * (SCREEN_WIDTH / FONT_RATIO + 1) + x)] = string[i];
	}

	return x;
}

void WriteStringToBuffer(const char* string, char* targetBuffer, int x, int y) {
	for (int i = 0; i < strlen(string); i++) {
		targetBuffer[i + (y * (SCREEN_WIDTH + 1) + x)] = string[i];
	}
}
