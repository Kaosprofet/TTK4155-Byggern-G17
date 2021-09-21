#include "game.h"
#include <avr/io.h>
#include "joystick.h"
#include "functions.h"


void initButton(void) {
	clearBit(DDRD,PD3);
	clearBit(DDRD,PD4);
}