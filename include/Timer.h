//
// This file is part of the GNU ARM Eclipse distribution.
// Copyright (c) 2014 Liviu Ionescu.
//

#ifndef TIMER_H_
#define TIMER_H_

#include "cmsis_device.h"
#include <stm32f401xe.h>
#include <stdint-gcc.h>
#include <stdint-gcc.h>


volatile uint32_t microseconds;

typedef enum{
	SECOND = 1,
	MILLISEC = 1000,
	MICROSEC = 1000000
}Time_Resolution;


void Init_Time(Time_Resolution res);

void Delay(uint16_t milliseconds);

//uint32_t Micros();

uint32_t Millis();


//systick interrupt


#endif // _TIME_H_
