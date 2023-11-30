/*
 * paint.c
 * 
 * Copyright 2021 gfl <>
 * 
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
 * MA 02110-1301, USA.
 * 
 * 
 */

#include <avr/pgmspace.h>
#include "paint.h"
#include "bilder.h"
#include "uart_hard.h"
#include "uart_print.h"
#include "tft.h"


//~ const uint16_t blau = 31;
//~ const uint16_t rot = 0xF800;
//~ const uint16_t weiss = 0xFFFF;
//~ const uint16_t schwarz = 0;
//~ #define BACKCOLOR blau
//~ #define TEXTCOLOR weiss

void initForTFT() {
	init_Pins();
#if defined(SPI_hard)	//in tft.h
	init_SPI();
#endif
	init_TFT();
	init_Touch();
}

/*
 * Für alle, TFT-Beschriftung unten:
 * MATCTE MY = MX = MV = 0
 * normal, x0/y0 oben links, x: 0 - 240, y: 0 - 320
 * MATCTE MY = 0, MX = 1, MV = 1 
 * 90° gedreht, x0/y0 oben rechts, x: 0 - 320, y: 0 - 240
 * MATCTE MY = 1, MX = 1, MV = 0
 * 180° gedreht, x0/y0 unten rechts, x: 0 - 240, y: 0 - 320
 * MATCTE MY = 1, MX = 0, MV = 1 
 * 270° gedreht, x0/y0 unten links, x: 0 - 320, y: 0 - 240
 * x = vertikal, y = horizontal
 */
 
 //so gehts!!!!!!!!!!!!!!
void drawImageBit(uint16_t x, uint16_t y, uint16_t imWidth, uint16_t imHight, uint16_t forColor, uint16_t backColor, const uint8_t *image) {
	uint8_t maske = 0x80;
	uint16_t p = 0;
	uint16_t xPsM8 = 0;
	uint16_t yPz = 0;
	for (uint16_t z = 0; z < imHight; ++z) {
		yPz = y + z;
		for (uint16_t s = 0; s < imWidth / 8; ++s) {
			xPsM8 = x + s * 8;
			for (int8_t b = 0; b < 8; ++b) {
				if (pgm_read_byte(&(image[p])) & maske)
					drawPix(xPsM8 + b, yPz, forColor);
				else
					drawPix(xPsM8 + b, yPz, backColor);
				maske >>= 1;
			}
			maske = 0x80;
			p++;
		}
	}
}
 
void drawImageWeckerAlles(uint16_t x, uint16_t y, uint16_t forColor, uint16_t backColor) {
	drawImageBit(x, y, weckerAlles_width, weckerAlles_hight, forColor, backColor, weckerAlles);
}
void drawImageWeckerAllein(uint16_t x, uint16_t y, uint16_t forColor, uint16_t backColor) {
	drawImageBit(x, y, weckerAllein_width, weckerAllein_hight, forColor, backColor, weckerAllein);
}
void drawImageWeckerKlingeln(uint16_t x, uint16_t y, uint16_t forColor, uint16_t backColor) {
	drawImageBit(x, y, weckerKlingelnLinks_width, weckerKlingelnLinks_hight, forColor, backColor, weckerKlingelnLinks);
	drawImageBit(x + 65, y, weckerKlingelnRechts_width, weckerKlingelnRechts_hight, forColor, backColor, weckerKlingelnRechts);
}
 
void drawPfeil(uint16_t x, uint16_t y, uint16_t color, uint8_t dir) {
	if (dir == 3)
		x += pgm_read_byte_near(&(pfeil[0][0])) - 1;	//Positionskorrektur
	else if(dir == 6)
		y += pgm_read_byte_near(&(pfeil[0][0])) - 1;
	uint8_t maske = 0x80;
	uint8_t zeile = pgm_read_byte_near(&(pfeil[0][0]));
	uint8_t spalte = pgm_read_byte_near(&(pfeil[0][1]));
	int16_t xPsM8 = 0, yPsM8 = 0, xPz = 0, xMz = 0, yPz = 0, yMz = 0;
	for(uint8_t z = 1; z < zeile; ++z) {
		yPz = y + z;
		yMz = y - z;
		xPz = x + z;
		xMz = x - z;
		for(uint8_t s = 0; s < spalte; ++s) {
			xPsM8 = x + s * 8; 
			yPsM8 = y + s * 8;
			for(int8_t b = 0; b < 8; ++b) {	//bit
				if(maske & (pgm_read_byte_near(&(pfeil[z][s])))) {
					switch(dir) {
						case 0: drawPix(xPsM8 + b, yPz, color); break;
						case 3: drawPix(xMz, yPsM8 + b, color); break;
						case 6: drawPix(xPsM8 + b, yMz, color); break;
						case 9: drawPix(xPz, yPsM8 + b, color); break;
						default : break;
					}				
				}
				maske >>= 1;
			}
			maske = 0x80;
		}
	}
}
 
