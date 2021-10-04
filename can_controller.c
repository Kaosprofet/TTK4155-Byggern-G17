#ifndef INCLUDES_H
#include "includes.h"
#endif


void can_controller_read(void){
    spi_transfer(0b00000011); //Read data from register beginning at selected adress
    spi_transfer(address); //The adress from where to begin reading data
}

void can_controller_write(void){
    spi_transfer(0b00000010); //OP code for writing data to specified register
    spi_transfer(address); //The adress from where to begin writing data

}
void can_controller_request_to_send(void){

}
void can_controller_read_status(void){
    spi_transfer(0b10100000); //OP code for reading tx/rx register status
}
void can_controller_bit_modify(void){

}
void can_controller_reset(void){
    spi_transfer(0b11000000); //resets the controller
}