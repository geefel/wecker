/*
 * Dieser uart_soft ist für den DFPlayerMini gebaut und nicht allgemein benutzbar!
 * Es wird TIMER0 und INT0 benutzt
 */

#include "uart_soft_PlayerMini.h"

const uint8_t ocrVal = 207;

void initSoftUart() 
{
	INTERRUPT_ALL_DISABLE;
	INTERRUPT_INT0_DISABLE;
	INTERRUPT_TIMER0_DISABLE;
	
	//Ports
	setOutput(TX_PIN_1);
	setInput(RX_PIN_1);
	setPin(TX_PIN_1);
	//setPin(RX_PIN_1);
  
	// Timer0
	TCCR0A = 0x00;    //Init.	
	TCCR0B = 0x00;    //Init. Vorteiler
	TCCR0A |= (1 << WGM01);			// Timer in CTC mode. Interrupt bei erreichen von TCNT0 == 0CR0A
	
	//Vergleichsregister
	//ocrVal = (int8_t)((uint32_t)F_CPU / (uint32_t)BAUD_ / getPrescaleVal((uint32_t)BAUD_)) - 1;
	OCR0A = ocrVal;
	
	//External INT0 interrupt
	EICRA = 0x00;             // Init.
	EICRA |= (1 << ISC01);    // Interrupt sense control: Eine fallende Flanke am INT0-Pin löst Interrupt aus 

	isTimerInterrupt = 0;
	isINT0_Interrupt = 0;

	INTERRUPT_ALL_ENABLE;
}

//Es wird TIMER0 an/aus gemacht, sonst keiner (auch global)
void sendDat(uint8_t dat) 
{
	uint16_t maske = 0x01;
	uint16_t d = (dat << 1) + 512;
	
	TIMER0_RESET;
	INTERRUPT_TIMER0_ENABLE;
	isTimerInterrupt = 0;
	TIMER0_START;

	for (uint8_t i = 0; i < 10; i++) {	
		if (d & maske) 
			setPin(TX_PIN_1);
		else
			clrPin(TX_PIN_1);
		maske <<= 1;
		isTimerInterrupt = 0;
		while (!isTimerInterrupt) {__asm__ volatile ("nop" ::);}
	}
	TIMER0_STOP;
}

void receiveDat(uint8_t *erg) {
	do {
		while (!isINT0_Interrupt) {__asm__ volatile ("nop" ::);}	//auf fallende Flanke warten
		/*0,5 Takte warten bis Mitte START erreicht */
		isTimerInterrupt = 0;
		OCR0A = ocrVal >> 1; 		//Vergleichsregister
		TIMER0_RESET;
		TIMER0_START;
		
		while (!isTimerInterrupt) {__asm__ volatile ("nop" ::);}
		TIMER0_STOP;
		isTimerInterrupt = 0;
		OCR0A = ocrVal;
		TIMER0_RESET;
		TIMER0_START;
		
		/*Daten-bits einsammeln*/
		ergReceive = 0;
		mask = 0x01;
		for (uint8_t i = 0; i < 8; ++i) 
		{
			while (!isTimerInterrupt) {__asm__ volatile ("nop" ::);}
			if(getPin(RX_PIN_1))
				ergReceive |= mask;
			mask <<= 1;
			isTimerInterrupt = 0;
		}

		erg[ergNr] = ergReceive;
		isINT0_Interrupt = 0;
		INT0_RESET;		//Interrupt-Flag löschen
	} while (++ergNr < 10);
}

//Timer0 interrupt service routine.
ISR(TIMER0_COMPA_vect) 
{
	isTimerInterrupt = 1;
}
//INT0 interrupr service routine.
ISR(INT0_vect) 
{
	isINT0_Interrupt = 1;
	
}
