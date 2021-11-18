#ifndef INCLUDES_H
#include "includes.h"
#endif

//Motorbox
#define mDIR PIO_PD10 //MJ1 DIR - PIN32 - PD10
#define mEN PIO_PD9  //EN - PIN30 - PD9
#define mSEL PIO_PD2 //SEL - PIN27 - PD2
#define mNOT_RST PIO_PD1 //NOT_RST - PIN26 - PD1
#define mNOT_OE PIO_PD0 //NOT_OE - PIN25 - PD0
#define encoderDataMask (0xFF<<1)

//PI controller
#define Slider_Kp 1
#define Slider_Ti 1
#define Joystick_Kp 1
#define Joystick_Ti 1
#define TimeStep 1
int16_t error_sum;

//Encoder
#define Encoder_min 0
#define Encoder_max 9000

//Motor
#define MOTOR_MAX 2000

//Define joystick controll
#define Joystick_sample_step 30



void motor_controll_init(void){
	//Enable PIO to controll the pins of the motor controller box MJ1
	PIOD->PIO_PER |= mDIR|mEN|mSEL|mNOT_RST|mNOT_OE;
	//Setting as output
	PIOD->PIO_OER |= mDIR|mEN|mSEL|mNOT_RST|mNOT_OE;
	
	//Enable PIO controll of the MJ2
	PIOC ->PIO_PER |= encoderDataMask;
	//Setting as input
	PIOC ->PIO_ODR |= encoderDataMask;
	
	//Enable clock on PIOC
	PMC->PMC_PCR |= PMC_PCR_EN |PMC_PCR_DIV_PERIPH_DIV_MCK|(ID_PIOC <<PMC_PCR_PID_Pos);
	PMC->PMC_PCER0 |= 1<<(ID_PIOC);
}

int encoder_read(void){
	//Set !OE low to enable output of encoder
	clearBit(PIOD,mNOT_OE);
	
	//Set SEL low to get high byte
	clearBit(PIOD, mSEL);
	
	//wait 20 microseconds
	delay_us(30);
	
	//Read MSB
	uint8_t encoder_msb =  readPin(PIOC,encoderDataMask)>>1; //Reads the data pins, bitshiftet back 1, since PC0 is not used
	
	//Set SEL to high to get low byte
	setBit(PIOD,mSEL);
	
	//wait 20 microseconds
	delay_us(30);
	
	//Read LSB
	uint8_t encoder_lsb = readPin(PIOC,encoderDataMask)>>1;
	
	clearBit(PIOD,mNOT_RST);
	setBit(PIOD,mNOT_RST);
	
	//Set !OE high to disable output of encoder;
	setBit(PIOD,mNOT_OE);
	
	//Processing data, combining MSB and LSB
	uint16_t encoder_data = (encoder_msb<<8)|encoder_lsb;
	
	//If the MSB is set, it is a negative number. Invert it
	if (encoder_data&(0x8000)){
		return ((uint16_t)(~encoder_data));
	}
	
	return -encoder_data;		
}

void encoder_reset(void){
		//Toggle !RST to reset encoder
		clearBit(PIOD,mNOT_RST);
		setBit(PIOD,mNOT_RST);
}



int16_t pos_ref = 0;

void motor_controll(difficulty diff){
	int16_t PI_out = 0;
	int16_t joystick = (int16_t)(controller.x);
	int16_t encoder = encoder_read();
	
	
	if(diff == HARD){
		//Uses PI-regulated position
		pos_ref = JoystickSpeedControll(pos_ref);
		PI_out = PI_controller_position(pos_ref,encoder,Joystick_Kp,Joystick_Ti);
	}
	else if(diff == EASY){
		//Joystick sets motor voltage. Prohibits it from running into walls.  
		uint8_t gain = 10;
		PI_out = joystick*10;
		if(encoder>Encoder_max && PI_out>0){
			PI_out = 0;
		}
		else if(encoder <= Encoder_min && PI_out<0){
			PI_out = 0; 
		}
	}
	else if(diff == SLIDER){
		int16_t r = controller.slider_2_val;
		int16_t y = map(encoder,0,Encoder_max,0,255);
		PI_out = PI_controller_position(r,y,Slider_Kp,Slider_Ti);
		
	}
	
	uint16_t DAC_out = 0;
	//Changing direction
	if(PI_out>=0){ 
		clearBit(PIOD,mDIR);
		DAC_out = PI_out;
	}
	else{ 
		setBit(PIOD,mDIR); 
		DAC_out = -PI_out;
	}
	
	if(DAC_out>MOTOR_MAX){
		DAC_out = MOTOR_MAX;
	}
	//Writing output
	setBit(PIOD,mEN);
	DAC_set_output(DAC_out);
	//printf("Encoder: %d, Joystick: %d, ref: %d, PI_output %d, DAC_out: %d \n\r", encoder,joystick,ref, PI_out, DAC_out);
}



uint16_t joy_counter = 0;
int16_t JoystickSpeedControll(int16_t r){
	joy_counter +=1;
	int16_t joystickVal = (int16_t)controller.x;
	if(joy_counter >= 20){
		r += joystickVal;
		joy_counter = 0;
	}
	return r;
}

int16_t PI_controller_position(int16_t r, int16_t y, int Kp, int Ti){
	int16_t e=r-y;
	error_sum += e;
	int16_t P = Kp*e;
	int16_t I = TimeStep*Ti*error_sum;
	int16_t output = P+I;
	return output;
}

