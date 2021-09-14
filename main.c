/*
 * TTK4155-Byggern-G17.c
 *
 * Created: 31/08/2021 14:03:31
 *
 */ 

#ifndef F_CPU // In case F_CPU not defined
#define F_CPU 4915200 // 4.9152 MHz
#endif

#define BAUD 9600

#include <avr/io.h>
#include "uart.h"
#include "adc.h"
#include "functions.h"
#include "sram.h"

void exercise1(void) {
	while(1) {
		unsigned char data = receiveByte(); //Recives the inputted characters
		transmitByte(data+5); //Transmits the inputted characters and moves it 5 characters down
	}
}

int exercise2(void){
	//setBit(MCUCR, SRE); //Enabeling external memory interface
	setBit(DDRE, PE1); //Enabling PE1 for output
	setBit(DDRA, PA0);
	setBit(DDRA, PA1);
	while(1){
		setBit(PORTE,PE1);
		toggleBit(PORTA, PA0);
		setBit(PORTA, PA1);
		unsigned char data = receiveByte(); //Recives the inputted characters
		transmitByte(data); //Transmits the inputted characters and moves it 5 characters down
	}
}

void exercise3(void){
	
}

int main(void) {
	initUart();
	initADC();
	SRAM_init();

	SRAM_test();


	//exercise1();
	//exercise2();
	//exercise3();
}



