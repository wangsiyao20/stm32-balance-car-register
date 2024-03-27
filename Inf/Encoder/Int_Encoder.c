#include "Int_Encoder.h"
#include "Dri_TIM.h"

/**
 * @brief ��ʼ��
 * 
 */
void Int_Encoder_Init(void) {
    Dri_TIM2_Init();
    Dri_TIM3_Init();
}

/**
 * @brief ��ȡ��������ֵ
 * 
 * @param timx 
 * @return int 
 */
int Int_Encoder_ReadCounter(uint8_t timx) {
    short encoder = 0;
    switch (timx) {
        case 2:
            // CNT��Χ��0-65535, int���Խ��յ�65535������Ҫ65535Ϊ-1������short����
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



