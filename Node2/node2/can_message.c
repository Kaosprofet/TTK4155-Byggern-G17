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

// Creates messages based on selected ID and sends over CAN
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
		message.id = status_id;
		message.data_length = 3;
		message.data[0] = game.game_status;
		message.data[1] = game.score;
		message.data[2] = game.mode;
		printf("Sending game status: %d, Message ID: %d, Length: %d, Data 1: %d, Data 2: %d\n\r", game.game_status, message.id, message.data_length, message.data[0], message.data[1]);
	}
	can_send(&message, 0);
}

// Fetches message from CAN controller and allocates values based on ID
void can_decode_message(void) {
	CAN0_Handler();

	//Controller data
	if (rx_message.id == controller_id) {  
		controller.x = rx_message.data[0];
		controller.y = rx_message.data[1];
		controller.button_state = rx_message.data[2];
		controller.slider_1_val = rx_message.data[3];
		controller.slider_2_val = rx_message.data[4];
	} //Game status
	else if (rx_message.id == status_id) {
		game.game_status = rx_message.data[0];
		game.score = rx_message.data[1];
		game.mode = rx_message.data[2];
		printf("Got game status: %d\n\r",game.game_status);
	} //Music status
	else if(rx_message.id == music_id) {
		music.play = rx_message.data[0];
		music.melody = rx_message.data[1];
	}
}