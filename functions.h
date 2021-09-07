#include <avr/io.h>

#define bitIsSet(reg, bit) (reg & (1<<bit))
#define waitBitIsSet(reg, bit) while(!bitIsSet(reg,bit))
#define waitBitIsClear(reg, bit) while(bitIsSet(reg,bit))
