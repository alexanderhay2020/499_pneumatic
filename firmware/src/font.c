// based on adafruit and sparkfun libraries

#include <string.h> // for memset
#include <xc.h> // for the core timer delay

#include "i2c.h"
#include "ssd1306.h"
#include "font.h" 

// make these functions:
void drawChar(unsigned short x, unsigned short y, char ch) {
    int i;
    int j;
    char fig;
    
    for (i=0; i<5; i++) {                       // letters are 5 pix wide
        fig = ASCII[ch-32][i];                   // 0-31 are printing functions
        for (j=0; j<8; j++) {                   // letters are 8 pix tall
            if(((fig >> j) & (0x01)) == 1){        
                ssd1306_drawPixel(x+i, y+j, 1);
            }
        }
    }
}


void drawString(unsigned short x, unsigned short y, char ch[]){
    int i=0,k=0;
    while(1){
        if (ch[i] == '\0')
            break;
        drawChar(x+k, y, ch[i]);
        i++;
        k+=6;
    }
}

