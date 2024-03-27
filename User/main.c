#include "stdint.h"
#include "stm32f10x.h"                  // Device header
#include "Driver_Usart.h"
#include "stdio.h"
#include "App_Task.h"
#include "Inf_OLED1315.h"
#include "Inf_TB6612.h"
#include "Int_Encoder.h"

/**
 * @description: for循环实现延时函数
 * @param {uint32_t} ms
 * @return {*}
 */
void for_delay_ms(uint32_t ms)
{
    uint32_t Delay = ms * 72000 / 9; /* 72M时钟频率 */
    do
    {
        __NOP(); /* 空指令（NOP）来占用 CPU 时间 */
    } while (Delay--);
}

int main() 
{
	
	Driver_USART1_Init();
	printf("hello...\r\n");
	// FreeRTOS_Start();
	Inf_OLED1315_Init();
	Inf_OLED1315_Clear();
	Inf_OLED1315_DrawPoint(10, 10, 1);
	Inf_OLED1315_ShowChar(20, 20, 'a', 8, 1);
	Inf_OLED1315_Refresh();
	

	Inf_TB6612_Init();
	for_delay_ms(100);
	// Inf_TB6612_SetPWM(5, 5);
	
	Int_Encoder_Init();

	short count2 = 0, count3 = 0;

	while(1) {
		count2 = Int_Encoder_ReadCounter(2);
		count3 = Int_Encoder_ReadCounter(3);
		printf("读到的值为：%dxxxxxxx%d\r\n", count2, count3);
		for_delay_ms(1000);
	}
}

