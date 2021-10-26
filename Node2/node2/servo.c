#ifndef INCLUDES_H
#include "includes.h"
#endif

#define servo_max 150
#define servo_min -150
#define servo_offset 0
static int32_t pwm_center = 3950;

void init_servo(void) {

	// Disable interrupts
	PIOC->PIO_IDR = PIO_PC19B_PWMH5;

	// select timer 0
	PIOC->PIO_ABSR |= PIO_PC19B_PWMH5;

	//Disable parallel io
	PIOC->PIO_PDR = PIO_PC19B_PWMH5;
	
	//Enable Clock for PWM in PMC
	PMC->PMC_PCR = PMC_PCR_EN | (0 << PMC_PCR_DIV_Pos) | PMC_PCR_CMD | (ID_PWM << PMC_PCR_PID_Pos);
	PMC->PMC_PCER1 |= 1 << (ID_PWM - 32);

	//Enable PWM (Vet ikke hva vi har definert som enable)
	PWM->PWM_ENA = PWM_ENA_CHID5;

	//Chanel mode (Her setter vi divisorverdien)
	PWM->PWM_CH_NUM[5].PWM_CMR = PWM_CMR_CPOL|PWM_CMR_CPRE_MCK_DIV_32;

	//Period
	PWM->PWM_CH_NUM[5].PWM_CPRD = 52500;

	//Center pos
	PWM->PWM_CH_NUM[5].PWM_CDTY = pwm_center;


}

void position_servo(int8_t position) {
	position = position + servo_offset;
	//printf("Position is %d\n\r",position);
	
	uint32_t pos = pwm_center + position*15.5;
	//printf("Position is %d, new position is: %d\n\r",position, pos);
	
	
	PWM->PWM_CH_NUM[5].PWM_CDTY = pos;
	//printf("Value of pwm_center - position is %d\n\r",(PWM->PWM_CH_NUM[5].PWM_CDTY));
}