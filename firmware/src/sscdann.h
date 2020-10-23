#ifndef SSCDANN_H__
#define SSCDANN_H__

#include<xc.h> // processor SFR definitions

#define SSCDANN_ADDR 0x28      // 40; 0010 1000 

void sscdann_read(signed char *, int);

short get_status(signed char *);
short get_pressure(signed char *);
short get_temperature(signed char *);

#endif