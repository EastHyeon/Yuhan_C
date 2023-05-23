#include <stdio.h>
#include <Windows.h>
#include <time.h>

int main() {
	clock_t deltaTimeStart = time(NULL);

	for (int i = 0; i < 400; i++)
	{
		system("cls");
		for (int j = 0; j < 300; j++)
		{
			printf("###################################################\n###################################################\n###################################################\n###################################################\n###################################################\n###################################################\n");
		}
	}

	clock_t deltaTimeEnd = time(NULL);
	printf("\n걸린 시간 : %d", deltaTimeEnd - deltaTimeStart);
}