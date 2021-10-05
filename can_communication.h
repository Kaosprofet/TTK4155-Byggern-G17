#ifndef CAN_COMMUNICATIONS_H
#define CAN_COMMUNICATIONS_H

//--------------------- MCP2515 ADDRESSES -------------
#define CAN_CTRL 0x0F //Controll register. The following are the different modes sent there. 
#define CAN_NORMAL	0b00000000
#define CAN_SLEEP 0b00100000
#define CAN_LOOPBACK 0b01000000
#define CAN_LISTEN 0b01100000
#define CAN_CONFIG 0b10000000

#define CAN_STAT 0x0E //Status register, read to return the status of the MCP

//interrupt enable
#define CANInterrruptEnable 0x2B
	#define MERRE 0b10000000 //Interrupt on error
	#define WAKIE 0b01000000 //Interrupt on CAN bus activity
	#define TX2IE 0b00010000 // TXB2 becoming empty
	#define TX1IE 0b00001000 // TXB1 becoming empty
	#define TX0IE 0b00000100 // TXB0 becoming empty
	#define RX1IE 0b00000010 // Interrupt when message received in RXB1
	#define RX0IE 0b00000001 // Interrupt when message received in RXB0

//Interrupt flag
#define CANInterruptFlags 0x2C

//Transmit buffers
#define TXB0CTRL 0x30
#define TXB1CTRL 0x40
#define TXB2CTRL 0x50
	//... sub bits
	#define ABTF 0b01000000 // (R) High if message was aborted
	#define MLOA 0b00100000 // (R) High if message lost arbitration while being sent
	#define TXERR 0b00010000 // (R) High if transmission error is detected
	#define TXREQ 0b00001000 // (R/W) High if the buffer is pending a transmission
	//Message priority(R/W):
	#define TXP_HIGH 0b00000011 //High 
	#define TXP_HIGHInter 0b00000010 //High-intermediate
	#define TXP_LOWInter 0b00000001 //Low intermediate
	#define TXP_LOW 0b00000000 //Low

//Pin control and status registers
#define TXRTSCTRL 0x0D
	//0 when in ready to send mode 
	#define B2RTS 0b00100000	
	#define B1RTS 0b00010000	 
	#define B0RTS 0b00001000	
	//Used to request message transmission
	#define B2RTSM 0b00000100
	#define B1RTSM 0b00000010
	#define B1RTSM 0b00000001

//Transmit Identifier high
#define TXB0SIDH 0x31
	
//Transmit identifier low
#define TXB0SIDL 0x32  	  	  	  	  		

//Transmit data length
#define TXB0DLC 0x35

//Transmit data buffer
#define TXB0Dm 0x36

//Receive buffers
#define RXB0CTRL 0x60

//Receive buffer identifiers
#define RXB0SIDH 0x61 //high
#define RXB0SIDL 0x62 //low

//Receive buffer data length
#define RXB0DLC 0x65

//Receive buffer data byte
#define RXB0DM 0x66


//----------------------OTHER Definitions and functions ------------
typedef struct{uint8_t ID; uint8_t length; uint8_t data[8];} can_message;
	
void CAN_controller_init(uint8_t can_mode); // Initializes using loop-back mode**
void CAN_sendmessage(can_message* message); // sends data**
uint8_t CAN_recieve_message(void); //receives data **


#endif