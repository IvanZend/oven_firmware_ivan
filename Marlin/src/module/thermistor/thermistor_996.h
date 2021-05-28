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
const long temptable_996[][2] PROGMEM = {
//*64

/*
*
* 65520 <---> 0
*
*	30000 - 468 - 20
*/
/* ---- - --- - 340*/	
/* ---- - --- - 320*/	
/* ---- - --- - 300*/	
/* ---- - --- - 280*/	
/* ---- - --- - 260*/	
/* 1350 - --- - 240*/	
/* 1450 - --- - 220*/	
/* 1800 - --- - 200*/	
/* 2200 - --- - 180*/	
/* 2800 - --- - 160*/	
/* 3300 - --- - 150*/	
/* 3600 - --- - 145*/	
/* 4000 - --- - 140*/	
/* 5900 - --- - 120*/	
/* 9200 - --- - 100*/	
/*11000 - --- - 95*/	
/*12600 - --- - 90*/	
/*14000 - --- - 85*/	
/*15200 - --- - 80*/	
/*25500 - --- - 60*/	
/*59000 - 921 - 20*/	
	{ OV(0.0), 		499 },
	{ OV(10.0), 	240 },
	
	{ OV(28.0), 	220 },
	{ OV(31.0), 	210 },	
	{ OV(33.8), 	200 },
	{ OV(39.0), 	190 },	
	{ OV(43.0), 	180 },
	{ OV(47.0), 	170 },
	{ OV(52.0), 	160 },
	{ OV(67.0), 	150 },
	{ OV(80.0), 	140 },	
	{ OV(135.0), 	110 },		
	{ OV(110.0), 	120 },	
	
	{ OV(167.8), 	100 },
	{ OV(211.0), 	90 },
	
	
	{ OV(294.5), 	75 },	
	
	{ OV(413.0), 	60 },
	{ OV(500.0), 	50 },
	// { OV(590.0), 	40 },		
	{ OV(660.0), 	35 },		
	// { OV(700.0), 	30 },
	{ OV(760.0), 	25 },
	{ OV(1023.0), 	0 }
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
