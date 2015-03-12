#include <msp430.h> 
//#include "digpot.h"
#include <math.h>

//Address for the slave Digital Potentiometer
#define ADDR 0x50
//Hex code to send write command to Digital Potentiometer
#define WRITE_OP_CODE 0xA0

/*
 * main.c
 * 		By Sebastian Roe & Jeramie Vens
 * 		Code to control the CAT5259YI Digital Potentiometer using the MSP430.
 */
void main(void) {
    WDTCTL = WDTPW | WDTHOLD;			// Stop watchdog timer
    /* Sets pins as follows:
     * 		P1.4: SMCLK ~ 1MHz
     * 		P1.6: SDA --Serial Data Read/Write Bus
     * 		P1.7: SCL ~ 200KHz
     */
    P1DIR |= 0xD0;
    P1SEL |= 0xD0;
    P1SEL2 |= 0xC0;

    /* Initializes control register 0 with
     *		Slave address mode = 7-Bit (Bit 6)
     *		Single master mode (Bit 5)
     *		Master I2C mode (Bit 3)
     *		I2C mode enabled (Bit 2-1)
     *		Synchronous mode on (Bit 0)
     */
    UCB0CTL0 = 0x0F;

    //Sets Baud Rate to 5
    UCB0BR0 = 0x05;
    UCB0BR1 = 0x00;
    //Output clock is 200KHz

    //Initializes status register
    UCB0STAT = 0x00;

    IFG2 &= ~(1 << 3);

    /* Initialized control register 1 with
     * 		Clock source = SMCLK (Bits 7-6)
     * 		Transmitter mode on (Bit 4)
     * 		Acknowledge mode = non-NACK (Bit 3)
     * 		No STOP condition (Bit 2)
     * 		No START condition (Bit 1)
     * 		Software reset mode enabled (Bit 0)
     */
    UCB0CTL1 = 0x90;
    UCB0CTL1 &= 0xFE;

    UCB0I2CSA = ADDR >> 1;				// Set Slave address of Digital Potentiometer in register

    int j;
    int i;
    /*
     * Continuous loop, raises value of digital pot by
     * 		(1/256)*(100K Ohms) ~391 Ohms/Step
     */
	while(1)
	{
		for(i=0; i<255; ++i)
		{
			UCB0CTL1 |= 0x02;			// Send ADDR to volatile memory
			while(!(IFG2 & 0x08));
										// Wait for Acknowledge

			UCB0TXBUF = WRITE_OP_CODE;	// Sends write op code to volatile memory
			while(!(IFG2 & 0x08));
										// Wait for Acknowledge

			UCB0TXBUF = i;				// Send i (Step point, 0-255)
			while(!(IFG2 & 0x08));
										// Wait for Acknowledge

			j=0;						// Wait for a little bit
		}

	}

}
