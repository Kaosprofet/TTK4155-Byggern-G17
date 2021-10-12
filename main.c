/*
 * TTK4155-Byggern-G17.c
 *
 * Created: 31/08/2021 14:03:31
 *
 */

#ifndef INCLUDES_H
#include "includes.h"
#endif

void exercise1(void)
{
	while (1)
	{
		unsigned char data = receiveByte(); //Recives the inputted characters
		transmitByte(data + 5);				//Transmits the inputted characters and moves it 5 characters down
	}
}

int exercise2(void)
{
	//enableEMI(); //Enabeling external memory interface
	setBit(DDRE, PE1); //Enabling PE1 for output
	setBit(DDRA, PA0);
	setBit(DDRA, PA1);
	while (1)
	{
		setBit(PORTE, PE1);
		toggleBit(PORTA, PA0);
		setBit(PORTA, PA1);
		unsigned char data = receiveByte(); //Recives the inputted characters
		transmitByte(data);					//Transmits the inputted characters and moves it 5 characters down
	}
}

void exercise3(void)
{
	playGame();
}
void exercise4(void)
{
	oled_test();
}

void testChipSelect(void)
{
	//Disables EMI
	clearBit(MCUCR, SRE);
	setBit(DDRA, PA7);
	setBit(DDRA, PA6);
	setBit(DDRA, PA5);
	setBit(DDRA, PA4);
	setBit(DDRA, PA3);
	setBit(DDRA, PA2);
	setBit(DDRA, PA1);
	setBit(DDRA, PA0);
	setBit(DDRC, PC3);
	setBit(DDRC, PC2); //Enabling PE1 for output
	setBit(DDRC, PC1);
	setBit(DDRC, PC0);
	while (1)
	{
		setBit(PORTA, PA7);
		setBit(PORTA, PA6);
		setBit(PORTA, PA5);
		setBit(PORTA, PA4);
		setBit(PORTA, PA3);
		setBit(PORTA, PA2);
		setBit(PORTA, PA1);
		setBit(PORTA, PA0);
		setBit(PORTC, PC3);
		setBit(PORTC, PC2);
		setBit(PORTC, PC1);
		setBit(PORTC, PC0);
	}
}

void spi_test(void)
{
	spi_init();
	while (1)
	{
		spi_transfer(0b00000000); //command
		spi_transfer(0b01111110); //data
	}
}

void can_controller_test(void){
	can_controller_init(CAN_LOOPBACK);
	while (1){
		can_controller_write(0b00001111, 0b11111111);
		uint8_t data = can_controller_read(0b00001111);
		printf("%d\n\r",data);
	}
}

int main(void)
{
	initGame();
	can_controller_test();
	//exercise3();
}
