// PIC32 control of pneumatic actuator

#include<xc.h>                      // processor SFR definitions
#include<sys/attribs.h>             // __ISR macro
#include<string.h>
#include<stdio.h>

#include "init.h"
#include "ili9341.h"
#include "adc.h"
#include "uart.h"

void ui(){
    
    LCD_clearScreen(ILI9341_BLACK);
    
    char header[200];
    int index = 0;
    
    sprintf(header, "Northwestern University");
    while(header[index]) {
        print_header(95 + 5*index, 0, header[index]);
        index++;
    }
    
    index = 0;
    sprintf(header, "Robotics and Sensorimotor Control Lab");
    while(header[index]) {
        print_header(75 + 5*index, 8, header[index]);
        index++;
    } 
}

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

void write_screen(int x, int y, char *msg){
    int index = 0;
    while(msg[index]) {
        print_char(x + 5*index, y, msg[index]);
        index++;
    }
}

int main() {

    // Initializations

//    SPI1_init();
//    LCD_init();
////    adcConfigureManual();             // Configure ADC
//    adcConfigureAutoScan(0x0020, 1);    // REMEMBER TO CHANGE AD1CON2SET
//    AD1CON1SET = 0x8000;                // start ADC
////    ctmu_setup ();
//    initUART();
    
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
    UART2Configure(9600, 0b0001, 0b0010);
    
    // variables
    int voltage;
    double pressure=0;
    char temp_msg[30];
//    int i = 0;
//    char tx_msg[10];    
//    char rx_msg[1024];
    char buf[1024];       // declare receive buffer with max size 1024
    
    ui();
    
    /* Add a small delay for the serial terminal
     *  Although the PIC sends out data fine, I've had some issues with serial terminals
     *  being garbled if receiving data too soon after bringing the DTR line low and
     *  starting the PIC's data transmission. This has ony been with higher baud rates ( > 9600) */
    int t;
    for( t=0 ; t < 100000 ; t++);
 
    sprintf(buf, "Hello Earthling!\r\n");
    SerialTransmit(buf);
//    SerialTransmit("Talk to me and hit 'enter'. Let the mocking begin!\r\n\r\n");
 
//    unsigned int rx_size;
    
    while (1) {

        _CP0_SET_COUNT(0);              // start timer 

        voltage = analogRead_auto();        // returns value between 0 and 1023
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
        
//        rx_size = SerialReceive(buf, 1024);     // wait here until data is received
//        SerialTransmit(buf);                    // Send out data exactly as received
// 
//        // if anything was entered by user, be obnoxious and add a '?'
//        if( rx_size > 0){ 
//            SerialTransmit("?\r\n");
//        }
//        SerialTransmit("\r\n");
    }
}