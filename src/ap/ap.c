/*
 * ap.c
 *
 *  Created on: Oct 27, 2021
 *      Author: 82109
 */


#include "ap.h"
#include "usbd_cdc_if.h"

extern uint32_t cdcAvailable(void);
extern uint8_t cdcRead(void);
extern void cdcDataIn(uint8_t rx_data);
extern uint32_t cdcWrite(uint8_t *p_data, uint32_t length);


void apInit(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOD_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, GPIO_PIN_RESET);

  /*Configure GPIO pin : PC13 */
  GPIO_InitStruct.Pin = GPIO_PIN_13;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  uartOpen(_DEF_UART1, 57600);
  uartOpen(_DEF_UART2, 57600);

}


void apMain(void)
{
    uint32_t pre_time;
    uint32_t pre_baud;


    pre_baud = uartGetBaud(_DEF_UART1);
    pre_time = millis();

	while(1)
	{

	  if(millis()-pre_time >= 1000)
	  {
		pre_time = millis();
		HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_13);
	  }

	  if(uartAvailable(_DEF_UART1) > 0)
	  {
		uint8_t rx_data;

		rx_data = uartRead(_DEF_UART1);
		uartPrintf(_DEF_UART1, "RxData : %c 0x%x\n", rx_data, rx_data);

	  }

      if(uartAvailable(_DEF_UART2) > 0)
      {
         uint8_t rx_data;

         rx_data = uartRead(_DEF_UART2);
         uartPrintf(_DEF_UART2, "UART2 Rx: %c %x\n", rx_data, rx_data);
      }


	  if(uartGetBaud(_DEF_UART1) != pre_baud)
	  {
		pre_baud = uartGetBaud(_DEF_UART1);
		uartPrintf(_DEF_UART1, "ChangedBaud : %d\n", uartGetBaud(_DEF_UART1));
	  }

	}


}
