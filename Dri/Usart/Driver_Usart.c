#include "Driver_Usart.h"

void Driver_USART1_Init(void)
{

    // 1.开启时钟
    // 1.1 开启串口1的时钟
    RCC->APB2ENR |= RCC_APB2ENR_USART1EN;
    // 1.2 开启gpio时钟(输出输入占了A9 A10)
    RCC->APB2ENR |= RCC_APB2ENR_IOPAEN;

    // 2.配置引脚 PA9(Tx) 和 PA10(Rx) 
    // 2.1 PA9设置为复用推挽输出 mode9--11    cnf9--10
    GPIOA->CRH |= (GPIO_CRH_MODE9 | GPIO_CRH_CNF9_1);
    // 2.2 PA10设置为浮空输入   MODE9--00   CNF9--01
    GPIOA->CRH |= GPIO_CRH_CNF9_0;

    // 3.配置波特率为 115200   计算方式略
    USART1->BRR = 0x271;
	// 	// 波特率57600
    // USART1->BRR = 0x4E2;

    // 4.usart1的发送和接收使能
    USART1->CR1 |= (USART_CR1_TE | USART_CR1_RE);

    // 5.usart1的总使能
    USART1->CR1 |= USART_CR1_UE;

}

/**
 * 发送一个字节
*/
void Driver_USART1_SendChar(uint8_t ch) {
    // 等待发送数据寄存器为空再发送数据，USART1->SR的txe位为1表示数据已经全部移到移位寄存器了。
    while((USART1->SR & USART_SR_TXE)==0);
    // 将数据放到数据寄存器
    USART1->DR = ch;
}

/**
 * 发送一个字符串
*/
void Driver_USART1_SendStr(uint8_t* str, uint8_t len) {
    for(uint8_t i=0; i<len; i++) {
        Driver_USART1_SendChar(*str++);
    }
}

/**
 * 接收一个字符
*/
uint8_t Driver_USART1_ReceiveChar(void)
{
    // 接收的数据寄存器没满则一直循环
    while((USART1->SR & USART_SR_RXNE) == 0);
    return USART1->DR;
}

/**
 * 接收一个字符串
*/
void Driver_USART1_ReceiveString(uint8_t *buff, uint8_t *len)
{
    uint8_t i = 0;
    while(1) {
        // 接收的数据寄存器没满则一直循环
        while((USART1->SR & USART_SR_RXNE) == 0) {
            // 在等待期间，判断是否空闲帧，如果是空闲帧
            if(USART1->SR & USART_SR_IDLE) {
                *len = i;
                return;
            }
        }
        buff[i] = USART1->DR;
        i++;
    }
}

// 使用printf会自动调用这个函数
int fputc(int c, FILE *file) {
    Driver_USART1_SendChar(c);
    return c;
}

