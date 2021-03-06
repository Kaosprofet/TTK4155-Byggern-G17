#ifndef INCLUDES_H
#include "includes.h"
#endif

volatile uint8_t can_interrupt_flag = 0;

int CAN_interrupt() {
	if(can_interrupt_flag){
		can_interrupt_flag = 0;
		return 1;
	}
	return 0;
}

//-----------------------------------------------SENDING----------------------------------------------------------

// Sends the inputted message
void CAN_send_message(can_message* message) {
	static int can_buffer = 0;

	uint8_t id = message->ID;
	
	// ID high
	char id_high = id/8;	
	CAN_controller_write(TXB0SIDH + 0x10*can_buffer, id_high);
	// ID low
	char id_low = (id %8)<<5;
	CAN_controller_write(TXB0SIDL + 0x10*can_buffer, id_low);
	
	// Sending the length to the length buffer
	uint8_t L = message->length;
	CAN_controller_write(TXB0DLC + 0x10*can_buffer, L);
	
	// Sending the data
	uint8_t* data2send = message->data;
	for(int i = 0; i<L;i++) { CAN_controller_write(TXB0Dm + i + 0x10*can_buffer, data2send[i]);
	}
	// Request to send
	CAN_controller_request_to_send(can_buffer);
}

// Checks the interrupt flag of a buffer. Returns 1 of it is zero
int CAN_buffer_tx_clear(int can_buffer) {
	uint8_t interrupt_flags = CAN_controller_read(CANInterruptFlags); // Reads the interrupt flags
	uint8_t check_bit = can_buffer+2;
	if(!bitIsSet(interrupt_flags,check_bit)) { return 0;}
	return 1;
}
int CAN_buffer_rx0_clear(void) {
	uint8_t interrupt_flags = CAN_controller_read(CANInterruptFlags); // Reads the interrupt flags
	uint8_t check_bit = RX0IF;
	if(!bitIsSet(interrupt_flags,check_bit)) { return 0;}
	return 1;
}
//Checks for errorflags
int CAN_error_check(void) {
	uint8_t byte = CAN_controller_read(CANInterruptFlags);
	if((byte & ERRIF) == ERRIF) { // Checks if the the error bit/flag is high
		printf("(!) CAN: Error detected\n\r");
		return 1;
	}
	return 0;
}

void CAN_send_game_status(void) {
	can_message gameStatus;
	gameStatus.ID = CAN_ID_GameStatus;
	gameStatus.data[0] = game.game_status;
	gameStatus.data[1] = game.score;
	gameStatus.data[2] = game.mode;
	gameStatus.length = 3;
	printf("Sending game status\n\r");
	CAN_send_message(&gameStatus);
}

void CAN_send_music_status(uint8_t play, uint8_t melody) {
	can_message music_status;
	music_status.ID = CAN_ID_Music;
	music_status.data[0] = play;
	music_status.data[1] = melody;
	music_status.length = 2;
	printf("Sending music status\n\r");
	CAN_send_message(&music_status);
}

//----------------------------------RECEIVING---------------------------------------------

 void CAN_decode_message(void) {
	 if (can_interrupt_flag) {  // CAN_buffer_rx0_clear
		 can_message message = CAN_receive_message();
		 
		 if(message.ID == CAN_ID_GameStatus) {
			 game.game_status = message.data[0];
			 printf("node2 game mode: %d, ", game.game_status);
			 game.score = message.data[1];
			 game.mode = message.data[2];
		 }
		 printf("Message ID: %d, Length: %d, Data 1: %d, Data 2: %d, Data 3: %d\n\r", message.ID, message.length, message.data[0], message.data[1], message.data[2]);
		 _delay_ms(30);
		 can_interrupt_flag = 0;
	 }
 }
            
// Receives a message
can_message CAN_receive_message(void) {
	can_message B1_message;
	
	CAN_get_message(0, &B1_message);
	CAN_controller_write(CANInterruptFlags, 0x00);

	return B1_message;
}

// Get the message from a specified buffer
void CAN_get_message(uint8_t buffernumber, can_message* message) {
	// Getting the ID
	uint8_t id_high = CAN_controller_read(RXB0SIDH + 0x10*buffernumber); // Getting the lower part
	uint8_t id_low = CAN_controller_read(RXB0SIDL + 0x10*buffernumber); // Getting the higher part
	message->ID = (id_high<<3) + (id_low>>5);
	
	// Getting the length of data
	uint8_t l = CAN_controller_read(RXB0DLC + 0x10*buffernumber);
	message->length = l&0b00001111;
	
	// Message length protection
	if(l>8) {l=8;}
		
	// Getting the data
	for(uint8_t i=0;i<l;i++) {
		message->data[i] = CAN_controller_read(RXB0DM + 0x10*buffernumber + i);
	}
}

int CAN_buffer_rx_clear(int can_buffer) {
	uint8_t interrupt_flags = CAN_controller_read(CANInterruptFlags); //Reads the interrupt flags
	if(!bitIsSet(interrupt_flags,can_buffer)){ return 1;}
	return 0;
}

int checkBitmask(int byte,int mask) {
	if((byte & mask) == mask) {return 1;}
	return 0;
}

//----------------------------TESTING-----------------------------------

void CAN_test(void) {
	// initializes CAN i LOOPBACK-mode.
	CAN_controller_init(CAN_LOOPBACK); 
	
	// Generates test message
	can_message testmessage1;
	testmessage1.ID = 10;
	testmessage1.data[0] = 22;
	testmessage1.length = 1;
	printf("Test1 ID: %d, Test1 length: %d, Test1 data: %d \n\r",testmessage1.ID, testmessage1.length, testmessage1.data[0]);
	
	// Sends test message
	CAN_send_message(&testmessage1);
	
	// Generates reception message-struct
	can_message rmessage;
	rmessage.ID = 0;
	rmessage.data[0]=0;
	rmessage.length = 0;
	can_message* p = &rmessage;
	
	// Waits for an interrupt flag to be set
	while(1){
		if(CAN_controller_read(CANInterruptFlags)>0){
			CAN_get_message(0,p); // reads from buffer 0.
			printf("Rec ID: %d, Rec length: %d, Rec data: %d \n\r",rmessage.ID, rmessage.length, rmessage.data[0]);
			break;
		}
	}
}

void CAN_test_normal(){
	CAN_controller_init(CAN_NORMAL);
	can_message testmessage1;
	testmessage1.ID = 69;
	testmessage1.data[0] = 123;
	testmessage1.length = 1;
	printf("Test1 ID: %d, Test1 length: %d, Test1 data: %d \n\r",testmessage1.ID, testmessage1.length, testmessage1.data[0]);
	while(1){
		CAN_send_message(&testmessage1);
	}
}

// Reads from a specified TX buffer
void CAN_test_Transmission(uint8_t buffernumber, can_message* message) {
	message->length= CAN_controller_read(TXB0DLC+0x10*buffernumber);
	uint8_t id_low = CAN_controller_read(TXB0SIDL);
	uint8_t id_high = CAN_controller_read(TXB0SIDH);
	message->ID = (id_high<<3) +(id_low>>5);
	for(uint8_t i=0;i<message->length;i++){
		message->data[i] = CAN_controller_read(TXB0Dm + 0x10*buffernumber + i);
	}
}

void CAN_test_recieve() {
	initGame();
	while(1) {
		can_message message =  CAN_receive_message();
		printf("ID: %d, Length: %d, Data: %d\n\r", message.ID, message.length, message.data[0]);
		_delay_ms(50);
	}
}

ISR(INT0_vect) {
	can_interrupt_flag = 1;
}