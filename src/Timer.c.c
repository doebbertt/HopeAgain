/********************************************************
 * Author: SUK & Thomas
 * Date: 03/14/2016
 * esp8266.c (esp8266 wifi library source file)
 ********************************************************/

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

/* TODO
void SysTick_Handler()
{

	millis++;
}
*/

//
