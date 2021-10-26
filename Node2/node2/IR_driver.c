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


uint16_t ADC_read(void){ 
	return ADC->ADC_CDR[0]; //Reads the last ADC converted ADC data. 
	}
	
void IR_init(void){
	ADC_init();
}

//Running average filter
uint16_t IR_filteredValue(void){
	//Moving all values in the running register
	for(int i = 0; i<filterLength-1;i++){ IR_raf[i+1] = IR_raf[i];}
		
	//Adding the latest value	
	IR_raf[0] = ADC_read();
	
	//Summarizes the the last values and returns the average
	uint32_t sum = 0;
	for(int j=0; j<filterLength;j++){ sum += IR_raf[j];}
	return sum/filterLength;
}

int IR_blocked(void){
	uint16_t ir_value = IR_filteredValue();
	if(ir_value < IR_BLOCK_THRESHOLD){ return 1;}
	else {return 0;}
}

void IR_print(void){
	printf("_IR_ Raw value: %d  Filtered value: %d   Blocked: %d \n\r",ADC_read(),IR_filteredValue(),IR_blocked());
}