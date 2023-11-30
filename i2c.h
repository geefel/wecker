#ifndef I2C_H
#define I2C_H

#include <stdint.h>
#include "pin.h"

#define I2C_SCL_PIN PINDEF(C, 5)
#define I2C_SDA_PIN PINDEF(C, 4)

void set_I2C_Clock(uint32_t frequency);
uint8_t start_I2C();
uint8_t send_I2C_Address(uint8_t adr, uint8_t rw);	//rw = 1 -> Lesen; rw = 0 -> Schreiben
uint8_t send_I2C_Data(uint8_t data);
uint8_t read_I2C_Data_ACK();
uint8_t read_I2C_Data_NACK();
void stop_I2C();

#endif //I2C_H
