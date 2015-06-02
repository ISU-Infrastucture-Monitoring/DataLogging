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


volatile unsigned long Timer1OfCount = 0;
volatile unsigned long Timer3OfCount = 0;
volatile unsigned char Done = 0;
static volatile unsigned char value = 128;

/******************************************************************************/
/* User Functions                                                             */
/******************************************************************************/

unsigned char autotune_pot(uint32_t);
uint32_t get_freq();

/* <Initialize variables in user.h and insert code for user algorithms.> */
volatile unsigned long sys_clock=0;
volatile unsigned char begin = 0;

struct TIME{        //Defines time as having seconds and milliseconds as components
    unsigned long seconds;
    unsigned short ms;    
}time;

#define LOW_FREQ_MODE
//#define HIGH_FREQ_MODE
#define NUM_SAMPLES 50000

void InitApp(void)
{
    //Timer 1
    T1CONbits.RD16 = 1;     //16Bit mode timer1
    IPR1bits.TMR1IP = 1;    //Set timer1 overflow interrupt priority high
    PIR1bits.TMR1IF = 0;    //Clears timer1 overflow interrupt flag
    T1CONbits.TMR1ON = 1;   //Turn on timer1
    PIE1bits.TMR1IE = 1;    //Enable timer1 overflow interrupts
    
#ifdef HIGH_FREQ_MODE
    T3CONbits.RD16 = 0;     //16Bit mode
    T3CONbits.T3CKPS = 0;   //Prescaler set to 1:1
    T3CONbits.TMR3CS = 1;   //Set clock to external source
    TRISCbits.RC0 = 1;
    IPR2bits.TMR3IP = 1;    //Set timer1 overflow interrupt priority high
    PIR2bits.TMR3IF = 0;    //Clears timer3 overflow interrupt flag
    PIE2bits.TMR3IE = 1;    //Enable timer3 overflow interrupts
    T3CONbits.TMR3ON = 1;
#endif
#ifdef LOW_FREQ_MODE
    CCP1CONbits.CCP1M = 7;    //Capture every 16th rising edge
    
    TRISCbits.RC2 = 1;          // set CCP pin as input
    
    IPR1bits.CCP1IP = 1;        //Set up capture mode interrupt as high priority
    PIR1bits.CCP1IF = 0;        //Clear capture mode interrupt flag
    PIE1bits.CCP1IE = 1;        //Enable capture mode interrupts
    
    CCP2CONbits.CCP2M = 0xA;    // Compare mode, software interrupt only
#endif
    
    //Timer 3
    //    T3CONbits.RD16 = 0;
    //    T3CONbits.T3CKPS = 3;       // divide by 8: 1MHz
    //    T3CONbits.TMR3CS = 0;
    //    
    //    T3CONbits.T3CCP1 = 1;
    //    T3CONbits.T3CCP2 = 0;
    //    
    //    CCPR2 = 10000;              // 100Hz system tick
    //    
    //    IPR2bits.CCP2IP = 0;        //Set up and enable capture interrupts for capture 2
    //    PIR2bits.CCP2IF = 0;
    //    PIE2bits.CCP2IE = 1;
    
    
    
    RCONbits.IPEN = 1;
    INTCONbits.GIE = 1;     //Enable all unmasked interrupts
    INTCONbits.PEIE = 1;    //Enable all unmasked peripheral interrupts
    
    
    TRISAbits.RA0 = 0;      //Set Pin RA0 to output mode (testing purposes)
    LATAbits.LA0 = 0;       //Set Pin RA0 low
    
    init_serial(on_line_received);
    init_DigPot();
    unsigned int k = 0;
    
    uint32_t freq;
    Write_to_Pot(128);
    unsigned long last_sys_clock = sys_clock;

        Write_to_Pot(109);
        value = 109;

        
        TRISAbits.RA0 = 0;
        
    while(1)
    {
        unsigned long i;
        while(!begin);
        LATAbits.LA0 = 1;   //Set RA0 high to signal beginning of test
        begin = 0;
        sys_clock = 0;
        for(i = 0; i < NUM_SAMPLES; i++)    //Run 500 sample test
        {
            if(1||last_sys_clock != sys_clock)
            {
                last_sys_clock = sys_clock;
                if(1 || (last_sys_clock % 10) == 0)
                {
                    freq = get_freq();
                    printf("%d, %d, %lu\n\r", i, value, freq);  //Print out sample values
                }
            }
        }
        LATAbits.LA0 = 0;   //Set RA0 low to signal end of test
    }
}

volatile uint8_t done;

//volatile union DWORD_UNION Timer1OFCountStop;
volatile uint32_t Timer1OFCountStop;
volatile unsigned long LastTimer1OfCount;
volatile unsigned short LastTimer1Value;
uint32_t get_freq() //Returns frequency as a raw number, before calculations
{
#ifdef LOW_FREQ_MODE
    uint32_t start, stop;
    TMR1 = 0;       //Clear Timer1 value
    Timer1OfCount = 0;  //Clear number of overflows
    done = 0;
    while(done == 0);
    start = CCPR1;      //Start capture mode, set start to current value
    while(done == 1);
    stop = CCPR1;       //Stop capture mode, set stop to current value
    unsigned long now = sys_clock;
    time.seconds = now;// / 100;
    time.ms = 0;//(now % 100)*10;
    uint32_t result = ((Timer1OFCountStop-1)<<16);  //Return resultant raw data
    result += stop;
    result -= start;
    return result;
#endif
#ifdef HIGH_FREQ_MODE
    TMR1 = 0;
    Timer1OfCount = 0;
    TMR3 = 0;
    Timer3OfCount = 0;
    while(Timer3OfCount < 1);
    uint32_t time = LastTimer1Value;
    time += (LastTimer1OfCount << 16);
    return time;
#endif
}


void on_line_received(char* str)    //Handles commands
{
    unsigned char _value = atoi(str);
    if(_value > 0)
    {
        printf("Received %d\n\r", _value);
        Write_to_Pot(_value);
        value = _value;
    }
    else if(str[0] == 'b')      //Begin test command
    {
        begin = 1;
    }
    else if(str[0] == 'c')      //Calibrate Pot command
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
    else if(str[0] == 'r')      //Reset to default condition command
    {
        printf("Reset\n\r");
        #asm
        reset
        #endasm
    }
    else
    {
        printf("Invalid string: \"%s\"\n\r",str);   //Command not found
    }
}

unsigned char autotune_pot(uint32_t setpoint)
{
    Write_to_Pot(value);    //Set pot to initial value
    uint32_t f;
    int i;
    for(i=0;i<16;i++){
    while((f = get_freq()) > setpoint) {    //While frequency is higher than setpoint, step it down
        printf("1: %lu, %u\n\r", f, value);
        value++;
        if(value == 0)
        {
            return -1;
        }
        Write_to_Pot(value);
    }
    while((f = get_freq()) < setpoint) {    //While frequency is lower than setpoint, step it up
        printf("1: %lu, %u\n\r", f, value);
        value--;
        if(value == 255)
        {
            return -1;
        }
        Write_to_Pot(value);
    }
    }
    return 0;
}
