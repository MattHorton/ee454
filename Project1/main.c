// 1. get value of separate phototransistors from adc channels
//    set up adc channels for phototransistor(s)
#define RCC_AHB2ENR (*((volatile unsigned long *) 0x4002104C)) //GPIO Enable Reg
#define GPIOB_MODER (*((volatile unsigned long *) 0x48000400)) //GPIO B Mode Reg
#define GPIOB_BSRR (*((volatile unsigned long *) 0x48000418)) //GPIO B Output Bit set/reset reg
#define LED_ON (1 << 3)
#define LED_OFF (1 << (3+16))

int main(){
int i;

RCC_AHB2ENR |= (1 << 1); //Bit 1 is GPIOB clock enable bit
GPIOB_MODER &= ~(3<<(2*3)); //clear PB3
GPIOB_MODER |= 1<<(2*3); //PB3 output
while(1){
GPIOB_BSRR |= LED_ON; //Turn on LED
for(i=0; i<100000000; i++);
GPIOB_BSRR |= LED_OFF; //Turn off LED
for(i=0; i<100000000; i++);
}
}