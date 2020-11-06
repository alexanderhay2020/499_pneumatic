#include <xc.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "init.h"
#include "adc.h"

//void initUART(char tx, char rx){
//    
//    U1MODE = 0;
//    // Set U1TX pin to B4
//    RPB4Rbits.RPB4R = tx;
//    
//    // Set U1RX pin to A4
//    U1RXRbits.U1RXR = rx;
//    
//    // Turn on UART1 without interrupt
//    U1MODEbits.BRGH = 0;                    // set baud to 9600 bits/sec
//    U1BRG = ((48000000 / 9600) / 16) - 1;
//    
//    // 8-bit, no parity bit, 1 stop bit (8N1 setup)
//    U1MODEbits.PDSEL = 0;
//    U1MODEbits.STSEL = 0;
//    
//    // Configure TX and RX pins as output and input pins
//    U1STAbits.UTXEN = 1;
//    U1STAbits.URXEN = 1;
//    
//    // Enable the UART
//    U1MODEbits.ON = 1;
//}

int UART2Configure(int desired_baud, char tx, char rx){
    
    RPB4Rbits.RPB4R = tx;    //SET RB4 to TX

    U2RXRbits.U2RXR = rx;    //SET RX to RA4
 
    U2MODE = 0;         // disable autobaud, TX and RX enabled only, 8N1, idle=HIGH
    U2STA = 0x1400;     // enable TX and RX
    U2BRG = (SYSCLK/(16*desired_baud))-1;//Baud2BRG(desired_baud); // U2BRG = (FPb / (16*baud)) - 1
 
    // Calculate actual assigned baud rate
    int actual_baud = SYSCLK / (16 * (U2BRG+1));
 
    return actual_baud;
} // END UART2Configure()
 
/* SerialTransmit() transmits a string to the UART2 TX pin MSB first
 *
 * Inputs: *buffer = string to transmit */
int SerialTransmit(const char *buffer){
    unsigned int size = strlen(buffer);
    while( size)
    {
        while( U2STAbits.UTXBF);    // wait while TX buffer full
        U2TXREG = *buffer;          // send single character to transmit buffer
 
        buffer++;                   // transmit next character on following loop
        size--;                     // loop until all characters sent (when size = 0)
    }
 
    while( !U2STAbits.TRMT);        // wait for last transmission to finish
 
    return 0;
}
 
/* SerialReceive() is a blocking function that waits for data on
 *  the UART2 RX buffer and then stores all incoming data into *buffer
 *
 * Note that when a carriage return '\r' is received, a nul character
 *  is appended signifying the strings end
 *
 * Inputs:  *buffer = Character array/pointer to store received data into
 *          max_size = number of bytes allocated to this pointer
 * Outputs: Number of characters received */
unsigned int SerialReceive(char *buffer, unsigned int max_size){
    unsigned int num_char = 0;
 
    /* Wait for and store incoming data until either a carriage return is received
     *   or the number of received characters (num_chars) exceeds max_size */
    while(num_char < max_size)
    {
        while( !U2STAbits.URXDA);   // wait until data available in RX buffer
        *buffer = U2RXREG;          // empty contents of RX buffer into *buffer pointer
 
        // insert nul character to indicate end of string
        if( *buffer == '\r'){
            *buffer = '\0';     
            break;
        }
 
        buffer++;
        num_char++;
    }
 
    return num_char;
} // END SerialReceive()