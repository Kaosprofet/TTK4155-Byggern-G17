#include "adc.h"
#include "functions.h"

#ifndef adcAddress
#define adcAddress = 0x1400
#endif

volatile char* adcChannel = adcAddress; // Starts by pointing to the first ADC channel
volatile char adcData;                  // Storage for read ADC values

void initADC(void) {
    cli(); // Temporary dissables global interupts

    clearBit(DDRD, PD3); // Sets PD3 as input
    setBit(PORTD, PD3);  // Sets pullup resistor on PD3

    setBit(GICR, INT1); // Sets interupt on PD3
    setBit(MCUCR, ISC11);
    clearBit(MCUCR, ISC10);
    

    sei() // Re-enables global interupts
}

ISR(INT1_vect) {
    adcData = adcChannel[0x00];
}