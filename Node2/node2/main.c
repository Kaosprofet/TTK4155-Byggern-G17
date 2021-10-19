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
	
	//led_test();
	
	can_test();
	
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
	
	
	while(1) {

	}	
}
void can_test(void) {
	can_init_def_tx_rx_mb(CAN0->CAN_BR);
}