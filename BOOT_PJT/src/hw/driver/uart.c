/*
 * uart.c
 *
 *  Created on: 2021. 10. 29.
 *      Author: 82109
 */






#include "uart.h"
#include "cdc.h"
#include "qbuffer.h"


static bool is_open[UART_MAX_CH];

static qbuffer_t qbuffer[UART_MAX_CH];

static uint8_t rx_buf[256]; 	 	  // qbuffer buf
//static uint8_t rx_data[UART_MAX_CH];  // rx INT buf

UART_HandleTypeDef huart1;
DMA_HandleTypeDef hdma_usart1_rx;


bool uartInit(void)
{
  for (int i =0; i < UART_MAX_CH; i++)
  {
     is_open[i] = false;
  }

  return true;
}


bool uartOpen(uint8_t ch, uint32_t baud)
{

  bool ret = false;
  switch(ch)
  {
    case _DEF_UART1:
	  is_open[ch] = true;
	  ret = true;
	break;

    case _DEF_UART2:
	  huart1.Instance = USART1;
	  huart1.Init.BaudRate = baud;
	  huart1.Init.WordLength = UART_WORDLENGTH_8B;
	  huart1.Init.StopBits = UART_STOPBITS_1;
	  huart1.Init.Parity = UART_PARITY_NONE;
	  huart1.Init.Mode = UART_MODE_TX_RX;
	  huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
	  huart1.Init.OverSampling = UART_OVERSAMPLING_16;

	  HAL_UART_DeInit(&huart1);

      qbufferCreate(&qbuffer[_DEF_UART2], &rx_buf[0], 256);

      __HAL_RCC_DMA1_CLK_ENABLE();

      /* DMA interrupt init */
      /* DMA1_Channel5_IRQn interrupt configuration */
      HAL_NVIC_SetPriority(DMA1_Channel5_IRQn, 5, 0);
      HAL_NVIC_EnableIRQ(DMA1_Channel5_IRQn);


      if (HAL_UART_Init(&huart1) != HAL_OK)
      {
        ret = false;
      }
      else
      {
        ret = true;
        is_open[ch] = true;

        if(HAL_UART_Receive_DMA(&huart1, (uint8_t *)&rx_buf[0], 256) != HAL_OK)
        {
          ret = false;
        }
        // update in out //
        qbuffer[ch].in = qbuffer[ch].len - hdma_usart1_rx.Instance->CNDTR;
        qbuffer[ch].out =  qbuffer[ch].in;

      }
      break;

  }

  return ret;
}


uint32_t uartAvailable(uint8_t ch)
{
  uint32_t ret = 0;

  switch(ch)
  {
    case _DEF_UART1:
    	ret = cdcAvailable();
    break;

    case _DEF_UART2:
    	// DMA는 자체 레지스터에서 데이터 수신하면 CNT감소하므로, 별도의 in++ (qbufferWrite) 가 필요 없음 //
        qbuffer[ch].in = ( qbuffer[ch].len - hdma_usart1_rx.Instance->CNDTR); // update in cnt //
        ret = qbufferAvailable(&qbuffer[ch]);
	break;
  }

  return ret;
}


uint8_t  uartRead(uint8_t ch)
{
  uint8_t ret = 0;

   switch(ch)
   {
     case _DEF_UART1:
       ret = cdcRead();
     break;

     case _DEF_UART2:
       qbufferRead(&qbuffer[ch], &ret, 1);
     break;

   }

   return ret;

}


uint32_t uartWrite(uint8_t ch, uint8_t *p_data, uint32_t length)
{
  uint32_t ret = 0;
  HAL_StatusTypeDef status;

  switch(ch)
  {
    case _DEF_UART1:
    //	ret = cdcWrite(p_data, length); // not use in DFU
    break;

    case _DEF_UART2:
	  status =  HAL_UART_Transmit(&huart1, p_data, length, 100);

	  if(status == HAL_OK)
	  {
		ret = length;
	  }

	break;
  }

  return ret;

}


