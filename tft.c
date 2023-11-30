
/*
 *
*/

#include <stdint.h>
#include <util/delay.h>
#include "tft.h"
#include "uart_print.h"

uint16_t xDispMax;
uint16_t yDispMax;
uint16_t dispRotation;

uint8_t get_TP_IRQ();
uint8_t get_MISO();
void set_MOSI(uint8_t mosi);
void set_Data_Command(uint8_t lcd_dc);
void set_LCD_CS(uint8_t lcd_cs);
void set_LCD_BL(uint8_t lcd_bl);
void set_SD_CS(uint8_t sd_cs);
void set_TP_CS(uint8_t tp_cs);
void set_TP_IRQ(uint8_t tp_irq);

void send_LCD_Register(uint8_t reg, uint8_t dat);
uint8_t get_LCD_Val(uint8_t command_data, uint8_t bef);

#if defined(SPI_hard) 	//in tft.h
uint8_t send_LCD_Command_Data(uint8_t command_data, uint8_t val);
#else
void send_LCD_Command_Data(uint8_t command_data, uint8_t val);
#endif

static const uint8_t isData = 1;
static const uint8_t isCommand = 0;
static const uint8_t isRegister = 0;
static const uint8_t high = 1;
static const uint8_t low = 0;
static const uint16_t xrawmax = 2096,	//aus Versuchen ermitteln
											yrawmax = 2096,
											xrawmin = 176,
											yrawmin = 224;
					   
void init_Pins() {
	setOutput(SCK_PIN);
	setInput(MISO_PIN);
	setOutput(MOSI_PIN);
	setOutput(LCD_CS_PIN);
	setOutput(LCD_BL_PIN);
	setOutput(LCD_DC_PIN);
	setOutput(SD_CS_PIN);
	setOutput(TP_CS_PIN);
	setInput(TP_IRQ_PIN);
}

#if defined(SPI_hard)//in tft.h
/* SPDR
	 * Datenregister für zu sendende und empfangende Daten
	 * SPCR-Register
	 * CPOL = 0, Flanke steigend, dann fallend, sonst umgekehrt
	 * CPHA = 0, steigende Flanke abtasten, fallende Flanke setzen, sonst umgekehrt
	 * SPIE = 0 (Interupt disable)
	 * SPE = 1, SPI enable
	 * DORD = 0, MSB first
	 * MSTR = 1,Master-Mode
	 * SPR0, SPR1, Teiler für clock
	 * SPRX (im Register SPSR) Cloxk * 2
	 */
void init_SPI() {
	//ZUERST die Pins definieren!!!!!
	SPCR = (1<<SPE)|(1<<MSTR)/*|(1<<SPR0)*/;
	//SPSR = 1<<SPI2X;
}
#endif

/*
	Memory Access Control MADCTE
	D7 MY Page Address Order: 0 = Top to Botton, 1 = umgekehrt
	D6 MX Column Address Order: 0 Left to Right, 1 = umgekehrt
	D5 MV Page/Column Order: 0 = normal, 1 = revers
	D4 ML Line Address Order: 0 = Refresh top to Bottom,1 = umgekehrt
	D3 RGB RGB/BGR Order: 0 = RGB,1 = BGR
	D2 MH Display Data Latch Order: 0 = Refresh Left to Right, 1 = umgekehrt
	MY MX MV 
	0  0  0  normal				
	0  1  1  90° drehen
	1  1  0  180° drehen
	1  0  1  270° drehen
	send_LCD_Register(0x36, 0b01100000);
	*/
