#include <xc.h>
#include <stdio.h>
#include <stdlib.h>

#include "init.h"
#include "adc.h"

void initUART(){
    // Set U1TX pin to B4
    RPB4Rbits.RPB4R = 0b0001;
    
    // Set U1RX pin to A4
    U1RXRbits.U1RXR = 0b0010;
    
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
    }
}

// Read from UART1
// Block other functions until you get a '\r' or '\n'
// Send the pointer to you char array and the number of elements in the array
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