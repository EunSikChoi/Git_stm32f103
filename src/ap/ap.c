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

}


void apMain(void)
{
	  uint32_t pre_time;

	    pre_time = millis();

		while(1)
		{

		  if(millis()-pre_time >= 1000)
		  {
			pre_time = millis();
			HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_13);
		  }

		  if(cdcAvailable() > 0)
		  {
			uint8_t rx_data;

			rx_data = cdcRead();
			cdcWrite((uint8_t *)"RxData : ", 10);
			cdcWrite(&rx_data, 1);
			cdcWrite((uint8_t *)"\n", 2);
		  }

		}


}
