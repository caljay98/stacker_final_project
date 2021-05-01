// final_project_main.c
//  TODO DOCS


// includes
#include "xc.h"
#include "final_project_main.h"
#include "neopixel_display_lib.h"
#include "arcade_button_lib.h"
#include <stdio.h> // for abs value
#include <stdlib.h> // for abs value 



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
uint8_t color_counter = 0;
uint8_t xPos = 0; //0 is right side of screen
uint8_t direction =0; //0 is left, 1 is right
uint8_t yLevel = 0; // 0 is starting Y of the game and will keep going up per button push
uint8_t currentWidth = 3; //width of blocks
uint8_t speed = 150; // this will decrease the delay (Speed up game) as it advances
uint8_t prevData[8][3]; //data of previous levels
uint8_t gameOver = 0; //0 game is going, 1 is game over
uint8_t buttonPress = 1; //turns 0 when button is pressed and 1 if not pressed
uint8_t buttonPrev =1; 
uint8_t winner = 0; //turns 1 when player has won
uint8_t screenCorrection = 0; //Var used to correct screen size when changing width of led matrix

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
    
    //int button led
    buttonInit(0.1);
    
    // init the game logic
    // TODO
}

//adds elements to the screen of previous Y Levels
void previousLevels(uint8_t x,uint8_t y, uint8_t w, uint8_t r, uint8_t g, uint8_t b){
    add_element_to_disp(x, y, w, 1, r, g, b);
}

// loop
//  function called as fast as possible in an endless loop
void loop(void)
{
    buttonPatternLED();
    int i;
    // get the correct color
    uint8_t red;
    uint8_t grn;
    uint8_t blu;
    
   
    
    
    red = UNPACK_RED(Wheel(color_counter)) >> 4;
    grn = UNPACK_GRN(Wheel(color_counter)) >> 4;
    blu = UNPACK_BLU(Wheel(color_counter)) >> 4;
    
     // increment the color_counter for the color wheel
    color_counter++;
    
    // write to the display background
    set_background(red, grn, blu);
    
    // wait for some time to let our eyes actually see the change
    delay(speed);
    
    //Game run if winner or gameOver has not been achieved
    if(winner == 0 && gameOver == 0){
        //adds starting 3x1 matrix to screen
        add_element_to_disp(xPos, yLevel, currentWidth, 1, grn, blu, red);
        //adds previous Y Level data to screen if there is any
        for(i=0; i <= yLevel; i++){
            previousLevels(prevData[i][0],prevData[i][1],prevData[i][2],blu,red,grn);
            }

        //reads in button presses and turns to a 0 if button is pressed
        buttonPress = readInputButton();
        
        //Button is pressed
        if(buttonPress == 0 && buttonPrev == 1){
            
            //Figures out how many off the matrix is from the last matrix if any
            if(prevData[yLevel-1][2] == currentWidth){
                if(yLevel > 0 && (prevData[yLevel-1][0] != xPos)){
                    int diff = prevData[yLevel-1][0] - xPos;
                    if(diff > 0){
                        xPos += diff;
                    }
                    diff = abs(diff);
                    if(diff < currentWidth){   
                        currentWidth = currentWidth - diff; 
                    
                //game is over if matrix is off more by than the previous size 
                    }else if(diff >= currentWidth){
                     gameOver = 1;
                    }   
                }
            //Additional check if the matrix was reduced by the game and not by missing the previous matrix
            }else if(prevData[yLevel-1][2] == currentWidth - 1){
                if(yLevel > 0 && (prevData[yLevel-1][0] != xPos+1)){
                    int diff = prevData[yLevel-1][0] - xPos +1;
                    if(diff > 0){
                        xPos += diff;
                    }
                    diff = abs(diff);
                    if(diff < currentWidth){   
                        currentWidth = currentWidth - diff; 
                    
                //game is over if matrix is off more by than the previous size 
                    }else if(diff >= currentWidth){
                     gameOver = 1;
                    }   
                }
            }
            //saves this levels data to be able to print it to screen
            prevData[yLevel][0] = xPos;
            prevData[yLevel][1] = yLevel;
            prevData[yLevel][2] = currentWidth;
            
            //restarts the the matrix to the right of the screen and bumps it up a level
            xPos = 0;
            yLevel++;
            
            //If matrix is 3 wide at Y level 3, then it is reduced by one
            //This will also occur at Y level 6 if matrix is 2 wide
            if(yLevel == 3 && currentWidth == 3){
                currentWidth = 2;
            }
            if(yLevel == 6 && currentWidth == 2){
                currentWidth =1;
            }
            
            //if player reaches end of the screen they win
            if(yLevel == 8){
                winner = 1;
                }
            
            //increases refresh rate which increases game speed
            speed -=10;
            
            //resets direction of the matrix to go left first
            direction =0;   
            
            
        //No button press
        }else{
            //Figures out screen correction because when reducing matrix size, it also reduced the screen size
            if(currentWidth == 3){
                screenCorrection = 5;
            }else if (currentWidth == 2){
                screenCorrection = 6;
            }else{
                screenCorrection =7;
                
            //Movement to the left
            }
            if(direction == 0){
                if(xPos < screenCorrection){
                    xPos++;
                }else{
                    direction = 1;
                }
                
            //Movement to the right
            }else if(direction == 1){
                if(xPos > 0){
                xPos--;
            }else{
                direction =0;
            }
        }
        }
    
    //prints a "W" to the screen if the player has won
    }else if(winner == 1){
        add_element_to_disp(7, 4, 1, 3, grn, blu, red);
        add_element_to_disp(6, 2, 1, 2, grn, blu, red);
        add_element_to_disp(5, 1, 1, 1, grn, blu, red);
        add_element_to_disp(3, 2, 2, 3, grn, blu, red);
        add_element_to_disp(2, 1, 1, 1, grn, blu, red);
        add_element_to_disp(1, 2, 1, 2, grn, blu, red);
        add_element_to_disp(0, 4, 1, 3, grn, blu, red);
        
        
    //prints a "L" to the screen if the player has lost    
    }else if(gameOver == 1){
        add_element_to_disp(5, 1, 1, 6, grn, blu, red);
        add_element_to_disp(2, 1, 3, 1, grn, blu, red);
        
    }
    //makes sure button can not be held down 
    buttonPrev = buttonPress; 
   
    //updates display and a slight delay to have some button debouncing
        update_display();
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


// Wheel
//  Input a value 0 to 255 to get a color value.
//  The colors are a transition r - g - b - back to r.
uint32_t Wheel(uint8_t WheelPos)
{
    uint32_t cdown = (255 - WheelPos * 3);
    uint32_t cup = (WheelPos * 3);
    
    if(WheelPos <= 85)
    {
        return PACK(cdown, 0, cup);
    }
    if(WheelPos <= 170)
    {
        return PACK(0, cup, cdown);
    }
    
    return PACK(cup, cdown, 0);
}


// End of final_project_main.c