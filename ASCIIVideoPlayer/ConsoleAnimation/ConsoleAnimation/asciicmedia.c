#include "asciimedia.h"

int PlayASCIIVideo(const char* videoName, int width, int heightLimit, int* fontSize) {
	FMOD_SYSTEM* FMODSystem;

	FMOD_System_Create(&FMODSystem);
	FMOD_System_Init(FMODSystem, 32, FMOD_INIT_NORMAL, NULL);

	char videoAdress[150] = "";
	char audioAdress[150] = "";
	sprintf_s(videoAdress, sizeof(videoAdress), BASE_VIDEO_ADRESS, videoName);

	int repeatFlag = 1;

	int Key;
	FMOD_CHANNEL* Channel = 0;
	float Volume = 0.1f;
	FMOD_SOUND* Sound = 0;
	bool IsFmodPlaying = false;

	FMOD_System_Create(&FMODSystem);
	FMOD_System_Init(FMODSystem, 32, FMOD_INIT_NORMAL, NULL);

	if (Sound != 0)
		FMOD_Sound_Release(Sound);

	sprintf_s(audioAdress, sizeof(audioAdress), BASE_AUDIO_ADRESS, videoName);
	audioAdress[strlen(audioAdress) - 1] = '3';
	FMOD_System_CreateSound(FMODSystem, audioAdress, FMOD_DEFAULT, 0, &Sound);

	if (Channel != 0)
		FMOD_Channel_IsPlaying(Channel, &IsFmodPlaying);
	if (IsFmodPlaying != 0)
		FMOD_Channel_Stop(Channel);

	CvCapture* capture = cvCreateFileCapture(videoAdress);

	if (!capture)
	{
		printf("Could not open video file\n");
		printf("%s", videoAdress);
		return -1;
	}

	IplImage* frame = 0;
	IplImage* grayFrame;
	IplImage* outputImage;

	float frameWidth = cvGetCaptureProperty(capture, CV_CAP_PROP_FRAME_WIDTH);
	float frameHeight = cvGetCaptureProperty(capture, CV_CAP_PROP_FRAME_HEIGHT);
	float frameRate = cvGetCaptureProperty(capture, CV_CAP_PROP_FPS);
	int frameCount = cvGetCaptureProperty(capture, CV_CAP_PROP_FRAME_COUNT);

	float imageRatio = 0.0f;


	if (frameWidth > frameHeight)
		imageRatio = frameWidth / frameHeight;
	else
		imageRatio = frameHeight / frameWidth;

	int DestWidth = width;
	int DestHeight = (width / 2) / imageRatio;

	int videoHeightLimit = heightLimit - 4;

	if (DestHeight > videoHeightLimit - 4 && videoHeightLimit != 0) {
		DestHeight = videoHeightLimit;
	}

	int iconWidth = DestWidth / 20;
	int iconHeight = heightLimit * 1.5 / 20;

	int extraSpace = (width / 2) - (DestWidth / 2);
	// 한 프레임을 저장할 버퍼의 크기
	int bufferSize = (DestWidth + extraSpace) * DestHeight + DestHeight;
	int iconBufferSize = iconWidth * iconHeight + iconHeight;

	char* screenBuffer = malloc(bufferSize * (sizeof(char)));

	char commandMessage[150] = "";
	SetConsoleFontSize(*fontSize);
	sprintf_s(commandMessage, sizeof(commandMessage), "mode con cols=%d lines=%d | title Playing: %s / FontSize: %d", width, heightLimit, videoName, *fontSize);
	system(commandMessage);
	SetCursorHide();

	IplImage* splayImage;
	IplImage* spauseImage;
	IplImage* playImage = cvCreateImage(cvSize(iconWidth, iconHeight), IPL_DEPTH_8U, 0);
	IplImage* pauseImage = cvCreateImage(cvSize(iconWidth, iconHeight), IPL_DEPTH_8U, 0);
	char* iconBuffer = malloc(iconBufferSize * (sizeof(char)));
	splayImage = cvLoadImage("Resources\\Icons\\Play.png", 0);
	spauseImage = cvLoadImage("Resources\\Icons\\Pause.png", 0);
	cvResize(splayImage, playImage, CV_INTER_LINEAR);
	cvResize(spauseImage, pauseImage, CV_INTER_LINEAR);

	const int LEN = width - (width * 20) / 100;
	const int VolumeLEN = width * 20 / 100;
	char* playBarBuffer = malloc((LEN + 1) * (sizeof(char)));
	char* volumeBarBuffer = malloc(VolumeLEN + 1 * (sizeof(char)));
	const char bar = '@';
	const char blank = ':';
	const int MAX = frameCount;
	float tick = (float)100 / LEN;
	float volumeTick = (float)100 / VolumeLEN;
	int bar_count;
	float percent;
	int currentFrame = 0;

	unsigned int maxTime = 0;

	FMOD_Sound_GetLength(Sound, &maxTime, FMOD_TIMEUNIT_MS);
	maxTime = maxTime / 2;
	unsigned int currentTime = 0;

	bool isPause = false;

	int startXPosition = (DestWidth / 2 + extraSpace) - iconWidth / 2;
	int startYPosition = heightLimit / 2 - iconHeight / 2;

	FMOD_System_PlaySound(FMODSystem, Sound, 0, 0, &Channel);
	FMOD_Channel_SetVolume(Channel, Volume);

	DWORD lastTick = 0;
	DWORD lastInputTick = 0;
	DWORD lastPauseTick = 0;
	while (true)
	{
		SetCursorPosition(0, 0);
		SetConsoleTextColor(White);
		DWORD currentTick = GetTickCount();

		if (currentTick - lastTick < 32)
			continue;
		lastTick = currentTick;

		if (currentTick - lastInputTick > 1000 / 7) {
			if (GetAsyncKeyState(VK_LEFT) & 0x8001) {
				if (!isPause) {
					isPause = true;
					FMOD_Channel_SetPaused(Channel, isPause);
					FMOD_Channel_GetPosition(Channel, &currentTime, FMOD_TIMEUNIT_MS);
					currentFrame = currentFrame - (24 * 5);
					currentTime = currentTime - 5000;
					if (currentFrame <= 0) {
						currentFrame = 0;
						currentTime = 0;
					}
					FMOD_Channel_SetPosition(Channel, currentTime, FMOD_TIMEUNIT_MS);
					cvSetCaptureProperty(capture, CV_CAP_PROP_POS_FRAMES, currentFrame);
					frame = cvQueryFrame(capture);
					isPause = false;
					FMOD_Channel_SetPaused(Channel, isPause);
				}
				else {
					FMOD_Channel_SetPaused(Channel, isPause);
					FMOD_Channel_GetPosition(Channel, &currentTime, FMOD_TIMEUNIT_MS);
					currentFrame = currentFrame - (24 * 5);
					currentTime = currentTime - 5000;
					if (currentFrame <= 0) {
						currentFrame = 0;
						currentTime = 0;
					}
					FMOD_Channel_SetPosition(Channel, currentTime, FMOD_TIMEUNIT_MS);
					cvSetCaptureProperty(capture, CV_CAP_PROP_POS_FRAMES, currentFrame);
					frame = cvQueryFrame(capture);
					FMOD_Channel_SetPaused(Channel, isPause);
				}
			}
			if (GetAsyncKeyState(VK_RIGHT) & 0x8001) {
				if (!isPause) {
					isPause = true;
					FMOD_Channel_SetPaused(Channel, isPause);
					FMOD_Channel_GetPosition(Channel, &currentTime, FMOD_TIMEUNIT_MS);
					currentFrame = currentFrame + (24 * 5);
					currentTime = currentTime + 5000;
					if (currentFrame >= frameCount) {
						currentFrame = frameCount - 1;
						currentTime = maxTime - 1;
					}
					FMOD_Channel_SetPosition(Channel, currentTime, FMOD_TIMEUNIT_MS);
					cvSetCaptureProperty(capture, CV_CAP_PROP_POS_FRAMES, currentFrame);
					frame = cvQueryFrame(capture);
					isPause = false;
					FMOD_Channel_SetPaused(Channel, isPause);
				}
				else {
					FMOD_Channel_SetPaused(Channel, isPause);
					FMOD_Channel_GetPosition(Channel, &currentTime, FMOD_TIMEUNIT_MS);
					currentFrame = currentFrame + (24 * 5);
					currentTime = currentTime + 5000;
					if (currentFrame >= frameCount) {
						currentFrame = frameCount - 1;
						currentTime = maxTime - 1;
					}
					FMOD_Channel_SetPosition(Channel, currentTime, FMOD_TIMEUNIT_MS);
					cvSetCaptureProperty(capture, CV_CAP_PROP_POS_FRAMES, currentFrame);
					frame = cvQueryFrame(capture);
					FMOD_Channel_SetPaused(Channel, isPause);
				}
			}
			if (GetAsyncKeyState(VK_UP) & 0x8001) {
				Volume += 0.1f;
				if (Volume > 1.0f)
					Volume = 1.0f;
				FMOD_Channel_SetVolume(Channel, Volume);
			}
			if (GetAsyncKeyState(VK_DOWN) & 0x8001) {
				Volume -= 0.1f;
				if (Volume < 0.0f)
					Volume = 0.0f;
				FMOD_Channel_SetVolume(Channel, Volume);
			}
			if (GetAsyncKeyState(VK_OEM_4) & 0x8001) {
				isPause = true;
				FMOD_Channel_SetPaused(Channel, isPause);
				*fontSize = *fontSize - 1;
				if (*fontSize <= 1) {
					*fontSize = 2;
				}
				SetConsoleFontSize(*fontSize);
				sprintf_s(commandMessage, sizeof(commandMessage), "mode con cols=%d lines=%d | title Playing: %s / FontSize: %d", width, heightLimit, videoName, *fontSize);
				system(commandMessage);
				isPause = false;
				FMOD_Channel_SetPaused(Channel, isPause);
			}
			if (GetAsyncKeyState(VK_OEM_6) & 0x8001) {
				isPause = true;
				FMOD_Channel_SetPaused(Channel, isPause);
				*fontSize = *fontSize + 1;
				if (*fontSize > 6) {
					*fontSize = 6;
				}
				SetConsoleFontSize(*fontSize);
				sprintf_s(commandMessage, sizeof(commandMessage), "mode con cols=%d lines=%d | title Playing: %s / FontSize: %d", width, heightLimit, videoName, *fontSize);
				system(commandMessage);
				isPause = false;
				FMOD_Channel_SetPaused(Channel, isPause);
				FMOD_Channel_SetPosition(Channel, currentFrame * 1000 / 24, FMOD_TIMEUNIT_MS);
			}
			if (GetAsyncKeyState(VK_ESCAPE) & 0x8001) {
				repeatFlag = 0;
				break;
			}
			lastInputTick = currentTick;
		}

		percent = (float)currentFrame / MAX * 100;
		bar_count = percent / tick;
		int barIndex = 0;

		for (int i = 0; i < LEN; i++)
		{
			if (bar_count > barIndex)
				playBarBuffer[i] = bar;
			else
				playBarBuffer[i] = blank;
			barIndex++;
		}
		playBarBuffer[LEN - 1] = '\0';
		SetConsoleTextColor(Green);
		SetCursorPosition(0, heightLimit - 3);
		puts(playBarBuffer);
		puts(playBarBuffer);

		SetConsoleTextColor(White);
		percent = (float)Volume / 1.0f * 100;
		bar_count = percent / volumeTick;
		barIndex = 0;
		for (int i = 0; i < VolumeLEN; i++)
		{
			if (bar_count > barIndex)
				volumeBarBuffer[i] = bar;
			else
				volumeBarBuffer[i] = blank;
			barIndex++;
		}
		volumeBarBuffer[VolumeLEN - 1] = '\0';
		SetCursorPosition(LEN + 1, heightLimit - 3);
		puts(volumeBarBuffer);
		SetCursorPosition(LEN + 1, heightLimit - 2);
		puts(volumeBarBuffer);

		if (!isPause)
			frame = cvQueryFrame(capture); // 프레임을 가져옴

		if (!frame)
			break;

		grayFrame = cvCreateImage(cvGetSize(frame), IPL_DEPTH_8U, CV_LOAD_IMAGE_GRAYSCALE);
		outputImage = cvCreateImage(cvSize(DestWidth, DestHeight), IPL_DEPTH_8U, CV_LOAD_IMAGE_GRAYSCALE);

		cvCvtColor(frame, grayFrame, CV_BGR2GRAY); // 흑백으로 변환
		cvResize(grayFrame, outputImage, CV_INTER_LINEAR);
		ConvertASCIIbyImage(screenBuffer, bufferSize, outputImage, DestWidth, DestHeight, extraSpace);
		SetCursorPosition(0, videoHeightLimit / 2 - DestHeight / 2);
		printf(screenBuffer);


		if (currentTick - lastPauseTick > 1000 / 7) {
			if (GetAsyncKeyState(VK_SPACE) & 0x8001) {
				isPause = !isPause;
				if (isPause) {
					FMOD_Channel_SetPaused(Channel, isPause);
					ConvertASCIIbyImage(iconBuffer, iconBufferSize, pauseImage, iconWidth, iconHeight, 0);
					for (int i = 0; i < iconBufferSize; i++)
					{
						int y = i / (iconWidth + 1);
						int x = i % (iconWidth + 1);
						if (iconBuffer[i] != ' ') {
							SetCursorPosition(x + startXPosition, y + startYPosition);
							printf("%c", iconBuffer[i]);
						}
					}
					Sleep(500);
				}
				if (!isPause) {
					ConvertASCIIbyImage(iconBuffer, iconBufferSize, playImage, iconWidth, iconHeight, 0);
					for (int i = 0; i < iconBufferSize; i++)
					{
						int y = i / (iconWidth + 1);
						int x = i % (iconWidth + 1);
						if (iconBuffer[i] != ' ') {
							SetCursorPosition(x + startXPosition, y + startYPosition);
							printf("%c", iconBuffer[i]);
						}
					}
					Sleep(500);
					FMOD_Channel_SetPaused(Channel, isPause);
				}
				SetCursorPosition(0, 0);
			}
			lastPauseTick = currentTick;
		}


		if (!isPause) {
			currentFrame++;
		}
		FMOD_System_Update(FMODSystem);
	}

	// 메모리 할당 해제

	FMOD_Sound_Release(Sound);
	free(screenBuffer);
	free(playBarBuffer);
	free(iconBuffer);
	cvReleaseCapture(&capture);
	cvReleaseImage(&grayFrame);
	cvReleaseImage(&splayImage);
	cvReleaseImage(&spauseImage);
	cvReleaseImage(&playImage);
	cvReleaseImage(&pauseImage);
	FMOD_System_Close(FMODSystem);
	FMOD_System_Release(FMODSystem);
	system("cls");

	return repeatFlag;
}

