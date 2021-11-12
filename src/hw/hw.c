/*
 * hw.c
 *
 *  Created on: Oct 27, 2021
 *      Author: 82109
 */

#include "hw.h"

void hwInit(void)
{

  bspInit();
  rtcInit();
  resetInit();

  ledInit();
  usbInit();

  if( resetGetCount() == 2)
  {
    //jump to system bootloader //  시스템 부트 로더 실행 //

	uartPrintf(_DEF_UART1, "OKOK\n");
    resetToSysBoot();

  }

  uartInit();

  flashInit();

}
