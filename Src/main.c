#include "global.h"


uint8_t status_dma_tx=1;

uint8_t data_for_spi[8] = {0,0,0,0,0,0,0,0};
uint8_t data_for_spi_ready = 0;
uint8_t size_data_spi = 0;
uint16_t MAX_X=320;
uint16_t MAX_Y=240;

int main(void){
//	uint8_t data[128];
//	for(int i = 0; i< 128; i++)
//		data[i] = i;
//	spi1_master_init();
//	spi1_SendDataDMA_1Byte(&data[0], 128);
	TFT_init();
	while (1)
	{

	}
}

