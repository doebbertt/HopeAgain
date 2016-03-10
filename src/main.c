/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_hal.h"
#include "nucleo_hal_bsp.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "diag/Trace.h"

/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Private variables ---------------------------------------------------------*/
//extern UART_HandleTypeDef huart2;
UART_HandleTypeDef huart2;
DMA_HandleTypeDef hdma_usart2_tx;
DMA_HandleTypeDef hdma_usart2_rx;
char *RST = "AT+RST";
char *CIPSTART = "AT+CIPSTART=""TCP"",""172.16.11.9"",33333";
char *CIPSEND = "AT+CIPSEND=5";
char *msg = "HELLO";
#define size_of_rx_circular_buffer 128
uint8_t rx_circular_buffer[size_of_rx_circular_buffer];
uint8_t const * rx_tail_ptr;


/* USER CODE BEGIN PV */
/* Private variables ---------------------------------------------------------*/

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void DMATransferComplete(DMA_HandleTypeDef *hdma);
void DMAReceivingComplete(DMA_HandleTypeDef *hdma);

/* USER CODE BEGIN PFP */
/* Private function prototypes -----------------------------------------------*/

/* USER CODE END PFP */

/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

 int main(void) {
  HAL_Init();
  char rxBuffer[32];
  char txBuffer[32];

  Nucleo_BSP_Init();

  hdma_usart2_tx.Instance = DMA1_Stream6;
  hdma_usart2_tx.Init.Direction = DMA_MEMORY_TO_PERIPH;
  hdma_usart2_tx.Init.PeriphInc = DMA_PINC_DISABLE;
  hdma_usart2_tx.Init.MemInc = DMA_MINC_ENABLE;
  hdma_usart2_tx.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
  hdma_usart2_tx.Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;
  hdma_usart2_tx.Init.Mode = DMA_NORMAL;
  hdma_usart2_tx.Init.Priority = DMA_PRIORITY_LOW;
  hdma_usart2_tx.Init.Channel = DMA_CHANNEL_4;
  hdma_usart2_tx.XferCpltCallback = &DMATransferComplete;
  HAL_DMA_Init(&hdma_usart2_tx);


  /*---------------- START hdma_usart2_rx ----------------------------------------------*/
  hdma_usart2_rx.Instance = DMA1_Stream5;
  hdma_usart2_rx.Init.Direction = DMA_PERIPH_TO_MEMORY;
  hdma_usart2_rx.Init.PeriphInc = DMA_PINC_DISABLE;
  hdma_usart2_rx.Init.MemInc = DMA_MINC_ENABLE;
  hdma_usart2_rx.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
  hdma_usart2_rx.Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;
  hdma_usart2_rx.Init.Mode = DMA_CIRCULAR;
  hdma_usart2_rx.Init.Priority = DMA_PRIORITY_LOW;
  hdma_usart2_tx.Init.FIFOMode = DMA_FIFOMODE_DISABLE;
  hdma_usart2_tx.Init.FIFOThreshold = DMA_FIFO_THRESHOLD_1QUARTERFULL;
  hdma_usart2_tx.Init.MemBurst = DMA_MBURST_SINGLE;
  hdma_usart2_tx.Init.PeriphBurst = DMA_PBURST_SINGLE;
  hdma_usart2_rx.Init.Channel = DMA_CHANNEL_4; //check other channel
  hdma_usart2_rx.XferCpltCallback = &DMAReceivingComplete;
  HAL_DMA_Init(&hdma_usart2_rx);
  __HAL_LINKDMA(&huart2, hdmarx, hdma_usart2_rx);

  uint8_t data[20];
  uint8_t error;


  /*-------------- END: hdma_usart2_rx ------------------------------------------------*/

  // DMA interrupt init
  HAL_NVIC_SetPriority(DMA1_Stream6_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(DMA1_Stream6_IRQn);

  HAL_DMA_Start_IT(&hdma_usart2_tx,  (uint32_t)RST,  (uint32_t)&huart2.Instance->DR, strlen(RST));
    huart2.Instance->CR3 |= USART_CR3_DMAT;

  HAL_Delay(3000);

  HAL_DMA_Start_IT(&hdma_usart2_tx,  (uint32_t)CIPSTART,  (uint32_t)&huart2.Instance->DR, strlen(CIPSTART));
  huart2.Instance->CR3 |= USART_CR3_DMAT;

  /* Infinite loop */

  HAL_UART_Receive_DMA(&huart2, rx_circular_buffer, size_of_rx_circular_buffer);

  while (1)
  {
	  //txBuffer[1] = USART_TX_Pin;
	  //trace_write((char*)txBuffer, 20);
	  //rxBuffer[1] = USART_RX_Pin;
	  //trace_write((char*)rxBuffer, 20);

	  if(HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_13) != GPIO_PIN_SET)
	  {

		  HAL_Delay(1000);
		  HAL_DMA_Start_IT(&hdma_usart2_tx,  (uint32_t)CIPSEND,  (uint32_t)&huart2.Instance->DR, strlen(CIPSEND));
		  huart2.Instance->CR3 |= USART_CR3_DMAT;
		  trace_write((char*)CIPSEND, 13);
		  HAL_Delay(2000);
		  HAL_DMA_Start_IT(&hdma_usart2_tx,  (uint32_t)msg,  (uint32_t)&huart2.Instance->DR, strlen(msg));
		  huart2.Instance->CR3 |= USART_CR3_DMAT;
		  trace_write((char*)msg, 5);

		  trace_write((char*)rx_circular_buffer, 20);


	  }

  }
}

void DMATransferComplete(DMA_HandleTypeDef *hdma) {
  if(hdma->Instance == DMA1_Stream6 && hdma->Init.Channel == DMA_CHANNEL_4) {
    //Disable UART DMA mode
    huart2.Instance->CR3 &= ~USART_CR3_DMAT;
    //Turn LD2 ON
    //HAL_GPIO_WritePin(LD2_GPIO_Port, LD2_Pin, GPIO_PIN_SET);
  }
}

void DMAReceivingComplete(DMA_HandleTypeDef *hdma) {
	if(hdma->Instance == DMA1_Stream5 && hdma->Init.Channel == DMA_CHANNEL_4){
		//Disable UART DMA mode
		huart2.Instance->CR3 &= ~USART_CR3_DMAT;
		//Turn LD2 ON
		HAL_GPIO_WritePin(LD2_GPIO_Port, LD2_Pin, GPIO_PIN_SET);
	}
}

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
