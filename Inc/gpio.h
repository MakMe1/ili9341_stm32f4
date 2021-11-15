#ifndef GPIO_H_
#define GPIO_H_


#include "global.h"

/**************************** FOR TFT DISPLAY
 * PA4 - CS
 * PA2 - RESET
 * PA3 - DC (data command)
 * PA7 - MOSI
 * PA5 - SCK
 * PA6 - MISO
 * PA1 - LED
 * ***************************************
 * **/


void GPIOC_Set_Reset_PIN(int pin_num, int pin_state);
void GPIOC_DIODES_Init();
void GPIOA_init_spi_TFT();

#endif /* GPIO_H_ */
