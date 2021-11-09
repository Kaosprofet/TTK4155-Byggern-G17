#ifndef MOTOR_DRIVER_H_
#define MOTOR_DRIVER_H_

//PI controller
#define Kp 0.01
#define Ti 1
#define N 10



void motor_controll_init(void);
void motor_controll(void);

int encoder_read(void);

#endif