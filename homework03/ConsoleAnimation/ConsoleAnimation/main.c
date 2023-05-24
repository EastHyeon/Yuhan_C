#include<stdio.h>
#include<Windows.h>
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

	// ���� �����Ӹ��� �̹����� ���� (jpg)

	CvCapture* capture = cvCreateFileCapture("Resources/Videos/BadApple.mp4");

	if (!capture)
	{
		printf("Could not open video file\n");
		return -1;
	}

	IplImage* frame;
	int frameCount = 0;  // ������ ������ ���� ���� ����

	for (int i = 0; i < 100; i++)
	{
		frame = cvQueryFrame(capture);
		char filename[50];
		sprintf_s(filename, sizeof(filename), "Generated/BadApple/frame_%d.jpg", frameCount);  // �̹��� ���� �̸��� �����մϴ�
		cvSaveImage(filename, frame, 0);  // ���� �������� �̹����� �����մϴ�.
		printf("frame_%d saved\n", frameCount);

		frameCount++;
	}
	
	system("cls");
	cvReleaseCapture(&capture);

	int lastTick = 0;

	int i = 0;
	while(i < frameCount - 1) {
		int currentTick = GetTickCount();
		if (currentTick - lastTick < 1000/30)
			continue;
		else {
			lastTick = currentTick;
			char Adress[50];
			sprintf_s(Adress, sizeof(Adress), "Generated/BadApple/frame_%d.jpg", i);  // �̹��� ���� �̸��� �����մϴ�.
			GotoXYZero();
			GenerateImageToASCII(Adress, 50);
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