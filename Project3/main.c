#include "../../headerFiles/horton_stm32l432.h"

int main() {
	//initialize
	//  adc
	//  pwm
	//  setpoint button PB0
	//  I2C ?
	
	//enable gpiob in peripheral enable register
	RCC->AHB2ENR |= (1<<1); //bit 1 is gpiob clock enable
	
	//set mode
	GPIOB->MODER &= 0x1111111C; //set PB0 to input
	
	
	
	return 0;
}