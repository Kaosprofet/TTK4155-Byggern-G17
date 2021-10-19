#ifndef INCLUDES_H
#include "includes.h"
#endif

volatile uint8_t can_interrupt_flag = 0;

int can_interrupt() {
	if(can_interrupt_flag){
		can_interrupt_flag = 0;
		return 1;
	}
	else{return 0;}
}

void CAN_test(void) {
	can_controller_init(CAN_LOOPBACK); //initializes CAN i LOOPBACK-mode. 
	
	//Generates test message
	can_message testmessage1;
	testmessage1.ID = 69;
	testmessage1.data[0] = 42;
	testmessage1.length = 1;
	printf("Test1 ID: %d, Test1 length: %d, Test1 data: %d \n\r",testmessage1.ID, testmessage1.length, testmessage1.data[0]);
	
	//Sends test message
	CAN_sendmessage(&testmessage1);
	
	//Generates reception message-struct
	can_message rmessage;
	rmessage.ID = 0;
	rmessage.data[0]=0;
	rmessage.length = 0;
	can_message* p = &rmessage;
	//Waites for an interrupt flag to be set
	while(1){
		if(can_controller_read(CANInterruptFlags)>0){
			can_get_message(0,p); //reads from buffer 0.
			printf("Rec ID: %d, Rec length: %d, Rec data: %d \n\r",rmessage.ID, rmessage.length, rmessage.data[0]);
			break;
		}
	}
}

void CAN_test_normal(){
	can_controller_init(CAN_NORMAL);
	can_message testmessage1;
	testmessage1.ID = 1;
	testmessage1.data[0] = 232;
	testmessage1.length = 1;
	printf("Test1 ID: %d, Test1 length: %d, Test1 data: %d \n\r",testmessage1.ID, testmessage1.length, testmessage1.data[0]);
	CAN_sendmessage(&testmessage1);	
	uint8_t byte = can_controller_read(CANInterruptFlags);
	printf("CANINTF: %d\n\r",byte);
	byte = can_controller_read(TXB0CTRL);
	printf("TXBOCTRL: %d\n\r",byte);
}

//Reads from a specified TX buffer
void CAN_test_Transmission(uint8_t buffernumber, can_message* message) {
	message->length= can_controller_read(TXB0DLC+0x10*buffernumber);
	uint8_t id_low = can_controller_read(TXB0SIDL);
	uint8_t id_high = can_controller_read(TXB0SIDH);
	message->ID = (id_high<<3) +(id_low>>5);
	for(uint8_t i=0;i<message->length;i++){
		message->data[i] = can_controller_read(TXB0Dm + 0x10*buffernumber + i);
	}
}

//Sends the selected mode to CANCTRL register (Manual page 60)
void write_CANCTRL(uint8_t can_mode) { can_controller_write(CAN_CTRL, can_mode); }

//Reads the status from the controller
uint8_t CAN_STATUS(void) { return can_controller_read(CAN_STAT);}


//-----------------------------------------------SENDING----------------------------------------------------------
//Sends the inputted message
void CAN_sendmessage(can_message* message) {
	static int can_buffer = 0;
	//Looping until we find a clear buffer
	while(CAN_buffer_tx_clear(can_buffer)) {
		can_buffer += 1;
		if(can_buffer>2){can_buffer=0;}
	}

	uint8_t id = message->ID;
	
	//ID high
	char id_high = id/8;	
	can_controller_write(TXB0SIDH + 0x10*can_buffer, id_high);
	//ID low
	char id_low = (id %8)<<5;
	can_controller_write(TXB0SIDL + 0x10*can_buffer, id_low);
	
	//Sending the length to the length buffer
	uint8_t L = message->length;
	can_controller_write(TXB0DLC + 0x10*can_buffer, L);
	
	//Sending the data
	uint8_t* data2send = message->data;
	for(int i = 0; i<L;i++) {
		can_controller_write(TXB0Dm + i + 0x10*can_buffer, data2send[i]);
	}
	
	//Request to send
	can_controller_request_to_send(can_buffer);
}

//Checks the interrupt flag of a buffer. Returns 1 of it is zero
int CAN_buffer_tx_clear(int can_buffer) {
	uint8_t interrupt_flags = can_controller_read(CANInterruptFlags); //Reads the interrupt flags
	uint8_t check_bit = can_buffer+2;
	if(!bitIsSet(interrupt_flags,check_bit)) { return 0;}
	else{ return 1;}
}

//checks for errorflags
int CAN_error_check(void) {
	uint8_t byte = can_controller_read(CANInterruptFlags);
	if((byte & ERRIF) == ERRIF) { //Checks if the the error bit/flag is high
		printf("(!) CAN: Error detected\n\r");
		return 1;
	}
	else {return 0;}
}

//----------------------------------RECEIVING---------------------------------------------

//Receives a message
can_message CAN_recieve_message() {
	can_message B1_message;
	can_message B2_message;
	
	//Getting the interrupt flags
	uint8_t CANINTF = can_controller_read(CANInterruptFlags);

	//Checks if there is a message in buffer 0
	if(checkBitmask(CANINTF,RX0IF)) {
		can_get_message(0, &B1_message);
		can_controller_bit_modify(CANInterruptFlags,RX0IF,0);
		return B1_message;
	}
	
	//Checks if there is a message in buffer 1
	if(checkBitmask(CANINTF,RX1IF)) {
		can_get_message(1,&B2_message);
		can_controller_bit_modify(CANInterruptFlags,RX1IF,0);
		return B2_message;
	}
	return B1_message;
}

//Get the message from a specified buffer
void can_get_message(uint8_t buffernumber, can_message* message) {
	//Getting the ID
	uint8_t id_high = can_controller_read(RXB0SIDH + 0x10*buffernumber); //getting the lower part
	uint8_t id_low = can_controller_read(RXB0SIDL + 0x10*buffernumber); //getting the higher part
	message->ID = (id_high<<3) + (id_low>>5);
	
	//Getting the length of data
	uint8_t l = can_controller_read(RXB0DLC + 0x10*buffernumber);
	message->length = l&0b00001111;
	
	if(l>8) {l=8;}
	//Getting the data
	for(uint8_t i=0;i<l;i++) {
		message->data[i] = can_controller_read(RXB0DM + 0x10*buffernumber + i);
	}
}

int CAN_buffer_rx_clear(int can_buffer) {
	uint8_t interrupt_flags = can_controller_read(CANInterruptFlags); //Reads the interrupt flags
	if(!bitIsSet(interrupt_flags,can_buffer)){ return 1;}
	else{ return 0;}
}

int checkBitmask(int byte,int mask) {
	if((byte & mask) == mask) {
		return 1;
	}
	else {return 0;}
}

ISR(INT0_vect) {
	can_interrupt_flag = 1;
}