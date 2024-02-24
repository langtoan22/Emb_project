#ifndef __SYSTICK__H
#define __SYSTICK__H

#include "stm32f10x.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x_gpio.h"
#include "system_stm32f10x.h"

void SystickInit(void);
void SysTick_Handler(void);
uint32_t getTick(void);
void Delay_Ms(int time);


#endif