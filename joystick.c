#include "joystick.h"


#include "adc.h"

#ifndef INCLUDES_H
#include "includes.h"
#endif



volatile struct controllers {
	volatile uint8_t slider1_val;
	volatile uint8_t slider2_val;
	
};

enum directions direction(uint8_t x_val, uint8_t y_val) {
	uint8_t deadzone = 10;
	uint8_t zero = 0;
	
	if (abs(x_val) > abs(y_val)) { //x>y -> left/right
		if (x_val > (zero + deadzone)) {
			return RIGHT;
		}
		else if (x_val < (zero-deadzone)) {
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

float joystickPercent(uint8_t val) {
	
	float per_val = val * (float)(100 + 100) / 255 - 100;
	return per_val;
}
