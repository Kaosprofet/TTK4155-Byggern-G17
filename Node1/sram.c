#ifndef INCLUDES_H
#include "includes.h"
#endif

#define ext_ram_size 0x800
#define sram_adress 0x1800

volatile char *ext_ram = (char *) sram_adress; // Pointer to start address for SRAM

void initSRAM(void) {
	//Enabling external memory interface
	setBit(MCUCR, SRE);	
}

uint8_t readSRAM(uint16_t address) {  
	return ext_ram[address];
}

void writeSRAM(uint16_t address, char data) {
	ext_ram[address] = data;
}
