/*
 * TTK4155-Byggern-G17.c
 *
 * Created: 31/08/2021 14:03:31
 *
 */

#ifndef INCLUDES_H
#include "includes.h"
#endif

void testChipSelect(void);
void can_controller_test(void);
void testCanJoystick(void);
void interuptTest(void);

int main(void) {
	playGame();
}


//--------------------------Test functions--------------------------
// Manually sets address pins to test address decoder
void testChipSelect(void) {
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
	
	while (1) {
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

// Sends data to can bus and reads it back with loopback
void can_controller_test(void) {
	CAN_controller_init(CAN_LOOPBACK);
	while (1) {
		CAN_controller_write(0b00001111, 0b11111111);
		uint8_t data = CAN_controller_read(0b00001111);
		printf("%d\n\r",data);
	}
}

// Sends controller values over CAN to node 2
void testCanJoystick(void) {
	initGame();
	calibrateJoystick();
	while(1){
		updateController();
		printController();
		CAN_send_inputData();
		_delay_ms(50);
	}
}

// Test for CAN bus interupts 
void interuptTest(void) {
	initGame();
	while(1) {
		if (can_interrupt_flag) {
			can_message message = CAN_receive_message();
			printf("Message ID: %d, Length %d, Data, %d\n\r", message.ID, message.length, message.data[0]);
			
			can_message TXmessage;
			TXmessage.ID = message.data[0];
			TXmessage.length = 1;
			TXmessage.data[0] = 69;
			CAN_send_message(&TXmessage);

			can_interrupt_flag = 0;
		}
	}
}