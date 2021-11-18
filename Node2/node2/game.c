#ifndef INCLUDES_H
#include "includes.h"
#endif

void start_game(void) {
	uint32_t score = 0;
		 
	//can_encode_message(status_id);
	//delay_ms(20);

	score = run_game();

	game.score = calculate_score(score);
	can_encode_message(status_id);
	delay_ms(10);
	printf("8bit score: %d\n\r", game.score);
}

uint32_t run_game(void) {
	uint32_t time_start = RTT->RTT_VR;
	uint32_t time_elapsed;
	uint32_t score = 0;
	motor_controll_init();
	printf("Running game \n\r");
	while(1) {
		CAN0_Handler(); //Listening for CAN messages. 
		delay_ms(10);
		servo_slider_controll();
		solenoidControll();
		motor_controll(HARD);                           
		
		if(RTT->RTT_VR<time_start){		// If RTT has overflowed recalculate time elapsed
			time_elapsed = 4294967296-time_start+RTT->RTT_VR;
		}
		else {
			time_elapsed = RTT->RTT_VR-time_start;
		}
		if (time_elapsed>60000){
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
	return time_elapsed;
}

uint8_t calculate_score(uint32_t raw_score) {
	// Calibrate better!
	raw_score = map(raw_score, 0, 60000, 0, 255); 
	return (uint8_t)raw_score;
}
