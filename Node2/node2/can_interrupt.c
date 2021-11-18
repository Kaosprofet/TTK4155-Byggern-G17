/*
 * interrupt.c
 *
 * Author: Gustav O. Often and Eivind H. J�lsgard
 *
 * For use in TTK4155 Embedded and Industrial Computer Systems Design
 * NTNU - Norwegian University of Science and Technology
 *
 */ 

#ifndef INCLUDES_H
#include "includes.h"
#endif

#define DEBUG_INTERRUPT 0

/**
 * \brief CAN0 Interrupt handler for RX, TX and bus error interrupts
 *
 * \param void
 *
 * \retval 
 */



void CAN0_Handler(void) {
	//if(DEBUG_INTERRUPT)printf("CAN0 interrupt\n\r");
	if(DEBUG_INTERRUPT)printf("CAN0 interrupt\n\r");
	char can_sr = CAN0->CAN_SR; 
	//RX interrupt
	//Only mailbox 1 and 2 specified for receiving
	if(can_sr & (CAN_SR_MB1 | CAN_SR_MB2) ) {
		CAN_MESSAGE message;
		if(can_sr & CAN_SR_MB1)	{ //Mailbox 1 event
			can_receive(&message, 1);

		}
		else if(can_sr & CAN_SR_MB2) { //Mailbox 2 event
			can_receive(&message, 2);
		}
		else {
			printf("CAN0 message arrived in non-used mailbox\n\r");
		}

		if(DEBUG_INTERRUPT)printf("message id: %d\n\r", message.id);
		if(DEBUG_INTERRUPT)printf("message data length: %d\n\r", message.data_length);
		for (int i = 0; i < message.data_length; i++) {
			if(DEBUG_INTERRUPT)printf("%d ", message.data[i]);
		}
		if(DEBUG_INTERRUPT)printf("\n\r");
		
		//Controller data
		if(message.id == controller_id) {
			controller.x = message.data[0];
			controller.y = message.data[1];
			controller.button_state = message.data[2];
			controller.slider_1_val = message.data[3];
			controller.slider_2_val = message.data[4];
			//printf("Joystick: X = %4d Y = %4d Sliders: 1 = %3d 2 = %3d  Buttons: 1 = %d\n\r", controller.x, controller.y, controller.slider_1_val, controller.slider_2_val,  controller.button_state);
		}
		else if (message.id == status_id) {
			game.game_status = message.data[0];
			game.score = message.data[1];
			printf("Got game status: %d\n\r",game.game_status);
		}
		
		
	}
	
	if(can_sr & CAN_SR_MB0) {
		//if(DEBUG_INTERRUPT) printf("CAN0 MB0 ready to send \n\r");
		
	//Disable interrupt
		CAN0->CAN_IDR = CAN_IER_MB0;
	}

	if(can_sr & CAN_SR_ERRP) {
		//if(DEBUG_INTERRUPT)printf("CAN0 ERRP error\n\r");
	}
	
	if(can_sr & CAN_SR_TOVF) {
		if(DEBUG_INTERRUPT)printf("CAN0 timer overflow\n\r");
	}
	
	NVIC_ClearPendingIRQ(ID_CAN0);
	//sei();*/
	
}