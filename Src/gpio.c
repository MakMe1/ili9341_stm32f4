#include "gpio.h"

void	GPIOA_ADC_init()
{
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;
	GPIOA->MODER |= GPIO_MODER_MODER0;
}

void GPIOA_init_spi_TFT(){
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;
	//PA5 - SCK, PA7 - MOSI , PA6 - MISO
	GPIOA->MODER |= GPIO_MODER_MODER5_1 | GPIO_MODER_MODER6_1| GPIO_MODER_MODER7_1;
	GPIOA->AFR[0] |= 0x555 << 20; // AF5 for PA5-7
	//for CS and other
	GPIOA->MODER |= GPIO_MODER_MODER4_0|GPIO_MODER_MODER3_0|GPIO_MODER_MODER2_0|GPIO_MODER_MODER1_0;
	SET_CS();
}
