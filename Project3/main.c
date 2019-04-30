//#include "../../headerFiles/horton_stm32l432.h"
#include "../../headerFiles/stm32l432.h"
void EXTI_Init(void);
int Result;

int main() {
	//initialize
	//  pa0 pushbutton set target
	//  adc
	//  pwm
	
	
	
	
	//pushbutton stuff from page 363
	RCC_AHB2ENR |= RCC_AHB2ENR_GPIOAEN;
	RCC_AHB2ENR |= RCC_AHB2ENR_GPIOBEN;
	//set mode of pin 0 as general purpose input
	GPIOA->MODER &= ~3UL; // set mode as input 00
	//set IO as no pull up no pull down
	GPIOA->PUPDR &= ~3UL; // pull up pull down mask
	GPIOB_MODER |= 0x000000D0; // general purpose output on pb2
	//pb2 push pull by default
	//pb2 no pull up pull down by default
	while(1) {
		//toggle red led when button PA0 is pushed
		if((GPIOA->IDR & 0x1) == 0x1) {
			GPIOB->ODR ^= GPIO_ODR_ODR_2;
			while((GPIOA->IDR & 0x1) != 0x00); // wait until button is released
		}
	}

	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	//adc stuff
	RCC_AHB2ENR |= (1 << 0); //Bit 0 is GPIOA clock enable bit
	//gpio a 0 and 1 MODE are already initialized to analog
	//GPIOA_PUPDR &=~(1 << 1); //set bit 1 in ASCR to close analog switch
	RCC_AHB2ENR |= (1 << 13); //enable adc clock bit
	ADC_CR |= (0 << 0);			  //ADEN
	ADC_CR |= (0 << 29);      //disable deep pwd
	//ADC_CR |= (0 << 28);    //disable internal voltage regulator ADVREGEN
	ADC_CCR|= (0 << 21);		  //set adc prescaler to 0 (clock not divided)
	ADC_CCR|= (0 << 20);
	ADC_CCR|= (0 << 19);
	ADC_CCR|= (0 << 18);
	ADC_CCR|= (0 << 17);		  //set to synchronous mode ADC_CCR_CKMODE
	ADC_CCR|= (1 << 16);
	ADC_CR |= (1 << 28);		  //enable internal voltage regulator
	ADC_CFGR|=(0 << 4);			  //set resolution to 12 bits
	ADC_CFGR|=(0 << 3);
	ADC_CFGR|=(0 << 5);			  //set data alignment to right
	ADC->SQR[0] =  //clear adc-sqr1-l bits in adc1-sqr1 to select 1 conversion in reg channel
	//specify channel number 6 as 1st conversion in reg seq adc1-sqr1
	//configure channel 6 as single ended adc-difsel
	//select adc sample time adc1-smpr1
	//select adc as discontinuous mode by clearing adc-cfgr-cont bits in adc-cfgr
	//clear adc-cfgr-ext bits in register adc-cfgr to select software trigger
	//enable adc1 by setting adc-cr-aden bit in adc-cr
	//wait until adc is ready adc-isr-adrdy bit in adc-isr

	//start one conversion by setting adc-cr-adstart bit in adc-cr
	//wait for conpletion of adc conversion by checking adc-csr-eoc-mst in adc-common-csr
	//conversion result is saved in adc-dr
	
	
	//----------------------------------------------------------------------
	//pwm stuff
	GPIOA_MODER |= (1 << 17);//set bit 17 to 1 16 to 0 for pwm gpio (PA8)
	GPIOA_MODER |= (1 << 16);//alternate function mode
	GPIOA_AFRH |= 0x00000001;//set to TIM1 (PWM)
	//enable clock of gpio
	//enable clock of timer 1

	//set gpio mode as alternate function
	//set the pin as pugh pull mode with no pull up pull down

	//select counting direction as up counting
	//program prescalar tim1->psc
	//program auto-reload value tim1->arr
	//select pwm mode 1 on channel 1 tim1->ccmr1
	//enable tim1_arr register preload tim1-cr1
	//enable all outputs moe bit in tim1-bdtr
	//enable complementary output of channel 1 ccn1 bit tim1-ccer
	//clear polarity bit to 0 cc1np in tim1-ccmr1
	//tim1-ccr1 = 500 duty cycle of oc1n = 50%
	//enable the counter of channel 1 cen bit in tim1-cr1
	
	
	
	return 0;
}




void ADC1_Wakeup(void) {
	int wait_time;
	ADC1_CR &= ~ADC_CR_DEEPPWD; //turn deep pwd off (bit 29)
	ADC1_CR |= ADC_CR_DEEPPWD; // turn deep pwd on
	if((ADC1_CR & (unsigned long)(1 << 29)) == (unsigned long)(1 << 29)) {
		ADC1_CR &= ~ADC_CR_DEEPPWD;
	}
	
	ADC1_CR |= ADC_CR_ADVREGEN; // enable adc internal voltage regulator
	wait_time = 20 * (80000000 / 1000000);
	while(wait_time != 0) {
		wait_time--;
	}
}

void ADC1_2_IRQHandler(void) {
		if((ADC1_ISR & ADC_ISR_EOC) == ADC_ISR_EOC) {
			Result = ADC1_DR;
		}
}