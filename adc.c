#include "adc.h"
#include "functions.h"
#include "sram.h"
#include "uart.h"
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

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

void readADC() {
	// Trigger read
	*adcVal = 0;
	// wait til converted
	_delay_us(70);
	// Store all data from ADC
	for (uint8_t i = 0; i < adc_num_channels; i++) {
		adcData[i] = *adcVal;
	}
}	

void adcTest(void) {
	enableEMI();
	
	volatile char data1;
	volatile char data2;
	volatile char data3;
	volatile char data4;

		*adcVal = 0;
		_delay_ms(70);
		data1 = *adcVal;
		printf("Data1 = %02d\n", data1);
		data2 = *adcVal;
		printf("Data2 = %02d\n", data2);
		data3 = *adcVal;
		printf("Data3 = %02d\n", data3);
		data4 = *adcVal;
		printf("Data4 = %02d\n", data4);
	
}


