
#ifndef F_CPU // In case F_CPU not defined
#define F_CPU 4915200 // 4.9152 MHz
#endif

#include <avr/interrupt.h>
#include <avr/io.h>
#include <stdio.h>
#include <util/delay.h>

enum directions {
	UP, DOWN, LEFT, RIGHT, NEUTRAL
}; 