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

/******************************************************************************/
/* Interrupt Routines                                                         */
/******************************************************************************/

/* High-priority service */

#if defined(__XC) || defined(HI_TECH_C)
void interrupt high_isr(void)
#elif defined (__18CXX)
#pragma code high_isr=0x08
#pragma interrupt high_isr
void high_isr(void)
#else
#error "Invalid compiler selection for implemented ISR routines"
#endif
 {

    /* This code stub shows general interrupt handling.  Note that these
    conditional statements are not handled within 3 seperate if blocks.
    Do not use a seperate if block for each interrupt flag to avoid run
    time errors. */

    volatile unsigned char i = 0;

    /* TODO Add High Priority interrupt routine code here. */

    /* Determine which flag generated the interrupt */
    if(PIR1bits.CCP1IF){
        PIR1bits.CCP1IF = 0;
        Timer1OFCountStop = Timer1OfCount;
        done ++;
    }
    else if (PIR1bits.TMR1IF) {
        PIR1bits.TMR1IF = 0; /* Clear Interrupt Flag 1 */
        Timer1OfCount++;
        //if (Timer1OfCount == 8) {
        //    T3CONbits.TMR3ON = 0; //Turn off timer3
        //    T1CONbits.TMR1ON = 0; //Turn off timer1
        //    Done = 1;
            
            //Setup Pin RA0 as output and toggles high/low
            //TRISAbits.RA0 = 0;
        //    LATAbits.LA0 ^=1;
            
        //}
    } else if (PIR2bits.TMR3IF) {
        PIR2bits.TMR3IF = 0; /* Clear Interrupt Flag 2 */
        LastTimer1Value = TMR1;
        LastTimer1OfCount = Timer1OfCount;
        Timer3OfCount++;
    } else {
        i++;
        /* Unhandled interrupts */
    }
}

/* Low-priority interrupt routine */
#if defined(__XC) || defined(HI_TECH_C)
void low_priority interrupt low_isr(void)
#elif defined (__18CXX)
#pragma code low_isr=0x18
#pragma interruptlow low_isr
void low_isr(void)
#else
#error "Invalid compiler selection for implemented ISR routines"
#endif
{

    /* This code stub shows general interrupt handling.  Note that these
    conditional statements are not handled within 3 seperate if blocks.
    Do not use a seperate if block for each interrupt flag to avoid run
    time errors. */


    /* TODO Add Low Priority interrupt routine code here. */

    /* Determine which flag generated the interrupt */
    if (PIR1bits.RCIF) {
        PIR1bits.RCIF = 0;      //Clear Interrupt Flag
        char_received(RCREG);
    } 
    else if (PIR2bits.CCP2IF){
        PIR2bits.CCP2IF = 0;    //Clear Interrupt Flag
        sys_clock ++;
    }
}
