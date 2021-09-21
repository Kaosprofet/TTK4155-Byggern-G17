/*
 * TTK4155-Byggern-G17.c
 *
 * Created: 31/08/2021 14:03:31
 *
 */ 

#ifndef INCLUDES_H
#include "includes.h"
#endif

void exercise1(void) {
	while(1) {
		unsigned char data = receiveByte(); //Recives the inputted characters
		transmitByte(data+5); //Transmits the inputted characters and moves it 5 characters down
	}
}

int exercise2(void){
	//enableEMI(); //Enabeling external memory interface
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
	playGame();
	
}

void testChipSelect(void){
	//Disables EMI
	clearBit(MCUCR, SRE);
	setBit(DDRC, PC3);
	//setBit(MCUCR, SRE); //Enabeling external memory interface
	setBit(DDRC, PC2); //Enabling PE1 for output
	setBit(DDRC, PC0);
	setBit(DDRC, PC1);
	while(1){
		clearBit(PORTC,PC3);
		clearBit(PORTC, PC2);
	}
}

int main(void) {
	initUart();
	initADC();
	initSRAM();

	//testChipSelect();
	//SRAM_test();
	//Chip_select_test();


	//exercise1();
	//exercise2();
	exercise3();
}