uint32_t uartPrintf(uint8_t ch, char *fmt, ...)  // #include <stdarg.h> 추가해야 가능 //
{
  char buf[256];
  va_list args;  						// memory address , args 이름으로 포인트 설정 // 포인터 변수 생성
  int len;
  uint32_t ret;

  va_start(args, fmt); 					// 가변인자중 첫번째 인자의 주소를 가르킴 // 즉 시작 주소 지정 //
  len = vsnprintf(buf, 256, fmt, args); // 가변인자 문자 출력  //(fmt에 만들어진 내용이 담길 버퍼, 최대크키 , 포멧, 가변인자 시작주소) // 리턴값은 길이 //
  ret = uartWrite(ch, (uint8_t *)buf, len);

  va_end(args); 						//args 목록 초기화

  return ret;

}

uint32_t uartGetBaud(uint8_t ch)
{
  uint32_t ret = 0;

  switch(ch)
  {
    case _DEF_UART1:
      ret = cdcGetBaud();
    break;

    case _DEF_UART2:
	  ret = huart1.Init.BaudRate;
	break;
  }

  return ret;


}



void HAL_UART_ErrorCallback(UART_HandleTypeDef *huart)
{
  if(huart->Instance == USART1)
  {

  }

}


void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
#if 0
  if(huart->Instance == USART1)
  {
    qbufferWrite(&qbuffer[_DEF_UART2], &rx_data[_DEF_UART2] , 1);

    HAL_UART_Receive_IT(&huart1, (uint8_t *)&rx_data[_DEF_UART2], 1) ; // Re enable IT

  }
#endif
}

void HAL_UART_MspInit(UART_HandleTypeDef* uartHandle)
{

  GPIO_InitTypeDef GPIO_InitStruct = {0};
  if(uartHandle->Instance==USART1)
  {
  /* USER CODE BEGIN USART1_MspInit 0 */

  /* USER CODE END USART1_MspInit 0 */
    /* USART1 clock enable */
    __HAL_RCC_USART1_CLK_ENABLE();

    __HAL_RCC_GPIOA_CLK_ENABLE();
    /**USART1 GPIO Configuration
    PA9     ------> USART1_TX
    PA10     ------> USART1_RX
    */
    GPIO_InitStruct.Pin = GPIO_PIN_9;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = GPIO_PIN_10;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    /* USART1 DMA Init */
    /* USART1_RX Init */
    hdma_usart1_rx.Instance = DMA1_Channel5;
    hdma_usart1_rx.Init.Direction = DMA_PERIPH_TO_MEMORY;
    hdma_usart1_rx.Init.PeriphInc = DMA_PINC_DISABLE;
    hdma_usart1_rx.Init.MemInc = DMA_MINC_ENABLE;
    hdma_usart1_rx.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
    hdma_usart1_rx.Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;
    hdma_usart1_rx.Init.Mode = DMA_CIRCULAR;
    hdma_usart1_rx.Init.Priority = DMA_PRIORITY_LOW;
    if (HAL_DMA_Init(&hdma_usart1_rx) != HAL_OK)
    {
      Error_Handler();
    }

    __HAL_LINKDMA(uartHandle,hdmarx,hdma_usart1_rx);

    /* USART1 interrupt Init */
    HAL_NVIC_SetPriority(USART1_IRQn, 3, 0);
    HAL_NVIC_EnableIRQ(USART1_IRQn);
  /* USER CODE BEGIN USART1_MspInit 1 */

  /* USER CODE END USART1_MspInit 1 */
  }
}

void HAL_UART_MspDeInit(UART_HandleTypeDef* uartHandle)
{

  if(uartHandle->Instance==USART1)
  {
  /* USER CODE BEGIN USART1_MspDeInit 0 */

  /* USER CODE END USART1_MspDeInit 0 */
    /* Peripheral clock disable */
    __HAL_RCC_USART1_CLK_DISABLE();

    /**USART1 GPIO Configuration
    PA9     ------> USART1_TX
    PA10     ------> USART1_RX
    */
    HAL_GPIO_DeInit(GPIOA, GPIO_PIN_9|GPIO_PIN_10);

    /* USART1 DMA DeInit */
    HAL_DMA_DeInit(uartHandle->hdmarx);

    /* USART1 interrupt Deinit */
    HAL_NVIC_DisableIRQ(USART1_IRQn);
  /* USER CODE BEGIN USART1_MspDeInit 1 */

  /* USER CODE END USART1_MspDeInit 1 */
  }
}

