#ifndef __INF_TB6612_H__
#define __INF_TB6612_H__

#include "stm32f10x.h"
#include "Dri_TIM.h"

/* 运动方向 */
enum MOTOR {
    GO = 0,         // 前进
    BACK,           // 后退
    BRAKE           // 刹车
};


/* 决定电机正转/反转的 */
#define Ain1(x)  if(x) GPIOB->ODR |= GPIO_ODR_ODR14; else GPIOB->ODR &= ~GPIO_ODR_ODR14
#define Ain2(x)  if(x) GPIOB->ODR |= GPIO_ODR_ODR15; else GPIOB->ODR &= ~GPIO_ODR_ODR15
#define Bin1(x)  if(x) GPIOB->ODR |= GPIO_ODR_ODR13; else GPIOB->ODR &= ~GPIO_ODR_ODR13
#define Bin2(x)  if(x) GPIOB->ODR |= GPIO_ODR_ODR12; else GPIOB->ODR &= ~GPIO_ODR_ODR12



void Inf_TB6612_Init(void);

void Inf_TB6612_SetPWM(int pwma, int pwmb);

#endif

