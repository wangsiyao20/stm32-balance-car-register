#include "Inf_MPU6050.h"

/**
 * @brief 初始化
 * 
 */
void Inf_MPU6050_Init(void) {
    // 1.初始化IIC
    Dri_I2C_Init();
    // 2.
}

/**
 * @brief 写字节
 * 
 * @param reg_addr 设备内部地址
 * @param byte 要写入的字节
 * @return uint8_t 成功返回0
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
 * @brief 连续写入
 * 
 * @param reg_addr 设备内部地址
 * @param bytes 要写入的数据
 * @return uint8_t 成功返回0
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
 * @brief 读取一个字节,数据传址
 * 
 * @param reg_addr 设备内部地址
 * @param byte 读来字节要存放的地方
 * @return uint8_t 成功返回0
 */
uint8_t Inf_MPU6050_ReadByte(uint8_t reg_addr, uint8_t *byte) {

    Dri_I2C_Start();
    Dri_I2C_SendAddr(MPU_IIC_ADDR << 1);
    Dri_I2C_Transmit(reg_addr);
    Dri_I2C_Start();
    Dri_I2C_SendAddr((MPU_IIC_ADDR << 1) | 1);
    // 提前准备好NACK
    Dri_I2C_NACK();
    *byte = Dri_I2C_ReadByte();

    Dri_I2C_Stop();

    return 0;
}

/**
 * @brief 连续读
 * 
 * @param reg_addr 要读取的内部地址
 * @param bytes 读出来的数据要存放的地方
 * @param size 要读取的数据个数
 * @return uint8_t 成功返回0
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


