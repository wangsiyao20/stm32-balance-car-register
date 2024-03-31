#ifndef __APP_CAR_H__
#define __APP_CAR_H__

#include "stm32f10x.h"

#include "Driver_Usart.h"
#include "stdio.h"
#include "Inf_OLED1315.h"
#include "Inf_TB6612.h"
#include "Int_Encoder.h"
#include "Inf_MPU6050.h"
#include "stdlib.h"
#include "math.h"
#include "Com_Filter.h"
#include "Dri_ADC.h"
#include "Com_PID.h"

extern float Acc_Angle; /* 通过加速度计算出来的倾角 */


void App_Car_Init(void);

void App_Car_OLED(void);

void App_Car_GetAngle_Encoder(void);

void App_Car_Control(void);

#endif

