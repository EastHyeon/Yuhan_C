#include <stdio.h>
#include <stdbool.h>
#include <Windows.h>

#define MS_PER_UPDATE 

enum ConsoleColor {
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

enum TileID {
	AIR = 0,
	STONE = 1,
	WOOD = 2,
	FRUIT = 3
};

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

void SetConsoleTextColor(int textColor, int backGroundColor) {
	HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(handle, (backGroundColor << 4) + textColor);
}

void ProcessInput() {

}

void Update() {

}

void Render() {

}

#define width 100
#define height 50
int main() {

	SetCursorHide();

	int TileMapBuffer[height][width * 2];
	for (int y = 0; y < height; y++)
	{
		for (int x = 0; x < 20; x++)
		{
			TileMapBuffer[y][x] = 0;
		}
	}
	bool first = true;

	DWORD currentTime = 0;
	DWORD lastTime = 0;
	while (true)
	{
		currentTime = GetTickCount64();
		//Input

		//Logic
		
		if (currentTime - lastTime > 1000) {
			for (int y = 0; y < height; y++)
			{
				for (int x = 0; x < width; x++)
				{
					TileMapBuffer[y][x] = 0;
				}
			}

			for (int i = 0; i < 15; i++)
			{
				int randX = rand() % width;
				int randY = rand() % height;

				TileMapBuffer[randY][randX] = FRUIT;
			}

			for (int i = 0; i < 5; i++)
			{
				int randX = rand() % width;
				int randY = rand() % height;

				TileMapBuffer[randY][randX] = WOOD;
			}
			lastTime = currentTime;
		}



		for (int y = 0; y < height; y++)
		{
			for (int x = 0; x < width; x++)
			{
				if(x == 0 || y == 0 || x == width - 1 || y == height - 1)
				TileMapBuffer[y][x] = STONE;
			}
		}

		//Render
		SetCursorPosition(0, 0);
		for (int y = 0; y < height; y++)
		{
			for (int x = 0; x < width; x++)
			{
				if (TileMapBuffer[y][x] == AIR) {
					printf("  ");

				}
				else if (TileMapBuffer[y][x] == STONE) {
					SetConsoleTextColor(BLACK, GRAY);
					printf("  ");
					SetConsoleTextColor(WHITE, BLACK);
				}
				else if (TileMapBuffer[y][x] == WOOD) {
					SetConsoleTextColor(WHITE, DAKR_YELLOW);
					printf("  ");
					SetConsoleTextColor(WHITE, BLACK);

				}
				else if (TileMapBuffer[y][x] == FRUIT) {
					SetConsoleTextColor(WHITE, RED);
					printf("  ");
					SetConsoleTextColor(WHITE, BLACK);
				}
			}
			printf("\n");
		}
		first = false;
	}
}