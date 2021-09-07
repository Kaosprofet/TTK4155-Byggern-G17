#include <avr/io.h>

#define setBit(reg, bit) (reg |= (1<<bit))
#define clearBit(reg, bit) (reg &= ~(1<<bit))
#define toggleBit(reg, bit) (reg ^= (1<<bit)) 

#define bitIsSet(reg, bit) (reg & (1<<bit))

#define waitUntilBitIsSet(reg, bit) while(!bitIsSet(reg,bit))
#define waitUntilBitIsClear(reg, bit) while(bitIsSet(reg,bit))