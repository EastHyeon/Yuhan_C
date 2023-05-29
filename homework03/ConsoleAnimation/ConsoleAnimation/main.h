#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <Windows.h>

#define SCREEN_WIDTH 500
#define SCREEN_HEIGHT 120
#define ARRAY_SIZE (SCREEN_WIDTH / FONT_RATIO) * (SCREEN_HEIGHT / FONT_RATIO) + SCREEN_HEIGHT / FONT_RATIO
#define FONT_RATIO 5

void InitializeMenu();
void PrintTitleScreen(void);