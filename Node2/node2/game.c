#ifndef INCLUDES_H
#include "includes.h"
#endif

void start_game(void) {
	uint32_t score = 0;
	
	score = run_game();

	game.score = calculate_score(score);
	can_encode_message(status_id);
	delay_ms(20);
}

uint32_t run_game(void) {
	uint32_t time_start = RTT->RTT_VR;
	uint32_t time_elapsed;
	uint32_t score = 0;
	motor_controll_init();
	while(1) {
		CAN0_Handler(); //Listening for CAN messages. 
		delay_ms(10);
		servo_slider_controll();
		solenoidControll();
		motor_controll();                           
		
		if(RTT->RTT_VR<time_start){	// If RTT has overflowed recalculate time elapsed
			time_elapsed = 4294967296-time_start+RTT->RTT_VR;
		}
		else {
			time_elapsed = RTT->RTT_VR-time_start;
		}
		if (time_elapsed>60000){ // Checks if time limit reached (1 min)
			printf("Time limit reached \n\r");
			game.game_status = 0;
			break;
		}
		
		if (IR_blocked()) { 
			printf("Ball was dropped! \n\r");
			game.game_status = 0;
			break;
		}
	}
	game_over_music(); // Plays losing music
	return time_elapsed;
}

// Calculates score based on time elapsed and returns a value between 0 and 255
uint8_t calculate_score(uint32_t raw_score) {
	raw_score = map(raw_score, 0, 60000, 0, 255); 
	return (uint8_t)raw_score;
}