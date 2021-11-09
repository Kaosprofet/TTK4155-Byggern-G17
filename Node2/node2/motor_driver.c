#ifndef INCLUDES_H
#include "includes.h"
#endif

//---------OUTPUT-MJ1----------
#define mDIR PIO_PD10 //MJ1 DIR - PIN32 - PD10
#define mEN PIO_PD9  //EN - PIN30 - PD9
#define mSEL PIO_PD2 //SEL - PIN27 - PD2
#define mNOT_RST PIO_PD1 //NOT_RST - PIN26 - PD1
#define mNOT_OE PIO_PD0 //NOT_OE - PIN25 - PD0
//---------INPUT-MJ2----------
//DO0 to DO7 - PIN33 to PIN40 - PC1-PC8
uint8_t motorDataPins[8] ={PIO_PC1, PIO_PC2, PIO_PC3, PIO_PC4, PIO_PC5, PIO_PC6, PIO_PC7, PIO_PC8};

void motor_controll_init(void){
	//Enable PIO to controll the pins of the motor controller box MJ1
	PIOD->PIO_PER |= mDIR|mEN|mSEL|mNOT_RST|mNOT_OE;
	//Setting as output
	PIOD->PIO_OER |= mDIR|mEN|mSEL|mNOT_RST|mNOT_OE;
	
}

uint16_t encoder_read(void){
	
}
