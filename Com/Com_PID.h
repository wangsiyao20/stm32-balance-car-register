#ifndef __COM_PID_H__
#define __COM_PID_H__

#include "stm32f10x.h"

int Com_PID_Balance(float angle, float gryo, float balance_angle, float Kp, float Kd);

int Com_PID_Velocity(int Encoder_A, int Encoder_B, float Kp, float Ki);

int Com_PID_Turn(float Z_Angle,float Kp);


#endif

