#include "Inf_OLED1315.h"
#include "Dri_SPI.h"
#include "App_Task.h"
#include "oledfont.h"

uint8_t OLED_GRAM[144][8];


/**
 * д�ֽ�	
 */
void Inf_OLED1315_WR_Byte(uint8_t dat, uint8_t mode) {
	
	// 1.�ж���д�����д����
	if(mode) {
		// 1.1 д���ݣ�����DC
		DC_HIGH();
	} else {
		// 1.2 д�������DC
		DC_LOW();
	}
	
	// 2.����һ���ֽ�
	Dri_SPI_WriteByte(dat);
	
	// 3.DC��Ϊ1
	DC_HIGH();
}


/**
 * ����OLED��ʾ
 */
void Inf_OLED1315_DisPlay_On(void)
{
	Inf_OLED1315_WR_Byte(0x8D, OLED_CMD); // ��ɱ�ʹ��
	Inf_OLED1315_WR_Byte(0x14, OLED_CMD); // ������ɱ�
	Inf_OLED1315_WR_Byte(0xAF, OLED_CMD); // ������Ļ
}

/**
 * �ر�OLED��ʾ
 */
void Inf_OLED1315_DisPlay_Off(void)
{
	Inf_OLED1315_WR_Byte(0x8D, OLED_CMD); // ��ɱ�ʹ��
	Inf_OLED1315_WR_Byte(0x10, OLED_CMD); // �رյ�ɱ�
	Inf_OLED1315_WR_Byte(0xAE, OLED_CMD); // �ر���Ļ
}

/**
 * �����Դ浽OLED
 */
void Inf_OLED1315_Refresh(void)
{

	for (uint8_t i = 0; i<8; i++) {
		Inf_OLED1315_WR_Byte(0xb0+i, OLED_CMD);   // ��������ʼ��ַ
		Inf_OLED1315_WR_Byte(0x00, OLED_CMD);	 // ���õ�����ʼ��ַ
		Inf_OLED1315_WR_Byte(0x10, OLED_CMD);    // ���ø�����ʼ��ַ
		for (uint8_t n = 0; n<128; n++) {
			Inf_OLED1315_WR_Byte(OLED_GRAM[n][i], OLED_DATA);
		}
	}
}


/**
 * ��������
 */
void Inf_OLED1315_Clear(void)
{

	for (uint8_t i=0; i<8; i++)
	{
		for (uint8_t n = 0; n < 128; n++)
		{
			OLED_GRAM[n][i] = 0; // �����������
		}
	}
	Inf_OLED1315_Refresh(); // ������ʾ
}


/**
 * ����
 */
void Inf_OLED1315_DrawPoint(uint8_t x, uint8_t y, uint8_t t) {
	uint8_t i, m, n;
	i = y / 8;	//�ڼ�ҳ��0-7
	m = y % 8;	// ҳ�еڼ��У�0-7
	n = 1 << m;
	if(t) {
		OLED_GRAM[x][i] |= n;
	} else {
		OLED_GRAM[x][i] = ~OLED_GRAM[x][i];
		OLED_GRAM[x][i] |= n;
		OLED_GRAM[x][i] = ~OLED_GRAM[x][i];
	}
}


