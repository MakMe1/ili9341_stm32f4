#include "ADC.h"

void	ADC_init()
{
	GPIOA_ADC_init();
	RCC->APB2ENR |= RCC_APB2ENR_ADC1EN;

	ADC1->SQR1 &= ~ADC_SQR1_L;		// 1 channel mode
	ADC1->SQR3 &= ~ADC_SQR3_SQ1;	// channel 0 selected

//	ADC1->CR2 |= ADC_CR2_CONT;
	ADC1->CR1 |= ADC_CR1_DISCEN;

	ADC1->CR2 |= ADC_CR2_EXTEN_0; //0x1 - rising edge
	ADC1->CR2 |= ADC_CR2_EXTSEL_3; // TIM3_TRGO event

	ADC1->CR2 |= ADC_CR2_DDS;    /* DMA Enable */
	ADC1->CR2 |= ADC_CR2_DMA;

	ADC1->CR2 |= ADC_CR2_ADON;
	ADC1->CR2 |= ADC_CR2_SWSTART;
}


