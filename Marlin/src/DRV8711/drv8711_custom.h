#pragma once

#ifndef __DRV8711_H_
#define __DRV8711_H_

// #include "stm32f4xx_hal.h"
// #include "stm32f4xx_hal_spi.h"
// // #include <string.h>
// // #include <stdlib.h>
// #include "main.h"
#include "mx_init.h"
// // // #include "fonts.h"
// // #include "motors.h"

#ifdef __cplusplus
 extern "C" {
#endif 


#define drv_8711REG_X					1
#define drv_8711REG_Y					2
#define drv_8711REG_Z					3
#define drv_8711REG_U					4
#define drv_8711REG_V					5
#define drv_8711REG_W					6
#define drv_8711REG_E0					4
#define drv_8711REG_E1					5
#define drv_8711REG_E2					6

#define CTRL_8711REG					0x0
#define TORQUE_8711REG					0x1
#define OFF_8711REG						0x2
#define BLANK_8711REG					0x3
#define DECAY_8711REG					0x4
#define STALL_8711REG					0x5
#define DRIVE_8711REG					0x6
#define STATUS_8711REG					0x7


// extern SPI_HandleTypeDef hspi3;
// extern UART_HandleTypeDef huart2;
// extern DMA_HandleTypeDef hdma_usart2_rx;

struct motorRegs{
	uint16_t Xreg;
	uint16_t Yreg;
	uint16_t Zreg;
	uint16_t E0reg;
	uint16_t E1reg;
	uint16_t E2reg;
};
struct drvDefSettings{
	uint16_t ctrl;
	uint16_t torque;
	uint16_t off;
	uint16_t blank;
	uint16_t decay;
	uint16_t stall;
	uint16_t drive;
};



void driver_onoff(uint8_t onoff, uint8_t axis, GPIO_TypeDef* port, uint16_t pin, void (*f)(uint8_t) );
void starterSettings(uint8_t axis, GPIO_TypeDef* port, uint16_t pin, void (*f)(uint8_t));
void RegisterTxRx(uint16_t, char * s, GPIO_TypeDef* port, uint16_t pin, void (*f)(uint8_t), uint8_t pin_stm);
void setSingleDriver(uint16_t DataByte1,uint16_t addr, uint16_t command, uint16_t read, GPIO_TypeDef* port, uint16_t pin);
void getRegFromAddr(char *buf,uint16_t addr);


#ifdef __cplusplus
}
#endif


#endif /* __DRV8711_H_ */