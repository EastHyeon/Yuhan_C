#include "main.h"
#include "asciimedia.h"
#include "input.h"
#include "utilites.h"

int fontSize = 4;
char* selectedVideoName;


int main() {
	enum GameState gameState = MAINMENU;
	DWORD currentTick = 0;
	DWORD lastTick, lastInputTick = 0;

	int temp = 0;

	// Idol 영상 문제있음
	char screenBuffer[BUFFER_SIZE];
	char videoName[50] = "";
	SetCursorHide();
	PlaySimpleASCIIVideo("Title.mp4", SCREEN_WIDTH, SCREEN_HEIGHT, &fontSize);
	Sleep(500);

	InitializeMenu();

	while (true)
	{
		switch (gameState)
		{
		case MAINMENU:
			gameState = WriteMainMenuScreen(screenBuffer);
			break;
		case HELPMENU:
			gameState = WriteHelpMenuScreen(screenBuffer);
			break;
		case VIDEOPLAY:
			gameState = PlayVideo(selectedVideoName);
			break;
		case QUITMENU:
			gameState = WriteQuitMenuScreen(screenBuffer);
			break;
		case QUIT:
			system("cls");
			return 0;
		default:
			break;
		}
	}
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
	InitializeBuffer(targetBuffer);

	int videoCount = 0;
	int inputSelect = 0;

	bool isSelectVideo = false;

	char videoNameBuffer[10][50];

	SearchFiles("Resources\\Videos\\", videoNameBuffer, 10);

	for (int i = 0; i <= 10; i++)
	{
		videoCount = i - 1;
		if (videoNameBuffer[i][0] == '\0') {
			if (videoCount < 0) {
				videoCount = 0;
			}
			break;
		}
	}

	DWORD currentTick = GetTickCount64();
	DWORD lastTick = currentTick;
	while (true) {
		currentTick = GetTickCount64();
		if (currentTick - lastTick > 1000 / 5) {
			if (GetAsyncKeyState(VK_W) & 0x8001 || GetAsyncKeyState(VK_UP) & 0x8001) {
				if (isSelectVideo) {
					if (inputSelect - 1 >= 0)
						inputSelect--;
				}
				else {
					if(inputSelect - 1 >= 0)
						inputSelect--;
				}
			}
			if (GetAsyncKeyState(VK_S) & 0x8001 || GetAsyncKeyState(VK_DOWN) & 0x8001) {
				if (isSelectVideo) {
					if (inputSelect + 1 <= videoCount)
						inputSelect++;
				}
				else {
					if (inputSelect + 1 <= 2)
						inputSelect++;
				}
			}
			if (GetAsyncKeyState(VK_RETURN) & 0x8001 || GetAsyncKeyState(VK_SPACE) & 0x8001) {
				if (isSelectVideo) {
					selectedVideoName = videoNameBuffer[inputSelect];
					return VIDEOPLAY;
				}
				else {
					switch (inputSelect)
					{
					case 0:
						inputSelect = 0;
						isSelectVideo = true;
						break;
					case 1:
						return HELPMENU;
					case 2:
						return QUITMENU;
					default:
						break;
					}
				}
			}
			if (GetAsyncKeyState(VK_ESCAPE) & 0x8001) {
				if (isSelectVideo) {
					inputSelect = 0;
					isSelectVideo = false;
				}
			}
			lastTick = currentTick;
		}


		InitializeBuffer(targetBuffer);
		WriteStringCenterToBuffer("C Language ASCII Video Player", targetBuffer, 2);
		char inputBuffer[50];
		sprintf_s(inputBuffer, sizeof(inputBuffer), "Select : %d", inputSelect);
		WriteStringCenterToBuffer(inputBuffer, targetBuffer, 4);

		int xPos = 0;
		WriteStringCenterToBuffer("+----------------------------------------+", targetBuffer, 7);
		for (int i = 0; i < 10; i++)
		{
			xPos = WriteStringCenterToBuffer("|                                        |", targetBuffer, 8 + i);
		}
		for (int i = 0; i <= videoCount; i++)
		{
			WriteStringToBufferWithLimit(videoNameBuffer[i], targetBuffer, xPos + 2, 8 + i, 35);
			if(strlen(videoNameBuffer[i]) > 35)
				WriteStringToBuffer("...", targetBuffer, xPos + 2 + 35, 8 + i);
		}
		WriteStringCenterToBuffer("+----------------------------------------+", targetBuffer, 18);

		WriteStringToBuffer("Videos", targetBuffer, xPos, 6);

		WriteStringToBuffer("Help", targetBuffer, xPos, 20);

		WriteStringToBuffer("Quit", targetBuffer, xPos, 22);

		if (isSelectVideo) {
			targetBuffer[(8 + inputSelect) * (SCREEN_WIDTH / FONT_RATIO + 1) + (xPos + 1)] = '[';
			targetBuffer[(8 + inputSelect) * (SCREEN_WIDTH / FONT_RATIO + 1) + (xPos + 40)] = ']';
		}
		else {
			switch (inputSelect)
			{
			case 0:
				targetBuffer[6 * (SCREEN_WIDTH / FONT_RATIO + 1) + (xPos - 2)] = '>';
				targetBuffer[20 * (SCREEN_WIDTH / FONT_RATIO + 1) + (xPos - 2)] = ' ';
				targetBuffer[22 * (SCREEN_WIDTH / FONT_RATIO + 1) + (xPos - 2)] = ' ';
				break;
			case 1:
				targetBuffer[6 * (SCREEN_WIDTH / FONT_RATIO + 1) + (xPos - 2)] = ' ';
				targetBuffer[20 * (SCREEN_WIDTH / FONT_RATIO + 1) + (xPos - 2)] = '>';
				targetBuffer[22 * (SCREEN_WIDTH / FONT_RATIO + 1) + (xPos - 2)] = ' ';
				break;
			case 2:
				targetBuffer[6 * (SCREEN_WIDTH / FONT_RATIO + 1) + (xPos - 2)] = ' ';
				targetBuffer[20 * (SCREEN_WIDTH / FONT_RATIO + 1) + (xPos - 2)] = ' ';
				targetBuffer[22 * (SCREEN_WIDTH / FONT_RATIO + 1) + (xPos - 2)] = '>';
				break;
			default:
				break;
			}
		}

		SetCursorPosition(0, 0);
		printf("%s", targetBuffer);
	}

	return MAINMENU;
}

