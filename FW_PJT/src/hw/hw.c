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

  uartInit();

  flashInit();

}
