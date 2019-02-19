#include<stdint.h>
#define __IO volatile

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
#define ADC_SQR1    (*((volatile unsigned long *) 0x50040030))

#define TIM1        (*((volatile unsigned long *) 0x40012C00)) //timer 1
typedef struct{
    __IO uint32_t   MODER;  // mode register
    __IO uint16_t   OTYPER; // output type register
    uint16_t        rev0;   // padding two bytes
    __IO uint32_t   OSPEEDR;// output speed register
    __IO uint32_t   PUPDR;  // pull up pull down register
    __IO uint16_t   IDR;    // input data register
    uint16_t        rev1;   // padding two bytes
    __IO uint16_t   ODR;    // output data register
    uint16_t        rev2;   // padding two bytes
    __IO uint16_t   BSSRL;  // bit set/reset register low
    __IO uint16_t   BDDRH;  // bit set/reset register high
    __IO uint32_t   LCKR;   // configuration lock register
    __IO uint32_t   AFR[2]; // alternate function register
    __IO uint32_t   BRR;    // bit reset register
    __IO uint32_t   ASCR;   // analog switch control register
} GPIO_Typedef;
typedef struct {
	//__IO uint(bits) name comment
	__IO uint32_t CR[3];//control registers
	__IO uint32_t BRR;  // bit reset register
	__IO uint32_t GTPR; //guard time and prescalar register
	__IO uint32_t RTOR; // reciever timeout register
	__IO uint32_t RQR;  //request register
	__IO uint32_t ISR;  //interrupt and status register
	__IO uint32_t ICR ; //interrupt flag clear register
	__IO uint32_t RDR;  //recieve data register
	__IO uint32_t TDR;  //transmit data register
} USART_Typedef;
typedef struct {
	__IO uint32_t ISR; //interrupt and status register
	__IO uint32_t IER; // interrupt enable register
	__IO uint32_t CR; //control register
	__IO uint32_t CFGR[2]; //configuration register
	__IO uint32_t SMPR[2]; //sample time registers
	__IO uint32_t TR[3]; // watchdog threshold registers
	__IO uint32_t SQR[4]; //regular sequence registers
	__IO uint32_t DR; //regular data register
	__IO uint32_t JSQR; //injected sequence register
	__IO uint32_t OFRy; //offset y register
	__IO uint32_t JDRy; // injected channel y data register
	__IO uint32_t AWD2CR; // analog watchdog 2 configuration register
	__IO uint32_t AWD3CR; // analog watchdog 3 configuration register
	__IO uint32_t DIFSEL; // differential mode selection register
	__IO uint32_t CALFACT; //calibration factors
} ADC_Typedef;

void USART_Init(USART_Typedef * USARTx);





int main(){
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
	//clear adc-sqr1-l bits in adc1-sqr1 to select 1 conversion in reg channel
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






	int i;
	RCC_AHB2ENR |= (1 << 1); //Bit 1 is GPIOB clock enable bit
	GPIOB_MODER &= ~(3<<(2*3)); //clear PB3
	GPIOB_MODER |= 1<<(2*3); //PB3 output
	while(1) {
		GPIOB_BSRR |= LED_ON; //Turn on LED
		for(i=0; i<1000000; i++);
		GPIOB_BSRR |= LED_OFF; //Turn off LED
		for(i=0; i<1000000; i++);
		//for loop of 2000 is 20ms*
	}
}




void USART_Init(USART_Typedef * USARTx) {
	USARTx->CR[0] = 0x00000000; //set ue (usart enable to 0)
	//USARTx->CR[0] = 
}
