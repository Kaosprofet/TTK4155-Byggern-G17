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
void can_test(void);
void inits(void);


int main(void) {
	inits();
	
	//led_test();
	//can_test();
	
	start_game();
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
void can_test(void) {
	can_encode_message(controller_id);
	while (1) {
		can_decode_message();

	}
}