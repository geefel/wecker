#ifndef RTC_H
#define RTC_H

#include <stdint.h>
#include "uhr.h"

#define DS1307_ADDRESS 0x68 ///< I2C address for DS1307
#define DS1307_CONTROL 0x07 ///< Control register
#define DS1307_NVRAM   0x08   ///< Start of RAM registers - 56 bytes, 0x08 to 0x3f

#define SQ_ON  			0b00010000
#define SQ_OFF 			0b00000000
#define SQ_PIN_HIGH 	0b10000000

/*
 * Startet Oszilator mit der Frequenz freq.
 * Freq: 0 = 1 Hz, 1 = 4096 Hz, 2 = 8192 Hz, 3 = 32768 Hz 
 */
void setSquarOn(int8_t freq);
void setSquarOff();
void setSquarPinHigh();

void getNow(struct wecker_ *data);
uint8_t getSecondFast();
void setClock(struct wecker_ *data);

#endif //RTC_H
