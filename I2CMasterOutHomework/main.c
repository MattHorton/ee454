#include "horton_stm32l432.h"
#include<stdint.h>
#define __IO volatile

//standard mode 100kbit/s
typedef struct {
	__IO uint32_t CR; // control register
	__IO uint32_t ICSCR; // 
	__IO uint32_t CFGR;
	__IO uint32_t PLLCFGR;
	__IO uint32_t PLLSAI1CFGR;
	__IO uint32_t CIER; //clock interrupt enable
	__IO uint32_t CIFR; //clock interrupt flag register
	__IO uint32_t CICR; // clock interrupt clear register
	__IO uint32_t AHB1RSTR; // ahb1 peripheral reset register
	__IO uint32_t AHB2RSTR; // ahb2 peripheral reset register
	__IO uint32_t AHB3RSTR; // ahb3 peripheral reset register
	__IO uint32_t APB1RSTR1; // apb1 peripheral reset register 1
	__IO uint32_t APB1RSTR2; // apb1 peripheral reset register 2
	__IO uint32_t APB2RSTR; // apb2 peripheral reset register
	__IO uint32_t AHB1ENR; // ahb1 peripheral clock enable register
	__IO uint32_t AHB2ENR; // ahb2 peripheral clock enable register
	__IO uint32_t AHB3ENR; // ahb3 peripheral clock enable
	__IO uint32_t APB1ENR1; // apb1 peripheral clock enable 1
	__IO uint32_t APB1ENR2; // apb1 peripheral clock enable 2
	__IO uint32_t APB2ENR; // apb2 peripheral clock enable
	__IO uint32_t AHB1SMENR; // ahb1 peripheral clocks enable in sleep/stop modes
	__IO uint32_t AHB2SMENR; // ahb2 peripheral clocks enable in sleep/stop modes
	__IO uint32_t AHB3SMENR; // ahb3 peripheral clocks enable in sleep/stop modes
	__IO uint32_t APB1SMENR1; //apb1 peripheral clocks enable in sleep/stop modes 1
	__IO uint32_t APB1SMENR2; //apb1 peripheral clocks enable in sleep/stop modes 2
	__IO uint32_t APB2SMENR; // apb2 peripheral clocks enable in sleep/stop modes
	__IO uint32_t CCIPR; // peripherals independent clock configuration
	__IO uint32_t BDCR; //backup domain control register
	__IO uint32_t CSR; // control/status register
	__IO uint32_t CRRCR; // clock recovery RC register
	__IO uint32_t CCIPR2; // peripherals independent clock configuration register
}RCC_Typedef;

typedef struct {
	__IO uint32_t CR1; // control register 1
	__IO uint32_t CR2; // control register 2
	__IO uint32_t OAR1; // own address 1 register
	__IO uint32_t OAR2; // own address 2 register
	__IO uint32_t TIMINGR; // timing register
	__IO uint32_t TIMEOUTR; // timeout register
	__IO uint32_t ISR; // interrupt and status register
	__IO uint32_t ICR; // interrupt clear register
	__IO uint32_t PECR; // PEC register ? (packet error checking)
	__IO uint32_t RXDR; // recieve data register
	__IO uint32_t TXDR; // transmit data register
}I2C_Typedef;
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
	
	
	
	
	//notes
	//I2C1 scl PA9  this can be found in data sheet
	//I2C1 sda PA10
	return 0;
}

void I2C_Init(I2C_Typedef* I2Cx) {
	uint32_t OwnAddr = 0x52;
}