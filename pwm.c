#include "adc.h"
#include "functions.h"

#ifndef F_CPU // In case F_CPU not defined
#define F_CPU 4915200 // 4.9152 MHz
#endif

#define PWM_PRESCALE F_CPU


void initPWM(void) {
	// PWM output pin
	setBit(DDRD, PD5);
	
	// Setts output pin to toggle
	clearBit(TCCR1A, COM1A1);
	setBit(TCCR1A, COM1A0);

	// CTC PWM enabled
	//setBit(TCCR0, WGM01);
	//clearBit(TCCR0, WGM00);
	clearBit(TCCR1B, WGM13);
	setBit(TCCR1B, WGM12);
	clearBit(TCCR1A, WGM11);
	clearBit(TCCR1A, WGM10);
	
	// Top value set to 0 => triggers comparator each cycle
	OCR1AH = 0x00;
	OCR1AL = 0x00;
	
	// Set prescaling to clk/1
	clearBit(TCCR1B, CS12);
	clearBit(TCCR1B, CS11);
	setBit(TCCR1B, CS10);
}