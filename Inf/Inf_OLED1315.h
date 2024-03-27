#ifndef __INF_OLED1315_H__
#define __INF_OLED1315_H__

#include "stm32f10x.h"
#include "Dri_SPI.h"


/* RES: PB0 */
#define RES_HIGH() (GPIOB->ODR |= GPIO_ODR_ODR0)
#define RES_LOW() (GPIOB->ODR &= ~GPIO_ODR_ODR0)

/* DC: PB1 */
#define DC_HIGH() (GPIOB->ODR |= GPIO_ODR_ODR1)
#define DC_LOW() (GPIOB->ODR &= ~GPIO_ODR_ODR1)

#define OLED_CMD  0 	//Ð´ÃüÁî
#define OLED_DATA 1 	//Ð´Êý¾Ý


void Inf_OLED1315_WR_Byte(uint8_t dat, uint8_t mode);
void Inf_OLED1315_DisPlay_On(void);
void Inf_OLED1315_DisPlay_Off(void);
void Inf_OLED1315_Refresh(void);
void Inf_OLED1315_Clear(void);
void Inf_OLED1315_DrawPoint(uint8_t x, uint8_t y, uint8_t t);
void Inf_OLED1315_ShowChar(uint8_t x, uint8_t y, uint8_t chr, uint8_t size1, uint8_t mode);
void Inf_OLED1315_ShowString(uint8_t x,uint8_t y,uint8_t *chr,uint8_t size1,uint8_t mode);
void Inf_OLED1315_ShowNum(uint8_t x,uint8_t y,uint32_t num,uint8_t len,uint8_t size1,uint8_t mode);
void Inf_OLED1315_Init(void);


#endif