//��ָ��λ����ʾһ���ַ�,���������ַ�
//x:0~127
//y:0~63
//size1:ѡ������ 6x8/6x12/8x16/12x24
//mode:0,��ɫ��ʾ;1,������ʾ
void Inf_OLED1315_ShowChar(uint8_t x, uint8_t y, uint8_t chr, uint8_t size1, uint8_t mode)
{
	u8 i,m,temp,size2,chr1;
	u8 x0=x,y0=y;
	if(size1==8)size2=6;
	else size2=(size1/8+((size1%8)?1:0))*(size1/2);  //�õ�����һ���ַ���Ӧ������ռ���ֽ���
	chr1=chr-' ';  //����ƫ�ƺ��ֵ
	for(i=0;i<size2;i++)
	{
		if(size1==8)
			  {temp=asc2_0806[chr1][i];} //����0806����
		else if(size1==12)
        {temp=asc2_0806[chr1][i];} //����1206����
		else if(size1==16)
        {temp=asc2_0806[chr1][i];} //����1608����
		else if(size1==24)
        {temp=asc2_0806[chr1][i];} //����2412����
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
 * ��ʼ��
 */
void Inf_OLED1315_Init(void)
{
    uint32_t i=200*72000;
    /* 1. ��ʼ��SPI */
    Dri_SPI_Init();

    /* 2. ��ʼ��DC��RES����*/
    /* 2.1. ����GPIO��PBʱ�� */
    RCC->APB2ENR |= RCC_APB2ENR_IOPBEN;
    /* 2.2. ��ʼ�����ţ�RES(PB0) DC(PB1) ʹ��ͨ��������� MODE=11 CNF:00*/
    GPIOB->CRL |= (GPIO_CRL_MODE0|GPIO_CRL_MODE1);
    GPIOB->CRL &= ~(GPIO_CRL_CNF0|GPIO_CRL_CNF1);
    /* 2.3. Ĭ������ģʽ��DC=1 */
    DC_HIGH(); 
    /* 2.4. ��ʼ���ڼ䲻��λ��RES=1 */
    RES_HIGH(); 

    /* 3. ����OLED��RES����-->200ms-->RES���� */
    RES_LOW();
    // vTaskDelay(200);
    while(i--);
    RES_HIGH();

    /* 4. oled����*/
    Inf_OLED1315_WR_Byte(0xAE, OLED_CMD); // �ر���ʾ

    Inf_OLED1315_WR_Byte(0x00, OLED_CMD); // ���õ��е�ַ
    Inf_OLED1315_WR_Byte(0x10, OLED_CMD); // ���ø��е�ַ
    Inf_OLED1315_WR_Byte(0x40, OLED_CMD); // ������ʼ�е�ַ��ӳ�� RAM ��ʾ��ʼ�У�(0x00~0x3F)
	  
    Inf_OLED1315_WR_Byte(0x81, OLED_CMD); // �Աȶȿ��ƼĴ����������Աȶ�����
    Inf_OLED1315_WR_Byte(0xCF, OLED_CMD); // �Աȶȣ����0xFF��
	  
    Inf_OLED1315_WR_Byte(0xA1, OLED_CMD); // ������ɨ�跽��0xa0: ���ҷ��ã�0xa1: ������
	  
    Inf_OLED1315_WR_Byte(0xC8, OLED_CMD); // ������ɨ�跽��0xc0: ���·��ã�0xc8: ������
	  
    Inf_OLED1315_WR_Byte(0xA6, OLED_CMD); // ����������ʾ
    Inf_OLED1315_WR_Byte(0xA8, OLED_CMD); // ���ö�·���ñȣ�1 �� 64��
    Inf_OLED1315_WR_Byte(0x3f, OLED_CMD); // ����ռ�ձȣ�1/64��
	  
    Inf_OLED1315_WR_Byte(0xD3, OLED_CMD); // ������ʾƫ�ƣ��ƶ�ӳ�� RAM ��������
    Inf_OLED1315_WR_Byte(0x00, OLED_CMD); // ��ƫ��
	  
    Inf_OLED1315_WR_Byte(0xd5, OLED_CMD); // ������ʾʱ�ӷ�Ƶ��/����Ƶ��
    Inf_OLED1315_WR_Byte(0x80, OLED_CMD); // ���÷�Ƶ�ȣ�����ʱ��Ϊ 100 ֡/��
	  
    Inf_OLED1315_WR_Byte(0xD9, OLED_CMD); // ����Ԥ������� �������OLED����Ӧ�ٶ�
    Inf_OLED1315_WR_Byte(0xF1, OLED_CMD); // ���15��ʱ������ �ŵ�1��ʱ������
	  
    Inf_OLED1315_WR_Byte(0xDA, OLED_CMD); // ���� COM ����Ӳ������
    Inf_OLED1315_WR_Byte(0x12, OLED_CMD); // ���� COM ����Ӳ������ֵ
	  
    Inf_OLED1315_WR_Byte(0xDB, OLED_CMD); // ���� VCOMH
    Inf_OLED1315_WR_Byte(0x40, OLED_CMD); // ���� VCOM ȡ����ѹˮƽ
	  
    Inf_OLED1315_WR_Byte(0x20, OLED_CMD); // �����ڴ��ַģʽ(ˮƽ0x00/��ֱ0x01/ҳ0x02)
    Inf_OLED1315_WR_Byte(0x02, OLED_CMD); // �ڴ��ַģʽ����Ϊ ҳ��ַģʽ
	  
    Inf_OLED1315_WR_Byte(0x8D, OLED_CMD); // ���õ�����(0x14���� 0x10 �ر�)
    Inf_OLED1315_WR_Byte(0x14, OLED_CMD); // �رյ�����
	  
    Inf_OLED1315_WR_Byte(0xA4, OLED_CMD); // �ر�������ʾ��0xa4/0xa5��
    Inf_OLED1315_WR_Byte(0xA6, OLED_CMD); // �رշ�����ʾ��0xa6/0xa7��
    Inf_OLED1315_Clear();                 // ����
    Inf_OLED1315_WR_Byte(0xAF, OLED_CMD); // �� OLED ���
}




