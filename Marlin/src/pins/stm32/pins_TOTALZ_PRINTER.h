/**
 * Marlin 3D Printer Firmware
 * Copyright (c) 2019 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
 *
 * Based on Sprinter and grbl.
 * Copyright (c) 2011 Camiel Gubbels / Erik van der Zalm
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

#include "Version.h"
/**
 * To build with Arduino IDE use "Discovery F407VG"
 * To build with PlatformIO use environment "STM32F4"
 */
#if !defined(STM32F4) && !defined(STM32F4xx)
  #error "Oops! Select an STM32F4 board in 'Tools > Board.'"
#endif

#define BOARD_INFO_NAME      "Misc. TOTALZ_STM32F429IIT6"
#define DEFAULT_MACHINE_NAME "TOTALZ_PRINTER"

//#define I2C_EEPROM

#ifndef E2END
  #define E2END 0xFFF // 4KB
#endif

// Ignore temp readings during development.
//#define BOGUS_TEMPERATURE_GRACE_PERIOD 2000

//
// Limit Switches
//
#define X_MIN_PIN          PA8
#define X_MAX_PIN          -1
#define Y_MIN_PIN          -1
#define Y_MAX_PIN          PG3
#define Z_MIN_PIN          PI3
#define Z_MAX_PIN          -1 //PA8


//
// Z Probe (when not Z_MIN_PIN)
//

//#ifndef Z_MIN_PROBE_PIN
//  #define Z_MIN_PROBE_PIN  PA4
//#endif

//
// Steppers
//
// #define A4988MOT
/*--------------------FOR STEPPERS--------------------*/
/*XYZ*/
//TRUE SETTINGS:
#define Y_DIR_PIN          PC13
#define Y_STEP_PIN         PC14
#ifdef  A4988MOT
  #define Y_ENABLE_PIN       PC15
#else 
  #define Y_ENABLE_PIN       PC15//-1//PC15//-1
#endif

#define X_DIR_PIN          PE2
#define X_STEP_PIN         PE3
#ifdef  A4988MOT
#define X_ENABLE_PIN       PI8
#else 
#define X_ENABLE_PIN       PI8//-1//PI8//-1
#endif
// //#ifndef X_CS_PIN
// //  #define X_CS_PIN         PD1
// //#endif
//#ifndef Y_CS_PIN
//  #define Y_CS_PIN         PE12
//#endif
#define Z_DIR_PIN          PB1
#define Z_STEP_PIN         PH4
#ifdef  A4988MOT
#define Z_ENABLE_PIN       PA3
#else 
#define Z_ENABLE_PIN       PA3//-1//PA3//-1
#endif
//#ifndef Z_CS_PIN
//  #define Z_CS_PIN         PD5
//#endif


#define E0_DIR_PIN         PA4
#define E0_STEP_PIN        PA5
#define E0_ENABLE_PIN      PA6

#define E1_DIR_PIN         PD11
#define E1_STEP_PIN        PD12
#define E1_ENABLE_PIN      PD13

#define E2_DIR_PIN         PG9
#define E2_STEP_PIN        PG13
#define E2_ENABLE_PIN      PF9//PG14

//#ifndef E0_CS_PIN
//  #define E0_CS_PIN         PB4
//#endif

// #define E1_STEP_PIN        PD12
// #define E1_DIR_PIN         PD11
// #define E1_ENABLE_PIN      PD13

// #define E2_STEP_PIN        PG13
// #define E2_DIR_PIN         PG9
// #define E2_ENABLE_PIN      PG14


//#ifndef E1_CS_PIN
//  #define E1_CS_PIN         PE5
//#endif

#define SCK_PIN            PC10
#define MISO_PIN           PC11
#define MOSI_PIN           PC12

#define CS_PIN			       


#ifdef W9825G6KH
#define T_CS_PIN			    PC8//PG2 //TS_CK2 55
#else 
#define T_CS_PIN			    PG2//PG2 //TS_CK2 55
#endif  
#define T_IRQ_PIN			    PF6  //US_DIR 151
#define T_SCK_PIN			    PB7  //TS_EN2 54
#define T_MISO_PIN			  PB2  //T_CK1 54
#define T_MOSI_PIN			  PF8  //T_EN1 54



