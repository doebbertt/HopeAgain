#include "Timer.h"

volatile uint32_t millis = 0;

void Init_Time(Time_Resolution res)
{
SysTick_Config((SystemCoreClock/res));

//Just making sure that the Systick Interrupt is top priority
//Or the timer will not be accurate
NVIC_SetPriority(SysTick_IRQn, 0);

}

uint32_t Millis()
{
return millis;
}

void SysTick_Handler()
{

	millis++;
}

//
