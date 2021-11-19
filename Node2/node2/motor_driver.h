#ifndef MOTOR_DRIVER_H_
#define MOTOR_DRIVER_H_

typedef enum{EASY = 0,SLIDER = 1, HARD = 2}difficulty;
int16_t PI_controller_position(int16_t r, int16_t y, float Kp, float Ti);

void motor_controll_init(void);
void motor_controll(void);

int encoder_read(void);

int16_t JoystickSpeedControll(int16_t r);
#endif