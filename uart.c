#ifndef INCLUDES_H
#include "includes.h"
#endif

#define BAUD_PRESCALE ((F_CPU / (BAUD * 16UL)) - 1)

void initUart() {
	// Casting 
	UBRR0H = (unsigned char)(BAUD_PRESCALE >> 8);
	UBRR0L = (unsigned char)BAUD_PRESCALE;

	UCSR0B |= (1 << TXEN0) | (1 << RXEN0);   // Recieve and transmit enabled

	UCSR0C = (1<<URSEL0)|(1<<USBS0)|(3<<UCSZ00);
	//UCSR0C |= (1 << USBS0) | (1 << URSEL0);  // 2 stop bits
	//UCSR0C |= (3 << UCSZ00);                 // 8 bit data recieve (UCSZ10)
	
	fdevopen(transmitByte,receiveByte); //enables printf function over uart
}

void transmitByte(unsigned char byte) {
    waitUntilBitIsSet(UCSR0A, UDRE0);
    UDR0 = byte;
}

uint8_t receiveByte() {
    waitUntilBitIsSet(UCSR0A, RXC0);
    return UDR0;
}