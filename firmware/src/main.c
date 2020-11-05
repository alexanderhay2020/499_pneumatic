// PIC32 control of pneumatic actuator

#include<xc.h>                      // processor SFR definitions
#include<sys/attribs.h>             // __ISR macro
#include<string.h>
#include<stdio.h>

#include "init.h"
#include "ili9341.h"
#include "adc.h"
#include "uart.h"

void write_screen(int x, int y, char *msg){
    int index = 0;
    while(msg[index]) {
        print_char(x + 5*index, y, msg[index]);
        index++;
    }
}

void plot_axis(){
    // (0,0)            (320,0)
    //   + - - - - - - - - -
    //   |
    //   |
    //   |
    // (0,240)
    
    // x-axis
    int x = 0;
    int x_offset = 210;
    int y = 0;
    int y_offset = 167;
    for(x=0;x<100;x++){
        LCD_drawPixel(x+x_offset, y+y_offset, ILI9341_WHITE);
    }
    
    // y-axis
    x = 0;
    y_offset = 40;
    for(y=0;y<127;y++){
        LCD_drawPixel(x+x_offset, y+y_offset, ILI9341_WHITE);
    }
}

void plot_data(int val, int * hist){
    
    int temp;
    int y_offset = 40;
    int i;
    char temp_msg[20];
    
    if(!hist[0]){
        hist[0] = val%128;
    }
    else{
        for(i=0; i<99; i++){
            temp = hist[i];
            hist[i+1] = temp;
        }
        hist[0] = val%128;
    }
    
//    int y = val%128;
    
    for(i=0;i<100;i++){
        LCD_drawPixel(210+i,hist[i]+y_offset, ILI9341_WHITE);
    }
    // Title
    sprintf(temp_msg, "Sensor Value (0-1023)");
    write_screen(210, 24, temp_msg);
    
    // Label
    sprintf(temp_msg, "Time (ms)");
    write_screen(240, 170, temp_msg);
}

void ui(){
    
    LCD_clearScreen(ILI9341_BLACK);
    
    plot_axis();
    
    char header[200];
    int index = 0;
    
    sprintf(header, "Northwestern University");
    while(header[index]) {
        print_header(105 + 5*index, 0, header[index]);
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
    double pressure = (((v_output/v_max))*150);//-14.7;
    
    return pressure;
}



int main() {

    // Initializations
    
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
    int voltage;
    double pressure=0;
    char temp_msg[30];
    int hist[128];

    ui();
    
    while (1) {

        _CP0_SET_COUNT(0);              // start timer 
        
        voltage = analogRead_auto();        // returns value between 0 and 1023
        pressure = transfer_function(voltage);
        plot_data(voltage, hist);
        
        if(PORTBbits.RB11){
            LATBbits.LATB12 = 1;
            LATBbits.LATB13 = 0;
            
            sprintf(temp_msg, "Actuator Status:     Extended ");
            write_screen(28, 104, temp_msg);
            while(_CP0_GET_COUNT() < 2400000){;}
            sprintf(temp_msg, "Actuator Contact:    Touched  ");
            write_screen(28, 120, temp_msg);            

        }
        if(PORTBbits.RB10){
            LATBbits.LATB12 = 0;
            LATBbits.LATB13 = 1;
            
            sprintf(temp_msg, "Actuator Status:     Retracted");
            write_screen(28, 104, temp_msg);
            
            sprintf(temp_msg, "Actuator Contact:    Untouched");
            write_screen(28, 120, temp_msg);
        }
        
        sprintf(temp_msg, "System Status");
        write_screen(28, 24, temp_msg);
        
        sprintf(temp_msg, "Pressure:            %5.3f PSI ", pressure);
        write_screen(28, 40, temp_msg);
        
        sprintf(temp_msg, "Update Frequency:    %3.1f Hz  ", (24000000.0/_CP0_GET_COUNT()));
        write_screen(28, 56, temp_msg);
        
        sprintf(temp_msg, "Serial Port:         Active");
        write_screen(28, 72, temp_msg);
        
        sprintf(temp_msg, "Actuator Control:    Manual");
        write_screen(28, 88, temp_msg);
    }
}