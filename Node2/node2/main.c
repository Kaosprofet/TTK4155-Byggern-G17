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


int main(void) {
    /* Initialize the SAM system */
    SystemInit();
	WDT->WDT_MR = WDT_MR_WDDIS;	// Watchdog disabled
	configure_uart();
	init_can();
	
	//led_test();
	
	//can_test();

	init_servo();
	
    while (1) {
		
    }
}

void led_test(void) {
	PIOA->PIO_PER |= LED_1;
	PIOA->PIO_OER |= LED_1;
	PIOA->PIO_PUDR |= LED_1;
	
	PIOA->PIO_PER |= LED_2;
	PIOA->PIO_OER |= LED_2;
	PIOA->PIO_PUDR |= LED_2;
	
	setBit(PIOA, LED_1);

}
void can_test(void) {
	CAN_MESSAGE message;
	can_encode_message(controller_id);
	while (1) {
		can_receive(&message,0);
		can_decode_message(&message);
		
	}
}