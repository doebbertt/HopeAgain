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

volatile uint8_t waitingForReponse;
volatile uint8_t OKFound;
volatile uint8_t ERRORFound;

//#define ESP_ResponseTimeout_ms 3000
//#define ESP_USART USART3


typedef enum
{
	ESP_RESPONSE_READY,ESP_RESPONSE_Link,ESP_RESPONSE_Unlink,ESP_RESPONSE_OK,
	ESP_RESPONSE_SEND_OK,ESP_RESPONSE_IPD,ESP_RESPONSE_ERROR,
	ESP_RESPONSE_Wrong_Syntax,ESP_RESPONSE_BUSY_P,ESP_RESPONSE_BUSY_INET
}ESP_Messages;

typedef enum
{
	WIFI_CHECK_MODULE_CONNECTION = 0,
	WIFI_CURRENT_STATUS, //Get current status
	WIFI_AP_LIST, // Get available AP's
	WIFI_FIRMWARE_VERSION, // Get current firmware version on ESP8266
	WIFI_GET_CURRENT_MODE, // Gets the current mode (Access Point, Station, both)
	WIFI_SET_MODE_BOTH_AP_ST, // Set Mode as both, Access Point, Station
	WIFI_JOIN_NONYA,
	WIFI_SHOW_CURRENT_CONNECTED_AP,
	WIFI_MODULE_RESET,
	WIFI_SET_MULTICONNECTION,
	WIFI_SET_BAUD_115200,
	WIFI_START_LOCAL_SERVER_PORT_80,
	WIFI_GET_CURRENT_IP,
	WIFI_SHOW_ACTIVE_CONNECTIONS,
	WIFI_LIST_CONNECTED_DEVICES_inAPModeOnly,
	WIFI_QUIT_CURRENT_AP,
	WIFI_START_CLIENT_CONN,
	WIFI_DISABLE_ECHO
}Wifi_Commands;

typedef enum
{
	OPEN,
	WEP,
	WPA_PSK,
	WPA2_PSK,
	WPA_WPA2_PSK
}Available_Encyption;

//TYPEDEF DECLARATIONS
typedef struct{
	uint8_t ConnectionNum;
	char *DataSize;
	char *RequestType; //ie.. POST, GET, PUT, DELETE
	char *URI; //ie.. /api/foo?id=123
	char *Headers;
	char *Body;
	uint8_t Valid;
}IPD_Data;

extern const char *ATCommandsArray[18];

#define WIFI_COMMAND(commandEnum) (ATCommandsArray[(commandEnum)])




void Wifi_ReadyWaitForAnswer();
void Wifi_WaitForAnswer();
void Wifi_CloseConnection(uint8_t connectionNum);
void Wifi_SendCustomCommand(char *customMessage);
void Wifi_SendCustomCommand_External_Wait(char *customMessage);
void Wifi_SendCommand(Wifi_Commands command );
IPD_Data Wifi_CheckDMABuff_ForIPDData();
void ConnectToAP(char *apName, char *password);
void StartLocalAP(char *SSID, char *password, uint8_t channel, Available_Encyption encypt);


#endif //_ESP8266_H_
