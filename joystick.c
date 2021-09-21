#include "joystick.h"


#include "adc.h"

volatile struct joysticks {
	volatile uint8_t x_val;
	volatile uint8_t y_val;
	volatile enum x_dir;
	volatile enum y_dir;
};

volatile struct joysticks fetchJoystick(void) {
	uint8_t *p;
	volatile struct joysticks joystick;
	
	p = readADC();
	
	joystick.x_val = *p;
	joystick.y_val = *(p+1);
	
	joystick.x_val = joystickPercent(joystick.x_val);
	joystick.y_val = joystickPercent(joystick.y_val);
	
	
	
	return joystick;
}

uint8_t joystickPercent(uint8_t val) {
	
	uint8_t per_val = val*100/255;
	
	return per_val;
}

enum direction(uint8_t x_val, uint8_t y_val) {
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

uint8_t fetchSlider(uint8_t num) {
	return 0;
}