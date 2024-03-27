#include "Dri_TIM.h"

/**
 * @brief TIM4��5 ��ʼ����pwmģʽ
 * 
 */
void Dri_TIM4_Init() {

    // pb8--tim4_ch3�� pb9--tim4_ch4   ��ʼ�����ţ� �������죬mode=11, cnf=10

    /* 1.  ����ʱ��*/
    /* 1.1 ��ʱ��4��ʱ�� */
    RCC->APB1ENR |= RCC_APB1ENR_TIM4EN;
    /* 1.2 GPIO��ʱ�� PB */
    RCC->APB2ENR |= RCC_APB2ENR_IOPBEN;

    // =========================================================

    // ͨ��3������
    /* 2. ����GPIO�ĸ���������� CNF=10 MODE=11*/
    GPIOB->CRH |= (GPIO_CRH_CNF8_1 | GPIO_CRH_MODE8);
    GPIOB->CRH &= ~GPIO_CRH_CNF8_0;

    /* 3. ��ʱ������ */
    /* 3.1 Ԥ��Ƶ�������� */            // Ƶ��10k
    TIM4->PSC = 720 - 1;
    /* 3.2 �Զ���װ�ؼĴ��������� */
    TIM4->ARR = 10 - 1;
    /* 3.3 �������ļ������� 0=���� 1=����*/
    TIM4->CR1 &= ~TIM_CR1_DIR;
    // /* 3.4 ����ͨ��3�Ĳ���ȽϼĴ��� */
    TIM4->CCR3 = 0;
    /* 3.5 ��ͨ��3����Ϊ���  CCMR2_CC3S=00 */
    TIM4->CCMR2 &= ~TIM_CCMR2_CC3S;
    /* 3.6 ����ͨ��������Ƚ�ģʽ */
    TIM4->CCMR2 |= TIM_CCMR2_OC3M_2;
    TIM4->CCMR2 |= TIM_CCMR2_OC3M_1;
    TIM4->CCMR2 &= ~TIM_CCMR2_OC3M_0;
    /* 3.7 ʹ��ͨ��3  CCER_CC3E=1 */
    TIM4->CCER |= TIM_CCER_CC3E;

    /* 3.8 ����ͨ���ļ��� 0=�ߵ�ƽ��Ч  1=�͵�ƽ��Ч */
    TIM4->CCER &= ~TIM_CCER_CC3P;

    // ==========================================================

    // ͨ��4������
    /* 2. ����GPIO�ĸ���������� PA1 CNF=10 MODE=11*/
    GPIOB->CRH |= (GPIO_CRH_CNF9_1 | GPIO_CRH_MODE9);
    GPIOB->CRH &= ~GPIO_CRH_CNF9_0;

    /* 3. ��ʱ������ */
    /* 3.1 Ԥ��Ƶ�������� */            // Ƶ��10k
    TIM4->PSC = 720 - 1;
    /* 3.2 �Զ���װ�ؼĴ��������� */
    TIM4->ARR = 10 - 1;
    /* 3.3 �������ļ������� 0=���� 1=����*/
    TIM4->CR1 &= ~TIM_CR1_DIR;
    /* 3.4 ����ͨ��4�Ĳ���ȽϼĴ��� */
    TIM4->CCR4 = 0;
    /* 3.5 ��ͨ��3����Ϊ���  CCMR2_CC3S=00 */
    TIM4->CCMR2 &= ~TIM_CCMR2_CC4S;
    /* 3.6 ����ͨ��������Ƚ�ģʽ */
    TIM4->CCMR2 |= TIM_CCMR2_OC4M_2;
    TIM4->CCMR2 |= TIM_CCMR2_OC4M_1;
    TIM4->CCMR2 &= ~TIM_CCMR2_OC4M_0;
    /* 3.7 ʹ��ͨ��4  CCER_CC4E=1 */
    TIM4->CCER |= TIM_CCER_CC4E;

    /* 3.8 ����ͨ���ļ��� 0=�ߵ�ƽ��Ч  1=�͵�ƽ��Ч */
    TIM4->CCER &= ~TIM_CCER_CC4P;

}

/**
 * @brief TIM2��ʼ��
 * 
 */
