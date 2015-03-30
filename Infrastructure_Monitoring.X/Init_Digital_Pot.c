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
#include <i2c.h>            //I2C library
#define DIGPOT_ADDR 0x50
#define WRITE_OP_CODE 0xA0


void init_DigPot(void) {

    TRISCbits.RC3 = 0;
    TRISCbits.RC4 = 0;
    PORTCbits.RC3 = 0;
    PORTCbits.RC4 = 0;

    OpenI2C(MASTER, SLEW_OFF);

    SSPADD = 0x18;            //Sets Baud rate to 100KHz

    //Initialize Digital Potentiometer
    Write_to_Pot(128);
    
}

void Write_to_Pot(unsigned char VAL)
{
    char i = 0;
    char rv = 0;
    unsigned char data[3], tmp;
    char status;
    
    data[i++] = DIGPOT_ADDR;
    data[i++] = WRITE_OP_CODE;
    data[i++] = VAL;
        
    IdleI2C();
    StartI2C();     //Generates Start condition
    for(i=0;i<3;i++)
    {
        do{
            status = WriteI2C(data[i]);
            IdleI2C();
            if(status == -1)
            {
                tmp = SSPBUF;
                SSPCON1bits.WCOL = 0;
            }
        } while(status != 0);
    }
    IdleI2C();
    StopI2C();      //Generates Stop condition
    
}