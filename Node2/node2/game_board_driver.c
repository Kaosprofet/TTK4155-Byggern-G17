#ifndef INCLUDES_H
#include "includes.h"
#endif

//IR-diode
#define IR_BLOCK_THRESHOLD 250 //Defines the threshold for which IR_blocked returns 1
#define filterLength 100 //Defines the length of RA-filter
uint16_t IR_raf[filterLength];
uint8_t filter_charge = 0;




void init_game_board(void) {
	IR_init();
	init_servo();
	solenoid_init();
}

//-------------------------SOLENOID-----------------------------------

void solenoid_init(void){
	PIOC->PIO_PER |= PIO_PC13;
	PIOC->PIO_OER |= PIO_PC13;
	PIOC->PIO_PUDR |= PIO_PC13;
	setBit(PIOC, PIO_PC13);
	RTT->RTT_MR = 0x20; 
}

void solenoidControll(void){
	static uint32_t kick_time = 0;

	if(controller.button_state > 9) {
		//Extend kicker
		clearBit(PIOC, PIO_PC13);
		kick_time = RTT->RTT_VR+200;
		// wait to stabilise system
		while (RTT->RTT_VR < kick_time){}
	}
	else {setBit(PIOC, PIO_PC13);}
}

// -----------------------------------------------IR---------------------------------------------
//Pin 44 at PC19


void ADC_init(void){
	ADC->ADC_MR=ADC_MR_FREERUN;//Set ADC mode freerun (no clock prescaler)
	ADC->ADC_CHER = ADC_CHER_CH0; //Enable AD0 peripheral.
	PMC->PMC_PCR = PMC_PCR_EN | PMC_PCR_DIV_PERIPH_DIV_MCK | (ID_ADC << PMC_PCR_PID_Pos); //Enable clock for ADC
	PMC->PMC_PCER1 |= 1 << (ID_ADC -32);
	ADC->ADC_CR = ADC_CR_START; //Starts ADC conversion
}

uint16_t ADC_read(void) { 
	uint16_t val = ADC->ADC_CDR[0];
	if(val == 0){ val = IR_BLOCK_THRESHOLD;} //Initial zero protection
	return val; //Reads the last ADC converted ADC data. 
	}
	
void IR_init(void) {
	ADC_init();
}

//Running average filter
uint16_t IR_filteredValue(void) {
	//Moving all values in the running register
	for(int i = 0; i<filterLength-1;i++){ IR_raf[i+1] = IR_raf[i];}
		
	//Adding the latest value	
	IR_raf[0] = ADC_read();
	
	//Summarizes the the last values and returns the average
	uint32_t sum = 0;
	for(int j=0; j<filterLength;j++){ sum += IR_raf[j];}
		
	//Makes sure that the filter does not return a value before it is charged
	if(filter_charge < filterLength){
		filter_charge +=1;
		return IR_BLOCK_THRESHOLD;
	}
	else {
		return sum/filterLength;
	}	
}

// Registers blocking of IR diode
int IR_blocked(void) {
	uint16_t ir_value = IR_filteredValue();
	if(ir_value < IR_BLOCK_THRESHOLD) {
		printf("IR value: %d\n\r",ir_value );
		return 1;
		}
	else {return 0;}
}

// Printing function fo testing
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
	//Enable PWM
	PWM->PWM_ENA = PWM_ENA_CHID5;
}

// Position servo based on joystick input signal
void position_servo(int8_t position) {
	position = position + servo_offset;
	//printf("Position is %d\n\r",position);
	
	uint32_t pos = pwm_center + position*15;
	//printf("Position is %d, new position is: %d\n\r",position, pos);
	
	PWM->PWM_CH_NUM[5].PWM_CDTY = pos;
	//printf("Value of pwm_center - position is %d\n\r",(PWM->PWM_CH_NUM[5].PWM_CDTY));
}

// Position servo based on slide-potmeter position
void servo_slider_controll(void){
	uint8_t slider;
	if(game.mode == 1){
		 slider = controller.slider_1_val;
	}
	else{
		 slider = controller.slider_2_val;
	}
	int8_t angle = map(slider,255,0,-100,100);
	position_servo(angle);
}

void servo_joystick_controll(void){
	position_servo(controller.x);	
}