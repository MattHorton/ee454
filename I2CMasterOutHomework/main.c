#include "../headerFiles/horton_stm32l432.h"


#define RCC ((RCC_Typedef *) 0x40021000)

//bitmasks (should probably be migrated to header)
#define RCC_CCIPR_I2C1SEL 0x00003000 // bits 12 & 13
#define RCC_CCIPR_I2C1SEL_01 0x00001000 //bit 12
#define RCC_APB1ENR1_I2C1EN (unsigned long)(1<<21)
#define RCC_APB1RSTR1_I2C1RST (unsigned long)(1<<21)
#define I2C_CR1_PE (unsigned long)(1<<0) // peripheral enable
#define I2C_CR1_ANFOFF (unsigned long)(1<<12) //analog noise filter off
#define I2C_CR1_DNF 0x00000F00 //bits 8-ll digital noise filter
#define I2C_CR1_ERRIE (unsigned long)(1<<7) // error interrupts enable
#define I2C_CR1_SMBus (unsigned long)(1<<20) //SMBus host address enable
#define	I2C_CR1_NOSTRETCH (unsigned long)(1<17) //clock stretching disbabled
#define I2C_TIMINGR_PRESC 0xF0000000 //timing prescaler (bits 31-28)
#define I2C_OAR1_OA1EN (unsigned long)(1 << 15) // own address 1 enable
#define I2C_CR2_AUTOEND (unsigned long)(1 << 25) // auto end
#define I2C_OAR2_OA2EN (unsigned long)(1 << 15) // own address 2 enable
#define I2C_CR2_ADD10 (unsigned long)(1 << 11) // 0:7-bit addressing mode 1:10-bit addressing mode
#define I2C_CR2_NACK (unsigned long) (1 << 15) // 0:ACK is sent after current recieved byte 1:NACK is sent after current recieved byte
	

#define I2C_CR2_SADD (unsigned long)
#define I2C_CR2_NBYTES (unsigned long)
#define I2C_CR2_RELOAD (unsigned long)
#define I2C_CR2_RD_WRN (unsigned long)
#define I2C_CR2_START (unsigned long)
#define I2C_CR2_STOP (unsigned long)

void I2C_clock_enable_clock_source();
void I2C_Init(I2C_Typedef* I2Cx);
void I2C_Start(I2C_Typedef* I2Cx, uint32_t DevAddress, uint8_t Size, uint8_t Direction);


int main () {

	
	
	//standard mode 100kbit/s
	
	
	
	//notes
	//I2C1 scl PA9  this can be found in data sheet
	//I2C1 sda PA10
	return 0;
}


void I2C_clock_enable_clock_source() {
	//select SYSCLK as source to drive I2C1
	RCC->APB1ENR1 |= RCC_APB1ENR1_I2C1EN;
	
	//00 = PCLK, 01 = sysclk, 10 = hsi16, 11 = reserved
	RCC->CCIPR &= ~(RCC_CCIPR_I2C1SEL); // clear bits
	RCC->CCIPR |= RCC_CCIPR_I2C1SEL_01; // select SYSCLK
	
	RCC->APB1RSTR1 |= RCC_APB1RSTR1_I2C1RST; // 1 = reset I2C1
	RCC->APB1RSTR1 &= ~RCC_APB1RSTR1_I2C1RST;// complete the reset
}

void I2C_Init(I2C_Typedef* I2Cx) {
	uint32_t OwnAddr = 0x52;
	I2C_clock_enable_clock_source();
	
	// I2C CR1 configuration
	// when the I2c is disabled (PE = 0), the I2C performs a software reset
	I2Cx->CR1 &= ~I2C_CR1_PE; //disable I2C
	I2Cx->CR1 &= ~I2C_CR1_ANFOFF; // 0: analog noise filter enabled
	I2Cx->CR1 &= ~I2C_CR1_DNF; // 0000: digital noise filter disabled
	I2Cx->CR1 |= I2C_CR1_ERRIE; // errors interrupt enable
	I2Cx->CR1 &= ~I2C_CR1_SMBus; // SMBus Mode : 0 = I2C mode; 1 = SMBus mode
	I2Cx->CR1 &= ~I2C_CR1_NOSTRETCH; // enable clock stretching
	
	// I2C TIMINGR configuration
	I2Cx->TIMINGR = 0;
	//SysTimer = 80MHz, PRESC = 7, 80MHz/(1+7) = 10MHz
	I2Cx->TIMINGR &= ~I2C_TIMINGR_PRESC; // clear the prescaler
	I2Cx->TIMINGR |= 7U << 28; // set clock prescalar to 7
	I2Cx->TIMINGR |= 49U; // scll : scl low period (master mode) > 4.7us
	I2Cx->TIMINGR |= 49U << 8; //sclh : scl high period (master mode) > 4.0us
	I2Cx->TIMINGR |= 14U << 20; // scldel : scl data setup time > 1.0us
	I2Cx->TIMINGR |= 15U << 16; // sdadel : data hold time > 1.25us
	
	// I2C Own address 1 register (I2C_OAR1)
	I2Cx->OAR1 &= ~I2C_OAR1_OA1EN;;
	I2Cx->OAR1 |= I2C_OAR1_OA1EN | OwnAddr; //7-bit own address
	I2Cx->OAR1 &= ~I2C_OAR2_OA2EN; // disable own address 2
	
	//I2C cr2 configuration
	I2Cx->CR2 &= I2C_CR2_ADD10; // 0 7 bit 1 10 bit
	I2Cx->CR2 |= I2C_CR2_AUTOEND; // enable the auto end
	I2Cx->CR2 |= I2C_CR2_NACK; // for slave mode : set NACK
	I2Cx->CR2 |= I2C_CR1_PE; // enable I2C1
}

void I2C_Start(I2C_Typedef* I2Cx, uint32_t DevAddress, uint8_t Size, uint8_t Direction) {
	// direction = 0: master requests a write transfer
	// direction = 1: master requests a read transfer
	
	uint32_t tmpreg = I2Cx->CR2;
	tmpreg &= (uint32_t)~((uint32_t)(I2C_CR2_SADD   | I2C_CR2_NBYTES  |
																	 I2C_CR2_RELOAD | I2C_CR2_AUTOEND |
																	 I2C_CR2_RD_WRN | I2C_CR2_START   |
																	 I2C_CR2_STOP))
	
	if (Direction == READ_FROM_SLAVE)
		tmpreg |= I2C_CR2_RD_WRN; // read from slave
	else
		tmpreg &= ~I2C_CR2_RD_WRN; // write to slave
	
	tmpreg |= (uint32_t)(((uint32_t) DevAddress  & I2C_CR2_SADD) |
											(((uint32_t) Size << 16) & I2C_CR2_NBYTES));
	tmpreg |= I2C_CR2_START;
	I2Cx->CR2 = tmpreg;
}