/********************************************************
 * Author: SUK & Thomas
 * Date: 03/14/2016
 * esp8266.c (esp8266 wifi library source file)
 ********************************************************/

#include "ESP8266.h"
#include "mxconstants.h"
#include "Timer.h"
#include "stm32f4xx_hal_conf.h"
#include "stm32f4xx_it.h"
#include "nucleo_hal_bsp.h"
#include "stm32f4xx_hal_usart.h"
#include <stdio.h>
#include <stdlib.h>

UART_HandleTypeDef huart1;
UART_HandleTypeDef huart2;
DMA_HandleTypeDef hdma_usart1_rx;
DMA_HandleTypeDef hdma_usart1_tx;
DMA_HandleTypeDef hdma_usart2_rx;
DMA_HandleTypeDef hdma_usart2_tx;
DMA_HandleTypeDef hdma_memtomem_dma2_stream0;


volatile uint8_t waitingForReponse = 1;
volatile uint8_t OKFound = 0;
volatile uint8_t ERRORFound = 0;
volatile uint32_t TxWaitForResponse_TimeStmp = 0;
extern volatile char rx_circular_buffer[RxBuffSize];
extern volatile char rx_circular_bufferTMP[RxBuffSizeTMP];


const char *ATCommandsArray[18] = {"AT\r\n",
	"AT+CIPSTATUS\r\n",
	"AT+GMR\r\n",
	"AT+CWMODE?\r\n",
	"AT+CWMODE=1\r\n",
	"AT+CWJAP=\"SSID\",\"PASSWORD\"\r\n",
	"AT+CIPSEND=5\r\n",
	"AT+CIPSEND=16\r\n",
	"AT+CIPSEND=13\r\n",
	"AT+CIPSEND=13\r\n",
	"AT+CIPSEND=11\r\n",
	"AT+CIPSEND=10\r\n",
	"AT+CIPSEND=7\r\n",
	"AT+RST\r\n",
	"AT+CIOBAUD=115200\r\n",
	"AT+CIPCLOSE",
	"AT+CIFSR\r\n",
	"AT+CIPSTART=\"TCP\",\"172.16.11.14\",8080\r\n",
	"ATE0\r\n"};


const char *ESP_Responses[11] =
{
		"ready",
		"Link",
		"Unlink",
		"SEND OK",
		"OK",
		"+IPD",
		"ALREADY CONNECTED",
		"wrong syntax",
		"busy p...",
		"busy",
		"ERROR"
};

void ClearArray_Size(char buffer[], uint16_t size)
{
	memset(buffer, '\0', size);

}

void SetArray_Size(char buffer[], uint16_t size)
{
	memset(buffer, '1', size);
}

void Wifi_ReadyWaitForAnswer()
{
	TxWaitForResponse_TimeStmp = Millis();
	waitingForReponse = 1;
	uint8_t i=0;
	//for (i=0;i<5;i++);
}

void Wifi_WaitForAnswer()
{
	while(waitingForReponse == 1 && (Millis() - TxWaitForResponse_TimeStmp) < ESP_ResponseTimeout_ms);
	OKFound=0;
	ERRORFound=0;
}

char *WaitForAnswer_cmd_Buffer;
char *WaitForAnswer_ans_Buffer;
char *WaitForAnswer_AlreadyConn;

