#ifndef JOYSTICK_H_
#define JOYSTICK_H_

#include <avr/io.h>

void initButton(void);						// Initializes booth buttons
void updateController(void);				// Updates all values for controller
void printController(void);					// Prints all values for controller
signed int joystickPercent(uint8_t val);	// Converts joystick ADC values to percent
void calibrateJoystick(void);
void CAN_send_inputData(void);					
enum directions direction(signed int x_val, signed int y_val);	// Calculates joystick direction


#endif