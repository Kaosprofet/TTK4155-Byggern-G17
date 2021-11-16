#ifndef INCLUDES_H
#include "includes.h"
#endif

static uint8_t can_new_message_flag = 0;

void CAN_controller_init(uint8_t can_mode) {
	spi_init();             // Initiate spi
	can_controller_reset(); // Reset the can controller
	_delay_ms(10);
	
	can_set_mode(CAN_CONFIG);
	//Timing
	CAN_controller_write(CNF1,(1<<6)|(11<<0));
	CAN_controller_write(CNF2, (1<<7)|(1<<6)|(3<<3)|(3<<0));
	CAN_controller_write(CNF3,4<<0);
	
	CAN_controller_write(CANInterrruptEnable, 0x3); //Enables interrupt on all receive and transmit.
	CAN_controller_write(CANInterruptFlags, 0x00);
	
	//Disable masks/filters on RXB0 and RXB1
	CAN_controller_write(RXB0CTRL,RX_FilterOff);
	CAN_controller_write(RXB1CTRL,RX_FilterOff);
	//Restets the data length registers to make sure Remote Transmission is turned off
	CAN_controller_write(TXB0DLC,0);
	CAN_controller_write(TXB0DLC + 0x10,0);
	CAN_controller_write(TXB0DLC + 0x20,0);
	
	can_set_mode(can_mode);
	cli();  // Disable global interrupts
	clearBit(DDRD,PD2);
	// Interrupt on falling edge PD2
	setBit(MCUCR, ISC01);
	clearBit(MCUCR, ISC00);
	setBit(GICR, INT0); // Enable interrupt on PD2
	sei();              // Enable global interrupts
}

void can_set_mode(uint8_t can_mode) {
    clearBit(PORTB, PB4); //Lower chip-select
    CAN_controller_write(CAN_CTRL, can_mode);
    setBit(PORTB, PB4); //Raise chip-select
	uint8_t byte = CAN_controller_read(CAN_STAT);
	if((byte & can_mode)!=can_mode){ //Error if the mode was not set
		printf("CAN ERROR: Mode was not set");
	}
}

void can_controller_reset(void) {
    clearBit(PORTB, PB4);  //Lower chip-select
    spi_write(0b11000000); //Resets the controller
    setBit(PORTB, PB4);    //Raise chip-select
}

uint8_t setbitfunction(uint8_t byte, uint8_t bit){
	byte = byte + (1<<(bit+1)); 
}

void can_controller_bit_modify(uint8_t address, uint8_t mask, uint8_t data) {
    clearBit(PORTB, PB4);  //Lower chip-select
    spi_write(CAN_BITMOD); // Op code for modifying bit
    spi_write(address);    //Adress of register to modify
	spi_write(mask);
	spi_write(data);
    setBit(PORTB, PB4);    //Raise chip-select
}

uint8_t can_controller_read_status(void) {
    clearBit(PORTB, PB4);   //Lower chip-select
    spi_write(0b10100000);  //OP code for reading tx/rx register status
    uint8_t status = spi_read();
    setBit(PORTB, PB4);     //Lower chip-select
    return status;
}

uint8_t CAN_controller_read(uint8_t address) {
    clearBit(PORTB, PB4);       //Lower chip-select
    spi_write(CAN_READ);        //Read data from register beginning at selected address
    spi_write(address);         //The address from where to begin reading data
    uint8_t value = spi_read(); //Reads data
    setBit(PORTB, PB4);         //Raise chip-select
    return value;               //Return the SPDR register which contains data read
}

void CAN_controller_request_to_send(uint8_t address) {
    clearBit(PORTB, PB4);                   //Lower chip-select
    //spi_write(setbitfunction(0x80,address));//spi_write(address);
	spi_write(0x81);
    setBit(PORTB, PB4);                     //Lower chip-select
}

void CAN_controller_write(uint8_t address, uint8_t data) {
    clearBit(PORTB, PB4);   //Lower chip-select
    spi_write(CAN_WRITE);   //OP code for writing data to specified register
    spi_write(address);     //The address from where to begin writing data
    spi_write(data);        //The data to write data
    setBit(PORTB, PB4);     //Raise chip-select
}