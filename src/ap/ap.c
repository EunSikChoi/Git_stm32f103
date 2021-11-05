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


        if(rx_data == '1')  //READ
        {
           uint8_t buf[32];

           logPrintf("Read...\n");

           flashRead(FLASH_USER_START_ADDR , buf, FLASH_USER_LENGTH); //LEN 단위 : 1바이트 크기
           //flashRead(0x8000000 + (60*1024) , buf, 32);
           for ( int i = 0; i < 32 ; i++)
           {
            // logPrintf("0x%X : 0x%X\n", 0x8000000 + (60*1024) + i, buf[i]);
             logPrintf("0x%X : 0x%X\n", (int32_t)FLASH_USER_START_ADDR + i, buf[i]);
           }

           rx_data = 0;

        }
        if(rx_data == '2')  // ERASE
        {
          logPrintf("Erase...\n");

          if(flashErase(FLASH_USER_START_ADDR , FLASH_USER_LENGTH) == true)
          //if(flashErase(0x8000000 + (60*1024) , 32) == true)
          {
            logPrintf("Erase OK\n");
          }
          else
          {
            logPrintf("Erase Fail\n");
          }

          rx_data = 0;

        }
        if (rx_data == '3')  // WRITE
        {
          uint8_t buf[32];

          for (int i=0; i<32; i++) //써야할 데이터 32바이트 생성
          {
            buf[i] = i;
          }

          logPrintf("Write...\n");

          if (flashWrite(FLASH_USER_START_ADDR, buf, FLASH_USER_LENGTH) == true)
          //if (flashWrite(0x8000000 + (60*1024), buf, 32) == true)
          {
            logPrintf("Wrtie OK\n");
          }
          else
          {
            logPrintf("Write Fail\n");
          }

          rx_data = 0;
        }


      }

	}


}
