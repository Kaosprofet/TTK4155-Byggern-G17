#ifndef IR_driver_H_
#define IR_driver_H_

#define IR_DIODE 23 //*
//Pin 44 at PC19

#define IR_BLOCK_THRESHOLD 500 //*

void IR_init(void);
uint8_t IR_raw(void); //Reads the variable***
uint8_t digitalFilter(uint8_t ir_value); //Filters the data**
int IR_blocked(uint8_t ir_value); //returns 1 if it gets blocked**


#endif