void init_TFT() {
/* init für ST7789 */
	set_Data_Command(isData);
	set_LCD_CS(high);
	set_LCD_BL(low);
	set_Data_Command(isCommand);
	set_LCD_BL(high);
	
	send_LCD_Command_Data(isCommand, 0x11);	//<Sleep out
	send_LCD_Command_Data(isCommand, 0x29);	//Display on
	send_LCD_Register(0x3a, 0x55);	//< COLMOD: Pixel Format Set
	send_LCD_Register(0x36, 0b00000000);	//< Memory Access Control Drehrichtung und RGB
	send_LCD_Register(0xc0,0x3c);	//< Power Control 1
	send_LCD_Register(0xc2,0x01);	//< Power Control 3
/* ENDE init für ST7789 */	
	
/* init für ILI9341 */
	//~ set_Data_Command(isData);
	//~ set_LCD_CS(high);
	//~ set_LCD_BL(low);
	//~ set_Data_Command(isCommand);
	//~ set_LCD_BL(high);
	
	  //~ _delay_ms(500);
    //~ send_LCD_Command_Data(isCommand, 0x01);
    //~ _delay_ms(200);

    //~ send_LCD_Command_Data(isCommand, 0xCF);
    //~ send_LCD_Command_Data(isData, 0x00);
    //~ send_LCD_Command_Data(isData, 0x8B);
    //~ send_LCD_Command_Data(isData, 0X30);

    //~ send_LCD_Command_Data(isCommand, 0xED);
    //~ send_LCD_Command_Data(isData, 0x67);
    //~ send_LCD_Command_Data(isData, 0x03);
    //~ send_LCD_Command_Data(isData, 0X12);
    //~ send_LCD_Command_Data(isData, 0X81);

    //~ send_LCD_Command_Data(isCommand, 0xE8);
    //~ send_LCD_Command_Data(isData, 0x85);
    //~ send_LCD_Command_Data(isData, 0x10);
    //~ send_LCD_Command_Data(isData, 0x7A);

    //~ send_LCD_Command_Data(isCommand, 0xCB);
    //~ send_LCD_Command_Data(isData, 0x39);
    //~ send_LCD_Command_Data(isData, 0x2C);
    //~ send_LCD_Command_Data(isData, 0x00);
    //~ send_LCD_Command_Data(isData, 0x34);
    //~ send_LCD_Command_Data(isData, 0x02);

    //~ send_LCD_Command_Data(isCommand, 0xF7);
    //~ send_LCD_Command_Data(isData, 0x20);

    //~ send_LCD_Command_Data(isCommand, 0xEA);
    //~ send_LCD_Command_Data(isData, 0x00);
    //~ send_LCD_Command_Data(isData, 0x00);

    //~ send_LCD_Command_Data(isCommand, 0xC0);                                                      /* Power control                */
    //~ send_LCD_Command_Data(isData, 0x1B);                                                   /* VRH[5:0]                     */

    //~ send_LCD_Command_Data(isCommand, 0xC1);                                                      /* Power control                */
    //~ send_LCD_Command_Data(isData, 0x10);                                                   /* SAP[2:0];BT[3:0]             */

    //~ send_LCD_Command_Data(isCommand, 0xC5);                                                      /* VCM control                  */
    //~ send_LCD_Command_Data(isData, 0x3F);
    //~ send_LCD_Command_Data(isData, 0x3C);

    //~ send_LCD_Command_Data(isCommand, 0xC7);                                                      /* VCM control2                 */
    //~ send_LCD_Command_Data(isData, 0xB7);

    //~ send_LCD_Command_Data(isCommand, 0x36);                                                      /* Memory Access Control        */
    //~ send_LCD_Command_Data(isData, 0x08);

    //~ send_LCD_Command_Data(isCommand, 0x3A);
    //~ send_LCD_Command_Data(isData, 0x55);

    //~ send_LCD_Command_Data(isCommand, 0xB1);
    //~ send_LCD_Command_Data(isData, 0x00);
    //~ send_LCD_Command_Data(isData, 0x1B);

    //~ send_LCD_Command_Data(isCommand, 0xB6);                                                      /* Display Function Control     */
    //~ send_LCD_Command_Data(isData, 0x0A);
    //~ send_LCD_Command_Data(isData, 0xA2);

    //~ send_LCD_Command_Data(isRegister, 0xF2);                                                      /* 3Gamma Function Disable      */
    //~ send_LCD_Command_Data(isData, 0x00);

    //~ send_LCD_Command_Data(isRegister, 0x26);                                                      /* Gamma curve selected         */
    //~ send_LCD_Command_Data(isData, 0x01);

    //~ send_LCD_Command_Data(isRegister, 0xE0);                                                      /* Set Gamma                    */
    //~ send_LCD_Command_Data(isData, 0x0F);
    //~ send_LCD_Command_Data(isData, 0x2A);
    //~ send_LCD_Command_Data(isData, 0x28);
    //~ send_LCD_Command_Data(isData, 0x08);
    //~ send_LCD_Command_Data(isData, 0x0E);
    //~ send_LCD_Command_Data(isData, 0x08);
    //~ send_LCD_Command_Data(isData, 0x54);
    //~ send_LCD_Command_Data(isData, 0XA9);
    //~ send_LCD_Command_Data(isData, 0x43);
    //~ send_LCD_Command_Data(isData, 0x0A);
    //~ send_LCD_Command_Data(isData, 0x0F);
    //~ send_LCD_Command_Data(isData, 0x00);
    //~ send_LCD_Command_Data(isData, 0x00);
    //~ send_LCD_Command_Data(isData, 0x00);
    //~ send_LCD_Command_Data(isData, 0x00);

    //~ send_LCD_Command_Data(isRegister, 0XE1);                                                      /* Set Gamma                    */
    //~ send_LCD_Command_Data(isData, 0x00);
    //~ send_LCD_Command_Data(isData, 0x15);
    //~ send_LCD_Command_Data(isData, 0x17);
    //~ send_LCD_Command_Data(isData, 0x07);
    //~ send_LCD_Command_Data(isData, 0x11);
    //~ send_LCD_Command_Data(isData, 0x06);
    //~ send_LCD_Command_Data(isData, 0x2B);
    //~ send_LCD_Command_Data(isData, 0x56);
    //~ send_LCD_Command_Data(isData, 0x3C);
    //~ send_LCD_Command_Data(isData, 0x05);
    //~ send_LCD_Command_Data(isData, 0x10);
    //~ send_LCD_Command_Data(isData, 0x0F);
    //~ send_LCD_Command_Data(isData, 0x3F);
    //~ send_LCD_Command_Data(isData, 0x3F);
    //~ send_LCD_Command_Data(isData, 0x0F);

    //~ send_LCD_Command_Data(isRegister, 0x11);                                                      /* Exit Sleep                   */
    //~ _delay_ms(120);
    //~ send_LCD_Command_Data(isRegister, 0x29);                                                      /* Display on                   */
		//~ set_LCD_BL(high);
/* ENDE init für ILI9341 */

}

