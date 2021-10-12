#define setBit(reg, bit) (reg->PIO_SODR = bit)
#define clearBit(reg, bit) (reg->PIO_CODR = bit)
#define bitIsSet(reg, bit) (reg->PIO_ODSR & bit)

#define toggleBit(reg, bit) (bitIsSet(reg, bit) ? LEDOFF : LEDON)

#define waitUntilBitIsSet(reg, bit) while(!bitIsSet(reg,bit))
#define waitUntilBitIsClear(reg, bit) while(bitIsSet(reg,bit))