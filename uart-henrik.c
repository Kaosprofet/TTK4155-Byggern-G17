#ifndef BAUD // In case BAUD not defined
#define BAUD 9600 
#endif

#ifndef F_CPU // In case F_CPU not defined
#define F_CPU 4915200 // 4.9152 MHz
#endif

#include <avr/io.h>
#include <util/setbaud.h>
#include "uart-henrik.h"


void transmitByte(uint_8 byte) {
    loop_until_bit_is_set(UCSR0A, UDRE0)
        UDR0 = byte;
}

uint8_t recieveByte() {

}