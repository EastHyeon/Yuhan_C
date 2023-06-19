#define HAVE_STRUCT_TIMESPEC
#include <stdio.h>
#include <stdbool.h>
#include <Windows.h>
#include <fmod.h>
#include <opencv/cv.h>
#include <opencv/highgui.h>
#include <time.h>
#include <wchar.h>
#include <locale.h>
#include <fcntl.h>
#include <io.h>


enum ConsoleColor {
	BLACK,
	DARK_BLUE,
	DARK_GREEN,
	DARK_CYAN,
	DARK_RED,
	DARK_MAGENTA,
	DARK_YELLOW,
	GRAY,
	DARK_GRAY,
	BLUE,
	GREEN,
	CYAN,
	RED,
	MAGENTA,
	YELLOW,
	WHITE,
	DEFAULT_BACKGROUND = BLACK,
	DEFAULT_TEXT = GRAY,
};

enum TileID {
	AIR = 0,
	STONE = 1,
	WOOD = 2,
	FRUIT = 3
};

typedef struct {
	enum ConsoleColor colorTag;
	unsigned char r;
	unsigned char g;
	unsigned char b;
} Color;

Color ConsoleColors[] = {
	{BLACK, 0, 0, 0},
	{DARK_BLUE, 0, 0, 128},
	{DARK_GREEN, 0, 128, 0},
	{DARK_CYAN, 0, 128, 128},
	{DARK_RED, 128, 0, 0},
	{DARK_MAGENTA, 128, 0, 128},
	{DARK_YELLOW, 128, 128, 0},
	{GRAY, 192, 192, 192},
	{DARK_GRAY, 128, 128, 128},
	{BLUE, 0, 0, 255},
	{GREEN, 0, 255, 0},
	{CYAN, 0, 255, 255},
	{RED, 255, 0, 0},
	{MAGENTA, 255, 0, 255},
	{YELLOW, 255, 255, 0},
	{WHITE, 255, 255, 255}
};

double colorDistance(Color color1, Color color2) {
	int dr = color1.r - color2.r;
	int dg = color1.g - color2.g;
	int db = color1.b - color2.b;

	return sqrt(dr * dr + dg * dg + db * db);
}

enum ConsoleColor findClosestConsoleColor(Color inputColor) {
	Color closestColor = ConsoleColors[0];
	double minDistance = colorDistance(inputColor, closestColor);
	for (int i = 1; i < sizeof(ConsoleColors) / sizeof(ConsoleColors[0]); i++)
	{
		double distance = colorDistance(inputColor, ConsoleColors[i]);
		if (distance < minDistance) {
			minDistance = distance;
			closestColor = ConsoleColors[i];
		}
	}

	return closestColor.colorTag;
}

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


void SetConsoleScreenSize(int width, int height) {
	char commandBuffer[100];
	sprintf_s(commandBuffer, sizeof(commandBuffer), "mode con cols=%d lines=%d", width, height);
	system(commandBuffer);
}

void SetConsoleFontSize(int size)
{
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

	CONSOLE_FONT_INFOEX fontInfo = { sizeof(CONSOLE_FONT_INFOEX) };
	GetCurrentConsoleFontEx(hConsole, FALSE, &fontInfo);

	fontInfo.dwFontSize.Y = size;

	SetCurrentConsoleFontEx(hConsole, FALSE, &fontInfo);
}

void WriteSpriteToBuffer(const char* fileAdress, int width, int height, wchar_t** targetBuffer) {
	IplImage* sourceImage;
	sourceImage = cvLoadImage(fileAdress, 3);

	int bufferSize = width * height;

	IplImage* Image = cvCreateImage(cvSize(width, height), IPL_DEPTH_8U, 3);

	if (!sourceImage)
		cvSet(Image, CV_RGB(255, 0, 255), NULL);
	else
		cvResize(sourceImage, Image, CV_INTER_LINEAR);

	int lastX = 0;
	for (int i = 0; i < bufferSize; i++)
	{
		int y = i / width;
		int x = i % width;

		CvScalar value = cvGet2D(Image, y, x);

		unsigned char R = value.val[2];
		unsigned char G = value.val[1];
		unsigned char B = value.val[0];

		wchar_t source[26];

		swprintf_s(source, 26, L"\x1b[38;2;%03hhu;%03hhu;%03hhum█ \x1b[0m", R, G, B);
		wcscat_s(targetBuffer[y], width * 26 + 1, source);
		Sleep(1);
	}
}

void SetConsoleTextColor(int textColor, int backGroundColor) {
	HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(handle, (backGroundColor << 4) + textColor);
}

void SetConsoleFont() {
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

	CONSOLE_FONT_INFOEX fontInfo;
	fontInfo.cbSize = sizeof(CONSOLE_FONT_INFOEX);
	GetCurrentConsoleFontEx(hConsole, FALSE, &fontInfo);

	wcscpy_s(fontInfo.FaceName, sizeof(fontInfo.FaceName) / sizeof(wchar_t), L"NSimSun");

	SetCurrentConsoleFontEx(hConsole, FALSE, &fontInfo);

	return 0;
}

void PrintColorPixel(enum ConsoleColor color) {
	SetConsoleTextColor(DEFAULT_TEXT, color);
	printf("  ");
	SetConsoleTextColor(DEFAULT_TEXT, DEFAULT_BACKGROUND);
	
}

void SetRegVirtualTerminal(bool flag) {
	HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
	DWORD dwMode = 0;

	GetConsoleMode(hOut, &dwMode);

	if (flag)
		dwMode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
	else
		dwMode &= ~ENABLE_VIRTUAL_TERMINAL_PROCESSING;

	SetConsoleMode(hOut, dwMode);

	printf("\x1b[31mThis is red text!\x1b[0m\n");
	printf("\x1b[32mThis is green text!\x1b[0m\n");
	printf("\x1b[33mThis is yellow text!\x1b[0m\n");
	printf("\x1b[34mThis is blue text!\x1b[0m\n");
	printf("\x1b[35mThis is magenta text!\x1b[0m\n");
	printf("\x1b[36mThis is cyan text!\x1b[0m\n");
	printf("\x1b[37mThis is white text!\x1b[0m\n");

	return 0;
}

void Init() {
	SetRegVirtualTerminal(true);
	setlocale(LC_ALL, "ko_KR.utf8");
	// 콘솔창의 인코딩을 유니코드로 변경
	_setmode(_fileno(stdout), _O_U16TEXT);
	SetConsoleFont();
}

int main() {

	Init();

	unsigned char R = 50;
	unsigned char G = 50;
	unsigned char B = 120;

	int width = 10;
	int height = 10;

	wchar_t** mainBuffer = (wchar_t**)malloc(height * sizeof(wchar_t*));

	for (int i = 0; i < height; i++) {
		mainBuffer[i] = (wchar_t*)malloc(width * 26 + 1 * sizeof(wchar_t));
	}
	for (int i = 0; i < height; i++)
	{
		mainBuffer[i][0] = '\0';
	}

	WriteSpriteToBuffer("TestImage.png", 10, 10, mainBuffer);
	while (1) {
		SetCursorPosition(0, 0);
		for (int i = 0; i < 5; i++) {
			wprintf(L"%s\n", mainBuffer[i]);
		}
	}

	return 0;
}