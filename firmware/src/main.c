// PIC32 control of pneumatic actuator

#include<xc.h>                      // processor SFR definitions
#include<sys/attribs.h>             // __ISR macro
#include<string.h>
#include<stdio.h>
#include<stdlib.h>

#include "init.h"
#include "ili9341.h"
#include "adc.h"
#include "uart.h"

double transfer_function(int voltage){
    /* Voltage divider, as built, reduces voltage from 5v to 3.27v
     int voltage has a value between 0 and 1023 (PIC32 has a 10-bit resolution ADC)
     Pressure sensor reads gage pressure
     Output is proportional to the difference between 
     applied pressure and atmospheric (ambient) pressure 
     */

    double v_supply = 3.36;
    double v_output = (voltage/1023.)*v_supply;
    
//    double v_min = v_supply * 0;
    double v_max = v_supply * 1;
    
    // Documented transfer function
//    double pressure = (v_min + ((v_max-v_min)/(0.8*v_supply))*(v_output-(0.1*v_supply)))+14.7;
    
    // Proportional transfer function
    double pressure = (((v_output/v_max))*150)-14.7;
    
    return pressure;
}

void _mon_putc (char c){
   while (U1STAbits.UTXBF); // Wait til current transmission is complete
   U1TXREG = c;
}

int main() {

    // Initializations//    AD1CON1SET = 0x8000;    // start ADC

    
    // Touch Sensor
    ANSELBbits.ANSB2 = 1;           // sets RB3 (AN4) as analog
    TRISBbits.TRISB2 = 1;           // sets RB2 as input
//    
//    // Pressure Sensor
    ANSELBbits.ANSB3 = 1;           // sets RB3 (AN5) as analog
    TRISBbits.TRISB3 = 1;           // sets RB3 as input
//    
//    // Toggle Switch
    TRISBbits.TRISB10 = 1;          // RB10 as input
    TRISBbits.TRISB11 = 1;          // RB11 as input
    ANSELBbits.ANSB12 = 0;          // B12 as digital
    TRISBbits.TRISB12 = 0;          // RB12 as output
    TRISBbits.TRISB13 = 0;          // RB13 as output

    init_pic();
    
    // variables
    int voltage = 0;
    int cap = 0;
    double pressure=0;
    char temp_msg[30]; 
//    char message[200]; 
//    char rx_msg[10];
    
    while (1) {

        _CP0_SET_COUNT(0);                  // start timer 
        
//        voltage = analogRead_auto();        // returns value between 0 and 1023
        voltage = analogRead(5);
        pressure = transfer_function(voltage);        

        sprintf(temp_msg, "Pressure:           %5.3f PSI ", pressure);
        write_screen(28, 24, temp_msg);
        
        sprintf(temp_msg, "Update Frequency:    %3.1f Hz", (24000000.0/_CP0_GET_COUNT()));
        write_screen(28, 40, temp_msg);
        
        sprintf(temp_msg, "RB10, Pin 21:   %d", PORTBbits.RB10);
        write_screen(28, 56, temp_msg);
        
        sprintf(temp_msg, "RB11, Pin 22:   %d", PORTBbits.RB11);
        write_screen(28, 72, temp_msg);
        
        sprintf(temp_msg, "RB12, Pin 23:   %d", LATBbits.LATB12);
        write_screen(28, 88, temp_msg);
        
        sprintf(temp_msg, "RB13, Pin 24:   %d", LATBbits.LATB13);
        write_screen(28, 104, temp_msg);
        
//        readUART(message, 200);  // get message from computer
//        sscanf(message, "%s", rx_msg);
//        write_screen(200, 104, rx_msg);
        
        cap = ctmu_read(4, (4800000/4)); // tuned
        
        if (cap>400){
            sprintf(temp_msg, "Touched: yes");
        }
        else{
            sprintf(temp_msg, "Touched: no ");
        }
        write_screen(200, 104, temp_msg);

    }
}