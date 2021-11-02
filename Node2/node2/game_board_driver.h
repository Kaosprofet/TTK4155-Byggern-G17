#ifndef GAME_BOARD_DRIVE_H_
#define GAME_BOARD_DRIVE_H_

//IR related
void IR_init(void); //Initializes everything needed to for ADC on channel 1
uint8_t IR_raw(void); //Reads the variable
uint16_t IR_filteredValue(void); //Filters the data
int IR_blocked(void); //returns 1 if it gets blocked
void IR_print(void); //Printing to UART for testing purposes



#endif	