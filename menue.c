#include "menue.h"
#include "paint.h"
#include "segment14Font.h"
#include "uhr.h"
#include "tft.h"
#include "einstellenForm.h"
#include "util/delay.h"

void menueAufbauen(struct wecker_ *weckerData) {  
  uint16_t xTouch = 0, yTouch = 0, zTouch = 0;
  uint8_t isWhile = 1;
	uint8_t wahl = 0;
	
	fillScreen(BACKCOLOR);
	setDimLevel(8);
	
	drawText(20, 20, 3, DEFCOLOR, BACKCOLOR, "UHR");
	drawRect(10, 10, 145, 50, 1, DEFCOLOR); 
	
	drawText(17, 105, 3, DEFCOLOR,BACKCOLOR, "DATUM");
	drawRect(10, 95, 145, 50, 1, DEFCOLOR);
	
	drawText(180, 20, 3, DEFCOLOR, BACKCOLOR, "WECKER");
	drawRect(165, 10, 145, 50, 1, DEFCOLOR);
	
	drawText(203, 105, 3, DEFCOLOR, BACKCOLOR, "MUSIK");
	drawRect(165, 95, 145, 50, 1, DEFCOLOR);
	
	drawText(180, 188, 3, DEFCOLOR, BACKCOLOR, "SYSTEM");	//Menü 5 noch nicht benutzt
	drawRect(165, 178, 145, 50, 1, DEFCOLOR);
	  
  drawPfeil(60, 175, DEFCOLOR, 9);	//zurück
  
  /*
 * wahl: 	1 = Uhr einstellen
 * 				2 = Datum einstellen
 * 				3 = Wecker einstellen
 * 				4 = Musik Auswahl, Lautstärke
 * 				5 = System
 */ 
  while (isWhile) {
		getxy(&xTouch, &yTouch, &zTouch);
		if (zTouch > TOUCH_THRES) 					//TOUCH_THRES in tft.h
		{
			if ((xTouch > 1) && (xTouch < 160)) 
			{
				if (yTouch < 80) 
				{
					isWhile = 0;
					wahl = 1;	//Uhr einstellen
				}
				else if ((yTouch > 80) && (yTouch < 160)) 
				{
					isWhile = 0;
					wahl = 2;	//Datum einstellen
				}
				else if (yTouch > 160) 
				{
					isWhile = 0;
					uhrAufbauen();
				}
			}
			else if (xTouch > 160) 
			{
				if (yTouch < 80) 
				{
					isWhile = 0;
					wahl = 3;	//Wecker einstellen
				}
				else if ((yTouch > 80) && (yTouch < 160)) 
				{
					isWhile = 0;
					wahl = 4;	//Musik auswahl
				}
				else if (yTouch > 160) 
				{
					isWhile = 0;
					wahl = 5;	//System
				}
			}
    }
    _delay_ms(100);
	}
	formAufbauen(weckerData, wahl);
}
