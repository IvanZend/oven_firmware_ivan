// // #include "../inc/MarlinConfig.h"
// #include "test.h"
// #include "../gcode/queue.h"
// //-----------------------------------------------
// uint8_t TEST_ipaddr_dest[4];
// uint16_t TEST_port_dest;
// extern UART_HandleTypeDef huart2;
// extern GCodeQueue queue;
// TEST_USART_prop_ptr TEST_usartprop;
// char TEST_str[30];
// char TEST_str1[100];
// u8_t TEST_data[100];
// struct tcp_pcb *TEST_server_pcb;
// __IO uint32_t TEST_message_count=0;
// //-----------------------------------------------
// enum TEST_server_states
// {
//   ES_NONE = 0,
//   ES_ACCEPTED,
//   ES_RECEIVED,
//   ES_CLOSING
// };
// //-----------------------------------------------
// struct TEST_server_struct *TEST_ss;
// //-----------------------------------------------
// static err_t TEST_tcp_server_accept(void *arg, struct tcp_pcb *newpcb, err_t err);
// static void TEST_tcp_server_connection_close(struct tcp_pcb *tpcb, struct TEST_server_struct *es);
// static void TEST_tcp_server_error(void *arg, err_t err);
// static err_t TEST_tcp_server_recv(void *arg, struct tcp_pcb *tpcb, struct pbuf *p, err_t err);
// static void TEST_tcp_server_send(struct tcp_pcb *tpcb, struct TEST_server_struct *es);
// static err_t TEST_tcp_server_sent(void *arg, struct tcp_pcb *tpcb, u16_t len);
// static err_t TEST_tcp_server_poll(void *arg, struct tcp_pcb *tpcb);
// //-----------------------------------------------
// void TEST_tcp_server_init(void)
// {
// 	  TEST_server_pcb = tcp_new();
// 	  if (TEST_server_pcb != NULL)
// 	  {
// 	    err_t err;
// 	    err = tcp_bind(TEST_server_pcb, IP_ADDR_ANY, 7);
// 	    if (err == ERR_OK)
// 	    {
// 	      TEST_server_pcb = tcp_listen(TEST_server_pcb);
// 	      tcp_accept(TEST_server_pcb, TEST_tcp_server_accept);
// 	    }
// 	    else
// 	    {
// 	      memp_free(MEMP_TCP_PCB, TEST_server_pcb);
// 	    }
// 	  }
// }
// //----------------------------------------------------------
// static err_t TEST_tcp_server_accept(void *arg, struct tcp_pcb *newpcb, err_t err)
// {
//   err_t ret_err;
//   struct TEST_server_struct *es;
//   LWIP_UNUSED_ARG(arg);
//   LWIP_UNUSED_ARG(err);
//   tcp_setprio(newpcb, TCP_PRIO_MIN);
//   es = (struct TEST_server_struct *)mem_malloc(sizeof(struct TEST_server_struct));
//   TEST_ss = (struct TEST_server_struct *)mem_malloc(sizeof(struct TEST_server_struct));
//   if (es != NULL)
//   {
//     es->state = ES_ACCEPTED;
//     es->pcb = newpcb;
//     TEST_ss->pcb = newpcb;
//     es->retries = 0;
//     es->p = NULL;
//     tcp_arg(newpcb, es);
//     tcp_recv(newpcb, TEST_tcp_server_recv);
//     tcp_err(newpcb, TEST_tcp_server_error);
//     tcp_sent(newpcb, TEST_tcp_server_sent);
//     tcp_poll(newpcb, TEST_tcp_server_poll, 0);
//     ret_err = ERR_OK;
//   }
//   else
//   {
//     TEST_tcp_server_connection_close(newpcb, es);
//     ret_err = ERR_MEM;
//   }
//   return ret_err;
// }
// //----------------------------------------------------------
// static void TEST_tcp_server_connection_close(struct tcp_pcb *tpcb, struct TEST_server_struct *es)
// {
//   // remove all callbacks
//   tcp_arg(tpcb, NULL);
//   tcp_sent(tpcb, NULL);
//   tcp_recv(tpcb, NULL);
//   tcp_err(tpcb, NULL);
//   tcp_poll(tpcb, NULL, 0);
//   if (es != NULL)
//   {
//     mem_free(es);
//   }
//   tcp_close(tpcb);
// }
// //----------------------------------------------------------
// static void TEST_tcp_server_error(void *arg, err_t err)
// {
// }
// //-----------------------------------------------
// static err_t TEST_tcp_server_recv(void *arg, struct tcp_pcb *tpcb, struct pbuf *p, err_t err)
// {
// 	struct TEST_server_struct *es;
// 	err_t ret_err;
// 	LWIP_ASSERT("arg != NULL",arg != NULL);
// 	es = (struct TEST_server_struct *)arg;
// 	if (p == NULL)
// 	{
// 		es->state = ES_CLOSING;
// 		if(es->p == NULL)
// 		{
// 			tcp_recved(tpcb, p->tot_len);
// 		}
// 		else
// 		{
// 			//acknowledge received packet
// 			tcp_sent(tpcb, TEST_tcp_server_sent);
// 			//send remaining data
// 			TEST_tcp_server_send(tpcb, es);
// 		}
// 		ret_err = ERR_OK;
// 	}
// 	else if(err != ERR_OK)
// 	{
// 		  if (p != NULL)
// 		  {
// 		    es->p = NULL;
// 		    pbuf_free(p);
// 		  }
// 		  ret_err = err;
// 	}
// 	else if(es->state == ES_ACCEPTED)
// 	{
// 		  tcp_recved(tpcb, p->tot_len);
// 		  strncpy(TEST_str1,(char*)(p->payload),p->len);
// 		  TEST_str1[p->len] = '\0';
// 		  HAL_UART_Transmit(&huart2, (uint8_t*)TEST_str1,strlen(TEST_str1),0x1000);
// 		  queue._enqueue(TEST_str1,true);
// 		  ret_err = ERR_OK;
// 	}
// 	else if (es->state == ES_RECEIVED)
// 	{
// 		  if(es->p == NULL)
// 		  {
// 		    ret_err = ERR_OK;
// 		  }
// 		  else
// 		  {
// 		    struct pbuf *ptr;
// 		    ptr = es->p;
// 		    pbuf_chain(ptr,p);
// 		  }
// 		  ret_err = ERR_OK;
// 	}
// 	else if(es->state == ES_CLOSING)
// 	{
// 		  tcp_recved(tpcb, p->tot_len);
// 		  es->p = NULL;
// 		  pbuf_free(p);
// 		  ret_err = ERR_OK;
// 	}
// 	else
// 	{
// 		  tcp_recved(tpcb, p->tot_len);
// 		  es->p = NULL;
// 		  pbuf_free(p);
// 		  ret_err = ERR_OK;
// 	}
// 	return ret_err;
// }
// //-----------------------------------------------
// static void TEST_tcp_server_send(struct tcp_pcb *tpcb, struct TEST_server_struct *es)
// {
//   struct pbuf *ptr;
//   err_t wr_err = ERR_OK;
//   while ((wr_err == ERR_OK) &&
//     (es->p != NULL) &&
//     (es->p->len <= tcp_sndbuf(tpcb)))
//   {
//     ptr = es->p;
//     wr_err = tcp_write(tpcb, ptr->payload, ptr->len, 1);
//     if (wr_err == ERR_OK)
//     {
//       u16_t plen;
//       u8_t freed;
//       plen = ptr->len;
//       es->p = ptr->next;
//       if(es->p != NULL)
//       {
//         pbuf_ref(es->p);
//       }
//       do
//       {
//         freed = pbuf_free(ptr);
//       }
//       while(freed == 0);
//       tcp_recved(tpcb, plen);
//     }
//     else if(wr_err == ERR_MEM)
//     {
//       es->p = ptr;
//     }
//     else
//     {
//       //other problem
//     }
//   }
// }
// //----------------------------------------------------------
// static err_t TEST_tcp_server_sent(void *arg, struct tcp_pcb *tpcb, u16_t len)
// {
// 	struct TEST_server_struct *es;
// 	LWIP_UNUSED_ARG(len);
// 	es = (struct TEST_server_struct *)arg;
// 	es->retries = 0;
// 	if(es->p != NULL)
// 	{
// 		TEST_tcp_server_send(tpcb, es);
// 	}
// 	else
// 	{
// 		if(es->state == ES_CLOSING)
// 			TEST_tcp_server_connection_close(tpcb, es);
// 	}
// 	return ERR_OK;
// }
// //-----------------------------------------------
// static err_t TEST_tcp_server_poll(void *arg, struct tcp_pcb *tpcb)
// {
// 	err_t ret_err;
// 	struct TEST_server_struct *es;
// 	es = (struct TEST_server_struct *)arg;
// 	HAL_GPIO_TogglePin(GPIOD, GPIO_PIN_14);
// 	if (es != NULL)
// 	{
// 	  if (es->p != NULL)
// 	  {
// 	  }
// 	  else
// 	  {
// 	    if(es->state == ES_CLOSING)
// 	    {
// 	      HAL_GPIO_WritePin(GPIOD, GPIO_PIN_14, GPIO_PIN_RESET);
// 	      HAL_GPIO_WritePin(GPIOD, GPIO_PIN_12, GPIO_PIN_SET);
// 	      TEST_tcp_server_connection_close(tpcb, es);
// 	    }
// 	    if(TEST_usartprop.is_text==1)
// 	    {
// 	      TEST_usartprop.is_text=0;
// 	    }
// 	  }
// 	  ret_err = ERR_OK;
// 	}
// 	else
// 	{
// 	  tcp_abort(tpcb);
// 	  ret_err = ERR_ABRT;
// 	}
// 	return ret_err;
// }
// //----------------------------------------------------------
// void TEST_net_ini(void)
// {
// 	TEST_usartprop.usart_buf[0]=0;
// 	TEST_usartprop.usart_cnt=0;
// 	TEST_usartprop.is_tcp_connect=0;
// 	TEST_usartprop.is_text=0;
// }
// //-----------------------------------------------
// void TEST_sendstring(char* buf_str)
// {
// 	TEST_usartprop.is_text=1;
// 	tcp_sent(TEST_ss->pcb, TEST_tcp_server_sent);
// 	tcp_write(TEST_ss->pcb, (void*)buf_str, strlen(buf_str), 1);
// 	tcp_recved(TEST_ss->pcb, strlen(buf_str));
// 	TEST_usartprop.usart_cnt=0;
// }
// void TEST_ETHsendchar(char* buf_str)
// {
// 	TEST_usartprop.is_text=1;
// 	tcp_sent(TEST_ss->pcb, TEST_tcp_server_sent);
// 	tcp_write(TEST_ss->pcb, (void*)buf_str, 1, 1);
// 	tcp_recved(TEST_ss->pcb, 1);
// 	TEST_usartprop.usart_cnt=0;
// }
// //-----------------------------------------------
// void TEST_string_parse(char* buf_str)
// {
// 	  HAL_UART_Transmit(&huart2, (uint8_t*)buf_str,strlen(buf_str),0x1000);
// 	  TEST_sendstring(buf_str);
// 	  HAL_GPIO_WritePin(GPIOD, GPIO_PIN_12|GPIO_PIN_13, GPIO_PIN_RESET);
// }
// //-----------------------------------------------
// void TEST_UART6_RxCpltCallback(void)
// {
// 	uint8_t b;
// 	b = TEST_str[0];
// 	//если вдруг случайно превысим длину буфера
// 	if (TEST_usartprop.usart_cnt>25)
// 	{
// 		TEST_usartprop.usart_cnt=0;
// 		HAL_UART_Receive_IT(&huart2,(uint8_t*)TEST_str,1);
// 		return;
// 	}
// 	TEST_usartprop.usart_buf[TEST_usartprop.usart_cnt] = b;
// 	if(b==0x0A)
// 	{
// 		TEST_usartprop.usart_buf[TEST_usartprop.usart_cnt+1]=0;
// 		TEST_string_parse((char*)TEST_usartprop.usart_buf);
// 		TEST_usartprop.usart_cnt=0;
// 		HAL_UART_Receive_IT(&huart2,(uint8_t*)TEST_str,1);
// 		return;
// 	}
// 	TEST_usartprop.usart_cnt++;
// 	HAL_UART_Receive_IT(&huart2,(uint8_t*)TEST_str,1);
// }
// //-----------------------------------------------
