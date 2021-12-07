#include "spi_dma.h"

uint8_t spi_buffer_tx[BUFFER_SPI_SIZE];
uint8_t spi_buffer_rx[BUFFER_SPI_SIZE];

extern uint8_t	Background_buf[480];
extern uint8_t	MainLine_buf[480];

void SPI1_Master_init(){
	GPIOA_init_spi_TFT();
	RCC->APB2ENR |= RCC_APB2ENR_SPI1EN;
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

void spi1_SendDataDMA_1Byte(uint8_t* data, uint16_t count_byte){
	while((SPI1->SR & SPI_SR_TXE) != SPI_SR_TXE);
	for (int i = 0; i < count_byte; i++)
		spi_buffer_tx[i] = data[i];
	DMA2_Stream3->CR &= ~DMA_SxCR_EN;
	DMA2_Stream3->M0AR =(uint32_t)(&spi_buffer_tx[0]);
	DMA2_Stream3->NDTR = count_byte;
	RESET_CS();
	Start_DMA_Send_Data();
}

void spi1_SendDataDMA_Column(uint8_t* data, uint16_t count_byte){
	DMA2_Stream3->CR &= ~DMA_SxCR_EN;
	DMA2_Stream3->M0AR = (uint32_t)(data);
	DMA2_Stream3->NDTR = count_byte;
	RESET_CS();
	Start_DMA_Send_Data();
}
