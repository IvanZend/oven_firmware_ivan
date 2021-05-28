
// #pragma once

// // #ifndef NET_H_
// // #define NET_H_
// //-----------------------------------------------
// // #include "../MarlinCore.h"
// // #include "../inc/MarlinConfig.h"
// // #include "../inc/MarlinConfigPre.h"
// // #include "../MarlinCore.h"
// #include "stm32f4xx_hal.h"
// #include <string.h>
// #include <stdlib.h>
// #include <stdint.h>
// #include "lwip.h"
// #include "lwip/tcp.h"
// // #include "../gcode/queue.h"


// #ifdef __cplusplus
//  extern "C" {
// #endif 

// struct TEST_server_struct
// {
//   u8_t state; /* current connection state */
//   u8_t retries;
//   struct tcp_pcb *pcb; /* pointer on the current tcp_pcb */
//   struct pbuf *p; /* pointer on the received/to be transmitted pbuf */
// };
// //-----------------------------------------------
// void TEST_net_ini(void);
// void TEST_tcp_server_init(void);
// void TEST_UART6_RxCpltCallback(void);
// //-----------------------------------------------
// typedef struct TEST_USART_prop{
//   uint8_t usart_buf[26];
//   uint8_t usart_cnt;
//   uint8_t is_tcp_connect;//статус попытки создать соединение TCP с сервером
//   uint8_t is_text;//статус попытки передать текст серверу
// } TEST_USART_prop_ptr;
// //-----------------------------------------------

// #ifdef __cplusplus
// }
// #endif

// // #endif /* NET_H_ */
