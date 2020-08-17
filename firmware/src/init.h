#include<xc.h>
#include<sys/attribs.h>

#ifndef INIT__H__
#define INIT__H__

void init_pic();
void init_i2c(void);
void setPin(unsigned char add, unsigned char reg, unsigned char val);
char readPin(unsigned char add, unsigned char reg);

#endif // INIT__H__