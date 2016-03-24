#include "Timer.h"
#include <time.h>
#include "stm32f4xx_it.h"


void Init_Time(Time_Resolution res)
{
SysTick_Config((SystemCoreClock/res));

NVIC_SetPriority(SysTick_IRQn, 0);

}

void delay(unsigned int mseconds)
{
    clock_t goal = mseconds + clock();
    while (goal > clock());
}

