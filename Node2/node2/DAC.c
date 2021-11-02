#ifndef INCLUDES_H
#include "includes.h"
#endif

void DAC_init(void){
	//DACC->DACC_WPMR=DACC_WPMR_WPEN;
    DACC->DACC_MR=DACC_MR_TRGEN_DIS;//Set DAC mode freerun (no clock prescaler)
	DACC->DACC_MR=DACC_MR_WORD_HALF;//Sets the DAC to read only [15-0] bits in the DACC_CDR register
    DACC->DACC_CHER = DACC_CHER_CH1; //DAC sends output on DAC 1
	DACC->DACC_MR = DACC_MR_USER_SEL_CHANNEL1;
	DACC->DACC_MR = DACC_MR_STARTUP_0; //Sets the DAC startup wait timer to 0
}

void DAC_set_output(void){
	//printf("%d\n\r",controller.y);
	uint16_t re_mapped_y= map(controller.y, -100, 100, 0, 4095);
	if (!DACC_IER_TXRDY){
    DACC->DACC_CDR = re_mapped_y; //Sets the value in the DACC_CDR register to be the re_mapped_value.
	}
	printf("%d\n\r",DACC_ISR_EOC);
}