void Dri_TIM2_Init() {

    // ʱ��
    RCC->APB1ENR |= RCC_APB1ENR_TIM2EN;
    RCC->APB2ENR |= RCC_APB2ENR_AFIOEN;
    RCC->APB2ENR |= RCC_APB2ENR_IOPAEN | RCC_APB2ENR_IOPBEN;

    // �ر�JTAG-DP������SW-DP
    AFIO->MAPR &= ~AFIO_MAPR_SWJ_CFG;
    AFIO->MAPR |= AFIO_MAPR_SWJ_CFG_1;

    // ��ӳ��
    AFIO->MAPR |= AFIO_MAPR_TIM2_REMAP;

    // PA15��PB3        ��������  mode=00,,cnf=01
    GPIOB->CRL &= ~GPIO_CRL_MODE3;
    GPIOB->CRL &= ~GPIO_CRL_CNF3_1;
    GPIOB->CRL |= GPIO_CRL_CNF3_0;

    GPIOA->CRH &= ~GPIO_CRH_MODE15;
    GPIOA->CRH &= ~GPIO_CRH_CNF15_1;
    GPIOA->CRH |= GPIO_CRH_CNF15_0;

    /* ��ʱ�����û������� */
    /* 3.1 Ԥ��Ƶ�������� */
    TIM2->PSC = 1 - 1;
    /* 3.2 �Զ���װ�ؼĴ��������ã���Ϊ���ֵ */
    TIM2->ARR = 0xFFFF;

    // ͨ��ӳ��
    TIM2->CCMR1 = TIM2->CCMR1 | TIM_CCMR1_CC1S_0 & (~TIM_CCMR1_CC1S_1);
    TIM2->CCMR1 = TIM2->CCMR1 | TIM_CCMR1_CC2S_0 & (~TIM_CCMR1_CC2S_1);

    // ���ò�����
    TIM2->CCER &= ~TIM_CCER_CC1P;
    TIM2->CCER &= ~TIM_CCER_CC2P;

    // ͬʱ��ͨ��1��ͨ��2���ؼ���������SMS=011
    TIM2->SMCR |= TIM_SMCR_SMS;
    TIM2->SMCR &= ~TIM_SMCR_SMS_2;

    // ������ʹ��
    TIM2->CR1 |= TIM_CR1_CEN;
        

}

/**
 * @brief TIM3��ʼ��
 * 
 */
void Dri_TIM3_Init() {

    // ʱ��
    RCC->APB1ENR |= RCC_APB1ENR_TIM3EN;
    RCC->APB2ENR |= RCC_APB2ENR_AFIOEN;
    RCC->APB2ENR |= RCC_APB2ENR_IOPBEN;

    // �ر�JTAG-DP������SW-DP
    AFIO->MAPR &= ~AFIO_MAPR_SWJ_CFG;
    AFIO->MAPR |= AFIO_MAPR_SWJ_CFG_1;

    // ��ӳ��
    AFIO->MAPR = AFIO->MAPR | AFIO_MAPR_TIM3_REMAP_1 & (~AFIO_MAPR_TIM3_REMAP_0);

    // Pb4��Pb5        ��������  mode=00,,cnf=01
    GPIOB->CRL &= ~GPIO_CRL_MODE4;
    GPIOB->CRL &= ~GPIO_CRL_CNF4_1;
    GPIOB->CRL |= GPIO_CRL_CNF4_0;

    GPIOB->CRL &= ~GPIO_CRL_MODE5;
    GPIOB->CRL &= ~GPIO_CRL_CNF5_1;
    GPIOB->CRL |= GPIO_CRL_CNF5_0;

    /* ��ʱ�����û������� */
    /* 3.1 Ԥ��Ƶ�������� */
    TIM3->PSC = 1 - 1;
    /* 3.2 �Զ���װ�ؼĴ��������ã���Ϊ���ֵ */
    TIM3->ARR = 0xFFFF;

    // ͨ��ӳ��
    TIM3->CCMR1 = TIM3->CCMR1 | TIM_CCMR1_CC1S_1 & (~TIM_CCMR1_CC1S_0);
    TIM3->CCMR1 = TIM3->CCMR1 | TIM_CCMR1_CC2S_1 & (~TIM_CCMR1_CC2S_0);

    // һ�����࣬һ��������
    TIM3->CCER &= ~TIM_CCER_CC1P;
    TIM3->CCER |= TIM_CCER_CC2P;


    // ͬʱ��ͨ��1��ͨ��2���ؼ���������SMS=011
    TIM3->SMCR |= TIM_SMCR_SMS;
    TIM3->SMCR &= ~TIM_SMCR_SMS_2;

    // ������ʹ��
    TIM3->CR1 |= TIM_CR1_CEN;
        

}



