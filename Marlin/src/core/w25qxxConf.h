#include "mx_init.h"

// #pragma once

#ifndef _W25QXXCONFIG_H
#define _W25QXXCONFIG_H

// #ifdef __cplusplus
// extern "C" {
// #endif

// extern SPI_HandleTypeDef  hspi3;
// #include "SPI.h"

// extern SPIClass SPI;

// #define _W25QXX_SPI                   SPI._spi
#define _W25QXX_CS_GPIO               FLASH_SPI1_GPIO_Port
#define _W25QXX_CS_PIN1                FLASH_SPI1_Pin
#define _W25QXX_CS_PIN2                FLASH_SPI2_Pin
#define _W25QXX_USE_FREERTOS          0
#define _W25QXX_DEBUG                 0
#define _CHIP_MONITOR				  0



// #ifdef __cplusplus
// }
// #endif
#endif
