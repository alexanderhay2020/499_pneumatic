// PIC32 control of pneumatic actuator

#include<xc.h>                      // processor SFR definitions
#include<sys/attribs.h>             // __ISR macro
#include<string.h>
#include<stdio.h>

#include "init.h"
#include "adc.h"
#include "i2c.h"
#include "ssd1306.h"
#include "font.h"

void wait(float time){
    
}

void convert(){
    
}

void draw_box(){
    
    int i = 0;
    char indicator_msg[10];
    
    // left horizontal lines
    for(i=15;i<49;i++){
        ssd1306_drawPixel(i,21,1);
        ssd1306_drawPixel(i,31,1);
    }
    // left vertical lines
    for(i=21;i<31;i++){
        ssd1306_drawPixel(15,i,1);
        ssd1306_drawPixel(48,i,1);
    }

    // right horizontal lines
    for(i=79;i<114;i++){
        ssd1306_drawPixel(i,21,1);
        ssd1306_drawPixel(i,31,1);
    }

    // right vertical lines
    for(i=22;i<31;i++){
        ssd1306_drawPixel(79,i,1);
        ssd1306_drawPixel(113,i,1);
    }

    sprintf(indicator_msg, "RIGHT");
    drawString(82,23,indicator_msg);

    sprintf(indicator_msg, "LEFT");
    drawString(20,23,indicator_msg);
}


void adcConfigureAutoScan(unsigned adcPINS, unsigned numPins){
    AD1CON1 = 0x0000; // disable ADC
 
    // AD1CON1<2>, ASAM    : Sampling begins immediately after last conversion completes
    // AD1CON1<7:5>, SSRC  : Internal counter ends sampling and starts conversion (auto convert)
    AD1CON1SET = 0x00e4;
 
    // AD1CON2<1>, BUFM    : Buffer configured as two 8-word buffers, ADC1BUF7-ADC1BUF0, ADC1BUFF-ADCBUF8
    // AD1CON2<10>, CSCNA  : Scan inputs
//    AD1CON2 = 0x0402;
    AD1CON2bits.BUFM = 1;
//    AD1CON2bits.CSCNA = 1; //<- this breaks it
 
    // AD2CON2<5:2>, SMPI  : Interrupt flag set at after numPins completed conversions
    AD1CON2SET = (numPins-1) << 2;
 
    // AD1CON3<7:0>, ADCS  : TAD = TPB * 2 * (ADCS<7:0> + 1) = 4 * TPB in this example
    // AD1CON3<12:8>, SAMC : Acquisition time = AD1CON3<12:8> * TAD = 15 * TAD in this example
    AD1CON3 = 0x0f01;
 
    // AD1CHS is ignored in scan mode
    AD1CHS = 0;
 
    // select which pins to use for scan mode
    AD1CSSL = adcPINS;
    AD1CON1bits.ON = 1;
}


int main() {

    init_pic();
    adcConfigureAutoScan(0x0008, 3);
//    init_i2c();
    ssd1306_setup();
//    ssd1306_clear();
//    init_ADC(14);
    
    TRISAbits.TRISA4 = 0;           // sets A4 as output
    TRISBbits.TRISB4 = 0;           // sets B4 as output
//    TRISBbits.TRISB5 = 1;           // sets B5 as input
    
    LATAbits.LATA4 = 0;             // sets A4 to low
    LATBbits.LATB4 = 1;             // sets B4 to high
    
//    char indicator_msg[10];
    char pressure_msg[20];

    int an1 = 0;
    int an2 = 0;
    int an3 = 0;
    
    while (1) {
        // use _CP0_SET_COUNT(0) and _CP0_GET_COUNT() to test the PIC timing
        // remember the core timer runs at half the sysclk

        _CP0_SET_COUNT(0);
//        int i = 0;
        
        ssd1306_clear();
//        ssd1306_drawPixel(0,0,LATAbits.LATA4); // flashes single LED on screen
//        
//        if (LATAbits.LATA4 == 0){
//            sprintf(indicator_msg, "v");
//            drawString(29,14,indicator_msg);  
//        }
//        else {
//            sprintf(indicator_msg, "v");
//            drawString(93,14,indicator_msg);
//        }
        LATAbits.LATA4 = !LATAbits.LATA4;
        LATBbits.LATB4 = !LATBbits.LATB4;
        while(_CP0_GET_COUNT() < 24000000/2){}
//
//        draw_box();
        

        while( ! IFS0bits.AD1IF);       // wait until buffers contain new samples
        AD1CON1bits.ASAM = 0;           // stop automatic sampling (essentially shut down ADC in this mode)

        if( AD1CON2bits.BUFS == 1){     // check which buffers are being written to and read from the other set
            an1 = ADC1BUF0;
            an2 = ADC1BUF1;
            an3 = ADC1BUF2;
        }
        else{
            an1 = ADC1BUF8;
            an2 = ADC1BUF9;
            an3 = ADC1BUFA;
        }
        AD1CON1bits.ASAM = 1;           // restart automatic sampling
        IFS0CLR = 0x10000000;           // clear ADC interrupt flag

//        int val = read_adc(5);
        sprintf(pressure_msg, "an1: %d", an1);
        drawString(0,0,pressure_msg); 
        sprintf(pressure_msg, "an2: %d", an2);
        drawString(0,8,pressure_msg); 
        sprintf(pressure_msg, "an3: %d", an3);
        drawString(0,16,pressure_msg); 
        ssd1306_update();
    }
}