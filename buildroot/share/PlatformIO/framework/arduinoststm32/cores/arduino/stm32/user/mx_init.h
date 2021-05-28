#include "stm32f4xx_hal.h"

#define LCD_PWM_Pin GPIO_PIN_7
#define LCD_PWM_GPIO_Port GPIOA
#define LCD_RES_Pin GPIO_PIN_7
#define LCD_RES_GPIO_Port GPIOH
#define USB_PWM_Pin GPIO_PIN_13
#define USB_PWM_GPIO_Port GPIOB
#define USART_DIR_Pin GPIO_PIN_7
#define USART_DIR_GPIO_Port GPIOD

extern LTDC_HandleTypeDef hltdc;
extern RNG_HandleTypeDef hrng;
extern UART_HandleTypeDef huart2;
// extern DMA_HandleTypeDef hdma_usart2_rx;
// extern DMA_HandleTypeDef hdma_usart2_tx;
extern SDRAM_HandleTypeDef hsdram1;


void MX_LTDC_Init(void);
// void MX_GPIO_Init(void);
// void MX_DMA_Init(void);
//void MX_LTDC_Init(void);
// void MX_USART2_UART_Init(void);
// void MX_FMC_Init(void);
//void MX_RNG_Init(void);
// void MX_USB_HOST_Process(void);
// #include <string.h>
// #include <stdlib.h>
// #include "fonts.h"

