#include "Dri_ADC.h"

void Driver_ADC1_Init(void)
{
    // TODO
    // /* 1. ʱ������ */
    // /* 1.1 adcʱ�� */
    // RCC->APB2ENR |= RCC_APB2ENR_ADC1EN;
    // RCC->CFGR |= RCC_CFGR_ADCPRE_1;
    // RCC->CFGR &= ~RCC_CFGR_ADCPRE_0;

    // /* 1.2 gpio��ʱ�� */
    // RCC->APB2ENR |= RCC_APB2ENR_IOPCEN;

    // /* 2. gpio����ģʽ: PC0 ģ������  CNF=00 MODE=00 */
    // GPIOC->CRL &= ~(GPIO_CRL_CNF0 | GPIO_CRL_MODE0);

    // /* 2. ADC������� */
    // /* 2.1 ����ɨ��ģʽ. ֻ��һ��ͨ������ɨ�� */
    // ADC1->CR1 &= ~ADC_CR1_SCAN;
    // /* 2.2 ��������ת��ģʽ CR2=CONT 1*/
    // ADC1->CR2 |= ADC_CR2_CONT;
    // /* 2.3 ���ݶ��뷽ʽ: �Ҷ��� ����� */
    // ADC1->CR2 &= ~ADC_CR2_ALIGN;
    // /* 2.4 ���ò���ʱ�� ADC_SMPR1  010=13.5����*/
    // ADC1->SMPR1 &= ~(ADC_SMPR1_SMP10_2 | ADC_SMPR1_SMP10_0);
    // ADC1->SMPR1 |= ADC_SMPR1_SMP10_1;
    // /* 2.6 ͨ��������� */
    // /* 2.6.1 ���ü���ͨ����Ҫת�� */
    // ADC1->SQR1 &= ~ADC_SQR1_L;
    // /* 2.6.1 ��ͨ�������õ�������.  */
    // ADC1->SQR3 &= ~ADC_SQR3_SQ1; /* �Ȱ�5λ���� */
    // ADC1->SQR3 |= 10 << 0;       /* �������5λ */
    // /* 2.7 ѡ��������� */
    // ADC1->CR2 |= ADC_CR2_EXTTRIG; /* ������������ⲿת�� */
    // ADC1->CR2 |= ADC_CR2_EXTSEL;  /* ѡ��ʹ���������ADC */
}