//dicke nach innen
void drawRect(uint16_t x, uint16_t y, uint16_t breit, uint16_t hoch, uint16_t dick, uint16_t color) {
	drawFilledRect(x, 							 y, 							breit, dick, color);	//oben
	drawFilledRect(x, 							 y + hoch - dick, breit, dick, color);	//unten
	drawFilledRect(x, 							 y, 							dick,  hoch, color);  //links
	drawFilledRect(x + breit - dick, y, 							dick,  hoch, color);	//rechts
}

void drawFilledRect(uint16_t x, uint16_t y, uint16_t breit, uint16_t hoch, uint16_t color) {
	uint16_t xi = x;
	uint16_t xbreit = x + breit;
	uint16_t yhoch = y + hoch;
	drawWindow(x, y, xbreit, yhoch, color);
	//~ do {
		//~ do {
			//~ drawPix(xi, y, color);
		//~ } while (++xi < xbreit);
		//~ xi = x;
	//~ } while (++y < yhoch);
}

void drawCircle(int16_t x, int16_t y, int16_t r, uint16_t color) {
	int16_t a = - r, b = 0, err = 2 - 2 * r, test; /* II. Quadrant */ 
	do {
		drawPix(x-a, y+b, color); /*   I. Quadrant */
		drawPix(x-b, y-a, color); /*  II. Quadrant */
		drawPix(x+a, y-b, color); /* III. Quadrant */
		drawPix(x+b, y+a, color); /*  IV. Quadrant */
		test = err;
		if (test <= b) err += ++b * 2 + 1;           /* e_xy+e_y < 0 */
		if (test > a || err > b) err += ++a * 2 + 1; /* e_xy+e_x > 0 or no 2nd y-step */
	} while (a < 0);
 
	/*eine andere Art, nicht so schöne Kreise*/
	//~ int16_t r2mr = r * r - r;
	//~ int16_t r2pr = r * r + r;
	//~ int16_t b2, a2;
	//~ for (int16_t b = -r; b <= r; b++) {
	 //~ b2 = b * b;
		//~ for (int16_t a = -r; a <= r; a++) {
			//~ a2 = a * a;
			//~ if ((a2 + b2 > r2mr) && (a2 + b2 < r2pr)) {
				//~ drawPix(x + a, y + b, color);
			//~ }
		//~ }
	//~ }
}

void drawFilledCircle(int16_t x, int16_t y, int16_t r, uint16_t color) {
	int16_t r2pr = r * r + r;
	int16_t b2, a2;
	for (int16_t b = -r; b <= r; b++) {
		b2 = b * b;
		for (int16_t a = -r; a <= r; a++) {
			a2 = a * a;
			if ((a2 + b2) < r2pr) {//  && !((a2 + b2) < r2pr - r - dicke?) ist für Ringe
					drawPix(x + a, y + b, color);
			}
		}
	}
	/*eine andere Art, nicht so schöne Kreise*/
	//~ int16_t f = 1 - r;
	//~ int16_t ddF_x = 1;
	//~ int16_t ddF_y = -2 * r;
	//~ int16_t x0 = 0;
	//~ int16_t y0 = r;
	
	//~ drawVLine(x, y - r, 2 * r + 1, color);
	//~ while (x0 < y0) {
		//~ if (f >= 0) {
			//~ y0--;
			//~ ddF_y += 2;
			//~ f += ddF_y;
		//~ }
		//~ x0++;
		//~ ddF_x += 2;
		//~ f += ddF_x;
		//~ drawVLine(x + x0, y - y0, 2 * y0 + 1, color);
		//~ drawVLine(x + y0, y - x0, 2 * x0 + 1, color);
		//~ drawVLine(x - x0, y - y0, 2 * y0 + 1, color);
		//~ drawVLine(x - y0, y - x0, 2 * x0 + 1, color);
	//~ }
}

void drawVLine(int16_t x, int16_t y, int16_t lang, uint16_t color) {
	int16_t z = 0;
	do {
		drawPix(x, y + z, color);
	} while (++z <= lang);
}

void drawHLine(int16_t x, int16_t y, int16_t breit, uint16_t color) {
	int16_t z = 0;
	do {
		drawPix(x + z, y, color);
	} while (++z <= breit);
}

void fillScreen(uint16_t color) {
	drawWindow(0, 0, xDispMax - 1, yDispMax - 1, color);	//0° Drehung 0, 0, 240 - 1, 320 - 1
	//~ uint16_t x = 0, y = 0;
	//~ do {
		//~ do {
			//~ drawPix(x, y, color);
		//~ } while (++y < 320);
		//~ y = 0;
	//~ } while (++x < 240);
}

void getxy(uint16_t *x, uint16_t *y, uint16_t *z) {
	getTouchPos(x, y, z);
}

void setDimLevel(int8_t dim) {
	setBacklightDim(dim);
}
