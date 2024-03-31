#include "stdint.h"
#include "stm32f10x.h"                  // Device header

#include "App_Car.h"
#include "App_Task.h"


extern float angle; // 卡尔曼滤波后的倾斜角度

extern short ax, ay, az;  // xyz轴上的加速度
extern short gx, gy, gz;  // xyz轴上的角速度

int main() 
{

	App_Car_Init();
	
	printf("balance car project begin...\r\n");

	Inf_OLED1315_Clear();
    /* 
        字体=> 8宽*16高
        BAT:     V  
        EA:
        EB:
        Angle:
     */
    Inf_OLED1315_ShowString(0, 0, "BAT:    V", 16, 1); 
    Inf_OLED1315_ShowString(0, 16, "EA:", 16, 1); 
    Inf_OLED1315_ShowString(0, 32, "EB:", 16, 1); 
    Inf_OLED1315_ShowString(0, 48, "Angle:", 16, 1); 
    Inf_OLED1315_Refresh();
	
	FreeRTOS_Start();

	while(1) {
		// App_Car_GetAngle_Encoder();
        // // printf("angle = %f\r\n", Acc_Angle);

        // printf("ax=%d\tay=%d\taz=%d\r\n", ax, ay, az);
        // printf("gx=%d\tgy=%d\tgz=%d\r\n", gx, gy, gz);


		// 延时100ms
		// for(uint32_t i=100*72000/9; i>1; i--) __NOP();
	}
}

