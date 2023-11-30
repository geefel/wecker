#include "wavPlayer.h"
#include <util/delay.h>
//~ #include "uart_hard.h"
//~ #include "uart_print.h"
#include "uart_soft_PlayerMini.h"
/*
   Format:START VER Len CMD Feedback para1 para2 checksum END
   Start: 0x7E
   VER:0xFF
   Len: alle Bytes außer Start, End und checksum
   CMD: Command
   Feedback?: 1 oder 0	:wird Antwort erwartet?
   para1: high data byte
   para2: low data Byte
   checksum: ohne Start und End
   End: 0xEF
*/

void sendCMD(uint8_t cmd, uint8_t feedback, uint16_t para) {
	uint8_t len = 6;
	uint8_t paraMSB = para >> 8;
	uint8_t paraLSB = (uint8_t)(para & 0xFF);
	uint16_t checkSum = 0xFFFF - (0xFF + len + cmd + feedback + paraMSB + paraLSB) + 1;
	uint8_t checkMSB = checkSum >> 8;
	uint8_t checkLSB = (uint8_t)(checkSum & 0xFF);

	uint8_t sendArr[] = {0x7E, 0xFF, 6, cmd, feedback, paraMSB, paraLSB, checkMSB, checkLSB, 0xEF};
	for (uint8_t i = 0; i < 10; ++i)
		sendDat(sendArr[i]);
}

uint16_t receiveInfo(uint8_t cmd, uint8_t feedback, uint16_t para) {
	uint8_t erg[10] = {0};
	ergNr = 0;
	isINT0_Interrupt = 0;
	INT0_RESET;		//Interrupt-Flag löschen
	INTERRUPT_INT0_ENABLE;
	INTERRUPT_TIMER0_ENABLE;
	
	sendCMD(cmd, feedback, para);
	receiveDat(erg);
	
	INTERRUPT_INT0_DISABLE;
	INTERRUPT_TIMER0_DISABLE;
//~ /*!debuging*/
	//~ for(int i = 0; i < 10; i++)
		//~ pInt(erg[i]);
//~ /*!debuging*/
	return (erg[5] << 8) + erg[6];
}

//-----------Befehle ohne Antwort----------
//~ /*CMD 0x01*/
//~ void playNext() {
  //~ sendCMD(0x01, 1, 0);
//~ }
//~ /*CMD 0x02*/
//~ void playPrev() {
    //~ sendCMD(0x02, 1, 0);
//~ }
//~ /*CMD 0x03 0-2999*/
//~ void playTrackInRoot(uint16_t p) {
  //~ sendCMD(0x03, 1, p);
//~ }
//~ /*CMD 0x04*/
//~ void volPlus() {
	//~ sendCMD(0x04, 1, 0);
//~ }
//~ /*CMD 0x05*/
//~ void volMinus() {
	//~ sendCMD(0x05, 1, 0);
//~ }
/*CMD 0x06; 0 bis 30 Volumen*/
void volSet(uint8_t p) {
  sendCMD(0x06, 1, p);
}
/*CMD 0x07; 0 bis 5: Normal/Pop/Rock/Jazz/Classic/Base*/
void eqSet(uint8_t p) {
	sendCMD(0x07, 1, p);
}
/*CMD 0x08; 0 bis 3: noRepeat/repeatFolder/repeatSingle/repeatRandom*/
void repeatMode(uint8_t p) {
	sendCMD(0x08, 1, p);
}
//~ /*CMD 0x09; playbackSorce 0 bis 4: U/TF/AUX/SLEEP/FLASH*/
//~ void pbSource(uint8_t p) {
	//~ sendCMD(0x09, 1, p);
//~ }
//~ /*CMD 0xA; Enter into standby – low power loss*/
//~ void enterStandby() {
	//~ sendCMD(0x0A, 1, 0);
//~ }
//~ /*CMD 0x0B; Normal working*/
//~ void normalWorking() {
	//~ sendCMD(0x0B, 1, 0);
//~ }
//~ /* CMD 0x0C; Reset Modul*/
//~ void resetPlayer() {
	//~ sendCMD(0x0C, 1, 0);
//~ }
//~ /*CMD 0x0D; Playback*/
//~ void play() {
	//~ sendCMD(0x0D, 1, 0);
