#include<xc.h>
#include<sys/attribs.h>

#ifndef UART__H__
#define UART__H__

//#define Baud2BRG(desired_baud)      ( (SYSCLK / (16*desired_baud))-1)

void UART_Init(int desired_baud);
int SerialTransmit(const char *buffer);
unsigned int SerialReceive(char *buffer, unsigned int max_size);

#endif // UART__H__