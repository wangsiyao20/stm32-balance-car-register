#include "Inf_MPU6050.h"


/**
 * @description: ���������ǵ�������
 * @param {uint8_t} fsr  ����������ֵ��  3 ----- +-2000 ��/s
 * @return {*}
 */
uint8_t Inf_MPU_Set_Gyro_Fsr(uint8_t fsr)
{
    /* bit4��bit3 */
    return Inf_MPU6050_WriteByte(MPU_GYRO_CFG_REG,fsr<<3);
}

/**
 * @description: ���ü��ٶȼƵ�������
 * @param {uint8_t} fsr ����������ֵ��  0  --------- +- 2g
 * @return {*}
 */
uint8_t Inf_MPU_Set_Accel_Fsr(uint8_t fsr)
{
    /* bit4��bit3 */
    return Inf_MPU6050_WriteByte(MPU_ACCEL_CFG_REG,fsr<<3);
}

/**
 * @description: ���õ�ͨ�˲����Ĵ�������Ƶ�ʣ��������Ĳ��ܹ���
 * @param {uint16_t} band_width ���õĴ���
 * @return {*}
 */
uint8_t Inf_MPU_Set_LPF(uint16_t band_width)
{
    uint8_t cfg=0;
    /* �Ӹߵ���ֵ���͵���ֵ�жϣ�������ݵ����ϸ�Ҫ�� */
    if(band_width>= 188)
    {
        cfg=1;
    }
    else if(band_width >= 98)
    {
        cfg = 2;
    }
    else if (band_width >= 42)
    {
        cfg = 3;
    }
    else if (band_width >= 20)
    {
        cfg=4;
    }
    else if (band_width >=10)
    {
        cfg = 5;
    }
    else 
    {
        cfg =6;
    }
    /* bit2 --- bit0 */
    return Inf_MPU6050_WriteByte(MPU_CFG_REG,cfg);

}

/**
 * @description: ���������ǵĲ����� = ���Ƶ��/(1+��Ƶ)�����õ�ͨ�˲�����=������/2;
 * @param {uint16_t} rate �����Ĳ���ֵ��4-1000���ֲ�min=4,1k/8k���Ǽ��ٶ�1k��
 * @return {*}
 */
uint8_t Inf_MPU_Set_Rate(uint16_t rate)
{
    uint8_t div=0;
    /* ����Ƶ��=���Ƶ�ʣ�1k��/(1+��Ƶ) ==�������Ƶ ===�� ���õ��Ĵ�����*/
    /* Ϊ���Ժ��ͨ���ԣ�������һ����ֵ���� */
    if(rate > 1000)
    {
        rate = 1000;
    }
    else if(rate < 4)
    {
        rate = 4;
    }

    /* �����Ƶ = ���Ƶ�ʣ�1k��/ ����Ƶ��rate -1 */
    div = 1000/rate - 1 ;
    /* ������õ��ķ�Ƶ��д�� �����ʷ�Ƶ������ */
    Inf_MPU6050_WriteByte(MPU_SAMPLE_RATE_REG,div);
    /* ˳���������µ�ͨ�˲��Ĵ��� = ������/2 */
    return Inf_MPU_Set_LPF(rate/2);
}


void for_delay_ms(uint32_t ms)
{
    uint32_t Delay = ms * 72000 / 9; /* 72Mʱ��Ƶ�� */
    do
    {
        __NOP(); /* ��ָ�NOP����ռ�� CPU ʱ�� */
    } while (Delay--);
}

/**
 * @brief ��ʼ��
 * 
 */
