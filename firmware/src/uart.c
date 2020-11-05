#include <xc.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "init.h"
#include "adc.h"

void initUART(char tx, char rx){
    
    U1MODE = 0;
    // Set U1TX pin to B4
    RPB4Rbits.RPB4R = tx;
    
    // Set U1RX pin to A4
    U1RXRbits.U1RXR = rx;
    
    // Turn on UART1 without interrupt
    U1MODEbits.BRGH = 0;                    // set baud to 9600 bits/sec
    U1BRG = ((48000000 / 9600) / 16) - 1;
    
    // 8-bit, no parity bit, 1 stop bit (8N1 setup)
    U1MODEbits.PDSEL = 0;
    U1MODEbits.STSEL = 0;
    
    // Configure TX and RX pins as output and input pins
    U1STAbits.UTXEN = 1;
    U1STAbits.URXEN = 1;
    
    // Enable the UART
    U1MODEbits.ON = 1;
}

void writeUART(char * string){
    while (*string != '\0'){
        while (U1STAbits.UTXBF){
            ;                               // Wait until TX buffer isn't full
        }
        U1TXREG = *string;
        string++;
    }
}

// Read from UART1
// Block other functions until you get a '\r' or '\n'
// Send the pointer to you char array and the number of elements in the array
//int readUART(char *buffer, unsigned int max_size){
//    unsigned int num_char = 0;
// 
//    /* Wait for and store incoming data until either a carriage return is received
//     *   or the number of received characters (num_chars) exceeds max_size */
//    while(num_char < max_size)
//    {
//        while(!U1STAbits.URXDA);   // wait until data available in RX buffer
//        *buffer = U1RXREG;          // empty contents of RX buffer into *buffer pointer
// 
//        // insert nul character to indicate end of string
//        if( *buffer == '\r'){
//            *buffer = '\0';     
//            break;
//        }
//        buffer++;
//        num_char++;
//    }
// 
//    return num_char;
//} 


void readUART(char * message, int maxLength){
    char data = 0;
    int complete = 0, num_bytes = 0;
    // loop until you get a '\r' or '\n'
    while (!complete){
        if (U1STAbits.URXDA){               // If data is available
            data = U1RXREG;                 // Read the data
            if ((data == '\n') || (data == '\r')){
                complete = 1;
            } else {
                message[num_bytes] = data;
                ++num_bytes;
                // Roll over if array is too small
                if (num_bytes >= maxLength){
                    num_bytes = 0;
                }
            }
        }
    }
    // End the string
    message[num_bytes] = '\0';
}