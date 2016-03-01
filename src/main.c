/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_hal.h"
#include "nucleo_hal_bsp.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
//#include <trace.h>

/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Private variables ---------------------------------------------------------*/
//extern UART_HandleTypeDef huart2;
UART_HandleTypeDef huart2;
DMA_HandleTypeDef hdma_usart2_tx;
DMA_HandleTypeDef hdma_usart2_rx;
char *msg = "Hello, Suk \r\n";
char *msg1 = "mmh...Heisse Schnitte!:-)\r\n";


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
  char     rxBuffer[32];

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
  hdma_usart2_rx.Init.Channel = DMA_CHANNEL_4; //check other channel
  hdma_usart2_rx.XferCpltCallback = &DMAReceivingComplete;
  HAL_DMA_Init(&hdma_usart2_rx);

  uint8_t data[20];
  uint8_t error;


  /*-------------- END: hdma_usart2_rx ------------------------------------------------*/

  // DMA interrupt init
  HAL_NVIC_SetPriority(DMA1_Stream6_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(DMA1_Stream6_IRQn);


  HAL_DMA_Start_IT(&hdma_usart2_tx,  (uint32_t)msg,  (uint32_t)&huart2.Instance->DR, strlen(msg));
  huart2.Instance->CR3 |= USART_CR3_DMAT;

  /* Infinite loop */

  //HAL_DMA_Start_IT(&hdma_usart2_rx, (uint8_t)data, (int32_t)&huart2. )


  //HAL_DMA_Start(&hdma_usart2_rx, (uint32_t)&huart2.Instance->DR, (uint32_t)&LD2_GPIO_Port->ODR, 1);



  while (1)
  {

	  if(HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_13) != GPIO_PIN_SET)
	  {

		  HAL_Delay(1000);
		  HAL_DMA_Start_IT(&hdma_usart2_tx,  (uint32_t)msg1,  (uint32_t)&huart2.Instance->DR, strlen(msg1));
		  huart2.Instance->CR3 |= USART_CR3_DMAT;

		  /*
		  HAL_DMA_Start(&hdma_usart2_rx, (uint32_t)&huart2.Instance->DR, (uint32_t)&LD2_GPIO_Port->ODR, 1);
		  //Enable UART in DMA Mode
		  huart2.Instance->CR3 |= USART_CR3_DMAR;
		  */
		  //error = HAL_UART_Receive_IT(&huart2, (uint8_t *) &rxBuffer, 4);
		  //error = HAL_UART_Receive_DMA(&huart2, (uint8_t *) &rxBuffer, 4);
		  //data = rxBuffer;
		  //trace_printf("Error: %d\n", error);
		  HAL_DMA_Start_IT(&hdma_usart2_rx, (uint32_t)&huart2.Instance->DR, (uint32_t)&rxBuffer, strlen(rxBuffer));
		  //Enable UART in DMA Mode
		  huart2.Instance->CR3 |= USART_CR3_DMAR;
		  trace_printf(rxBuffer);
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