//
// Temperature Sensors
//
//PC0, PC1, PA0, PA1 - аналоговые пины температуры
//PF6 <=> PF9 - GPIO пины для максин от CS4 до CS1 (они идут в обратном порядке) <- больше не актуально, максин нет

#define TEMP_0_PIN         -1//PA0//-6//PC1//PF7   // Analog Input pc0=32pin; pc1=33pin...
#define TEMP_1_PIN         -1   // Analog Input
#define TEMP_2_PIN         -1   // Analog Input
#define TEMP_3_PIN         -1   // Analog Input
#define TEMP_4_PIN         -1   // Analog Input
#define TEMP_5_PIN         -1	//PA0   // Analog Input
#define TEMP_6_PIN         -1   // Analog Input
// #define TEMP_7_PIN         PA1   // Analog Input


#define TEMP_BED_PIN           PC0
#define TEMP_CHAMBER_PIN       PA0
// #define MAX6675_DO_PIN         PA1   // 
// #define MAX6675_SCK_PIN        PA1   // 

#define ADS_CS1_PIN         PA10//PF6  // 
#define ADS_CS2_PIN         PF7//PF6  //

// #define MAX6675_SS_bed_PIN       PA10   // 
// #define MAX6675_SS2_PIN        PA1   // 

//
// Heaters / Fans
//

#define OVEN_HEATER_600_PIN     PA15
#define OVEN_HEATER_800_PIN     PB3
#define OVEN_VACUUM_PUMP_PIN    PB10

#define HEATER_0_PIN       OVEN_HEATER_600_PIN    // было PC9
#define HEATER_1_PIN       OVEN_HEATER_800_PIN    // было PB10
#define HEATER_2_PIN       -1 
#define HEATER_3_PIN       -1
#define HEATER_4_PIN       -1
#define HEATER_5_PIN       -1
#define HEATER_BED_PIN      PC9 //PA15
#define HEATER_CHAMBER_PIN    OVEN_VACUUM_PUMP_PIN //PB3

#ifndef FAN_PIN
  #define FAN_PIN          PC6
#endif
#define FAN1_PIN           -1

#ifdef W9825G6KH
  #define FAN2_PIN           -1
#else
  #define FAN2_PIN           PC8
#endif
#define FAN3_PIN           -1
#define FAN4_PIN           -1

#define ORIG_E0_AUTO_FAN_PIN  FAN1_PIN // Use this by NOT overriding E0_AUTO_FAN_PIN

//
// Misc. Functions
//

//#define CASE_LIGHT_PIN_CI PF13
//#define CASE_LIGHT_PIN_DO PF14
//#define NEOPIXEL_PIN      PF13

//
// Průša i3 MK2 Multi Material Multiplexer Support
//

//#define E_MUX0_PIN         PG3
//#define E_MUX1_PIN         PG4

//
// Servos
//

#define SERVO1_PIN         PH6
//#define SERVO2_PIN         PE14


#define SDSS               PB13
#define SS_PIN             -1//PA10
#define LED_PIN            PB9         //Alive
#define PS_ON_PIN          -1
#define KILL_PIN           -1
#define PWR_LOSS           -1         //Power loss / nAC_FAULT

//
// LCD / Controller
//

#define SD_DETECT_PIN      PB8
#define BEEPER_PIN         PI11
#define FSMC_CS_PIN        -1
#define FSMC_RS_PIN        -1
#define LCD_PINS_RS        -1
#define LCD_PINS_ENABLE    -1
#define LCD_PINS_D4        -1
#define LCD_PINS_D5        -1
#define LCD_PINS_D6        -1
#define LCD_PINS_D7        -1
#define BTN_EN1            -1
#define BTN_EN2            -1
#define BTN_ENC            -1

//
// Filament runout
//

#define FIL_RUNOUT_PIN     -1

//
// ST7920 Delays
//
#if HAS_GRAPHICAL_LCD
  #define BOARD_ST7920_DELAY_1 DELAY_NS(96)
  #define BOARD_ST7920_DELAY_2 DELAY_NS(48)
  #define BOARD_ST7920_DELAY_3 DELAY_NS(715)
#endif
