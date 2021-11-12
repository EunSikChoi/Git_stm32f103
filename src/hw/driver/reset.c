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
  bool ret = true;
  bool is_debug = false;


  // debug is connected // from cmsis //
  //
  if (CoreDebug->DHCSR & CoreDebug_DHCSR_C_DEBUGEN_Msk)
  {
    is_debug = true;
  }

  // if Reset key push // reset key not operate if debug is connected,
  //
  if (RCC->CSR & (1<<26) && is_debug != true)
  {
	rtcbackupRegWrite(1, rtcbackupRegRead(1) + 1);
    delay(500);
    reset_count = rtcbackupRegRead(1); // SAVE reset cnt
  }

  rtcbackupRegWrite(1, 0); // reset backup resister //


  return ret;
}

void resetToSysBoot(void)
{
  void (*SysMemBootJump)(void);
  volatile uint32_t addr = 0x1FFF0000;


  HAL_RCC_DeInit();
  // reset systick
  SysTick->CTRL = 0;
  SysTick->LOAD = 0;
  SysTick->VAL  = 0;

  // reset interuppt
  for (int i=0;i<8;i++)
  {
    NVIC->ICER[i]=0xFFFFFFFF; // clear register
    NVIC->ICPR[i]=0xFFFFFFFF; // pending register
    __DSB();
    __ISB();
  }

  //set jump adder
  SysMemBootJump = (void (*)(void)) (*((uint32_t *)(addr + 4)));

  //set main stack pointer
  __set_MSP(*(uint32_t *)addr);

  // jump to set addr
  SysMemBootJump();
}


uint32_t resetGetCount(void)
{
  return reset_count;
}
