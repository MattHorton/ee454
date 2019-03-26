#include "../headerFiles/horton_stm32l432.h"

#define RCC ((RCC_Typedef *) 0x40021000)
#define RCC_AHB1ENR_DMA1EN 0x00000001
#define DMA_CCR_MEM2MEM (unsigned long) (1 << 14)
	
#define DMA1 ((DMA_Typedef *) 0x40020000

int main() {
	
	//suppose adc runs three regular channels
	uint16_t ADC_Results[4]; // buffer for four ADC results
	//initialization of ADC,GPIO, and clocks (not shown here yet)
	
	
	//enable DMA1 clock
	RCC->AHB1ENR |= RCC_AHB1ENR_DMA1EN;
	
	// DMA1 channel 1 configuration for ADC1
	DMA1->CCR1 &= ~DMA_CCR_MEM2MEM; // disable memory to memory mode
	
	//dma1channel1 adc1 CxS[3:0] = 0000
	
	//dd
	return 0;
}