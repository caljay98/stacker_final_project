// neopixel_display_lib.c
//  TODO DOCS
// uses timer2 and OC1


// includes
#include "neopixel_display_lib.h"


// global variables
volatile uint32_t* frame_buffer;        // the frame buffer read from. Must be 64 elements long
volatile uint8_t current_led = 0;       // counter for each led
volatile uint8_t current_bit = 0;       // counter for each bit in the color code


// display_init
//  This function will init the display and all of the needed interrupts. Parameters
//  for the function are still TBD. Should always succeed
void display_init(volatile uint32_t* buffer)
{
    frame_buffer = buffer;
    
    // TODO init all of the hardware peripherals
}


// set_frame_buffer
//  This function will change the frame_buffer pointer to the function parameter
//  frame_buffer must be 8x8. This function has no return as it cannot fail (except
//  for a segfault)
void set_frame_buffer(volatile uint32_t* new_buffer)
{
    frame_buffer = new_buffer;
}


// add_element_to_disp
//  function to add a rectangular element to the screen. This element will have
//  a xpos and ypos, width and height, and color. Returns any errors that arise
DISPLAY_ERR add_element_to_disp(uint8_t xpos, uint8_t ypos,
        uint8_t width, uint8_t height, uint32_t color)
{
    // TODO
    
    return NOT_IMPLEMENTED;
}


// set_pixel
//  function to set the color of an individual pixel on the display. Returns errors
//  if bad arguments are inputted
DISPLAY_ERR set_pixel(uint8_t xpos, uint8_t ypos, uint32_t color)
{
    // check to make sure the inputs are valid
    // TODO
    
    frame_buffer[(ypos*DISP_WIDTH) + xpos] = color;
    
    return NOT_IMPLEMENTED;
}


// update_display
//  this function will start the process to refresh the display using timer2 and
//  OC1. Takes the contents of the frame buffer and displays them on the appropriate
//  LED on the display. The single output is controlled by OC1 and timer2. Timer2 is
//  set to the cycle of the LEDs (1.5us), and OC1R will determine the high time which
//  is based on if the bit should be high or low. Every time OC1 resets OC1R is set
//  based on the next bit in the sequenceThe entire refreshing sequence takes 2.304ms.
//  This function will hang until the refreshing is complete
void update_display(void)
{
    // start up the peripherals as needed
    // TODO
    
    // get the first bit in the sequence and set OC1RS as needed
    // TODO
    
    // start OC1R and timer2
    // TODO
    
    // loop for each bit in the sequence
    // TODO
    
    // disable the peripherals until next update sequence
    // TODO
}


// End of neopixel_display_lib.c