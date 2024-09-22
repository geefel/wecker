#include <stdio.h>
#include "uhr.h"
#include "rtc.h"
#include "tft.h"
#include "paint.h"
#include "segment14Font.h"
#include "menue.h"
#include "uart_print.h"
#include "wavPlayer.h"
#include <util/delay.h>

void drawTime(struct wecker_ *data);
void drawDatumZeile(struct wecker_ *data);
void testAlarm(struct wecker_ *data);
void weckerAlarmAlarm();
void weckerAlarmAn();
void weckerSchlummernAn();
void weckerAlarmAus(struct wecker_ *data);
uint8_t initStruct();

enum  wStatus
{
	weckerAus,
	weckerAn,
	weckerAlarm,
	weckerSchlummern
};

struct wecker_ data;
 
const char *dayName[] = {"SONNTAG   ", "MONTAG    ", "DIENSTAG  ", "MITTWOCH  ", "DONNERSTAG", "FREITAG   ", "SAMSTAG    "};

uint8_t initStruct() {
	data.datumDD = 1;
	data.datumMM = 1;
	data.datumYY = 15;
	data.weckerHH = 0;
	data.weckerMM = 1;
	data.weckerStatus = weckerAus;
	data.weckerZeitSchlummer = 5;	//in Minuten
	data.musikTitel = 1;
	data.musikLaut = 5;
	data.musikEQ = 1;
	data.dimBacklight = 0;
	return 1;
}



char numToChar(uint8_t num) {
	char ch = 'X';
	switch (num) 
	{
		case 0: ch = '0'; break;
		case 1: ch = '1'; break;
		case 2: ch = '2'; break;
		case 3: ch = '3'; break;
		case 4: ch = '4'; break;
		case 5: ch = '5'; break;
		case 6: ch = '6'; break;
		case 7: ch = '7'; break;
		case 8: ch = '8'; break;
		case 9: ch = '9'; break;
		default: break;
	};
	return ch;
}

void uhrAufbauen() {	
	/* muss weg */
	//struct wecker_ data;
	data.uhrSec = 10;
	data.uhrMM = 18;
	data.uhrHH = 12;
	data.datumDD = 28;
	data.datumMM = 10;
	data.datumYY = 23;
	setClock(&data);
	/* muss weg ENDE */
	
	uint8_t oldSec = 0;
	
	static uint8_t initStr = 0;
	if (!initStr)	//wird nur ein einziges mal ausgeführt
		initStr = initStruct();
		
	set_TFT_rotation(90);
	setBacklightDim(9);	//data.dimBacklight
	fillScreen(BACKCOLOR);
	
	getNow(&data);
  drawDatumZeile(&data);
  
  drawText(50, 40, 1, DEFCOLOR, BACKCOLOR, "HELL");
  drawText(175, 40, 1, DEFCOLOR, BACKCOLOR, "DUNKEL");
  
  drawZiffer(10, 60, 8, DEFCOLOR, BACKCOLOR, data.uhrHH / 10);
  drawZiffer(68, 60, 8, DEFCOLOR, BACKCOLOR, data.uhrHH % 10);
  drawDoublePoint(125, 60, 8, DEFCOLOR);
  drawZiffer(142, 60, 8, DEFCOLOR, BACKCOLOR, data.uhrMM / 10);
  drawZiffer(200, 60, 8, DEFCOLOR, BACKCOLOR, data.uhrMM % 10);
  drawZiffer(260, 60 + 50, 3, DEFCOLOR, BACKCOLOR, data.uhrSec / 10);
  drawZiffer(285, 60 + 50, 3, DEFCOLOR, BACKCOLOR, data.uhrSec % 10);
  drawPfeil(280, 170, DEFCOLOR, 3);
  
  if (data.weckerStatus == weckerAus)
		weckerAlarmAus(&data);
	else
		weckerAlarmAn();  
	
  uint16_t xTouch, yTouch, zTouch;
  _delay_ms(50);
  for (;;) 
  {
    data.uhrSec = getSecondFast();  //Sekunden separat holen, nicht mit DateTime now = rtc.now();
    if (oldSec != data.uhrSec) 
    {
      oldSec = data.uhrSec;
      drawTime(&data);
    }
    
    getxy(&xTouch, &yTouch, &zTouch);
    if (zTouch > TOUCH_THRES) {	//TOUCH_THRES in tft.h
			if(data.weckerStatus < weckerAlarm) {
				if (yTouch < 120) 				//Bildschirmhelligkeit
				{
					if ((xTouch > 10) && (xTouch < 140))
					{	
						data.dimBacklight++;
						setBacklightDim(data.dimBacklight);
					}
					else if ((xTouch > 142) && (xTouch < 270))
					{
						data.dimBacklight--;
						setBacklightDim(data.dimBacklight);
					}
					_delay_ms(100); 
				}
				if ((xTouch > 280) && (xTouch < 320) && (yTouch > 170) && (yTouch < 240)) {	//Menü
					menueAufbauen(&data);
				}
			}
      
      else {										//nur wenn Wecker läutet
				if ((xTouch > 85) && (xTouch < 280)/* && (yTouch > 140) && (yTouch > 240)*/) 
				{
					switch (data.weckerStatus) 
					{
						case weckerAlarm: weckerSchlummernAn(&data); break;
						case weckerSchlummern: weckerAlarmAus(&data); break;
					}
				}
			}
		}
    //_delay_ms(10);
  }
}

