#include "Com_PID.h"


/**
 * @brief ֱ������PD���ƣ�
 * 
 * @param angle С����б�� ���������˲���ĽǶȣ�
 * @param gryo ���ٶȣ�Y�ᣩ
 * @param balance_angle ��е��ֵ��ԭƽ��ʱ�ĽǶȣ�
 * @param Kp Pϵ��
 * @param Kd Dϵ��
 * @return int ֱ���������PWM
 */
int Com_PID_Balance(float angle, float gryo, float balance_angle, float Kp, float Kd) {

    
    /* ����Ƕ�ƫ�� = ��ʵ�Ƕ�-ƽ��Ƕ� */
    float bias = angle - balance_angle;
	printf("gyroY=%.1f\r\n",gryo);
    return Kp * bias + Kd * gryo;

}

/**
 * @brief �ٶȻ���PI���ƣ�
 * 
 * @param Encoder_A �����ٶȣ���������ֵ��
 * @param Encoder_B �����ٶȣ���������ֵ��
 * @param Kp Pϵ��
 * @param Ki Iϵ��
 * @return int �ٶȻ������PWM
 */
int Com_PID_Velocity(int Encoder_A, int Encoder_B, float Kp, float Ki) {
    int Encoder = 0;    //�ٶ�ƫ��
    static int last_Encoder = 0;    // �ϴε��ٶ�ƫ��
    static int Encoder_I = 0;   // λ�ƣ����ٶ�ƫ��Ļ���
    /* �����ٶ�ƫ�� = (A+B)/2-0 ===> ��/2����Ҳ���� */
    Encoder = Encoder_A + Encoder_B;
    if(last_Encoder == 0) last_Encoder = Encoder;
    /* һ�׵�ͨ�˲��� 0.8 * �ϴ�ƫ�� + 0.2 * ���ƫ�� */
    Encoder = last_Encoder * 0.8 + Encoder * 0.2;
    last_Encoder = Encoder;

    /* ���ٶ�ƫ�������֣�ֱ���ۼ� */
    Encoder_I += Encoder;

    /* �����޷� */
    if (Encoder_I > 10000)
    {
        Encoder_I = 10000;
    }
    else if (Encoder_I < -10000)
    {
        Encoder_I = -10000;
    }

    /* ��ʽ���� */
    return Kp * Encoder + Ki * Encoder_I;

}


/**
 * @description: ת�򻷿���
 * @param {float} Z_Angle   Z����ٶ�
 * @param {float} Kp        Pϵ��
 * @return {*}  ת�������pwm
 */
int Com_PID_Turn(float Z_Angle,float Kp)
{
    return Kp*Z_Angle;

}