uint8_t rxCount = 0;
void Wifi_WaitForAnswerCMD(char *cmdToWaitFor, uint16_t *cmdSize, Wifi_Commands command)
{
	while(waitingForReponse == 1 && (Millis() - TxWaitForResponse_TimeStmp) < ESP_ResponseTimeout_ms)
	{
		uint8_t i = 0;
		// implemented out b/c of gdb optimizing out variables -> see documentation
		while(((strlen(rx_circular_bufferTMP)) == 0) && (i == 40)){
			getResponse_CMD(cmdSize, command);
			i++;
		}
		if((rx_circular_bufferTMP[0] != '\0') || (rx_circular_buffer[2] != '\0')){
			uint8_t n=0;
			while(memmem(rx_circular_bufferTMP, strlen(rx_circular_bufferTMP), ESP_Responses[n],strlen(ESP_Responses[n])) == NULL){
				n++;
				if(n==12){
					n=10;
					break;
				}
			}
			if(n==10){
				trace_printf("\n Error:ESP module not responding or showing an error -> rx_circular_buffer: %s", rx_circular_buffer);
				trace_printf("\n Value of n is %d", n);
				waitingForReponse = 0;
			}else{
				OKFound=1;
				waitingForReponse = 0;
				trace_printf("\n The ESP module answered with the following command -> rx_circular_buffer: %s", rx_circular_buffer);
				trace_printf("\n Value of n is %d", n);
			}
		}
	};

}
/*
 * tried to implement without clearing the buffer but "tmp" always got
 * <optimized out> which is a known gdb error -> see documentation
*/
void getResponse_CMD(uint16_t *cmdSize, Wifi_Commands command){

		volatile int found = 0;
		volatile char tmp = "";
		char pretmp = "";
		volatile uint16_t n = 0;
		if (command == WIFI_MODULE_RESET){
			cmdSize = 458;
		}else if(command == WIFI_START_CLIENT_CONN){
			cmdSize = cmdSize + 6;
		}
		while(cmdSize != n){
			huart1.pRxBuffPtr++;
			n++;
		}
		while(found != 2){
			tmp = *(huart1.pRxBuffPtr);
			if(tmp == bslash_nul){
				found = 2;
			}else if (tmp == bslash_r){
				pretmp = tmp;
				n++;
				huart1.pRxBuffPtr++;
			}else if(tmp == bslash_n){
				n++;
				huart1.pRxBuffPtr++;
			}else{
				rx_circular_bufferTMP[rxCount] = tmp;
				huart1.pRxBuffPtr++;
				rxCount++;
			}
		}
}
/*
 * tried to implement without clearing the buffer but "tmp" always got
 * <optimized out> which is a known gdb error -> see documentation
*/
void getResponse_WIFI_AT(uint16_t *cmdSize, Wifi_Commands command){
	if(command == WIFI_CHECK_MODULE_CONNECTION){
		uint8_t found = 0;
		uint8_t n=-2;
		uint8_t r_count = 0;
		uint8_t n_count = 0;
		char cmd = ATCommandsArray[command];
		char tmp ="";
		while(strlen(ATCommandsArray[command]) != n){
			huart1.pRxBuffPtr++;
			n++;
		}
		while(OKFound != 2){
			tmp = "";
			rx_circular_buffer;
			//tmp = *(huart1.pRxBuffPtr++);
			if(OKFound ==0 && n_count == 0 && r_count == 0){ //&& ((*(huart1.pRxBuffPtr++) != '\n') || (*(huart1.pRxBuffPtr++) != '\r'))){
				rx_circular_bufferTMP[rxCount] = *(huart1.pRxBuffPtr);
				rxCount++;
				huart1.pRxBuffPtr++;
				if(rx_circular_bufferTMP[0] == 'O' && rx_circular_bufferTMP[1] == 'K'){
					OKFound = 1;
				}
			}else if(*(huart1.pRxBuffPtr) == '\r'){
				r_count++;
				huart1.pRxBuffPtr++;
			}else if(*(huart1.pRxBuffPtr) == '\n'){
				n_count++;
				huart1.pRxBuffPtr++;
			}else if(r_count > 0 && n_count > 0){
				OKFound = 2;
			}else {
				ERRORFound = 1;
			}
		}
	}
}


uint32_t pointerRange = 0;
void Wifi_WaitForAnswer_SEND_OK(uint16_t cmdSize)
{

	while(waitingForReponse == 1 && (Millis() - TxWaitForResponse_TimeStmp) < ESP_ResponseTimeout_ms)
	{
		pointerRange = huart1.pRxBuffPtr;
		WaitForAnswer_cmd_Buffer = memmem(rx_circular_buffer,strlen(rx_circular_buffer),ESP_Responses[3],strlen(ESP_Responses[3]));
		if(strlen(WaitForAnswer_cmd_Buffer)>0)
		{
			while(waitingForReponse == 1 && (Millis() - TxWaitForResponse_TimeStmp) < ESP_ResponseTimeout_ms)
			{
				if(WaitForAnswer_ans_Buffer == memmem(rx_circular_buffer,strlen(rx_circular_buffer),"OK\r\n",4))
				{
					pointerRange = WaitForAnswer_cmd_Buffer - WaitForAnswer_ans_Buffer;
					//ClearArray_Size(WaitForAnswer_cmd_Buffer, cmdSize + 9);
					OKFound=1;
					waitingForReponse = 0;
				}
			}
		}
	}

}

void Wifi_SendCustomCommand(Wifi_Commands command, SensorData cmd)
{

			HAL_DMA_Start_IT(&hdma_usart1_tx, ATCommandsArray[command],  (uint32_t)&huart1.Instance->DR, strlen(ATCommandsArray[command]));
					huart1.Instance->CR3 |= USART_CR3_DMAT;

			Wifi_ReadyWaitForAnswer();
			uint16_t i = 0;
			// implemented out b/c of gdb optimizing out variables -> see documentation
			while(((strlen(rx_circular_bufferTMP) == 0)) && (i == 40)){
				getResponse_CMD(strlen(ATCommandsArray[command]), command);
				i++;
			}

			ClearArray_Size(rx_circular_bufferTMP, RxBuffSizeTMP);

			Wifi_WaitForAnswer();
			HAL_DMA_Start_IT(&hdma_usart1_tx, SensorArray[cmd],  (uint32_t)&huart1.Instance->DR, strlen(SensorArray[cmd]));
							huart1.Instance->CR3 |= USART_CR3_DMAT;

			Wifi_ReadyWaitForAnswer();

			Wifi_WaitForAnswerCMD(ATCommandsArray[command], strlen(ATCommandsArray[command]), command);
			ClearArray_Size(rx_circular_bufferTMP, RxBuffSizeTMP);
}

void Wifi_SendCommand(Wifi_Commands command)
{

	HAL_DMA_Start_IT(&hdma_usart1_tx, ATCommandsArray[command],  (uint32_t)&huart1.Instance->DR, strlen(ATCommandsArray[command]));
		huart1.Instance->CR3 |= USART_CR3_DMAT;

	Wifi_ReadyWaitForAnswer();

	Wifi_WaitForAnswerCMD(ATCommandsArray[command], strlen(ATCommandsArray[command]), command);

}
