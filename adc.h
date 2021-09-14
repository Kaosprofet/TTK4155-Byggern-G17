#ifndef ADC_H_
#define ADC_H_


void initADC(void);
void selectADCChannel(uint8_t channel);
char ADCRead(void);

#endif