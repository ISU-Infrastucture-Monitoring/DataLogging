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
#include "fifo.h"
#include "system.h"

struct SERIAL_STRUCT
{
    fifo_t fifo;
    void (*on_line_received)(char* str);
    char string[256];
}serial;

void init_serial(void (*line_received)(char* str))
{
    SPBRGH = 0;
    //SPBRG = 12;             //Sets Baud Rate to 9600
    SPBRG = 138;     //Attempt to fix Baud Rate, set as close to 57600 as possible
    
    //Config Baud rate to 16bit Asynchronous
    TXSTAbits.BRGH = 1;     //Sets Baud Rate to High Speed mode
    BAUDCONbits.BRG16 = 1;  //Sets Baud Rate to 16Bit mode

    TXSTAbits.SYNC = 0;     //Set UART mode to Asynchronous
    RCSTAbits.SPEN = 1;     //Enable Serial port

    TXSTAbits.TXEN = 1;     //Enable transmission
    RCSTAbits.CREN = 1;
    
    TRISCbits.RC6 = 1;      //Set Pin RC6 to TX
    TRISCbits.RC7 = 1;      //Set Pin RC7 to RX

    PIR1bits.TXIF = 0;      //Clear transmit interrupt bit
    PIR1bits.RCIF = 0;      //Clear receive interrupt bit
    
    IPR1bits.RCIP = 0;      //Set RX to low priority
    PIE1bits.RCIE = 1;      //Enable receive interrupt bit
}

void putch(char c)
{
    PIR1bits.TXIF = 0;
    TXREG = c;
    while(PIR1bits.TXIF == 0);
    PIR1bits.TXIF = 0;
}

inline void char_received(char ch)
{
    fifo_push(&serial.fifo, ch);
    if(ch == '\n' || ch == '\r')
    {
        char i;
        char* ptr = serial.string;
        for(i=fifo_pop(&serial.fifo); i; i=fifo_pop(&serial.fifo))
        {
            *ptr = i;
            ptr++;
        }
        *ptr = 0;
        on_line_received(serial.string);
    }
}