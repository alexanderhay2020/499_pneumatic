#include<xc.h>                      // processor SFR definitions
#include<sys/attribs.h>             // __ISR macro

#include "init.h"
#include "i2c.h"

// DEVCFG0
#pragma config DEBUG = ON           // disable debugging
#pragma config JTAGEN = OFF         // disable jtag
#pragma config ICESEL = ICS_PGx1    // use PGED1 and PGEC1
#pragma config PWP = OFF            // disable flash write protect
#pragma config BWP = OFF            // disable boot write protect
#pragma config CP = OFF             // disable code protect

// DEVCFG1
#pragma config FNOSC = PRIPLL       // use primary oscillator with pll
#pragma config FSOSCEN = OFF        // disable secondary oscillator
#pragma config IESO = OFF           // disable switching clocks
#pragma config POSCMOD = HS         // high speed crystal mode
#pragma config OSCIOFNC = OFF       // disable clock output
#pragma config FPBDIV = DIV_1       // divide sysclk freq by 1 for peripheral bus clock
#pragma config FCKSM = CSDCMD       // disable clock switch and FSCM
#pragma config WDTPS = PS1048576    // use largest wdt
#pragma config WINDIS = OFF         // use non-window mode wdt
#pragma config FWDTEN = OFF         // wdt disabled
#pragma config FWDTWINSZ = WINSZ_25 // wdt window at 25%

// DEVCFG2 - get the sysclk clock to 48MHz from the 8MHz crystal
#pragma config FPLLIDIV = DIV_2     // divide input clock to be in range 4-5MHz
#pragma config FPLLMUL = MUL_24     // multiply clock after FPLLIDIV
#pragma config FPLLODIV = DIV_2     // divide clock after FPLLMUL to get 48MHz

// DEVCFG3
#pragma config USERID = 0           // some 16bit userid, doesn't matter what
#pragma config PMDL1WAY = OFF       // allow multiple reconfigurations
#pragma config IOL1WAY = OFF        // allow multiple reconfigurations

void init_pic(){
    
    __builtin_disable_interrupts(); // disable interrupts while initializing things

    // set the CP0 CONFIG register to indicate that kseg0 is cacheable (0x3)
    __builtin_mtc0(_CP0_CONFIG, _CP0_CONFIG_SELECT, 0xa4210583);
    
    BMXCONbits.BMXWSDRM = 0x0;      // 0 data RAM access wait states
    INTCONbits.MVEC = 0x1;          // enable multi vector interrupts
    DDPCONbits.JTAGEN = 0;          // disable JTAG to get pins back

    __builtin_enable_interrupts();
}

void init_i2c(void){

//    I2C1CONbits.ON = 1;
    i2c_master_setup();
//    i2c_master_start();
//    i2c_master_send(SLAVE);        // set the address w/ write
//    i2c_master_send(0x00);          // set the register (IODIRA)
//    i2c_master_send(0x00);          // set the value    (0000 0000) for outputs
//    i2c_master_stop();              // send a stop
//    i2c_master_start();
//    i2c_master_send(SLAVE);        // set the address w/ write
//    i2c_master_send(0x01);          // set the register (IODIRB)
//    i2c_master_send(0xFF);          // set the value    (1111 1111) for inputs
    i2c_master_stop();              // send a stop
}

void setPin(unsigned char add, unsigned char reg, unsigned char val){
    
    i2c_master_start();
    i2c_master_send(add);
    i2c_master_send(reg);
    i2c_master_send(val);          // FF-yellow on; 00-yellow off
    i2c_master_stop();
}


char readPin(unsigned char add, unsigned char reg){
    
    i2c_master_start();
    i2c_master_send(add|0);         // address for writing
    i2c_master_send(reg);           // register to read from
    i2c_master_restart();           // restart
    i2c_master_send(add|0x1);         // address for reading
    
    char recv = i2c_master_recv();  // received value
    
    i2c_master_ack(1);              // acknowledge bit
    i2c_master_stop();              // stop
    
    return recv;
}

unsigned char hexaDeciNum[100]; // 128x32/8. Every bit is a pixel

void decToHex(int n){    
    // char array to store hexadecimal number 
//    char hexaDeciNum[100]; 
      
    // counter for hexadecimal number array 
    int i = 0; 
    while(n!=0) 
    {    
        // temporary variable to store remainder 
        int temp  = 0; 
          
        // storing remainder in temp variable. 
        temp = n % 16; 
          
        // check if temp < 10 
        if(temp < 10) 
        { 
            hexaDeciNum[i] = temp + 48; 
            i++; 
        } 
        else
        { 
            hexaDeciNum[i] = temp + 55; 
            i++; 
        } 
          
        n = n/16; 
    } 
//    return hexaDeciNum;
} 