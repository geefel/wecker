#ifndef PAINT_H
#define PAINT_H

#include <stdint.h>


#define DEFCOLOR 0xC61FUL
#define BACKCOLOR 0x0810
#define SELCOLOR 0xF800
#define WECKERCOLOR 0x0821
#define WECKERALARMCOLOR 0xE945

void initForTFT();
void drawImageBit(uint16_t x, uint16_t y, uint16_t imWidth, uint16_t imHight, uint16_t forColor, uint16_t backColor, const uint8_t *imageS);
void drawPfeil(uint16_t x, uint16_t y, uint16_t color, uint8_t dir);
void drawRect(uint16_t x, uint16_t y, uint16_t breit, uint16_t hoch, uint16_t dick, uint16_t color);
void drawFilledRect(uint16_t x, uint16_t y, uint16_t breit, uint16_t hoch, uint16_t color);
void drawCircle(int16_t x, int16_t y, int16_t r, uint16_t color);
void drawFilledCircle(int16_t x, int16_t y, int16_t radius, uint16_t color);
void drawVLine(int16_t x, int16_t y, int16_t lang, uint16_t color);
void drawHLine(int16_t x, int16_t y, int16_t breit, uint16_t color);
void fillScreen(uint16_t color);

void drawImageWeckerAlles(uint16_t x, uint16_t y, uint16_t forColor, uint16_t backColor);
void drawImageWeckerAllein(uint16_t x, uint16_t y, uint16_t forColor, uint16_t backColor);
void drawImageWeckerKlingeln(uint16_t x, uint16_t y, uint16_t forColor, uint16_t backColor);
void getxy(uint16_t *x, uint16_t *y, uint16_t *z);
void setDimLevel(int8_t dim);


#endif	//PAINT_H
