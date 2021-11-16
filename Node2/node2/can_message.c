#ifndef INCLUDES_H
#include "includes.h"
#endif

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
		message.id = controller_id;
		message.data_length = 5;
		message.data[0] = controller.x;
		message.data[1] = controller.y;
		message.data[2] = controller.button_state;
		message.data[3] = controller.slider_1_val;
		message.data[4] = controller.slider_2_val;
	}
	else if (ID == status_id) {
		message.id = controller_id;
		message.data_length = 2;
		message.data[0] = game.game_status;
		message.data[1] = game.score;
		printf("Sending game status: %d, Message ID: %d, Length: %d, Data 1: %d, Data 2: %d\n\r", game.game_status, message.id, message.data_length, message.data[0], message.data[1]);
	}
	can_send(&message, 0);
}

void can_decode_message(void) {
	CAN_MESSAGE message = CAN0_Handler(void);

	//Controller data
	if (message.id == controller_id) {  
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