#ifndef INCLUDES_H
#include "includes.h"
#endif

void initButton(void) {
	clearBit(DDRD,PD3);
	clearBit(DDRD,PD4);
}

void updateController(void) {
	uint8_t *p;
	
	p = readADC();
	
	controller.org_x_val = *p;
	controller.org_y_val = *(p+1);
	controller.x_val = joystickPercent(*p);
	controller.y_val = joystickPercent(*(p+1));
	controller.slider1_val = *(p+2);
	controller.slider2_val = *(p+3);
	
	controller.dir = direction(controller.x_val, controller.y_val);
}

void printController(void) { 
	printf("Joystick: X = %4d Y = %4d X_org = %4d T_org = %4d dir = %d  Sliders: 1 = %3d 2 = %3d  Buttons: 1 = %d 2 = %d\n\r", controller.x_val, controller.y_val, controller.org_x_val, controller.org_y_val, controller.dir, controller.slider1_val, controller.slider2_val,  (bool)bitIsSet(PIND, PD3),  (bool)bitIsSet(PIND, PD4));
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

void calibrateJoystick(void) {
	uint8_t *p;
	p = readADC();
	
	controller.x_zero = *p;
	controller.y_zero = *(p+1);
}

signed int joystickPercent(uint8_t val) {
	// This functions interpolates the measured value 0-255 into a -100-100 percent value. We currently assumes that the zero point
	// remains equal between the two axis.
	signed int per_val;
	if (val >= controller.x_zero) {
					//(x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min
		per_val = ((signed int)val - controller.x_zero) * 100.0/(255.0-controller.x_zero);
		return per_val;
	}
	else if (val < controller.x_zero) {
		per_val = (signed int)val * 100.0 /(controller.x_zero-1) - 100.0;
		return per_val;
	}
	return 0;
}

void CAN_send_inputData(void){
	can_message manInput;
	manInput.ID = CAN_ID_ManInputData;
	manInput.data[0] = controller.x_val; //Joystick x-axis value %
	manInput.data[1] = controller.y_val; //Joystick y-axis value %
	manInput.data[2] = bitIsSet(PIND,PD4); //Button1
	manInput.data[3] = controller.slider1_val; //Slider 1 value %
	manInput.data[4] = controller.slider2_val; //Slider 2 value %
	manInput.length = 5; 
	CAN_send_message(&manInput); //Sends the message
}
