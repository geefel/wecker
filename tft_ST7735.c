/*
 * Für ST7735
*/

#include <stdint.h>
#include <util/delay.h>
#include "tft_ST7735.h"

void set_MOSI(uint8_t mosi);
void set_Data_Command(uint8_t lcd_dc);
void set_LCD_CS(uint8_t lcd_cs);
void set_LCD_BL(uint8_t lcd_bl);

void send_LCD_Register(uint8_t reg, uint8_t dat);
uint8_t send_LCD_Command_Data(uint8_t is_command_or_data, uint8_t val);

static const uint8_t isData = 1;
static const uint8_t isCommand = 0;
static const uint8_t isRegister = 0;
static const uint8_t high = 1;
static const uint8_t low = 0;
					   
void init_Pins() {
	setOutput(SCK_PIN);
	setOutput(RESET_PIN);
	setOutput(DIN_PIN);
	setOutput(CS_PIN);
	setOutput(BL_PIN);
	setOutput(DC_PIN);
}

void init_SPI() {
	SPCR = (1<<SPE)|(1<<MSTR)|(1<<SPI2X)|(1<<CPOL)|(1<<CPHA);
}
/*
   Beschriftung unten
	Memory Access Control MADCTL
	D7 MY Page Address Order: 0 = Top to Botton, 1 = umgekehrt
	D6 MX Column Address Order: 0 Left to Right, 1 = umgekehrt
	D5 MV Page/Column Order: 0 = normal, 1 = revers
	D4 ML Line Address Order: 0 = Refresh top to Bottom,1 = umgekehrt
	D3 RGB RGB/BGR Order: 0 = RGB,1 = BGR
	D2 MH Display Data Latch Order: 0 = Refresh Left to Right, 1 = umgekehrt
	MY MX MV 
	0  0  0  normal							x von li nach re, y von oben nach unten (0,0) oben links
	0  1  1  90° drehen					x von oben nach unten, y von re nach li (0,0) oben rechts
	1  1  0  180° drehen				x von re nach li, y von unten nach oben (0,0) unten rechts
	1  0  1  270° drehen				x von unten nach oben, y von li nach re (0,0) unten links
	send_LCD_Register(0x36, 0b);
	*/
	
uint8_t madctl;
void setMADCTL(uint8_t m) {
	madctl = m;
}

