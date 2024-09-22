#ifndef WAVPLAYER_H
#define WAVPLAYER_H

#include <stdint.h>

void sendCMD(uint8_t cmd, uint8_t feedback, uint16_t para);
uint16_t receiveInfo(uint8_t cmd, uint8_t feedback, uint16_t para);

//~ void playNext();
//~ void playPrev();
//~ void playTrackInRoot(uint16_t p);
//~ void volPlus();
//~ void volMinus();
void volSet(uint8_t p);
void eqSet(uint8_t p);
void repeatMode(uint8_t p);
//~ void pbSource(uint8_t p);
//~ void enterStandby();
//~ void normalWorking();
//~ void resetPlayer();
//~ void play();
void pausePlay();
void playFoldersTrack(uint8_t folder, uint8_t track);
//~ void volAdjustSet(uint8_t vol);
//~ void repeatSet(uint8_t r);
//~ uint8_t sendInitPara();
//~ uint8_t getError();
//~ uint8_t getStatus();
uint16_t getVol();
uint8_t getEQ();
//~ uint8_t getPlayMode();
//~ uint8_t getSoftVers();
uint16_t getNumTfCardFiles();
//~ uint16_t getNumUDisk();
//~ uint16_t getNumFlashFiles();
//~ uint8_t getKeepOn();
//~ uint16_t getCurrentTfCardTrack();
//~ uint16_t getCurrentUDiskTrack();
//~ uint16_t getCurrentFlashTrack();

#endif  //WAVPLAYER_H