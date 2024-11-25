#include "delay.h"
#include "main.h"

static uint8_t mhz = 72;

void HAL_Delay_Init(uint8_t xmhz){
    mhz = xmhz;
}

void HAL_Delay_us(uint32_t us)
{
    uint32_t start_val, tickn, delays, wait;

    start_val = SysTick->VAL;
    tickn = HAL_GetTick();
    delays = us * mhz;
    if (delays > start_val) {
        while (HAL_GetTick() == tickn)
            ;
        wait = mhz * 1000 + start_val - delays;
        while (wait < SysTick->VAL)
            ;
    }
    else {
        wait = start_val - delays;
        while (wait < SysTick->VAL && HAL_GetTick() == tickn)
            ;
    }
}

void HAL_Delay_us2(uint32_t nus) // not tested
{
    uint32_t temp;
    SysTick->LOAD = nus * (mhz>>3);
    SysTick->VAL = 0x00;
    SysTick->CTRL |= SysTick_CTRL_ENABLE_Msk;
    do {
        temp = SysTick->CTRL;
    } while ((temp & 0x01) && !(temp & (1 << 16)));
    SysTick->CTRL &= ~SysTick_CTRL_ENABLE_Msk;
    SysTick->VAL = 0X00;
}