#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <Windows.h>

#define SCREEN_WIDTH 500
#define SCREEN_HEIGHT 160
#define FONT_RATIO 4
#define BUFFER_SIZE SCREEN_WIDTH / FONT_RATIO * SCREEN_HEIGHT / FONT_RATIO + SCREEN_HEIGHT / FONT_RATIO

enum GameState {
	QUIT = 0,
	QUITMENU,
	MAINMENU,
	VIDEOPLAY,
	HELPMENU
};

void InitializeMenu();
void InitializeBuffer(char*);
void PrintTitleScreen(void);
enum GameState WriteMainMenuScreen(char*);
enum GameState WriteHelpMenuScreen(char*);
enum GameState PlayVideo(const char*);
int WriteStringCenterToBuffer(const char*, char*, int);
void WriteStringToBufferWithLimit(const char*, char*, int, int, int);
void WriteStringToBuffer(const char*, char*, int, int);
void SearchFiles(const char*, char[][50], int);