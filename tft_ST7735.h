#ifndef TFT_ST7735_H
#define TFT_ST7735_H

#include "pin.h"
#include <avr/io.h>
																 //Arduino UNO Pin
#define SCK_PIN 	 	PINDEF(B, 5) //13	CLK	Orange
#define MISO_PIN 	 	PINDEF(B, 4) //12	
#define DIN_PIN 	 	PINDEF(B, 3) //11	DIN Grün
#define CS_PIN 			PINDEF(B, 2) //10	CS	Gelb
#define DC_PIN 			PINDEF(B, 1) //9	DC	Blau
#define RESET_PIN		PINDEF(B, 0) //8	RST	Braun
#define BL_PIN 			PINDEF(D, 7) //7	BL	Grau

#define CASET 0x2A 		// Column Address Set
#define RASET 0x2B 		// Page Address Set
#define RAMWR 0x2C 		// Memory Write
#define RAMRD 0x2E 		// Memory Read

void init_Pins();
void init_SPI();
void setMADCTL(uint8_t m);
void init_TFT();
void setRotaton(uint16_t rot);
void set_Sleep_On_TFT();
void set_Sleep_Off_TFT();
void drawPix(uint16_t x, uint16_t y, uint16_t color);
void drawWindow(uint16_t x, uint16_t y, uint16_t x1, uint16_t y2, uint16_t color);
void set_LCD_BL(uint8_t lcd_bl);
void setBacklightDim(int8_t dim);



#endif	//TFT_ST7735_H

