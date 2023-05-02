#include <stdio.h>

void test(){
	printf("test\n");
}

enum GameState{
	PLAY = 0,
	PAUSE
};

int main(int argc, char** argv) {
	int num = 0;
	int sub_num = 0;
	int result = 0;
	enum GameState game_state;

	game_state = PLAY;
	scanf("%d", &num);
	printf("나의 연애 횟수는: %d회\n", game_state);

	num = 0;
	printf("Hello, World!\n");
	test();
	do{
		printf("\n");
		printf("입력:");
		scanf("%d", &num);
		printf("두번째 숫자 입력:");
		scanf("%d", &sub_num);

		if (num == 0){
			printf("0\n");
		}
		else if (num == 1){
			printf("1\n");
		}
		else {
			printf("other number\n");
		}

		switch((int)num)
		{
		case 0:
			printf("case 0\n");
			break;
		case 1:
			printf("case 1\n");
			break;
		default:
			printf("예외\n");
			break;
		}

		result = num + sub_num;
		printf("더한 결과: %d\n", result);
		result = num - sub_num;
		printf("뺀 결과: %d\n", result);
		result = num / sub_num;
		printf("나눈 결과: %d\n", result);
		result = num * sub_num;
		printf("곱한 결과: %d\n", result);
	} while (1);

	return 0;
}

