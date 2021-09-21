#ifndef JOYSTICK_H_
#define JOYSTICK_H_

#include <avr/io.h>

enum directions {
	UP, DOWN, LEFT, RIGHT, NEUTRAL
};

// Full struct for spill
volatile struct controllers {
	signed int offset;
	volatile signed int x_val;
	volatile signed int y_val;
	volatile enum directions dir;
	volatile uint8_t slider1_val;
	volatile uint8_t slider2_val;
};

// Slettes når controller med pointer er oppe
volatile struct joysticks {
	uint8_t offset;
	volatile signed int x_val;
	volatile signed int y_val;
	volatile enum directions dir;
};
// Slettes når controller med pointer er oppe
volatile struct slide_controllers {
	volatile uint8_t slider1_val;
	volatile uint8_t slider2_val;
};

volatile struct joysticks fetchJoystick(void);
volatile struct slide_controllers fetchController(void);

void updateController(volatile struct controllers *controller);
void printController(volatile struct controllers *controller);


signed int joystickPercent(uint8_t val);

enum directions direction(signed int x_val, signed int y_val);

void joystickTest(void);



#endif