enum GameState WriteHelpMenuScreen(char* targetBuffer) {
	InitializeBuffer(targetBuffer);
	int inputSelect = 0;

	bool isSelectVideo = false;

	char commandMessage[80];

	DWORD currentTick = GetTickCount64();
	DWORD lastTick = currentTick;
	while (true) {
		currentTick = GetTickCount64();
		if (currentTick - lastTick > 1000 / 5) {
			if (GetAsyncKeyState(VK_RETURN) & 0x8001 || GetAsyncKeyState(VK_SPACE) & 8001) {
				return MAINMENU;
			}
			lastTick = currentTick;
		}
		
		WriteStringCenterToBuffer("Help", targetBuffer, 2);
		
		WriteStringToBuffer("This program was developed to convert video to ASCII art and output it to a command prompt window.", targetBuffer, 10, 4);
		WriteStringToBuffer("SpaceBar:  Play/Pause", targetBuffer, 10, 6);
		WriteStringToBuffer("Left/Right arrow:  Seek backward/forward 5 seconds.", targetBuffer, 10, 7);
		WriteStringToBuffer("Up/Down arrow:  Increase/Decrease volume 10%.", targetBuffer, 10, 8);
		WriteStringToBuffer("[ / ]:  Increase/Decrease FontSize.", targetBuffer, 10, 9);
		WriteStringToBuffer("Escapse:  Return to main menu.", targetBuffer, 10, 10);

		WriteStringToBuffer("Developed by Kim Dong Hyeon(202327005)", targetBuffer, 10, 12);

		SetCursorPosition(0, 0);
		printf("%s", targetBuffer);
	}

	return HELPMENU;
}
enum GameState WriteQuitMenuScreen(char* targetBuffer) {
	InitializeBuffer(targetBuffer);

	int inputSelect = 0;

