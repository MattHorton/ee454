#define __IO volatile //allows read and write

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
} GPIO_Typedef

typedef struct{
    __IO uint32_t CR;//clock control register
    __IO uint32_t ICSCR;//internal clock sources calibration register
    __IO uint32_t CFGR;//clock configuration register

    __IO uint32_t AHB1ENR;
    __IO uint32_t AHB2ENR;
    __IO uint32_t AHB3ENR;
} RCC_Typedef;

#define RCC ((RCC_Typedef *) 0x40021000)

#define GPIOB ((GPIO_Typedef *) 0x4000400))
#define RCC_AHB2ENR_GPOIBEN (0x00000002)

RCC->AHB2ENR |= RCC_AHB2ENR_GPIOBEN; // enable clock to gpio port b


//actual code for 454 adc/pot to pwm/led
RCC->CR |= (1 << 8);//1 in eigth bit is turn on hsi
