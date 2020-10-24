// PIC32 control of pneumatic actuator

#include<xc.h>                      // processor SFR definitions
#include<sys/attribs.h>             // __ISR macro
#include<string.h>
#include<stdio.h>

#include "init.h"
//#include "adc.h"
//#include "i2c.h"
#include "ili9341.h"
//#include "sscdann.h"

void ui(){
    char header[200];

    int index = 0;
    sprintf(header, "Northwestern University");
    while(header[index]) {
        print_header(65 + 5*index, 0, header[index]);
        index++;
    }
    
    index = 0;
    sprintf(header, "Robotics and Sensorimotor Control Lab");
    while(header[index]) {
        print_header(28 + 5*index, 8, header[index]);
        index++;
    } 
}

int main() {

    // initializations
    init_pic();
//    init_i2c();
    SPI1_init();
    LCD_init();
    LCD_clearScreen(ILI9341_BLACK);
    ui();

    // set pins
    TRISAbits.TRISA4 = 0;           // sets A4 as output
    LATAbits.LATA4 = 0;             // sets A4 to low
    TRISBbits.TRISB11 = 1;          // RB11 as input
    TRISBbits.TRISB12 = 1;          // RB12 as input
    TRISBbits.TRISB13 = 0;          // RB13 as output
    LATBbits.LATB13 = 1;            // 0=0v, 1=3.2v
    
    // variables
    char temp_msg[20];
    int index = 0;

    while (1) {
        // use _CP0_SET_COUNT(0) and _CP0_GET_COUNT() to test the PIC timing
        // remember the core timer runs at half the sysclk

        _CP0_SET_COUNT(0);
        
        // characters are 5x8
        // screen is 128x32
//        ssd1306_clear();
        if (LATAbits.LATA4 == 0){
            LCD_drawPixel(0,0,ILI9341_WHITE); // flashes single LED on screen
        }
        else{
            LCD_drawPixel(0,0,ILI9341_BLACK); // flashes single LED on screen
        }

        
        index = 0;
            sprintf(temp_msg, "RB11, Pin 22:   %d",PORTBbits.RB11);
            while(temp_msg[index]) {
                print_char(28 + 5*index, 56, temp_msg[index]);
                index++;
            }
//        }
//        else {//if(PORTBbits.RB11 == 1){
            index = 0;
            sprintf(temp_msg, "RB12, Pin 23:   %d",PORTBbits.RB12);
            while(temp_msg[index]) {
                print_char(28 + 5*index, 72, temp_msg[index]);
                index++;
            }
        
        LATAbits.LATA4 = !LATAbits.LATA4;

        while(_CP0_GET_COUNT() < 24000000/2){}
    }
}