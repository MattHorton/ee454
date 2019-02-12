// 1. get value of separate phototransistors from adc channels
//    set up adc channels for phototransistor(s)

#define RCC_AHB2ENR (*((volatile unsigned long *) 0x4002104C)) //GPIO Enable Reg
#define GPIOB_MODER (*((volatile unsigned long *) 0x48000400)) //GPIO B Mode Reg
#define GPIOB_BSRR (*((volatile unsigned long *) 0x48000418)) //GPIO B Output Bit set/reset reg
#define LED_ON (1 << 3)
#define LED_OFF (1 << (3+16))

#define GPIOA_MODER (*((volatile unsigned long *) 0x48000000)) //GPIO A Mode Reg
#define GPIOA_PUPDR (*((volatile unsigned long *) 0x4800000C)) //GPIO pullup/down
#define GPIOA_AFRH 	(*((volatile unsigned long *) 0x48000024))	 //alternate function includeing PA8

#define MASTER_ADC1 (*((volatile unsigned long *) 0x50040000))
#define ADC_CR      (*((volatile unsigned long *) 0x50040008))
#define ADC_CCR     (*((volatile unsigned long *) 0x50040300))
#define	ADC_CFGR    (*((volatile unsigned long *) 0x5004000C))

#define TIM1        (*((volatile unsigned long *) 0x40012C00)) //timer 1


int main(){
	/*
	int i;
	RCC_AHB2ENR |= (1 << 1); //Bit 1 is GPIOB clock enable bit
	GPIOB_MODER &= ~(3<<(2*3)); //clear PB3
	GPIOB_MODER |= 1<<(2*3); //PB3 output
	GPIOB_BSRR |= LED_ON; //Turn on LED
	for(i=0; i<50; i++);
	GPIOB_BSRR |= LED_OFF; //Turn off LED
	for(i=0; i<1950; i++);
	//for loop of 2000 is 20ms*
	*/
			
	//initializing adc ports
	RCC_AHB2ENR |= (1 << 0); //Bit 0 is GPIOA clock enable bit
													 //gpio a 0 and 1 MODE are already initialized to analog
	//GPIOA_PUPDR &=~(1 << 1); //set bit 1 in ASCR to close analog switch
		
		
		
	//adc stuff
	RCC_AHB2ENR |= (1 << 13);//enable adc clock bit
	ADC_CR |= (0 << 0);			//adc enable = 0
	ADC_CR |= (0 << 29);     //disable deep pwd
	//ADC_CR |= (0 << 28);     //disable internal voltage regulator
	ADC_CCR|= (0 << 21);		 //set adc prescaler to 0 (clock not divided)
	ADC_CCR|= (0 << 20);
	ADC_CCR|= (0 << 19);
	ADC_CCR|= (0 << 18);
	ADC_CCR|= (0 << 17);		 //set to synchronous mode
	ADC_CCR|= (1 << 16);
	ADC_CR |= (1 << 28);		//enable internal voltage regulator
	ADC_CFGR|=(0 << 4);			//set resolution to 12 bits
	ADC_CFGR|=(0 << 3);
	ADC_CFGR|=(0 << 5);			//set data alignment to right
		
	//pwm stuff
	GPIOA_MODER |= (1 << 17);//set bit 17 to 1 16 to 0 for pwm gpio (PA8)
	GPIOA_MODER |= (1 << 16);//alternate function mode
	GPIOA_AFRH |= 0x00000001;//set to TIM1 (PWM)

}