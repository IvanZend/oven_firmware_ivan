/**
  ******************************************************************************
  * @file    hw_config.c
  * @author  WI6LABS
  * @version V1.0.0
  * @date    01-August-2016
  * @brief   provide some hw interface for the Arduino interface
  *
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT(c) 2016 STMicroelectronics</center></h2>
  *
  * Redistribution and use in source and binary forms, with or without modification,
  * are permitted provided that the following conditions are met:
  *   1. Redistributions of source code must retain the above copyright notice,
  *      this list of conditions and the following disclaimer.
  *   2. Redistributions in binary form must reproduce the above copyright notice,
  *      this list of conditions and the following disclaimer in the documentation
  *      and/or other materials provided with the distribution.
  *   3. Neither the name of STMicroelectronics nor the names of its contributors
  *      may be used to endorse or promote products derived from this software
  *      without specific prior written permission.
  *
  * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
  * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
  * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
  * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
  * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
  * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
  * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
  * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
  * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
  * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ******************************************************************************
  */
#include "stm32_def.h"
#include "hw_config.h"
#include "usbd_if.h"
#include "dwt.h"

#include "user/W9812G6.h"
#include "user/mx_init.h"
#include "user/LTDC/ltdc.h"
#include "user/LTDC/fonts.h"

#ifdef __cplusplus
extern "C" {
#endif


extern LTDC_HandleTypeDef hltdc;
extern RNG_HandleTypeDef hrng;
extern UART_HandleTypeDef huart2;
// extern DMA_HandleTypeDef hdma_usart2_rx;
// extern DMA_HandleTypeDef hdma_usart2_tx;
extern SDRAM_HandleTypeDef hsdram1;

// char rxData[40];
// extern ApplicationTypeDef Appli_state;
// FATFS usb_filesys;
// DIR usb_directory;
// FIL usb_textfile;
// FIL MyFile;
// FRESULT usb_result;
// extern char SD_Path[4]; /* SD logical drive path */
// uint8_t sect[4096];
// uint32_t bytesread = 0;

// int bmp1 = (uint8_t *)0xC03A9800;
/**
  * @brief  This function performs the global init of the system (HAL, IOs...)
  * @param  None
  * @retval None
  */

void hw_config_init(void)
{
  /* Init DWT if present */
#ifdef DWT_BASE
  // dwt_init();
#endif

  /* Initialize the HAL */
  // HAL_Init();

  /* Configure the system clock */
  // SystemClock_Config();


 // __enable_irq();
#if defined (USBCON) && defined(USBD_USE_CDC)
  USBD_CDC_init();
#endif

}
#ifdef __cplusplus
}
#endif

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
