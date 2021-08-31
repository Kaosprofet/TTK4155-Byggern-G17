#ifndef BAUD // In case BAUD not defined
#define BAUD 9600 
#endif

#ifndef F_CPU // In case F_CPU not defined
#define F_CPU 4915200 // 4.9152 MHz
#endif

#include <avr/io.h>

void initUart();

void transmitByte(uint_8 byte);
uint_8 recieveByte();
