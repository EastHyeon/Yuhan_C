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

/// <summary>
/// 폰트 크기를 4로 설정하고
/// 영상을 아스키아트로 변환하여 출력합니다.
/// </summary>
/// <param name="videoName"> 영상의 파일명 (확장자명 제외, mp4 확장자만 받습니다.)</param>
/// <param name=""> Fmod System 구조체 </param>
/// <param name=""> 영상의 세로길이 </param>
/// <param name=""> 영상의 가로길이 제한 </param>
/// <param name=""> 폰트 사이즈 </param>
int PlayASCIIVideo(const char*, int, int, int*);
void PlaySimpleASCIIVideo(const char*, int, int, int*);
/// <summary>
/// IplImage를 읽어서 문자열로 변환해 줍니다. 성능상 문제로 이미지 크기를 직접 받습니다.
/// </summary>
/// <param name=""> 변환된 문자열을 받을 변수 </param>
/// <param name=""> 문자열의 크기 </param>
/// <param name=""> 변환 대상이 될 이미지 객체 </param>
/// <param name=""> 이미지의 가로 크기 </param>
/// <param name=""> 이미지의 세로 크기</param>
void ConvertASCIIbyImage(char*, int, IplImage*, int, int, int);
int WriteImageByWidth(const char*, char*, int, int);
int WriteImageByHeight(const char*, char*, int, int);