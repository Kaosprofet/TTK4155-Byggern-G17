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
void servo_slider_controll(void); 

//Solenoid
void solenoid_init(void);
void solenoidControll(void); //Uses the button

#endif	