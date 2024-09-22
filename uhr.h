#ifndef UHR_H
#define UHR_H
#include <stdio.h>
//#include "uart_print.h"
	
struct wecker_ {
	uint8_t tag;				// Day: 0 = Sonntag, 1 = Montag, ... 6 = Samstag
	uint8_t datumDD;    // Day 1-31
	uint8_t datumMM;    // Month 1-12
	uint8_t datumYY;		// Year 0 - 99
	uint8_t uhrHH;			// Hours 0-23
	uint8_t uhrMM;   		// Minutes 0-59
	uint8_t uhrSec;
	uint8_t weckerHH;
	uint8_t weckerHHSchlum;
	uint8_t weckerMM;
	uint8_t weckerMMSchlum;
	uint8_t weckerZeitSchlummer;
	uint8_t musikTitel;
	uint8_t musikLaut;
	int8_t dimBacklight;
	uint8_t musikEQ;
	uint8_t weckerStatus; //wecker aus: 0, an: 1
	
};

void uhrAufbauen();

#endif //UHR_H
