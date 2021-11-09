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
  uartOpen(_DEF_UART1, 57600);
  uartOpen(_DEF_UART2, 57600);
}


void apMain(void)
{
    uint32_t pre_time;
    uint32_t led_blink_time = 1000;

    pre_time = millis();

    if(resetGetCount() >= 2)
    {
      led_blink_time = 200;
    }

	while(1)
	{

	  if(millis()-pre_time >= led_blink_time)
	  {
		pre_time = millis();
		HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_13);

		if(GPIOC->ODR & (1<<13))
		{
			uartPrintf(_DEF_UART1, "TOGGLE: %d\n", pre_time);
		}



	  }

	}


}