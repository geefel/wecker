#include "paint.h"
#include "uart_soft_PlayerMini.h"
#include "tft.h"
#include "uhr.h"
#include "i2c.h"
#include "uart_hard.h"
#include "menue.h"
#include "rtc.h"

#define BAUD 19200

//hier ist Display um 90° gedreht, d.h. x: 0-320; y: 0-240
//Beschriftung des Displays ist rechts: [x,y] = [0,0] ist links oben:
//Pin-Def in tft.h!
int main(void) {
	_delay_ms(200);
	initSoftUart();			//uart_soft_PlayerMini.h
	initForTFT();			//paint.h
	set_I2C_Clock(100000);	//i2c.h für RTC
	
	uhrAufbauen();
	
	return 0;
}
