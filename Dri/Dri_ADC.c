#include "Dri_ADC.h"

void Driver_ADC1_Init(void)
{
    // TODO
    // /* 1. 时钟配置 */
    // /* 1.1 adc时钟 */
    // RCC->APB2ENR |= RCC_APB2ENR_ADC1EN;
    // RCC->CFGR |= RCC_CFGR_ADCPRE_1;
    // RCC->CFGR &= ~RCC_CFGR_ADCPRE_0;

    // /* 1.2 gpio的时钟 */
    // RCC->APB2ENR |= RCC_APB2ENR_IOPCEN;

    // /* 2. gpio工作模式: PC0 模拟输入  CNF=00 MODE=00 */
    // GPIOC->CRL &= ~(GPIO_CRL_CNF0 | GPIO_CRL_MODE0);

    // /* 2. ADC相关配置 */
    // /* 2.1 禁用扫描模式. 只有一个通道不用扫描 */
    // ADC1->CR1 &= ~ADC_CR1_SCAN;
    // /* 2.2 启用连续转换模式 CR2=CONT 1*/
    // ADC1->CR2 |= ADC_CR2_CONT;
    // /* 2.3 数据对齐方式: 右对齐 左对齐 */
    // ADC1->CR2 &= ~ADC_CR2_ALIGN;
    // /* 2.4 设置采样时间 ADC_SMPR1  010=13.5周期*/
    // ADC1->SMPR1 &= ~(ADC_SMPR1_SMP10_2 | ADC_SMPR1_SMP10_0);
    // ADC1->SMPR1 |= ADC_SMPR1_SMP10_1;
    // /* 2.6 通道组的配置 */
    // /* 2.6.1 配置几个通道需要转换 */
    // ADC1->SQR1 &= ~ADC_SQR1_L;
    // /* 2.6.1 把通道号配置到组里面.  */
    // ADC1->SQR3 &= ~ADC_SQR3_SQ1; /* 先把5位清零 */
    // ADC1->SQR3 |= 10 << 0;       /* 设置最后5位 */
    // /* 2.7 选择软件触发 */
    // ADC1->CR2 |= ADC_CR2_EXTTRIG; /* 开启规则组的外部转换 */
    // ADC1->CR2 |= ADC_CR2_EXTSEL;  /* 选择使用软件触发ADC */
}


