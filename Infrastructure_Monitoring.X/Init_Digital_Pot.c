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
   // TRISCbits.RC3 = 0;      //Sets RC3 pin to output mode for SCL
   // TRISCbits.RC4 = 1;      //Sets RC4 pin to output mode for SDA
      TRISC = 0x00;
      PORTC = 0x00;
   // LATCbits.LATC3 = 1;
   // LATCbits.LATC4 = 1;
    
    OpenI2C(MASTER, SLEW_OFF);
    
    //Initialize I2C communication
//    SSPCON1bits.WCOL = 0;   //Clears Write Collision detect bit
//    SSPCON1bits.SSPM = 8;   //Sets I2C to Master mode
//    SSPCON1bits.SSPOV = 0;  //Clears Overflow Indicator bits
//    
    SSPADD = 0x18;            //Sets Baud rate to 100KHz
//    
//    SSPSTATbits.SMP = 1;    //Set Slew Rate control to disabled
//    SSPSTATbits.CKE = 0;    //Disable SMBus specific input
//    
//    SSPCON1bits.SSPEN = 1;  //Enables Serial port functionality on RC3 & RC4
    
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
    
    //rv = putsI2C(data);
    
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
   // CloseI2C();
    
    
//    
//    SSPCON2bits.SEN = 1;    //Start condition
//    while(SSPCON2bits.SEN);
//    SSPBUF = DIGPOT_ADDR;   //Load slave ADDR into SSPBUF & begin transmit
//    while(SSPSTATbits.RW); //Wait until finished
//    PIR1bits.SSPIF = 0;     //Clear interrupt flag
//    
//    SSPBUF = WRITE_OP_CODE; //Load Write code into SSPBUF & begin transmit
//    while(SSPSTATbits.RW); //Wait until finished
//    PIR1bits.SSPIF = 0;     //Clear interrupt flag
//    
//    SSPBUF = VAL;          //Load value into SSPBUF & begin transmit
//    while(SSPSTATbits.RW); //Wait until finished
//    PIR1bits.SSPIF = 0;     //Clear interrupt flag
//    
//    SSPCON2bits.PEN = 1;    //Stop condition
    
}