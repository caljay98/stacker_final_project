// final_project_main.c
//  TODO DOCS


// includes
#include "xc.h"
#include "final_project_main.h"
#include "neopixel_display_lib.h"


// pragmas
// CW1: FLASH CONFIGURATION WORD 1 (see PIC24 Family Reference Manual 24.1)
#pragma config ICS = PGx1          // Comm Channel Select (Emulator EMUC1/EMUD1 pins are shared with PGC1/PGD1)
#pragma config FWDTEN = OFF        // Watchdog Timer Enable (Watchdog Timer is disabled)
#pragma config GWRP = OFF          // General Code Segment Write Protect (Writes to program memory are allowed)
#pragma config GCP = OFF           // General Code Segment Code Protect (Code protection is disabled)
#pragma config JTAGEN = OFF        // JTAG Port Enable (JTAG port is disabled)


// CW2: FLASH CONFIGURATION WORD 2 (see PIC24 Family Reference Manual 24.1)
#pragma config I2C1SEL = PRI       // I2C1 Pin Location Select (Use default SCL1/SDA1 pins)
#pragma config IOL1WAY = OFF       // IOLOCK Protection (IOLOCK may be changed via unlocking seq)
#pragma config OSCIOFNC = ON       // Primary Oscillator I/O Function (CLKO/RC15 functions as I/O pin)
#pragma config FCKSM = CSECME      // Clock Switching and Monitor (Clock switching is enabled, 
                                       // Fail-Safe Clock Monitor is enabled)
#pragma config FNOSC = FRCPLL      // Oscillator Select (Fast RC Oscillator with PLL module (FRCPLL))


// global variables / defines
uint8_t brightness = 0;

int main(void)
{
    setup();
    while(1)
    {
        loop();
    }
    return 0;
}


// setup
//  function called once on boot
void setup(void)
{    
    //Set RCDIV=1:1 (default 2:1) 32MHz or FCY/2=16M
    CLKDIVbits.RCDIV = 0;
    
    // init the button
    // TODO
    
    // init the display
    display_init();
    
    // init the game logic
    // TODO
}


// loop
//  function called as fast as possible in an endless loop
void loop(void)
{
    set_background(0xAA, 0xAA, 0xAA);
    update_display();
    delay(10);
    brightness++;
}


// delay
//  function that will keep the program in a loop to add a pause to the program
//  wait_time is inputted as ms. This function uses timer5, with a max input of
//  1048
void delay(uint16_t wait_time)
{
    // setup timer5 to work as a ms timer
    T5CON = 0;                  // reset timer5 settings
    TMR5 = 0;                   // reset the timer5 counter
    T5CONbits.TCKPS = 0b11;     // 1:256 mode
    PR5 = (wait_time*62.5)-1;   // convert from ms to ticks
    _T5IE = 0;                  // no ISR
    _T5IF = 0;                  // reset the interrupt flag
    T5CONbits.TON = 1;          // start the timer
    
    while (!_T5IF);             // wait until the timer overflows once
    
    return;
}


// End of final_project_main.c