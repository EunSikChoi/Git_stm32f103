/*
 * rtc.h
 *
 *  Created on: 2021. 11. 5.
 *      Author: 82109
 */

#ifndef SRC_COMMON_HW_RTC_H_
#define SRC_COMMON_HW_RTC_H_


#include "hw_def.h"

#ifdef _USE_HW_RTC

bool 	rtcInit(void);
void 	rtcbackupRegWrite(uint32_t index , uint32_t data);
uint32_t rtcbackupRegRead(uint32_t index);

#endif


#endif /* SRC_COMMON_HW_RTC_H_ */
