#include "Dri_I2C.h"

/**
 * 初始化
*/
void Dri_I2C_Init(void)
{
    /* 1.时钟控制使能IIC 和 GPIOB */
    RCC->APB1ENR |= RCC_APB1ENR_I2C2EN;
    RCC->APB2ENR |= RCC_APB2ENR_IOPBEN;

    /* 2.配置PB10、PB11为复用开漏 */
        // PB10 和 PB11 引脚为 I2C2
    GPIOB->CRH |= ( GPIO_CRH_MODE10 | GPIO_CRH_CNF10 | GPIO_CRH_MODE11 | GPIO_CRH_CNF11 );

    /* 3.配置IIC， */
    // 3.0 配置SMBUS模式，0为IIC模式；1为SMBUS模式。
    I2C2->CR1 &= ~I2C_CR1_SMBUS;

    // 3.1 配置IIC时钟频率 FREQ
        // 这里就不先将0-5位清零了，直接 或 操作了。
    I2C2->CR2 |= 0x24;

    // 3.2 配置模式（标准模式/快速模式）
    I2C2->CCR &= ~I2C_CCR_FS;

    // 3.3 配置通讯速率（标准模式100KHz,快速模式400KHz）, 
            // 计算为： CCR = Thigh / T  
            //  --->  100KHz/s 即 10us/1Hz， Thigh+Tlow=10us，Thigh=5us (即5us产生一个高电平)
            //  --->  T为周期（产生一个方波需要的时间） = 1s/f     f为频率,   即 1s/36MHz，即 1us/36Hz， 即 1/36us产生一个方波
            //  --->  CRR = 5 / (1/36) = 180
    I2C2->CCR |= 180;

    // 3.4 允许的SCL最大上升时间  TRISE = (最大上升沿时间/时钟周期)+1   （100KHz要求最大上升沿时间不超过1us）
    I2C2->TRISE |= 37;

    // 3.5 IIC使能PE
    I2C2->CR1 |= I2C_CR1_PE;

}

/**
 * 产生开始条件
 * 1成功；0失败
*/
uint8_t Dri_I2C_Start(void)
{
    /* 1.起始条件产生，无所谓主从，1就行 */
    I2C2->CR1 |= I2C_CR1_START;

    /* 2.用SR1的SB位判断主模式起始条件，加个检测超时的变量判断*/
    uint16_t timeout = 0xffff;
    while(!(I2C2->SR1&I2C_SR1_SB) && timeout) {
        timeout--;
    }

    /* 3.返回开始条件是否返回成功 */
    return timeout ? 1: 0;
}

/**
 * 产生终止条件
*/
void Dri_I2C_Stop(void)
{
    /* 产生终止条件 */
        // 终止产生后，自动变为从模式
    I2C2->CR1 |= I2C_CR1_STOP;
}

/**
 * 产生应答
*/
void Dri_I2C_ACK(void)
{
    /* 产生应答信号 */
    I2C2->CR1 |= I2C_CR1_ACK;
}

/**
 * 产生无应答
*/
void Dri_I2C_NACK(void)
{
    /* 产生无应答信号 */
    I2C2->CR1 &= ~I2C_CR1_ACK;
}

/**
 * 发送地址
 * 发送成功返回1；失败返回0
*/
uint8_t Dri_I2C_SendAddr(uint8_t addr) {

    // TODO 等待这个会影响小车的数据读取，，根本不要等这个位
    // uint16_t timeout = 0xffff;
    // while (((I2C2->SR1 & I2C_SR1_TXE) == 0) && timeout)
    // {
    //     timeout--;
    // }

    /* 把数据写入到数据寄存器中 */
    I2C2->DR = addr;

    /* 等待地址发送完成。*/
    uint16_t timeout = 0xffff;
    while(!(I2C2->SR1 & I2C_SR1_ADDR) /* 地址还没有发送结束 */ && timeout) {
        timeout--;
    }
    // 走出循环后，如果timeout为0，则是超时了；如果timeout非0，则是发送结束了。
    if(timeout) {
        // 通过  读取SR1寄存器 再 读取SR2寄存器 对 ADDR寄存器进行清零
        // I2C2->SR1;
        I2C2->SR2;
        return 1;
    }
    return 0;
}

/**
 * 发送一个字节
 * 1成功；0失败
*/
uint8_t Dri_I2C_Transmit(uint8_t byte)
{
    /* 1.等待数据寄存器空 */
    uint16_t timeout = 0xffff;
    while(!(I2C2->SR1&I2C_SR1_TXE) && timeout) {
        timeout--;
    }
    if(!timeout) return 0;

    /* 2.往数据寄存器中写数据 */
    I2C2->DR = byte;

    /* 3. 通过SR1_BTF位判断数据是否发送结束，并返回 */
    timeout = 0xffff;
    while(!(I2C2->SR1&I2C_SR1_BTF) && timeout) {
        timeout--;
    }
    return timeout ? 1:0;
}

/**
 * 读取一个字节
*/
uint8_t Dri_I2C_ReadByte(void)
{
    /* 1.通过SR1_RxNE位判断数据寄存器是否非空 */
    uint16_t timeout = 0xffff;
    while(!(I2C2->SR1&I2C_SR1_RXNE) && timeout) {
        timeout--;
    }

    /* 2.根据判断结果返回数据寄存器中的值 */
    return timeout ? I2C2->DR : 0;
}


