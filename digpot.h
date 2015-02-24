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
#include <stdint.h>

//Slave Address
#define staticAddr 0x05

//Selects pin A1 as high, must be pulled high, and all other pins (A0, A2, A3) pulled low.
#define AddrSelect 0x02

//OP Commands
#define ReadWCRCmd 0x90
#define WriteWCRCmd 0xa0
#define ReadDRCmd 0xb0
#define WriteDRCmd 0xc0
#define XFR_DR_to_WCR 0xd0
#define XFR_WCR_to_DR 0xe0
#define GangXFR_DR_to_WCR 0x01
#define GangXFR_WCR_to_DR 0x02
#define Inc_Dec_Enable 0x02

//Potentiometer and Data Register Address
#define DataRegister 0x00
#define Pot_0 0x00


//Function prototypes
uint8_t DigPot_Write(uint8_t memAddress, uint8_t cmd, uint16_t data);	//Writes to register for Pot0
uint16_t DigPot_Read(uint8_t memAddress);								//Reads register data from Pot0

void DigPot_SetWiper(uint16_t value);									//Sets wiper value in register
void DigPot_Increment();												//Increments wiper value by one step (one step = (1/256)*50k )
void DigPot_Decrement();												//Decrements wiper value by one step (one step = (1/256)*50k )

uint8_t testConnect();													//Tests connection to Digital Potentiometer
void DigPot_init();														//Initiates the Digital Potentiometer, sets value of wiper to lowest (50k)

void TransferWCR_to_DR();												//Transfers WCR data to Data Register
void TransferDR_to_WCR();												//Transfers Data Register Data to WCR

#endif /* DIGPOT_H_ */
