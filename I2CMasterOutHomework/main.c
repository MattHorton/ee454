#include "../headerFiles/horton_stm32l432.h"

#define READ_FROM_SLAVE 1
#define NULL 0

#define I2C_ISR_TXIS (unsigned long) (1 << 1)
#define I2C_TXDR_TXDATA 0x000000FF
#define I2C_ISR_TC (unsigned long) (1 << 6)
#define I2C_ISR_NACKF (unsigned long) (1 << 4)
#define I2C_ISR_RXNE (unsigned long) (1 << 2)
#define I2C_RXDR_RXDATA 0x000000FF



void I2C_clock_enable_clock_source();
void I2C_Init(I2C_Typedef* I2Cx);
void I2C_Start(I2C_Typedef* I2Cx, uint32_t DevAddress, uint8_t Size, uint8_t Direction);
void I2C_Stop(I2C_Typedef* I2Cx);
void I2C_WaitLineIdle(I2C_Typedef* I2Cx);
int8_t I2C_SendData(I2C_Typedef* I2Cx, uint8_t SlaveAddress, uint8_t* pData, uint8_t Size);
int8_t I2C_RecieveData(I2C_Typedef* I2Cx, uint8_t SlaveAddress, uint8_t* pData, uint8_t Size);

int main () {
	//standard mode 100kbit/s
	//6th line in SendData
	//while((I2Cx->ISR & I2C_ISR_TXIS) == 0) issue with using TXE and TXIS
	//I2C1 scl PA9  this can be found in data sheet
	//I2C1 sda PA10
	
	
	
	uint8_t Data_Recieve[6];
	uint8_t Data_Send[6];
	unsigned long int SlaveAddress;
	
	//System_Clock_Init();
	I2C_clock_enable_clock_source();
	
	//I2C_GPIO_init();
	//I2C_Initialization(I2C1);
	
	while(1) {
		SlaveAddress = 0x48 << 1; //A0 = 1001000 = 0x48
		Data_Send[0] = 0x00; // 00 = command to read temperature sensor
		I2C_SendData(I2C1, SlaveAddress, Data_Send, 1);
		I2C_RecieveData(I2C1, SlaveAddress, Data_Recieve, 1);
		for(i = 0; i < 50000; i++); //short software delay
	}
	
	
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
																	 I2C_CR2_STOP));
	
	if (Direction == READ_FROM_SLAVE)
		tmpreg |= I2C_CR2_RD_WRN; // read from slave
	else
		tmpreg &= ~I2C_CR2_RD_WRN; // write to slave
	
	tmpreg |= (uint32_t)(((uint32_t) DevAddress  & I2C_CR2_SADD) |
											(((uint32_t) Size << 16) & I2C_CR2_NBYTES));
	tmpreg |= I2C_CR2_START;
	I2Cx->CR2 = tmpreg;
}

void I2C_Stop(I2C_Typedef* I2Cx) {
	I2Cx->CR2 |= I2C_CR2_STOP;
	while((I2Cx->ISR & I2C_ISR_STOPF) == 0);
	I2Cx->ICR |= I2C_ICR_STOPCF;
}

void I2C_WaitLineIdle(I2C_Typedef* I2Cx) {
	while((I2Cx->ISR & I2C_ISR_BUSY) == I2C_ISR_BUSY);
}

int8_t I2C_SendData(I2C_Typedef* I2Cx, uint8_t SlaveAddress, uint8_t* pData, uint8_t Size) {
	int i;
	if(Size <= 0 || pData == NULL) return -1;
	//Wait until the line is idle
	I2C_WaitLineIdle(I2Cx);
	
	//the last argument: 0 = sending data to the slave
	I2C_Start(I2Cx, SlaveAddress, Size, 0);
	
	for(i = 0; i < Size; i++) {
		//TXIS bit is set by hardware when the txdr register is empty and the
		//data to be transmitted must be written in the TXDR register. It is
		//cleared when the next data to be sent is written in the TXDR register.
		//the TXIS flag is not set when a NACK is recieved
		while( (I2Cx->ISR & I2C_ISR_TXIS) == 0 );
		
		//TXIS is cleared by writing to the TXDR register
		I2Cx->TXDR = pData[i] & I2C_TXDR_TXDATA;
	}
	
	//wait until TC flag is set
	while((I2Cx->ISR & I2C_ISR_TC) == 0 && (I2Cx->ISR & I2C_ISR_NACKF) == 0 );
	
	if( (I2Cx->ISR & I2C_ISR_NACKF) != 0)
		return -1;
	
	I2C_Stop(I2Cx);
	
	return 0;
}

int8_t I2C_RecieveData(I2C_Typedef* I2Cx, uint8_t SlaveAddress, uint8_t* pData, uint8_t Size) {
	int i;
	
	if(Size <= 0 || pData == NULL) return -1;
	
	I2C_WaitLineIdle(I2Cx);
	
	I2C_Start(I2Cx, SlaveAddress, Size, 1); //1 = recieving from slave
	
	for(i = 0; i < Size; i++) {
		//wait until RXNE flag is set
		while( (I2Cx->ISR & I2C_ISR_RXNE) == 0);
		pData[i] = I2Cx->RXDR & I2C_RXDR_RXDATA;
	}
	
	while((I2Cx->ISR & I2C_ISR_TC) == 0); // wait until TCR flag is set
	
	I2C_Stop(I2Cx);
	return 0;
}