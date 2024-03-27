#include "Driver_Usart.h"

void Driver_USART1_Init(void)
{

    // 1.����ʱ��
    // 1.1 ��������1��ʱ��
    RCC->APB2ENR |= RCC_APB2ENR_USART1EN;
    // 1.2 ����gpioʱ��(�������ռ��A9 A10)
    RCC->APB2ENR |= RCC_APB2ENR_IOPAEN;

    // 2.�������� PA9(Tx) �� PA10(Rx) 
    // 2.1 PA9����Ϊ����������� mode9--11    cnf9--10
    GPIOA->CRH |= (GPIO_CRH_MODE9 | GPIO_CRH_CNF9_1);
    // 2.2 PA10����Ϊ��������   MODE9--00   CNF9--01
    GPIOA->CRH |= GPIO_CRH_CNF9_0;

    // 3.���ò�����Ϊ 115200   ���㷽ʽ��
    USART1->BRR = 0x271;
	// 	// ������57600
    // USART1->BRR = 0x4E2;

    // 4.usart1�ķ��ͺͽ���ʹ��
    USART1->CR1 |= (USART_CR1_TE | USART_CR1_RE);

    // 5.usart1����ʹ��
    USART1->CR1 |= USART_CR1_UE;

}

/**
 * ����һ���ֽ�
*/
void Driver_USART1_SendChar(uint8_t ch) {
    // �ȴ��������ݼĴ���Ϊ���ٷ������ݣ�USART1->SR��txeλΪ1��ʾ�����Ѿ�ȫ���Ƶ���λ�Ĵ����ˡ�
    while((USART1->SR & USART_SR_TXE)==0);
    // �����ݷŵ����ݼĴ���
    USART1->DR = ch;
}

/**
 * ����һ���ַ���
*/
void Driver_USART1_SendStr(uint8_t* str, uint8_t len) {
    for(uint8_t i=0; i<len; i++) {
        Driver_USART1_SendChar(*str++);
    }
}

/**
 * ����һ���ַ�
*/
uint8_t Driver_USART1_ReceiveChar(void)
{
    // ���յ����ݼĴ���û����һֱѭ��
    while((USART1->SR & USART_SR_RXNE) == 0);
    return USART1->DR;
}

/**
 * ����һ���ַ���
*/
void Driver_USART1_ReceiveString(uint8_t *buff, uint8_t *len)
{
    uint8_t i = 0;
    while(1) {
        // ���յ����ݼĴ���û����һֱѭ��
        while((USART1->SR & USART_SR_RXNE) == 0) {
            // �ڵȴ��ڼ䣬�ж��Ƿ����֡������ǿ���֡
            if(USART1->SR & USART_SR_IDLE) {
                *len = i;
                return;
            }
        }
        buff[i] = USART1->DR;
        i++;
    }
}

// ʹ��printf���Զ������������
int fputc(int c, FILE *file) {
    Driver_USART1_SendChar(c);
    return c;
}

