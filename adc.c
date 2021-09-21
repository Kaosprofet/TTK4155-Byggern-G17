#ifndef INCLUDES_H
#include "includes.h"
#endif

#define adc_address 0x1400
#define adc_num_channels 4
//#define adc_config 0b10010000 // Brukes om vi vil hente single verdier

// Starts by pointing to the first ADC channel
volatile uint8_t *adcVal = (uint8_t *)adc_address;
// Storage for read ADC values
volatile uint8_t adcData[adc_num_channels];





void initADC(void) {
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

uint8_t * readADC(){
	
	// Trigger read
	*adcVal = 0;
	
	// wait til converted
	_delay_us(70);
	// Store all data from ADC
	for (uint8_t i = 0; i < adc_num_channels; i++) {
		adcData[i] = *adcVal;
	}
	return adcData;
}	

void adcTest(void) {
	uint8_t *p;
	
	*adcVal = 0;
	
	p = readADC();
	_delay_us(70);
	
	uint8_t x_val = *p;
	uint8_t y_val = *(p+1);
	
	signed int x_per_val = joystickPercent(x_val);
	signed int y_per_val = joystickPercent(y_val);
	signed int test2 = -100;
	
	enum directions dir = direction(x_val, y_val);
	
	printf("X = %d ", x_val);
	printf("Y = %d ", y_val);
	printf("X percent = %d ", x_per_val);
	printf("Y percent = %d ", y_per_val);
	printf("Dir = %d ", dir);
	printf("Test = %d\n\r ", test2);
	
}


