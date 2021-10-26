#ifndef SERVO_H
#define SERVO_H

#include <stdint.h>

void init_servo(void);
void position_servo(int8_t position);

#endif