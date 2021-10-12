#ifndef INCLUDES_H
#include "includes.h"
#endif


void CAN_test(void){
	CAN_com_init(CAN_LOOPBACK);
	can_message testmessage;
	can_message rmessage;
	while(1){
		testmessage.ID = 69;
		testmessage.data[0] = 'h';
		testmessage.data[1] = 'e';
		testmessage.data[2] = 'i';
		testmessage.length = 3;
		
		CAN_sendmessage(&testmessage);
		rmessage = CAN_recieve_message();
		//printf("Data: &d \n\r",rmessage.data[0]);
	}
}


void CAN_com_init(uint8_t can_mode){
	can_controller_init(can_mode);
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
	can_controller_request_to_send(RTS_TX0 + can_buffer);	
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
	can_message B1_message;
	can_message B2_message;
	
	//Getting the interrupt flags
	uint8_t CANINTF = can_controller_read(CANInterruptFlags);

	//Checks if there is a message in buffer 0
	if(checkBitmask(CANINTF,RX0IF)){
		can_get_message(0, &B1_message);
		can_controller_bit_modify(CANInterruptFlags,RX0IF,0);
		return B1_message;
		
	}
	//Checks if there is a message in buffer 1
	if(checkBitmask(CANINTF,RX1IF)){
		can_get_message(1,&B2_message);
		can_controller_bit_modify(CANInterruptFlags,RX1IF,0);
		return B2_message;
	}
	return B1_message;
}

//Get the message from a specified buffer
void can_get_message(uint8_t buffernumber, can_message* message){
	//Getting the ID
	uint8_t id_high = can_controller_read(RXB0SIDH + 0x10*buffernumber); //getting the lower part
	uint8_t id_low = can_controller_read(RXB0SIDL + 0x10*buffernumber); //getting the higher part
	uint8_t mask = 0xE0;
	id_low = (id_low & mask);
	message->ID = 0x8*id_high + id_low/0x20;
	
	//Getting the length of data
	uint8_t l = can_controller_read(RXB0DLC + 0x10*buffernumber);
	message->length = (l & 0xF);
	
	//Getting the data
	for(uint8_t i=0;i<l;i++){
		message->data[i] = can_controller_read(RXB0DM + 0x10*buffernumber + i);
	}
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
