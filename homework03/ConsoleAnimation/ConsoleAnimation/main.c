#include<stdio.h>
#include<opencv/cv.h>
#include<opencv/highgui.h>

void GenerateImageToASCII(const char*, int);

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

	GenerateImageToASCII("Resources/TestImage.jpg", 50);



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

	char buffer[60];
	// �� ���� �̰� string.h �� ���� �̰� printf���� ���� ��ɸ� �����°���
	sprintf_s(buffer, sizeof(buffer), "mode con cols=%d lines=%d | title Generated", width, height);
	system(buffer);

	// ��ȭ �� �Ҹ� ������ and �� ���� ä������ ����

	// ���� �̹����� resizedImage�� �������� ũ�⸦ ��ȯ�Ͽ� ����
	cvResize(sourceImage, resizedImage, CV_INTER_LINEAR);

	for (int y = 0; y < height; y++)
	{
		for (int x = 0; x < width; x++)
		{
			unsigned char pixel = cvGet2D(resizedImage, y, x).val[0];
			int iPixel = pixel;
			float fIndex = pixel / 256.0f * strlen(asciiChars);
			int index = fIndex;
			printf("%c", asciiChars[index]);
		}
		printf("\n");
	}

	cvSaveImage("Generated/TestImage.jpg", resizedImage, NULL);

	// �̹��� �Ҵ� ����
	cvReleaseImage(&sourceImage);
	cvReleaseImage(&resizedImage);
}