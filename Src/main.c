#include "global.h"
#include <math.h>

uint8_t status_dma_tx=1;


uint16_t data_adc;

int main(void){
	SetSysClk_168MHz();

	TFT_init();

	tft_color_ALL(BLACK);

	TIM3_init_TRGO();
	ADC_init();
	DMA2_init();

	uint8_t tmp = 0;
	while (1)
	{

//		tft_color_ALL(GREEN);
//		tft_color_ALL(RED);
//		tft_color_ALL(BLUE);
//		tft_color_ALL(CYAN);
//		tft_color_ALL(MAGENTA);
//		tft_color_ALL(YELLOW);
//		tft_color_ALL(WHITE);
//		tft_color_ALL(BLACK);

		ADC_Get_Data();
	}
}

// ------- Rainbow -------
//tft_color_ALL(GREEN);
//tft_color_ALL(RED);
//tft_color_ALL(BLUE);
//tft_color_ALL(CYAN);
//tft_color_ALL(MAGENTA);
//tft_color_ALL(YELLOW);
//tft_color_ALL(WHITE);
//tft_color_ALL(BLACK);


// ------- Triangle -------
//	uint8_t triangle_buf[8];
//	triangle_buf[7] = 0x01;
//	triangle_buf[6] = 0x03;
//	triangle_buf[5] = 0x07;
//	triangle_buf[4] = 0x0F;
//	triangle_buf[3] = 0x1F;
//	triangle_buf[2] = 0x3F;
//	triangle_buf[1] = 0x7F;
//	triangle_buf[0] = 0xFF;
//	WriteToFlash(&triangle_buf[0], 9600);



// ------- SIN -------
//uint8_t val[640];
//	int prev;
//	for (int i = 0; i < 640; i++)
//	{
//		val[i] = 120 - (int)(120 * sin((double)i/8/3.14));
//	}
//	prev = val[0];

//	for (int i = 0; i < 320; i++)
//	{
//		tft_color_XY(i, prev, i, val[i], GREEN);
//		prev = val[i];
//	}
//	for (int k = 0; k < 320; k++)
//	{
//		tft_color_ALL(BLACK);
//		prev = val[k];
//		for (int i = 0; i < 320; i++)
//		{
//			tft_color_XY(i, prev, i, val[i+k], GREEN);
//			prev = val[i+k];
//		}
////		tft_write_cmd(0x01,1,1);
////		tft_color_ALL(BLACK);
////		for (int i = 0; i < 10; i++);
//		prev = val[k];
//		for (int i = 0; i < 320; i++)
//		{
//			tft_color_XY(i, prev, i, val[i+k], BLACK);
//			prev = val[i+k];
//		}
//	}
