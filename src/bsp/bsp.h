/*
 * bsp.h
 *
 *  Created on: Oct 27, 2021
 *      Author: 82109
 */

#ifndef SRC_BSP_BSP_H_
#define SRC_BSP_BSP_H_


#include "def.h"
#include "stm32f1xx_hal.h"
#include "usb_device.h"

void bspInit(void);
void delay(uint32_t ms);
uint32_t millis(void);

void Error_Handler(void);



#endif /* SRC_BSP_BSP_H_ */
