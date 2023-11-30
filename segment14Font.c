#include <stdlib.h>

#include "segment14Font.h"
#include "tft.h"
#include "paint.h"
#include "uart_print.h"

/*
   Abstände: 7 * dicke für Zahlen/Buchstaben.
   Nach Punkten 2 * dicken
   Höhe = dicke * 11
*/
 
uint8_t draw(uint16_t x, uint16_t y, uint16_t dicke, uint16_t color, char sign);
void segment(uint16_t vpos, uint16_t hpos, uint16_t dicke, uint16_t color, uint8_t segNr);
void clearDigit(uint16_t x, uint16_t y, uint8_t dicke, uint16_t backcolor);

void clearDigit(uint16_t x, uint16_t y, uint8_t dicke, uint16_t backcolor) {
	drawFilledRect(x, y, dicke * 7, dicke * 11, backcolor);	
}

void drawZiffer(uint16_t x, uint16_t y, uint16_t dicke, uint16_t color, uint16_t backcolor, uint8_t z) {
	char erg = 'X';
	switch (z) {
		case 0: erg = '0'; break;
		case 1: erg = '1'; break;
		case 2: erg = '2'; break;
		case 3: erg = '3'; break;
		case 4: erg = '4'; break;
		case 5: erg = '5'; break;
		case 6: erg = '6'; break;
		case 7: erg = '7'; break;
		case 8: erg = '8'; break;
		case 9: erg = '9'; break;
		default: break;
	};
	drawChar(x, y, dicke, color, backcolor, erg);
}

/*
 * Nomenklatur:
 * M = Mul = Multiplikation
 * P = Plus
 */
void drawText(uint16_t x, uint16_t y, uint16_t dicke, uint16_t color, uint16_t backcolor, const char *txt) {
  uint8_t i = 0;
  uint16_t dM7 = dicke * 7;
  uint16_t xPiMdM7 = 0;
  while (txt[i] != '\0') {
		xPiMdM7 = x + i * dM7;
		clearDigit(xPiMdM7, y, dicke, backcolor);
		draw(xPiMdM7, y, dicke, color, txt[i]);
    ++i;	//Zeichenvorschub
  }
}

void drawChar(uint16_t x, uint16_t y, uint16_t dicke, uint16_t color, uint16_t backcolor, char txt) {
	clearDigit(x, y, dicke, backcolor);
	draw(x, y, dicke, color, txt);
}

void drawDoublePoint(uint16_t x, uint16_t y, uint16_t dicke, uint16_t color) {
	uint16_t dickeMul3 = dicke * 3;
	uint16_t dickeMul7 = dicke * 7;
	uint16_t xPdM3 = x;// + dickeMul3;
	uint16_t yPdM3 = y + dickeMul3;
	uint16_t yPdM7 = y + dickeMul7;

  for (uint8_t i = 0; i < dicke; ++i) {
    for (uint8_t j = 0; j < dicke; ++j) {
      drawPix(xPdM3 + i, yPdM7 + j, color);
      drawPix(xPdM3 + i, yPdM3 + j, color);
		}
	}
}

void drawPoint(uint16_t x, uint16_t y, uint16_t dicke, uint16_t color) {
  uint8_t i, j;
  x += dicke * 3;
  y += dicke * 9;
  
  for (i = 0; i < dicke; ++i) {
    for (j = 0; j < dicke; ++j)
      drawPix(x + i, y + j, color);
  }
}
void drawMinus(uint16_t x, uint16_t y, uint16_t dicke, uint16_t color) {
  uint8_t i, j;
  x += dicke;
  y += dicke * 5;
  uint16_t dM4 = dicke * 4;
  
  for (i = 0; i < dM4; ++i) {
    for (j = 0; j < dicke; ++j)
      drawPix(x + i, y + j, color);
  }
}
//dir = 0: nach oben, dir = 3: nach rechts, usw.
void drawTriangl(uint16_t x, uint16_t y, uint8_t dicke, uint16_t color, uint8_t dir) {
	uint8_t anfang, ende, i, j;
	switch (dir) {
		case 0: {             //nach oben
			if (dicke % 2 == 0) {
				anfang = dicke / 2;
				ende = anfang + 1;
			}
			else /*if (dicke % 2 > 0)*/ {
				anfang = dicke / 2 - 1;
				ende = anfang + 2;
			}
			for (i = 0; i < dicke / 2; ++i, --anfang) {
				for (j = anfang; j < ende; ++j)
					drawPix(x + i, y + j, color);
			}
			for (; i < dicke; ++i, ++anfang) {
				for (j = anfang; j < ende; ++j)
					drawPix(x + i, y + j, color);
			}
			break;
		}
		case 3: {               //nach rechts
			anfang = 0;
			ende = dicke - 1;
			for (i = 0; i < dicke; ++i, ++anfang, --ende) {
				for (j = anfang; j < ende; ++j)
					drawPix(x + i, y + j, color);
			}
			break;
		}
		case 6: {             //nach unten
			anfang = 0;
			ende = 1;
			for (i = 0; i < dicke / 2; ++i, ++ende) {
				for (j = anfang; j < ende; ++j)
					drawPix(x + i, y + j, color);
			}
			for (; i < dicke; ++i, --ende) {
				for (j = anfang; j < ende; ++j)
					drawPix(x + i, y + j, color);
			}
			break;
		}
		case 9: {                 //nach links
			if (dicke % 2 > 0) {
				anfang = dicke / 2;
				ende = anfang + 1;
			}
			else /*if (dicke % 2 == 0)*/ {
				anfang = dicke / 2 - 1;
				ende = anfang + 2;
			}
			for (i = 0; i < dicke; ++i, --anfang, ++ende) {
				for (j = anfang; j < ende; ++j)
					drawPix(x + i, y + j, color);
			}
		}
		default: break;
	}
}

