//actual code for 454 adc/pot to pwm/led
RCC->CR |= (1 << 8);//1 in eigth bit is turn on hsi
