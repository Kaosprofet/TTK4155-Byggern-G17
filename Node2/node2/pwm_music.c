// Driver used for generating PWM for music playlist

#ifndef INCLUDES_H
#include "includes.h"
#endif

#define MCK 84E6
#define DIVB 17
#define CLK_B 1E6

void music_pwm_init(void){
	PIOC->PIO_IDR |= PIO_PC18B_PWMH6; 	// Disable interrupts
	PIOC->PIO_ABSR |= PIO_PC18B_PWMH6; 	// select timer 0
	PIOC->PIO_PDR |= PIO_PC18B_PWMH6; 	//Disable parallel io
	
	//Enable Clock for PWM in PMC
	PMC->PMC_PCR = PMC_PCR_EN | PMC_PCR_DIV_PERIPH_DIV_MCK | (ID_PWM << PMC_PCR_PID_Pos);
	PMC->PMC_PCER1 |= 1 << (ID_PWM - 32);
	
	//Enable PWM for PIN 44
	PWM->PWM_ENA |= PWM_ENA_CHID6;

    //Set PWM clock B to 
    PWM->PWM_CLK |= PWM_CLK_PREB(0) | PWM_CLK_DIVB(DIVB);

    //Set pwm on pin 44 to be left allined so formulas applies
    PWM->PWM_CH_NUM[6].PWM_CMR = PWM_CMR_CPRE_CLKB;
	
	//Intialize channel number 6 to zero so it does not play at the beginning
    PWM->PWM_CH_NUM[6].PWM_CPRD = 0;

    //Set duty cycle to a known value
    PWM->PWM_CH_NUM[6].PWM_CDTY = 1000;
}

    void pwm_select_frequency(int frequency){ //Period is (CPRD*DIVB/MCK) and freq is 1/Period
        if (frequency == 0){ //If frequency set is less than 0, set it as 0
           PWM->PWM_CH_NUM[6].PWM_CPRD = 0;
        }
        else { //Else sets the correct period based on frequency
            int period_music = (MCK/(frequency * DIVB)); //Calculate period for given frequency
            PWM->PWM_CH_NUM[6].PWM_CPRD = period_music; //Set period to PWM  
        }
    }
