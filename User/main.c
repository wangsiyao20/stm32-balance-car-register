#include "stdint.h"
#include "stm32f10x.h"                  // Device header
#include "Driver_Usart.h"
#include "stdio.h"
#include "App_Task.h"
#include "Inf_OLED1315.h"
#include "Inf_TB6612.h"
#include "Int_Encoder.h"
#include "Inf_MPU6050.h"
#include "stdlib.h"
#include "math.h"
#include "Com_Filter.h"


#define PI 3.1415926

extern float angle;

/**
 * @description: forѭ��ʵ����ʱ����
 * @param {uint32_t} ms
 * @return {*}
 */
void for_delay_ms(uint32_t ms)
{
    uint32_t Delay = ms * 72000 / 9; /* 72Mʱ��Ƶ�� */
    do
    {
        __NOP(); /* ��ָ�NOP����ռ�� CPU ʱ�� */
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
	
	// Int_Encoder_Init();

	// short count2 = 0, count3 = 0;

	Inf_MPU6050_Init();
	// Dri_I2C_Init();

	// ��ȡ�Ĵ����ĵ�ַ
	uint8_t read_dat = 0;
	if(!Inf_MPU6050_ReadByte(MPU_DEVICE_ID_REG, &read_dat)) {
		printf("��ȡ��MPU6050��ID:%d\r\n", read_dat);
	}

	short ax=0, ay=0, az=0;
	short gx=0, gy=0, gz=0;
	float Acc_Angle=0; /* ͨ�����ٶȼ����������� */
	float GryoY=0;
	while(1) {
		/* ===============����MPU6050��ȡԭʼ����============================ */
		ax=0,ay=0,az=0;
		gx=0,gy=0,gz=0;
        Inf_MPU6050_GetAccl(&ax,&ay,&az);
		Inf_MPU6050_GetGyro(&gx,&gy,&gz);
		Acc_Angle = (float)(atan2(ax, az) * 180 /PI);
		
        printf("==============================\r\n");
        printf("ax=%d\r\n",ax);
        printf("ay=%d\r\n",ay);
        printf("az=%d\r\n",az);
        printf("gx=%d\r\n",gx);
        printf("gy=%d\r\n",gy);
        printf("gz=%d\r\n",gz);
		printf("ƫб�Ƕ�Ϊ��%f��\r\n", Acc_Angle);
		GryoY = -gy/16.4;
		Com_Filter_Kalman(Acc_Angle, GryoY);
		printf("�����������˲��ںϼ���ĽǶ�=%2.1f\r\n",angle);
		// count2 = Int_Encoder_ReadCounter(2);
		// count3 = Int_Encoder_ReadCounter(3);
		// printf("������ֵΪ��%dxxxxxxx%d\r\n", count2, count3);
		for_delay_ms(100);
	}
}

