
#include <avr/interrupt.h>
#include <avr/io.h>
#include <stdio.h>
#include <util/delay.h>

enum directions {
	UP, DOWN, LEFT, RIGHT, NEUTRAL
}; 