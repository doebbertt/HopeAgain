
/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_hal.h"
#include "nucleo_hal_bsp.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "diag/Trace.h"
#include "ESP8266.h"
#include "stm32f4xx_hal_usart.h"

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
//char *AT = "AT";
//char *RST = "AT+RST";
//char *CIPSTART = "AT+CIPSTART=""TCP"",""172.16.11.6"",33333";
//char *CIPSEND = "AT+CIPSEND=5";
char *msg = "HELLO";
//volatile extern uint8_t waitingForResponse;
uint8_t TxCounter = 0;

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
  /*
  HAL_DMA_Start_IT(&hdma_usart1_tx,  (uint32_t)RST,  (uint32_t)&huart1.Instance->DR, strlen(RST));
      huart1.Instance->CR3 |= USART_CR3_DMAT;
  trace_write((char*)RST, 6);

  HAL_UART_Receive_DMA(&huart1, rx_circular_buffer, RxBuffSize);

  trace_write((char*)rx_circular_buffer, 20);

  if(rx_circular_buffer[0] == 'O' && rx_circular_buffer[1] == 'K')
  {
	  HAL_DMA_Start_IT(&hdma_usart1_tx,  (uint32_t)CIPSTART,  (uint32_t)&huart1.Instance->DR, strlen(CIPSTART));
			huart1.Instance->CR3 |= USART_CR3_DMAT;
	  trace_write((char*)CIPSTART, 41);


  }

  HAL_DMA_Start_IT(&hdma_usart1_tx,  (uint32_t)CIPSTART,  (uint32_t)&huart1.Instance->DR, strlen(CIPSTART));
      huart1.Instance->CR3 |= USART_CR3_DMAT;
  trace_write((char*)CIPSTART, 41);
	*/
  HAL_UART_Receive_DMA(&huart1, rx_circular_buffer, RxBuffSize);

  //HAL_Delay(2000);


  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
	  if(HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_13) != GPIO_PIN_SET)
	  	  {
		  	  //Wifi_SendCommand(WIFI_MODULE_RESET);
		  	  //HAL_DMA_Start_IT(&hdma_usart1_rx, (uint32_t)&huart1.Instance->DR, rx_circular_buffer, strlen(rx_circular_buffer));
		  	  //Wifi_SendCommand(WIFI_MODULE_RESET);
		  	  //ClearArray_Size(rx_circular_buffer, 2000);
		  	  //HAL_Delay(2000);
		  	  Wifi_SendCommand(WIFI_CHECK_MODULE_CONNECTION);
		  	  trace_printf("rx_circular_buffer: %s", rx_circular_bufferTMP);
		  	  trace_printf("rx_circular_buffer: %s", rx_circular_buffer);
		  	  ClearArray_Size(rx_circular_bufferTMP, RxBuffSizeTMP);
		  	  //HAL_Delay(500);
		  	  Wifi_SendCommand(WIFI_START_CLIENT_CONN);
		  	  //HAL_Delay(500);
		  	  trace_printf("rx_circular_buffer: %s", rx_circular_bufferTMP);
		  	  trace_printf("rx_circular_buffer: %s", rx_circular_buffer);
		  	  ClearArray_Size(rx_circular_bufferTMP, RxBuffSizeTMP);
		  	  Wifi_SendCustomCommand("Heisse Schnitte");
		  	  trace_printf("rx_circular_buffer: %s", rx_circular_bufferTMP);
		  	  trace_printf("rx_circular_buffer: %s", rx_circular_buffer);
		  	  ClearArray_Size(rx_circular_bufferTMP, RxBuffSizeTMP);
		  	  Wifi_SendCommand(WIFI_CLOSE_SOCKET_CONN);
		  	  uint_fast8_t n = 0;
		  	  //for(n=0; n<15; n++){
				  uint_fast8_t found = 0;
				  /*
				  while(__HAL_USART_GET_IT_SOURCE(&huart1, USART_IT_TXE) == RESET)
				  {
					  HAL_DMA_Start_IT(&hdma_usart1_tx, ATCommandsArray[n],  (uint32_t)&huart1.Instance->DR, strlen(AT));
						huart1.Instance->CR3 |= USART_CR3_DMAT;
						trace_write((char*)rx_circular_buffer, 20);
						if(strcmp(rx_circular_buffer, "OK\r\n") == 0){ // rx_circular_buffer[0] == 'O' && rx_circular_buffer[1] == 'K'){
							found = 1;
							n++;
						}else{
							trace_write((char*)rx_circular_buffer && (char*)"ESP module not responding", 30);
						}
				  }*/
		  	  //}

//		  	  if(found == 1){
//		  		memset(rx_circular_buffer, '\0', RxBuffSize);
//		  		found = 0;
//		  	  }
//		  	  while(__HAL_USART_GET_IT_SOURCE(&huart1, USART_IT_TXE) == RESET)
//			  {
//		  		  HAL_DMA_Start_IT(&hdma_usart1_tx,  (uint32_t)RST,  (uint32_t)&huart1.Instance->DR, strlen(RST));
//					huart1.Instance->CR3 |= USART_CR3_DMAT;
//					trace_write((char*)rx_circular_buffer, 20);
//			  }
//
//
//	  		  trace_write((char*)rx_circular_buffer, 20);
//
//
	  	  }

  }
  /* USER CODE END 3 */

}

/** System Clock Configuration
*/
/*
void SystemClock_Config(void)
{

  RCC_OscInitTypeDef RCC_OscInitStruct;

  __PWR_CLK_ENABLE();

  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE2);

  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = 16;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  HAL_RCC_OscConfig(&RCC_OscInitStruct);

  HAL_SYSTICK_Config(HAL_RCC_GetHCLKFreq()/1000);

  HAL_SYSTICK_CLKSourceConfig(SYSTICK_CLKSOURCE_HCLK);

  //SysTick_IRQn interrupt configuration
  HAL_NVIC_SetPriority(SysTick_IRQn, 0, 0);
}
*/

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

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
