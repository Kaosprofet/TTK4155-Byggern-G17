#ifndef ADC_H_
#define ADC_H_

#include <avr/io.h>

void initADC(void);
void selectADCChannel(uint8_t channel);
uint8_t * readADC(void);
void adcTest(void);

#endif