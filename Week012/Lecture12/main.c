#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include "screen.h"
#include "utilites.h"

int main() {
	char screenBuffer[SCREEN_SIZE];

	int gameState = 1;
	int inputValue = 0;

	while (gameState) {
		setCursorPositionZero();
		clearBuffer(screenBuffer);

		if (gameState == 1) {
			writeStringCenterToBuffer("Game Menu", screenBuffer, 1);
			writeStringCenterToBuffer("1. Start", screenBuffer, 3);
			writeStringCenterToBuffer("2. Help", screenBuffer, 4);
			writeStringCenterToBuffer("3. Exit", screenBuffer, 5);
		}
		else if (gameState == 2) {
			writeStringCenterToBuffer("Help", screenBuffer, 1);
			writeStringCenterToBuffer("blahblahblahblahblahblahblahblahblahblah", screenBuffer, 3);
			writeStringCenterToBuffer("blahblahblahblahblahblahblahblah", screenBuffer, 4);
			writeStringCenterToBuffer("Input Any key to return to the Game Menu.", screenBuffer, 6);
		}
		else if (gameState == 3) {
			writeStringCenterToBuffer("Are you Sure Exit?", screenBuffer, 1);
			writeStringCenterToBuffer("1. Yes", screenBuffer, 3);
			writeStringCenterToBuffer("2. No", screenBuffer, 4);
		}

		printf("%s\n", screenBuffer);
		scanf_s("%d", &inputValue);
		system("cls");

		if (gameState == 1) {
			if (inputValue == 2)
				gameState = 2;
			else if (inputValue == 3)
				gameState = 3;
			else
				gameState = 1;
		}
		else if (gameState == 2) {
			gameState = 1;
		}
		else if (gameState == 3) {
			if (inputValue == 1)
				gameState = 0;
			else
				gameState = 1;
		}
	}

	return 0;
}