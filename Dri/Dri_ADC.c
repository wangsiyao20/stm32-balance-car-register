#include "Dri_ADC.h"

void Dri_ADC1_Init(void)
{
    /* 1. ʱ������ */
    /* 1.1 adcʱ�� */
    RCC->APB2ENR |= RCC_APB2ENR_ADC1EN;
    RCC->CFGR |= RCC_CFGR_ADCPRE_1;
    RCC->CFGR &= ~RCC_CFGR_ADCPRE_0;

    /* 1.2 gpio��ʱ�� */
    RCC->APB2ENR |= RCC_APB2ENR_IOPAEN;

    /* 2. gpio����ģʽ: PC0 ģ������  CNF=00 MODE=00 */
    GPIOA->CRL &= ~(GPIO_CRL_CNF1 | GPIO_CRL_MODE1);

    /* 2. ADC������� */
    /* 2.1 ����ɨ��ģʽ. ֻ��һ��ͨ������ɨ�� */
    ADC1->CR1 &= ~ADC_CR1_SCAN;
    /* 2.2 ��������ת��ģʽ */
    ADC1->CR2 &= ~ADC_CR2_CONT;
    /* 2.3 ���ݶ��뷽ʽ: �Ҷ��� ����� */
    ADC1->CR2 &= ~ADC_CR2_ALIGN;
    /* 2.4 ���ò���ʱ�� ADC_SMPR1  �������*/
    ADC1->SMPR1 |= ADC_SMPR1_SMP10;
    /* 2.6 ͨ��������� */
    /* 2.6.1 ���ü���ͨ����Ҫת�� */
    ADC1->SQR1 &= ~ADC_SQR1_L;
    /* 2.6.1 ��ͨ�������õ�������.  */
    ADC1->SQR3 &= ~ADC_SQR3_SQ1; /* �Ȱ�5λ���� */
    ADC1->SQR3 |= 1 << 0;       /* �������5λ */
    /* 2.7 ѡ��������� */
    ADC1->CR2 |= ADC_CR2_EXTTRIG; /* ������������ⲿת�� */
    ADC1->CR2 |= ADC_CR2_EXTSEL;  /* ѡ��ʹ���������ADC */

    /* 1. �ϵ�: ��ADC������ģʽ���� */
    ADC1->CR2 |= ADC_CR2_ADON;

    /* 2. ִ��У׼ */
    ADC1->CR2 |= ADC_CR2_CAL;
    while (ADC1->CR2 & ADC_CR2_CAL)
        ;

    /* 3. ADON = 1, ��ʼת�� 0>1 ������ģʽ���� 1->1 ��ʼ */
    ADC1->CR2 |= ADC_CR2_ADON;

    /* 4. ʹ�������ʼת������ͨ�� */;
    ADC1->CR2 |= ADC_CR2_SWSTART;

    /* 5. �ȴ��״�ת����� */
    while((ADC1->SR & ADC_SR_EOC) == 0);
}


double Dri_ADC1_ReadV(void)
{
    // ��Ϊ�ǲ�����ת��������ÿ�ζ�Ҫ�ؿ�ת��
    /* 3. ADON = 1, ��ʼת�� 0>1 ������ģʽ���� 1->1 ��ʼ */
    ADC1->CR2 |= ADC_CR2_ADON;

    /* 4. ʹ�������ʼת������ͨ�� */;
    ADC1->CR2 |= ADC_CR2_SWSTART;

    /* 5. �ȴ��״�ת����� */
    while((ADC1->SR & ADC_SR_EOC) == 0);

    // 12λ��ADC ��Χ [0, 4095]
    return ADC1->DR * 3.3 / 4095 * 4;       // *4����ΪӲ����·��ѹ�����
}


