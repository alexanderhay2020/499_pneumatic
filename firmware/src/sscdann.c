// based on adafruit and sparkfun libraries

#include <string.h> // for memset
#include <xc.h> // for the core timer delay

#include "i2c.h"
#include "sscdann.h"

//const long OutMax = 14745, OutMin = 1638;
//const long PressureMax = 150, PressureMin = 0; // PSI

void sscdann_read(signed char *data, int amt) {
//    i2c_master_setup();
    i2c_master_start();
    i2c_master_send(SSCDANN_ADDR | 1);   // R/W = 1 = read
    int i;
    for(i=0;i<amt;i++){
//        data[i] = i2c_master_recv(); // receive a byte from GP7
//        if(i == (amt-1)) {
//            i2c_master_ack(1);          // make the nack bit
//        }
//        else {
//            i2c_master_ack(0);          // make the ack bit
//        }
    }
    i2c_master_stop();
}

short get_status(signed char *data) {
    return ((data[1]<<8 | data[0]));
}

short get_pressure(signed char *data) {
    return (data[3]<<8 | data[2]);
}

short get_temperature(signed char *data) {
    return (data[5]<<8 | data[4]);
}
