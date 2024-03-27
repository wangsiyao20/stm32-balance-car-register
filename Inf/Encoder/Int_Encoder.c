#include "Int_Encoder.h"
#include "Dri_TIM.h"

/**
 * @brief 初始化
 * 
 */
void Int_Encoder_Init(void) {
    Dri_TIM2_Init();
    Dri_TIM3_Init();
}

/**
 * @brief 读取编码器的值
 * 
 * @param timx 
 * @return int 
 */
int Int_Encoder_ReadCounter(uint8_t timx) {
    short encoder = 0;
    switch (timx) {
        case 2:
            // CNT范围是0-65535, int可以接收到65535，我需要65535为-1，所以short可以
            encoder = (short)TIM2 -> CNT;   
            TIM2 -> CNT = 0;
            break;
        case 3:
            encoder = (short)TIM3 -> CNT;   
            TIM3 -> CNT = 0;
            break;
        default:
            break;
    }
    return encoder;
}



