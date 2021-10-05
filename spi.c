#ifndef INCLUDES_H
#include "includes.h"
#endif

void spi_init(void)
{
    setBit(MCUCR, SRE);  //Enable peripherial pinmode
    setBit(DDRB, PB5);   //Enable MOSI as output
    setBit(DDRB, PB7);   //Enable SCK as output
    setBit(DDRB, PB4);   //Enable SS as output
    clearBit(DDRB, PB6); //Enable MISO as input
    setBit(PORTB, PB4);  //Sets the SS pin to high
    setBit(SPCR, SPE);   //Enable SPI
    setBit(SPCR, MSTR);  //Enable master mode
    setBit(SPCR, SPR0);  //Sets the SCK frequency to Fclk/16
    clearBit(SPSR, SPIF);
    clearBit(SPCR, SPR1);
    clearBit(SPSR, SPI2X); //Sets SPI transfer speed to normal
}

void spi_transfer(unsigned char Data)
{
    SPDR = Data; //Writes data for the SPI to transfer

    waitUntilBitIsSet(SPSR, SPIF);
}
