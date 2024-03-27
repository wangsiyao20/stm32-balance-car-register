#ifndef __DRI_SPI_H__
#define __DRI_SPI_H__

#include "stm32f10x.h"

/* CS: PA4 */
#define CS_HIGH() (GPIOA->ODR |= GPIO_ODR_ODR4)
#define CS_LOW() (GPIOA->ODR &= ~GPIO_ODR_ODR4)

/* CLK: PA5 */
#define SCK_HIGH() (GPIOA->ODR |= GPIO_ODR_ODR5)
#define SCK_LOW() (GPIOA->ODR &= ~GPIO_ODR_ODR5)

/* MOSI: PA7 */
#define MOSI_HIGH() (GPIOA->ODR |= GPIO_ODR_ODR7)
#define MOSI_LOW() (GPIOA->ODR &= ~GPIO_ODR_ODR7)

void Dri_SPI_Init(void);

void Dri_SPI_WriteByte(uint8_t byte);



#endif