void PlaySimpleASCIIVideo(const char* videoName, int width, int heightLimit, int* fontSize) {
	char videoAdress[150] = "";
	sprintf_s(videoAdress, sizeof(videoAdress), BASE_VIDEO_ADRESS, videoName);

	CvCapture* capture = cvCreateFileCapture(videoAdress);

	if (!capture)
	{
		printf("Could not open video file\n");
		printf("%s", videoAdress);
		return -1;
	}

	IplImage* frame = 0;
	IplImage* grayFrame;
	IplImage* outputImage;

	float frameWidth = cvGetCaptureProperty(capture, CV_CAP_PROP_FRAME_WIDTH);
	float frameHeight = cvGetCaptureProperty(capture, CV_CAP_PROP_FRAME_HEIGHT);
	float frameRate = cvGetCaptureProperty(capture, CV_CAP_PROP_FPS);
	int frameCount = cvGetCaptureProperty(capture, CV_CAP_PROP_FRAME_COUNT);

	float imageRatio = 0.0f;


	if (frameWidth > frameHeight)
		imageRatio = frameWidth / frameHeight;
	else
		imageRatio = frameHeight / frameWidth;

	int DestHeight = (width / 2) / imageRatio;

	if (DestHeight > heightLimit && heightLimit != 0) {
		DestHeight = heightLimit;
	}

	// 한 프레임을 저장할 버퍼의 크기
	int bufferSize = width * DestHeight + DestHeight;

	char* screenBuffer = malloc(bufferSize * (sizeof(char)));

	char commandMessage[150] = "";
	SetConsoleFontSize(*fontSize);
	sprintf_s(commandMessage, sizeof(commandMessage), "mode con cols=%d lines=%d", width, heightLimit);
	system(commandMessage);
	SetCursorHide();

	int currentFrame = 0;

	DWORD lastTick = 0;
	DWORD lastInputTick = 0;
	DWORD lastPauseTick = 0;
	while (currentFrame < frameCount)
	{
		SetCursorPosition(0, 0);
		SetConsoleTextColor(White);
		DWORD currentTick = GetTickCount();

		if (currentTick - lastTick < 32)
			continue;
		lastTick = currentTick;

		frame = cvQueryFrame(capture); // 프레임을 가져옴

		if (!frame)
			break;

		grayFrame = cvCreateImage(cvGetSize(frame), IPL_DEPTH_8U, CV_LOAD_IMAGE_GRAYSCALE);
		outputImage = cvCreateImage(cvSize(width, DestHeight), IPL_DEPTH_8U, CV_LOAD_IMAGE_GRAYSCALE);

		cvCvtColor(frame, grayFrame, CV_BGR2GRAY); // 흑백으로 변환
		cvResize(grayFrame, outputImage, CV_INTER_LINEAR);
		ConvertASCIIbyImage(screenBuffer, bufferSize, outputImage, width, DestHeight, 0);
		SetCursorPosition(0, heightLimit / 2 - DestHeight / 2);
		printf(screenBuffer);

		currentFrame++;
	}

	// 메모리 할당 해제

	free(screenBuffer);
	cvReleaseCapture(&capture);
	cvReleaseImage(&grayFrame);
	system("cls");
}