void setRotaton(uint16_t rot) {
	enum {
		MV = 5,
		MX,
		MY
	};
	
	switch (rot) {
		case   0: madctl &= ~((1 << MV, | (1 << MX) | (1 << MY));       ; break;
		case  90: madctl &= ~(1 << MY); madctl |= (1 << MX ) | (1 << MV); break;
		case 180: madctl &= ~(1 << MV); madctl |= (1 << MY ) | (1 << MX); break;
		case 270: madctl &= ~(1 << MX); madctl |= (1 << MY ) | (1 << MV); break;
	{
}

void init_TFT() {
	_delay_ms(200);
	clrPin(RESET_PIN);
	_delay_ms(200);
	setPin(RESET_PIN);
	_delay_ms(200);
	
	send_LCD_Command_Data(isCommand, 0x36);		//MADCTL
	send_LCD_Command_Data(isData, madctl); 

	send_LCD_Command_Data(isCommand, 0x3A); 	//COLMOD: Pixel Format Set
	send_LCD_Command_Data(isData, 0x05);

	//send_LCD_Command_Data(isCommand, 0x21); 	//INVON Display Inversion On
	//~ send_LCD_Command_Data(isCommand, 0x20); 	//INVON Display Inversion Off

	send_LCD_Command_Data(isCommand, CASET);		//CASET Column Address Set
	send_LCD_Command_Data(isData, 0x00);				//Startadresse HighByte
	send_LCD_Command_Data(isData, 0x00);				//Startadresse LowByte
	send_LCD_Command_Data(isData, 0x00);				//Endeadresse HighByte = x-Pixel-Anzahl
	send_LCD_Command_Data(isData, 0x7F);				//Endeadresse LowByte

	send_LCD_Command_Data(isCommand, RASET);		//RASET Row Address Set
	send_LCD_Command_Data(isData, 0x00);				//wie oben
	send_LCD_Command_Data(isData, 0x00);
	send_LCD_Command_Data(isData, 0x00);				// = y-Pixel-Anzahl
	send_LCD_Command_Data(isData, 0x9F);

	//~ send_LCD_Command_Data(isCommand, 0xB2);		//FRMCTR2 In Idle Mode (8-colors)
	//~ send_LCD_Command_Data(isData, 0x0C);
	//~ send_LCD_Command_Data(isData, 0x0C);
	//~ send_LCD_Command_Data(isData, 0x00);
	//~ send_LCD_Command_Data(isData, 0x33);
	//~ send_LCD_Command_Data(isData, 0x33);

	//~ send_LCD_Command_Data(isCommand, 0xB7);		//
	//~ send_LCD_Command_Data(isData, 0x35); 

	//~ send_LCD_Command_Data(isCommand, 0xBB);		//
	//~ send_LCD_Command_Data(isData, 0x1F);

	//~ send_LCD_Command_Data(isCommand, 0xC0);		//PWCTR1 Power Control Setting
	//~ send_LCD_Command_Data(isData, 0x2C);

	//~ send_LCD_Command_Data(isCommand, 0xC2);		//PWCTR3 In Normal Mode (Full Clors)	(???)
	//~ send_LCD_Command_Data(isData, 0x01);

	//~ send_LCD_Command_Data(isCommand, 0xC3);		//PWCTR4 In Idle Mode (8-colors)
	//~ send_LCD_Command_Data(isData, 0x12);   

	//~ send_LCD_Command_Data(isCommand, 0xC4);		//PWCTR5 In Partial Mode + Full colors
	//~ send_LCD_Command_Data(isData, 0x20);

	//~ send_LCD_Command_Data(isCommand, 0xC6);		//
	//~ send_LCD_Command_Data(isData, 0x0F); 

	//~ send_LCD_Command_Data(isCommand, 0xD0);		//
	//~ send_LCD_Command_Data(isData, 0xA4);
	//~ send_LCD_Command_Data(isData, 0xA1);

	//~ send_LCD_Command_Data(isCommand, 0xE0);		//GAMCTRP1 Gamma adjustment
	//~ send_LCD_Command_Data(isData, 0xD0);
	//~ send_LCD_Command_Data(isData, 0x08);
	//~ send_LCD_Command_Data(isData, 0x11);
	//~ send_LCD_Command_Data(isData, 0x08);
	//~ send_LCD_Command_Data(isData, 0x0C);
	//~ send_LCD_Command_Data(isData, 0x15);
	//~ send_LCD_Command_Data(isData, 0x39);
	//~ send_LCD_Command_Data(isData, 0x33);
	//~ send_LCD_Command_Data(isData, 0x50);
	//~ send_LCD_Command_Data(isData, 0x36);
	//~ send_LCD_Command_Data(isData, 0x13);
	//~ send_LCD_Command_Data(isData, 0x14);
	//~ send_LCD_Command_Data(isData, 0x29);
	//~ send_LCD_Command_Data(isData, 0x2D);

	//~ send_LCD_Command_Data(isCommand, 0xE1);		//GAMCTRN1 Gamma adjustment
	//~ send_LCD_Command_Data(isData, 0xD0);
	//~ send_LCD_Command_Data(isData, 0x08);
	//~ send_LCD_Command_Data(isData, 0x10);
	//~ send_LCD_Command_Data(isData, 0x08);
	//~ send_LCD_Command_Data(isData, 0x06);
	//~ send_LCD_Command_Data(isData, 0x06);
	//~ send_LCD_Command_Data(isData, 0x39);
	//~ send_LCD_Command_Data(isData, 0x44);
	//~ send_LCD_Command_Data(isData, 0x51);
	//~ send_LCD_Command_Data(isData, 0x0B);
	//~ send_LCD_Command_Data(isData, 0x16);
	//~ send_LCD_Command_Data(isData, 0x14);
	//~ send_LCD_Command_Data(isData, 0x2F);
	//~ send_LCD_Command_Data(isData, 0x31);

	send_LCD_Command_Data(isCommand, 0x11);		//SLPOUT Sleep Out & Booster On
	//send_LCD_Command_Data(isCommand, 0x10);		//SLPIN Sleep In & Booster Off

	send_LCD_Command_Data(isCommand, 0x29);		//DISPON Main screen turn on
	//send_LCD_Command_Data(isCommand, 0x28);		//Main screen turn off
	set_LCD_BL(high);
}

/*This command causes the LCD module to enter the minimum power consumption mode. 
 * In this mode e.g. the DC/DC converter is stopped, Internal oscillator is stopped, 
 * and panel scanning is stopped.
 * MCU interface and memory are still working and the memory keeps its contents.
 * Sleep In Mode can only be left by the Sleep Out Command (11h).
 * Wait 120msec after sending Sleep Out command (when in Sleep In Mode) before Sleep In command can be sent.
 */
void set_Sleep_On_TFT() {
	send_LCD_Command_Data(isCommand, 0x10);
	set_LCD_BL(low);
}
/*
 * Wenn TFT aufwacht braucht es 5 ms um default values zu laden, dann erst werden andere Befehle
 * anerkannt. 
 */
void set_Sleep_Off_TFT() {
	//send_LCD_Command_Data(isCommand, 0x11);
	init_TFT();
}

inline void set_MOSI(uint8_t mosi) {
	if (mosi)
		setPin(DIN_PIN);
	else
		clrPin(DIN_PIN);
}

inline void set_Data_Command(uint8_t lcd_dc) {
	if (lcd_dc)
		setPin(DC_PIN);
	else
		clrPin(DC_PIN);
}

inline void set_LCD_CS(uint8_t lcd_cs) {
	if (lcd_cs)
		setPin(CS_PIN);
	else
		clrPin(CS_PIN);
}

/*
 * lcd_bl = 1: an, 0: aus
 */
void set_LCD_BL(uint8_t lcd_bl) {
	if (lcd_bl)
		setPin(BL_PIN);
	else
		clrPin(BL_PIN);
}

void send_LCD_Register(uint8_t reg, uint8_t dat) {
	send_LCD_Command_Data(isRegister, reg);
	send_LCD_Command_Data(isData, dat); 
}

/*
 * CS und Command/Data werden von SPI nicht gesetzt, deshalb selber machen.
 * SPSR und SPDR müssen ausgelesen werden, sonst wird SPIF nicht gelöscht.
 */
uint8_t send_LCD_Command_Data(uint8_t is_command_or_data, uint8_t val) {
	set_LCD_CS(low);
	set_Data_Command(is_command_or_data);
  SPDR = val;
  while (!(SPSR & (1<<SPIF)));
	val = SPSR;
	set_LCD_CS(high);
	return SPDR;
}

void drawWindow(uint16_t x, uint16_t y, uint16_t x1, uint16_t y1, uint16_t color) {
	uint32_t width = x1 - x + 1;
	uint32_t hight = y1 - y + 1;
	
	send_LCD_Command_Data(isCommand, CASET);	//colum address set
	send_LCD_Command_Data(isData, (x >> 8) & 0xFF);
	send_LCD_Command_Data(isData, x & 0xFF);
	send_LCD_Command_Data(isData, (x1 >> 8) & 0xFF);
	send_LCD_Command_Data(isData, x1 & 0xFF);
	send_LCD_Command_Data(isCommand, RASET);	//row address set
	send_LCD_Command_Data(isData, (y >> 8) & 0xFF);
	send_LCD_Command_Data(isData, y & 0xFF);
	send_LCD_Command_Data(isData, (y1 >> 8) & 0xFF);
	send_LCD_Command_Data(isData, y1 & 0xFF);
	
	send_LCD_Command_Data(isCommand, RAMWR);	//hier speichern der Farbe
	for (uint32_t i = 0; i < width * hight; ++i) {
		send_LCD_Command_Data(isData, (color >> 8) & 0xFF);
		send_LCD_Command_Data(isData, color & 0xFF);
	}
}

void drawPix(uint16_t x, uint16_t y, uint16_t color) {
	send_LCD_Command_Data(isCommand, CASET);	//colum address set
	send_LCD_Command_Data(isData, (x >> 8) & 0xFF);
	send_LCD_Command_Data(isData, x & 0xFF);
	send_LCD_Command_Data(isCommand, RASET);	//row address set
	send_LCD_Command_Data(isData, (y >> 8) & 0xFF);
	send_LCD_Command_Data(isData, y & 0xFF);
	send_LCD_Command_Data(isCommand, RAMWR);	//hier speichern der Farbe
	send_LCD_Command_Data(isData, (color >> 8) & 0xFF);
	send_LCD_Command_Data(isData, color & 0xFF);
}

uint8_t dimLevel[] = {0, 1, 2, 4, 8, 16, 32, 64, 128, 255};

void setBacklightDim(int8_t dim) {
	dim = dim > 9 ? 9 : dim < 0 ? 0 : dim;
	TCCR1A |= (1<<COM1A1) | (1<<WGM10);  //Set Timer Register 16-Bit Timer 1 an PB1
	TCCR1B |= (1<<WGM12) | (1<<CS11); 
	OCR1A = dimLevel[dim];
}

