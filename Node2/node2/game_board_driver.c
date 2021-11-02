#ifndef INCLUDES_H
#include "includes.h"
#endif

void init_game_board(void){
	IR_init();
	init_servo();
	solenoid_init();
	motor_controll_init();
}

//------------------------PI CONTROLLER --------------------------

uint16_t PI_controller(uint16_t r, uint16_t y){
	uint16_t e = r - y;
	//Shifting the array of error values
	for(int i = 0; i<N-1;i++){ e_vec[i+1] = e_vec[i];}
	e_vec[0] = e; //Adding the last value;
	uint16_t e_sum = 0;
	//Calculation the sum
	for(int j=0; j<N;j++){ e_sum += e_vec[j];}
	return Kp*e+Ti*e_sum;
}



//-------------------------SOLENOID-----------------------------------

void solenoid_init(void){
	
}

void solenoidControll(void){
	uint8_t button = controller.button_state;
	if(button > 0){
		//extend
	}
	else {
		//retract
	}
}

// -----------------------------------------------IR---------------------------------------------
//Pin 44 at PC19
#define IR_BLOCK_THRESHOLD 500 //Defines the threshold for which IR_blocked returns 1
#define filterLength 20 //Defines the length of RA-filter

uint16_t IR_raf[filterLength];

void ADC_init(void){
	ADC->ADC_MR=ADC_MR_FREERUN;//Set ADC mode freerun (no clock prescaler)
	ADC->ADC_CHER = ADC_CHER_CH0; //Enable AD0 peripheral.
	PMC->PMC_PCR = PMC_PCR_EN | PMC_PCR_DIV_PERIPH_DIV_MCK | (ID_ADC << PMC_PCR_PID_Pos); //Enable clock for ADC
	PMC->PMC_PCER1 |= 1 << (ID_ADC -32);
	ADC->ADC_CR = ADC_CR_START; //Starts ADC conversion
}


uint16_t ADC_read(void){ 
	uint16_t val = ADC->ADC_CDR[0];
	if(val == 0){ val = IR_BLOCK_THRESHOLD}
	return val; //Reads the last ADC converted ADC data. 
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
	bool ir_value_triggered = (ir_value < IR_BLOCK_THRESHOLD);
	if(ir_value_triggered) { // && ir_last_value!=true && ir_debounce == 0
		//ir_last_value = true;
		//ir_debounce = IR_DEBOUNCE_TIME; 
		return 1;
	}
	else {
		//if (ir_debounce != 0) {
		//	ir_debounce = ir_debounce-1;
		//	//printf("%d\n\r",ir_debounce);
		//}
		//if (ir_debounce == 0 && ir_value_triggered != true) {
		//	ir_last_value = false;
		//}
		return 0;
	}
}

void IR_print(void){
	printf("_IR_ Raw value: %d  Filtered value: %d   Blocked: %d \n\r",ADC_read(),IR_filteredValue(),IR_blocked());
}

//---------------------------SERVO ---------------------------

#define servo_max 150
#define servo_min -150
#define servo_offset 0
static int32_t pwm_center = 3500;

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

	//Chanel mode (Her setter vi divisorverdien)
	PWM->PWM_CH_NUM[5].PWM_CMR = PWM_CMR_CPOL|PWM_CMR_CPRE_MCK_DIV_32;

	//Period
	PWM->PWM_CH_NUM[5].PWM_CPRD = 52500;

	//Center pos
	PWM->PWM_CH_NUM[5].PWM_CDTY = pwm_center;


	//Enable PWM (Vet ikke hva vi har definert som enable)
	PWM->PWM_ENA = PWM_ENA_CHID5;
}

void position_servo(int8_t position) {
	position = position + servo_offset;
	//printf("Position is %d\n\r",position);
	
	uint32_t pos = pwm_center + position*15;
	//printf("Position is %d, new position is: %d\n\r",position, pos);
	
	
	PWM->PWM_CH_NUM[5].PWM_CDTY = pos;
	//printf("Value of pwm_center - position is %d\n\r",(PWM->PWM_CH_NUM[5].PWM_CDTY));
}