#include "Inf_MPU6050.h"


/**
 * @description: 设置陀螺仪的满量程
 * @param {uint8_t} fsr  满量程配置值：  3 ----- +-2000 °/s
 * @return {*}
 */
uint8_t Inf_MPU_Set_Gyro_Fsr(uint8_t fsr)
{
    /* bit4、bit3 */
    return Inf_MPU6050_WriteByte(MPU_GYRO_CFG_REG,fsr<<3);
}

/**
 * @description: 设置加速度计的满量程
 * @param {uint8_t} fsr 满量程配置值：  0  --------- +- 2g
 * @return {*}
 */
uint8_t Inf_MPU_Set_Accel_Fsr(uint8_t fsr)
{
    /* bit4、bit3 */
    return Inf_MPU6050_WriteByte(MPU_ACCEL_CFG_REG,fsr<<3);
}

/**
 * @description: 设置低通滤波器的带宽（截至频率，低于他的才能过）
 * @param {uint16_t} band_width 设置的带宽
 * @return {*}
 */
uint8_t Inf_MPU_Set_LPF(uint16_t band_width)
{
    uint8_t cfg=0;
    /* 从高的阈值往低的阈值判断（从最宽容到最严格要求） */
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
 * @description: 设置陀螺仪的采样率 = 输出频率/(1+分频)；设置低通滤波带宽=采样率/2;
 * @param {uint16_t} rate 期望的采样值：4-1000（手册min=4,1k/8k考虑加速度1k）
 * @return {*}
 */
uint8_t Inf_MPU_Set_Rate(uint16_t rate)
{
    uint8_t div=0;
    /* 采样频率=输出频率（1k）/(1+分频) ==》算出分频 ===》 设置到寄存器中*/
    /* 为了以后的通用性，可以做一个阈值限制 */
    if(rate > 1000)
    {
        rate = 1000;
    }
    else if(rate < 4)
    {
        rate = 4;
    }

    /* 计算分频 = 输出频率（1k）/ 采样频率rate -1 */
    div = 1000/rate - 1 ;
    /* 将计算得到的分频，写入 采样率分频计数器 */
    Inf_MPU6050_WriteByte(MPU_SAMPLE_RATE_REG,div);
    /* 顺便设置以下低通滤波的带宽 = 采样率/2 */
    return Inf_MPU_Set_LPF(rate/2);
}


void for_delay_ms(uint32_t ms)
{
    uint32_t Delay = ms * 72000 / 9; /* 72M时钟频率 */
    do
    {
        __NOP(); /* 空指令（NOP）来占用 CPU 时间 */
    } while (Delay--);
}

/**
 * @brief 初始化
 * 
 */
void Inf_MPU6050_Init(void) {


    // // 1.初始化IIC
    // Dri_I2C_Init();
    // // 2.电源管理1寄存器
    //     // 2.1 通过电源管理1寄存器先重启一下所有内部寄存器到默认值
    //     Inf_MPU6050_WriteByte(MPU_PWR_MGMT1_REG, 0x80);
    //     // 2.2 延时100ms等待重启完成
    //     for(uint32_t i=100*72000/9; i>1; i--) __NOP();
    //     // 2.3 配置电源管理1寄存器，使设备处于非睡眠
    //     Inf_MPU6050_WriteByte(MPU_PWR_MGMT1_REG, 0x01);
    // // 3.用户配置寄存器 （使用默认的全0就行）
    // // 4.加速度量程配置 
    // Inf_MPU6050_WriteByte(MPU_ACCEL_CFG_REG, 0x00 << 3);
    // // 5.陀螺仪配置，，满量程 +/-2000°
    // Inf_MPU6050_WriteByte(MPU_GYRO_CFG_REG, 0x03 << 3);

    // // 6.设置带宽，带宽至少是采样率的一半， 这里选188，， 
    // // 后面选采样率时注意，rate/2 >= 188,, 后面rate选500吧，500/s
    // Inf_MPU6050_WriteByte(MPU_CFG_REG, 0x01);

    // // 7.设置采样率
    // // 上面的配置，，使得陀螺仪输出频率=1kHz，，采样频率=陀螺仪输出频率/（1+SMPLRT_DIV）
    // // 如果使用500/s作为采样频率，则 分频值 = ( 1k / 500 ) - 1;
    // // 采样慢，输出快，，可能重复输出，【我猜的】
    // Inf_MPU6050_WriteByte(MPU_SAMPLE_RATE_REG, 0x01);

    // ===============================  可调采样率初始化，把上面注掉的函数打开 ===================================

    uint8_t res=0;
    /* 1、 初始化I2C2 */
    Dri_I2C_Init();

    /* 2、 复位 */
    Inf_MPU6050_WriteByte(MPU_PWR_MGMT1_REG, 0X80);
    /* 3、延迟一下 */
    // for(uint32_t i=100*72000/9; i>1; i--) __NOP();
    for_delay_ms(100);
    /* 4、 唤醒 */
    Inf_MPU6050_WriteByte(MPU_PWR_MGMT1_REG, 0X00);
    /* 5、设置满量程：陀螺仪（角速度），+- 2000 °/s ,设为11*/
    Inf_MPU_Set_Gyro_Fsr(3);
    /* 6、设置满量程: 加速度， +- 2g，设为0 */
    Inf_MPU_Set_Accel_Fsr(0);
    /* 7、禁用中断、FIFO、I2C AUX模式 */
    Inf_MPU6050_WriteByte(MPU_INT_EN_REG, 0X00);
    Inf_MPU6050_WriteByte(MPU_FIFO_EN_REG, 0X00);
    Inf_MPU6050_WriteByte(MPU_USER_CTRL_REG, 0X00);
    /* 判断是否可用，如果可以读到器件ID，说明前面的初始化没有错误 */
    Inf_MPU6050_ReadByte(MPU_DEVICE_ID_REG,&res);
    if (res == MPU_IIC_ADDR)
    {
        /* 8、时钟：默认8M晶振===》 X轴上的时钟 */
        Inf_MPU6050_WriteByte(MPU_PWR_MGMT1_REG, 0X01);
        /* 8、陀螺仪的采样率\设置低通滤波带宽(采样率一半) ，如果频率100，10ms采样一次*/
        Inf_MPU_Set_Rate(100);     
        /* 9、加速度与陀螺仪都工作 */
        Inf_MPU6050_WriteByte(MPU_PWR_MGMT2_REG, 0X00); 
    }
     
}

/**
 * @description: 获取加速度的原始数据
 * @param {short} *ax
 * @param {short} *ay
 * @param {short} *az
 * @return {*}
 */
uint16_t Inf_MPU6050_GetAccl(short *ax, short *ay, short *az)
{
    uint8_t acc_buff[6];
    /* 每个轴对应2个寄存器：高8位、低8位， 第一个是0x3B（X轴的高位） */
    Inf_MPU6050_ReadStr(MPU_ACCEL_XOUTH_REG,acc_buff,6);
    /* 
        acc_buff[0]:X轴加速度的高8位，
        acc_buff[1]:X轴加速度的低8位，
        acc_buff[2]:Y轴加速度的高8位，
        acc_buff[3]:Y轴加速度的低8位，
        acc_buff[4]:Z轴加速度的高8位，
        acc_buff[5]:Z轴加速度的低8位，
     */
    *ax= ((short)acc_buff[0] << 8) | acc_buff[1];
    *ay= ((short)acc_buff[2] << 8) | acc_buff[3];
    *az= ((short)acc_buff[4] << 8) | acc_buff[5];
}

/**
 * @description: 获取陀螺仪的角速度原始数据
 * @param {short} *gx
 * @param {short} *gy
 * @param {short} *gz
 * @return {*}
 */
uint16_t Inf_MPU6050_GetGyro(short *gx, short *gy, short *gz)
{
    uint8_t gyro_buff[6];
    /* 每个轴对应2个寄存器：高8位、低8位， 第一个是0x43（X轴的高位） */
    Inf_MPU6050_ReadStr(MPU_GYRO_XOUTH_REG,gyro_buff,6);
    /* 
        gyro_buff[0]:X轴角速度的高8位，
        gyro_buff[1]:X轴角速度的低8位，
        gyro_buff[2]:Y轴角速度的高8位，
        gyro_buff[3]:Y轴角速度的低8位，
        gyro_buff[4]:Z轴角速度的高8位，
        gyro_buff[5]:Z轴角速度的低8位，
     */
    *gx= ((short)gyro_buff[0] << 8) | gyro_buff[1];
    *gy= ((short)gyro_buff[2] << 8) | gyro_buff[3];
    *gz= ((short)gyro_buff[4] << 8) | gyro_buff[5];
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
    Dri_I2C_Stop();
    *byte = Dri_I2C_ReadByte();


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
        if (i == size - 1)
        {
            /* 7、 发送非应答、停止信号（建议最后一个字节前做） */
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


