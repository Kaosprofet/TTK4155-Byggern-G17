#ifndef INCLUDES_H
#include "includes.h"
#endif

void can_controller_read(uint8_t address)
{
    clearBit(PORTB, PB4);             //Lower chip-select
    spi_transfer_command(0b00000011); //Read data from register beginning at selected adress
    spi_transfer_data(address);       //The adress from where to begin reading data
    setBit(PORTB, PB4);               //Raise chip-select
}

void can_controller_write(uint8_t address)
{
    clearBit(PORTB, PB4);             //Lower chip-select
    spi_transfer_command(0b00000010); //OP code for writing data to specified register
    spi_transfer_data(address);       //The adress from where to begin writing data
    setBit(PORTB, PB4);                //Raise chip-select
}
void can_controller_request_to_send_TXB0(uint8_t address)
{
    spi_transfer_command(0b10000001);
    spi_transfer_data(address);
}
void can_controller_read_status(uint8_t address)
{
    spi_transfer_command(0b10100000); //OP code for reading tx/rx register status
    spi_transfer_data(address);
}
void can_controller_bit_modify(uint8_t address)
{
    spi_transfer_command(0b00000101);
    spi_transfer_data(address);
}
void can_controller_reset(void)
{
    spi_transfer(0b11000000); //resets the controller
}