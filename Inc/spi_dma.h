#ifndef SPI_DMA_H_
#define SPI_DMA_H_


#include "global.h"

#define BUFFER_SPI_SIZE 128

void SPI1_Master_init();
void dma_tx_init();
void spi1_SendDataDMA_1Byte(uint8_t* data, uint16_t count_byte);

void spi1_SendDataDMA_Column(uint8_t* data, uint16_t count_byte);


#endif /* SPI_DMA_H_ */
