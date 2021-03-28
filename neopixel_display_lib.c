// neopixel_display_lib.c
//  TODO DOCS
// uses timer2 and OC1


// includes
#include "neopixel_display_lib.h"


// global variables
volatile uint32_t frame_buffer[NUM_OF_PIX];     // the frame buffer read from. Must be 64 elements long
volatile uint8_t current_led = 0;               // counter for each led
volatile uint8_t current_bit = 0;               // counter for each bit in the color code


// display_init
//  This function will init the display and all of the needed interrupts. Parameters
//  for the function are still TBD. Should always succeed
void display_init()
{
    // TODO init all of the hardware peripherals
}


// set_background
//  function to set the background color of the display. Call this before adding
//  elements to the display as those will replace the background pixels in that
//  spot
void set_background(uint32_t color)
{
    uint8_t x, y;
    
    for (y = 0; y < DISP_HEIGHT; y++)
    {
        for (x = 0; x < DISP_WIDTH; x++)
        {
            frame_buffer[(y*DISP_WIDTH) + x] = color;
        }
    }
}


// add_element_to_disp
//  function to add a rectangular element to the screen. This element will have
//  a xpos and ypos, width and height, and color. Returns any errors that arise.
//  If the width or height forces part of an element out of the display, the valid
//  parts of the element will still be drawn
DISPLAY_ERR add_element_to_disp(uint8_t xpos, uint8_t ypos,
        uint8_t width, uint8_t height, uint32_t color)
{
    uint8_t x, y;
    
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
            frame_buffer[(y*DISP_WIDTH) + x] = color;
        }
    }
    
    return DISP_SUCCESS;
}


// set_pixel
//  function to set the color of an individual pixel on the display. Returns errors
//  if bad arguments are inputted
DISPLAY_ERR set_pixel(uint8_t xpos, uint8_t ypos, uint32_t color)
{
    // check to make sure the inputs are valid
    if (xpos < 0 || xpos >= DISP_WIDTH || ypos < 0 || ypos >= DISP_HEIGHT)
    {
        return POS_OUT_OF_RANGE;
    }
    
    frame_buffer[(ypos*DISP_WIDTH) + xpos] = color;
    
    return DISP_SUCCESS;
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