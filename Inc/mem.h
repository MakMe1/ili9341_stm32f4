#ifndef _MEM_H_
#define _MEM_H_

#include "global.h"

void EraseSector(void);
void WriteToFlash(uint8_t *data, int size_data);
void ReadFromFlash(uint8_t *data, uint32_t mem_address);

#endif
