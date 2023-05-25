#include<stdio.h>
#include<Windows.h>
#include<stdbool.h>
#include<opencv/cv.h>
#include<opencv/highgui.h>

void GenerateImageToASCII(const char*, int);
void GotoXYZero();

int main() {

	// ���� �����Ӹ��� �̹����� ���� (jpg)

	const char* videoName = "BadApple";

	char videoAdress[100] = "";
	sprintf_s(videoAdress, sizeof(videoAdress), "Resources/Videos/%s.mp4", videoName);
	char frameAdress[100] = "";
	sprintf_s(frameAdress, sizeof(frameAdress), "Generated/%s/", videoName);

	CvCapture* capture = cvCreateFileCapture(videoAdress);

	if (!capture)
	{
		printf("Could not open video file\n");
		printf("%s", videoAdress);
		return -1;
	}

	IplImage* frame;
	IplImage* grayFrame;
	int frameCount = 0;  // ������ ������ ���� ���� ����
	bool ConvertingFlag = true;
	bool isSelectMaxFrame = false;

	const char bar = '-';
	const char blank = ' ';
	const int LEN = 20;
	int maxFrameCount = 500;

	if (isSelectMaxFrame)
		maxFrameCount = (int)cvGetCaptureProperty(capture, CV_CAP_PROP_FRAME_COUNT);

	int bar_count = 0;
	float percent = 0.0f;
	if (ConvertingFlag) {
		for (int i = 0; i < maxFrameCount; i++)
		{
			// �������� ������
			frame = cvQueryFrame(capture);
			if (!frame)
				break;

			grayFrame = cvCreateImage(cvGetSize(frame), IPL_DEPTH_8U, CV_LOAD_IMAGE_GRAYSCALE);

			// ������� ��ȯ
			cvCvtColor(frame, grayFrame, CV_BGR2GRAY);

			char filename[50];
			sprintf_s(filename, sizeof(filename), "%sframe_%d.jpg", frameAdress, frameCount);  // �̹��� ���� �̸��� ����
			cvSaveImage(filename, grayFrame, 0);  // ���� �������� �̹����� ����

			printf("\r Converting %s video to image... %d/%d [", videoName, frameCount, (maxFrameCount - 1));

			percent = (float)frameCount / (maxFrameCount - 1) * 100;
			bar_count = percent / (100 / LEN);
			for (int i = 0; i < LEN; i++)
			{
				if (bar_count > i) {
					printf("%c", bar);
				}
				else {
					printf("%c", blank);
				}
			}
			printf("] %0.2f%%", percent);
			frameCount++;
		}
		cvReleaseCapture(&capture);
		cvReleaseImage(&grayFrame);
	}
	
	system("cls");
	printf("\rConverting Complete");
	Sleep(1000);

	int lastTick = 0;

	if (!ConvertingFlag) {
		frameCount = maxFrameCount;
	}

	int i = 0;
	while(i < frameCount) {
		int currentTick = GetTickCount();
		if (currentTick - lastTick < 1000/30)
			continue;
		else {
			lastTick = currentTick;
			char Adress[50];
			sprintf_s(Adress, sizeof(Adress), "%s/frame_%d.jpg", frameAdress, i);
			GotoXYZero();
			GenerateImageToASCII(Adress, 80);
			sprintf_s(Adress, sizeof(Adress), "Current frame: %d", i);
			printf("%s", Adress);
			GotoXYZero();
			i++;
		}
	}

	return 0;
}

void GenerateImageToASCII(const char* ImageAdress, int width) {
	// ���� ǥ���� character �迭
	char asciiChars[14] = " .,-~:;=!*#$@\0";


	// �̹��� ����
	IplImage* sourceImage = 0;
	IplImage* resizedImage = 0;
	// �̹��� ������� �ε� (iscolor > 0 �̸� �÷�, = 0 �̸� ������� ��ȯ, < 0 �̸� ���� �״��)
	sourceImage = cvLoadImage(ImageAdress, CV_LOAD_IMAGE_GRAYSCALE);

	if (!sourceImage)
		printf("fetal error!");

	//�̹��� ũ�� ���ϱ�
	float imageWidth = sourceImage->width;
	float imageHeight = sourceImage->height;

	// ��ǥ �̹��� ��
	int height = 0;

	// �̹��� ����
	float imageRatio = imageHeight / imageWidth;
	// �̹��� ������ ��ǥ ����ũ�⸦ ���ؼ� ��ǥ ����ũ�⸦ ����
	height = imageRatio * width;
	width = width * 2;
	// ��ǥ �̹��� ũ��� �̹����� ����
	resizedImage = cvCreateImage(cvSize(width, height), IPL_DEPTH_8U, CV_LOAD_IMAGE_GRAYSCALE);

	// ���� �̹����� resizedImage�� �������� ũ�⸦ ��ȯ�Ͽ� ����
	cvResize(sourceImage, resizedImage, CV_INTER_LINEAR);

	 int bufferSize = width * height + height;
	 // bufferSize ��ŭ �޸𸮿� �����Ҵ�
	char* screenBuffer = malloc(bufferSize*(sizeof(char)));

	int bufferIndex = 0;
	for (int y = 0; y < height; y++)
	{
		for (int x = 0; x < width; x++)
		{
			unsigned char pixel = cvGet2D(resizedImage, y, x).val[0];
			int iPixel = pixel;
			float fIndex = pixel / 256.0f * strlen(asciiChars);
			int index = fIndex;
			screenBuffer[bufferIndex] = asciiChars[index];
			bufferIndex++;
		}
		screenBuffer[bufferIndex] = '\n';
		bufferIndex++;
	}
	screenBuffer[bufferIndex - 1] = '\0';
	printf("%s\n", screenBuffer);
	cvSaveImage("Generated/TestImage.jpg", resizedImage, NULL);


	// �޸� �Ҵ� ����
	free(screenBuffer);
	cvReleaseImage(&sourceImage);
	cvReleaseImage(&resizedImage);
}

void GotoXYZero() {
	COORD pos = { 0, 0 };
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
}