//~ }
/*CMD 0x0E; Pause*/
void pausePlay() {
	sendCMD(0x0E, 1, 0);
}
/*CMD 0x0F; track in a folder*/
void playFoldersTrack(uint8_t folder, uint8_t track) {
	uint16_t p = (folder << 8) | track;
  sendCMD(0x0F, 1, p);
}
//~ /*CMD 0x10; Volume adjust set: highByte = 1, lowByte = 0 bis 31*/
//~ void volAdjustSet(uint8_t vol) {
	//~ uint16_t p = 0x0100 | vol;
	//~ sendCMD(0x10, 1, p);
//~ }

//~ /*CMD 0x11; Repeat play: 1:start repeat play, 0:stop play*/
//~ void repeatSet(uint8_t p) {
	//~ sendCMD(0x11, 1, p);
//~ }
//-----------Befehle mit Antwort----------
//
//~ /*CMD 0x3A; Return data:
	//~ 01 USB flash drive is plugged in 
	//~ 02 SD card is plugged in
	//~ 04 USB cable connected to PC is plugged in*/
//~ /*CMD 0x3B; Return data:
	//~ 01 USB flash drive is pulled out
	//~ 02 SD card is pulled out
	//~ 04 USB cable connected to PC is pulled out*/
//~ /*CMD 0x3C STAY	N/A Return data after Track finished, data: trackNr, USB flash drive*/
//~ /*CMD 0x3D STAY N/A Return data after Track finished, data: trackNr, SD-Card flash drive*/
//~ /*CMD 0x3E STAY N/A*/

//~ /*CMD 0x3F Returns witch Dev is online. 0: non, 1: USB, 2: SD-Card, 4: PC*/
//~ uint8_t sendInitPara() {
	//~ return receiveInfo(0x43, 0, 0);
//~ }

//~ /*CMD 0x40 Returns an error, request retransmission
	//~ 01 Module busy (this info is returned when the initialization is not done)
	//~ 02 Currently sleep mode (supports only specified device in sleep mode)
	//~ 03 Serial receiving error (a frame has not been received completely yet)
	//~ 04 Checksum incorrect
	//~ 05 Specified track is out of current track scope
	//~ 06 Specified track is not found
	//~ 07 Insertion error (an inserting operation only can be done when a track is being played)
	//~ 08 SD card reading failed (SD card pulled out or damaged)
	//~ 0A Entered into sleep mode*/
//~ uint8_t getError() {
	//~ return receiveInfo(0x40, 0, 0);
//~ }

//~ /*CMD 0x41 ACK from Module after every message*/

//~ /*CMD 0x42 Query the current status*/
//~ uint8_t getStatus() {
	//~ return receiveInfo(0x42, 0, 0);
//~ }
/*CMD 0x43 Query the current volume*/
uint16_t getVol() {
	return receiveInfo(0x43, 0, 0);
}
/*CMD 0x44 0 bis 5: Normal/Pop/Rock/Jazz/Classic/Base*/
uint8_t getEQ() {
	return receiveInfo(0x44, 0, 0);
}
//~ /*CMD 0x45 0 bis 3: noRepeat/repeatFolder/repeatSingle/repeatRandom*/
//~ uint8_t getPlayMode() {
	//~ return receiveInfo(0x45, 0, 0);
//~ }
//~ /*CMD 0x46 Query the current software version*/
//~ uint8_t getSoftVers() {
	//~ return receiveInfo(0x46, 0, 0);
//~ }
//~ /*CMD 0x47 Query the total number of U-Disk*/
//~ uint16_t getNumUDisk() {
	//~ return receiveInfo(0x47, 0, 0);
//~ }
/*CMD 0x48 Query the total number of TF cardfiles*/
uint16_t getNumTfCardFiles() {
	return receiveInfo(0x48, 0, 0);
}
//~ /*CMD 0x49 Query the total number of flash-files*/
//~ uint16_t getNumFlashFiles() {
	//~ return receiveInfo(0x49, 0, 0);
//~ }
//~ /*CMD 0x4A Keep on*/
//~ uint8_t getKeepOn() {
	//~ return receiveInfo(0x4A, 0, 0);
//~ }
//~ /*CMD 0x4B Queries the current track of TF card*/
//~ uint16_t getCurrentTfCardTrack() {
	//~ return receiveInfo(0x4B, 0, 0);
//~ }
//~ /*CMD 0x4C Queries the current track of U-Disk*/
//~ uint16_t getCurrentUDiskTrack() {
	//~ return receiveInfo(0x4C, 0, 0);
//~ }
//~ /*CMD 0x4D Queries the current track of Flash*/
//~ uint16_t getCurrentFlashTrack() {
	//~ return receiveInfo(0x4D, 0, 0);
//~ }
