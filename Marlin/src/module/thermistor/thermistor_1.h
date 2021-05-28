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

// R25 = 100 kOhm, beta25 = 4092 K, 4.7 kOhm pull-up, bed thermistor
const long temptable_1[][2] PROGMEM = {
  { OV(  1472), 300 },
  { OV(  1600), 295 },
  { OV(  1728), 290 },
  { OV(  1792), 285 },
  { OV(  1984), 280 },
  { OV(  2112), 275 },
  { OV(  2240), 270 },
  { OV(  2432), 265 },
  { OV(  2624), 260 },
  { OV(  2816), 255 },
  { OV(  3072), 250 },
  { OV(  3328), 245 },
  { OV(  3584), 240 },
  { OV(  3904), 235 },
  { OV(  4224), 230 },
  { OV(  4544), 225 },
  { OV(  4992), 220 },
  { OV(  5376), 215 },
  { OV(  5888), 210 },
  { OV(  6400), 205 },
  { OV(  6976), 200 },
  { OV(  7680), 195 },
  { OV(  8384), 190 },
  { OV(  9152), 185 },
  { OV(  9984), 180 },
  { OV( 10944), 175 },
  { OV( 11968), 170 },
  { OV( 13120), 165 },
  { OV( 14336), 160 },
  { OV( 15680), 155 },
  { OV( 17152), 150 },
  { OV( 18752), 145 },
  { OV( 20480), 140 },
  { OV( 22272), 135 },
  { OV( 24256), 130 },
  { OV( 26304), 125 },
  { OV( 28480), 120 },
  { OV( 30720), 115 },
  { OV( 33024), 110 },
  { OV( 35392), 105 },
  { OV( 37824), 100 },
  { OV( 40192),  95 },
  { OV( 42560),  90 },
  { OV( 44928),  85 },
  { OV( 47168),  80 },
  { OV( 49280),  75 },
  { OV( 51264),  70 },
  { OV( 53120),  65 },
  { OV( 54848),  60 },
  { OV( 56384),  55 },
  { OV( 57792),  50 },
  { OV( 59008),  45 },
  { OV( 60096),  40 },
  { OV( 61056),  35 },
  { OV( 61824),  30 },
  { OV( 62528),  25 },
  { OV( 63040),  20 },
  { OV( 63552),  15 },
  { OV( 63936),  10 },
  { OV( 64256),   5 },
  { OV( 64512),   0 },
  { OV( 64768),  -5 },
  { OV( 65024), -10 },
  { OV( 65280), -15 }
};
