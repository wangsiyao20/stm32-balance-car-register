#include "App_Car.h"


#define PI 3.1415926
extern float angle; // �������˲������б�Ƕ�

int16_t encoder_Count2, encoder_Count3;  // ����������ֵ


float GryoY = 0; // Y����ٶ�
float GryoZ = 0; // Z����ٶ�

/* ֱ���� */
float balance_angle = 2.3; // �ǶȻ�е��ֵ 5.6
float balance_Kp = -640.0;  // -900
float balance_Kd = -8.1;   // -13

/* �ٶȻ� */
float velocity_Kp = -0.0;  //
float velocity_Ki = -0.0;    // Kp=1/200 Kp

/* ת�� */
float turn_Kp = 0.0;

// /* ֱ���� */
// float balance_angle = 2.3; // �ǶȻ�е��ֵ 2.0
// float balance_Kp = -850.0;  // -900
// float balance_Kd = -12.0;   // -13
// /* �ٶȻ� */
// float velocity_Kp = -0.0;  //
// float velocity_Ki = -0.0;    // Kp=1/200 Kp
// /* ת�� */
// float turn_Kp = 0.0;


void App_Car_Init() {
	Driver_USART1_Init();
	Dri_ADC1_Init();        // �ɼ���Դ��ѹ
	Inf_OLED1315_Init();    // oled��ʾ��
	Inf_TB6612_Init();      // ���������������tim���pwm������
	Inf_MPU6050_Init();     // ���ٶȣ���������Ƕ�
    // Int_Encoder_Init();     // ͨ��tim�����������ֵ
}


/**
 * С��oled��ʾģ��
 */
void App_Car_OLED() {
	static char EA[7], EB[7]; // 1������λ+5������λ+1������λ
    static char AngleStr[6];  // 1������λ+2������+1��С����+1��С��+1������λ
	static char BAT[5];       // 2������+1��С����+1��С��+1������λ
    static double BAT_BAL = 0.0;
    static uint8_t bat_count = 0;
    /* ��ʾ��ص�ѹ: 1sˢ��һ�� */
    if (bat_count++ == 20)      // 20*50ms���� 50ms����һ���������
    {
        bat_count = 0;
        BAT_BAL = Dri_ADC1_ReadV();
        sprintf(BAT, "%3.1f", BAT_BAL);
        Inf_OLED1315_ShowString(32, 0, BAT, 16, 1); /* ��һ��===��BAT: ��ѹֵ��������8��*16�� */
    }
    /* ��ʾ��������ֵ��50msˢ��һ��(��Ӧ��������ö�ȡ����������ֱ�Ӷ�ÿ10ms��ȡ�Ľ��) */
    sprintf(EA, "%6d", encoder_Count2);
    sprintf(EB, "%6d", encoder_Count3);
    Inf_OLED1315_ShowString(24, 16, EA, 16, 1); /* �ڶ���===��EA: ����ֵ��������8��*16�� */
    Inf_OLED1315_ShowString(24, 32, EB, 16, 1); /* ������===��EB: ����ֵ��������8��*16�� */
    /* ��ʾ��������ǣ�50msˢ��һ�� */
    sprintf(AngleStr, "%4.1f", angle);
    Inf_OLED1315_ShowString(48, 48, AngleStr, 16, 1); /* ������===��Angle: �Ƕ�ֵ��������8��*16�� */
    /* ͳһˢд���Դ��� */
    Inf_OLED1315_Refresh();
}

/**
 * @brief ��ȡ�ǶȺͱ�������ֵ
 * 
 */
float Acc_Angle=0; /* ͨ�����ٶȼ����������� */
	
short ax=0, ay=0, az=0;  // xyz���ϵļ��ٶ�
short gx=0, gy=0, gz=0;  // xyz���ϵĽ��ٶ�
void App_Car_GetAngle_Encoder() {
    
//    ax=0,ay=0,az=0;
//    gx=0,gy=0,gz=0;
    Inf_MPU6050_GetAccl(&ax,&ay,&az);
    Inf_MPU6050_GetGyro(&gx,&gy,&gz);
    Acc_Angle = (float)(atan2(ax, az) * 180 / PI);
    
    // printf("ƫб�Ƕ�Ϊ��%f��\r\n", Acc_Angle);
    GryoY = -gy/16.4;
	/* �ó�Z����ٶȣ�����ת��Ҫ�ã� */
    GryoZ = gz / 16.4;
    Com_Filter_Kalman(Acc_Angle, GryoY);
    // printf("�����������˲��ںϼ���ĽǶ�=%2.1f\r\n",angle);
    // encoder_Count2 = Int_Encoder_ReadCounter(2);
    // encoder_Count3 = Int_Encoder_ReadCounter(3);
    // printf("������ֵΪ��%dxxxxxxx%d\r\n", encoder_Count2, encoder_Count3);
    //for(uint32_t i=100*72000/9; i>1; i--) __NOP();	// ��ʱ100ms
}

/**
 * @brief ����PID����
 * 
 */
void App_Car_Control() {

    // 1.ֱ����PD����
    int balance_out = Com_PID_Balance(angle, GryoY, balance_angle, balance_Kp, balance_Kd);

    // 2.�ٶȻ�PI����
    int velocity_out = Com_PID_Velocity(encoder_Count2, encoder_Count3, velocity_Kp, velocity_Ki);

    // 3.ת��P����
    int turn_out = Com_PID_Turn(gz, turn_Kp);

    // int turn_out = 0;

    // 4.ͨ��PID�㷨���õ�3��������
    int PWMA = balance_out + velocity_out + turn_out;
    int PWMB = balance_out + velocity_out - turn_out;

    Inf_TB6612_SetPWM(PWMA,PWMB);
	// Inf_TB6612_SetPWM(0,1000);

}


