#include "Inf_TB6612.h"
#include "stdlib.h"
#include "stdio.h"


/**
 * @brief ��ʼ��
 * 
 */
void Inf_TB6612_Init() {

    // 1.��ʼ����ʱ��4
    Dri_TIM4_Init();

    // 2.��ʼ�� AINx��BINx���ţ�ͨ�����������mode=11��cnf=00��
    RCC->APB2ENR |= RCC_APB2ENR_IOPBEN;
    GPIOB->CRH |= (GPIO_CRH_MODE12 | GPIO_CRH_MODE13 | GPIO_CRH_MODE14 | GPIO_CRH_MODE15);
    GPIOB->CRH &= ~(GPIO_CRH_CNF12 | GPIO_CRH_CNF13 | GPIO_CRH_CNF14 | GPIO_CRH_CNF15);

}


/* ������������ */
void Inf_TB6612_MotorA(uint8_t state) {
    switch (state) {
        case GO:
            // printf("A-GO\r\n");
            Ain1(0);
            Ain2(1);
            break;
        case BACK:
            // printf("A-BACK\r\n");
            Ain1(1);
            Ain2(0);
            break;
        case BRAKE:
            // printf("A-BRAKE\r\n");
            Ain1(1);
            Ain2(1);
            break;
        default:
            break;
    }
}

/* �����ҵ������ */
void Inf_TB6612_MotorB(uint8_t state) {
    switch (state) {
        case GO:
            // printf("B-GO\r\n");
            Bin1(0);
            Bin2(1);
            break;
        case BACK:
            // printf("B-BACK\r\n");
            Bin1(1);
            Bin2(0);
            break;
        case BRAKE:
            // printf("B-BRAKE\r\n");
            Bin1(1);
            Bin2(1);
            break;
        default:
            break;
    }
}

/**
 * @brief ����pwma�ķ��źʹ�С�����Ƶ���ķ����ת��
 * 
 * @param pwma ��������PWMAֵ
 * @param pwmb ��������PWMBֵ
 */
void Inf_TB6612_SetPWM(int pwma, int pwmb) {

    // ���ݷ����ж�Ҫת���ķ���
    if(pwma < 0) {
        Inf_TB6612_MotorA(BACK);
    } else {
        Inf_TB6612_MotorA(GO);
    }

    // ����pwmAֵ��TIM4_CH4
    TIM4->CCR4 = abs(pwma);

    // B
    if(pwmb < 0) {
        Inf_TB6612_MotorB(BACK);
    } else {
        Inf_TB6612_MotorB(GO);
    }

    // ����pwmAֵ��TIM4_CH4
    TIM4->CCR3 = abs(pwmb);
}


