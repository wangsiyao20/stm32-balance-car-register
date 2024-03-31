#include "Com_PID.h"


/**
 * @brief 直立环（PD控制）
 * 
 * @param angle 小车倾斜角 （卡尔曼滤波后的角度）
 * @param gryo 角速度（Y轴）
 * @param balance_angle 机械中值（原平衡时的角度）
 * @param Kp P系数
 * @param Kd D系数
 * @return int 直立环输出的PWM
 */
int Com_PID_Balance(float angle, float gryo, float balance_angle, float Kp, float Kd) {

    
    /* 计算角度偏差 = 真实角度-平衡角度 */
    float bias = angle - balance_angle;
	printf("gyroY=%.1f\r\n",gryo);
    return Kp * bias + Kd * gryo;

}

/**
 * @brief 速度环（PI控制）
 * 
 * @param Encoder_A 左轮速度（编码器的值）
 * @param Encoder_B 右轮速度（编码器的值）
 * @param Kp P系数
 * @param Ki I系数
 * @return int 速度环输出的PWM
 */
int Com_PID_Velocity(int Encoder_A, int Encoder_B, float Kp, float Ki) {
    int Encoder = 0;    //速度偏差
    static int last_Encoder = 0;    // 上次的速度偏差
    static int Encoder_I = 0;   // 位移，，速度偏差的积分
    /* 计算速度偏差 = (A+B)/2-0 ===> 不/2，，也可以 */
    Encoder = Encoder_A + Encoder_B;
    if(last_Encoder == 0) last_Encoder = Encoder;
    /* 一阶低通滤波： 0.8 * 上次偏差 + 0.2 * 这次偏差 */
    Encoder = last_Encoder * 0.8 + Encoder * 0.2;
    last_Encoder = Encoder;

    /* 对速度偏差作积分：直接累加 */
    Encoder_I += Encoder;

    /* 积分限幅 */
    if (Encoder_I > 10000)
    {
        Encoder_I = 10000;
    }
    else if (Encoder_I < -10000)
    {
        Encoder_I = -10000;
    }

    /* 公式计算 */
    return Kp * Encoder + Ki * Encoder_I;

}


/**
 * @description: 转向环控制
 * @param {float} Z_Angle   Z轴角速度
 * @param {float} Kp        P系数
 * @return {*}  转向环输出的pwm
 */
int Com_PID_Turn(float Z_Angle,float Kp)
{
    return Kp*Z_Angle;

}

