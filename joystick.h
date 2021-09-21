#ifndef JOYSTICK_H_
#define JOYSTICK_H_

#include <avr/io.h>

uint8_t fetchJoystick(void);
uint8_t fetchSlider(uint8_t num);

#endif