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
#include <stdlib.h>
#endif

#include "user.h"
#include "system.h"


volatile unsigned short Timer1OfCount = 0;
volatile unsigned short Timer3OfCount = 0;
volatile unsigned char Done = 0;

/******************************************************************************/
/* User Functions                                                             */
/******************************************************************************/

unsigned char autotune_pot(uint32_t);
uint32_t get_freq();

/* <Initialize variables in user.h and insert code for user algorithms.> */
volatile unsigned long sys_clock=0;

struct TIME{
    unsigned long seconds;
    unsigned short ms;    
}time;

void InitApp(void)
{
    //Timer 1
    T1CONbits.RD16 = 1;     //16Bit mode timer1
    IPR1bits.TMR1IP = 1;    //Set timer1 overflow interrupt priority high
    PIR1bits.TMR1IF = 0;    //Clears timer1 overflow interrupt flag
    T1CONbits.TMR1ON = 1;   //Turn on timer1
    PIE1bits.TMR1IE = 1;    //Enable timer1 overflow interrupts

    //Timer 3
//#ifndef DEBUG
//    T3CONbits.RD16 = 0;     //16Bit mode timer3
//    T3CONbits.T3SYNC = 1;   //Sync timer3 with external clock source
//    T3CONbits.TMR3CS = 1;   //Set timer3 source to external signal
//#else
//    T3CONbits.RD16 = 0;     //16Bit mode
//    T3CONbits.T3CKPS = 2;   //Prescaler set to 1:1
//    T3CONbits.TMR3CS = 0;   //Set clock to external source
//#endif

//    IPR2bits.TMR3IP = 1;    //Set timer1 overflow interrupt priority high
//    PIR2bits.TMR3IF = 0;    //Clears timer3 overflow interrupt flag
//    PIE2bits.TMR3IE = 1;    //Enable timer3 overflow interrupts

    CCP1CONbits.CCP1M = 7;    //Capture every 16th rising edge
    
    TRISCbits.RC2 = 1;      // set CCP pin as input
    
    IPR1bits.CCP1IP = 1;
    PIR1bits.CCP1IF = 0;
    PIE1bits.CCP1IE = 1;
    
    
    CCP2CONbits.CCP2M = 0xA;    // Compare mode, software interrupt only
    
    T3CONbits.RD16 = 0;
    T3CONbits.T3CKPS = 3;
    T3CONbits.TMR3CS = 0;
    
    T3CONbits.T3CCP1 = 1;
    T3CONbits.T3CCP2 = 0;
    
    CCPR2 = 62500;
    
    IPR2bits.CCP2IP = 0;
    PIR2bits.CCP2IF = 0;
    PIE2bits.CCP2IE = 1;
    
    T3CONbits.TMR3ON = 1;
    
    RCONbits.IPEN = 1;
    INTCONbits.GIE = 1;     //Enable all unmasked interrupts
    INTCONbits.PEIE = 1;    //Enable all unmasked peripheral interrupts
    
    
    TRISAbits.RA0 = 0;      //Set Pin RA0 to output mode
    
    init_serial(on_line_received);
    init_DigPot();
    unsigned int k = 0;
    
    uint32_t freq;
    Write_to_Pot(128);
    unsigned long last_sys_clock = sys_clock;
    if(autotune_pot(32000))
    {
        printf("Calibration freq out of range\n\r");
        Write_to_Pot(128);
    }
    while(1)
    {
        if(last_sys_clock != sys_clock)
        {
            last_sys_clock = sys_clock;
            if((sys_clock % 2) == 0)
            {
                freq = get_freq();
                printf("%lu.%03d, %lu\n\r", time.seconds, time.ms, freq);
            }
        }
    }
}

volatile uint8_t done;

//volatile union DWORD_UNION Timer1OfCountStop;
volatile uint32_t Timer1OfCountStop;

uint32_t get_freq()
{
    uint32_t start, stop;
    TMR1 = 0;
    Timer1OfCount = 0;
    done = 0;
    while(done == 0);
    start = CCPR1;
    while(done == 1);
    stop = CCPR1;
    unsigned long now = sys_clock;
    time.seconds = now >> 2;
    time.ms = (now&0x3)*250;
    return Timer1OfCountStop + stop - start;
}


void on_line_received(char* str)
{
    unsigned char value = atoi(str);
    if(value > 0)
    {
        printf("Received %d\n\r", value);
        Write_to_Pot(value);
    }
    else if(str[0] == 'c')
    {
        uint32_t sp = atol(str+1);
        if(sp == 0)
            sp = 32000;
        
        printf("Calibrate to %ul\n\r", sp);
        if(autotune_pot(sp))
        {
            printf("Calibration frequency out of range\n\r");
            Write_to_Pot(128);
        }
    }
    else if(str[0] == 'r')
    {
	printf("Reset\n\r");
	#asm
	reset
	#endasm
    }
    else
    {
        printf("Invalid string: \"%s\"\n\r",str);
    }
}

unsigned char autotune_pot(uint32_t setpoint)
{
    static unsigned char value = 128;
    Write_to_Pot(value);
    uint32_t f;
    while((f = get_freq()) > setpoint) {
        //printf("1: %lu, %u\n\r", f, value);
        value--;
        if(value == 0)
        {
            return -1;
        }
        Write_to_Pot(value);
    }
    while((f = get_freq()) < setpoint) {
        //printf("1: %lu, %u\n\r", f, value);
        value ++;
        if(value == 255)
        {
            return -1;
        }
        Write_to_Pot(value);
    }
    return 0;
}
