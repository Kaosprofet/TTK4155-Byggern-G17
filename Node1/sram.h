#ifndef SRAM_H
#define SRAM_H

#include <avr/io.h>
#include <stdlib.h>

void initSRAM(void);

uint8_t readSRAM(uint16_t address);

void writeSRAM(uint16_t address, char data);

#endif