void drawTime(struct wecker_ *data) {
  drawZiffer(285, 60 + 50, 3, DEFCOLOR, BACKCOLOR, data->uhrSec % 10);
  if (data->uhrSec % 10 == 0) {
    drawZiffer(260, 60 + 50, 3, DEFCOLOR, BACKCOLOR, data->uhrSec / 10);
    if (data->uhrSec / 10 == 0) {
      getNow(data);   	//wieder großes Zeit-Paket abholen, jede Minute
      drawZiffer(200, 60, 8, DEFCOLOR, BACKCOLOR, data->uhrMM % 10);
			testAlarm(data);	//sollte der Wecker klingeln?
      if (data->uhrMM % 10 == 0) {
        drawZiffer(142, 60, 8, DEFCOLOR, BACKCOLOR, data->uhrMM / 10);
        if (data->uhrMM / 10 == 0) {
          drawZiffer(68, 60, 8, DEFCOLOR, BACKCOLOR, data->uhrHH % 10);
          if (data->uhrHH % 10 == 0) {
            drawZiffer(10, 60, 8, DEFCOLOR, BACKCOLOR, data->uhrHH / 10);
            if (data->uhrHH == 0) {
              drawDatumZeile(data);
            }
          }
        }
      }
    }
  }
}

void testAlarm(struct wecker_ *data) {
	if (data->weckerStatus == weckerAn){
		if ((data->weckerHH == data->uhrHH) && (data->weckerMM == data->uhrMM))
				weckerAlarmAlarm(data);
	}
	if (data->weckerStatus == weckerSchlummern){
		if ((data->weckerHHSchlum == data->uhrHH) && (data->weckerMMSchlum == data->uhrMM))
				weckerAlarmAlarm(data);
	}
}

//Alarm und Schlummern ist aus/gestoppt
void weckerAlarmAus(struct wecker_ *data) {
	drawImageWeckerAllein(15, 170, WECKERCOLOR, BACKCOLOR);
	drawText(85, 170, 5, BACKCOLOR, BACKCOLOR, "    ");
	data->weckerStatus = weckerAus;
}

//Weckeralarm ist an
void weckerAlarmAn() {
	drawImageWeckerAllein(15, 170, DEFCOLOR, BACKCOLOR);
	char we[3];
	we[0] = numToChar(data.weckerHH / 10);
	we[1] = numToChar(data.weckerHH % 10);
	we[2] = '\0';
	drawText(75, 203, 2, DEFCOLOR, BACKCOLOR, we);
	drawDoublePoint(105, 203, 2, DEFCOLOR);
	we[0] = numToChar(data.weckerMM / 10);
	we[1] = numToChar(data.weckerMM % 10);
	we[2] = '\0';
	drawText(110, 203, 2, DEFCOLOR, BACKCOLOR, we);
}
//Alarm wurde von testAlarm() ausgelöst
void weckerAlarmAlarm(struct wecker_ *data) {
	drawText(150, 170, 5, BACKCOLOR, BACKCOLOR, "   ");
	if (data->weckerZeitSchlummer > 9) {	//!
		drawZiffer(150, 170, 5, WECKERALARMCOLOR, BACKCOLOR, data->weckerZeitSchlummer / 10);
		drawZiffer(195, 170, 5, WECKERALARMCOLOR, BACKCOLOR, data->weckerZeitSchlummer % 10);
	}
	else
		drawZiffer(195, 170, 5, WECKERALARMCOLOR, BACKCOLOR, data->weckerZeitSchlummer);
	drawText(240, 170, 5, WECKERALARMCOLOR, BACKCOLOR, "M");
	data->weckerStatus = weckerAlarm;
	data->weckerMMSchlum = data->uhrMM + data->weckerZeitSchlummer;
	data->weckerHHSchlum = data->uhrHH;
	if (data->weckerHHSchlum > 59) {
		data->weckerMMSchlum -= 60;
		data->weckerHHSchlum += 1;
	}
	//Musik play
	volSet(data->musikLaut);
	_delay_ms(50);
	repeatMode(1);	//Repeat Folder
	_delay_ms(50);
	playFoldersTrack(1, data->musikTitel);
	_delay_ms(50);
	eqSet(data->musikEQ);
}
//Alarm ist gestoppt, soll geschlummert werden oder ganz aus?
void weckerSchlummernAn(struct wecker_ *data) {
	data->weckerStatus = weckerSchlummern;
	drawImageWeckerAllein(15, 170, DEFCOLOR, BACKCOLOR);
	drawText(150, 170, 5, WECKERALARMCOLOR, BACKCOLOR, "AUS");
	//Musik stop
	pausePlay();
}

void drawDatumZeile(struct wecker_ *data) {
  char datum[11];

  datum[0] = numToChar(data->datumDD / 10);
  datum[1] = numToChar(data->datumDD % 10);
  datum[2] = '.';
  datum[3] = numToChar(data->datumMM / 10);
  datum[4] = numToChar(data->datumMM % 10);
  datum[5] = '.';
  datum[6] = '2';
  datum[7] = '0';
  datum[8] = numToChar(data->datumYY / 10);
  datum[9] = numToChar(data->datumYY % 10);
  datum[10] = '\0';
  
  drawText(10, 10, 2, DEFCOLOR, BACKCOLOR, dayName[data->tag]);
  drawText(168, 10, 2, DEFCOLOR, BACKCOLOR, datum);
}
