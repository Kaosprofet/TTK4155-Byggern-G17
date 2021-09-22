#ifndef INCLUDES_H
#include "includes.h"
#endif

void initButton(void) {
	clearBit(DDRD,PD3);
	clearBit(DDRD,PD4);
}

void updateController(struct controllers *controller) {
	uint8_t *p;
	
	p = readADC();
	
	controller->x_val = joystickPercent(*p, controller);
	controller->y_val = joystickPercent(*(p+1),controller);
	controller->slider1_val = *(p+2);
	controller->slider2_val = *(p+3);
	
	controller->dir = direction(controller->x_val, controller->y_val);
}

void printController(struct controllers *controller) { 
	printf("Joystick: X = %4d Y = %4d dir = %d  Sliders: 1 = %3d 2 = %3d  Buttons: 1 = %d 2 = %d\n\r", controller->x_val, controller->y_val, controller->dir, controller->slider1_val, controller->slider2_val,  (bool)bitIsSet(PIND, PD3),  (bool)bitIsSet(PIND, PD4));
}

 enum directions direction(signed int x_val, signed int y_val) {
	signed int deadzone = 20;
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

void calibrateJoystick(struct controllers *controller) {
	uint8_t *p;
	p = readADC();
	
	controller->x_zero = joystickPercent(*p, controller);
	controller->y_zero = joystickPercent(*(p+1), controller);
}

signed int joystickPercent(uint8_t val, struct controllers *controller) {
	signed int per_val;
	if (val >= controller->x_zero) {
		per_val = ((signed int)val - controller->x_zero) * 100.0/(255.0-controller->x_zero);
		return per_val;
	}
	else if (val <= (controller->x_zero-1)) {
		per_val = (signed int)val * 100.0 /(controller->x_zero-1) - 100.0;
		return per_val;
	}
}