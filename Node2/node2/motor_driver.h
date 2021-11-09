#ifndef MOTOR_DRIVER_H_
#define MOTOR_DRIVER_H_



int16_t PI_controller(int16_t r, int16_t y);

void motor_controll_init(void);
void motor_controll(void);

int encoder_read(void);

#endif