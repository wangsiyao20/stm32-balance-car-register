#ifndef __DRIVER_USART_H_
#define __DRIVER_USART_H_
#include "stm32f10x.h"
#include "stdio.h"

/**
 * 初始化串口1
*/
void Driver_USART1_Init(void);

/**
 * 发送一个字节
*/
void Driver_USART1_SendChar(uint8_t ch);

/**
 * 发送一个字符串
*/
void Driver_USART1_SendStr(uint8_t*, uint8_t);

/**
 * 接收一个字符
*/
uint8_t Driver_USART1_ReceiveChar(void);

/**
 * 接收一个字符串
*/
void Driver_USART1_ReceiveString(uint8_t* buff, uint8_t *len);



#endif
