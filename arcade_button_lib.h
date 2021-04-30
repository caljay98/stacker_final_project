/* 
 * File:   arcade_button_lib.h
 * Author: Juyee Vang
 *
 * Created on April 23, 2021, 4:30 PM
 */

#ifndef ARCADE_BUTTON_LIB_H
#define	ARCADE_BUTTON_LIB_H

#include "xc.h"
#include <stdint.h>

#ifdef	__cplusplus
extern "C" {
#endif

void buttonInit(float tempdelta);
void buttonPatternLED(void);
void setButtonLED(int Val);
uint16_t readInputButton();

#ifdef	__cplusplus
}
#endif

#endif	/* ARCADE_BUTTON_LIB_H */