void Inf_MPU6050_Init(void) {


    // // 1.��ʼ��IIC
    // Dri_I2C_Init();
    // // 2.��Դ����1�Ĵ���
    //     // 2.1 ͨ����Դ����1�Ĵ���������һ�������ڲ��Ĵ�����Ĭ��ֵ
    //     Inf_MPU6050_WriteByte(MPU_PWR_MGMT1_REG, 0x80);
    //     // 2.2 ��ʱ100ms�ȴ��������
    //     for(uint32_t i=100*72000/9; i>1; i--) __NOP();
    //     // 2.3 ���õ�Դ����1�Ĵ�����ʹ�豸���ڷ�˯��
    //     Inf_MPU6050_WriteByte(MPU_PWR_MGMT1_REG, 0x01);
    // // 3.�û����üĴ��� ��ʹ��Ĭ�ϵ�ȫ0���У�
    // // 4.���ٶ��������� 
    // Inf_MPU6050_WriteByte(MPU_ACCEL_CFG_REG, 0x00 << 3);
    // // 5.���������ã��������� +/-2000��
    // Inf_MPU6050_WriteByte(MPU_GYRO_CFG_REG, 0x03 << 3);

    // // 6.���ô������������ǲ����ʵ�һ�룬 ����ѡ188���� 
    // // ����ѡ������ʱע�⣬rate/2 >= 188,, ����rateѡ500�ɣ�500/s
    // Inf_MPU6050_WriteByte(MPU_CFG_REG, 0x01);

    // // 7.���ò�����
    // // ��������ã���ʹ�����������Ƶ��=1kHz��������Ƶ��=���������Ƶ��/��1+SMPLRT_DIV��
    // // ���ʹ��500/s��Ϊ����Ƶ�ʣ��� ��Ƶֵ = ( 1k / 500 ) - 1;
    // // ������������죬�������ظ���������Ҳµġ�
    // Inf_MPU6050_WriteByte(MPU_SAMPLE_RATE_REG, 0x01);

    // ===============================  �ɵ������ʳ�ʼ����������ע���ĺ����� ===================================

    uint8_t res=0;
    /* 1�� ��ʼ��I2C2 */
    Dri_I2C_Init();

    /* 2�� ��λ */
    Inf_MPU6050_WriteByte(MPU_PWR_MGMT1_REG, 0X80);
    /* 3���ӳ�һ�� */
    // for(uint32_t i=100*72000/9; i>1; i--) __NOP();
    for_delay_ms(100);
    /* 4�� ���� */
    Inf_MPU6050_WriteByte(MPU_PWR_MGMT1_REG, 0X00);
    /* 5�����������̣������ǣ����ٶȣ���+- 2000 ��/s ,��Ϊ11*/
    Inf_MPU_Set_Gyro_Fsr(3);
    /* 6������������: ���ٶȣ� +- 2g����Ϊ0 */
    Inf_MPU_Set_Accel_Fsr(0);
    /* 7�������жϡ�FIFO��I2C AUXģʽ */
    Inf_MPU6050_WriteByte(MPU_INT_EN_REG, 0X00);
    Inf_MPU6050_WriteByte(MPU_FIFO_EN_REG, 0X00);
    Inf_MPU6050_WriteByte(MPU_USER_CTRL_REG, 0X00);
    /* �ж��Ƿ���ã�������Զ�������ID��˵��ǰ��ĳ�ʼ��û�д��� */
    Inf_MPU6050_ReadByte(MPU_DEVICE_ID_REG,&res);
    if (res == MPU_IIC_ADDR)
    {
        /* 8��ʱ�ӣ�Ĭ��8M����===�� X���ϵ�ʱ�� */
        Inf_MPU6050_WriteByte(MPU_PWR_MGMT1_REG, 0X01);
        /* 8�������ǵĲ�����\���õ�ͨ�˲�����(������һ��) �����Ƶ��100��10ms����һ��*/
        Inf_MPU_Set_Rate(100);     
        /* 9�����ٶ��������Ƕ����� */
        Inf_MPU6050_WriteByte(MPU_PWR_MGMT2_REG, 0X00); 
    }
     
}

/**
 * @description: ��ȡ���ٶȵ�ԭʼ����
 * @param {short} *ax
 * @param {short} *ay
 * @param {short} *az
 * @return {*}
 */
uint16_t Inf_MPU6050_GetAccl(short *ax, short *ay, short *az)
{
    uint8_t acc_buff[6];
    /* ÿ�����Ӧ2���Ĵ�������8λ����8λ�� ��һ����0x3B��X��ĸ�λ�� */
    Inf_MPU6050_ReadStr(MPU_ACCEL_XOUTH_REG,acc_buff,6);
    /* 
        acc_buff[0]:X����ٶȵĸ�8λ��
        acc_buff[1]:X����ٶȵĵ�8λ��
        acc_buff[2]:Y����ٶȵĸ�8λ��
        acc_buff[3]:Y����ٶȵĵ�8λ��
        acc_buff[4]:Z����ٶȵĸ�8λ��
        acc_buff[5]:Z����ٶȵĵ�8λ��
     */
    *ax= ((short)acc_buff[0] << 8) | acc_buff[1];
    *ay= ((short)acc_buff[2] << 8) | acc_buff[3];
    *az= ((short)acc_buff[4] << 8) | acc_buff[5];
}

/**
 * @description: ��ȡ�����ǵĽ��ٶ�ԭʼ����
 * @param {short} *gx
 * @param {short} *gy
 * @param {short} *gz
 * @return {*}
 */
uint16_t Inf_MPU6050_GetGyro(short *gx, short *gy, short *gz)
{
    uint8_t gyro_buff[6];
    /* ÿ�����Ӧ2���Ĵ�������8λ����8λ�� ��һ����0x43��X��ĸ�λ�� */
    Inf_MPU6050_ReadStr(MPU_GYRO_XOUTH_REG,gyro_buff,6);
    /* 
        gyro_buff[0]:X����ٶȵĸ�8λ��
        gyro_buff[1]:X����ٶȵĵ�8λ��
        gyro_buff[2]:Y����ٶȵĸ�8λ��
        gyro_buff[3]:Y����ٶȵĵ�8λ��
        gyro_buff[4]:Z����ٶȵĸ�8λ��
        gyro_buff[5]:Z����ٶȵĵ�8λ��
     */
    *gx= ((short)gyro_buff[0] << 8) | gyro_buff[1];
    *gy= ((short)gyro_buff[2] << 8) | gyro_buff[3];
    *gz= ((short)gyro_buff[4] << 8) | gyro_buff[5];
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
    Dri_I2C_Stop();
    *byte = Dri_I2C_ReadByte();


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
        if (i == size - 1)
        {
            /* 7�� ���ͷ�Ӧ��ֹͣ�źţ��������һ���ֽ�ǰ���� */
            Dri_I2C_NACK();
            Dri_I2C_Stop();
        }
        else
        {
            Dri_I2C_ACK();
        }
        bytes[i] = Dri_I2C_ReadByte();
    }

    return 0;
}


