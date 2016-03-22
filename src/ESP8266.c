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



char *ESP_IPD_Data_Buffer_Pntr;
char ESP_IPD_DataBuffer[RxBuffSize];

UART_HandleTypeDef huart1;
UART_HandleTypeDef huart2;
DMA_HandleTypeDef hdma_usart1_rx;
DMA_HandleTypeDef hdma_usart1_tx;
DMA_HandleTypeDef hdma_usart2_rx;
DMA_HandleTypeDef hdma_usart2_tx;
DMA_HandleTypeDef hdma_memtomem_dma2_stream0;


char commandToSend[70];
volatile uint8_t waitingForReponse = 1;
volatile uint8_t OKFound = 0;
volatile uint8_t ERRORFound = 0;
volatile uint32_t TxWaitForResponse_TimeStmp = 0;
extern volatile char rx_circular_buffer[RxBuffSize];
extern volatile char rx_circular_bufferTMP[RxBuffSizeTMP];



uint32_t dimmingValueToValidate = 30000; //Just a starting value that is outside the allowed

extern volatile uint32_t dimmingValue;
extern uint32_t lastDMABuffPoll;


uint16_t incommingDimmingValue = 0;
char *dimmingString;
char *URI;
char *queryString1;
char *queryValue1;
char *queryString2;
char *queryValue2;
char *SSID = "CWLAN";
char *PASSWORD = "wedel";
char *protocol = "TCP";
char *IP = "172.16.11.04";
char *PORT = "8080";


IPD_Data currentIPD;

//METHOD DECLARATIONS
IPD_Data ProcessIPD_Data(char *IPD_Buffer);



const char *ATCommandsArray[18] = {"AT\r\n",
	"AT+CIPSTATUS\r\n",
	"AT+CWLAP\r\n",
	"AT+GMR\r\n",
	"AT+CWMODE?\r\n",
	"AT+CWMODE=3\r\n",
	"AT+CWJAP=\"SSID\",\"PASSWORD\"\r\n",
	"AT+CWJAP?\r\n",
	"AT+RST\r\n",
	"AT+CIPMUX=1\r\n",
	"AT+CIOBAUD=115200\r\n",
	"AT+CIPCLOSE",
	"AT+CIFSR\r\n",
	"AT+CIPSTART=?\r\n",
	"AT+CWLIF\r\n",
	"AT+CWQAP\r\n",
	"AT+CIPSTART=\"TCP\",\"172.16.11.4\",8080\r\n",
	"ATE0\r\n",};


const char *ESP_Responses[11] =
{
		"ready",
		"Link",
		"Unlink",
		"OK",
		"SEND OK",
		"+IPD",
		"ERROR",
		"wrong syntax",
		"busy p...",
		"busy inet...",
		"ALREADY CONNECTED"
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

		getResponse_CMD(cmdSize, command);
		//if(strlen(rx_circular_bufferTMP) > 0){
		if((rx_circular_bufferTMP[0] != '\0') || (rx_circular_buffer[2] != '\0')){
			//rx_circular_bufferTMP[0] = 'O';
			//rx_circular_bufferTMP[1] = 'K';
			uint8_t n=0;
			while(memmem(rx_circular_bufferTMP, strlen(rx_circular_bufferTMP), ESP_Responses[n],strlen(ESP_Responses[n])) == NULL){
			//while(strstr(rx_circular_bufferTMP, ESP_Responses[n]) == NULL){
				n++;
				if(n==12){
					n=6;
					break;
				}
			}
			if(n==6){
				trace_printf("\n Error:ESP module not responding or showing an error -> rx_circular_buffer: %s", rx_circular_bufferTMP);
				//trace_write((char*)rx_circular_bufferTMP, 50);
				trace_printf("\n Value of n is %d", n);
				waitingForReponse = 0;
				//ClearArray_Size(rx_circular_bufferTMP, 1000);
			}else{
				OKFound=1;
				waitingForReponse = 0;
				trace_printf("\n The ESP module answered with the following command -> rx_circular_buffer: %s", rx_circular_bufferTMP);
				//trace_write((char*)rx_circular_bufferTMP, 50);
				trace_printf("\n Value of n is %d", n);
				//ClearArray_Size(rx_circular_bufferTMP, 1000);
			}
		}
	};

}

