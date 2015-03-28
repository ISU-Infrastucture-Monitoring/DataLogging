#include <msp430.h>
/*
 * ClockSetup430G.c
 *	Serves similar purpose to code previously written for MSP430FR5969,
 *	counts up until overflow, then toggles a pin high/low.
 *
 *  Created on: Mar 24, 2015
 *      Author: Sebastian A Roe
 */

int main(void) {

	// Stop watchdog timer
	WDTCTL = WDTPW | WDTHOLD;

	//Initialize Pin 1.5 to output direction
	P1DIR = 0x20;
	P1SEL = 0x00;
	P1SEL2 = 0x00;

	//Initialize Pin 1.5 to High
	P1OUT = 0x20;

	//Initializes TimerA clock to use SMCLK as source, with no pre-scaler
	TACTL = 0x222;
	//Sets initial counter to 0
	TAR = 0x00;

	__enable_interrupt();
	while(TAR < 0x200);
	P1OUT = 0x00;
	while(1);
}

#pragma vector = TIMER0_A1_VECTOR
__interrupt void TIMERA1_ISR (void) {
	switch(TAIV) {
		case 0x0A:
			P1OUT ^= 0x20;
			break;
		default:
			P1OUT ^= 0x20;
			break;
	}

}
