#include "global.h"


uint8_t status_dma_tx=1;

uint8_t data_for_spi[8] = {0,0,0,0,0,0,0,0};
uint8_t data_for_spi_ready = 0;
uint8_t size_data_spi = 0;
uint16_t MAX_X=320;
uint16_t MAX_Y=240;




/* PLL_VCO = (HSE_VALUE or HSI_VALUE / PLL_M) * PLL_N */
#define PLL_M      16
#define PLL_N      336

/* SYSCLK = PLL_VCO / PLL_P */
#define PLL_P      2

/* USB OTG FS, SDIO and RNG Clock =  PLL_VCO / PLLQ */
#define PLL_Q      8

void SetSysClock_168MHz(void);
void set_sysclk_to_84(void);

int main(void){
//	SetSysClock_168MHz();
	set_sysclk_to_84();
//	EraseSector();
	uint8_t triangle_buf[8];
	triangle_buf[7] = 0x01;
	triangle_buf[6] = 0x03;
	triangle_buf[5] = 0x07;
	triangle_buf[4] = 0x0F;
	triangle_buf[3] = 0x1F;
	triangle_buf[2] = 0x3F;
	triangle_buf[1] = 0x7F;
	triangle_buf[0] = 0xFF;
//	WriteToFlash(&triangle_buf[0], 9600);
	TFT_init();
	while (1)
	{

	}
}


void SetSysClock_168MHz(void)
{
  /* At this stage the HSI is already enabled and used as System clock source */

    /* Select regulator voltage output Scale 1 mode, System frequency up to 168 MHz */
  RCC->APB1ENR |= RCC_APB1ENR_PWREN;
  PWR->CR |= PWR_CR_VOS;

  /* HCLK = SYSCLK / 1*/
  RCC->CFGR |= RCC_CFGR_HPRE_DIV1;

  /* PCLK2 = HCLK / 2*/
  RCC->CFGR |= RCC_CFGR_PPRE2_DIV2;

  /* PCLK1 = HCLK / 1*/
  RCC->CFGR |= RCC_CFGR_PPRE1_DIV4;

  /* Configure the main PLL */
  RCC->PLLCFGR = PLL_M | (PLL_N << 6) | (((PLL_P >> 1) -1) << 16) |
                 (RCC_PLLCFGR_PLLSRC_HSI) | (PLL_Q << 24);

  /* Enable the main PLL */
  RCC->CR |= RCC_CR_PLLON;

  /* Wait till the main PLL is ready */
  while((RCC->CR & RCC_CR_PLLRDY) == 0);
  /* Configure Flash prefetch, Instruction cache, Data cache and wait state */
  FLASH->ACR = FLASH_ACR_ICEN |FLASH_ACR_DCEN |FLASH_ACR_LATENCY_5WS;

  /* Select the main PLL as system clock source */
  RCC->CFGR &= (uint32_t)((uint32_t)~(RCC_CFGR_SW));
  RCC->CFGR |= RCC_CFGR_SW_PLL;

  /* Wait till the main PLL is used as system clock source */
  while ((RCC->CFGR & (uint32_t)RCC_CFGR_SWS ) != RCC_CFGR_SWS_PLL);
}

/* set sysclock to 84Mhz
 * no need to touch M, N or Q
 * PLL_P needs to be doubled
 */
void set_sysclk_to_84(void)
{
    SystemInit();

    #undef PLL_P
    uint32_t PLL_P = 4;

    /* Enable HSE (CR: bit 16) */
    RCC->CR |= (1 << 16);
    /* Wait till HSE is ready (CR: bit 17) */
    while(!(RCC->CR & (1 << 17)));

    /* set voltage scale to 1 for max frequency */
    /* first enable power interface clock (APB1ENR:bit 28) */
    RCC->APB1ENR |= (1 << 28);

    /* then set voltage scale to 1 for max frequency (PWR_CR:bit 14)
     * (0) scale 2 for fCLK <= 144 Mhz
     * (1) scale 1 for 144 Mhz < fCLK <= 168 Mhz
     */
    PWR->CR |= (1 << 14);

    /* set AHB prescaler to /1 (CFGR:bits 7:4) */
    RCC->CFGR |= (0 << 4);
    /* set ABP low speed prescaler to /4 (APB1) (CFGR:bits 12:10) */
    RCC->CFGR |= (5 << 10);
    /* set ABP high speed prescaper to /2 (ABP2) (CFGR:bits 15:13) */
    RCC->CFGR |= (4 << 13);

    /* Set M, N, P and Q PLL dividers
     * PLLCFGR: bits 5:0 (M), 14:6 (N), 17:16 (P), 27:24 (Q)
     * Set PLL source to HSE, PLLCFGR: bit 22, 1:HSE, 0:HSI
     */
    RCC->PLLCFGR = PLL_M | (PLL_N << 6) | (((PLL_P >> 1) -1) << 16) |
                   (PLL_Q << 24) | (1 << 22);
    /* Enable the main PLL (CR: bit 24) */
    RCC->CR |= (1 << 24);
    /* Wait till the main PLL is ready (CR: bit 25) */
    while(!(RCC->CR & (1 << 25)));
    /* Configure Flash
     * prefetch enable (ACR:bit 8)
     * instruction cache enable (ACR:bit 9)
     * data cache enable (ACR:bit 10)
     * set latency to 2 wait states (ARC:bits 2:0)
     *   see Table 10 on page 80 in RM0090
     */
    FLASH->ACR = (1 << 8) | (1 << 9) | (1 << 10 ) | (2 << 0);

    /* Select the main PLL as system clock source, (CFGR:bits 1:0)
     * 0b00 - HSI
     * 0b01 - HSE
     * 0b10 - PLL
     */
    RCC->CFGR &= ~(3U << 0);
    RCC->CFGR |=  (2  << 0);
    /* Wait till the main PLL is used as system clock source (CFGR:bits 3:2) */
    while (!(RCC->CFGR & (2 << 2)));

    SystemCoreClock = 84000000;
}
