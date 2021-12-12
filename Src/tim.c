#include "tim.h"

/* configure TIM3 to control ADC frequency */
void TIM3_init_TRGO(){
	RCC->APB1ENR |= RCC_APB1ENR_TIM3EN;
	TIM3->CR1 |= TIM_CR1_ARPE;
	TIM3->PSC = PSC_val - 1;
	TIM3->ARR = ARR_val - 1;
	TIM3->CR2 |= TIM_CR2_MMS_1;
	TIM3->CR1 |= TIM_CR1_CEN;
}