void getResponse_CMD(uint16_t *cmdSize, Wifi_Commands command){

		int found = 0;
		char tmp = "";
		char pretmp = "";
		uint16_t n = 0;
		if (command == WIFI_MODULE_RESET){
//			while(*(huart1.pRxBuffPtr + n) != '\0')
//			{
//				n++;
//			}
//			n = n - 30;
			n = 458;
		}else if(command == WIFI_START_CLIENT_CONN){
			n = cmdSize + 6;
		}else{
			n = cmdSize;
		}
		while(found != 2){
			rx_circular_buffer;

			tmp = *(huart1.pRxBuffPtr + n);
			if(tmp == '\0'){
				found = 2;
			}else if (tmp == '\r'){
				pretmp = tmp;
				n++;
//				tmp = *(huart1.pRxBuffPtr + n);
//				if(tmp == '\n'){
//					found = found +1;
//					n++;
//				}
			}else if(tmp == '\n'){ //&& pretmp != '\r'){
				n++;
			}else{
				rx_circular_bufferTMP[rxCount] = tmp;
				//trace_write((char*)rx_circular_bufferTMP, 20);
				rxCount++;
				n++;
			}
		}
		trace_write((char*)rx_circular_bufferTMP, rxCount);

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
			//Check for OK or Error Message
		}
	}

}

void Wifi_SendCustomCommand(char *customMessage)
{
			//Wifi_SendCustomCommand_External_Wait(customMessage);
			uint8_t length = (int)customMessage;
			char cMsgLength = (char)strlen(customMessage);
			char ADD []= "\r\n";
			char ATSEND []= "AT+CIPSEND=";
			char ATSENDcommand = strcat("AT+CIPSEND=",cMsgLength);


			HAL_DMA_Start_IT(&hdma_usart1_tx, ATSENDcommand,  (uint32_t)&huart1.Instance->DR, strlen(ATSENDcommand));
				huart1.Instance->CR3 |= USART_CR3_DMAT;

			Wifi_WaitForAnswer();
			HAL_DMA_Start_IT(&hdma_usart1_tx, customMessage,  (uint32_t)&huart1.Instance->DR, strlen(customMessage));
							huart1.Instance->CR3 |= USART_CR3_DMAT;
			Wifi_WaitForAnswer();
			//for (wi=0;wi<735000;wi++);
}

void Wifi_SendCustomCommand_External_Wait(char *customMessage)
{
		while(*customMessage)
		{
			while(USART_GetFlagStatus(ESP_USART,USART_FLAG_TXE) == RESET);
			USART_SendData(ESP_USART,*customMessage++);
		}

		while(USART_GetFlagStatus(ESP_USART, USART_FLAG_TXE) == RESET);
			USART_SendData(ESP_USART,'\r');

		while(USART_GetFlagStatus(ESP_USART, USART_FLAG_TXE) == RESET);
			Wifi_ReadyWaitForAnswer();
			USART_SendData(ESP_USART,'\n');

			//Wifi_WaitForAnswer();
			//for (wi=0;wi<735000;wi++);
}

//Waits to return untill wifi responds (OK or ERROR)
void Wifi_SendCommand(Wifi_Commands command)
{

	HAL_DMA_Start_IT(&hdma_usart1_tx, ATCommandsArray[command],  (uint32_t)&huart1.Instance->DR, strlen(ATCommandsArray[command]));
		huart1.Instance->CR3 |= USART_CR3_DMAT;

	Wifi_ReadyWaitForAnswer();

	Wifi_WaitForAnswerCMD(ATCommandsArray[command], strlen(ATCommandsArray[command]), command);

}

char *IPD_Processing_buf;
char *ConnectNum;
//Breaks the IPD message into a proper request object
IPD_Data ProcessIPD_Data(char *IPD_Buffer)
{
	//IPD_Processing_buf = strdupa(IPD_Buffer);
	//IPD_Processing_buf = &IPD_Buffer + 5;
	IPD_Data thisIPDMessage;

	strtok(IPD_Buffer,",");

	ConnectNum = strtok(NULL,",");
	thisIPDMessage.ConnectionNum = atoi(ConnectNum);

	thisIPDMessage.DataSize = strtok(NULL,":");
	//TODO: Probably need to add a check to make sure actual datasize matches expected..

	thisIPDMessage.RequestType = strtok(NULL," ");

	thisIPDMessage.URI = strtok(NULL," ");

	strtok(NULL,"\r\n");

	thisIPDMessage.Headers = strtok(NULL,"{");

	thisIPDMessage.Body = strtok(NULL,"}");
	return thisIPDMessage;

}

