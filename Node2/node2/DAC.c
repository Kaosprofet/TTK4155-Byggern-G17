#ifndef INCLUDES_H
#include "includes.h"
#endif

void DAC_init(void){
	//DACC->DACC_WPMR=DACC_WPMR_WPEN;
    DACC->DACC_MR=DACC_MR_TRGEN_DIS;//Set DAC mode freerun (no clock prescaler)
	DACC->DACC_MR=DACC_MR_WORD_HALF;//Sets the DAC to read only [15-0] bits in the DACC_CDR register
    DACC->DACC_CHER = DACC_CHER_CH1; //DAC sends output on DAC 1
}

void DAC_set_output(void){
	uint16_t re_mapped_y= map(controller.y, -100, 100, 0, 65535);
    DACC->DACC_CDR = re_mapped_y; //Sets the value in the DACC_CDR register to be the re_mapped_value. 
}