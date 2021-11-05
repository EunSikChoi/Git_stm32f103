/*
 * reset.c
 *
 *  Created on: 2021. 11. 5.
 *      Author: 82109
 */



#include "reset.h"
#include "rtc.h"

static uint32_t reset_count = 0;

bool resetInit(void)
{
  bool  ret = true;

 //Reset 핀이 눌리면//
 if(RCC->CSR & (1<<26))
 {
   rtcbackupRegWrite(1, rtcbackupRegRead(1) + 1); // 1- 10번 주소에 Write 가능 //
   delay(500);
   reset_count = rtcbackupRegRead(1);

 }

 rtcbackupRegWrite(1, 0);

 return ret;

}

uint32_t resetGetCount(void)
{
  return reset_count;
}
