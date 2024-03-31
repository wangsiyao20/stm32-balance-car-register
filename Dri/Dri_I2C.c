#include "Dri_I2C.h"

/**
 * ��ʼ��
*/
void Dri_I2C_Init(void)
{
    /* 1.ʱ�ӿ���ʹ��IIC �� GPIOB */
    RCC->APB1ENR |= RCC_APB1ENR_I2C2EN;
    RCC->APB2ENR |= RCC_APB2ENR_IOPBEN;

    /* 2.����PB10��PB11Ϊ���ÿ�© */
        // PB10 �� PB11 ����Ϊ I2C2
    GPIOB->CRH |= ( GPIO_CRH_MODE10 | GPIO_CRH_CNF10 | GPIO_CRH_MODE11 | GPIO_CRH_CNF11 );

    /* 3.����IIC�� */
    // 3.0 ����SMBUSģʽ��0ΪIICģʽ��1ΪSMBUSģʽ��
    I2C2->CR1 &= ~I2C_CR1_SMBUS;

    // 3.1 ����IICʱ��Ƶ�� FREQ
        // ����Ͳ��Ƚ�0-5λ�����ˣ�ֱ�� �� �����ˡ�
    I2C2->CR2 |= 0x24;

    // 3.2 ����ģʽ����׼ģʽ/����ģʽ��
    I2C2->CCR &= ~I2C_CCR_FS;

    // 3.3 ����ͨѶ���ʣ���׼ģʽ100KHz,����ģʽ400KHz��, 
            // ����Ϊ�� CCR = Thigh / T  
            //  --->  100KHz/s �� 10us/1Hz�� Thigh+Tlow=10us��Thigh=5us (��5us����һ���ߵ�ƽ)
            //  --->  TΪ���ڣ�����һ��������Ҫ��ʱ�䣩 = 1s/f     fΪƵ��,   �� 1s/36MHz���� 1us/36Hz�� �� 1/36us����һ������
            //  --->  CRR = 5 / (1/36) = 180
    I2C2->CCR |= 180;

    // 3.4 �����SCL�������ʱ��  TRISE = (���������ʱ��/ʱ������)+1   ��100KHzҪ�����������ʱ�䲻����1us��
    I2C2->TRISE |= 37;

    // 3.5 IICʹ��PE
    I2C2->CR1 |= I2C_CR1_PE;

}

/**
 * ������ʼ����
 * 1�ɹ���0ʧ��
*/
uint8_t Dri_I2C_Start(void)
{
    /* 1.��ʼ��������������ν���ӣ�1���� */
    I2C2->CR1 |= I2C_CR1_START;

    /* 2.��SR1��SBλ�ж���ģʽ��ʼ�������Ӹ���ⳬʱ�ı����ж�*/
    uint16_t timeout = 0xffff;
    while(!(I2C2->SR1&I2C_SR1_SB) && timeout) {
        timeout--;
    }

    /* 3.���ؿ�ʼ�����Ƿ񷵻سɹ� */
    return timeout ? 1: 0;
}

/**
 * ������ֹ����
*/
void Dri_I2C_Stop(void)
{
    /* ������ֹ���� */
        // ��ֹ�������Զ���Ϊ��ģʽ
    I2C2->CR1 |= I2C_CR1_STOP;
}

/**
 * ����Ӧ��
*/
void Dri_I2C_ACK(void)
{
    /* ����Ӧ���ź� */
    I2C2->CR1 |= I2C_CR1_ACK;
}

/**
 * ������Ӧ��
*/
void Dri_I2C_NACK(void)
{
    /* ������Ӧ���ź� */
    I2C2->CR1 &= ~I2C_CR1_ACK;
}

/**
 * ���͵�ַ
 * ���ͳɹ�����1��ʧ�ܷ���0
*/
uint8_t Dri_I2C_SendAddr(uint8_t addr) {

    // TODO �ȴ������Ӱ��С�������ݶ�ȡ����������Ҫ�����λ
    // uint16_t timeout = 0xffff;
    // while (((I2C2->SR1 & I2C_SR1_TXE) == 0) && timeout)
    // {
    //     timeout--;
    // }

    /* ������д�뵽���ݼĴ����� */
    I2C2->DR = addr;

    /* �ȴ���ַ������ɡ�*/
    uint16_t timeout = 0xffff;
    while(!(I2C2->SR1 & I2C_SR1_ADDR) /* ��ַ��û�з��ͽ��� */ && timeout) {
        timeout--;
    }
    // �߳�ѭ�������timeoutΪ0�����ǳ�ʱ�ˣ����timeout��0�����Ƿ��ͽ����ˡ�
    if(timeout) {
        // ͨ��  ��ȡSR1�Ĵ��� �� ��ȡSR2�Ĵ��� �� ADDR�Ĵ�����������
        // I2C2->SR1;
        I2C2->SR2;
        return 1;
    }
    return 0;
}

/**
 * ����һ���ֽ�
 * 1�ɹ���0ʧ��
*/
uint8_t Dri_I2C_Transmit(uint8_t byte)
{
    /* 1.�ȴ����ݼĴ����� */
    uint16_t timeout = 0xffff;
    while(!(I2C2->SR1&I2C_SR1_TXE) && timeout) {
        timeout--;
    }
    if(!timeout) return 0;

    /* 2.�����ݼĴ�����д���� */
    I2C2->DR = byte;

    /* 3. ͨ��SR1_BTFλ�ж������Ƿ��ͽ����������� */
    timeout = 0xffff;
    while(!(I2C2->SR1&I2C_SR1_BTF) && timeout) {
        timeout--;
    }
    return timeout ? 1:0;
}

/**
 * ��ȡһ���ֽ�
*/
uint8_t Dri_I2C_ReadByte(void)
{
    /* 1.ͨ��SR1_RxNEλ�ж����ݼĴ����Ƿ�ǿ� */
    uint16_t timeout = 0xffff;
    while(!(I2C2->SR1&I2C_SR1_RXNE) && timeout) {
        timeout--;
    }

    /* 2.�����жϽ���������ݼĴ����е�ֵ */
    return timeout ? I2C2->DR : 0;
}


