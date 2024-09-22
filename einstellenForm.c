#include "einstellenForm.h"
#include "rtc.h"
#include "paint.h"
#include "tft.h"
#include "segment14Font.h"
#include "wavPlayer.h"
#include <stdint.h>
#include <util/delay.h>

void drawZif(uint8_t zahl, uint8_t wert, uint8_t wahl);
void touch(struct wecker_ *data, uint8_t wahl);
void drawOK(uint16_t col);
void drawTest(uint16_t col);
void drawAnAus(uint8_t anAus);

int8_t z1 = 0, z2 = 0, z3 = 0, z4 = 0;
int8_t z1Max = 0, z1Min = 0, z2Max = 0, z2Min = 0, z3Max = 0, z3Min = 0, z4Min = 0, z4Max = 0;
uint8_t ddMax[13] = {29, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

/*
 * wahl: 	1 = Datum einstellen
 * 				2 = Uhrzeit einstellen
 * 				3 = Wecker einstellen
 * 				4 = Musik auswahl & Lautstärke
 * 				5 = System
 */ 
void formAufbauen(struct wecker_ *data, uint8_t wahl) {
	char *seitenName = "";
	fillScreen(BACKCOLOR); 
	//Variablen übergeben, Min/Max bereit stellen
	switch (wahl) {
		case 1: 
			seitenName = "       UHRZEIT EINSTELLEN";
			z1 = data->uhrHH;
			z2 = data->uhrMM;
			z3 = data->uhrSec;
			z1Min = z2Min = z3Min = 0;
			z1Max = 23;
			z2Max = 59;
			z3Max = 59;
		break;
		case 2:
			seitenName = "       DATUM EINSTELLEN";
			z1 = data->datumDD;
			z2 = data->datumMM;
			z3 = data->datumYY;
			z1Min = z2Min = z3Min = 1;
			if (z3 % 4)						//Schaltjahr
				z1Max = ddMax[0];
			else
				z1Max = ddMax[z2];
			z2Max = 12;
			z3Max = 99;
		break;
		case 3:
			seitenName = "       WECKER EINSTELLEN     SCHLUMM MIN";
			z1 = data->weckerHH;
			z2 = data->weckerMM;
			z3 = data->weckerZeitSchlummer;
			z4 = data->weckerStatus;
			z1Min = z2Min = z4Min = 0;
			z3Min = 1;
			z1Max = 24;
			z2Max = 59;
			z3Max = 20;
			z4Max = 1;
		break;
		case 4:
			seitenName = "         TITEL       LAUT        EQ";
			z1 = data->musikTitel;
			z2 = getVol();
			z3 = data->musikEQ;
			z1Min = 1;
			z1Max = getNumTfCardFiles();	//Abhängig von Anzahl Titel in Folder 01 auf FCard
			z2Min = 0;
			z2Max = 30;
			z3Min = 0;
			z3Max = 5;
		break;
		case 5:
			seitenName = "         HELL";
			z1 = data->dimBacklight;
			z2 = data->musikEQ;
			z3 = 0;
			z1Min = 0;
			z1Max = 9;
			z2Min = 1;	//?
			z2Max = 6;	//?
			z3Min = 0;	//?
			z3Max = 1;	//?
		break;
		default: 
		break;
	}
	drawText(10, 10, 1, DEFCOLOR, BACKCOLOR, seitenName);
	
	drawZif(1, z1, wahl);										//Einstellbereich links
	drawPfeil(60, 85, DEFCOLOR, 0);
	drawPfeil(60, 135, DEFCOLOR, 6);
	if (wahl == 2)
		drawPoint(113, 35, 4, DEFCOLOR);
	else if (wahl == 1 || wahl == 3) 
		drawDoublePoint(126, 35, 4, DEFCOLOR);
		
	drawZif(2, z2, wahl);										//Einstellbereich mitte
	drawPfeil(140, 85, DEFCOLOR, 0);
	drawPfeil(140, 135, DEFCOLOR, 6);
	if (wahl == 2)
		drawPoint(193, 35, 4, DEFCOLOR);
		
	drawZif(3, z3, wahl);										//Einstellbereich rechts
	drawPfeil(220, 85, DEFCOLOR, 0);
	drawPfeil(220, 135, DEFCOLOR, 6);	
	
	drawPfeil(10, 180, DEFCOLOR, 9);				//zurück
	drawOK(DEFCOLOR);												//speichern
	if (wahl == 3)													//WeckerAnAus
		drawAnAus(z4);
	if (wahl == 4)													//Test Musik
		drawTest(DEFCOLOR);
	touch(data, wahl);
}

void drawOK(uint16_t col) {
	drawText(152, 200, 3, col, BACKCOLOR, "OK");
}

void drawTest(uint16_t col) {
	drawText(220, 200, 3, col, BACKCOLOR, "TEST");
}
void drawAnAus(uint8_t anAus) {
	if (anAus)
		drawText(220, 200, 3, DEFCOLOR, BACKCOLOR, "AN ");
	else
		drawText(220, 200, 3, DEFCOLOR, BACKCOLOR, "AUS");
}

//bereich: links/mitte/rechts; wert: der dargestellt werden soll; wahl: Menüauswahl
void drawZif(uint8_t bereich, uint8_t wert, uint8_t wahl) {
	switch (bereich) {
		case 1:
			drawZiffer(60, 30, 4, DEFCOLOR, BACKCOLOR, wert / 10);
			drawZiffer(90, 30, 4, DEFCOLOR, BACKCOLOR, wert % 10);
		break;
		case 2:
			if (wahl != 5) {
				drawZiffer(140, 30, 4, DEFCOLOR, BACKCOLOR, wert / 10);
				drawZiffer(170, 30, 4, DEFCOLOR, BACKCOLOR, wert % 10);
			}
		break;
		case 3:
			if (wahl != 5) {
				drawZiffer(220, 30, 4, DEFCOLOR, BACKCOLOR, wert / 10);
				drawZiffer(250, 30, 4, DEFCOLOR, BACKCOLOR, wert % 10);
			}
			//~ else {
				//~ if (wert == 1)
					//~ drawText(220, 50, 4, DEFCOLOR, BACKCOLOR, "AN ");
				//~ else
					//~ drawText(220, 50, 4, DEFCOLOR, BACKCOLOR, "AUS");
			//~ }
		break;
	}
}

void touch(struct wecker_ *data, uint8_t wahl) {
	uint16_t xTouch, yTouch, zTouch;
  uint8_t isWhile = 1;
  uint8_t isMusikPlay = 0;
  while (isWhile) {
	getxy(&xTouch, &yTouch, &zTouch);
	if (zTouch > TOUCH_THRES) {												//TOUCH_THRES in tft.h
			
			/*********zurück zur Uhr*************/
      if ((xTouch > 0) && (xTouch < 40) && (yTouch > 170) && (yTouch < 240)) {
				isWhile = 0;
				if (wahl == 4) pausePlay();
        uhrAufbauen();
      }
      
			/*********Dreiecke links*****************/
      else if 	((xTouch > 70) && (xTouch < 134)) {
				if 			(yTouch > 100 && yTouch < 150) {			//links oben
					z1++;
					if (wahl != 2) 
						z1 = z1 <= z1Max ? z1 : z1Max;
					else {
						if (!(z3 % 4))
							z1 = z1 <= ddMax[0] ? z1 : ddMax[0];
						else
							z1 = z1 <= ddMax[z2] ? z1 : ddMax[z2];
					}
					if (wahl == 5)
						setDimLevel(z1);
					drawZif(1, z1, wahl);
					drawOK(SELCOLOR);
				}
				else if ((yTouch > 161) && (yTouch < 200)) {		//links unten
					z1--;
					z1 = z1 >= z1Min ? z1 : z1Min;
					if (wahl == 5)
						setDimLevel(z1);
					drawZif(1, z1, wahl);
					drawOK(SELCOLOR);
				}
      }
      
      /*********Dreiecke mitte*****************/
      else if 	((xTouch > 135) && (xTouch < 219)) {
				if 			((yTouch > 100) && (yTouch < 150)) {			//mitte oben
					z2++;
					z2 = z2 <= z2Max ? z2 : z2Max;
					drawZif(2, z2, wahl);
					drawOK(SELCOLOR);
				}
				else if ((yTouch > 161) && (yTouch < 200)) {			//mitte unten
					z2--;
					z2 = z2 >= z2Min ? z2 : z2Min;
					drawZif(2, z2, wahl);
					drawOK(SELCOLOR);
				}
				
				/*********speichern*****************/
				else if ((yTouch > 201) && (yTouch < 240)) {			//ok
					switch(wahl) {
						case 1: 
							data->uhrHH = z1;
							data->uhrMM = z2;
							data->uhrSec = z3;
							setClock(data);
						break;
						case 2:
							data->datumDD = z1;
							data->datumMM = z2;
							data->datumYY = z3;
							setClock(data);
						break;
						case 3:
							data->weckerHH = z1;
							data->weckerMM = z2;
							data->weckerZeitSchlummer = z3;
							data->weckerStatus = z4;
						break;
						case 4:
							data->musikTitel = z1;
							data->musikLaut = z2;
							data->musikEQ = z3;
						break;
						case 5:
							data->dimBacklight = z1;
						break;
					}
					drawOK(DEFCOLOR);
				}
      }
      
			/*********Dreiecke rechts*****************/
			else if 	((xTouch > 220) && (xTouch < 295)) {
				if 			((yTouch > 100) && (yTouch < 150)) {			//rechts oben
					z3++;
					z3 = z3 <= z3Max ? z3 : z3Max;
					drawZif(3, z3, wahl);
					drawOK(SELCOLOR);
				}
				else if ((yTouch > 161) && (yTouch < 200)) {			//rechts unten
					z3--;
					z3 = z3 >= z3Min ? z3 : z3Min;
					drawZif(3, z3, wahl);
					drawOK(SELCOLOR);
				}
				
				else if ((yTouch > 201) && (yTouch < 240)) {			//Ecke rechts unten
					/*********Wecker AnAus**************/
					if (wahl == 3) {
						z4 = z4 == 1 ? 0 : 1;
						drawAnAus(z4);
						drawOK(SELCOLOR);
					}
					/*********Test Musik****************/
					else if (wahl == 4) {
						isMusikPlay = isMusikPlay == 0 ? 1 : 0;
						if (!isMusikPlay) {
							volSet(data->musikLaut);
							_delay_ms(50);
							playFoldersTrack(1, data->musikTitel);
							_delay_ms(50);
							eqSet(data->musikEQ);
							drawTest(SELCOLOR);
						}
						else {
							pausePlay();
							drawTest(DEFCOLOR);
						}
					}
				}
			}
      
    }
    _delay_ms(50);
	}
}

