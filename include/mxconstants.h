#ifndef MXCONSTANTS_H_
#define MXCONSTANTS_H_


#define ESP_ResponseTimeout_ms 600
#define ESP_USART USART1
#define DEBUG_CMD_USART USART2

//USART buffers and related
#define RxBuffSize 		2000
#define RxBuffSizeTMP	500
#define USART_TxComplete_Timeout_ms 300//3000
//#define DMA_Rx_Buff_Poll_Int_ms 500
#define bslash_nul '\0'
#define bslash_r '\r'
#define bslash_n '\n'

//Wifi related Variables and declarations
//#define WIFI_COMMAND_ERROR "ERROR" // Expected response from ESP8266 on error
//#define WIFI_COMMAND_ACCEPTED "OK" // Expected response from ESP8266 on successful command process
//#define WIFI_RX_LineComplete = "\r\n"



/* Private define ------------------------------------------------------------*/

#define B1_Pin GPIO_PIN_13
#define B1_GPIO_Port GPIOC
#define LD2_Pin GPIO_PIN_5
#define LD2_GPIO_Port GPIOA
#define TMS_Pin GPIO_PIN_13
#define TMS_GPIO_Port GPIOA
#define SWO_Pin GPIO_PIN_3
#define SWO_GPIO_Port GPIOB


/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/

#endif // MXCONSTANTS_H_
