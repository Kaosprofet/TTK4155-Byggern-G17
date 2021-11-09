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
#define MOTOR_MAX 1000

int16_t e_vec[N];

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

void motor_controll(void){
	//Regulator
	int32_t r = map(controller.slider_2_val, 0,255, -3000,3000); //Remaps the slider position value
	
	int32_t y = encoder_read();
	//int32_t PI_out = PI_controller(r,y);
	//printf("Reference: %d, encoder: %d, PI Output: %d\n\r",r,y,PI_out);
	
	int32_t PI_out = 0;
	uint16_t DAC_out = 0;
	//Changing direction
	if(PI_out>=0){ 
		clearBit(PIOD,mDIR);
		DAC_out = -PI_out;
	}
	else{ 
		setBit(PIOD,mDIR); 
		DAC_out = PI_out;
	}
	if(DAC_out>MOTOR_MAX){
		DAC_out = MOTOR_MAX;
	}
	//Writing output
	setBit(PIOD,mEN);
	DAC_set_output(DAC_out);
	printf("Slider %d, Reference: %d, encoder: %d, PI Output: %d, DAC Output: %d\n\r",controller.slider_2_val,r,y,PI_out,DAC_out);
	//printf("Val: %d\n\r", controller.slider_2_val);
}

