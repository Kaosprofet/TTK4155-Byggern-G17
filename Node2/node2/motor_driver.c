#ifndef INCLUDES_H
#include "includes.h"
#endif

//---------OUTPUT-MJ1----------
#define mDIR PIO_PD10 //MJ1 DIR - PIN32 - PD10
#define mEN PIO_PD9  //EN - PIN30 - PD9
#define mSEL PIO_PD2 //SEL - PIN27 - PD2
#define mNOT_RST PIO_PD1 //NOT_RST - PIN26 - PD1
#define mNOT_OE PIO_PD0 //NOT_OE - PIN25 - PD0
//---------INPUT-MJ2----------
//DO0 to DO7 - PIN33 to PIN40 - PC1-PC8
#define encoderDataMask (0xFF<<1)
#define ENCODER_MAX 4000
#define MOTOR_MAX 4000
//PI controller
#define N 10
#define N2 100
int16_t error_vec[N];
int16_t error_vec2[N2];

int16_t PI_controller_position(int16_t r, int16_t y){
	int16_t e=r-y;
	uint8_t Kp =1;
	uint8_t Ti = 2;
	for(int i = 0; i<N-1;i++){
		error_vec[i+1] = error_vec[i];
	}
	error_vec[0]= e; 
	int32_t e_sum = 0;
	for(int j=0; j<N;j++){
		e_sum += error_vec[j];
	}
	
	if(e<10){
		Kp = 3;
		if (e<5){
			Kp = 10;
		}
	}
		
	return (Kp*e+Ti*e_sum);
}



int16_t PI_controller_speed(int16_t r, int16_t y, uint16_t position){
		int16_t e=r-y;
		float Kp =0.01;
		float Ti = 0.02;
		float Td = 0.0001;
		for(int i = 0; i<N2-1;i++){
			error_vec2[i+1] = error_vec2[i];
		}
		
		error_vec2[0]= e;
		int32_t e_sum = 0;
		for(int j=0; j<N2;j++){
			e_sum += error_vec2[j];
		}
		if (e_sum > ENCODER_MAX){e_sum = ENCODER_MAX;}
		else if(e_sum < 0){e_sum = 0;} 
		delay_ms(100);
		return (Kp*e+Ti*e_sum);
}

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


uint8_t MotorRef = 0;
#define MotorRef_max 255
uint16_t encoder_prev = 0;
uint16_t encoder = 0;

#define joy_filter_N 50
int8_t joystickvalues[joy_filter_N];

uint16_t joystick_filter(void){
		for(int i = 0; i<joy_filter_N-1;i++){ joystickvalues[i+1] = joystickvalues[i];}
		
		//Adding the latest value
		joystickvalues[0] = controller.x;
		
		//Summarizes the the last values and returns the average
		uint32_t sum = 0;
		for(int j=0; j<joy_filter_N;j++){ sum += joystickvalues[j];}
}

uint16_t ref = 0;

void motor_controll(difficulty diff){
	//Regulator
	//int32_t r = controller.slider_2_val; 
	uint16_t PI_out = 0;
	
	encoder_prev = encoder;
	uint16_t encoder = encoder_read();
	printf("Encoder: %d\n\r", encoder);
	uint16_t speed = encoder_prev - encoder;
	
	//int16_t y = map(encoder,0,10000,0,255);
	//int32_t PI_out = PI_controller(r,y);
	//printf("Reference: %d, encoder: %d, PI Output: %d\n\r",r,y,PI_out);
	int8_t joystick = joystick_filter();
	if(diff == HARD){
		//Uses PI-regulated position
		ref = JoystickSpeedControll(ref);
		PI_out = PI_controller_position(ref,encoder);
	}
	else if(diff == EASY){
		//Joystick sets motor voltage. Prohibits it from running into walls.  
		uint8_t gain = 10;
		PI_out = joystick*10;
		if(encoder>8000 && PI_out>0){
			PI_out = 0;
		}
		else if(encoder <= 0 && PI_out<0){
			PI_out = 0; 
		}
		
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
	//printf("Slider %d, Reference: %d, encoder: %d, PI Output: %d, DAC Output: %d\n\r",controller.slider_2_val,MotorRef,y,PI_out,DAC_out);
	//printf("Val: %d\n\r", controller.slider_2_val);
}



uint32_t joy_counter = 0;

uint32_t JoystickSpeedControll(int16_t r){
	joy_counter +=1;
	
	
	int16_t joystickVal = controller.x;
	
	if(joy_counter >= 50){
		r += joystickVal;
		joy_counter = 0;
	}
	
	if(r>MotorRef_max){
		r= MotorRef_max;
		printf("Ref at max right \n\r");
	}
	if(r<0){
		r=0;
		printf("Ref at max left \n\r");
	}
	return r;
}