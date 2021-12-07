#include "DMA.h"

extern uint8_t	spi_buffer_tx[STEP];
extern uint8_t	spi_buffer_rx[STEP];

extern uint8_t	Background_buf[480];
extern uint8_t	MainLine_buf[480];

int16_t data_adc_buff[2][STEP];

uint8_t	ADC_BUFFER_STATUS = 0;
uint8_t DMA_STATUS_FLAG = 0;

void DMA2_Stream0_IRQHandler(){
	DMA2->LIFCR |= DMA_LIFCR_CTCIF0;
	ADC_BUFFER_STATUS = BUF_IS_FULL;
}

void DMA2_Stream3_IRQHandler(){
	if (DMA2->LISR & DMA_LISR_TCIF3) {
		DMA2->LIFCR |= DMA_LIFCR_CTCIF3;
		DMA_STATUS_FLAG = 1;
		TickDelay(3); // experimental value
		SET_CS();
	}
}

void DMA2_init() {
	ADC_BUFFER_STATUS = BUF_IN_PROCESS;
	RCC->AHB1ENR |= RCC_AHB1ENR_DMA2EN;
	DMA2_Stream0->CR |= DMA_SxCR_DBM;	// Double buffer node
	DMA2_Stream0->PAR = (uint32_t)(&ADC1->DR);
	DMA2_Stream0->M0AR = (uint32_t)(&data_adc_buff[0][0]);
	DMA2_Stream0->M1AR = (uint32_t)(&data_adc_buff[1][0]);
	DMA2_Stream0->NDTR = STEP;
	DMA2_Stream0->CR |= DMA_SxCR_TCIE;
	DMA2_Stream0->CR |= DMA_SxCR_MINC;
	DMA2_Stream0->CR |= DMA_SxCR_MSIZE_0;
	DMA2_Stream0->CR |= DMA_SxCR_PSIZE_0;
	DMA2_Stream0->CR |= DMA_SxCR_EN;
	NVIC_SetPriority(DMA2_Stream0_IRQn, 3);
	NVIC_EnableIRQ(DMA2_Stream0_IRQn);
}

void dma_tx_init(){
	RCC->AHB1ENR |= RCC_AHB1ENR_DMA2EN;
	DMA2_Stream3->CR |= DMA_SxCR_DIR_0; // Memory-to-peripheral mode
	DMA2_Stream3->M0AR = (uint32_t)(&spi_buffer_tx[0]);
	DMA2_Stream3->PAR = (uint32_t)(&(SPI1->DR));
	DMA2_Stream3->CR |= 0x3 << DMA_SxCR_CHSEL_Pos; // Channel 3 selected
	DMA2_Stream3->CR |= DMA_SxCR_MINC; // Memory increment mode
	DMA2_Stream3->CR |= DMA_SxCR_TCIE; // Transfer complete interrupt enable
	DMA2_Stream3->CR |= DMA_SxCR_PL_1; //Priority level high
	NVIC_SetPriority(DMA2_Stream3_IRQn, 3);
	NVIC_EnableIRQ(DMA2_Stream3_IRQn);
}
