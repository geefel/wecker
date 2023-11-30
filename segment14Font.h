#ifndef SEGMENT14FONT_H
#define SEGMENT14FONT_H

#include <stdint.h>

void drawDoublePoint(uint16_t x, uint16_t y, uint16_t dicke, uint16_t color);
void drawPoint(uint16_t x, uint16_t y, uint16_t dicke, uint16_t color);
void drawMinus(uint16_t x, uint16_t y, uint16_t dicke, uint16_t color);
void drawText(uint16_t x, uint16_t y, uint16_t dicke, uint16_t color, uint16_t backcolor, const char *txt);
void drawChar(uint16_t x, uint16_t y, uint16_t dicke, uint16_t color, uint16_t backcolor, char txt);
void drawZiffer(uint16_t x, uint16_t y, uint16_t dicke, uint16_t color, uint16_t backcolor, uint8_t z);
void drawTriangl(uint16_t x, uint16_t y, uint8_t dicke, uint16_t color, uint8_t dir);
void drawLine(uint16_t x0, uint16_t y0,uint16_t x1, uint16_t y1, uint16_t color);


//****************************************************
//an-/auspositionen der Segmente in der 14-Seg-Anzeige

static const uint8_t seg[39][15] = {
	{1,2,6,3,5,4,0},//0		0b0000000001111110	0x007E
	{2,3,0},//						0b0000000000001100	0x000C
	{1,2,8,7,5,4,0},//		0b0000000110110110	0x01B6
	{1,2,8,3,4,0},//			0b0000000100011110	0x011E
	{6,2,7,8,3,0},//			0b0000000111001100	0x01CC
	{1,6,7,8,3,4,0},//5		0b0000000111011010	0x01DA
	{1,6,7,8,5,3,4,0},//	0b0000000111111010	0x01FA
	{1,11,13,0},//				0b0010100000000010	0x2802
	{1,2,6,7,8,5,3,4,0},//0b0000000111111110	0x01FE
	{1,6,2,7,8,3,0},//9		0b0000000111001110	0x01CE
	{1,6,2,7,8,5,3,0},//A
	{1,10,2,8,13,3,4,0},
	{1,6,5,4,0},
	{1,10,2,13,3,4,0},
	{1,6,7,8,5,4,0},
	{1,6,7,8,5,0},//F
	{1,6,8,5,3,4,0},
	{6,2,7,8,5,3,0},
	{1,10,13,4,0},
	{2,5,3,4,0},
	{6,11,7,5,12,0},//K
	{6,5,4,0},
	{6,9,11,2,5,3,0},
	{6,9,2,5,12,3,0},
	{1,6,2,5,3,4,0},
	{1,6,2,7,8,5,0},//P
	{1,6,2,5,12,3,4,0},
	{1,6,2,7,8,5,12,0},
	{1,6,7,8,3,4,0}, //S alt: {1,9,8,3,4,0},
	{1,10,13,0},
	{6,2,5,3,4,0},
	{6,11,5,14,0},
	{6,2,5,14,12,3,0},
	{9,11,14,12,0}, //X
	{9,11,13,0},
	{1,11,14,4,0}, //Z alt {1,11,7,8,14,4,0},
	{7,8,0},       //Bindestrich
	{0},//alle aus
	{1,6,9,10,11,2,7,8,5,14,13,12,3,4,0} //alle Segmente an
};
        
//~ //*************************************************************
//~ static const uint8_t tagBuchstaben[8][10] = {
  //~ {'S', 'O', 'N', 'N', 'T', 'A', 'G', ' ', ' ', ' '},
  //~ {'M', 'O', 'N', 'T', 'A', 'G', ' ', ' ', ' ', ' '},
  //~ {'D', 'I', 'E', 'N', 'S', 'T', 'A', 'G', ' ', ' '},
  //~ {'M', 'I', 'T', 'T', 'W', 'O', 'C', 'H', ' ', ' '},
  //~ {'D', 'O', 'N', 'N', 'E', 'R', 'S', 'T', 'A', 'G'},
  //~ {'F', 'R', 'E', 'I', 'T', 'A', 'G', ' ', ' ', ' '},  
  //~ {'S', 'A', 'M', 'S', 'T', 'A', 'G', ' ', ' ', ' '}  
//~ };
//~ //****************************************************************************
//~ static const uint8_t ziffer[10] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9'};
//~ //****************************************************************************


#endif //SEGMENT14FONT_H
