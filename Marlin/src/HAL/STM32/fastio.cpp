/**
 * Marlin 3D Printer Firmware
 * Copyright (c) 2020 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
 *
 * Based on Sprinter and grbl.
 * Copyright (c) 2011 Camiel Gubbels / Erik van der Zalm
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
#if defined(ARDUINO_ARCH_STM32) && !defined(STM32GENERIC)

#include "../../inc/MarlinConfig.h"

GPIO_TypeDef* FastIOPortMap[LastPort + 1];

void FastIO_init() {
  LOOP_L_N(i, NUM_DIGITAL_PINS)
    FastIOPortMap[STM_PORT(digitalPin[i])] = get_GPIO_Port(STM_PORT(digitalPin[i]));
}
void pinSwitchFunc(uint32_t ulPin, bool ulVal){
  // GPIO_TypeDef* GPIOxx = (GPIO_TypeDef*)(0x40000000UL + 0x00020000UL+(0x0400UL*(((uint32_t)(digitalPin[ulPin]) >> 4) & 0xF)));


// if(ulPin > 130){

//   if(ulVal == true)
//   {

//   }
// }
////////////////////////////////////////////////////////////////////////////////////////////////
if((((uint32_t)(digitalPin[ulPin]) >> 4) & 0xF) == 0){//port A
  ((GPIO_TypeDef*)(AHB1PERIPH_BASE+(0x0400UL*(((uint32_t)(digitalPin[ulPin]) >> 4) & 0xF)))) -> BSRR = 1UL << (((uint32_t)(digitalPin[ulPin]) & 0xF) + ((ulVal) ? 0 : 16));
}
////////////////////////////////////////////////////////////////////////////////////////////////
if((((uint32_t)(digitalPin[ulPin]) >> 4) & 0xF) == 1){//port B
  ((GPIO_TypeDef*)(AHB1PERIPH_BASE+(0x0400UL*(((uint32_t)(digitalPin[ulPin]) >> 4) & 0xF)))) -> BSRR = 1UL << (((uint32_t)(digitalPin[ulPin]) & 0xF) + ((ulVal) ? 0 : 16));
}
////////////////////////////////////////////////////////////////////////////////////////////////
if((((uint32_t)(digitalPin[ulPin]) >> 4) & 0xF) == 2){//port C
  ((GPIO_TypeDef*)(AHB1PERIPH_BASE+(0x0400UL*(((uint32_t)(digitalPin[ulPin]) >> 4) & 0xF)))) -> BSRR = 1UL << (((uint32_t)(digitalPin[ulPin]) & 0xF) + ((ulVal) ? 0 : 16));
}
////////////////////////////////////////////////////////////////////////////////////////////////
if((((uint32_t)(digitalPin[ulPin]) >> 4) & 0xF) == 3){//port D
  ((GPIO_TypeDef*)(AHB1PERIPH_BASE+(0x0400UL*(((uint32_t)(digitalPin[ulPin]) >> 4) & 0xF)))) -> BSRR = 1UL << (((uint32_t)(digitalPin[ulPin]) & 0xF) + ((ulVal) ? 0 : 16));
}
////////////////////////////////////////////////////////////////////////////////////////////////
if((((uint32_t)(digitalPin[ulPin]) >> 4) & 0xF) == 4){//port E
  ((GPIO_TypeDef*)(AHB1PERIPH_BASE+(0x0400UL*(((uint32_t)(digitalPin[ulPin]) >> 4) & 0xF)))) -> BSRR = 1UL << (((uint32_t)(digitalPin[ulPin]) & 0xF) + ((ulVal) ? 0 : 16));
}
////////////////////////////////////////////////////////////////////////////////////////////////
if((((uint32_t)(digitalPin[ulPin]) >> 4) & 0xF) == 5){//port F
  ((GPIO_TypeDef*)(AHB1PERIPH_BASE+(0x0400UL*(((uint32_t)(digitalPin[ulPin]) >> 4) & 0xF)))) -> BSRR = 1UL << (((uint32_t)(digitalPin[ulPin]) & 0xF) + ((ulVal) ? 0 : 16));
}
////////////////////////////////////////////////////////////////////////////////////////////////
if((((uint32_t)(digitalPin[ulPin]) >> 4) & 0xF) == 6){//port G
  ((GPIO_TypeDef*)(AHB1PERIPH_BASE+(0x0400UL*(((uint32_t)(digitalPin[ulPin]) >> 4) & 0xF)))) -> BSRR = 1UL << (((uint32_t)(digitalPin[ulPin]) & 0xF) + ((ulVal) ? 0 : 16));
}
////////////////////////////////////////////////////////////////////////////////////////////////
if((((uint32_t)(digitalPin[ulPin]) >> 4) & 0xF) == 7){//port H
  ((GPIO_TypeDef*)(AHB1PERIPH_BASE+(0x0400UL*(((uint32_t)(digitalPin[ulPin]) >> 4) & 0xF)))) -> BSRR = 1UL << (((uint32_t)(digitalPin[ulPin]) & 0xF) + ((ulVal) ? 0 : 16));
}
////////////////////////////////////////////////////////////////////////////////////////////////
if((((uint32_t)(digitalPin[ulPin]) >> 4) & 0xF) == 8){//port I
  ((GPIO_TypeDef*)(AHB1PERIPH_BASE+(0x0400UL*(((uint32_t)(digitalPin[ulPin]) >> 4) & 0xF)))) -> BSRR = 1UL << (((uint32_t)(digitalPin[ulPin]) & 0xF) + ((ulVal) ? 0 : 16));
}
////////////////////////////////////////////////////////////////////////////////////////////////
if((((uint32_t)(digitalPin[ulPin]) >> 4) & 0xF) == 9){//port J
  ((GPIO_TypeDef*)(AHB1PERIPH_BASE+(0x0400UL*(((uint32_t)(digitalPin[ulPin]) >> 4) & 0xF)))) -> BSRR = 1UL << (((uint32_t)(digitalPin[ulPin]) & 0xF) + ((ulVal) ? 0 : 16));
}
////////////////////////////////////////////////////////////////////////////////////////////////
if((((uint32_t)(digitalPin[ulPin]) >> 4) & 0xF) == 10){//port K
  ((GPIO_TypeDef*)(AHB1PERIPH_BASE+(0x0400UL*(((uint32_t)(digitalPin[ulPin]) >> 4) & 0xF)))) -> BSRR = 1UL << (((uint32_t)(digitalPin[ulPin]) & 0xF) + ((ulVal) ? 0 : 16));
}

//   GPIOx -> BSRR = (1UL << (((uint32_t)(digitalPin[ulPin]) & 0xF) + ((ulVal) ? 0 : 16)));
// ((GPIO_TypeDef*)(AHB1PERIPH_BASE+(0x0400UL*(((uint32_t)(digitalPin[ulPin]) >> 4) & 0xF)))) -> BSRR = 1UL << (((uint32_t)(digitalPin[ulPin]) & 0xF) + ((ulVal) ? 0 : 16));

//  if(ulVal == true)
//  {
// 	  GPIOxx -> BSRR = ((uint32_t)(1UL << ((uint32_t)(digitalPin[ulPin]) & 0xF)));
//  }
//  else
//  {
// 	  GPIOxx -> BSRR = (uint32_t)(1UL << (((uint32_t)(digitalPin[ulPin]) & 0xF))) << 16U;
//  }

  // FastIOPortMap[(((uint32_t)(digitalPin[ulPin]) >> 4) & 0xF)] -> BSRR = (1UL << (((uint32_t)(digitalPin[ulPin]) & 0xF) + ((ulVal) ? 0 : 16)));
}
#endif
