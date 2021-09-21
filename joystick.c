#include "joystick.h"
#include <avr/io.h>

#include "adc.h"

volatile struct joystics {
	volatile uint8_t x_val;
	volatile uint8_t y_val;
};

uint8_t fetchJoystick(char xy) {
	return 0;
}

uint8_t fetchSlider(uint8_t num) {
	return 0;
}