#include<xc.h>
#include<sys/attribs.h>

#ifndef ADC__H__
#define ADC__H__

//void init_ADC(void);
unsigned int adc_sample_convert(int pin);
unsigned int read_adc(int pin);

#endif // ADC__H__