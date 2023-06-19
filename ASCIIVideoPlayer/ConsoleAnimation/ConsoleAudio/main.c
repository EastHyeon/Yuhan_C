#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>
#include <time.h>

char menu[465];
char mapBuffer[100][40 * 2];
void GotoXY(int x, int y) {
	COORD pos = { x, y };
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
}
void gotoxy(int x, int y) {
	COORD Pos = { x, y };
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), Pos);
}

int writeStringToBuffer(const char* string, int x, int y)
{
	int i = 0;
	while (i < strlen(string))
	{
		GotoXY(0, 0);
		Sleep(16);
		menu[i + x + 31 * y] = string[i];
		printf("%s\n", menu);
		i++;
	}

}

int StringToBuffer(const char* string, int x, int y) {
	int i = 0;
	while (i < strlen(string))
	{
		mapBuffer[i + x][i + 40 * 2 * y] = string[i];
		i++;
	}
	return 0;
}



void move_char() {
	int playerX = 1;
	int playerY = 1;

	// 1ms

	int currentTick = 0;
	int lastTick = 0;

	while (1) {

		// Input PASS
		currentTick = GetTickCount64();
		if (currentTick - lastTick > 1000 / 20) {
			if (GetAsyncKeyState(0x41) & 0x8000) { //����
				if (playerX > 1)
					playerX = playerX - 1;
			}
			if (GetAsyncKeyState(0x44) & 0x8000) { //������
				if (playerX + 1 < 40 * 2 - 6)
					playerX = playerX + 1;
			}
			if (GetAsyncKeyState(0x57) & 0x8000) { //��
				if (playerY > 1)
					playerY = playerY - 1;
			}
			if (GetAsyncKeyState(0x53) & 0x8000) { //�Ʒ�
				if (playerY + 1 < 99)
					playerY = playerY + 1;
			}
			lastTick = currentTick;
		}

		GamePlayToBuffer("(-,-)", playerX, playerY);


		gotoxy(0, 0);
		int indexY = 0;
		while (indexY < 100) {
			printf("%s", mapBuffer[indexY]);
			printf("\n");
			indexY++;
		}
	}
}

int clear_screen()
{
	int i = 0;
	// ���ΰ� 0���� 30���� ĭ�� �� 31�� 31��°�� ���๮��

	while (i < 465)
	{
		int x = i % 31;
		int y = i / 31;

		if (x == 30)
		{
			menu[i] = '\n';
		}
		else if (x == 0 || x == 29 || y == 0 || y == 14)
		{
			menu[i] = '#';
		}
		else
		{
			menu[i] = ' ';
		}
		i = i + 1;
	}
	menu[464] = '\0';
	return 0;
}


int main()
{
	clock_t start, end;
	start = clock();
	int cpu_time;
	int inputValue = 0;
	int game_state = 1;
	clear_screen();
	writeStringToBuffer("Loding...", 6, 6);
	Sleep(2000);

	while (game_state)
	{


		GotoXY(0, 0);
		clear_screen();


		if (game_state == 1) {
			writeStringToBuffer("Eat Survival", 9, 2);
			writeStringToBuffer("V.0.1", 12, 3);
			writeStringToBuffer("1. Start Game", 5, 7);
			writeStringToBuffer("2. How to Play", 5, 8);
			writeStringToBuffer("3. Exit", 5, 9);
		}
		else if (game_state == 2) {
			writeStringToBuffer("Survive from hunger", 6, 6);
			writeStringToBuffer("Press w,a,s,d to move", 5, 8);
			writeStringToBuffer("Retrun to menu?", 8, 11);
			writeStringToBuffer("1.yes 2.no", 10, 12);
		}
		else if (game_state == 3) {
			writeStringToBuffer("Are you sure Exit?", 6, 6);
			writeStringToBuffer("1.yes 2.no", 10, 7);
		}

		GotoXY(0, 15);
		scanf_s("%d", &inputValue);
		system("cls");


		// ���θ޴����� �Է��� ó����
		if (game_state == 1) {
			if (inputValue == 1) {
				system("cls");
				while (1)
				{

					move_char();
				}

			}

			else if (inputValue == 2)
				game_state = 2;
			else if (inputValue == 3)
				game_state = 3;
		}
		else if (game_state == 2) {
			if (inputValue == 1)
				game_state = 1;
			else if (inputValue == 2)
				game_state = 2;
			else
				game_state = 2;

		}
		else if (game_state == 3) {
			if (inputValue == 1) {
				clear_screen();
				end = clock();
				cpu_time = ((int)(end - start) / CLOCKS_PER_SEC);
				char time[20];
				sprintf_s(time, sizeof(time), "%d", cpu_time);
				writeStringToBuffer("Program execution time", 3, 6);
				writeStringToBuffer(time, 25, 6);
				Sleep(1000);
				writeStringToBuffer("Exit...", 6, 9);
				Sleep(2000);
				game_state = 0;
			}
			else if (inputValue == 2)
				game_state = 1;
			else
				game_state = 3;
		}

	}

	return 0;
}