	DWORD currentTick = GetTickCount64();
	DWORD lastTick = currentTick;
	while (true) {
		currentTick = GetTickCount64();
		if (currentTick - lastTick > 1000 / 5) {
			if (GetAsyncKeyState(VK_W) & 0x8001 || GetAsyncKeyState(VK_UP) & 0x8001) {
				if (inputSelect - 1 >= 0)
					inputSelect--;
			}
			if (GetAsyncKeyState(VK_S) & 0x8001 || GetAsyncKeyState(VK_DOWN) & 0x8001) {
				if (inputSelect + 1 <= 1)
					inputSelect++;
			}
			if (GetAsyncKeyState(VK_RETURN) & 0x8001 || GetAsyncKeyState(VK_SPACE) & 0x8001) {
				switch (inputSelect)
				{
				case 0:
					return QUIT;
				case 1:
					return MAINMENU;
				default:
					break;
				}
			}
			lastTick = currentTick;
		}

		WriteStringCenterToBuffer("Are you sure quit?", targetBuffer, 2);

		switch (inputSelect)
		{
		case 0:
			WriteStringCenterToBuffer("[      ]", targetBuffer, 4);
			WriteStringCenterToBuffer("        ", targetBuffer, 5);
			break;
		case 1:
			WriteStringCenterToBuffer("        ", targetBuffer, 4);
			WriteStringCenterToBuffer("[      ]", targetBuffer, 5);
			break;
		default:
			break;
		}

		WriteStringCenterToBuffer("Quit", targetBuffer, 4);
		WriteStringCenterToBuffer("Cancel", targetBuffer, 5);

		SetCursorPosition(0, 0);
		printf("%s", targetBuffer);
	}

	return QUITMENU;
}


enum GameState PlayVideo(const char* videoName) {
	int flag = 1;
	while (flag)
	{
		flag = PlayASCIIVideo(videoName, SCREEN_WIDTH, SCREEN_HEIGHT, &fontSize);
	}
	InitializeMenu();
	return MAINMENU;
}

int WriteStringCenterToBuffer(const char* string, char* targetBuffer, int y) {
	int x = SCREEN_WIDTH / FONT_RATIO / 2 - strlen(string) / 2;

	for (int i = 0; i < strlen(string); i++) {
		if (string[i] == '\0')
			break;
		targetBuffer[i + (y * (SCREEN_WIDTH / FONT_RATIO + 1) + x)] = string[i];
	}

	return x;
}

void WriteStringToBufferWithLimit(const char* string, char* targetBuffer, int x, int y, int limit) {
	for (int i = 0; i < limit; i++) {
		if (string[i] == '\0')
			break;
		targetBuffer[i + (y * (SCREEN_WIDTH / FONT_RATIO + 1) + x)] = string[i];
	}
}

void WriteStringToBuffer(const char* string, char* targetBuffer, int x, int y) {
	for (int i = 0; i < strlen(string); i++) {
		if (string[i] == '\0')
			break;
		targetBuffer[i + (y * (SCREEN_WIDTH / FONT_RATIO + 1) + x)] = string[i];
	}
}

void SearchFiles(const char* folderPath, char targetBuffer[][50], int fileLeadLimit) {
	WIN32_FIND_DATAA findData; 
	HANDLE hFind;

	char searchPath[MAX_PATH];
	sprintf_s(searchPath, sizeof(searchPath), "%s\\*", folderPath);

	hFind = FindFirstFileA(searchPath, &findData);
	if (hFind == INVALID_HANDLE_VALUE) {
		printf("폴더를 찾을 수 없습니다: %s\n", folderPath);
		return;
	}

	int index = 0;
	do {
		if (strcmp(findData.cFileName, ".") != 0 && strcmp(findData.cFileName, "..") != 0) {
			for (int i = 0; i < strlen(findData.cFileName); i++)
			{
				targetBuffer[index][i] = findData.cFileName[i];
			}
			targetBuffer[index][strlen(findData.cFileName)] = '\0';
			index++;
			if (index > 10)
				break;
		}
	} while (FindNextFileA(hFind, &findData));

	for (int i = index; i < fileLeadLimit; i++)
	{
		targetBuffer[i][0] = '\0';
	}

	FindClose(hFind);
}