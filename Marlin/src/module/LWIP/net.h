
#pragma once

#ifndef NET_H_
#define NET_H_
//-----------------------------------------------
// #include "../MarlinCore.h"
// #include "../inc/MarlinConfig.h"
// #include "../inc/MarlinConfigPre.h"
// #include "../MarlinCore.h"
#include "stm32f4xx_hal.h"
#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include "lwip.h"
#include "lwip/tcp.h"
// #include "../gcode/queue.h"


#ifdef __cplusplus
 extern "C" {
#endif 

struct server_struct
{
  u8_t state; /* current connection state */
  u8_t retries;
  struct tcp_pcb *pcb; /* pointer on the current tcp_pcb */
  struct pbuf *p; /* pointer on the received/to be transmitted pbuf */
};
//-----------------------------------------------
void net_ini(void);
void tcp_server_init(void);
void UART6_RxCpltCallback(void);
void sendstring(char* buf_str);
void ETHsendchar(char* buf_str);
int net_data_read(void);
int net_data_available(void);
//-----------------------------------------------
typedef struct USART_prop{
  uint8_t usart_buf[26];
  uint8_t usart_cnt;
  uint8_t is_tcp_connect;//статус попытки создать соединение TCP с сервером
  uint8_t is_text;//статус попытки передать текст серверу
} USART_prop_ptr;
//-----------------------------------------------

#ifdef __cplusplus
}
#endif

#endif /* NET_H_ */
