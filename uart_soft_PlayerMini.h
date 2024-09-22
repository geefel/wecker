#ifndef UART_SOFT_GFL_H
#define UART_SOFT_GFL_H

#include <stdint.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include "pin.h"

#define RX_PIN_1 									PINDEF(D, 2)  //2
#define TX_PIN_1 									PINDEF(D, 6)  //6
#define BAUD_ 										9600

#define INTERRUPT_TIMER0_ENABLE 	TIMSK0 |= (1 << OCIE0A)
#define INTERRUPT_TIMER0_DISABLE 	TIMSK0 &= ~(1 << OCIE0A)
#define INTERRUPT_INT0_ENABLE 		EIMSK |= (1<< INT0)
#define INTERRUPT_INT0_DISABLE 		EIMSK &= ~(1<< INT0)
#define INT0_RESET 								EIFR |= 1 << INTF0
#define INTERRUPT_ALL_DISABLE 		SREG &= ~0x80
#define INTERRUPT_ALL_ENABLE 			SREG |= 0x80
#define TIMER0_STOP 							TCCR0B &= ~((1 << CS02) || (1 << CS01) || (1 << CS00))
#define TIMER0_START 							TCCR0B |= (1 << CS01)	//prescal = 8
#define TIMER0_RESET 							TCNT0 = 0

void initSoftUart();
void sendDat(uint8_t dat);
void receiveDat(uint8_t erg[10]);
uint16_t ergReceive;
uint16_t mask;
uint8_t ergNr;
volatile uint8_t isTimerInterrupt;
volatile uint8_t isINT0_Interrupt;

#endif //UART_SOFT_GFL_H
