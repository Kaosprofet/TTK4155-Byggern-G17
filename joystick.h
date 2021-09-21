#ifndef JOYSTICK_H_
#define JOYSTICK_H_

#include <avr/io.h>

enum directions {
	UP, DOWN, LEFT, RIGHT, NEUTRAL
};

volatile struct joysticks {
	volatile float x_val;
	volatile float y_val;
	volatile enum directions dir;
};

volatile struct joysticks fetchJoystick(void);
volatile struct controllers fetchController(void);

float joystickPercent(uint8_t val);
enum directions direction(uint8_t x_val, uint8_t y_val);

void joystickTest(void);

#endif