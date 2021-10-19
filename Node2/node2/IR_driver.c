#ifndef INCLUDES_H
#include "includes.h"
#endif

void ADC_init(void){
	ADC->ADC_MR=ADC_MR_FREERUN;//Set ADC mode freerun (no clock prescaler)
	ADC->ADC_CHER = ADC_CHER_CH0; //Enable AD0 peripheral.
	PMC->PMC_PCR = PMC_PCR_EN | PMC_PCR_DIV_PERIPH_DIV_MCK | (ID_ADC << PMC_PCR_PID_Pos); //Enable clock for ADC
	PMC->PMC_PCER1 |= 1 << (ID_ADC -32);
	ADC->ADC_CR = ADC_CR_START; //Starts ADC conversion
}

/*
uint16_t ADC_read(void){ 
	return ADC->ADC_CR[0];
	}
	
*/
void IR_init(void){
	ADC_init();
}

uint8_t IR_raw(void){
	
}

uint8_t digitalFilter(uint8_t ir_value){
	
}

int IR_blocked(uint8_t ir_value){
	if(ir_value < IR_BLOCK_THRESHOLD){ return 1;}
	else {return 0;}
}