#ifndef INCLUDES_H
#include "includes.h"
#endif

#define servo_max 150
#define servo_min -150
#define servo_offset 0

void init_servo(void) {
	
	// Disable interrupts
	PIOC->PIO_IDR = PIO_PC19B_PWMH5;
	
	//Enable Clock for PWM in PMC
	PMC->PMC_PCR = PMC_PCR_EN | (0 << PMC_PCR_DIV_Pos) | PMC_PCR_CMD | (ID_PWM << PMC_PCR_PID_Pos);
	PMC->PMC_PCER1 |= 1 << (ID_PWM - 32);
}

void position_servo(uint8_t position) {
	position = position + servo_offset;
	
	if (position < servo_min) {
		position = servo_min;
	}
	else if (position > servo_max) {
		position = servo_max;
	}
	
}