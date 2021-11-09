/*
 * usb.c
 *
 *  Created on: 2021. 11. 5.
 *      Author: 82109
 */


#include "usb.h"
#include "usb_device.h"

bool usbInit(void)
{
  bool ret = true;

  __HAL_RCC_GPIOA_CLK_ENABLE();// A 클럭은 USB_CDC 이기 떄문에 여기서 활성화 안하면 아래 초기화 수행 안됨 //

  GPIO_InitTypeDef GPIO_InitStruct = {0};

  GPIO_InitStruct.Pin   = GPIO_PIN_12;
  GPIO_InitStruct.Mode  = GPIO_MODE_OUTPUT_PP;  //OUTPUT
  GPIO_InitStruct.Pull  = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_12, GPIO_PIN_RESET); //SET LOW
  delay(100);
  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_12, GPIO_PIN_SET);//SET HIGH

  GPIO_InitStruct.Mode  = GPIO_MODE_INPUT;//INPUT
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);


  MX_USB_DEVICE_Init();

  return ret;
}
