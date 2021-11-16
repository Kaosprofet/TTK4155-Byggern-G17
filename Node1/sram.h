#include <avr/io.h>
#include <stdlib.h>
#include "functions.h"

//void enableEMI(void);

void initSRAM(void);

uint8_t readSRAM(uint16_t address);
void writeSRAM(uint16_t address, char data);

	
void SRAM_test(void);
