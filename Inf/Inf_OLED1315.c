#include "Inf_OLED1315.h"
#include "Dri_SPI.h"
#include "App_Task.h"
#include "oledfont.h"

uint8_t OLED_GRAM[144][8];


/**
 * 写字节	
 */
void Inf_OLED1315_WR_Byte(uint8_t dat, uint8_t mode) {
	
	// 1.判断是写命令还是写数据
	if(mode) {
		// 1.1 写数据，拉高DC
		DC_HIGH();
	} else {
		// 1.2 写命令，拉低DC
		DC_LOW();
	}
	
	// 2.发送一个字节
	Dri_SPI_WriteByte(dat);
	
	// 3.DC设为1
	DC_HIGH();
}


/**
 * 开启OLED显示
 */
void Inf_OLED1315_DisPlay_On(void)
{
	Inf_OLED1315_WR_Byte(0x8D, OLED_CMD); // 电荷泵使能
	Inf_OLED1315_WR_Byte(0x14, OLED_CMD); // 开启电荷泵
	Inf_OLED1315_WR_Byte(0xAF, OLED_CMD); // 点亮屏幕
}

/**
 * 关闭OLED显示
 */
void Inf_OLED1315_DisPlay_Off(void)
{
	Inf_OLED1315_WR_Byte(0x8D, OLED_CMD); // 电荷泵使能
	Inf_OLED1315_WR_Byte(0x10, OLED_CMD); // 关闭电荷泵
	Inf_OLED1315_WR_Byte(0xAE, OLED_CMD); // 关闭屏幕
}

/**
 * 更新显存到OLED
 */
void Inf_OLED1315_Refresh(void)
{

	for (uint8_t i = 0; i<8; i++) {
		Inf_OLED1315_WR_Byte(0xb0+i, OLED_CMD);   // 设置行起始地址
		Inf_OLED1315_WR_Byte(0x00, OLED_CMD);	 // 设置低列起始地址
		Inf_OLED1315_WR_Byte(0x10, OLED_CMD);    // 设置高列起始地址
		for (uint8_t n = 0; n<128; n++) {
			Inf_OLED1315_WR_Byte(OLED_GRAM[n][i], OLED_DATA);
		}
	}
}


/**
 * 清屏函数
 */
void Inf_OLED1315_Clear(void)
{

	for (uint8_t i=0; i<8; i++)
	{
		for (uint8_t n = 0; n < 128; n++)
		{
			OLED_GRAM[n][i] = 0; // 清除所有数据
		}
	}
	Inf_OLED1315_Refresh(); // 更新显示
}


/**
 * 画点
 */
void Inf_OLED1315_DrawPoint(uint8_t x, uint8_t y, uint8_t t) {
	uint8_t i, m, n;
	i = y / 8;	//第几页：0-7
	m = y % 8;	// 页中第几行：0-7
	n = 1 << m;
	if(t) {
		OLED_GRAM[x][i] |= n;
	} else {
		OLED_GRAM[x][i] = ~OLED_GRAM[x][i];
		OLED_GRAM[x][i] |= n;
		OLED_GRAM[x][i] = ~OLED_GRAM[x][i];
	}
}


//在指定位置显示一个字符,包括部分字符
//x:0~127
//y:0~63
//size1:选择字体 6x8/6x12/8x16/12x24
//mode:0,反色显示;1,正常显示
void Inf_OLED1315_ShowChar(uint8_t x, uint8_t y, uint8_t chr, uint8_t size1, uint8_t mode)
{
	u8 i,m,temp,size2,chr1;
	u8 x0=x,y0=y;
	if(size1==8)size2=6;
	else size2=(size1/8+((size1%8)?1:0))*(size1/2);  //得到字体一个字符对应点阵集所占的字节数
	chr1=chr-' ';  //计算偏移后的值
	for(i=0;i<size2;i++)
	{
		if(size1==8)
			  {temp=asc2_0806[chr1][i];} //调用0806字体
		else if(size1==12)
        {temp=asc2_0806[chr1][i];} //调用1206字体
		else if(size1==16)
        {temp=asc2_0806[chr1][i];} //调用1608字体
		else if(size1==24)
        {temp=asc2_0806[chr1][i];} //调用2412字体
		else return;
		for(m=0;m<8;m++)
		{
			if(temp&0x01)Inf_OLED1315_DrawPoint(x,y,mode);
			else Inf_OLED1315_DrawPoint(x,y,!mode);
			temp>>=1;
			y++;
		}
		x++;
		if((size1!=8)&&((x-x0)==size1/2))
		{x=x0;y0=y0+8;}
		y=y0;
  }
}



/**
 * 初始化
 */
