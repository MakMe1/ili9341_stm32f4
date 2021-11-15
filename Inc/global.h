#ifndef GLOBAL_H_
#define GLOBAL_H_


#include <stdint.h>
#include "stm32f4xx.h"
#include "gpio.h"
#include "spi_dma.h"
#include "tft.h"
#include "command.h"

extern uint8_t status_dma_tx;
extern uint16_t MAX_Y;
extern uint16_t MAX_X;
extern uint16_t Y_SIZE;
extern uint16_t X_SIZE;

#define FONT_Y 8
#define FONT_X 8

#define CS_POS		4
#define DC_POS		3
#define RESET_POS	2
#define LED_POS		1

#define RESET_ACTIVE()	GPIOA->BSRR |= 0x1 << RESET_POS
#define RESET_IDLE()	GPIOA->BSRR |= 0x1 << (RESET_POS + 16)

#define DC_COMMAND()	GPIOA->BSRR |= 0x1 << (DC_POS + 16)
#define DC_DATA()		GPIOA->BSRR |= 0x1 << DC_POS

#define SET_CS()		GPIOA->BSRR |= 0x1 << CS_POS
#define RESET_CS()		GPIOA->BSRR |= 0x1 << (CS_POS + 16)

#define LED_on()		GPIOA->BSRR |= 0x1 << LED_POS
#define LED_off()		GPIOA->BSRR |= 0x1 << (LED_POS + 16)

#define BLACK 0x0000
#define GREEN 0x001F
#define RED 0xB800
#define BLUE 0x07E0
//#define GREEN 0x07E0
//#define RED 0xF800
//#define BLUE 0x001F
#define CYAN 0x07FF
#define MAGENTA 0xF81F
#define YELLOW 0xFFE0
#define WHITE 0xFFFF

//(blue & 0x1F) | ((green & 0x3F) << 5) | ((red & 0x1F) << 11)


#define Start_DMA_Send_Data()	DMA2_Stream3->CR |= DMA_SxCR_EN
#define Stop_DMA_Send_Data()	DMA2_Stream3->CR &= ~DMA_SxCR_EN


#endif /* GLOBAL_H_ */