//--------------------------------------------------------------
// File     : sensor.h
//--------------------------------------------------------------

//--------------------------------------------------------------
#ifndef __SENSOR_H
#define __SENSOR_H


//--------------------------------------------------------------
// Includes
//--------------------------------------------------------------
#include "Timer.h"
#include "stm32f4xx_hal.h"
#include "nucleo_hal_bsp.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "diag/Trace.h"
#include "stm32f4xx_hal_conf.h"
#include "stm32f4xx_it.h"
#include "nucleo_hal_bsp.h"


//--------------------------------------------------------------
// Enable/Disable the conversion of temperature to a string
//--------------------------------------------------------------
#define  USE_TEMP2STR
#define  USE_HUM2STR
#define  USE_LIGHT2STR


//--------------------------------------------------------------
// One-Wire Function-Command
//--------------------------------------------------------------
#define  ONE_WIRE_TEMP_CONVERT_FKT_CMD      0x40  	// start temp measurement
#define  ONE_WIRE_HUM_CONVERT_CMD			0x41	// start humidity measurement
#define	 ONE_WIRE_LIGHT_CONVERT_CMD			0x42	// start light measurement

#define  ONE_WIRE_RD_TEMP_FCT_CMD    		0x51  	// read temp value
#define  ONE_WIRE_RD_HUM_FCT_CMD			0x52	// read humidity value
#define  ONE_WIRE_RD_LIGHT_FCT_CMD			0x53	// read light value

//--------------------------------------------------------------
// defines
//--------------------------------------------------------------

#define  SensorTemp_ERR_CODE   	-1000  	// if an Error occurs this temp value will be transmitted
#define  SensorHum_ERR_CODE		-1001	// if an Error occurs this humidity value will be transmitted
#define	 SensorLight_ERR_CODE	-1002	// if an Error occurs this light value will be transmitted



//--------------------------------------------------------------
// Sensor Functions
//--------------------------------------------------------------
float getTempSensorVal();
float getHumiditySensorVal();
float getLightSensorVal();


#ifdef USE_TEMP2STR
void Sensor_TempToStr(float val, char *ptr);
#endif

#ifdef USE_HUM2STR
void Sensor_HumToStr(float val, char *ptr);
#endif

#ifdef USE_LIGHT2STR
void Sensor_LightToStr(float val, char *ptr);
#endif


//--------------------------------------------------------------
#endif // __SENSOR_H
