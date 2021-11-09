#define setBit(reg, bit) (reg->PIO_SODR = bit)
#define clearBit(reg, bit) (reg->PIO_CODR = bit)
#define bitIsSet(reg, bit) (reg->PIO_ODSR & bit)
#define readPin(reg,mask) (reg->PIO_PDSR & mask)

#define toggleBit(reg, bit) (bitIsSet(reg, bit) ? clearBit(reg, bit) : setBit(reg, bit))

#define waitUntilBitIsSet(reg, bit) while(!bitIsSet(reg,bit))
#define waitUntilBitIsClear(reg, bit) while(bitIsSet(reg,bit))

#define map(input, input_min, input_max, output_min, output_max) ((input-input_min)*(output_max-output_min)/(input_max-input_min)+output_min)

#define _delay_ms(time) (while(RTT->RTT_VR<RTT->RTT_VR+time*1000))
#define _delay_us(time) (while(RTT->RTT_VR<RTT->RTT_VR+time))