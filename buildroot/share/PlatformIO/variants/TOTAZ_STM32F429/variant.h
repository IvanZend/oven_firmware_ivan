/*
 *******************************************************************************
 * Copyright (c) 2017, STMicroelectronics
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 * 3. Neither the name of STMicroelectronics nor the names of its contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
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
 *******************************************************************************
 */
#pragma once

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

/*----------------------------------------------------------------------------
 *        Pins
 *----------------------------------------------------------------------------*/

#define STM32F4X_PIN_NUM  176  //176 pins mcu, 140 gpio
#define STM32F4X_GPIO_NUM 140

#define PC13  0 //GPIO Y DIR
#define PC14  1 //GPIO Y STEP
#define PC15  2 //GPIO Y ENABLE
#define PH0   3 //OSC_IN
#define PH1   4 //OSC_OUT
#define PB2   5 //TouchScreen Right Down
#define PB10  6 //TIM2_CH3 / HEATER3 PWM
#define PB11  7 //TIM2_CH4 / HEATER4 PWM
#define PB12  8 //OTG_HS_ID
#define PB13  9 //OTG_HS_VBUS
#define PB14  10 //OTG_HS_DM
#define PB15  11 //OTG_HS_DP
#define PC6   12 //TIM3_CH1 / FAN1 PWM
#define PC7   13 //TIM3_CH2 / FAN2 PWM
#define PC8   14 //TIM3_CH3 / FAN3 PWM
#define PC9   15 //TIM3_CH4 / FAN4 PWM
#define PA8   16 //DIOD1
#define PA9   17 //DID2
#define PA10  18 //GPIO MAX6675 NCS (SPI)
#define PA11  19 //GPIO FLASH 1 NCS (SPI)
#define PA12  20 //GPIO FLASH 2 NCS (SPI)
#define PA13  21 //0:JTMS-SWDIO
#define PA14  22 //0:JTCK-SWCLK
#define PA15  23 //HEATER1 PWM
#define PC10  24 //SPI3_SCK
#define PC11  25 //SPI3_MISO
#define PC12  26 //SPI3_MOSI
#define PD2   27 //DID4
#define PB3   28 //HEATER2 PWM
#define PB4   29 //DIEN2
#define PB5   30 //CAN2_RX
#define PB6   31 //CAN2_TX
#define PB7   32 //TIM4_CH2 LED RED PWM
#define PB8   33 //TIM4_CH3 LED GREEN PWM
#define PB9   34 //TIM4_CH4 LED BLUE PWM
#define PA0   35 //ADC123_IN0 / PT100 AIN7
#define PA1   36 //ADC123_IN1 / PT100 AIN8
#define PA2   37 //GPIO Z DIR
#define PA3   38 //GPIO Z ENABLE
#define PA4   39 //GPIO U DIR
#define PA5   40 //GPIO U STEP
#define PA6   41 //GPIO U ENABLE
#define PA7   42 //GPIO LCD_PWM	TFT_LCD
#define PB0   43 //ADC12_IN8 / TouchScreen ADC
#define PB1   44 //GPIO	TouchScreen Left Down
#define PC0   45 //ADC123_IN10	PT100 AIN5
#define PC1   46 //ADC123_IN11	PT100 AIN6
#define PC2   47 //FMC_SDNE0	SDRAM
#define PC3   48 //FMC_SDCKE0	SDRAM
#define PC4   49 //GPIO	TouchScreen Left Top
#define PC5   50 //GPIO	TouchScreen Right Top
#define PF3   51 //FMC_A3	SDRAM
#define PF4   52 //FMC_A4	SDRAM
#define PF5   53 //FMC_A5	SDRAM
#define PF6   54 //ADC3_IN4	PT100 AIN1
#define PF7   55 //ADC3_IN5	PT100 AIN2
#define PF8   56 //ADC3_IN6	PT100 AIN3
#define PF9   57 //ADC3_IN7	PT100 AIN4
#define PF10  58 //LCD_DE	TFT_LCD
#define PE2   (35+24) //GPIO	X DIR
#define PE3   (36+24) //GPIO	X STEP
#define PE4   (37+24) //LCD_B0	TFT_LCD
#define PE5   (38+24) //LCD_G0	TFT_LCD
#define PE6   (39+24) //LCD_G1	TFT_LCD
#define PE7   (40+24) //FMC_D4	SDRAM
#define PE8   (41+24) //FMC_D5	SDRAM
#define PE9   (42+24) //FMC_D6	SDRAM
#define PE10  (43+24) //FMC_D7	SDRAM
#define PE11  (44+24) //FMC_D8	SDRAM
#define PE12  (45+24) //FMC_D9	SDRAM
#define PE13  (46+24) //FMC_D10	SDRAM
#define PE14  (47+24) //FMC_D11	SDRAM
#define PE15  (48+24) //FMC_D12	SDRAM
#define PD8   (49+24) //FMC_D13	SDRAM
#define PD9   (50+24) //FMC_D14	SDRAM
#define PD10  (51+24) //FMC_D15	SDRAM
#define PD11  (52+24) //GPIO	V DIR
#define PD12  (53+24) //GPIO	V STEP
#define PD13  (54+24) //GPIO	V ENABLE
#define PD14  (55+24) //FMC_D0	SDRAM
#define PD15  (56+24) //FMC_D1	SDRAM
#define PD0   (57+24) //FMC_D2	SDRAM
#define PD1   (58+24) //FMC_D3	SDRAM
#define PD3   (59+24) //DID5
#define PD4   (60+24) //DIEN1
#define PD5   (61+24) //USART2_TX
#define PD6   (62+24) //USART2_RX
#define PD7   (63+24) //RS-485_DIR (GPIO)
#define PE0   (64+24) //FMC_NBL0	SDRAM
#define PE1   (65+24) //FMC_NBL1	SDRAM
#define PF0   (66+24) //FMC_A0	SDRAM
#define PF1   (67+24) //FMC_A1	SDRAM
#define PF2   (68+24) //FMC_A2	SDRAM
#define PF11  (69+24) //FMC_SDNRAS	SDRAM
#define PF12  (70+24) //FMC_A6	SDRAM
#define PF13  (71+24) //FMC_A7	SDRAM
#define PF14  (72+24) //FMC_A8	SDRAM
#define PF15  (73+24) //FMC_A9	SDRAM
#define PG0   (74+24) //FMC_A10	SDRAM
#define PG1   (75+24) //FMC_A11	SDRAM
#define PG2   (76+24) //TS_CK1
#define PG3   (77+24) //DIOD0
#define PG4   (78+24) //FMC_BA0	SDRAM
#define PG5   (79+24) //FMC_BA1	SDRAM
#define PG6   (80+24) //LCD_R7	TFT_LCD
#define PG7   (81+24) //LCD_CLK	TFT_LCD
#define PG8   (82+24) //FMC_SDCLK	SDRAM
#define PG9   (83+24) //GPIO	W DIR
#define PG10  (84+24) //LCD_B2	TFT_LCD
#define PG11  (85+24) //LCD_B3	TFT_LCD
#define PG12  (86+24) //LCD_B1	TFT_LCD
#define PG13  (87+24) //GPIO	W STEP
#define PG14  (88+24) //GPIO	W ENABLE
#define PG15  (89+24) //FMC_SDNCAS	SDRAM
#define PI8   (90+24) //GPIO	X ENABLE
#define PI9   (91+24) //LCD_VSYNC	TFT_LCD
#define PI10  (92+24) //LCD_HSYNC	TFT_LCD
#define PI11  (93+24) //GPIO	BUZZER
#define PH2   (94+24) //LCD_R0	TFT_LCD
#define PH3   (95+24) //LCD_R1	TFT_LCD
#define PH4   (96+24) //GPIO	Z STEP
#define PH5   (97+24) //FMC_SDNWE	SDRAM
#define PH6   (98+24) //TIM12_CH1	SERVO PWM
#define PH7   (99+24) //LCD_RES	TFT_LCD
#define PH8   (100+24) //LCD_R2	TFT_LCD
#define PH9   (101+24) //LCD_R3	TFT_LCD
#define PH10  (102+24) //LCD_R4	TFT_LCD
#define PH11  (103+24) //LCD_R5	TFT_LCD
#define PH12  (104+24) //LCD_R6	TFT_LCD
#define PH13  (105+24) //LCD_G2	TFT_LCD
#define PH14  (106+24) //LCD_G3	TFT_LCD
#define PH15  (107+24) //LCD_G4	TFT_LCD
#define PI0   (108+24) //LCD_G5	TFT_LCD
#define PI1   (109+24) //LCD_G6	TFT_LCD
#define PI2   (110+24) //LCD_G7	TFT_LCD
#define PI3   (111+24) //DID3
#define PI4   (112+24) //LCD_B4	TFT_LCD
#define PI5   (113+24) //LCD_B5	TFT_LCD
#define PI6   (114+24) //LCD_B6	TFT_LCD
#define PI7   (115+24) //LCD_B7	TFT_LCD

