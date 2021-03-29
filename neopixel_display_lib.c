// neopixel_display_lib.c
//  TODO DOCS
// uses timer2 and OC1


// includes
#include "neopixel_display_lib.h"


// global variables
// the frame buffer read from. Must be 64 pixels long, with 3 bytes for pixels.
// Stored in the same order as the neopixel, G-R-B.
uint8_t frame_buffer[NUM_OF_PIX*BYTES_PER_PIX];
uint16_t current_byte = 0;                 // counter for each led
uint16_t current_bit = 0;                  // counter for each bit in the color code


// display_init
//  This function will init the display and all of the needed interrupts. Parameters
//  for the function are still TBD. Should always succeed
void display_init()
{
    // set each pin to digital output
    AD1PCFG = 0xffff;
    
    // set RB6 to output
    TRISBbits.TRISB6 = 0;
    
    // map RB6 to the hardware output compare
    __builtin_write_OSCCONL(OSCCON & 0xBF);         // unlock PPS
    RPOR3bits.RP6R = 18;                            // map the correct pin to output compare
    __builtin_write_OSCCONL(OSCCON | 0x40);         // lock PPS
    
    // configure the output compare peripheral
    OC1CON = 0;                                     // start with the reg reset
    OC1CONbits.OCTSEL = 0;                          // use timer2 as the source
    OC1CONbits.OCM = OCMODE_PWM;                    // PWM mode on this pin
    OC1R = 0;
    
    // configure timer2, which controls the entire cycle
    T2CON = 0;                                      // start with the reg reset
    T2CONbits.TCKPS1 = 0;
    T2CONbits.TCKPS0 = 0;                           // set to 1:1 operation
    TMR2 = 0;                                       // reset the TMR2 counting reg
    PR2 = BIT_SEND_CYCLES - 1;                      // 1.5us cycle
    
    // set the background to all off
    set_background(0, 0, 0);
}


// set_background
//  function to set the background color of the display. Call this before adding
//  elements to the display as those will replace the background pixels in that
//  spot
void set_background(uint8_t red, uint8_t grn, uint8_t blu)
{
    uint8_t curr_pix = 0;
    
    // run through each pixel in the display and set the color
    while (curr_pix < (NUM_OF_PIX*BYTES_PER_PIX))
    {
        frame_buffer[curr_pix] = grn;
        curr_pix++;
        frame_buffer[curr_pix] = red;
        curr_pix++;
        frame_buffer[curr_pix] = blu;
        curr_pix++;
    }
}


// add_element_to_disp
//  function to add a rectangular element to the screen. This element will have
//  a xpos and ypos, width and height, and color. Returns any errors that arise.
//  If the width or height forces part of an element out of the display, the valid
//  parts of the element will still be drawn
DISPLAY_ERR add_element_to_disp(uint8_t xpos, uint8_t ypos,
        uint8_t width, uint8_t height, uint8_t red, uint8_t grn, uint8_t blu)
{
    uint8_t x, y, curr_pix;
    
    // check the positions are in range
    if (xpos < 0 || xpos >= DISP_WIDTH || ypos < 0 || ypos >= DISP_HEIGHT)
    {
        return POS_OUT_OF_RANGE;
    }
    
    // check the width and height
    if (width <= 0 || height <= 0)
    {
        return SIZE_NOT_VALID;
    }
    
    // run through each pixel in the element and set the color of that pixel
    for (y = ypos; y < (ypos+height) && y < DISP_HEIGHT; y++)
    {
        for (x = xpos; x < (xpos+width) && x < DISP_HEIGHT; x++)
        {
            curr_pix = ((y*DISP_WIDTH) + x)*BYTES_PER_PIX;
            frame_buffer[curr_pix] = grn;
            frame_buffer[curr_pix + 1] = red;
            frame_buffer[curr_pix + 2] = blu;
        }
    }
    
    return DISP_SUCCESS;
}


