#include "../horton_stm32l432.h"


#define RCC ((RCC_Typedef *) 0x40021000)
#define RCC_CCIPR_I2C1SEL 0x00003000 // bits 12 & 13
#define RCC_CCIPR_I2C1SEL_01 0x00001000 //bit 12
#define RCC_APB1ENR1_I2C1EN (unsigned long)(1<<21)
#define RCC_APB1RSTR1_I2C1RST (unsigned long)(1<<21)

int main () {
	//select SYSCLK as source to drive I2C1
	RCC->APB1ENR1 |= RCC_APB1ENR1_I2C1EN;
	
	//00 = PCLK, 01 = sysclk, 10 = hsi16, 11 = reserved
	RCC->CCIPR &= ~(RCC_CCIPR_I2C1SEL); // clear bits
	RCC->CCIPR |= RCC_CCIPR_I2C1SEL_01; // select SYSCLK
	
	RCC->APB1RSTR1 |= RCC_APB1RSTR1_I2C1RST; // 1 = reset I2C1
	RCC->APB1RSTR1 &= ~RCC_APB1RSTR1_I2C1RST;// complete the reset
	
	
	//standard mode 100kbit/s
	
	
	
	//notes
	//I2C1 scl PA9  this can be found in data sheet
	//I2C1 sda PA10
	return 0;
}

void I2C_Init(I2C_Typedef* I2Cx) {
	uint32_t OwnAddr = 0x52;
}
