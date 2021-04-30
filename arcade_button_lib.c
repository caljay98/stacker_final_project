// Arcade button library
// Initializes the button for input and to flash

#include "arcade_button_lib.h"


// global variables
volatile float byteFrameNumber = 0;
float delta = 0.1;

// Function initializes the arcade button with polling for input and the breathing pattern using Output Compare
void buttonInit(float tempdelta) {
    delta = tempdelta;
    TRISB |= 0x0100;        // Set pin RB8 as button input
    CNPU2 |= 0b1000000;
    
    TRISBbits.TRISB4 = 0;    // Set pin RB4 as an output
    LATB = 0xffff;
    
    __builtin_write_OSCCONL(OSCCON & 0xbf); // unlock PPS, only changing bit 6
    RPOR2bits.RP4R = 19;  // Use Pin RP4 for Output Compare 2 = "19" (Table 10-3)
    __builtin_write_OSCCONL(OSCCON | 0x40); // lock   PPS
   
    T3CON = 0;
    T3CONbits.TCKPS = 0b0; // Set prescaler to 1:256
    TMR3 = 0;
    PR3 = 1000 - 1; 
    T3CONbits.TON = 1; // Turn on timer 3
    
    OC2CON = 0;
    OC2CONbits.OCTSEL = 0b1; // Timer 3 for output compare
    OC2CONbits.OCM = 0b110; // PWM mode on OC1, Fault pin, OCF1 disabled
    
    
}

// Function to display a breathing pattern
void buttonPatternLED(void) {   
    setButtonLED((int)(PR3*byteFrameNumber));
    byteFrameNumber += delta;
    if ((byteFrameNumber >= 1) || (byteFrameNumber <= 0.01)) {
        delta = -delta;
    }
}

void setButtonLED(int Val) {
   OC2RS = Val; // Shadow register is a line of protection. 
                // Write to the shadow register, don't have 
                // write directly to OC2 register
}

// Input logic
uint16_t readInputButton() {
    uint16_t mask = 0x0100;
    mask &= PORTB;
    if (mask == 0x0100) {
        return 1;
    }
    else {
        return 0;
    }
}

// End of arcade_button_lib.c
