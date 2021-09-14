#ifndef ADC_H_
#define ADC_H_

typedef enum {channel1, channel2, channel3, channel4} channel_t;

void initADC(void);
void selectADCChannel(channel_t channel);
char ADCRead(void);

#endif