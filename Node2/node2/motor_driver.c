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
	
	//Enable PIO controll of the MJ2
	PIOC ->PIO_PER |= PIO_PC1|PIO_PC2|PIO_PC3|PIO_PC4|PIO_PC5|PIO_PC6|PIO_PC7|PIO_PC8;
	//Setting as input
	PIOC ->PIO_ODR |= PIO_PC1|PIO_PC2|PIO_PC3|PIO_PC4|PIO_PC5|PIO_PC6|PIO_PC7|PIO_PC8;
	
	//Enable clock on PIOC
	PMC->PMC_PCR |= PMC_PCR_EN |PMC_PCR_DIV_PERIPH_DIV_MCK|(ID_PIOC <<PMC_PCR_PID_Pos);
	PMC->PMC_PCER0 |= 1<<(ID_PIOC);
	
}

uint16_t encoder_read(void){
	//Set !OE low to enable output of encoder
	clearBit(PIOD,mNOT_OE);
	//Set SEL low to get high byte
	clearBit(PIOD, mSEL);
	//wait 20 microseconds*
	
	uint8_t encoder_msb =  readPin(PIOC,(0xFF << DO0_IDX));
}