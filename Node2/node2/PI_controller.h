#ifndef PI_CONTROLLER_H_
#define PI_CONTROLLER_H_
#define Kp 1
#define Ti 100
#define N 20

uint16_t e_vec[N];

uint16_t PI_controller(uint16_t r, uint16_t y);

#endif
