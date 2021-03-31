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
    POS_OUT_OF_RANGE = 1,
    SIZE_NOT_VALID = 2,
            
    NOT_IMPLEMENTED = 99
} DISPLAY_ERR;

// function prototypes
void display_init();
void set_background(uint8_t red, uint8_t grn, uint8_t blu);
DISPLAY_ERR add_element_to_disp(uint8_t xpos, uint8_t ypos,
        uint8_t width, uint8_t height, uint8_t red, uint8_t grn, uint8_t blu);
DISPLAY_ERR set_pixel(uint8_t xpos, uint8_t ypos,
        uint8_t red, uint8_t grn, uint8_t blu);
void update_display(void);

// general defines
#define OCMODE_OFF 0b0
#define OCMODE_PWM 0b110
#define DISP_WIDTH 8
#define DISP_HEIGHT 8
#define NUM_OF_PIX 64
#define BYTES_PER_PIX 3
#define TOTAL_BYTES 192             // this is so no math needs to be done in asm
#define WRITE_0_HIGH_CYCLES 6       // .375us
#define WRITE_1_HIGH_CYCLES 12      // .75us
#define BIT_SEND_CYCLES 34          // 2.125us (spec says this should be less than 1.5us but it works with timings up to 6.25us and maybe more)

// some helpful macro functions
#define PACK_COLOR(r, g, b) ((((uint32_t)(r) & 0xFF) << 16) | (((uint32_t)(g) & 0xFF) << 8) | ((uint32_t)(b) & 0xFF))
#define UNPACK_RED(packed) (0xFF & ((packed) >> 16))
#define UNPACK_GRN(packed) (0xFF & ((packed) >> 8))
#define UNPACK_BLU(packed) (0xFF & (packed))

#endif


// End of neopixel_display_lib.h