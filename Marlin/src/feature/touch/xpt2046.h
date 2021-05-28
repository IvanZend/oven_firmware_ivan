/**
 * Marlin 3D Printer Firmware
 * Copyright (c) 2020 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
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

#include <stdint.h>

// Relies on XPT2046-compatible mode of ADS7843,
// hence no Z1 / Z2 measurements are possible.

#define XPT2046_DFR_MODE 0x00
#define XPT2046_SER_MODE 0x04
#define XPT2046_CONTROL  0x80

struct touchEXTI{
	bool TOUCHED;
	uint16_t t_x;
	uint16_t t_y;
  uint16_t code;
  uint32_t time;
};

enum XPTCoordinate : uint8_t {
  XPT2046_X  = 0x10,
  XPT2046_Y  = 0x50,
  XPT2046_Z1 = 0x30,
  XPT2046_Z2 = 0x40
};

typedef enum {
	T_noTouch,				//Нет нажатия
	T_pressed,				//Нажат (передний фронт /)
	T_shortHoldDown,	//Короткое удерживание (-)
	T_longHoldDown,		//Длинное удерживание (-)
	T_released, 			//Отпущено (задний фронт \)
} touchStates;
typedef enum {
	XPT2046_LANDSCAPE,
	XPT2046_PORTRAIT,
	XPT2046_LANDSCAPE_180,
	XPT2046_PORTRAIT_180,
} touchOrienation;


typedef struct {
	uint16_t x;
	uint16_t y;
        uint16_t z1;
        uint16_t z2;
	touchStates state;
} touch_t;

typedef void (*screenFunc_t)();

#define XPT2046_SAMPLES 16

#ifndef XPT2046_Z1_THRESHOLD
  #define XPT2046_Z1_THRESHOLD 10
#endif

class XPT2046 {
public:
  static void init();
  static uint16_t read_buttons(screenFunc_t currentScreen);
  static uint8_t returnBtn(screenFunc_t currentScreen);
  bool getTouchPoint(uint16_t &x, uint16_t &y);
  static bool isTouched();
  inline void waitForRelease() { while (isTouched()) { /* nada */ } }
  inline void waitForTouch(uint16_t &x, uint16_t &y) { while (!getTouchPoint(x, y)) { /* nada */ } }
  static touch_t get16samples();
private:
  static uint16_t getInTouch(const XPTCoordinate coordinate);
  static uint16_t get1sample(const XPTCoordinate coordinate);
  static uint16_t SPI_Soft_Getdata(uint8_t coord);
  };

extern XPT2046 touch;
