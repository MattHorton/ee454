void EXTI_Init(void) {
	//enable the syscfg clock
	//RCC->APB2ENR |= RCC_APB2ENR_SYSCFGEN RCC_APB2ENR;
	RCC->APB2ENR |= 1<<0;
	
	//select PA.3	as the trigger source of EXTI 3
	//SYSCFG->EXTICR[0] &= ~SYSCFG_EXTICR1_EXTI3;
	//SYSCFG->EXTICR[0] |=  SYSCFG_EXTICR1_EXTI3_PA;
	//SYSCFG->EXTICR[0] &= ~(0X000F);
	SYSCFG_EXTICR1 &= ~SYSCFG_EXTICR1_EXTI3;
	SYSCFG_EXTICR1 |=  SYSCFG_EXTICR1_EXTI3_PA;
	SYSCFG_EXTICR1 &= ~(0X000F);
	
	//enable rising edge trigger for exti 3
	//rising trigger selection register (RSTR)
	//0 = disabled; 1 = enabled
	//EXTI->RTSR |= EXTI_RTSR_RT3;
	EXTI_RTSR1 |= 1<<3;
	
	//disable falling edge trigger for exti3
	//falling trigger selection register(FSTR)
	//0 = disabled; 1 = enabled
	//EXTI->FTSR &= ~EXTI_FTSR_RT3;
	EXTI_FTSR1 &= ~(1<<3);
	
	//enable EXTI 3 interrupt
	//interrupt mask register: 0 = masked, 1 = unmasked
	//"masked" means that processor ignores the corresponding interrupt
	//EXTI->IMR1 |= EXTI_IMR1_IM3; // enable exti line 3
	EXTI_IMR1 |= 1<<3; // enable exti line 3
	
	//set exti 3 priority to 1
	//NVIC_SetPriority(EXTI3_IRQn, 1);
	
	//enable exti 3 interrupt
	//NVIC_EnableIRQ(EXTI3_IRQn);
}


void EXTI3_IRQHandler(void) {
	//check for exti 3 interrupt flag
	//if((EXTI->PR1 & EXTI_PR1_PIF3) == EXTI_PR1_PIF3) {
	
	if((EXTI->PR1 & EXTI_PR1_PIF3) == EXTI_PR1_PIF3) {
		//toggle led
		GPIOB->ODR ^= 1<<8;
		
		//clear interrupt pending request
		//EXTI->PR1 |= EXTI_PR1_PIF3; // write a 1 to clear
		EXTI->PR1 |= EXTI_PR1_PIF3; // write a 1 to clear
	}
}



