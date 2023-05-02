#include "DRS.h"

int main()
{
	int game_state = 1;
	
	/* 게임 루프
	 * while문이 계속 반복해서 입력을 받는다.
	 * 입력받은 값을 game_state에 저장한다.
	 */
	while(game_state)
	{
		print_title_screen();
		printf("input>");
		scanf("%d", &game_state);
		// 디렉토리 생성
		system("mkdir donghyeon_babo");
	}


	return 0;
}