// This must be a literal
#define NUM_DIGITAL_PINS        (STM32F4X_GPIO_NUM)
// This must be a literal with a value less than or equal to MAX_ANALOG_INPUTS
#define NUM_ANALOG_INPUTS       (24)
#define NUM_ANALOG_FIRST        35

// Below ADC, DAC and PWM definitions already done in the core
// Could be redefined here if needed
// ADC resolution is 12bits
//#define ADC_RESOLUTION          12
//#define DACC_RESOLUTION         12

// PWM resolution
/*
 * BEWARE:
 * Changing this value from the default (1000) will affect the PWM output value of analogWrite (to a PWM pin)
 * Since the pin is toggled on capture, if you change the frequency of the timer you have to adapt the compare value (analogWrite thinks you did)
 */
//#define PWM_FREQUENCY           20000
//The bottom values are the default and don't need to be redefined
//#define PWM_RESOLUTION          8
//#define PWM_MAX_DUTY_CYCLE      255

// Below SPI and I2C definitions already done in the core
// Could be redefined here if differs from the default one
// SPI Definitions
#define PIN_SPI_MOSI            PC12
#define PIN_SPI_MISO            PC11
#define PIN_SPI_SCK             PC10
// #define PIN_SPI_SS              -1

// I2C Definitions
#define PIN_WIRE_SDA            PB7
#define PIN_WIRE_SCL            PB6

