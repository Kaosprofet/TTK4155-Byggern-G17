#ifndef JOYSTICK_H_
#define JOYSTICK_H_

#include <avr/io.h>

enum directions {
	UP, DOWN, LEFT, RIGHT, NEUTRAL
};

struct controllers {											// Full struct for controller
	signed int x_zero;
	signed int y_zero;
	volatile signed int x_val;
	volatile signed int y_val;
	volatile enum directions dir;
	volatile uint8_t slider1_val;
	volatile uint8_t slider2_val;
};

void initButton(void);											// Initializes booth buttons
void updateController(struct controllers *controller);			// Updates all values for controller
void printController(struct controllers *controller);			// Prints all values for controller
signed int joystickPercent(uint8_t val, struct controllers *controller);						// Converts joystick ADC values to percent
enum directions direction(signed int x_val, signed int y_val);	// Calculates joystick direction
void calibrateJoystick(struct controllers *controller);

#endif