// set_pixel
//  function to set the color of an individual pixel on the display. Returns errors
//  if bad arguments are inputted
DISPLAY_ERR set_pixel(uint8_t xpos, uint8_t ypos,
        uint8_t red, uint8_t grn, uint8_t blu)
{
    uint8_t this_pix;
    
    // check to make sure the inputs are valid
    if (xpos < 0 || xpos >= DISP_WIDTH || ypos < 0 || ypos >= DISP_HEIGHT)
    {
        return POS_OUT_OF_RANGE;
    }
    
    this_pix = ((ypos*DISP_WIDTH) + xpos)*BYTES_PER_PIX;
    
    frame_buffer[this_pix] = grn;
    frame_buffer[this_pix + 1] = red;
    frame_buffer[this_pix + 2] = blu;
    
    return DISP_SUCCESS;
}


// update_display
//  this function will start the process to refresh the display using timer2 and
//  OC1. Takes the contents of the frame buffer and displays them on the appropriate
//  LED on the display. The single output is controlled by OC1 and timer2. Timer2 is
//  set to the cycle of the LEDs (1.5us), and OC1R will determine the high time which
//  is based on if the bit should be high or low. Every time OC1 resets OC1R is set
//  based on the next bit in the sequence. The entire refreshing sequence takes 2.304ms.
//  This function will hang until the refreshing is complete
void update_display(void)
{
    // reset the counter variables
    current_bit = 0x80;
    current_byte = 0;
    
    // get the first bit in the sequence and set OC1RS as needed
    if (frame_buffer[current_byte] & current_bit)
    {
        // the first bit is a 1
        OC1RS = WRITE_1_HIGH_CYCLES;
    }
    else
    {
        // the first bit is a 0
        OC1RS = WRITE_0_HIGH_CYCLES;
    }
    
    // reset to right after the falling edge so OC1 starts low
    TMR2 = WRITE_0_HIGH_CYCLES+1;
    
    // this will hang while outputting the bit sequence, and will determine if the
    // next bit sent will need to be a 0 or a 1. It is written in assembly to be
    // extra speedy
    
    // push all of our working regs to the stack
    asm("push w0");                     // used as a temp reg for movement
    asm("push w1");
    asm("push w2");
    asm("push w3");
    asm("push w4");
    
    // put T2IF(IFS0 bit 7), OC1RS, current_byte and bit in working regs, we will be using them a lot
    WREG1 = IFS0;
    WREG2 = OC1RS;
    WREG3 = current_byte;
    WREG4 = current_bit;
        
    // start timer2 beginning the whole process
    T2CONbits.TON = 1;
    
    asm("START:");
    
    // wait here until timer2 resets (TODO this may not work, I think OC1 resets this)
    while(!_T1IF);
    
    // right shift the current_bit by 1
    asm("mov #0x01, w0");
    asm("asr [w4], w0");
    
    // check if the current_bit underflowed, meaning we need to move on to the next byte
    asm("bra NC, CHECK_END");
    
    // the current_bit underflowed, increase the current_byte and set current_bit to 0x80
    asm("inc [w3], [w3]");
    asm("mov #0x80, w0");
    asm("mov w0, [w4]");
    
    asm("CHECK_END:");
    // check if the current_byte is at the end of the array, meaning we are done
    // sending bits and should jump to the end and turn off the timer2
    if (current_byte >= TOTAL_BYTES)
    {
        asm("bra END");
    }
    
    // check if the next bit in the sequence is a 1 or a 0 and set OC1RS accordingly
    if (frame_buffer[current_byte] & current_bit)
        OC1RS = WRITE_1_HIGH_CYCLES;
    
    else
        OC1RS = WRITE_0_HIGH_CYCLES;
    
    asm("bra START");
    
    asm("END:");
    while(!_T1IF);                  // wait until the last cycle is over (TODO this may not work as OC1 resets the bit)
    T2CONbits.TON = 0;              // turn off the timer, this will also disable OC1
    
    // pop everything off the stack
    asm("pop w4");
    asm("pop w3");
    asm("pop w2");
    asm("pop w1");
    asm("pop w0");
}


// End of neopixel_display_lib.c