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
   rtcbackupRegWrite(1, rtcbackupRegRead(1) + 1);
   delay(500);
   reset_count = rtcbackupRegRead(1);

 }

 rtcbackupRegWrite(1, 0);


 if (reset_count != 2)
 {
   void (**jump_func)() = (void (**)())(0x8006000 + 4);

   if ((uint32_t)(*jump_func) != 0xFFFFFFFF)
   {
     HAL_RCC_DeInit();
     HAL_DeInit();

     for (int i=0; i<8; i++)
     {
       NVIC->ICER[i] = 0xFFFFFFFF;
       __DSB();
       __ISB();
     }
     SysTick->CTRL = 0;

     (*jump_func)();
   }
 }


  return ret;

}

uint32_t resetGetCount(void)
{
  return reset_count;
}
