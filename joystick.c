#include "joystick.h"


#include "adc.h"
#include "includes.h"

volatile struct joysticks {
	volatile double x_val;
	volatile double y_val;
	volatile enum directions dir;
};

volatile struct controllers {
	volatile uint8_t slider1_val;
	volatile uint8_t slider2_val;
	
};

enum directions direction(uint8_t x_val, uint8_t y_val) {
	uint8_t deadzone = 10;
	
	if (abs(x_val) > abs(y_val)) { //x>y -> left/right
		if (x_val > 127 + deadzone) {
			return RIGHT;
		}
		else if (x_val < 127-deadzone) {
			return LEFT;
		}
	}
	else {						//x<y -> up/down
		if (y_val > 127 + deadzone) {
			return UP;
		}
		else if (y_val < 127 - deadzone) {
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
	
	joystick.dir = direction(x_val, y_val);
	
	joystick.x_val = joystickPercent(x_val);
	joystick.y_val = joystickPercent(y_val);
	
	
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

double joystickPercent(uint8_t val) {
	
	double per_val = val * (double)(100 + 100) / 255 + 100;
	return per_val;
}


uint8_t fetchSlider(uint8_t num) {
	return 0;
}