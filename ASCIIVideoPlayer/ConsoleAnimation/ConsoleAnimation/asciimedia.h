#pragma once

#include <fmod.h>
#include <opencv/cv.h>
#include <opencv/highgui.h>

#include <stdio.h>
#include <stdbool.h>
#include <Windows.h>

#include "input.h"
#include "utilites.h"

#define BASE_VIDEO_ADRESS "Resources\\Videos\\%s"
#define BASE_AUDIO_ADRESS "Resources\\Audios\\%s"

#define MAX_COLOR_RANGE 14

int PlayASCIIVideo(const char*, int, int, int*);
void PlaySimpleASCIIVideo(const char*, int, int, int*);

void ConvertASCIIbyImage(char*, int, IplImage*, int, int, int);
int WriteImageByWidth(const char*, char*, int, int);
int WriteImageByHeight(const char*, char*, int, int);