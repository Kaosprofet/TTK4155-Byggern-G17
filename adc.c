#include "adc.h"
#include "functions.h"
#include "sram.h"
#include "uart.h"
#include <avr/io.h>
#include <avr/interrupt.h>

#ifndef adcAddress
#define adcAddress 0x1400
#endif

 // Starts by pointing to the first ADC channel
volatile char *adcChannel = (char *)adcAddress;
volatile char adcData;                  // Storage for read ADC values

void adcTest(void) {
	enableEMI();
	char data1 = adcChannel[0];
	printf("Data1 = %02d\n", data1);
	char data2 = adcChannel[1];
	printf("Data2 = %02d\n", data2);
	char data3 = adcChannel[2];
	printf("Data3 = %02d\n", data3);
	char data4 = adcChannel[3];
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

void selectADCChannel(uint8_t channel) {
	// Vi kan kanskje gj�re dette p� en bedre m�te, men har ikke helt skj�nt hvordan vi setter setter adressene skikkelig
    char channelAdress = 0x00;

    switch (channel) {
    case 1:
        channelAdress = 0x00;
        break;
    case 2:
        channelAdress = 0x01;
        break;
    case 3:
        channelAdress = 0x02;
        break;
    case 4:
        channelAdress = 0x03;
        break;
    }

    adcChannel[0x00] = channelAdress;
}

//ISR(INT1_vect) {
//    //adcData = adcChannel[0x00];
//}

