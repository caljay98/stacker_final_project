// final_project_main.h
//  header file for final_project_main.c


// This is a guard condition so that contents of this file are not included
// more than once.  
#ifndef FINAL_PROJECT_MAIN_H
#define	FINAL_PROJECT_MAIN_H

#include <xc.h>
#include <stdint.h>

// function prototypes
int main(void);
void setup(void);
void loop(void);
void delay(uint16_t wait_time);

// Some nice macro functions for setting and resetting bits
#define SET_BIT(val, bit) ((val) | (1 << (bit)))
#define RESET_BIT(val, bit) ((val) & ~(1 << (bit)))

// general defines
// TODO


// some nice macro functions
#define PACK(r, g, b) ((((r) & 0xFF) << 16) | (((g) & 0xFF) << 8) | ((b) & 0xFF))
#define UNPACK_RED(packed) (0xFF & ((packed) >> 16))
#define UNPACK_GRN(packed) (0xFF & ((packed) >> 8))
#define UNPACK_BLU(packed) (0xFF & (packed))

#endif


// End of final_project_main.h