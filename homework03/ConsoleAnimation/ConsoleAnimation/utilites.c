#pragma once
#include "main.h"
#include "utilites.h"

void SetCursorPosition(int x, int y) {
	COORD pos = { x, y };
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
}

void SetCursorHide() {
	CONSOLE_CURSOR_INFO cursorInfo = { 0, };
	cursorInfo.dwSize = 1;
	cursorInfo.bVisible = false;
	SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cursorInfo);
}

void SetConsoleFontSize(int size)
{
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

	CONSOLE_FONT_INFOEX fontInfo = { sizeof(CONSOLE_FONT_INFOEX) };
	GetCurrentConsoleFontEx(hConsole, FALSE, &fontInfo);

	fontInfo.dwFontSize.Y = size; // 폰트의 세로 크기를 변경합니다.

	SetCurrentConsoleFontEx(hConsole, FALSE, &fontInfo);
}

void SetConsoleTextColor(int colorNum) {
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), colorNum);
}