void ConvertASCIIbyImage(char* targetBuffer, int bufferSize, IplImage* outputImage, int width, int height, int extraSpaces) {
	// 명도를 표현할 character
	char asciiChars[14] = " .,-~:;=!*#$@";

	for (int i = 0; i < bufferSize; i++)
	{
		int y = i / (width + extraSpaces + 1);
		int x = i % (width + extraSpaces + 1);


		if (x < extraSpaces) {
			targetBuffer[i] = ' ';
		}
		else if (x == width + extraSpaces) {
			targetBuffer[i] = '\n';
		}
		else {
			unsigned char pixel = cvGet2D(outputImage, y, x - extraSpaces).val[0];
			int iPixel = pixel;
			float fIndex = pixel / 256.0f * strlen(asciiChars);
			int index = fIndex;
			targetBuffer[i] = asciiChars[index];
		}
	}
	targetBuffer[bufferSize - 1] = '\0';
}

int WriteImageByWidth(const char* fileName, char* targetBuffer, int DestWidth, int extraSpaces) {
	char asciiChars[14] = " .,-~:;=!*#$@";

	IplImage* stitleImage;
	stitleImage = cvLoadImage(fileName, 0);

	if (!stitleImage) {
		printf("Could not open video file\n");
		printf("%s", fileName);
	}

	int swidth = stitleImage->width;
	int sheight = stitleImage->height;

	float imageRatio = swidth / sheight;
	int DestHeight = (DestWidth / 2) / imageRatio;

	if (swidth < sheight) {
		imageRatio = sheight / swidth;
		DestHeight = (DestWidth / 2) * imageRatio;
	}

	int bufferSize = (DestWidth + extraSpaces) * DestHeight + DestHeight;

	IplImage* titleImage = cvCreateImage(cvSize(DestWidth, DestHeight), IPL_DEPTH_8U, 0);

	cvResize(stitleImage, titleImage, CV_INTER_LINEAR);

	for (int i = 0; i < bufferSize; i++)
	{
		int y = i / (DestWidth + extraSpaces + 1);
		int x = i % (DestWidth + extraSpaces + 1);

		if (x < extraSpaces) {
			targetBuffer[i] = ' ';
		}
		else if (x == DestWidth + extraSpaces) {
			targetBuffer[i] = '\n';
		}
		else {
			unsigned char pixel = cvGet2D(titleImage, y, x - extraSpaces).val[0];
			int iPixel = pixel;
			float fIndex = pixel / 256.0f * strlen(asciiChars);
			int index = fIndex;
			targetBuffer[i] = asciiChars[index];
		}
	}
	targetBuffer[bufferSize - 1] = '\0';

	return DestHeight;
}

