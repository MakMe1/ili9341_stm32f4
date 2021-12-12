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

void	GPIOA_TFT_config();
void	GPIOA_ADC_init();

#endif /* GPIO_H_ */
