#include "screen.h"

void clearBuffer(char* targetBuffer) {
	for (int i = 0; i < SCREEN_SIZE; i++)
	{
		int currentX = i % (SCREEN_MAX_X + 1);
		if (currentX == SCREEN_MAX_X)
			targetBuffer[i] = '\n';
		else
			targetBuffer[i] = ' ';
	}
	targetBuffer[SCREEN_SIZE - 1] = '\0';
}

void writeStringToBuffer(const char* message, char* targetBuffer, int x, int y)
{

}

void writeStringCenterToBuffer(const char* message, char* targetBuffer, int y)
{
	for (int i = 0; i < strlen(message); i++) {
		targetBuffer[i + (y * (SCREEN_MAX_X + 1) + ((SCREEN_MAX_X / 2) - (strlen(message) / 2)))] = message[i];
	}
}
