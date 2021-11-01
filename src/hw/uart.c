/*
 * uart.c
 *
 *  Created on: 2021. 10. 29.
 *      Author: 82109
 */






#include "uart.h"
#include "cdc.h"


static bool is_open[UART_MAX_CH];

UART_HandleTypeDef huart1;

//extern uint32_t cdcAvailable(void);
//extern uint8_t cdcRead(void);
//extern void cdcDataIn(uint8_t rx_data);
//extern uint32_t cdcWrite(uint8_t *p_data, uint32_t length);
//extern uint32_t cdcGetBaud(void);




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

	  if (HAL_UART_Init(&huart1) != HAL_OK)
	  {
		ret = false;
	  }
	  else
	  {
		ret = true;
	    is_open[ch] = true;
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
    ret = cdcWrite(p_data, length);
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
  va_list args;  						// memory address , args 이름으로 포인트 설정 //
  int len;
  uint32_t ret;

  va_start(args, fmt); 					//arg에 fmt(가변크기) 만큼 크기 설정
  len = vsnprintf(buf, 256, fmt, args); // 가변인자 문자 출력  //(버퍼주소, 크키 , 가변인자크기, 가변인자 메모리주소)
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
  }

  return ret;


}


void HAL_UART_MspInit(UART_HandleTypeDef* uartHandle)
{

  GPIO_InitTypeDef GPIO_InitStruct = {0};
  if(uartHandle->Instance==USART1)
  {
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

    /* USART1 interrupt Init */
    HAL_NVIC_SetPriority(USART1_IRQn, 3, 0);
    HAL_NVIC_EnableIRQ(USART1_IRQn);

  }
}

void HAL_UART_MspDeInit(UART_HandleTypeDef* uartHandle)
{

  if(uartHandle->Instance==USART1)
  {

    /* Peripheral clock disable */
    __HAL_RCC_USART1_CLK_DISABLE();

    /**USART1 GPIO Configuration
    PA9     ------> USART1_TX
    PA10     ------> USART1_RX
    */
    HAL_GPIO_DeInit(GPIOA, GPIO_PIN_9|GPIO_PIN_10);

    /* USART1 interrupt Deinit */
    HAL_NVIC_DisableIRQ(USART1_IRQn);

  }
}