int WriteImageByHeight(const char* fileName, char* targetBuffer, int DestHeight, int extraSpaces) {
	char asciiChars[14] = " .,-~:;=!*#$@";

	IplImage* stitleImage;
	stitleImage = cvLoadImage(fileName, 0);

	int swidth = stitleImage->width;
	int sheight = stitleImage->height;

	float imageRatio = swidth / sheight;
	int DestWidth = DestHeight * imageRatio;

	if (swidth < sheight) {
		imageRatio = sheight / swidth;
		DestWidth = DestHeight / imageRatio;
	}

	DestWidth *= 2;

	int bufferSize = (DestWidth + extraSpaces) * DestHeight + DestHeight;

	IplImage* titleImage = cvCreateImage(cvSize(DestWidth, DestHeight), IPL_DEPTH_8U, 0);

	cvResize(stitleImage, titleImage, CV_INTER_LINEAR);

	for (int i = 0; i < bufferSize; i++)
	{
		int y = i / (DestWidth + extraSpaces + 1);
		int x = i % (DestWidth + extraSpaces + 1);

		if (x < extraSpaces) {
			targetBuffer[i] = ' ';
		}
		else if (x == DestWidth + extraSpaces) {
			targetBuffer[i] = '\n';
		}
		else {
			unsigned char pixel = cvGet2D(titleImage, y, x - extraSpaces).val[0];
			int iPixel = pixel;
			float fIndex = pixel / 256.0f * strlen(asciiChars);
			int index = fIndex;
			targetBuffer[i] = asciiChars[index];
		}
	}
	targetBuffer[bufferSize - 1] = '\0';

	return DestWidth;
}