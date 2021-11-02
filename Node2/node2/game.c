#ifndef INCLUDES_H
#include "includes.h"
#endif

void start_game(void) {
	uint32_t score = 0;
	
	game.game_status = 1;
		
	can_decode_message();
	
	switch(game.game_status) {
		case(0):
		break;
		case(1):
		run_game();
		break;
		
	}
	game.score = calculate_score(score);
	//can_encode_message(30);
	printf("8bit score: %d\n\r", game.score);
}

uint32_t run_game(void) {
	uint32_t score = 0;
		
	while(1) {
		//can_decode_message();
		//position_servo(controller.x);
		score = score+1;
		if (IR_blocked()) {break;}
		printf("Score: %d\n\r", score);
	return score;
	}
}

uint8_t calculate_score(uint32_t raw_score) {
	// Calibrate better!
	raw_score = raw_score/20000000; 
	return (uint8_t)raw_score;
}
