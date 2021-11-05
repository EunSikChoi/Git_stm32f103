/*
 * rtc.c
 *
 *  Created on: 2021. 11. 5.
 *      Author: 82109
 */


#include "rtc.h"

RTC_HandleTypeDef hrtc;

bool rtcInit(void)
{
  bool ret = true;

  hrtc.Instance = RTC;
  hrtc.Init.AsynchPrediv = RTC_AUTO_1_SECOND;
  hrtc.Init.OutPut = RTC_OUTPUTSOURCE_NONE;

  if (HAL_RTC_Init(&hrtc) != HAL_OK)
  {
    Error_Handler();
  }

  return ret;
}

void rtcbackupRegWrite(uint32_t index , uint32_t data)
{
	HAL_RTCEx_BKUPWrite(&hrtc, index, data);
}

uint32_t rtcbackupRegRead(uint32_t index)
{
	return HAL_RTCEx_BKUPRead(&hrtc, index);
}

void HAL_RTC_MspInit(RTC_HandleTypeDef* rtcHandle)
{

  if(rtcHandle->Instance==RTC)
  {

    HAL_PWR_EnableBkUpAccess();

    __HAL_RCC_BKP_CLK_ENABLE();

    __HAL_RCC_RTC_ENABLE();

  }
}

void HAL_RTC_MspDeInit(RTC_HandleTypeDef* rtcHandle)
{

  if(rtcHandle->Instance==RTC)
  {
    __HAL_RCC_RTC_DISABLE();

  }
}
