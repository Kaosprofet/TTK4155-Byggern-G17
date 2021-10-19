#ifndef SERVO_H
#define SERVO_H

#include <stdint.h>

void init_servo(void);
void position_servo(uint8_t position);

#endif