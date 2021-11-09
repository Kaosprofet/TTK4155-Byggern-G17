#ifndef INCLUDES_H
#include "includes.h"
#endif

void DAC_init(void){
    DACC->DACC_MR |=DACC_MR_TRGEN_DIS;//Set DAC mode free run (no clock prescaler)
	DACC->DACC_MR |=DACC_MR_WORD_HALF;//Sets the DAC to read only [15-0] bits in the DACC_CDR register
	DACC->DACC_CHDR = DACC_CHDR_CH1;
    DACC->DACC_CHER = DACC_CHER_CH1; //DAC sends output on DAC 1
	DACC->DACC_MR |= DACC_MR_USER_SEL_CHANNEL1;
	PMC->PMC_PCER1 |= 1 << (ID_DACC - 32);
	DACC->DACC_MR |= DACC_MR_STARTUP_0; //Sets the DAC startup wait timer to 0
}

void DAC_set_output(uint16_t PID_output){
	delay_us(400);
	if ((DACC_ISR_TXRDY) == 1){ //Checks if DACC FIFO is ready
		DACC->DACC_CDR = PID_output; //Sets the value in the DACC_CDR register to be the re_mapped_value
	}
}