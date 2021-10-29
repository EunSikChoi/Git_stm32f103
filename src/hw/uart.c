/*
 * uart.c
 *
 *  Created on: 2021. 10. 29.
 *      Author: 82109
 */






#include "uart.h"


static bool is_open[UART_MAX_CH];


extern uint32_t cdcAvailable(void);
extern uint8_t cdcRead(void);
extern void cdcDataIn(uint8_t rx_data);
extern uint32_t cdcWrite(uint8_t *p_data, uint32_t length);
extern uint32_t cdcGetBaud(void);




bool uartInit(void)
{
  for (int i =0; i < UART_MAX_CH; i++)
  {
     is_open[i] = false;
  }

  return true;
}


bool uartOpen(uint8_t ch, uint32_t baud)
{

  bool ret = false;
  switch(ch)
  {
    case _DEF_UART1:
    is_open[ch] = true;
    ret = true;
    break;
  }

  return ret;
}


uint32_t uartAvailable(uint8_t ch)
{
  uint32_t ret = 0;

  switch(ch)
  {
    case _DEF_UART1:
    ret = cdcAvailable();
    break;
  }

  return ret;
}


uint8_t  uartRead(uint8_t ch)
{
  uint8_t ret = 0;

   switch(ch)
   {
     case _DEF_UART1:
     ret = cdcRead();
     break;
   }

   return ret;

}


uint32_t uartWrite(uint8_t ch, uint8_t *p_data, uint32_t length)
{
  uint32_t ret = 0;

  switch(ch)
  {
    case _DEF_UART1:
    ret = cdcWrite(p_data, length);
    break;
  }

  return ret;

}


uint32_t uartPrintf(uint8_t ch, char *fmt, ...)  // #include <stdarg.h> 추가해야 가능 //
{
  char buf[256];
  va_list args;  						// memory address , args 이름으로 포인트 설정 //
  int len;
  uint32_t ret;

  va_start(args, fmt); 					//arg에 fmt(가변크기) 만큼 크기 설정
  len = vsnprintf(buf, 256, fmt, args); // 가변인자 문자 출력  //(버퍼주소, 크키 , 가변인자크기, 가변인자 메모리주소)
  ret = uartWrite(ch, (uint8_t *)buf, len);

  va_end(args); 						//args 목록 초기화


  return ret;

}

uint32_t uartGetBaud(uint8_t ch)
{
  uint32_t ret = 0;

  switch(ch)
  {
    case _DEF_UART1:
    ret = cdcGetBaud();
    break;
  }

  return ret;


}