//def ist Rotation = 0°
void set_TFT_rotation(uint16_t grad) {
	dispRotation = grad;
	switch (grad) {
		case 0: 	
			send_LCD_Register(0x36, 0b00000000);
			xDispMax = 240;
			yDispMax = 320; 
			break;
		case 90: 	
			send_LCD_Register(0x36, 0b01100000);
			xDispMax = 320;
			yDispMax = 240; 
			break;
		case 180: 
			send_LCD_Register(0x36, 0b11000000);
			xDispMax = 240;
			yDispMax = 320; 
			break;
		case 270: 
			send_LCD_Register(0x36, 0b10100000);
			xDispMax = 320;
			yDispMax = 240;  
			break;
		default: break;
	};
}

void init_Touch() {
	set_TP_CS(high);
	set_TP_IRQ(high);
}

inline uint8_t get_MISO() {
	return getPin(MISO_PIN);
}

//~ static uint8_t get_TP_IRQ() {
	//~ return getPin(TP_IRQ_PIN);
//~ }

inline void set_MOSI(uint8_t mosi) {
	if (mosi)
		setPin(MOSI_PIN);
	else
		clrPin(MOSI_PIN);
}

inline void set_Data_Command(uint8_t lcd_dc) {
	if (lcd_dc)
		setPin(LCD_DC_PIN);
	else
		clrPin(LCD_DC_PIN);
}

inline void set_LCD_CS(uint8_t lcd_cs) {
	if (lcd_cs)
		setPin(LCD_CS_PIN);
	else
		clrPin(LCD_CS_PIN);
}

inline void set_LCD_BL(uint8_t lcd_bl) {
	if (lcd_bl)
		setPin(LCD_BL_PIN);
	else
		clrPin(LCD_BL_PIN);
}

inline void set_SD_CS(uint8_t sd_cs) {
	if (sd_cs)
		setPin(SD_CS_PIN);
	else
		clrPin(SD_CS_PIN);
}

