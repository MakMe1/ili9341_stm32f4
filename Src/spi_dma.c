#include "spi_dma.h"

uint8_t spi_buffer_tx[BUFFER_SPI_SIZE];
uint8_t spi_buffer_rx[BUFFER_SPI_SIZE];

//void DMA2_Stream3_IRQHandler(){
//	if (DMA2->LISR & DMA_LISR_TCIF3) {
//		DMA2->LIFCR |= DMA_LIFCR_CTCIF3;
//		status_dma_tx = 1;
//		TickDelay(3); // experimental value
//		SET_CS();
//	}
//}

//void dma_tx_init(){
//	RCC->AHB1ENR |= RCC_AHB1ENR_DMA2EN;
//	DMA2_Stream3->CR |= DMA_SxCR_DIR_0; // Memory-to-peripheral mode
//	DMA2_Stream3->M0AR = (uint32_t)(&spi_buffer_tx[0]);
//	DMA2_Stream3->PAR = (uint32_t)(&(SPI1->DR));
//	DMA2_Stream3->CR |= 0x3 << DMA_SxCR_CHSEL_Pos; // Channel 3 selected
//	DMA2_Stream3->CR |= DMA_SxCR_MINC; // Memory increment mode
//	DMA2_Stream3->CR |= DMA_SxCR_TCIE; // Transfer complete interrupt enable
//	DMA2_Stream3->CR |= DMA_SxCR_PL_1; //Priority level high
//	NVIC_SetPriority(DMA2_Stream3_IRQn, 3);
//	NVIC_EnableIRQ(DMA2_Stream3_IRQn);
//}


void spi1_master_init() {
	GPIOA_init_spi_TFT();
	RCC->APB2ENR |= RCC_APB2ENR_SPI1EN;
//	SPI1->CR1 &= ~SPI_CR1_SPE;
	SPI1->CR1 |= SPI_CR1_BR_0; // f/32
	SPI1->CR1 &= ~SPI_CR1_LSBFIRST; // MSB transmits first
	SPI1->CR1 &= ~SPI_CR1_CPOL; // SPI1 mode: 0
	SPI1->CR1 &= ~SPI_CR1_CPHA; //
	SPI1->CR1 |= SPI_CR1_MSTR;	// Master mode
	SPI1->CR1 |= SPI_CR1_SSM; // Software NSS control
	SPI1->CR1 |= SPI_CR1_SSI; // NSS in high level by default

	SPI1->CR1 &= ~SPI_CR1_DFF; // 8 bit format

	dma_tx_init();
	SPI1->CR2 |= SPI_CR2_TXDMAEN;

	SPI1->CR1 |= SPI_CR1_SPE;
}

void spi1_SendDataDMA_1Byte(uint8_t* data, uint16_t count_byte) {
	while((SPI1->SR & SPI_SR_TXE) != SPI_SR_TXE);
	status_dma_tx = 0;

	for (int i = 0; i < count_byte; i++) {
		spi_buffer_tx[i] = data[i];
	}

	DMA2_Stream3->CR &= ~DMA_SxCR_EN;
	DMA2_Stream3->NDTR = count_byte;
//	DMA2_Stream3->CR |= DMA_SxCR_MINC;	//increment memory
	RESET_CS();
	Start_DMA_Send_Data();
}
