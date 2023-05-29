#pragma once

enum ConsoleColor {
	Black = 0,
	DarkBlue = 1,
	DarkGreen = 2,
	DarkSkyBlue = 3,
	DarkRed = 4,
	DarkPurple = 5,
	DarkYellow = 6,
	Gray = 7,
	DarkGray = 8,
	Blue = 9,
	Green = 10,
	SkyBlue = 11,
	Red = 12,
	Purple = 13,
	Yellow = 14,
	White = 15
};

enum CurrentInput {
	KEY_Wait,
	KEY_W,
	KEY_A,
	KEY_S,
	KEY_D,
	KEY_UpArrow,
	KEY_LeftArrow,
	KEY_DownArrow,
	KEY_RightArrow,
	KEY_Spacebar,
	KEY_Enter
} currentInput;

#define VK_W 0x57
#define VK_A 0x41
#define VK_S 0x53
#define VK_D 0x44