uint8_t draw(uint16_t x, uint16_t y, uint16_t dicke, uint16_t color, char sign) {
  const uint8_t *s;
  switch (sign) {
    case '0': s = seg[0]; break;
    case '1': s = seg[1]; break;
    case '2': s = seg[2]; break;
    case '3': s = seg[3]; break;
    case '4': s = seg[4]; break;
    case '5': s = seg[5]; break;
    case '6': s = seg[6]; break;
    case '7': s = seg[7]; break;
    case '8': s = seg[8]; break;
    case '9': s = seg[9]; break;
    case 'A': s = seg[10]; break;
    case 'B': s = seg[11]; break;
    case 'C': s = seg[12]; break;
    case 'D': s = seg[13]; break;
    case 'E': s = seg[14]; break;
    case 'F': s = seg[15]; break;
    case 'G': s = seg[16]; break;
    case 'H': s = seg[17]; break;
    case 'I': s = seg[18]; break;
    case 'J': s = seg[19]; break;
    case 'K': s = seg[20]; break;
    case 'L': s = seg[21]; break;
    case 'M': s = seg[22]; break;
    case 'N': s = seg[23]; break;
    case 'O': s = seg[24]; break;
    case 'P': s = seg[25]; break;
    case 'Q': s = seg[26]; break;
    case 'R': s = seg[27]; break;
    case 'S': s = seg[28]; break;
    case 'T': s = seg[29]; break;
    case 'U': s = seg[30]; break;
    case 'V': s = seg[31]; break;
    case 'W': s = seg[32]; break;
    case 'X': s = seg[33]; break;
    case 'Y': s = seg[34]; break;
    case 'Z': s = seg[35]; break;
    //case '-': s = seg[36]; break;
    case ' ': s = seg[37]; break;
    case '*': s = seg[38]; break;
    case ':': drawDoublePoint(x, y, dicke, color); return 1;;
    case '.': drawPoint(x, y, dicke, color); return 1;
    case '-': drawMinus(x, y, dicke, color); return 1;
    //case '_': drawLine(x, y, dicke, color);
    default: break;
  }
  int c = 0;
  while (s[c] > 0) {
    segment(x, y, dicke, color, s[c]);
    ++c;
  }
  return 1;
}
/*
   Segmente der 14 Seg-Anzeige
   segNr: (Vertikale) 5 = links unten, 6 = links oben, 3 = rechts unten, 2 = rechts oben, 13 = mitte unten, 10 = mitte oben
   segNr: (Diagonale) 9 = oben links, 11 = oben rechts, 12 = unten rechts, 14 unten links
   segNr: (Horizontale) 4 = unten, 1 = oben, 7 = mitte links, 8 = mitte rechts
   vpos/hpos wie Gesammtzifferkoordinaten, Versatz wird in Funktion erstellt
*/
void segment(uint16_t x, uint16_t y, uint16_t dicke, uint16_t color, uint8_t segNr) {
  uint8_t dir;
  uint8_t a, e, i, j;
  uint16_t x0 = 0, y0 = 0, x1 = 0, y1 = 0;
  uint8_t laengeRechteck = dicke * 4;
  uint8_t zeile = (dicke / 2 + 1); //Anzahl Leerzeilen bis erste Zeile mit Pixel
  uint8_t hoehe = dicke - zeile;

  //Ob Diagonal, Vertikal oder Horizontal
  if (segNr == 1 || segNr == 4 ||  segNr == 7 || segNr == 8) //Horizontal
    dir = 1;
  else if (segNr == 2 || segNr == 3 || segNr == 5 || segNr == 6 || segNr == 10 || segNr == 13)  //Vertikal
    dir = 2;
  else if (segNr == 9 || segNr == 11 || segNr == 12 || segNr == 14)  //Diagonale
    dir = 3;

  // Versatz der Segmente
  switch (segNr) {
    case 6:													//Vertikale links oben
			//x = x, y = y
      break;
    case 2:													//rechts oben
      x = x + dicke * 5;
      break;
    case 5:													//links unten
      y = y + dicke * 5;
      break;
    case 3:													//rechts unten
      y = y + dicke * 5;
      x = x + dicke * 5;
      break;
    case 10:												//mitte oben
      x = x + dicke * 2.7;
      y = y + dicke * 0.5;
      laengeRechteck = dicke * 3.5;
      break;
    case 13:												//mitte unten
      x = x + dicke * 2.7;
      y = y + dicke * 5;
      laengeRechteck = dicke * 3.5;
      break;
    case 4:											   //Horizontale unten
			x1 = x + dicke * 5;//neu
			y = y + dicke * 10;
      break;
    case 1:                       //oben
			x1 = x + dicke * 5;//neu
      //x = x, y = y
      break;
    case 7:                       //mitte links
      y = y + dicke * 5;
      x1 = x + dicke * 2.5;//neu
      laengeRechteck = dicke * 1.5;
      break;
    case 8:                       //mitte rechts
      y = y + dicke * 5;
      x = x + dicke * 2.5;
      x1 = x + dicke * 2.5;//neu
      laengeRechteck = dicke * 1.5;
      break;
    case 9:                      //Diagonale 9 oben links
      x0 = x + dicke * 1;
      y0 = y + dicke * 1;
      x1 = x + dicke * 3;
      y1 = y + dicke * 4.5;
      break;
    case 12:                      //12 unten rechts
      x0 = x + dicke * 3;
      y0 = y + dicke * 4.5;
      x1 = x + dicke * 5;
      y1 = y + dicke * 8;
      break;
    case 14:                       //14 unten links
      x0 = x + dicke * 3;
      y0 = y + dicke * 4.5;
      x1 = x + dicke * 1;
      y1 = y + dicke * 8;
      break;
    case 11:                      //11 oben rechts
      x0 = x + dicke * 5;
      y0 = y + dicke * 1;
      x1 = x + dicke * 3;
      y1 = y + dicke * 4.5;
      break;
  }

  //ist dicke ungerade oder gerade?	So: dicke % 2 > 0 oder besser Frage nur nach letzter Ziffer (dicke & 1) == 1 ist ungerade
  if (dicke & 1) {
    a = dicke / 2;
    e = a + 1;
  }
  else {
    a = dicke / 2 - 1;
    e = a + 2;
  }

  switch (dir) {

    //Horizontales Segment
    case 1:
      for (i = 0; i < hoehe; ++i, ++zeile, --a, ++e) {    //linkes Dreieck
        for (j = a; j < e; ++j)
          drawPix(x + zeile, y + j, color);
      }
      
      drawWindow(x + dicke, y, x1 - 1, y + dicke - 1, color);	//mittleres Rechteck
      zeile += laengeRechteck;
      
      a = 1;                                              //rechtes Dreieck
      e = dicke - 1;
      for (i = 0; i < hoehe; ++i, ++zeile, ++a, --e) {
        for (j = a; j < e; ++j)
          drawPix(x + zeile, y + j, color);
      }
      break;

    //Vertikales Segment
    case 2:
      for (i = 0; i < hoehe; ++i, ++zeile, --a, ++e) {   //oberes Dreieck
        for (j = a; j < e; ++j)
          drawPix(x + j, y + zeile, color);
      }
      
      drawWindow(x, y + dicke , x + dicke - 1, y + dicke * 5 - 1, color);	//mittleres Rechteck
      zeile += laengeRechteck;
      
      a = 1;                                             //unteres Dreieck
      e = dicke - 1;
      for (i = 0; i < hoehe; ++i, ++zeile, ++a, --e) {
        for (j = a; j < e; ++j)
          drawPix(x + j, y + zeile, color);
      }
      break;

    //diagonales Segment
    case 3:
      for (uint16_t i = 0; i < dicke * 2; ++i) //dicke der Balken
        drawLine(x0, y0 + i, x1, y1 + i, color);
      break;
  }
}

void drawLine(uint16_t x0, uint16_t y0,uint16_t x1, uint16_t y1, uint16_t color) {
	int x = x1 - x0;
	int y = y1 - y0;
	int dx = abs(x), sx = x0 < x1 ? 1 : -1;
	int dy = -abs(y), sy = y0 < y1 ? 1 : -1;
	int err = dx + dy, e2;

	for (;;){
		drawPix(x0, y0 , color);
		e2 = 2 * err;
		if (e2 >= dy) {     
			if (x0 == x1) break;
			err += dy; x0 += sx;
		}
		if (e2 <= dx) {
			if (y0 == y1) break;
			err += dx; y0 += sy;
		}
	}
}
