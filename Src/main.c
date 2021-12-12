#include "global.h"


uint16_t data_adc;

int main(void){
	SetSysClk_168MHz();
	TFT_init();

	Greetings();
	tft_color_Background();

	TIM3_init_TRGO();
	ADC_init();
	DMA2_init();
	while (1){
		tft_ADCtoFrame();
	}
}
