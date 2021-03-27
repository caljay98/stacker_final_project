// neopixel_display_lib.h
//  header file for neopixel_display_lib.c


// This is a guard condition so that contents of this file are not included
// more than once.  
#ifndef NEOPIXEL_DISPLAY_LIB_H
#define	NEOPIXEL_DISPLAY_LIB_H

#include <xc.h>
#include <stdint.h>

// structs / enums
typedef enum
{
    DISP_SUCCESS = 0,
    
    NOT_IMPLEMENTED = -99
} DISPLAY_ERR;

// function prototypes
// TODO

// general defines
#define DISP_WIDTH 8
#define DISP_HEIGHT 8
#define WRTIE_0_HIGH_CYCLES 6       // .375us
#define WRITE_1_HIGH_CYCLES 12      // .75us

#endif


// End of neopixel_display_lib.h