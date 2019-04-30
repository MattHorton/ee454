//#include "../../headerFiles/horton_stm32l432.h"
#include "../../headerFiles/stm32l432.h"
void EXTI_Init(void);


int main() {
	//initialize
	//  adc
	//  pwm
	//  setpoint button PB0 w/ external interrupt
	//  I2C ?
	
	//enable gpiob in peripheral enable register
	RCC->AHB2ENR |= (1<<1); //bit 1 is gpiob clock enable
	
	//set mode
	GPIOB->MODER &= 0x1111111C; //set PB0 to input
	//poll for button press (external interrupt)
	

	
	
	return 0;
}

void EXTI_Init(void) {
	//enable the syscfg clock
	//RCC->APB2ENR |= /*RCC_APB2ENR_SYSCFGEN*/RCC_APB2ENR;
	RCC->APB2ENR |= 1<<1;
	
	//select PA.3	as the trigger source of EXTI 3
	SYSCFG->EXTICR[0] &= ~SYSCFG_EXTICR1_EXTI3;
	SYSCFG->EXTICR[0] |=  SYSCFG_EXTICR1_EXTI3_PA;
	SYSCFG->EXTICR[0] &= ~(0X000F);
	
	//enable rising edge trigger for exti 3
	//rising trigger selection register (RSTR)
	//0 = disabled; 1 = enabled
	EXTI->RTSR |= EXTI_RTSR_RT3;
	
	//disable falling edge trigger for exti3
	//falling trigger selection register(FSTR)
	//0 = disabled; 1 = enabled
	EXTI->FTSR &= ~EXTI_FTSR_RT3;
	
	//enable EXTI 3 interrupt
	//interrupt mask register: 0 = masked, 1 = unmasked
	//"masked" means that processor ignores the corresponding interrupt
	EXTI->IMR1 |= EXTI_IMR1_IM3; // enable exti line 3
	
	//set exti 3 priority to 1
	NVIC_SetPriority(EXTI3_IRQn, 1);
	
	//enable exti 3 interrupt
	NVIC_EnableIRQ(EXTI3_IRQn);
}

void EXTI3_IRQHandler(void) {
	//check for exti 3 interrupt flag
	if((EXTI->PR1 & EXTI_PR1_PIF3) == EXTI_PR1_PIF3) {
		//toggle led
		GPIOB->ODR ^= 1<<8;
		
		//clear interrupt pending request
		EXTI->PR1 |= EXTI_PR1_PIF3; // write a 1 to clear
	}
}