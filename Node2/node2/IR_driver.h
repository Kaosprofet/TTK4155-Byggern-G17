#ifndef IR_driver_H_
#define IR_driver_H_

#define IR_DIODE 23 //*
//Pin 44 at PC19
#define IR_BLOCK_THRESHOLD 500 //Defines the threshold for which IR_blocked returns 1
#define filterLength 20 //Defines the length of RA-filter

uint16_t IR_raf[filterLength];

void IR_init(void); //Initializes everything needed to for ADC on channel 1
uint8_t IR_raw(void); //Reads the variable
uint16_t IR_filteredValue(void); //Filters the data**
int IR_blocked(void); //returns 1 if it gets blocked**


#endif