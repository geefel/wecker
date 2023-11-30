/*
 * R/W=1 -> Lesen; RW=0 -> Schreiben
 * MSB: Most Significant Bit
 * alle Datenpakete sind 9 bit lang: 8 Datenbit, 1 ACK
 * alle Adresspakete sind 9 bit lang: 7 Adressbit, 1 RW-bit, 1 ACK
 * lesen: R = 1
 * schreiben: W = 0
 * ACK: Slave setzt Leitung auf Low (SCL ist gerade high)
 * Adresse 0000 000 ist reserviert, hiermit werden alle Slaves angesprochen
 * Übertragung: START, Adresspaket, Datenpaket, STOP
 * START: SCL ist high, SDA wechselt nach low
 * STOP: SCL ist nach ACK-Takt wieder(!) auf high, SDA wechselt nach high
 * 
 * SCL-Rate wird durch das TWBR (TWI Bit Rate Register) und TWPS - TWI Prescaler Bits im TWI Status Register festgelegt.
 * Interne Taktfrequenz der Slaves muss mindestens 16 mal höher als die Taktrate des Bus.
 * Reihenfolge: Datum setzen, Clock high, Clock low, Datum setzen, ...  
 * MC hat je ein Daten- und Adressschieberegister (TWAR (TWI Adress Register)), einen START/STOP Controller und die Arbitrierungslogik
 * 
 * "Adress Match Unit". Diese überwacht, ob die empfangenen Adress-Bytes mit denen im TWAR (TWI Adress Register) übereinstimmen.
 * Wenn TWGCE (TWI General Call Recognition Enable) in TWAR gesetzt ist, wird überprüft, ob ein allgemeiner Ruf (0000 000) vorliegt.
 * TWCR (TWI Control Register):
 * 	TWINT (TWI Interrupt Flag): wird gesetzt u.a. START, STOP, Adresse oder Daten usw.
 * 	wenn TWINT gesetzt steht in TWSR Infos über Empfangenes
 */




#include <util/twi.h>
#include "i2c.h"

#define IS_START 0x08
#define IS_REP_START 0x10
#define IS_SL_ADR_W_ACK 0x18
#define IS_SL_ADR_W_NOT_ACK 0x20
#define IS_MT_DATA_W_ACK	0x28
#define IS_MT_DATA_W_NOT_ACK	0x30

//frequenz: die Übertrageungsfrequenz die Slave haben möchte
void set_I2C_Clock(uint32_t frequency) {
	TWBR = ((F_CPU / frequency) - 16) / 2;
	setOutput(I2C_SCL_PIN);
	setOutput(I2C_SDA_PIN);
}

uint8_t start_I2C() {
	TWCR = TWCR = (1 << TWINT) | (1 << TWSTA) | (1 << TWEN);	//Sende Start-Bedingung 
	while  (!(TWCR & (1<<TWINT)));								//Warte bis TWINT-Bit gesetzt. Dann wurde die Startbedingung gesendet. 
	if ((TWSR & 0xF8)	!= IS_START)							//Prüfe Inhalt des TWI-Statusregisters ohne die Vorteiler-Bits. Wenn nicht START dann FEHLER
		return 0;
  return 1;
}

//rw = 1 -> Lesen; rw = 0 -> Schreiben
uint8_t send_I2C_Address(uint8_t adr, uint8_t rw) {
	TWDR = adr << 1 | rw;										//Lade SLA_W ins TWDR-Register. 
	TWCR = (1<<TWINT) | (1<<TWEN);								//Lösche das TWINT-Bit im TWCR zum Absenden der Adresse 
	while (!(TWCR & (1<<TWINT)));								//Warte bis TWINT-Bit gesetzt. Dann wurde SLA+W abgesendet, und ACK oder NACK wurde empfangen.
	if ((TWSR & 0xF8) != IS_SL_ADR_W_ACK)						//Prüfe Inhalt des TWI-Statusregisters ohne die Vorteiler-Bits. Wenn nicht MT_SLA_ACK dann FEHLER 
    return 0;
  return 1;
}

uint8_t send_I2C_Data(uint8_t data) {
	TWDR = data;												//Lade Daten ins TWDR-Register. 
	TWCR = (1<<TWINT) | (1<<TWEN);								//Lösche das TWINT-Bit im TWCR zum Absenden
	while (!(TWCR &  (1<<TWINT)));								//Warte bis TWINT-Bit gesetzt. Dann sind die Daten abgesendet und ACK oder NACK empfangen worden. 
	if ((TWSR & 0xF8) != IS_MT_DATA_W_ACK)						//Prüfe Inhalt des TWI-Statusregisters ohne die Vorteiler-Bits. Wenn nicht MT_DATA_ACK dann FEHLER
    return 0;
  return 1;
}

uint8_t read_I2C_Data_ACK() {
	TWCR = (1<<TWINT) | (1<<TWEN) | (1<<TWEA);
	while(!(TWCR & (1<<TWINT)));    
  return TWDR;
}

uint8_t read_I2C_Data_NACK() {									//Daten holen MUSS (manchmal)bei letztem Byte mit NACK erfolgen 
	TWCR = (1<<TWINT) | (1<<TWEN);
	while(!(TWCR & (1<<TWINT)));    
  return TWDR;
}

void stop_I2C() {
	TWCR = (1<<TWINT) | (1<<TWEN) | (1<<TWSTO);					//Sende Stopp-Bedingung
}
