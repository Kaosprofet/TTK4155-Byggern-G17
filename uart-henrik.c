#ifndef BAUD // In case BAUD not defined
#define BAUD 9600 
#endif

#ifndef F_CPU // In case F_CPU not defined
#define F_CPU 4915200 // 4.9152 MHz
#endif

#define BAUD_PRESCALE (((F_CPU / (BAUD * 16UL))) - 1)	//Sjekk om 16 eller 8 er rett

#include <avr/io.h>
#include "functions.h"
#include "uart-henrik.h"


void initUart() {

    UBRR0H = (BAUD_PRESCALE >> 8);
    UBRR0L = BAUD_PRESCALE;

    UCSR0B = (1 << TXEN0) | (1 << RXEN0)  // Recieve and transmit enabled
    UCSR0C = (1 << UCSZ1) | (1 << UCSZ0)  // 8bit data recieve

}

void transmitByte(uint_8 byte) {
    whaitBitIsSet(UCSR0A, UDRE0)
    UDR0 = byte;
}

uint8_t recieveByte() {
    whaitBitIsSet(UCSR0A, RXC0)
    return UDR0;
}