inline void set_TP_CS(uint8_t tp_cs) {
	if (tp_cs)
		setPin(TP_CS_PIN);
	else
		clrPin(TP_CS_PIN);
}

inline void set_TP_IRQ(uint8_t tp_irq) {
	if (tp_irq)
		setPin(TP_IRQ_PIN);
	else
		clrPin(TP_IRQ_PIN);
}

void send_LCD_Register(uint8_t reg, uint8_t dat) {
	send_LCD_Command_Data(isRegister, reg);
	send_LCD_Command_Data(isData, dat); 
}

#if defined(SPI_hard)	//in tft.h
//SPI-Version
/*
 * CS und Command/Data werden von SPI nicht gesetzt, deshalb selber machen.
 * SPSR und SPDR müssen ausgelesen werden, sonst wird SPIF nicht gelöscht.
 */
uint8_t send_LCD_Command_Data(uint8_t is_command_data, uint8_t val) {
	set_LCD_CS(low);
	set_Data_Command(is_command_data);
  SPDR = val;
  while (!(SPSR & (1<<SPIF)));
	val = SPSR;
	set_LCD_CS(high);
	return SPDR;
}

/*
 * CS und Command/Data werden von SPI nicht gesetzt, deshalb selber machen.
 * SPSR und SPDR müssen ausgelesen werden, sonst wird SPIF nicht gelöscht.
 */
uint8_t get_LCD_Val(uint8_t is_command_data, uint8_t val) {
	set_LCD_CS(low);
	set_Data_Command(is_command_data);
  SPDR = val;
//send command
  while (!(SPSR & (1<<SPIF)));
	val = SPSR;
	SPDR = 0;
//reseive data
	while (!(SPSR & (1<<SPIF)));
	val = SPSR;
	set_LCD_CS(high);
	return SPDR;
}

#else
/*
 * Software-SPI
 * is_command_data: ob Command (auch bei Register-Nr.) oder Data gesendet wird
 * val: Command/Register oder Data
 */
void send_LCD_Command_Data(uint8_t is_command_data, uint8_t val) {	
	
	set_LCD_CS(low);
	clrPin(SCK_PIN);
	set_Data_Command(is_command_data);

	if (val & 0b10000000)
		setPin(MOSI_PIN);
	else
		clrPin(MOSI_PIN);
	setPin(SCK_PIN);
	clrPin(SCK_PIN);
	
	if (val & 0b01000000)
		setPin(MOSI_PIN);
	else
		clrPin(MOSI_PIN);
	setPin(SCK_PIN);
	clrPin(SCK_PIN);
	
	if (val & 0b00100000)
		setPin(MOSI_PIN);
	else
		clrPin(MOSI_PIN);
	setPin(SCK_PIN);
	clrPin(SCK_PIN);
	
	if (val & 0b00010000)
		setPin(MOSI_PIN);
	else
		clrPin(MOSI_PIN);
	setPin(SCK_PIN);
	clrPin(SCK_PIN);
	
	if (val & 0b00001000)
		setPin(MOSI_PIN);
	else
		clrPin(MOSI_PIN);
	setPin(SCK_PIN);
	clrPin(SCK_PIN);
	
	if (val & 0b00000100)
		setPin(MOSI_PIN);
	else
		clrPin(MOSI_PIN);
	setPin(SCK_PIN);
	clrPin(SCK_PIN);
	
	if (val & 0b00000010)
		setPin(MOSI_PIN);
	else
		clrPin(MOSI_PIN);
	setPin(SCK_PIN);
	clrPin(SCK_PIN);
	
	if (val & 0b00000001)
		setPin(MOSI_PIN);
	else
		clrPin(MOSI_PIN);
	setPin(SCK_PIN);
	clrPin(SCK_PIN);

	set_LCD_CS(high);
}

