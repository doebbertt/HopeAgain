#ifndef MXCONSTANTS_H_
#define MXCONSTANTS_H_


#define ESP_ResponseTimeout_ms 6000
#define ESP_USART USART1
#define DEBUG_CMD_USART USART3

//USART buffers and related
#define RxBuffSize 1000
#define USART_TxComplete_Timeout_ms 1000
#define DMA_Rx_Buff_Poll_Int_ms 500

//Wifi related Variables and declarations
#define WIFI_COMMAND_ERROR "ERROR" // Expected response from ESP8266 on error
#define WIFI_COMMAND_ACCEPTED "OK" // Expected response from ESP8266 on successful command process
#define WIFI_RX_LineComplete = "\r\n"

//WebServer related defines
#define RESPONSE_BUFFER_SIZE 300

//Helpful Macros
#define countof(a)   (sizeof(a) / sizeof(*(a)))
#define ARRAYSIZE(x) (sizeof(x)/sizeof (x)[0]) // Need to test this one.. I think its broken.


/* Private define ------------------------------------------------------------*/

#define B1_Pin GPIO_PIN_13
#define B1_GPIO_Port GPIOC
#define LD2_Pin GPIO_PIN_5
#define LD2_GPIO_Port GPIOA
#define TMS_Pin GPIO_PIN_13
#define TMS_GPIO_Port GPIOA
#define SWO_Pin GPIO_PIN_3
#define SWO_GPIO_Port GPIOB




/**
  * @}
  */ 

/**
  * @}
*/ 

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/

#endif // MXCONSTANTS_H_