void Inf_OLED1315_Init(void)
{
    uint32_t i=200*72000;
    /* 1. 初始化SPI */
    Dri_SPI_Init();

    /* 2. 初始化DC和RES引脚*/
    /* 2.1. 开启GPIO的PB时钟 */
    RCC->APB2ENR |= RCC_APB2ENR_IOPBEN;
    /* 2.2. 初始化引脚：RES(PB0) DC(PB1) 使用通用推挽输出 MODE=11 CNF:00*/
    GPIOB->CRL |= (GPIO_CRL_MODE0|GPIO_CRL_MODE1);
    GPIOB->CRL &= ~(GPIO_CRL_CNF0|GPIO_CRL_CNF1);
    /* 2.3. 默认数据模式，DC=1 */
    DC_HIGH(); 
    /* 2.4. 初始化期间不复位，RES=1 */
    RES_HIGH(); 

    /* 3. 重置OLED：RES拉低-->200ms-->RES拉高 */
    RES_LOW();
    // vTaskDelay(200);
    while(i--);
    RES_HIGH();

    /* 4. oled配置*/
    Inf_OLED1315_WR_Byte(0xAE, OLED_CMD); // 关闭显示

    Inf_OLED1315_WR_Byte(0x00, OLED_CMD); // 设置低列地址
    Inf_OLED1315_WR_Byte(0x10, OLED_CMD); // 设置高列地址
    Inf_OLED1315_WR_Byte(0x40, OLED_CMD); // 设置起始行地址（映射 RAM 显示起始行）(0x00~0x3F)
	  
    Inf_OLED1315_WR_Byte(0x81, OLED_CMD); // 对比度控制寄存器，开启对比度设置
    Inf_OLED1315_WR_Byte(0xCF, OLED_CMD); // 对比度（最高0xFF）
	  
    Inf_OLED1315_WR_Byte(0xA1, OLED_CMD); // 设置列扫描方向（0xa0: 左右反置，0xa1: 正常）
	  
    Inf_OLED1315_WR_Byte(0xC8, OLED_CMD); // 设置行扫描方向（0xc0: 上下反置，0xc8: 正常）
	  
    Inf_OLED1315_WR_Byte(0xA6, OLED_CMD); // 设置正常显示
    Inf_OLED1315_WR_Byte(0xA8, OLED_CMD); // 设置多路复用比（1 到 64）
    Inf_OLED1315_WR_Byte(0x3f, OLED_CMD); // 设置占空比（1/64）
	  
    Inf_OLED1315_WR_Byte(0xD3, OLED_CMD); // 设置显示偏移（移动映射 RAM 计数器）
    Inf_OLED1315_WR_Byte(0x00, OLED_CMD); // 不偏移
	  
    Inf_OLED1315_WR_Byte(0xd5, OLED_CMD); // 设置显示时钟分频比/振荡器频率
    Inf_OLED1315_WR_Byte(0x80, OLED_CMD); // 设置分频比，设置时钟为 100 帧/秒
	  
    Inf_OLED1315_WR_Byte(0xD9, OLED_CMD); // 设置预充电周期 可以提高OLED的响应速度
    Inf_OLED1315_WR_Byte(0xF1, OLED_CMD); // 充电15个时钟周期 放电1个时钟周期
	  
    Inf_OLED1315_WR_Byte(0xDA, OLED_CMD); // 设置 COM 引脚硬件配置
    Inf_OLED1315_WR_Byte(0x12, OLED_CMD); // 设置 COM 引脚硬件配置值
	  
    Inf_OLED1315_WR_Byte(0xDB, OLED_CMD); // 设置 VCOMH
    Inf_OLED1315_WR_Byte(0x40, OLED_CMD); // 设置 VCOM 取消电压水平
	  
    Inf_OLED1315_WR_Byte(0x20, OLED_CMD); // 设置内存地址模式(水平0x00/垂直0x01/页0x02)
    Inf_OLED1315_WR_Byte(0x02, OLED_CMD); // 内存地址模式设置为 页地址模式
	  
    Inf_OLED1315_WR_Byte(0x8D, OLED_CMD); // 设置电流泵(0x14开启 0x10 关闭)
    Inf_OLED1315_WR_Byte(0x14, OLED_CMD); // 关闭电流泵
	  
    Inf_OLED1315_WR_Byte(0xA4, OLED_CMD); // 关闭整个显示（0xa4/0xa5）
    Inf_OLED1315_WR_Byte(0xA6, OLED_CMD); // 关闭反显显示（0xa6/0xa7）
    Inf_OLED1315_Clear();                 // 清屏
    Inf_OLED1315_WR_Byte(0xAF, OLED_CMD); // 打开 OLED 面板
}




