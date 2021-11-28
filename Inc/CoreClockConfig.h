#ifndef _CoreClockCongig_H_
#define _CoreClockConfig_H_

#include "global.h"

#ifdef SCK_168
	/* PLL_VCO = (HSE_VALUE or HSI_VALUE / PLL_M) * PLL_N */
	#undef PLL_M
	uint32_t PLL_M = 16;
	#undef PLL_N
	uint32_t PLL_N = 336;

	/* SYSCLK = PLL_VCO / PLL_P */
	#undef PLL_P
	uint32_t PLL_P = 2;

	/* USB OTG FS, SDIO and RNG Clock =  PLL_VCO / PLLQ */
	#undef PLL_Q
	uint32_t PLL_Q = 8;
#endif

#ifdef SCK_84
    #undef PLL_P
    uint32_t PLL_P = 4;
#endif


void SetSysClk_168MHz(void);
void SetSysClk_84MHz(void);

#endif
