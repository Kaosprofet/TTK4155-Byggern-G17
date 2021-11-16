/*
 * node2.c
 *
 * Created: 12/10/2021 10:28:43
 * Author : henri
 */ 

#ifndef INCLUDES_H
#include "includes.h"
#endif

#define LED_1 PIO_PA19
#define LED_2 PIO_PA20

void led_test(void);
void IR_test(void);
void can_test(void);
void inits(void);


int main(void) {
	inits();
	
	//led_test();
	//IR_test();
	//can_test();
	//game.game_status=0;
	//while(!game.game_status){
	//	can_decode_message();
	//	delay_ms(10);
	//} //Wait to start game
	//start_game();
	can_com_status_test();
}

void inits(void) {
	/* Initialize the SAM system */
	SystemInit();
	WDT->WDT_MR = WDT_MR_WDDIS;	// Watchdog disabled
	configure_uart();
	init_can();
	init_game_board();
	DAC_init();
	controller =  (Controller){0, 0, 0, 0, 0};
	game.game_status = 0;
}

void led_test(void) {
	PIOA->PIO_PER |= LED_1;
	PIOA->PIO_OER |= LED_1;
	PIOA->PIO_PUDR |= LED_1;
	
	PIOA->PIO_PER |= LED_2;
	PIOA->PIO_OER |= LED_2;
	PIOA->PIO_PUDR |= LED_2;
	
	setBit(PIOA, LED_1);
	clearBit(PIOA,LED_2);
	while(RTT->RTT_VR<RTT->RTT_VR+100){}
	setBit(PIOA, LED_2);
	clearBit(PIOA,LED_1);
	while(RTT->RTT_VR<RTT->RTT_VR+100){}
}

void IR_test(void) {
	while(1){
		IR_print();
	}
}

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
	
	//venter på ok
	uint8_t cond = 1;
	while(cond){
		rx1 = CAN0_Handler();
		printf("ID: %d, DATA: %d\n\r", rx1.id, rx1.data);
		if(rx1.id == 1 && rx1.data[0] == 69){
			cond =0;
		}
	}
	printf("Got ok \n\r");
	
}