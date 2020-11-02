#include<xc.h>
#include<sys/attribs.h>

#ifndef UART__H__
#define UART__H__

void initUART(char tx, char rx);
void writeUART(char * string);
int readUART(char * message, int maxLength);

#endif // UART__H__