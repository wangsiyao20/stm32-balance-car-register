#include "Dri_TIM.h"

/**
 * @brief TIM4、5 初始化，pwm模式
 * 
 */
void Dri_TIM4_Init() {

    // pb8--tim4_ch3、 pb9--tim4_ch4   初始化引脚， 复用推挽，mode=11, cnf=10

    /* 1.  开启时钟*/
    /* 1.1 定时器4的时钟 */
    RCC->APB1ENR |= RCC_APB1ENR_TIM4EN;
    /* 1.2 GPIO的时钟 PB */
    RCC->APB2ENR |= RCC_APB2ENR_IOPBEN;

    // =========================================================

    // 通道3的配置
    /* 2. 设置GPIO的复用推挽输出 CNF=10 MODE=11*/
    GPIOB->CRH |= (GPIO_CRH_CNF8_1 | GPIO_CRH_MODE8);
    GPIOB->CRH &= ~GPIO_CRH_CNF8_0;

    /* 3. 定时器配置 */
    /* 3.1 预分频器的配置 */            // 频率10k
    TIM4->PSC = 720 - 1;
    /* 3.2 自动重装载寄存器的配置 */
    TIM4->ARR = 10 - 1;
    /* 3.3 计数器的计数方向 0=向上 1=向下*/
    TIM4->CR1 &= ~TIM_CR1_DIR;
    // /* 3.4 配置通道3的捕获比较寄存器 */
    TIM4->CCR3 = 0;
    /* 3.5 把通道3配置为输出  CCMR2_CC3S=00 */
    TIM4->CCMR2 &= ~TIM_CCMR2_CC3S;
    /* 3.6 配置通道的输出比较模式 */
    TIM4->CCMR2 |= TIM_CCMR2_OC3M_2;
    TIM4->CCMR2 |= TIM_CCMR2_OC3M_1;
    TIM4->CCMR2 &= ~TIM_CCMR2_OC3M_0;
    /* 3.7 使能通道3  CCER_CC3E=1 */
    TIM4->CCER |= TIM_CCER_CC3E;

    /* 3.8 设置通道的极性 0=高电平有效  1=低电平有效 */
    TIM4->CCER &= ~TIM_CCER_CC3P;

    // ==========================================================

    // 通道4的配置
    /* 2. 设置GPIO的复用推挽输出 PA1 CNF=10 MODE=11*/
    GPIOB->CRH |= (GPIO_CRH_CNF9_1 | GPIO_CRH_MODE9);
    GPIOB->CRH &= ~GPIO_CRH_CNF9_0;

    /* 3. 定时器配置 */
    /* 3.1 预分频器的配置 */            // 频率10k
    TIM4->PSC = 720 - 1;
    /* 3.2 自动重装载寄存器的配置 */
    TIM4->ARR = 10 - 1;
    /* 3.3 计数器的计数方向 0=向上 1=向下*/
    TIM4->CR1 &= ~TIM_CR1_DIR;
    /* 3.4 配置通道4的捕获比较寄存器 */
    TIM4->CCR4 = 0;
    /* 3.5 把通道3配置为输出  CCMR2_CC3S=00 */
    TIM4->CCMR2 &= ~TIM_CCMR2_CC4S;
    /* 3.6 配置通道的输出比较模式 */
    TIM4->CCMR2 |= TIM_CCMR2_OC4M_2;
    TIM4->CCMR2 |= TIM_CCMR2_OC4M_1;
    TIM4->CCMR2 &= ~TIM_CCMR2_OC4M_0;
    /* 3.7 使能通道4  CCER_CC4E=1 */
    TIM4->CCER |= TIM_CCER_CC4E;

    /* 3.8 设置通道的极性 0=高电平有效  1=低电平有效 */
    TIM4->CCER &= ~TIM_CCER_CC4P;

}

/**
 * @brief TIM2初始化
 * 
 */
