#ifndef JOYSTICK_H_
#define JOYSTICK_H_

#include <avr/io.h>

enum directions {
	UP, DOWN, LEFT, RIGHT, NEUTRAL
};

volatile struct joysticks {
	uint8_t offset;
	volatile signed int x_val;
	volatile signed int y_val;
	volatile enum directions dir;
};

volatile struct controllers {
	volatile uint8_t slider1_val;
	volatile uint8_t slider2_val;
};

volatile struct joysticks fetchJoystick(void);
volatile struct controllers fetchController(void);

signed int joystickPercent(uint8_t val);

enum directions direction(signed int x_val, signed int y_val);

void joystickTest(void);



#endif