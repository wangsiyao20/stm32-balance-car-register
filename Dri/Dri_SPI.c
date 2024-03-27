#include "Dri_SPI.h"

/**
	初始化SPI
*/
void Dri_SPI_Init(void) {
	
	// 开启GOIO时钟  PA
	RCC->APB2ENR |= RCC_APB2ENR_IOPAEN;
	
	// 初始化引脚：CS(PA4)、CLK(PA5)、MOSI(PA7) 使用推挽输出 MODE=11   CNF=00
	GPIOA->CRL |= (GPIO_CRL_MODE4|GPIO_CRL_MODE5 | GPIO_CRL_MODE7);
	GPIOA->CRL &= ~(GPIO_CRL_CNF4|GPIO_CRL_CNF5 | GPIO_CRL_CNF7);
	
	// 片选默认不选中
	CS_HIGH();
}

/**
	SPI写字节
*/
void Dri_SPI_WriteByte(uint8_t byte){
	
	// 选中
	CS_LOW();
	
	for(uint8_t i=0; i<8; i++) {
		
		SCK_LOW();
		// 把数据输出到MOSI，先从高位发送
		if( byte & 0x80 ) {
			MOSI_HIGH();
		} else {
			MOSI_LOW();
		}
		
		// 拉高时钟（第一个跳变）
		SCK_HIGH();
		// 移位
		byte <<= 1;
	}
	
	// 不选中
	CS_HIGH();
	
}


