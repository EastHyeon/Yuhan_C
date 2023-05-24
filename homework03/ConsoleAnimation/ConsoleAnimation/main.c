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

	// 비디오 프레임마다 이미지로 추출 (jpg)

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

	char buffer[60];
	// 말 못함 이거 string.h 에 있음 이건 printf에서 포멧 기능만 가져온거임
	sprintf_s(buffer, sizeof(buffer), "mode con cols=%d lines=%d | title Generated", width, height);
	system(buffer);

	// 전화 중 소리 못들음 and 말 못함 채팅으로 ㄱㄱ

	// 원본 이미지를 resizedImage로 선형으로 크기를 변환하여 넣음
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

	// 이미지 할당 해제
	cvReleaseImage(&sourceImage);
	cvReleaseImage(&resizedImage);
}