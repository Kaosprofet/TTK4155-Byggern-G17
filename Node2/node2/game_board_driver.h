#ifndef GAME_BOARD_DRIVE_H_
#define GAME_BOARD_DRIVE_H_

#include <stdint.h>

void init_game_board(void);

//IR related
void IR_init(void); //Initializes everything needed to for ADC on channel 1
uint8_t IR_raw(void); //Reads the variable
uint16_t IR_filteredValue(void); //Filters the data
int IR_blocked(void); //returns 1 if it gets blocked
void IR_print(void); //Printing to UART for testing purposes

//Servo 
void init_servo(void);
void position_servo(int8_t position);

//Solenoid
void solenoid_init(void);
void solenoidControll(void); //Uses the button

//PI controller
#define Kp 1
#define Ti 100
#define N 20
uint16_t e_vec[N];
uint16_t PI_controller(uint16_t r, uint16_t y);

#endif	