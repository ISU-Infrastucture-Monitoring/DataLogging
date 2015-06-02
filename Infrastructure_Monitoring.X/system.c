/******************************************************************************/
/*Files to Include                                                            */
/******************************************************************************/

#if defined(__XC)
    #include <xc.h>         /* XC8 General Include File */
#elif defined(HI_TECH_C)
    #include <htc.h>        /* HiTech General Include File */
#elif defined(__18CXX)
    #include <p18cxxx.h>    /* C18 General Include File */
#endif

#if defined(__XC) || defined(HI_TECH_C)

#include <stdint.h>         /* For uint8_t definition */
#include <stdbool.h>        /* For true/false definition */

#endif

#include "system.h"

void ConfigureOscillator(void)
{
    OSCCONbits.IRCF = 7;    //Set Internal oscillator to 8MHz
    OSCCONbits.SCS = 0;     //Source clock from primary oscillator
    
    OSCTUNEbits.PLLEN = 1;  //Enables PLL multiplier of 4x clock to achieve a 32MHz clock
    
#ifndef DEBUG
    while(!OSCCONbits.IOFS);
#endif
}
