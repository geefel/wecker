#include "rtc.h"
#include "i2c.h"

static uint8_t bcd2bin(uint8_t val);
static uint8_t bin2bcd(uint8_t val);

//Convert a binary coded decimal value to binary. RTC stores time/date values as BCD.
static uint8_t bcd2bin(uint8_t val) {
	return val - 6 * (val >> 4);
}

//Convert a binary value to BCD format for the RTC registers
static uint8_t bin2bcd(uint8_t val) {
	return val + 6 * (val / 10);
}

/*
 * Variation of Sillke for the Gregorian calendar.
 * With 0=Sunday, 1=Monday, ... 6=Saturday
*/
int dayofweek(int day, int month, int year) {
	if ((month -= 2) <= 0) {
		month += 12;
		year--;
	}
	return (83 * month / 32 + day + year + year / 4 - year / 100 + year / 400) % 7;
}

/*
 * Startet Oszilator mit der Frequenz freq.
 * Freq: 0 = 1 Hz, 1 = 4096 Hz, 2 = 8192 Hz, 3 = 32768 Hz 
 */
void setSquarOn(int8_t freq) {
	start_I2C();
	send_I2C_Address(DS1307_ADDRESS, 0);
	send_I2C_Data(DS1307_CONTROL);
	send_I2C_Data(SQ_ON + freq);
	stop_I2C();
}

/*
 * stoppt Oszilator, setzt SquarPin auf low
 */
void setSquarOff() {
	start_I2C();
	send_I2C_Address(DS1307_ADDRESS, 0);
	send_I2C_Data(DS1307_CONTROL);
	send_I2C_Data(SQ_OFF);
	stop_I2C();
}

/*
 * setzt SquarPin auf High, startet NICHT den Oszilator
 */
void setSquarPinHigh() {
	start_I2C();
	send_I2C_Address(DS1307_ADDRESS, 0);
	send_I2C_Data(DS1307_CONTROL);
	send_I2C_Data(SQ_PIN_HIGH);
	stop_I2C();
}

void setClock(struct wecker_ *data) {
	start_I2C();
	send_I2C_Address(DS1307_ADDRESS, 0);
	send_I2C_Data(0);													//Registerplatz auf den geschrieben werden soll angeben
	send_I2C_Data(bin2bcd(data->uhrSec));
	send_I2C_Data(bin2bcd(data->uhrMM));
	send_I2C_Data(bin2bcd(data->uhrHH));
	send_I2C_Data(bin2bcd(dayofweek(data->datumDD, data->datumMM, data->datumYY + 2000)));
	send_I2C_Data(bin2bcd(data->datumDD));
	send_I2C_Data(bin2bcd(data->datumMM));
	send_I2C_Data(bin2bcd(data->datumYY));
	stop_I2C();
}

void getNow(struct wecker_ *data) {
	start_I2C();
	send_I2C_Address(DS1307_ADDRESS, 0);
	send_I2C_Data(0);													//Registerplatz von dem gelesen werden soll angeben
	//stop_I2C();															//brauchen wir hier nicht, weil es weiter geht
	start_I2C();
	send_I2C_Address(DS1307_ADDRESS, 1);
	data->uhrSec = bcd2bin(read_I2C_Data_ACK() & 0x7F);	//aus Register 0x00 holen. Chip geht jetzt automatisch nach Register 0x01
	data->uhrMM = bcd2bin(read_I2C_Data_ACK());				//aus Register 0x01 holen. Chip geht jetzt automatisch nach Register 0x02. usw.
	data->uhrHH = bcd2bin(read_I2C_Data_ACK());
	data->tag = bcd2bin(read_I2C_Data_ACK());
	data->datumDD  = bcd2bin(read_I2C_Data_ACK());
	data->datumMM = bcd2bin(read_I2C_Data_ACK());
	data->datumYY = bcd2bin(read_I2C_Data_NACK());				//Daten holen MUSS bei letztem Byte mit NACK erfolgen 
	stop_I2C();
}

uint8_t getSecondFast() {  //die Sekunden separat holen
	uint8_t erg;
	start_I2C();
	send_I2C_Address(DS1307_ADDRESS, 0);
	send_I2C_Data(0);
	//stop_I2C();
	start_I2C();
	send_I2C_Address(DS1307_ADDRESS, 1);
	erg = bcd2bin(read_I2C_Data_NACK() & 0x7F);
	stop_I2C();
	return erg;	
}


