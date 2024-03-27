#ifndef __DRI_I2C_H__
#define __DRI_I2C_H__

#include "stm32f10x.h"

void Dri_I2C_Init(void);

uint8_t Dri_I2C_Start(void);

void Dri_I2C_Stop(void);

void Dri_I2C_ACK(void);

void Dri_I2C_NACK(void);

uint8_t Dri_I2C_SendAddr(uint8_t addr);

uint8_t Dri_I2C_Transmit(uint8_t byte);

uint8_t Dri_I2C_ReadByte(void);

#endif

