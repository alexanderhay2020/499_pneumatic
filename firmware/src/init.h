#include<xc.h>
#include<sys/attribs.h>


#ifndef INIT__H__
#define INIT__H__

#define SYSCLK (48000000)

void init_pic();
void setPin(unsigned char add, unsigned char reg, unsigned char val);
char readPin(unsigned char add, unsigned char reg);

#endif // INIT__H__