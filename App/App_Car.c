#include "App_Car.h"


#define PI 3.1415926
extern float angle; // 卡尔曼滤波后的倾斜角度

int16_t encoder_Count2, encoder_Count3;  // 编码器计数值


float GryoY = 0; // Y轴角速度
float GryoZ = 0; // Z轴角速度

/* 直立环 */
float balance_angle = 2.3; // 角度机械中值 5.6
float balance_Kp = -640.0;  // -900
float balance_Kd = -8.1;   // -13

/* 速度环 */
float velocity_Kp = -0.0;  //
float velocity_Ki = -0.0;    // Kp=1/200 Kp

/* 转向环 */
float turn_Kp = 0.0;

// /* 直立环 */
// float balance_angle = 2.3; // 角度机械中值 2.0
// float balance_Kp = -850.0;  // -900
// float balance_Kd = -12.0;   // -13
// /* 速度环 */
// float velocity_Kp = -0.0;  //
// float velocity_Ki = -0.0;    // Kp=1/200 Kp
// /* 转向环 */
// float turn_Kp = 0.0;


void App_Car_Init() {
	Driver_USART1_Init();
	Dri_ADC1_Init();        // 采集电源电压
	Inf_OLED1315_Init();    // oled显示屏
	Inf_TB6612_Init();      // 电机驱动，（包括tim输出pwm方波）
	Inf_MPU6050_Init();     // 加速度，，计算出角度
    // Int_Encoder_Init();     // 通过tim计算编码器的值
}


/**
 * 小车oled显示模块
 */
void App_Car_OLED() {
	static char EA[7], EB[7]; // 1个符号位+5个数字位+1个结束位
    static char AngleStr[6];  // 1个符号位+2个整数+1个小数点+1个小数+1个结束位
	static char BAT[5];       // 2个整数+1个小数点+1个小数+1个结束位
    static double BAT_BAL = 0.0;
    static uint8_t bat_count = 0;
    /* 显示电池电压: 1s刷新一次 */
    if (bat_count++ == 20)      // 20*50ms，， 50ms调用一次这个函数
    {
        bat_count = 0;
        BAT_BAL = Dri_ADC1_ReadV();
        sprintf(BAT, "%3.1f", BAT_BAL);
        Inf_OLED1315_ShowString(32, 0, BAT, 16, 1); /* 第一行===》BAT: 电压值，字体是8宽*16高 */
    }
    /* 显示编码器的值：50ms刷新一次(不应在这里调用读取函数，而是直接读每10ms读取的结果) */
    sprintf(EA, "%6d", encoder_Count2);
    sprintf(EB, "%6d", encoder_Count3);
    Inf_OLED1315_ShowString(24, 16, EA, 16, 1); /* 第二行===》EA: 编码值，字体是8宽*16高 */
    Inf_OLED1315_ShowString(24, 32, EB, 16, 1); /* 第三行===》EB: 编码值，字体是8宽*16高 */
    /* 显示计算后的倾角：50ms刷新一次 */
    sprintf(AngleStr, "%4.1f", angle);
    Inf_OLED1315_ShowString(48, 48, AngleStr, 16, 1); /* 第四行===》Angle: 角度值，字体是8宽*16高 */
    /* 统一刷写到显存中 */
    Inf_OLED1315_Refresh();
}

/**
 * @brief 获取角度和编码器的值
 * 
 */
float Acc_Angle=0; /* 通过加速度计算出来的倾角 */
	
short ax=0, ay=0, az=0;  // xyz轴上的加速度
short gx=0, gy=0, gz=0;  // xyz轴上的角速度
void App_Car_GetAngle_Encoder() {
    
//    ax=0,ay=0,az=0;
//    gx=0,gy=0,gz=0;
    Inf_MPU6050_GetAccl(&ax,&ay,&az);
    Inf_MPU6050_GetGyro(&gx,&gy,&gz);
    Acc_Angle = (float)(atan2(ax, az) * 180 / PI);
    
    // printf("偏斜角度为：%f°\r\n", Acc_Angle);
    GryoY = -gy/16.4;
	/* 得出Z轴角速度（后续转向环要用） */
    GryoZ = gz / 16.4;
    Com_Filter_Kalman(Acc_Angle, GryoY);
    // printf("经过卡尔曼滤波融合计算的角度=%2.1f\r\n",angle);
    // encoder_Count2 = Int_Encoder_ReadCounter(2);
    // encoder_Count3 = Int_Encoder_ReadCounter(3);
    // printf("读到的值为：%dxxxxxxx%d\r\n", encoder_Count2, encoder_Count3);
    //for(uint32_t i=100*72000/9; i>1; i--) __NOP();	// 延时100ms
}

/**
 * @brief 进行PID控制
 * 
 */
void App_Car_Control() {

    // 1.直立环PD控制
    int balance_out = Com_PID_Balance(angle, GryoY, balance_angle, balance_Kp, balance_Kd);

    // 2.速度环PI控制
    int velocity_out = Com_PID_Velocity(encoder_Count2, encoder_Count3, velocity_Kp, velocity_Ki);

    // 3.转向环P控制
    int turn_out = Com_PID_Turn(gz, turn_Kp);

    // int turn_out = 0;

    // 4.通过PID算法设置到3个输出相加
    int PWMA = balance_out + velocity_out + turn_out;
    int PWMB = balance_out + velocity_out - turn_out;

    Inf_TB6612_SetPWM(PWMA,PWMB);
	// Inf_TB6612_SetPWM(0,1000);

}


