#include<xc.h>
#include<sys/attribs.h>

#ifndef UART__H__
#define UART__H__

//#define Baud2BRG(desired_baud)      ( (SYSCLK / (16*desired_baud))-1)

int UART2Configure(int desired_baud, char tx, char rx);
int SerialTransmit(const char *buffer);
unsigned int SerialReceive(char *buffer, unsigned int max_size);

#endif // UART__H__