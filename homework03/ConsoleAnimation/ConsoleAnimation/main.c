#include<stdio.h>
#include<Windows.h>
#include<stdbool.h>
#include<opencv/cv.h>
#include<opencv/highgui.h>

void GenerateImageToASCII(const char*, int);
void GotoXYZero();

int main() {
	//IplImage* img = cvCreateImage(cvSize(600, 600), IPL_DEPTH_8U, 1);
	//for (int y = 0; y < img->height; y++) {
	//	for (int x = 0; x < img->width; x++) {
	//		int idx = y + x * img->widthStep;
	//		img->imageData[idx] = 100;
	//	}
	//}
	//cvNamedWindow("img", CV_WINDOW_AUTOSIZE);
	//cvShowImage("img", img);
	//cvSaveImage("img.png", img, NULL);
	//cvWaitKey(0);
	//cvDestroyAllWindows();
	//return 0;

	// 비디오 프레임마다 이미지로 추출 (jpg)

	CvCapture* capture = cvCreateFileCapture("Resources/Videos/Idol.mp4");

	if (!capture)
	{
		printf("Could not open video file\n");
		return -1;
	}

	IplImage* frame;
	IplImage* grayFrame;
	int frameCount = 0;  // 추출한 프레임 수를 세는 변수
	bool ConvertingFlag = true;
	bool isSelectMaxFrame = false;

	const char bar = '-';
	const char blank = ' ';
	const int LEN = 20;
	int maxFrameCount = 1000;

	if (isSelectMaxFrame)
		maxFrameCount = (int)cvGetCaptureProperty(capture, CV_CAP_PROP_FRAME_COUNT);

	int bar_count = 0;
	float percent = 0.0f;
	if (ConvertingFlag) {
		for (int i = 0; i < maxFrameCount; i++)
		{
			// 프레임을 가져옴
			frame = cvQueryFrame(capture);
			if (!frame)
				break;

			grayFrame = cvCreateImage(cvGetSize(frame), IPL_DEPTH_8U, CV_LOAD_IMAGE_GRAYSCALE);

			// 흑백으로 변환
			cvCvtColor(frame, grayFrame, CV_BGR2GRAY);

			char filename[50];
			sprintf_s(filename, sizeof(filename), "Generated/Idol/frame_%d.jpg", frameCount);  // 이미지 파일 이름을 생성
			cvSaveImage(filename, grayFrame, 0);  // 현재 프레임을 이미지로 저장

			printf("\r Converting video to image... %d/%d [", frameCount, (maxFrameCount - 1));

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

	int i = 0;
	while(i < frameCount) {
		int currentTick = GetTickCount();
		if (currentTick - lastTick < 1000/30)
			continue;
		else {
			lastTick = currentTick;
			char Adress[50];
			sprintf_s(Adress, sizeof(Adress), "Generated/Idol/frame_%d.jpg", i);  // 이미지 파일 이름을 생성합니다.
			GotoXYZero();
			GenerateImageToASCII(Adress, 100);
			sprintf_s(Adress, sizeof(Adress), "Current frame: %d", i);  // 이미지 파일 이름을 생성합니다.
			printf("%s", Adress);
			GotoXYZero();
			i++;
		}
	}

	return 0;
}

void GenerateImageToASCII(const char* ImageAdress, int width) {
	// 명도를 표현할 character 배열
	char asciiChars[14] = " .,-~:;=!*#$@\0";

	// 이미지 선언
	IplImage* sourceImage = 0;
	IplImage* resizedImage = 0;
	// 이미지 흑백으로 로드 (iscolor > 0 이면 컬러, = 0 이면 흑백으로 변환, < 0 이면 원본 그대로)
	sourceImage = cvLoadImage(ImageAdress, CV_LOAD_IMAGE_GRAYSCALE);

	//이미지 크기 구하기
	float imageWidth = sourceImage->width;
	float imageHeight = sourceImage->height;

	// 목표 이미지 값
	int height = 0;

	// 이미지 비율
	float imageRatio = imageHeight / imageWidth;
	// 이미지 비율에 목표 가로크기를 곱해서 목표 세로크기를 구함
	height = imageRatio * width;
	width = width * 2;
	// 목표 이미지 크기로 이미지를 생성
	resizedImage = cvCreateImage(cvSize(width, height), IPL_DEPTH_8U, CV_LOAD_IMAGE_GRAYSCALE);

	// 원본 이미지를 resizedImage로 선형으로 크기를 변환하여 넣음
	cvResize(sourceImage, resizedImage, CV_INTER_LINEAR);

	 int bufferSize = width * height + height;
	 // bufferSize 만큼 메모리에 동적할당
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


	// 메모리 할당 해제
	free(screenBuffer);
	cvReleaseImage(&sourceImage);
	cvReleaseImage(&resizedImage);
}

void GotoXYZero() {
	COORD pos = { 0, 0 };
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
}