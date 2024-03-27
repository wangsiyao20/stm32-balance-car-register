#ifndef __INT_ENCODER_H__
#define __INT_ENCODER_H__

#include "stm32f10x.h"


void Int_Encoder_Init(void);

int Int_Encoder_ReadCounter(uint8_t timx);

#endif

