#ifndef INCLUDES_H
#include "includes.h"
#endif

uint8_t can_controller_read(uint8_t address)
{
    clearBit(PORTB, PB4);       //Lower chip-select
    spi_write(0b00000011);      //Read data from register beginning at selected address
    spi_write(address);         //The address from where to begin reading data
    uint8_t value = spi_read(); //Reads data
    setBit(PORTB, PB4);         //Raise chip-select
    return value;               //Return the SPDR register which contains data read
}

void can_controller_write(uint8_t address, uint8_t data)
{
    clearBit(PORTB, PB4);  //Lower chip-select
    spi_write(0b00000010); //OP code for writing data to specified register
    spi_write(address);    //The address from where to begin writing data
    spi_write(data);       //The data to write data
    setBit(PORTB, PB4);    //Raise chip-select
}
void can_controller_request_to_send(uint8_t address)
{
    clearBit(PORTB, PB4); //Lower chip-select
    spi_write(setbitfunction(0x80,address));
    //spi_write(address);
    setBit(PORTB, PB4); //Lower chip-select
}
uint8_t can_controller_read_status(void)
{
    clearBit(PORTB, PB4);  //Lower chip-select
    spi_write(0b10100000); //OP code for reading tx/rx register status
    uint8_t status = spi_read();
    setBit(PORTB, PB4); //Lower chip-select
    return status;
}
void can_controller_bit_modify(uint8_t address, uint8_t mask, uint8_t data)
{
    clearBit(PORTB, PB4);  //Lower chip-select
    spi_write(0b00000101); // Op code for modifying bit
    spi_write(address);    //Adress of register to modify
	spi_write(mask);
	spi_write(data);
    setBit(PORTB, PB4);    //Raise chip-select
}
void can_controller_reset(void)
{
    clearBit(PORTB, PB4);  //Lower chip-select
    spi_write(0b11000000); //Resets the controller
    setBit(PORTB, PB4);    //Raise chip-select
}

void can_set_mode(uint8_t can_mode)
{
    clearBit(PORTB, PB4); //Lower chip-select
    can_controller_write(CAN_CTRL, can_mode);
    setBit(PORTB, PB4); //Raise chip-select
}

void can_controller_init(uint8_t can_mode)
{
    spi_init();             // Initiate spi
    can_controller_reset(); // Reset the cancontroller
    _delay_ms(10);
	can_set_mode(can_mode);
    // Disable global interrupts
    cli();
    // Interrupt on falling edge PD2
    setBit(MCUCR, ISC01);
    clearBit(MCUCR, ISC00);
    // Enable interrupt on PD2
    setBit(GICR, INT0);
    // Enable global interrupts
    sei();
}

uint8_t setbitfunction(uint8_t byte, uint8_t bit){
	uint8_t bits[8]={1,2,4,8,16,32,64,128};
	byte += bits[bit]; 
}