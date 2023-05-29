#pragma once

#include <fmod.h>
#include <opencv/cv.h>
#include <opencv/highgui.h>

#include <stdio.h>
#include <stdbool.h>
#include <Windows.h>

#define BASE_VIDEO_ADRESS "Resources\\Videos\\%s.mp4"
#define BASE_AUDIO_ADRESS "Resources\\Audios\\%s.mp3"

#define MAX_COLOR_RANGE 14

/// <summary>
/// ��Ʈ ũ�⸦ 4�� �����ϰ�
/// ������ �ƽ�Ű��Ʈ�� ��ȯ�Ͽ� ����մϴ�.
/// </summary>
/// <param name=""> ������ ���ϸ� (Ȯ���ڸ� ����, mp4 Ȯ���ڸ� �޽��ϴ�.)</param>
/// <param name=""> ������ ���α��� </param>
/// <param name=""> ������ ���α��� ���� </param>
void PlayASCIIVideo(const char*, FMOD_SYSTEM*, int, int, int*);
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