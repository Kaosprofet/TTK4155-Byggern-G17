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
#include <stdio.h>
#include <stdbool.h>

#include "joystick.h"
#include "uart.h"
#include "adc.h"
#include "functions.h"
#include "sram.h"
#include "pwm.h"
#include "game.h"

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
	
	volatile struct joysticks joystick;	
	joystick.offset = 25;
	volatile struct controllers controller;
	initButton();
	while(1) {
		 joystick = fetchJoystick();
		 controller = fetchController();
		 
		 bool button1 = bitIsSet(PIND, PD3);
		 bool button2 = bitIsSet(PIND, PD4);
		 
		 
		 printf("Joystick: X = %4d Y = %4d dir = %d  Sliders: 1 = %3d 2 = %3d  Buttons: 1 = %d 2 = %d\n\r", joystick.x_val, joystick.y_val, joystick.dir, controller.slider1_val, controller.slider2_val, button1, button2);
		 //adcTest();
	}
	
	
}

void test(void){
	setBit(DDRC, PC3);
	//setBit(MCUCR, SRE); //Enabeling external memory interface
	setBit(DDRC, PC2); //Enabling PE1 for output
	setBit(DDRC, PC0);
	setBit(DDRC, PC1);
	while(1){
		clearBit(PORTC,PC3);
		setBit(PORTC, PC2);
	}
}

int main(void) {
	initUart();
	enableEMI();
	initADC();

	
	//SRAM_init();

	//SRAM_test();
	//Chip_select_test();


	//exercise1();
	//exercise2();
	exercise3();
}



