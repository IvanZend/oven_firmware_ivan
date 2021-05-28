/**
 * Marlin 3D Printer Firmware
 * Copyright (c) 2020 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
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

// Pt1000 with 1k0 pullup
const long temptable_997[][2] PROGMEM = {
//*64

/*
*
* 32768 <---> 0
*
*	30000 - 468 - 20
*/
	
	{ OV(0.0), 		999 },


	
	{ OV(1550.0), 	321 },//
	{ OV(1750.0), 	312 },//
	{ OV(2100.0), 	302 },//
	{ OV(2400.0), 	292 },//
	{ OV(2700.0), 	283 },//
	{ OV(3150.0), 	274 },//
	{ OV(3650.0), 	263 },//
	{ OV(4200.0), 	253 },//
	{ OV(5100.0), 	244 },//
	{ OV(6050.0), 	234 },//
	{ OV(7150.0), 	225 },//
	{ OV(8200.0), 	215 },//
	{ OV(9500.0), 	205 },//
	{ OV(11200.0), 	195 },//
	{ OV(12800.0), 	186 },//
	{ OV(16700.0), 	174 },//	
	{ OV(19800.0), 	159 },//
	{ OV(23200.0), 	148 },//
	{ OV(27400.0), 	138 },//
	{ OV(31700.0), 	130 },//
	{ OV(35200.0), 	120 },//
	{ OV(39800.0), 	112 },//
	{ OV(41500.0), 	100 },//
	{ OV(46400.0), 	93 },//
	{ OV(50400.0), 	80 },//
	{ OV(53600.0), 	72 },//
	{ OV(57500.0), 	57 },//
	{ OV(61700.0), 	33 },//
	{ OV(62400.0), 	27 },//
	{ OV(62700.0), 	25 },//
	{ OV(63000.0), 	24 },//
	{ OV(64000.0), 	23 },//
	{ OV(65000.0), 	1 },
	{ OV(65535.0), 	0 }
  // { OV( 10), 300 },
  // { OV( 29), 280 },//
  // { OV( 30), 260 },//
  // { OV( 17), 240 },//
  /////////////////////////
  // { OV( 22), 220 },//
  // { OV( 28), 200 },//
  // { OV( 35), 180 },//
  // { OV( 44), 160 },//
  // { OV( 56), 140 },//
  // { OV( 80), 120 },//
  // { OV( 123), 100 },//
  // { OV( 192),  80 },//
  // { OV( 300),  60 },//
  // { OV( 480),  40 },//
  // { OV( 590),  25 },//100k//123 - 25
  // { OV(1020), -15 }
  ////////////////////////////////
  // { OV(  23), 300 },
  // { OV( 0), 280 },//
  // { OV( 0), 260 },//
  // { OV( 0), 240 },//
  // { OV(  24), 220 },//
  // { OV( 0), 200 },//
  // { OV( 100), 180 },//
  // { OV( 0), 160 },//
  // { OV( 0), 140 },//
  // { OV( 0), 120 },//
  // { OV( 0), 100 },//
  // { OV( 0),  80 },//
  // { OV( 0),  60 },//
  // { OV( 0),  40 },//
  // { OV( 590),  25 },//100k//123 - 25
  // { OV(1020), -15 }
};
