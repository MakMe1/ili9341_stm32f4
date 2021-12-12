#ifndef GLOBAL_H_
#define GLOBAL_H_

#include "stm32f4xx.h"
#include "gpio.h"
#include "spi_dma.h"
#include "tft.h"
#include "command.h"
#include "tim.h"
#include "ADC.h"
#include "CoreClockConfig.h"
#include "DMA.h"
#include "library.h"

#define SCK_168

// define some colors
#define	BLACK	0x0000
#define	WHITE	0xFFFF
#define	GREEN	0x07E0
#define	RED		0x001F
#define	BLUE	0xB800
#define	CYAN	0xFFE0
#define	MAGENTA	0xF81F
#define	YELLOW	0x07FF

#define BackgroundColor	BLACK // color of background
#define MainLineColor	GREEN // color of main line
#define VoltageColor	WHITE // color of voltage levels

#define	Y_SIZE	240 // Y definition of display
#define X_SIZE	320 // X definition of display

#define FONT_Y	8 // Y definition of letters
#define FONT_X	8 // X definition of letters

#define	PSC_val	300
#define	ARR_val	350
#define STEP	200 // measurements per frame

#define FRAME_BUF_SIZE	640 // size of frame buffer

// pins positions
#define LED_POS		1
#define RESET_POS	2
#define DC_POS		3
#define CS_POS		4

#define RESET_ACTIVE()	GPIOA->BSRR |= 0x1 << (RESET_POS +16)
#define RESET_IDLE()	GPIOA->BSRR |= 0x1 << RESET_POS

#define DC_COMMAND()	GPIOA->BSRR |= 0x1 << (DC_POS + 16)
#define DC_DATA()		GPIOA->BSRR |= 0x1 << DC_POS

#define SET_CS()		GPIOA->BSRR |= 0x1 << CS_POS
#define RESET_CS()		GPIOA->BSRR |= 0x1 << (CS_POS + 16)

#define LED_on()		GPIOA->BSRR |= 0x1 << LED_POS
#define LED_off()		GPIOA->BSRR |= 0x1 << (LED_POS + 16)

#define Start_DMA_Send_Data()	DMA2_Stream3->CR |= DMA_SxCR_EN
#define Stop_DMA_Send_Data()	DMA2_Stream3->CR &= ~DMA_SxCR_EN


#endif /* GLOBAL_H_ */
