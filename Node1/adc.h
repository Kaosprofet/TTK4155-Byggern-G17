#ifndef ADC_H_
#define ADC_H_

#include <avr/io.h>

void initADC(void);
uint8_t * readADC(void);

#endif