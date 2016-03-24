/********************************************************
 * Author: SUK & Thomas
 * Date: 03/14/2016
 * esp8266.h (esp8266 wifi library include file)
 ********************************************************/

#ifndef _ESP8266_H_
#define _ESP8266_H_

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
#include "sensor.h"

volatile uint8_t waitingForReponse;
volatile uint8_t OKFound;
volatile uint8_t ERRORFound;


typedef enum
{
	WIFI_CHECK_MODULE_CONNECTION = 0,
	WIFI_CURRENT_STATUS, //Get current status
	WIFI_FIRMWARE_VERSION, // Get current firmware version on ESP8266
	WIFI_GET_CURRENT_MODE, // Gets the current mode (Access Point, Station, both)
	WIFI_SET_MODE_CLIENT, // Set Mode as client
	WIFI_JOIN_NONYA,
	WIFI_SEND_MSG,
	WIFI_SEND_TEMP_MSG,
	WIFI_SEND_TEMP_VAL,
	WIFI_SEND_HUM_MSG,
	WIFI_SEND_HUM_VAL,
	WIFI_SEND_LIGHT_MSG,
	WIFI_SEND_LIGHT_VAL,
	WIFI_MODULE_RESET,
	WIFI_SET_BAUD_115200,
	WIFI_CLOSE_SOCKET_CONN,
	WIFI_GET_CURRENT_IP,
	WIFI_START_CLIENT_CONN,
	WIFI_DISABLE_ECHO,
}Wifi_Commands;

extern const char *ATCommandsArray[18];

#define WIFI_COMMAND(commandEnum) (ATCommandsArray[(commandEnum)])

void Wifi_ReadyWaitForAnswer();
void Wifi_WaitForAnswer();
void Wifi_CloseConnection(uint8_t connectionNum);
void Wifi_SendCustomCommand(Wifi_Commands command, SensorData cmd);
void Wifi_SendCommand(Wifi_Commands command);


#endif //_ESP8266_H_
