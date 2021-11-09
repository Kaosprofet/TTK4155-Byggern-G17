#ifndef INCLUDES_H
#include "includes.h"
#endif

uint16_t DACC_value = 2047;

void DAC_init(void){
	//DACC->DACC_WPMR=DACC_WPMR_WPEN;
	//DACC->DACC_CR=DACC_CR_SWRST; //Software reset of DACC
	//while(RTT->RTT_VR<RTT->RTT_VR+100){}
    DACC->DACC_MR=DACC_MR_TRGEN_DIS;//Set DAC mode free run (no clock prescaler)
	DACC->DACC_MR=DACC_MR_WORD_HALF;//Sets the DAC to read only [15-0] bits in the DACC_CDR register
    DACC->DACC_CHER = DACC_CHER_CH0; //DAC sends output on DAC 0
	DACC->DACC_MR = DACC_MR_USER_SEL_CHANNEL0;
	PMC->PMC_PCER1 |= 1 << (ID_DACC - 32);
	DACC->DACC_MR = DACC_MR_STARTUP_0; //Sets the DAC startup wait timer to 0'
	DACC->DACC_CDR = DACC_value; //Initializes DAC value
}

void DAC_set_output(void){
	uint16_t re_mapped_y= map(controller.y, -100, 100, 0, 4095); //Remaps controller value
	//printf("%d\n\r",DACC_value);
	//while(RTT->RTT_VR<RTT->RTT_VR+0.05){}
	if ((DACC_ISR_TXRDY) == 1){ //Checks if DACC FIFO is ready
		if (re_mapped_y > 4000){
			DACC_value = DACC_value +10; 
		}
		else if (re_mapped_y < 100){
			DACC_value = DACC_value -10;
		}
		else {
		}
		if (DACC_value < 50){
			DACC_value = 50;
		}
		else if (DACC_value > 4000){
			DACC_value = 4000;
		}
		DACC->DACC_CDR = DACC_value; //Sets the value in the DACC_CDR register to be the re_mapped_value
		//while(RTT->RTT_VR<RTT->RTT_VR+1){}
		//printf("%d\n\r",DACC_value);
		//printf("%d\n\r",DACC_value);
	}

}