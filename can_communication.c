#ifndef INCLUDES_H
#include "includes.h"
#include "can_communication.h"
#endif


void CAN_test(void){
	CAN_controller_init(CAN_LOOPBACK);
}


void CAN_controller_init(uint8_t can_mode){
	write_CANCTRL(can_mode); //Setting the CAN controller to the specified mode
	can_controller_write(CANInterrruptEnable, (RX0IE + RX1IE + TX0IE + TX1IE + TX2IE)); //Enables interrupt on all receive and transmit buffers. 
}

//Sends the selected mode to CANCTRL register (Manual page 60)
void write_CANCTRL(uint8_t can_mode){ can_controller_write(CAN_CTRL, can_mode); }

//Reads the status from the controller
uint8_t CAN_STATUS(void){ return can_controller_read(CAN_STAT);}


//-----------------------------------------------SENDING----------------------------------------------------------
//Sends the inputted message
void CAN_sendmessage(can_message* message){
	static int can_buffer = 0;
	
	//Looping until we find a clear buffer
	while(CAN_buffer_tx_clear(can_buffer)){
		can_buffer += 1;
		if(can_buffer>2){can_buffer=0;}
	}
	
	//Sending ID to the identifier buffer
	uint8_t id = message->ID;
		//ID high	
		uint8_t ID_high = id/8;
		can_controller_write(TXB0SIDH + 0x10*can_buffer, ID_high);
		//ID low
		uint8_t ID_low = id%8;
		can_controller_write(TXB0SIDL + 0x10*can_buffer, ID_low);
	
	//Sending the length to the length buffer
	uint8_t L = message->length;
		can_controller_write(TXB0DLC + 0x10*can_buffer, L);
	
	//Sending the data
	uint8_t* data2send = message->data;
	for(int i = 0; i<L;i++){
		can_controller_write(TXB0Dm + i + 0x10*can_buffer, data2send[i]);
	}
	//Request to send
	can_controller_request_to_send();	
}

//Checks the interrupt flag of a buffer. Returns 1 of it is zero
int CAN_buffer_tx_clear(int can_buffer){
	uint8_t interrupt_flags = can_controller_read(CANInterruptFlags); //Reads the interrupt flags
	uint8_t check_bit = can_buffer+2;
	if(!bitIsSet(interrupt_flags,check_bit)){ return 1;}
	else{ return 0;}
}



//checks for errorflags
int CAN_error_check(void){
	uint8_t byte = can_controller_read(CANInterruptFlags);
	if(byte & ERRIF == ERRIF){ //Checks if the the error bit/flag is high
		printf("(!) CAN: Error detected\n\l");
		return 1;
	}
	else {return 0;}
}

//----------------------------------RECEIVING---------------------------------------------

//Receives a message
can_message CAN_recieve_message(){
	uint8_t CANINTE = can_controller_read(CANInterrruptEnable);
	can_message message;
	//Checks if there is a message in buffer 0
	if(checkBitmask(CANINTE,RX0IE)){
		uint8_t ID0 = can_get_ID(0);
		uint8_t LO = can_get_length(0);
	}
	//Checks if there is a message in buffer 1
	if(checkBitmask(CANINTE,RX1IE)){
		uint8_t ID1 = can_get_ID(1);
		uint8_t L1 = can_get_length(1);
	}
	
	return message;
}

//get ID
int can_get_ID(int buffernumber){
	uint8_t id_high = can_controller_read(RXB0SIDH + 0x10*buffernumber); //getting the lower part
	uint8_t id_low = can_controller_read(RXB0SIDL + 0x10*buffernumber); //getting the higher part
	uint8_t mask = 0xE0;
	id_low = (id_low & mask);
	return 0x8*id_high + id_low/0x20; //returning the ID
}

int can_get_length(int buffernumber){
	uint8_t length = can_controller_read(RXB0DLC + 0x10*buffernumber);
	return (length & 0xF);
}

int CAN_buffer_rx_clear(int can_buffer){
	uint8_t interrupt_flags = can_controller_read(CANInterruptFlags); //Reads the interrupt flags
	if(!bitIsSet(interrupt_flags,can_buffer)){ return 1;}
	else{ return 0;}
}

int checkBitmask(int byte,int mask){
	if(byte & mask == mask){
		return 1;
	}
	else {return 0;}
}
