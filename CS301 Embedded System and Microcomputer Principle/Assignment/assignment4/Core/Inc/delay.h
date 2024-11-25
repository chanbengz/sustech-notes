#ifndef _DELAY_H_
#define _DELAY_H_

#include <stdint.h>

void HAL_Delay_Init(uint8_t xmhz);
void HAL_Delay_us(uint32_t us);
void HAL_Delay_us2(uint32_t nus); // not tested

#endif