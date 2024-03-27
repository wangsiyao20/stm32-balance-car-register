#ifndef __DRIVER_USART_H_
#define __DRIVER_USART_H_
#include "stm32f10x.h"
#include "stdio.h"

/**
 * ��ʼ������1
*/
void Driver_USART1_Init(void);

/**
 * ����һ���ֽ�
*/
void Driver_USART1_SendChar(uint8_t ch);

/**
 * ����һ���ַ���
*/
void Driver_USART1_SendStr(uint8_t*, uint8_t);

/**
 * ����һ���ַ�
*/
uint8_t Driver_USART1_ReceiveChar(void);

/**
 * ����һ���ַ���
*/
void Driver_USART1_ReceiveString(uint8_t* buff, uint8_t *len);



#endif
