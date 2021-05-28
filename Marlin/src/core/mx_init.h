#include "stm32f4xx_hal.h"
#include "../../Version.h"
#include "stm32_def.h"
// #include "../inc/MarlinConfig.h"
// #include "usb_host.h"
// #include "fatfs.h"

#ifdef __cplusplus
 extern "C" {
#endif 


// #ifndef DMA2D_MAX_LAYER
//  #define DMA2D_MAX_LAYER  17U
// #else
//   #undef DMA2D_MAX_LAYER
//   #define DMA2D_MAX_LAYER  17U
// #endif

/*--------------------FOR STEPPERS--------------------*/
/*XYZ*/
//TRUE SETTINGS:
#define X_STEP_Pin_MX GPIO_PIN_3
#define X_STEP_GPIO_Port GPIOE
#define X_ENABLE_Pin_MX GPIO_PIN_8
#define X_ENABLE_GPIO_Port GPIOI
#define Y_STEP_Pin_MX GPIO_PIN_14
#define Y_STEP_GPIO_Port GPIOC
#define Y_ENABLE_Pin_MX GPIO_PIN_15
#define Y_ENABLE_GPIO_Port GPIOC
#define Z_STEP_Pin_MX GPIO_PIN_4
#define Z_STEP_GPIO_Port GPIOH
#define Z_ENABLE_Pin_MX GPIO_PIN_3
#define Z_ENABLE_GPIO_Port GPIOA

//TEMP SETTINGS:



/*EXTRUDERS*/
//TRUE SETTINGS:
#define E0_STEP_Pin_MX GPIO_PIN_5
#define E0_STEP_GPIO_Port GPIOA
#define E0_ENABLE_Pin_MX GPIO_PIN_6
#define E0_ENABLE_GPIO_Port GPIOA
#define E1_STEP_Pin_MX GPIO_PIN_12
#define E1_STEP_GPIO_Port GPIOD
#define E1_ENABLE_Pin_MX GPIO_PIN_13
#define E1_ENABLE_GPIO_Port GPIOD
#define E2_STEP_Pin_MX GPIO_PIN_13
#define E2_STEP_GPIO_Port GPIOG
#define E2_ENABLE_Pin_MX GPIO_PIN_9
#define E2_ENABLE_GPIO_Port GPIOF


/*--------------------OUTPUTS, CHIP-SELECT--------------------*/
/*SPIs-TALK*/
#define ADS_CS1_Pin GPIO_PIN_10
#define ADS_CS1_GPIO_Port GPIOA
#define ADS_CS2_Pin GPIO_PIN_7
#define ADS_CS2_GPIO_Port GPIOF
#define FLASH_SPI1_Pin GPIO_PIN_11
#define FLASH_SPI1_GPIO_Port GPIOA
#define FLASH_SPI2_Pin GPIO_PIN_12
#define FLASH_SPI2_GPIO_Port GPIOA
/*TRIGGERS-LOW-FOR-ENABLE*/
#define DIEN1_Pin GPIO_PIN_4
#define DIEN1_GPIO_Port GPIOD
#define DIEN2_Pin GPIO_PIN_4
#define DIEN2_GPIO_Port GPIOB
/*TOUCH*/

#define TS_MOSI_Pin GPIO_PIN_8  //EN1   //PB2
#define TS_MOSI_GPIO_Port GPIOF 
#define TS_SCK_Pin GPIO_PIN_7   //EN2   //PC4
#define TS_SCK_GPIO_Port GPIOB
#define TS_MISO_Pin GPIO_PIN_2  //CK1   //PG2
#define TS_MISO_GPIO_Port GPIOB
#ifdef W9825G6KH
	#define TS_CS_Pin GPIO_PIN_8    //CK2   //PC5
	#define TS_CS_GPIO_Port GPIOC
#else
	#define TS_CS_Pin GPIO_PIN_2    //CK2   //PC5
	#define TS_CS_GPIO_Port GPIOG
#endif
#define TS_IRQ_Pin GPIO_PIN_6   //USART_DIR //PD7
#define TS_IRQ_GPIO_Port GPIOF


/*OTHERS*/
#define LCD_PWM_Pin GPIO_PIN_0
#define LCD_PWM_GPIO_Port GPIOB
#define LCD_RES_Pin GPIO_PIN_7
#define LCD_RES_GPIO_Port GPIOH
#define USB_PWM_Pin GPIO_PIN_13
#define USB_PWM_GPIO_Port GPIOB
// #define USART_DIR_Pin GPIO_PIN_7
// #define USART_DIR_GPIO_Port GPIOD
#define BUZZER_Pin GPIO_PIN_11
#define BUZZER_GPIO_Port GPIOI

/*--------------------INPUTS-FOR-TRIGGERS--------------------*/
#define DIOD0_Pin GPIO_PIN_3
#define DIOD0_GPIO_Port GPIOG
#define DIOD1_Pin GPIO_PIN_8
#define DIOD1_GPIO_Port GPIOA
#define DID2_Pin GPIO_PIN_9
#define DID2_GPIO_Port GPIOA
#define DID3_Pin GPIO_PIN_3
#define DID3_GPIO_Port GPIOI
#define DID4_Pin GPIO_PIN_2
#define DID4_GPIO_Port GPIOD
#define DID5_Pin GPIO_PIN_3
#define DID5_GPIO_Port GPIOD

/*--------------------ETHERNET--------------------*/
#define MDC_Pin GPIO_PIN_1
#define MDC_GPIO_Port GPIOC
#define REFCLK_Pin GPIO_PIN_1
#define REFCLK_GPIO_Port GPIOA
#define MDIO_Pin GPIO_PIN_2
#define MDIO_GPIO_Port GPIOA
#define CRSDV_Pin GPIO_PIN_7
#define CRSDV_GPIO_Port GPIOA
#define RXD0_Pin GPIO_PIN_4
#define RXD0_GPIO_Port GPIOC
#define RXD1_Pin GPIO_PIN_5
#define RXD1_GPIO_Port GPIOC
#define TXEN_Pin GPIO_PIN_11
#define TXEN_GPIO_Port GPIOB
#define TXD0_Pin GPIO_PIN_12
#define TXD0_GPIO_Port GPIOB
#define TXD1_Pin GPIO_PIN_14
#define TXD1_GPIO_Port GPIOG


/*--------------------FOR-TIMERS--------------------*/
/*HEATERS*/
#define HR1_PWM_Pin GPIO_PIN_9
#define HR1_PWM_GPIO_Port GPIOC
#define HR2_PWM_Pin GPIO_PIN_10
#define HR2_PWM_GPIO_Port GPIOB
#define HR3_PWM_Pin GPIO_PIN_3
#define HR3_PWM_GPIO_Port GPIOB
#define HR4_PWM_Pin GPIO_PIN_15
#define HR4_PWM_GPIO_Port GPIOA

/*FANS*/
#define FAN1_PWM_Pin GPIO_PIN_6
#define FAN1_PWM_GPIO_Port GPIOC
#define FAN2_PWM_Pin GPIO_PIN_7
#define FAN2_PWM_GPIO_Port GPIOC

#ifndef W9825G6KH
    #define FAN3_PWM_Pin GPIO_PIN_8
    #define FAN3_PWM_GPIO_Port GPIOC
#endif


// #define FAN4_PWM_Pin GPIO_PIN_9
// #define FAN4_PWM_GPIO_Port GPIOC


/*LEDS*/

// #define RED_PWM_Pin GPIO_PIN_7
// #define RED_PWM_GPIO_Port GPIOB

#define GRN_PWM_Pin GPIO_PIN_8
#define GRN_PWM_GPIO_Port GPIOB
#define BLUE_PWM_Pin GPIO_PIN_9
#define BLUE_PWM_GPIO_Port GPIOB
/*SERVOS*/
#define SRV_PWM_Pin GPIO_PIN_6
#define SRV_PWM_GPIO_Port GPIOH

#define W25ADDR_ENDSTOPTEST 				0x0000
#define W25ADDR_BABYSTEPS 					0x1000

extern LTDC_HandleTypeDef hltdc;
// extern RNG_HandleTypeDef hrng;
extern UART_HandleTypeDef huart2;
// extern SPI_HandleTypeDef hspi3;
// extern DMA_HandleTypeDef hdma_usart2_rx;
// extern DMA_HandleTypeDef hdma_usart2_tx;
extern SDRAM_HandleTypeDef hsdram1;
extern TIM_HandleTypeDef htim2;
extern TIM_HandleTypeDef htim3;
extern TIM_HandleTypeDef htim4;
extern TIM_HandleTypeDef htim12;
// extern ApplicationTypeDef Appli_state;

void delay_us(uint32_t delay);
void MX_LTDC_Init(void);
void MX_GPIO_Init(void);
void MX_DMA_Init(void);
void MX_SPI3_Init(void);
void MX_USART2_UART_Init(void);
void MX_FMC_Init(void);

void MX_TIM2_Init(void);
void MX_TIM4_Init(void);
void MX_TIM3_Init(void);
void MX_TIM12_Init(void);
// void MX_RNG_Init(void);
void MX_USB_HOST_Process(void);
void TestFuncForReference(int x);
int check_SPI_enables();
// #include <string.h>
// #include <stdlib.h>
// #include "fonts.h"

#ifdef __cplusplus
}
#endif