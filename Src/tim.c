#include "tim.h"

void TIM3_init_TRGO()
{
	RCC->APB1ENR |= RCC_APB1ENR_TIM3EN;
	TIM3->CR1 |= TIM_CR1_ARPE;
	TIM3->PSC = 8000 - 1;
	TIM3->ARR = 10 - 1;

	TIM3->CR2 |= TIM_CR2_MMS_1;

//	ADC1-> SR |= ADC_SR_EOC; //End of conversion interrupt enable
//	NVIC_SetPriority(ADC1_IRQn, 3); // setting the priority
//	NVIC_EnableIRQ(ADC1_IRQn);///Enabling an interrupt in the interrupt controller

	TIM3->CR1 |= TIM_CR1_CEN;
}
