/*
 * cdc.h
 *
 *  Created on: 2021. 11. 1.
 *      Author: 82109
 */

#ifndef SRC_COMMON_CDC_H_
#define SRC_COMMON_CDC_H_



#include "hw_def.h"

#ifdef _USE_HW_CDC

bool     cdcInit(void);
uint8_t  cdcRead(void);
uint8_t  CDC_Transmit_FS(uint8_t* Buf, uint16_t Len);
uint32_t cdcWrite(uint8_t *p_data, uint32_t length);
uint32_t cdcGetBaud(void);
uint32_t cdcAvailable(void);

void     cdcDataIn(uint8_t rx_data);
#endif


#endif /* SRC_COMMON_CDC_H_ */
