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
#define DIGPOT_ADDR 0x50
#define WRITE_OP_CODE 0xA0


void init_DigPot() {
    //Initialize I2C communication
    SSPCON1bits.WCOL = 0;   //Clears Write Collision detect bit
    SSPCON1bits.SSPEN = 1;  //Enables Serial port functionality on RC3 & RC4
    SSPCON1bits.SSPM = 8;   //Sets I2C to Master mode
    
    SSPADD = 19;            //Sets Baud rate to 100KHz
    
    SSPSTATbits.SMP = 1;    //Set Slew Rate control to disabled
    SSPSTATbits.CKE = 0;    //Disable SMBus specific input
    
    TRISCbits.RC3 = 0;      //Sets RC3 pin to output mode for SCL
    TRISCbits.RC4 = 0;      //Sets RC4 pin to output mode for SDA

    //Initialize Digital Potentiometer
    Write_to_Pot(128);
    
}

void Write_to_Pot(unsigned char VAL)
{
    
    SSPCON2bits.SEN = 1;    //Start condition
    
    SSPBUF = DIGPOT_ADDR;   //Load slave ADDR into SSPBUF & begin transmit
    while(!PIR1bits.SSPIF); //Wait until finished
    PIR1bits.SSPIF = 0;     //Clear interrupt flag
    
    SSPBUF = WRITE_OP_CODE; //Load Write code into SSPBUF & begin transmit
    while(!PIR1bits.SSPIF); //Wait until finished
    PIR1bits.SSPIF = 0;     //Clear interrupt flag
    
    SSPBUF = VAL;          //Load value into SSPBUF & begin transmit
    while(!PIR1bits.SSPIF); //Wait until finished
    PIR1bits.SSPIF = 0;     //Clear interrupt flag
    
    SSPCON2bits.PEN = 1;    //Stop condition
    
}