/*
 * digpot.h
 *  Created on: Feb 22, 2015
 *      Author: Sebastian
 *		This code is to control the CAT5259YI Digital Potentiometer using a 
 *		TI MSP430 Launchpad.
 *		This header file contains all necessary prototypes to adjust the potentiometer's 
 *		value between 50k Ohms and 100k Ohms, as well as the necessary constant definitions.
 */

#ifndef DIGPOT_H_
#define DIGPOT_H_
typedef unsigned char uint8_t;
typedef unsigned char uint16_t;

//#define DigPot_Address


//Function prototypes
uint8_t DigPot_Write(uint8_t memAddress, uint8_t cmd, uint16_t data);	//Writes to register for Pot0
uint16_t DigPot_Read(uint8_t memAddress);								//Reads register data from Pot0

void DigPot_SetWiper(uint16_t value);									//Sets wiper value in register
void DigPot_Increment();												//Increments wiper value by one step (one step = (1/256)*50k )
void DigPot_Decrement();												//Decrements wiper value by one step (one step = (1/256)*50k )

uint8_t testConnect();													//Tests connection to Digital Potentiometer
void DigPot_init();														//Initiates the Digital Potentiometer, sets value of wiper to lowest (50k)


#endif /* DIGPOT_H_ */
