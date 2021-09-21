#ifndef JOYSTICK_H_
#define JOYSTICK_H_

#include <avr/io.h>

volatile struct joysticks fetchJoystick(void);
uint8_t fetchSlider(uint8_t num);
uint8_t joystickPercent(uint8_t val);
enum directions direction(uint8_t x_val, uint8_t y_val);

#endif