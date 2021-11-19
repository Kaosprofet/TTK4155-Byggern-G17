/*
 * node2.c
 *
 * Created: 12/10/2021 10:28:43
 * Author : henri
 */ 

#ifndef INCLUDES_H
#include "includes.h"
#endif

void IR_test(void);
void can_test(void);
void inits(void);

int main(void) {
	inits();
	while(1) {
		while(!game.game_status){
			CAN0_Handler(); //Checks for can messages
			song_select(); //Checks if entered high score menu to play
		} //Wait to start game
		printf("Starting game\n\r");
		start_game();
	}
}

void inits(void) {
	/* Initialize the SAM system */
	SystemInit();
	WDT->WDT_MR = WDT_MR_WDDIS;	// Watchdog disabled
	configure_uart();
	init_can();
	init_game_board();
	DAC_init();
	music_pwm_init();
	controller =  (Controller){0, 0, 0, 0, 0};
	game.game_status = 0;
}

//------------------------------------------- Tests -----------------------------------------------

// Prints output from IR diode
void IR_test(void) {
	while(1){
		IR_print();
	}
}

// Tests CAN between node 1 and 2
void can_test(void) {
	can_encode_message(controller_id);
	while (1) {
		can_decode_message();

	}
}

void can_test_send(void){
	CAN_MESSAGE message;
	message.id = 20;
	message.data_length = 1;
	message.data[0] = 5;
	can_send(&message,0);
}

void can_com_status_test(void){
	CAN_MESSAGE m1;
	m1.id = 1;
	m1.data_length = 1;
	m1.data[0] = 1;
	CAN_MESSAGE m2;
	m2.id = 2;
	m2.data_length = 1;
	m2.data[0] = 2;
	CAN_MESSAGE m3;
	m2.id = 3;
	m2.data_length = 1;
	m2.data[0] = 3;
	
	can_send(&m1,0);
	printf("Sent first message \n\r ");
	CAN_MESSAGE rx1;
	
	//venter p� ok
	uint8_t cond = 1;
	while(cond){
		CAN0_Handler();
		//printf("ID: %d, DATA: %d\n\r", rx1.id, rx1.data);
		if(rx_message.id == 1 && rx_message.data[0]==69){
			cond =0;
		}
	}
	printf("Got ok \n\r");
	
}