// PIC32 control of pneumatic actuator

#include<xc.h>                      // processor SFR definitions
#include<sys/attribs.h>             // __ISR macro
#include<string.h>
#include<stdio.h>

#include "init.h"
#include "ili9341.h"
#include "adc.h"

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

void heartbeat(int freq){
    
    double freq_clock = (1./freq)*24000000;
    
//    int index = 0;
//    char temp_msg[20];
//
//    sprintf(temp_msg, "fps_clock:           %f", fps_clock);
//    while(temp_msg[index]) {
//        print_char(28 + 5*index, 56, temp_msg[index]);
//        index++;
//    }

    if (LATAbits.LATA4 == 0){
        LCD_drawPixel(0,0,ILI9341_WHITE); // flashes single LED on screen
    }
    else{
        LCD_drawPixel(0,0,ILI9341_BLACK); // flashes single LED on screen
    }

    LATAbits.LATA4 = !LATAbits.LATA4;
    
    // core timer runs at half the sysclk (48MHz, 48000000)

    while(_CP0_GET_COUNT() < freq_clock){;} // 5Hz (4800000/24000000)
    
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
//    TRISAbits.TRISA4 = 0;           // sets RA4 as output
//    LATAbits.LATA4 = 0;             // sets RA4 to low
//    TRISBbits.TRISB4 = 0;           // sets RB4 as output
//    LATBbits.LATB4 = 0;             // sets RB4 to low
    
    TRISBbits.TRISB10 = 1;          // RB10 as input
    TRISBbits.TRISB11 = 1;          // RB11 as input
    ANSELBbits.ANSB12 = 0;          // B12 as digital
    TRISBbits.TRISB12 = 0;          // RB12 as output
    TRISBbits.TRISB13 = 0;          // RB13 as output

    init_pic();
    
    // variables
//    int freq = 5;
    int voltage;
    int index = 0;
    int cap = 0;
    double pressure=0;
    char temp_msg[30];
    
    ui();
    
    while (1) {

        _CP0_SET_COUNT(0);              // start timer 

//        heartbeat(freq);                   // 5 Hz

//        voltage = analogRead(5); // note that we call pin AN5 (RB3) by it's analog number
//        delay_us(voltage);       // delay according to the voltage at RB3 (AN5)
//        LATBINV = 0x0020;     // invert the state of RB5
        voltage = analogRead_auto();        // returns value between 0 and 1023
//        pressure = transfer_function(voltage);
//        cap = do_cap(4, (4800000/8)); // tuned

        // Check if sensor is clipping
        // Clipping occurs at 2.5% and 97.5% V_s (ADC values of 26 and 997)
        if (voltage < 26 || voltage > 997){
            index = 0;
            sprintf(temp_msg, "Status: Clipping");
            while(temp_msg[index]) {
                print_char(200 + 5*index, 24, temp_msg[index]);
                index++;
            }
        }
        else{
            pressure = transfer_function(voltage);
            index = 0;
            sprintf(temp_msg, "Status: Normal  ");
            while(temp_msg[index]) {
                print_char(200 + 5*index, 24, temp_msg[index]);
                index++;
            }
        }

        index = 0;
        sprintf(temp_msg, "ADC Read:           %d  ", voltage);
        while(temp_msg[index]) {
            print_char(28 + 5*index, 86, temp_msg[index]);
            index++;
        }
        
        index = 0;
        sprintf(temp_msg, "Pressure:           %5.3f PSI ", pressure);
        while(temp_msg[index]) {
            print_char(28 + 5*index, 24, temp_msg[index]);
            index++;
        }
        
        index = 0;
        sprintf(temp_msg, "Update Frequency:    %3.1f Hz", (24000000.0/_CP0_GET_COUNT()));
        while(temp_msg[index]) {
            print_char(28 + 5*index, 40, temp_msg[index]);
            index++;
        }
        
        if(PORTBbits.RB11){
            LATAbits.LATA4 = 1;             // sets RA4 to hi
            LATBbits.LATB4 = 0;
            index = 0;
            sprintf(temp_msg, "EXTENDED ");
            while(temp_msg[index]) {
                print_char(240 + 5*index, 40, temp_msg[index]);
                index++;
            } 
        }
        else{
            LATAbits.LATA4 = 0;             // sets RA4 to hi
            LATBbits.LATB4 = 1;
            index = 0;
            sprintf(temp_msg, "RETRACTED");
            while(temp_msg[index]) {
                print_char(240 + 5*index, 40, temp_msg[index]);
                index++;
            } 
        }
        if (cap>1120){
            index = 0;
            sprintf(temp_msg, "Untouched");
            while(temp_msg[index]) {
                print_char(240 + 5*index, 56, temp_msg[index]);
                index++;
            } 
        }
        else{
            index = 0;
            sprintf(temp_msg, "Touched");
            while(temp_msg[index]) {
                print_char(240 + 5*index, 56, temp_msg[index]);
                index++;
            }             
        }
        
//        draw_piston(PORTBbits.RB11);
//        
        index = 0;
        sprintf(temp_msg, "RB10, Pin 21:   %d", PORTBbits.RB10);
        while(temp_msg[index]) {
            print_char(28 + 5*index, 56, temp_msg[index]);
            index++;
        }
        index = 0;
        sprintf(temp_msg, "RB11, Pin 22:   %d", PORTBbits.RB11);
        while(temp_msg[index]) {
            print_char(28 + 5*index, 72, temp_msg[index]);
            index++;
        }
    }
}