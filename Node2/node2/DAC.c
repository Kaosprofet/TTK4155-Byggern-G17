#ifndef INCLUDES_H
#include "includes.h"
#endif

int16_t DACC_value = 0;

void DAC_init(void){
	//DACC->DACC_WPMR=DACC_WPMR_WPEN;
	//DACC->DACC_CR=DACC_CR_SWRST; //Software reset of DACC
	//while(RTT->RTT_VR<RTT->RTT_VR+100){}
    DACC->DACC_MR |=DACC_MR_TRGEN_DIS;//Set DAC mode free run (no clock prescaler)
	DACC->DACC_MR |=DACC_MR_WORD_HALF;//Sets the DAC to read only [15-0] bits in the DACC_CDR register
	DACC->DACC_CHDR = DACC_CHDR_CH1;
    DACC->DACC_CHER = DACC_CHER_CH1; //DAC sends output on DAC 1
	DACC->DACC_MR |= DACC_MR_USER_SEL_CHANNEL1;
	PMC->PMC_PCER1 |= 1 << (ID_DACC - 32);
	DACC->DACC_MR |= DACC_MR_STARTUP_0; //Sets the DAC startup wait timer to 0'
	DACC->DACC_CDR = DACC_value; //Initializes DAC value
}

void DAC_set_output(void){
	int16_t re_mapped_y= map(controller.y, -100, 100, -2047, 2047); //Remaps controller value
	delay_us(400);
	if ((DACC_ISR_TXRDY) == 1){ //Checks if DACC FIFO is ready
		if (re_mapped_y < -50){
			DACC_value = DACC_value - 1;
		}
		else if (re_mapped_y > 1900){
			DACC_value = DACC_value + 1;
		}
		if (DACC_value > 4095){
			DACC_value = 4095;
		}
		else if (DACC_value < 0){
			DACC_value = 0; 
		}
		DACC->DACC_CDR = DACC_value; //Sets the value in the DACC_CDR register to be the re_mapped_value
		//while(RTT->RTT_VR<RTT->RTT_VR+1){}
		//printf("%d\n\r",DACC_value);
		//printf("%d\n\r",DACC_value);
	}

}