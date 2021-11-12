/*
 * hw.c
 *
 *  Created on: Oct 27, 2021
 *      Author: 82109
 */

#include "hw.h"
#include "ap.h"

void hwInit(void)
{

  bspInit();
  resetInit();

  rtcInit();



  ledInit();
  usbInit();

  uartInit();

  cliInit();

  flashInit();

}
