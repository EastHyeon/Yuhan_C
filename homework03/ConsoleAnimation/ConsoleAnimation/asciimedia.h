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
/// ��Ʈ ũ�⸦ 4�� �����ϰ�
/// ������ �ƽ�Ű��Ʈ�� ��ȯ�Ͽ� ����մϴ�.
/// </summary>
/// <param name="videoName"> ������ ���ϸ� (Ȯ���ڸ� ����, mp4 Ȯ���ڸ� �޽��ϴ�.)</param>
/// <param name=""> Fmod System ����ü </param>
/// <param name=""> ������ ���α��� </param>
/// <param name=""> ������ ���α��� ���� </param>
/// <param name=""> ��Ʈ ������ </param>
int PlayASCIIVideo(const char*, int, int, int*);
void PlaySimpleASCIIVideo(const char*, int, int, int*);
/// <summary>
/// IplImage�� �о ���ڿ��� ��ȯ�� �ݴϴ�. ���ɻ� ������ �̹��� ũ�⸦ ���� �޽��ϴ�.
/// </summary>
/// <param name=""> ��ȯ�� ���ڿ��� ���� ���� </param>
/// <param name=""> ���ڿ��� ũ�� </param>
/// <param name=""> ��ȯ ����� �� �̹��� ��ü </param>
/// <param name=""> �̹����� ���� ũ�� </param>
/// <param name=""> �̹����� ���� ũ��</param>
void ConvertASCIIbyImage(char*, int, IplImage*, int, int, int);
int WriteImageByWidth(const char*, char*, int, int);
int WriteImageByHeight(const char*, char*, int, int);