IPD_Data Wifi_CheckDMABuff_ForIPDData()
{
	currentIPD.Valid = 0;
	//if((Millis() - lastDMABuffPoll) >= DMA_Rx_Buff_Poll_Int_ms)
	//		{
				//Probably need to check for new client ({clientNum},CONNECT)
				lastDMABuffPoll = Millis();
				ESP_IPD_Data_Buffer_Pntr = memmem(rx_circular_buffer,RxBuffSize,"+IPD",4);
				if(ESP_IPD_Data_Buffer_Pntr)
				{
					//position = DMA_GetCurrDataCounter(DMA1_Channel3);
					//position = strlen(rx_circular_buffer);
					//Copy IPD message and data to its own buffer so DMA can go about its business
					strcpy(ESP_IPD_DataBuffer,ESP_IPD_Data_Buffer_Pntr);
					DMA_Cmd(DMA_CHANNEL_4,DISABLE);

					//Wipes the received message from the DMA buffer (using the pointer to the data)
					//This makes sure the data doesn't get mistaken for a new request, on the next buffer polling.
					ClearArray_Size(ESP_IPD_Data_Buffer_Pntr,strlen(ESP_IPD_Data_Buffer_Pntr));
					DMA_Initialize(rx_circular_buffer, RxBuffSize);


					//now we process since DMA isn't going to stomp on us.
					currentIPD = ProcessIPD_Data(ESP_IPD_DataBuffer);

						//TODO: Need to add a level of error detection/correction as data may be missing the
					if(strstr(currentIPD.RequestType, "POST"))
					{
						//if URI contains dimming (the test for now)
						if(strstr(currentIPD.URI, "dimming"))
						{
							if(strstr(currentIPD.URI, "?"))//If query String is found
							{
								URI = strtok(currentIPD.URI, "?");
								if(strstr(URI,"="))//If URI was sent prepended with a '/' this will be true
								{
									queryString1 = strtok(URI, "=");

									queryValue1 = strtok(NULL, "\0");
								}
								else
								{
								queryString1 = strtok(NULL, "=");
								if(strstr(currentIPD.URI, "&"))
								{
									queryValue1 = strtok(NULL, "&");
								}
								else
								{
									queryValue1 = strtok(NULL, "\0");
								}
								}
								currentIPD.Valid = 1;
							}

							dimmingValueToValidate = atoi(queryValue1);
							if(dimmingValueToValidate <= 13000)
							{
								dimmingValue = dimmingValueToValidate;

								RefreshCustomRESTResponseDimmer("172.20.112.136", "192.168.4.1", dimmingValue);
								//SendRESTResponse(currentIPD.ConnectionNum, RESTResponse_Headers_Test_OK, customRESTResponse);
							}
							else {
								RefreshCustomRESTResponse("172.20.112.136", "192.168.4.1", "dimmingValue", "InvalidValue");
							}
								currentIPD.Valid = 1;
						}
					}
					//printf("Incoming webrequest\r\n");
				}
				//DMA_Rx_Buff_Index = strlen(rx_circular_buffer);
				//tstBuff = mempcpy(rx_circular_buffer_Buffer, rx_circular_buffer, size_of_rx_circular_buffer);
				//DMA_Rx_Buff_Index = tstBuff - &rx_circular_buffer_Buffer[0];
				//tstBuff = memmem(rx_circular_buffer,sizeof(rx_circular_buffer),"OK\r\n",4);
				//ClearArray_Size(rx_circular_buffer, sizeof(rx_circular_buffer));

		//	}

				return currentIPD;
}


///Pretty self explanatory
void ConnectToAP(char *apName, char *password) //Will utilize the arguments later, for now static to Nonya
{
	//TODO: Need to add check that ESP is in a compatible client mode
	sprintf(commandToSend,"AT+CWJAP=\"%s\",\"%s\"",apName,password);
	Wifi_SendCustomCommand(commandToSend);
}

//Configures ESP82667 Access Point with given parameters.
void StartLocalAP(char *SSID, char *password, uint8_t channel, Available_Encyption encypt)
{
	sprintf(commandToSend, "AT+CWSAP=\"\",\"\",\"\",\"\"");
}
