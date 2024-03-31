#include "Inf_TB6612.h"
#include "stdlib.h"
#include "stdio.h"


/**
 * @brief 初始化
 * 
 */
void Inf_TB6612_Init() {

    // 1.初始化定时器4
    Dri_TIM4_Init();

    // 2.初始化 AINx、BINx引脚，通用推挽输出（mode=11，cnf=00）
    RCC->APB2ENR |= RCC_APB2ENR_IOPBEN;
    GPIOB->CRH |= (GPIO_CRH_MODE12 | GPIO_CRH_MODE13 | GPIO_CRH_MODE14 | GPIO_CRH_MODE15);
    GPIOB->CRH &= ~(GPIO_CRH_CNF12 | GPIO_CRH_CNF13 | GPIO_CRH_CNF14 | GPIO_CRH_CNF15);

}


/* 控制左电机方向 */
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

/* 控制右电机方向 */
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
 * @brief 根据pwma的符号和大小，控制电机的方向和转速
 * 
 * @param pwma 带正负的PWMA值
 * @param pwmb 带正负的PWMB值
 */
void Inf_TB6612_SetPWM(int pwma, int pwmb) {

    // 根据符号判断要转动的方向
    if(pwma < 0) {
        Inf_TB6612_MotorA(BACK);
    } else {
        Inf_TB6612_MotorA(GO);
    }

    // 设置pwmA值，TIM4_CH4
    TIM4->CCR4 = abs(pwma);

    // B
    if(pwmb < 0) {
        Inf_TB6612_MotorB(BACK);
    } else {
        Inf_TB6612_MotorB(GO);
    }

    // 设置pwmA值，TIM4_CH4
    TIM4->CCR3 = abs(pwmb);
}


