#ifndef INCLUDES_H
#include "includes.h"
#endif

coords joystick;
uint8_t button_state;
uint8_t slider_1_val;
uint8_t slider_2_val;

void init_can() {
	uint32_t br;
	br = 125 << CAN_BR_BRP_Pos;
	br |= 1 << CAN_BR_SJW_Pos;
	br |= 3 << CAN_BR_PHASE1_Pos;
	br |= 3 << CAN_BR_PROPAG_Pos;
	br |= 4 << CAN_BR_PHASE2_Pos;
	uint8_t test = can_init_def_tx_rx_mb(br);
	
	if(test) {
		printf("CAN initialization failed\n\r");
	}
	else {
		printf("CAN initialized\n\r");
	}
}

void can_encode_message(uint8_t ID) {
	CAN_MESSAGE message;
	if (ID == controller_id) {
		message.id = 20;
		message.data_length = 5;
		message.data[0] = joystick.x;
		message.data[1] = joystick.y;
		message.data[2] = button_state;
		message.data[3] = slider_1_val;
		message.data[4] = slider_2_val;
	}
	else if (ID == status_id) {
		
	}
	can_send(&message, 0);
}

void can_decode_message(CAN_MESSAGE *message) {
	//Controller data
	if (message->id == controller_id) {  
		joystick.x = (uint8_t)message->data[0];
		joystick.y = (uint8_t)message->data[1];
		button_state = message->data[2];
		slider_1_val = message->data[3];
		slider_2_val = message->data[4];
		printf("Joystick: X = %4d Y = %4d Sliders: 1 = %3d 2 = %3d  Buttons: 1 = %d\n\r", joystick.x, joystick.y, slider_1_val, slider_2_val,  button_state);
	}
	else if (message->id == status_id) {
		
	}
}