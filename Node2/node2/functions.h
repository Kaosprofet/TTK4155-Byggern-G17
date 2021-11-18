#ifndef FUNCTIONS_H
#define FUNCTIONS_H

// Bit manipulations (Only IO pins)
#define setBit(reg, bit) (reg->PIO_SODR = bit)
#define clearBit(reg, bit) (reg->PIO_CODR = bit)
#define toggleBit(reg, bit) (bitIsSet(reg, bit) ? clearBit(reg, bit) : setBit(reg, bit))

// Bit tests (Only IO pins)
#define bitIsSet(reg, bit) (reg->PIO_ODSR & bit)
#define readPin(reg,mask) (reg->PIO_PDSR & mask)

// Wait loops (Only IO pins)
#define waitUntilBitIsSet(reg, bit) while(!bitIsSet(reg,bit))
#define waitUntilBitIsClear(reg, bit) while(bitIsSet(reg,bit))

// Map function for interpolation
#define map(input, input_min, input_max, output_min, output_max) ((input-input_min)*(output_max-output_min)/(input_max-input_min)+output_min)

#endif