#include <stdio.h>

int main (){
	int enemy_hp = 30;
	int player_atk = 0;
	
	printf("주인공의 공격력을 입력하십시오. : ");
	scanf("%d", &player_atk);

	printf("주인공의 공격력은 %d 입니다.\n", player_atk);

	enemy_hp = enemy_hp - player_atk;
	printf("주인공이 적을 공격하여 %d의 데미지를 입혔습니다.\n", player_atk);
	if(enemy_hp < 0){
		printf("\n");
		printf("적의 잔여 HP:0\n");
		printf("\n");
	}
	else{
		printf("\n");
		printf("적의 잔여 HP:%d\n", enemy_hp);
		printf("\n");
	}

	if(enemy_hp <= 0){
		printf("몬스터를 잡았습니다!\n");
		printf("HAPPY END");
	}
	else{
		printf("적이 주인공을 공격하여 주인공이 뒤졌습니다.\n");
		printf("GAME OVER");
	}

	return 0;
}
