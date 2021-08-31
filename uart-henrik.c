#ifndef BAUD // In case BAUD not defined
#define BAUD 9600 
#endif

#ifndef F_CPU // In case F_CPU not defined
#define F_CPU 4915200 // 4.9152 MHz
#endif

#include <avr/io.h>
#include "functions.h"
#include "uart-henrik.h"


void initUart() {

    UBRR0H = UBRRH_VALUE;
    UBRR0L = UBRRL_VALUE;

    UCSR0A |= (1 << U2X0);
}

void transmitByte(uint_8 byte) {
    whaitBitIsSet(UCSR0A, UDRE0)
    UDR0 = byte;
}

uint8_t recieveByte() {
    whaitBitIsSet(UCSR0A, RXC0)
    return UDR0;
}