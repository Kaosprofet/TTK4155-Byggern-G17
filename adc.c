#include "adc.h"
#include "functions.h"
#include <avr/io.h>
#include <avr/interrupt.h>

#ifndef adcAddress
#define adcAddress 0x1400
#endif

volatile char* adcChannel = adcAddress; // Starts by pointing to the first ADC channel
volatile char adcData;                  // Storage for read ADC values


void initADC(void) {
    cli(); // Temporary dissables global interupts

    clearBit(DDRD, PD3); // Sets PD3 as input
    setBit(PORTD, PD3);  // Sets pullup resistor on PD3

    setBit(GICR, INT1); // Sets interupt on PD3

    setBit(MCUCR, ISC11); // Trigger on fallin edge
    clearBit(MCUCR, ISC10);
    
    sei(); // Re-enables global interupts
}

void selectADCChannel(uint8_t channel) {
	// Vi kan kanskje gj�re dette p� en bedre m�te, men har ikke helt skj�nt hvordan vi setter setter adressene skikkelig
    char channelAdress = 0x00;

    switch (channel) {
    case 1:
        channelAdress = 0x04;
        break;
    case 2:
        channelAdress = 0x05;
        break;
    case 3:
        channelAdress = 0x06;
        break;
    case 4:
        channelAdress = 0x07;
        break;
    default:
        return 0;//EXIT_FAILURE;
    }

    adcChannel[0x00] = channelAdress;
}

ISR(INT1_vect) {
    adcData = adcChannel[0x00];
}

