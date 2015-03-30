/******************************************************************************/
/* Files to Include                                                           */
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
#include <stdio.h>
#endif

#include "user.h"
#include "system.h"


volatile unsigned short Timer1OfCount = 0;
volatile unsigned short Timer3OfCount = 0;
volatile unsigned char Done = 0;

/******************************************************************************/
/* User Functions                                                             */
/******************************************************************************/


uint32_t get_freq();

/* <Initialize variables in user.h and insert code for user algorithms.> */

void InitApp(void)
{
    //Timer 1
    T1CONbits.RD16 = 1;     //16Bit mode timer1
    IPR1bits.TMR1IP = 1;    //Set timer1 overflow interrupt priority high
    PIR1bits.TMR1IF = 0;    //Clears timer1 overflow interrupt flag
    PIE1bits.TMR1IE = 1;    //Enable timer1 overflow interrupts

    //Timer 3
#ifndef DEBUG
    T3CONbits.RD16 = 0;     //16Bit mode timer3
    T3CONbits.T3SYNC = 1;   //Sync timer3 with external clock source
    T3CONbits.TMR3CS = 1;   //Set timer3 source to external signal
#else
    T3CONbits.RD16 = 0;
    T3CONbits.T3CKPS = 2;
    T3CONbits.TMR3CS = 0;
#endif

    IPR2bits.TMR3IP = 1;    //Set timer1 overflow interrupt priority high
    PIR2bits.TMR3IF = 0;    //Clears timer1 overflow interrupt flag
    PIE2bits.TMR3IE = 1;    //Enable timer1 overflow interrupts


    INTCONbits.GIE = 1;     //Enable all unmasked interrupts
    INTCONbits.PEIE = 1;    //Enable all unmasked peripheral interrupts
    TRISAbits.RA0 = 0;      //Set Pin RA0 to output mode
    
    init_serial();
    init_DigPot();
    unsigned char k = 0;
    
    uint32_t freq;
    while(1)
    {
        Write_to_Pot(k);
        k += 8;
        //freq = get_freq();
        //printf("%lu\n", freq);
    }
}

uint32_t get_freq()
{
    int i = 0;
    Timer1OfCount = 0;
    Timer3OfCount = 0;

    TMR3H = 0;
    TMR3L = 0;
    TMR1H = 0;
    TMR1L = 0;
    
    Done = 0;

    T1CONbits.TMR1ON = 1;   //Turn on timer1
    T3CONbits.TMR3ON = 1;   //Turn on timer3

    while (!Done)
    {
        // wait to be done
        i++;
    }

    // we are done
    uint32_t result = TMR3L;
    result += TMR3H << 8;
    result += ((uint32_t)Timer3OfCount) << 16;

    return result;
}