// Timer Definitions
//Do not use timer used by PWM pins when possible. See PinMap_PWM in PeripheralPins.c
// #define TIMER_TONE              TIM6
// #define TIMER_SERIAL            TIM7

// Do not use basic timer: OC is required
// #define TIMER_SERVO             TIM2  //TODO: advanced-control timers don't work

// UART Definitions
// Define here Serial instance number to map on Serial generic name
#define SERIAL_UART_INSTANCE    2 //ex: 2 for Serial2 (USART2)
// DEBUG_UART could be redefined to print on another instance than 'Serial'
//#define DEBUG_UART              ((USART_TypeDef *) U(S)ARTX) // ex: USART3
// DEBUG_UART baudrate, default: 9600 if not defined
//#define DEBUG_UART_BAUDRATE     x
// DEBUG_UART Tx pin name, default: the first one found in PinMap_UART_TX for DEBUG_UART
//#define DEBUG_PINNAME_TX        PX_n // PinName used for TX

// Default pin used for 'Serial' instance (ex: ST-Link)
// Mandatory for Firmata
#define PIN_SERIAL_RX           PD6
#define PIN_SERIAL_TX           PD5

#ifdef __cplusplus
} // extern "C"
#endif
/*----------------------------------------------------------------------------
 *        Arduino objects - C++ only
 *----------------------------------------------------------------------------*/

#ifdef __cplusplus
// These serial port names are intended to allow libraries and architecture-neutral
// sketches to automatically default to the correct port name for a particular type
// of use.  For example, a GPS module would normally connect to SERIAL_PORT_HARDWARE_OPEN,
// the first hardware serial port whose RX/TX pins are not dedicated to another use.
//
// SERIAL_PORT_MONITOR        Port which normally prints to the Arduino Serial Monitor
//
// SERIAL_PORT_USBVIRTUAL     Port which is USB virtual serial
//
// SERIAL_PORT_LINUXBRIDGE    Port which connects to a Linux system via Bridge library
//
// SERIAL_PORT_HARDWARE       Hardware serial port, physical RX & TX pins.
//
// SERIAL_PORT_HARDWARE_OPEN  Hardware serial ports which are open for use.  Their RX & TX
//                            pins are NOT connected to anything by default.
// #define SERIAL_PORT_MONITOR     Serial
// #define SERIAL_PORT_HARDWARE    Serial1
#endif
