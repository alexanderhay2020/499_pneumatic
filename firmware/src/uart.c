#include <xc.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "init.h"
#include "adc.h"

void UART_Init(int desired_baud){
    // UART source:
    // https://www.aidanmocke.com/blog/2018/08/29/uart/
    
    U1MODE = 0;                     // Set UART 5 off prior to setting it up
    RPB4Rbits.RPB4R = 0b0001;       // Set U1TX pin to B4
    U1RXRbits.U1RXR = 0b0010;       // Set U1RX pin to A4
    U1MODEbits.BRGH = 0;            // We want standard speed mode. Not necessary as we just set U1MODE to 0 so this is just for explanation's sake
    U1BRG = (SYSCLK / (16 * desired_baud)) - 1; // This is the formula straight from the datasheet
    U1STA = 0;                      // Disable the TX and RX pins, clear all flags
    U1STAbits.UTXEN = 1;            // Enable the TX pin
    U1STAbits.URXEN = 1;            // Enable the RX pin
    U1MODEbits.PDSEL = 0;           // PDSEL controls how many data bits and how many parity bits we want, this is the default of 8-bit data, no parity bits that most terminals use
    U1MODEbits.STSEL = 0;           // STSEL controls how many stop bits we use, let's use the default of 1
    U1MODEbits.ON = 1;              // Turn on the UART 5 peripheral
}
