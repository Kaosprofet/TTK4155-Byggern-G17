#ifndef BAUD // In case BAUD not defined
#define BAUD 9600 
#endif

#ifndef F_CPU // In case F_CPU not defined
#define F_CPU 4915200 // 4.9152 MHz
#endif

#include <avr/io.h>
#include "functions.h"

void initUart();

void transmitByte(uint8_t byte);
uint8_t receiveByte();
