#include <avr/io.h>
#include <stdlib.h>
#include "functions.h"

//void enableEMI(void);

void initSRAM(void);

uint8_t readSRAM(uint8_t address);
void writeSRAM(uint8_t address, uint8_t data);

	
void SRAM_test(void);

void Chip_select_test(void);