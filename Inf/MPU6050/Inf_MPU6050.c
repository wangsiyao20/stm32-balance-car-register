#include "Inf_MPU6050.h"

/**
 * @brief ��ʼ��
 * 
 */
void Inf_MPU6050_Init(void) {
    // 1.��ʼ��IIC
    Dri_I2C_Init();
    // 2.
}

/**
 * @brief д�ֽ�
 * 
 * @param reg_addr �豸�ڲ���ַ
 * @param byte Ҫд����ֽ�
 * @return uint8_t �ɹ�����0
 */
uint8_t Inf_MPU6050_WriteByte(uint8_t reg_addr, uint8_t byte) {

    Dri_I2C_Start();
    Dri_I2C_SendAddr( MPU_IIC_ADDR << 1 );
    Dri_I2C_Transmit(reg_addr);
    Dri_I2C_Transmit(byte);
    Dri_I2C_Stop();

    return 0;

}


/**
 * @brief ����д��
 * 
 * @param reg_addr �豸�ڲ���ַ
 * @param bytes Ҫд�������
 * @return uint8_t �ɹ�����0
 */
uint8_t Inf_MPU6050_WriteStr(uint8_t reg_addr, uint8_t * bytes, uint8_t size) {

    Dri_I2C_Start();
    Dri_I2C_SendAddr( MPU_IIC_ADDR << 1 );
    Dri_I2C_Transmit(reg_addr);
    for(uint8_t i=0; i<size; i++) {
        Dri_I2C_Transmit(bytes[i]);
    }
    Dri_I2C_Stop();

    return 0;

}

/**
 * @brief ��ȡһ���ֽ�,���ݴ�ַ
 * 
 * @param reg_addr �豸�ڲ���ַ
 * @param byte �����ֽ�Ҫ��ŵĵط�
 * @return uint8_t �ɹ�����0
 */
uint8_t Inf_MPU6050_ReadByte(uint8_t reg_addr, uint8_t *byte) {

    Dri_I2C_Start();
    Dri_I2C_SendAddr(MPU_IIC_ADDR << 1);
    Dri_I2C_Transmit(reg_addr);
    Dri_I2C_Start();
    Dri_I2C_SendAddr((MPU_IIC_ADDR << 1) | 1);
    // ��ǰ׼����NACK
    Dri_I2C_NACK();
    *byte = Dri_I2C_ReadByte();

    Dri_I2C_Stop();

    return 0;
}

/**
 * @brief ������
 * 
 * @param reg_addr Ҫ��ȡ���ڲ���ַ
 * @param bytes ������������Ҫ��ŵĵط�
 * @param size Ҫ��ȡ�����ݸ���
 * @return uint8_t �ɹ�����0
 */
uint8_t Inf_MPU6050_ReadStr(uint8_t reg_addr, uint8_t *bytes, uint8_t size) {

    Dri_I2C_Start();
    Dri_I2C_SendAddr(MPU_IIC_ADDR << 1);
    Dri_I2C_Transmit(reg_addr);
    Dri_I2C_Start();
    Dri_I2C_SendAddr((MPU_IIC_ADDR << 1) | 1);
    for(uint8_t i = 0; i<size; i++) {
        if(i == size-1) {
            Dri_I2C_NACK();
        }
        bytes[i] = Dri_I2C_ReadByte();
    }
    Dri_I2C_Stop();

    return 0;
}


