#include<stdio.h>
#include<Windows.h>
#include<stdbool.h>
#include<opencv/cv.h>
#include<opencv/highgui.h>

void PlayASCIIVideo(const char*);
void GenerateASCIIbyImage(char*, IplImage*, int, int);
void GotoXY(int, int); 
void SetCursorHide(void);
void SetConsoleFontSize(int);

enum WindowState {
	EXIT = 0,
	EXITMENU,
	MAINMENU,
	VIDEO,
	IMG,
	HELPMENU
};

#define DEFAULT_WIDTH 100
#define DEFAULT_HEIGHT 40

int main() {
	system("title Console Animation");
	SetCursorHide();

	PlayASCIIVideo("OshinoKo-Idol");
	
	system("cls");
	Sleep(1000);
	return 0;
}

void PlayASCIIVideo(const char* videoName) {
	char videoAdress[100] = "";
	sprintf_s(videoAdress, sizeof(videoAdress), "Resources\\Videos\\%s.mp4", videoName);

	CvCapture* capture = cvCreateFileCapture(videoAdress);

	if (!capture)
	{
		printf("Could not open video file\n");
		printf("%s", videoAdress);
		return -1;
	}

	IplImage* frame;
	IplImage* grayFrame;
	IplImage* outputImage;

	float frameWidth = cvGetCaptureProperty(capture, CV_CAP_PROP_FRAME_WIDTH);
	float frameHeight = cvGetCaptureProperty(capture, CV_CAP_PROP_FRAME_HEIGHT);
	float frameRate = cvGetCaptureProperty(capture, CV_CAP_PROP_FPS);
	float frameCount = cvGetCaptureProperty(capture, CV_CAP_PROP_FRAME_COUNT);
	
	float imageRatio = 0.0f;

	int DestWidth = 0;
	int DestHeight = 140;

	if (frameWidth > frameHeight)
		imageRatio = frameWidth / frameHeight;
	else
		imageRatio = frameHeight / frameWidth;

	DestWidth = imageRatio * DestHeight;
	DestWidth = DestWidth * 2;

	// bufferSize 크기의 screenBuffer 메모리에 동적할당
	int bufferSize = DestWidth * DestHeight + DestHeight;
	char* screenBuffer = malloc(bufferSize * (sizeof(char)));


	char commandMeg[90] = "";
	SetConsoleFontSize(4);
	sprintf_s(commandMeg, sizeof(commandMeg), "mode con cols=%d lines=%d | title Playing %s", DestWidth, DestHeight, videoName);
	system(commandMeg);
	SetCursorHide();


	DWORD lastTick = 0;
	while (true)
	{
		GotoXY(0, 0);

		DWORD currentTick = GetTickCount();
		if (currentTick - lastTick < 31.5f)
			continue;
		lastTick = currentTick;

		frame = cvQueryFrame(capture); // 프레임을 가져옴

		if (!frame)
			break;

		grayFrame = cvCreateImage(cvGetSize(frame), IPL_DEPTH_8U, CV_LOAD_IMAGE_GRAYSCALE);
		outputImage = cvCreateImage(cvSize(DestWidth, DestHeight), IPL_DEPTH_8U, CV_LOAD_IMAGE_GRAYSCALE);


		cvCvtColor(frame, grayFrame, CV_BGR2GRAY); // 흑백으로 변환
		cvResize(grayFrame, outputImage, CV_INTER_LINEAR);
		GenerateASCIIbyImage(screenBuffer, outputImage, DestWidth, DestHeight);
		printf("%s", screenBuffer);
	}

	// 메모리 할당 해제
	free(screenBuffer);
	cvReleaseCapture(&capture);
	cvReleaseImage(&grayFrame);
	system("cls");
	SetConsoleFontSize(12);
	sprintf_s(commandMeg, sizeof(commandMeg), "mode con cols=%d lines=%d", DEFAULT_WIDTH, DEFAULT_HEIGHT);
	system(commandMeg);
}

void GenerateASCIIbyImage(char* targetBuffer, IplImage* outputImage, int width, int height) {
	// 명도를 표현할 character 배열
	char asciiChars[14] = " .,-~:;=!*#$@\0";

	if (!outputImage)
		printf("fetal error!");


	for (int i = 0; i < (height * width + height); i++)
	{
		int y = i / (width + 1);
		int x = i % (width + 1);
		if (x == width)
			targetBuffer[i] = '\n';
		else {
			unsigned char pixel = cvGet2D(outputImage, y, x).val[0];
			int iPixel = pixel;
			float fIndex = pixel / 256.0f * strlen(asciiChars);
			int index = fIndex;
			targetBuffer[i] = asciiChars[index];
		}
	}
	targetBuffer[(height * width + height) - 1] = '\0';

	// 메모리 할당 해제
	cvReleaseImage(&outputImage);
}

void GotoXY(int x, int y) {
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