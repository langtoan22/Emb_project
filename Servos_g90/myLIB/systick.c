#include "systick.h"

volatile uint32_t msTick;
void SysTick_Handler(void)
{
    msTick++;
}

void SystickInit(void)
{
    SystemCoreClockUpdate();
    SysTick_Config(SystemCoreClock / 1000);

		/*SysTick->LOAD = 72 * 1000  - 1;
		SysTick ->VAL = 0;
		SysTick ->CTRL = 5;	// chon clock AHP. enable systick (101 = 5)
							// 1 = clocksource
							// 0 =  tick Int đc bat len 1 khi ma có 1 su kien interrup systmem dua ra ngoai
							// 1 = enable
		while( ! (SysTick ->CTRL & (1 << 16))) // khi ma load tu 72000 den 0 thi bit 16 bat len 1
		{
		}*/
			SysTick_Handler();
}

uint32_t getTick(void)
{
	return msTick;
}



void Delay_Ms(int time)
{
  uint32_t start = getTick();
	
  while (getTick() - start <= time)
  {
    // Không làm gì cả, chờ đợi
  }
}

