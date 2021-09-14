#include "adc.h"
#include "functions.h"
#include "sram.h"
#include "uart.h"
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

#ifndef adcAddress
#define adcAddress 0x1400
#endif

// Starts by pointing to the first ADC channel
volatile char *adcChannel = (char *)adcAddress;
//volatile char adcData;                  // Storage for read ADC values

volatile struct adcData {
	volatile char channel1;
	volatile char channel2; 
	volatile char channel3;
	volatile char channel4;  
};

void adcTest(void) {
	enableEMI();
	
	volatile char data1;
	volatile char data2;
	volatile char data3;
	volatile char data4;

		adcChannel[0] = 0x00;
		_delay_ms(10);
		data1 = adcChannel[0];
		printf("Data1 = %02d\n", data1);
		data2 = adcChannel[0];
		printf("Data2 = %02d\n", data2);
		data3 = adcChannel[0];
		printf("Data3 = %02d\n", data3);
		data4 = adcChannel[0];
		printf("Data4 = %02d\n", data4);
	
}

void initADC(void) {
    //cli(); // Temporary dissables global interupts

    //clearBit(DDRD, PD3); // Sets PD3 as input
    //setBit(PORTD, PD3);  // Sets pullup resistor on PD3

    //setBit(GICR, INT1); // Sets interupt on PD3

    //setBit(MCUCR, ISC11); // Trigger on fallin edge
    //clearBit(MCUCR, ISC10);
    
    //sei(); // Re-enables global interupts

}

void readADC() {
	// Trigger read
	// wait til converted
	
	
	// Read data from adc
	adcData.channel1 = adcChannel[0];
	adcData.channel2 = adcChannel[0];
	adcData.channel3 = adcChannel[0];
	adcData.channel4 = adcChannel[0];
}	



