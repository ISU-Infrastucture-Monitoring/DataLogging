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

void init_serial()
{
    SPBRGH = 0;
    SPBRG = 68;             //Sets Baud Rate to 115942
    TXSTAbits.BRGH = 1;     //Sets Baud Rate to High Speed mode
    BAUDCONbits.BRG16 = 1;  //Sets Baud Rate to 16Bit mode

    TXSTAbits.SYNC = 0;     //Set UART mode to Asynchronous
    RCSTAbits.SPEN = 1;     //Enable Serial port

    TXSTAbits.TXEN = 1;     //Enable transmission
    
    TRISCbits.RC6 = 1;      //Set Pin RC6 to TX
    TRISCbits.RC7 = 1;      //Set Pin RC7 to RX

}

void putch(char c)
{
    PIR1bits.TXIF = 0;
    TXREG = c;
    while(PIR1bits.TXIF == 0);
    PIR1bits.TXIF = 0;
}