uint8_t get_LCD_Val(uint8_t is_command_data, uint8_t val) {
	uint8_t maske = 0b10000000;
	uint8_t erg = 0;
	
	set_LCD_CS(low);
	set_Data_Command(is_command_data);
	
	do {
		if (val & maske)
			setPin(MOSI_PIN);
		else
			clrPin(MOSI_PIN);
		setPin(SCK_PIN);
		clrPin(SCK_PIN);
		maske >>= 1;
	} while (maske > 0);
	
	setPin(MOSI_PIN);		//ist besser
	maske = 0b10000000;
	
	do {
		setPin(SCK_PIN);
		if (getPin(MISO_PIN))
			erg |= maske;
		clrPin(SCK_PIN);
		maske >>= 1;
	} while(maske > 0);
	
	set_LCD_CS(high);
	return erg;
}

#endif

uint16_t get_Touch_Val(uint16_t command) {
	uint16_t erg = 0;
	set_TP_CS(low);
  SPDR = command;
  __asm__ volatile("nop");
  while (!(SPSR & (1<<SPIF)));
  erg = SPDR;
  SPDR = 0;
  __asm__ volatile("nop");
  while (!(SPSR & (1<<SPIF)));
  erg = SPDR << 4;
  SPDR = 0;
  __asm__ volatile("nop");
  while (!(SPSR & (1<<SPIF)));
  erg += SPDR;
  set_TP_CS(high); 
	return erg;
}

void getTouchPos(uint16_t *x, uint16_t *y, uint16_t *z) {
	int16_t xraw = 0, yraw = 0;
	switch (dispRotation) {
		case 0:
			xraw = get_Touch_Val(0xD1);
			yraw = get_Touch_Val(0x91);
			break;
		case 90:
			xraw = get_Touch_Val(0x91);
			yraw = yrawmax - get_Touch_Val(0xD1);
			break;
		case 180:																	//?????????? Testen!
			xraw = xrawmax - get_Touch_Val(0xD1);
			yraw = yrawmax - get_Touch_Val(0x91);
			break;
		case 270:																	//?????????? Testen!
			xraw = xrawmax - get_Touch_Val(0x91);
			yraw = get_Touch_Val(0xD1);
			break;
		default: break;
	}
	*z = get_Touch_Val(0xB0);	//>200
	//*z2 = get_Touch_Val(0xC0);

	*x = (int32_t)(xraw - xrawmin) * xDispMax / (int32_t)(xrawmax - xrawmin);
	*y = (int32_t)(yraw - yrawmin) * yDispMax / (int32_t)(yrawmax - yrawmin);
	*x = xDispMax - *x;
	*y = yDispMax - *y;
}

inline uint8_t get_High_Byte(uint16_t val) {
	return (uint8_t)(val >> 8) & 0xFF;
}

inline uint8_t get_Low_Byte(uint16_t val) {
	return (uint8_t) val & 0xFF;
}

void drawWindow(uint16_t x, uint16_t y, uint16_t x1, uint16_t y1, uint16_t color) {
	uint32_t width = x1 - x + 1;
	uint32_t hight = y1 - y + 1;
	
	send_LCD_Command_Data(isCommand, CASET);	//colum address set
	send_LCD_Command_Data(isData, (x >> 8) & 0xFF);
	send_LCD_Command_Data(isData, x & 0xFF);
	send_LCD_Command_Data(isData, (x1 >> 8) & 0xFF);
	send_LCD_Command_Data(isData, x1 & 0xFF);
	send_LCD_Command_Data(isCommand, PASET);	//row address set
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
	send_LCD_Command_Data(isCommand, PASET);	//row address set
	send_LCD_Command_Data(isData, (y >> 8) & 0xFF);
	send_LCD_Command_Data(isData, y & 0xFF);
	send_LCD_Command_Data(isCommand, RAMWR);	//hier speichern der Farbe
	send_LCD_Command_Data(isData, (color >> 8) & 0xFF);
	send_LCD_Command_Data(isData, color & 0xFF);
}

uint8_t dimLevel[] = {0, 1, 2, 3, 4, 8, 16, 32, 64, 255};

int8_t setBacklightDim(int8_t dim) {
	dim = dim > 9 ? 9 : dim;
	dim = dim < 0 ? 0 : dim;
	TCCR1A |= (1<<COM1A1) | (1<<WGM10);  //Set Timer Register   
	TCCR1B |= (1<<WGM12) | (1<<CS11); 
	OCR1A = dimLevel[dim];
	return dim;
}
