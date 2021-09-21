#include "joystick.h"


#include "adc.h"
#include <stdio.h>

#ifndef INCLUDES_H
#include "includes.h"
#endif




 enum directions direction(signed int x_val, signed int y_val) {
	signed int deadzone = 30;
	signed int zero = 0;
	
	if (abs(x_val) > abs(y_val)) { //x>y -> left/right
		if (x_val > (zero + deadzone)) {
			return RIGHT;
		}
		else if (x_val < (zero - deadzone)) {
			return LEFT;
		}
	}
	else {						//x<y -> up/down
		if (y_val > (zero + deadzone)) {
			return UP;
		}
		else if (y_val < (zero - deadzone)) {
			return DOWN;
		}
	}
	return NEUTRAL;
}

volatile struct joysticks fetchJoystick(void) {
	uint8_t *p;
	volatile struct joysticks joystick;
	
	p = readADC();
	
	uint8_t x_val = *p;
	uint8_t y_val = *(p+1);
	
	
	
	joystick.x_val = joystickPercent(x_val);
	joystick.y_val = joystickPercent(y_val);
	
	joystick.dir = direction(joystick.x_val, joystick.y_val);
	
	return joystick;
}

volatile struct controllers fetchController(void) {
	uint8_t *p;
	volatile struct controllers controller;
	
	p = readADC();
	
	controller.slider1_val = *(p+2);
	controller.slider2_val = *(p+3);
	
	return controller;
}

signed int joystickPercent(uint8_t val) {
	
	signed int per_val = (signed int)val * 200.0 / 255.0 - 100.0;
	return per_val;
}
