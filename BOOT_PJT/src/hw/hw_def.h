/*
 * hw_def.h
 *
 *  Created on: Oct 27, 2021
 *      Author: 82109
 */

#ifndef SRC_HW_HW_DEF_H_
#define SRC_HW_HW_DEF_H_



#include "def.h"
#include "bsp.h"

#define FLASH_USER_START_ADDR (uint32_t)0x0800EFEA
#define FLASH_USER_LENGTH 32


#define _USE_HW_CDC
#define _USE_HW_FLASH
#define _USE_HW_LED
#define _USE_HW_RTC
#define _USE_HW_RESET
#define _USE_HW_USB

#define _USE_HW_UART
#define      HW_UART_MAX_CH          2





#endif /* SRC_HW_HW_DEF_H_ */