void Dri_TIM2_Init() {

    // 时钟
    RCC->APB1ENR |= RCC_APB1ENR_TIM2EN;
    RCC->APB2ENR |= RCC_APB2ENR_AFIOEN;
    RCC->APB2ENR |= RCC_APB2ENR_IOPAEN | RCC_APB2ENR_IOPBEN;

    // 关闭JTAG-DP，启用SW-DP
    AFIO->MAPR &= ~AFIO_MAPR_SWJ_CFG;
    AFIO->MAPR |= AFIO_MAPR_SWJ_CFG_1;

    // 重映射
    AFIO->MAPR |= AFIO_MAPR_TIM2_REMAP;

    // PA15、PB3        浮空输入  mode=00,,cnf=01
    GPIOB->CRL &= ~GPIO_CRL_MODE3;
    GPIOB->CRL &= ~GPIO_CRL_CNF3_1;
    GPIOB->CRL |= GPIO_CRL_CNF3_0;

    GPIOA->CRH &= ~GPIO_CRH_MODE15;
    GPIOA->CRH &= ~GPIO_CRH_CNF15_1;
    GPIOA->CRH |= GPIO_CRH_CNF15_0;

    /* 定时器配置基本配置 */
    /* 3.1 预分频器的配置 */
    TIM2->PSC = 1 - 1;
    /* 3.2 自动重装载寄存器的配置，设为最大值 */
    TIM2->ARR = 0xFFFF;

    // 通道映射
    TIM2->CCMR1 = TIM2->CCMR1 | TIM_CCMR1_CC1S_0 & (~TIM_CCMR1_CC1S_1);
    TIM2->CCMR1 = TIM2->CCMR1 | TIM_CCMR1_CC2S_0 & (~TIM_CCMR1_CC2S_1);

    // 设置不反相
    TIM2->CCER &= ~TIM_CCER_CC1P;
    TIM2->CCER &= ~TIM_CCER_CC2P;

    // 同时在通道1和通道2边沿计数，，，SMS=011
    TIM2->SMCR |= TIM_SMCR_SMS;
    TIM2->SMCR &= ~TIM_SMCR_SMS_2;

    // 计数器使能
    TIM2->CR1 |= TIM_CR1_CEN;
        

}

/**
 * @brief TIM3初始化
 * 
 */
void Dri_TIM3_Init() {

    // 时钟
    RCC->APB1ENR |= RCC_APB1ENR_TIM3EN;
    RCC->APB2ENR |= RCC_APB2ENR_AFIOEN;
    RCC->APB2ENR |= RCC_APB2ENR_IOPBEN;

    // 关闭JTAG-DP，启用SW-DP
    AFIO->MAPR &= ~AFIO_MAPR_SWJ_CFG;
    AFIO->MAPR |= AFIO_MAPR_SWJ_CFG_1;

    // 重映射
    AFIO->MAPR = AFIO->MAPR | AFIO_MAPR_TIM3_REMAP_1 & (~AFIO_MAPR_TIM3_REMAP_0);

    // Pb4、Pb5        浮空输入  mode=00,,cnf=01
    GPIOB->CRL &= ~GPIO_CRL_MODE4;
    GPIOB->CRL &= ~GPIO_CRL_CNF4_1;
    GPIOB->CRL |= GPIO_CRL_CNF4_0;

    GPIOB->CRL &= ~GPIO_CRL_MODE5;
    GPIOB->CRL &= ~GPIO_CRL_CNF5_1;
    GPIOB->CRL |= GPIO_CRL_CNF5_0;

    /* 定时器配置基本配置 */
    /* 3.1 预分频器的配置 */
    TIM3->PSC = 1 - 1;
    /* 3.2 自动重装载寄存器的配置，设为最大值 */
    TIM3->ARR = 0xFFFF;

    // 通道映射
    TIM3->CCMR1 = TIM3->CCMR1 | TIM_CCMR1_CC1S_1 & (~TIM_CCMR1_CC1S_0);
    TIM3->CCMR1 = TIM3->CCMR1 | TIM_CCMR1_CC2S_1 & (~TIM_CCMR1_CC2S_0);

    // 一个反相，一个不反相
    TIM3->CCER &= ~TIM_CCER_CC1P;
    TIM3->CCER |= TIM_CCER_CC2P;


    // 同时在通道1和通道2边沿计数，，，SMS=011
    TIM3->SMCR |= TIM_SMCR_SMS;
    TIM3->SMCR &= ~TIM_SMCR_SMS_2;

    // 计数器使能
    TIM3->CR1 |= TIM_CR1_CEN;
        

}



