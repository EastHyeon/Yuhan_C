#include<stdio.h>
#include<opencv/cv.h>
#include<opencv/highgui.h>
int main() {
	IplImage* img = cvCreateImage(cvSize(600, 600), IPL_DEPTH_8U, 1);
	for (int y = 0; y < img->height; y++) {
		for (int x = 0; x < img->width; x++) {
			int idx = y + x * img->widthStep;
			img->imageData[idx] = 100;
		}
	}
	cvNamedWindow("img", CV_WINDOW_AUTOSIZE);
	cvShowImage("img", img);
	cvSaveImage("img.png", img, NULL);
	cvWaitKey(0);
	cvDestroyAllWindows();
	return 0;
}