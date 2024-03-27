#include "Dri_SPI.h"

/**
	��ʼ��SPI
*/
void Dri_SPI_Init(void) {
	
	// ����GOIOʱ��  PA
	RCC->APB2ENR |= RCC_APB2ENR_IOPAEN;
	
	// ��ʼ�����ţ�CS(PA4)��CLK(PA5)��MOSI(PA7) ʹ��������� MODE=11   CNF=00
	GPIOA->CRL |= (GPIO_CRL_MODE4|GPIO_CRL_MODE5 | GPIO_CRL_MODE7);
	GPIOA->CRL &= ~(GPIO_CRL_CNF4|GPIO_CRL_CNF5 | GPIO_CRL_CNF7);
	
	// ƬѡĬ�ϲ�ѡ��
	CS_HIGH();
}

/**
	SPIд�ֽ�
*/
void Dri_SPI_WriteByte(uint8_t byte){
	
	// ѡ��
	CS_LOW();
	
	for(uint8_t i=0; i<8; i++) {
		
		SCK_LOW();
		// �����������MOSI���ȴӸ�λ����
		if( byte & 0x80 ) {
			MOSI_HIGH();
		} else {
			MOSI_LOW();
		}
		
		// ����ʱ�ӣ���һ�����䣩
		SCK_HIGH();
		// ��λ
		byte <<= 1;
	}
	
	// ��ѡ��
	CS_HIGH();
	
}


