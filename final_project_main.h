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

#endif


// End of final_project_main.h