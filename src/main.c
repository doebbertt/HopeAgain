
/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_hal.h"
#include "nucleo_hal_bsp.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "diag/Trace.h"
#include "ESP8266.h"
#include "stm32f4xx_hal_usart.h"
#include "sensor.h"

/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Private variables ---------------------------------------------------------*/
UART_HandleTypeDef huart1;
UART_HandleTypeDef huart2;
DMA_HandleTypeDef hdma_usart1_rx;
DMA_HandleTypeDef hdma_usart1_tx;
DMA_HandleTypeDef hdma_usart2_rx;
DMA_HandleTypeDef hdma_usart2_tx;
DMA_HandleTypeDef hdma_memtomem_dma2_stream0;
char rx_circular_buffer[RxBuffSize];
char rx_circular_bufferTMP[RxBuffSizeTMP];
char sensor_temp_str;

/* USER CODE BEGIN PV */
/* Private variables ---------------------------------------------------------*/

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/


/* USER CODE BEGIN PFP */
/* Private function prototypes -----------------------------------------------*/

int main(void)
{

	Nucleo_BSP_Init();


  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration----------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();


  HAL_UART_Receive_DMA(&huart1, rx_circular_buffer, RxBuffSize);

	//Reset ESP8266
//  Wifi_SendCommand(WIFI_MODULE_RESET);
//  trace_printf("rx_circular_buffer: %s", rx_circular_bufferTMP);
//  trace_printf("rx_circular_buffer: %s", rx_circular_buffer);
//  ClearArray_Size(rx_circular_bufferTMP, RxBuffSizeTMP);
//  ClearArray_Size(rx_circular_buffer, RxBuffSize);

  uint8_t Mode = 0;
  while (1)
  {

	  if(HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_13) != GPIO_PIN_SET)
	  	  {
		  	  if(Mode == 0){
		  		  Mode = 1;
		  	  }else{
		  		  Mode = 0;
		  	  }
		  	  Wifi_SendCommand(WIFI_CHECK_MODULE_CONNECTION);
		  	  //trace_printf("rx_circular_buffer: %s", rx_circular_bufferTMP);
		  	  trace_printf("rx_circular_buffer: %s", rx_circular_buffer);
		  	  //ClearArray_Size(rx_circular_bufferTMP, RxBuffSizeTMP);
		  	  Wifi_SendCommand(WIFI_START_CLIENT_CONN);
		  	  //trace_printf("rx_circular_buffer: %s", rx_circular_bufferTMP);
		  	  trace_printf("rx_circular_buffer: %s", rx_circular_buffer);
		  	  ClearArray_Size(rx_circular_bufferTMP, RxBuffSizeTMP);

		  	  Wifi_SendCustomCommand(WIFI_SEND_TEMP_MSG, SENSOR_TEMP_MSG);
		  	  if(Mode == 0){
		  		  Wifi_SendCustomCommand(WIFI_SEND_TEMP_VAL, SENSOR_TEMP1);
		  	  }else{
		  		Wifi_SendCustomCommand(WIFI_SEND_TEMP_VAL, SENSOR_TEMP2);
		  	  }
		  	  //trace_printf("rx_circular_buffer: %s", rx_circular_bufferTMP);
		  	  trace_printf("rx_circular_buffer: %s", rx_circular_buffer);
		  	  ClearArray_Size(rx_circular_bufferTMP, RxBuffSizeTMP);
		  	  Wifi_SendCustomCommand(WIFI_SEND_HUM_MSG, SENSOR_HUM_MSG);
		  	  if(Mode == 0){
		  		Wifi_SendCustomCommand(WIFI_SEND_HUM_VAL, SENSOR_HUM1);
		  	  }else{
		  		Wifi_SendCustomCommand(WIFI_SEND_HUM_VAL, SENSOR_HUM2);
		  	  }
		  	  //trace_printf("rx_circular_buffer: %s", rx_circular_bufferTMP);
		  	  trace_printf("rx_circular_buffer: %s", rx_circular_buffer);
		  	  ClearArray_Size(rx_circular_bufferTMP, RxBuffSizeTMP);
		  	  Wifi_SendCustomCommand(WIFI_SEND_LIGHT_MSG, SENSOR_LIGHT_MSG);
		  	  if(Mode == 0){
		  		Wifi_SendCustomCommand(WIFI_SEND_LIGHT_VAL, SENSOR_LIGHT1);
		  	  }else{
		  		Wifi_SendCustomCommand(WIFI_SEND_LIGHT_VAL, SENSOR_LIGHT2);
		  	  }
		  	  //trace_printf("rx_circular_buffer: %s", rx_circular_bufferTMP);
		  	  trace_printf("rx_circular_buffer: %s", rx_circular_buffer);
		  	  ClearArray_Size(rx_circular_bufferTMP, RxBuffSizeTMP);

		  	  Wifi_SendCommand(WIFI_CLOSE_SOCKET_CONN);
		  	  ClearArray_Size(rx_circular_buffer, RxBuffSize);
	  	  }

  }
  /* USER CODE END 3 */

}

#ifdef USE_FULL_ASSERT

/**
   * @brief Reports the name of the source file and the source line number
   * where the assert_param error has occurred.
   * @param file: pointer to the source file name
   * @param line: assert_param error line source number
   * @retval None
   */
void assert_failed(uint8_t* file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
    ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */

}

#endif

/**
  * @}
  */ 

/**
  * @}
*/ 

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
