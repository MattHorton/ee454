#include<stdint.h>
#define __IO volatile
	
#define EXTI 	(*((volatile unsigned long *) 0x40010400))
	
#define EXTI0 (*(volatile unsigned long *) 0x0000005C)) 	// EXTI_line_0_interrupt
																													//position 6
//must set the bit representative of position in the enable so set bit 6 in iser
#define NVIC_ISER0 (*(volatile unsigned long *) 0xE000E100))
#define NVIC_ICER0 (*(volatile unsigned long *) 0XE000E180))
//p328 in reference manual PA0 -> EXT0


int main() {
	
	
	
}