/**
 * Marlin 3D Printer Firmware
 *
 * Copyright (c) 2020 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
 * Copyright (c) 2016 Bob Cousins bobcousins42@googlemail.com
 * Copyright (c) 2015-2016 Nico Tonnhofer wurstnase.reprap@gmail.com
 * Copyright (c) 2017 Victor Perez
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */
#pragma once


#define CPU_32_BIT

#include "stm32f4xx_hal.h"
#ifdef __cplusplus
 extern "C" {
#endif 

#define SDRAM_TIMEOUT     							((uint32_t)0xFFFF)
#define SDRAM_MODEREG_BURST_LENGTH_1				((uint16_t)0x0000)
#define SDRAM_MODEREG_BURST_LENGTH_2            	((uint16_t)0x0001)
#define SDRAM_MODEREG_BURST_LENGTH_4            	((uint16_t)0x0002)
#define SDRAM_MODEREG_BURST_LENGTH_8            	((uint16_t)0x0004)
#define SDRAM_MODEREG_BURST_TYPE_SEQUENTIAL     	((uint16_t)0x0000)
#define SDRAM_MODEREG_BURST_TYPE_INTERLEAVED    	((uint16_t)0x0008)
#define SDRAM_MODEREG_CAS_LATENCY_2             	((uint16_t)0x0020)
#define SDRAM_MODEREG_CAS_LATENCY_3             	((uint16_t)0x0030)
#define SDRAM_MODEREG_OPERATING_MODE_STANDARD   	((uint16_t)0x0000)
#define SDRAM_MODEREG_WRITEBURST_MODE_PROGRAMMED	((uint16_t)0x0000)
#define SDRAM_MODEREG_WRITEBURST_MODE_SINGLE    	((uint16_t)0x0200)

#define SDRAM_BANK_ADDR                 			((uint32_t)0xC0000000)

#define REFRESH_COUNT ((uint32_t)637)   /* SDRAM refresh counter (100Mhz SD clock) */


void w9812g6_init(SDRAM_HandleTypeDef *hsdram);
#ifdef __cplusplus
}
#endif