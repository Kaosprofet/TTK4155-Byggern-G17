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
		score = run_game();
		break;
		
	}
	game.score = calculate_score(score);
	//can_encode_message(30);
	printf("8bit score: %d\n\r", game.score);
}

uint32_t run_game(void) {
	uint32_t score = 0;
		
	while(1) {
		can_decode_message();
		position_servo(controller.x);
		solenoidControll();
		DAC_set_output();
		//printf("x: %d, y: %d, button: %d, s1: %d, s2: %d\n\r", controller.x, controller.y, controller.button_state, controller.slider_1_val, controller.slider_2_val);
		score = score+1;
		printf("pinstate: %d\n\r", (PIOC->PIO_SODR & PIO_PC13));
		//IR_print();
		if (IR_blocked()) {
			//printf("break");
			//break;
		}
		
	}
	//printf("Score: %d\n\r", score);
	return score;
}

uint8_t calculate_score(uint32_t raw_score) {
	// Calibrate better!
	raw_score = raw_score/20000000; 
	return (uint8_t)raw_score;
}
