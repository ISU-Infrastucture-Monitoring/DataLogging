#include <msp430.h> 
#include <math.h>
/*
 * main.c
 * 		Main function to use external clock for MSP430FR series launchpad
 *
 */

int main(void) {

	// Stop watchdog timer
    WDTCTL = WDTPW | WDTHOLD;

    //Sets pins J.6 and J.7 to external HF crystal mode
    PJSEL1 = 0x00;
    PJSEL0 = 0x40;

    //Set pin 1.5 to Digital I/O initialized high
    P1DIR = 0x10;
    P1SEL1 = 0x00;
    P1SEL0 = 0x00;

    P1OUT = 0x20;

    //Initializes CLK control register 0 with cskey = 96h
    CSCTL0 = 0x9600;

    //Initializes CLK control register 2 with external crystal set as source for MCLK and SMCLK
    CSCTL1 = 0x55;

    //Initialzes MCLK source divider to /1, and SMCLK source divider to /1 (will be divided in timer pre-scaler)
    CSCTL3 = 0x00;

    //Initializes HF CLK source to 1, sourced from external crystal, turns on SMCLK, sets current draw for external crystal to intermediate value
    CSCTL4 = 0x4C09;

    //Initializes all fault counters to off
    CSCTL5 = 0x00;

    //Initializes conditional module requests for SMCLK and MCLK to on
    CSCTL6 = 0x06;

    //Initializes Timer_A with SMCLK source, pre-scaler /2, continuous mode, cleared counter, interrupts enabled
    TA1CTL = 0x262;

    //Initializes Timer_A counter to 0
    TA1R = 0x00;

    //Enables global iterrupts
    __enable_interrupt();

    while(1);

}

#pragma vector=TIMER1_A1_VECTOR
__interrupt void TIMERA1_ISR (void){
	switch (TA1IV) {
		case 0x0E:
			P1OUT ^= 0x20;
			break;
	}
}

