#include "mem.h"

extern int CURRENT_FRAME_ADDRESS;

static void	UnlockFlash(void)
{
	//Write KEY1 = 0x45670123
	//Write KEY2 = 0xCDEF89AB
	if ((FLASH->CR & FLASH_CR_LOCK) != 0)
	{
		FLASH->KEYR = 0x45670123;
		FLASH->KEYR = 0xCDEF89AB;
	}
}

static void	LockFlash(void)
{
	while ((FLASH->SR & FLASH_SR_BSY) == FLASH_SR_BSY);
	FLASH->CR = FLASH_CR_LOCK;
}

void EraseSector(void)
{
	UnlockFlash();
	while(FLASH->SR & FLASH_SR_BSY);

	FLASH->CR |= FLASH_CR_SER;
	FLASH->CR |= FLASH_CR_SNB_0 | FLASH_CR_SNB_1;
	FLASH->CR |= FLASH_CR_STRT;

	while(FLASH->SR & FLASH_SR_BSY);
	FLASH->CR &= ~FLASH_CR_SER;
	LockFlash();
}

//Sector 3 0x0800C000 - 0x0800FFFF 16 Kbytes

//void WriteToFlash(uint8_t *data, int size_data)
//{
//	EraseSector();
//	UnlockFlash();
//
//	FLASH->CR |= FLASH_CR_PG;
//
//	for (int  i = 0; i < size_data; i++)
//	{
////		*(__IO uint8_t*)(FRAME_ADDRESS + i) = data[i];
//		*(__IO uint8_t*)(CURRENT_FRAME_ADDRESS + i) = data[i/30%8];
////		*(__IO uint8_t*)(FRAME_ADDRESS + i) = i%30;
////		*(__IO uint8_t*)(FRAME_ADDRESS + i) = 0;
//		while ((FLASH->SR & FLASH_SR_BSY) != 0);
//		if((FLASH->SR & FLASH_SR_EOP) == 0)
//			FLASH->SR |= FLASH_SR_EOP;
//	}
//
//	FLASH->CR &= ~FLASH_CR_PG;
//
//	LockFlash();
//}

//void MoveFlashData()
//{
//
//	FLASH->CR |= FLASH_CR_PG;
//
//	uint8_t data;
//	for (int  i = 0; i < 9570; i++)
//	{
//		data = *(__IO uint8_t*)(FRAME_ADDRESS + i + 30);
//		*(__IO uint8_t*)(FRAME_ADDRESS + i) = (uint8_t)data;
//		while ((FLASH->SR & FLASH_SR_BSY) != 0);
//		if((FLASH->SR & FLASH_SR_EOP) == 0)
//			FLASH->SR |= FLASH_SR_EOP;
//	}
//
//	FLASH->CR &= ~FLASH_CR_PG;
//
//	LockFlash();
//	WriteZerosToFlash();
//}

//void WriteZerosToFlash()
//{
//	UnlockFlash();
//
//	FLASH->CR |= FLASH_CR_PG;
//
//	for (int  i = 0; i < 30; i++)
//	{
//		*(__IO uint8_t*)(FRAME_ADDRESS + 9570 + i) = 0;
//		while ((FLASH->SR & FLASH_SR_BSY) != 0);
//		if((FLASH->SR & FLASH_SR_EOP) == 0)
//			FLASH->SR |= FLASH_SR_EOP;
//	}
//
//	FLASH->CR &= ~FLASH_CR_PG;
//
//	LockFlash();
//}

void ReadFromFlash(uint8_t *data, uint32_t mem_address)
{
	*data = *(__IO uint8_t*)(mem_address);
}
