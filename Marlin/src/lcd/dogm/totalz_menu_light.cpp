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

//
// status_screen_DOGM.cpp
// Standard Status Screen for Graphical Display
//

#include "../../inc/MarlinConfigPre.h"

#include "ststusScreen_dimensions.h"
#include "dogm_Statusscreen.h"
#include "ultralcd_DOGM.h"
#include "../ultralcd.h"
#include "../LTDC/ltdc.h"
#include "../lcdprint.h"
#include "../../libs/numtostr.h"

#include "../../module/motion.h"
#include "../../module/temperature.h"
#include "../../core/w25qxx.h"

extern LCD_DrawPropTypeDef lcdprop;

#define FONT_W(N)    (lcdprop.pFont[N]->Width)
#define FONT_H(N)    (lcdprop.pFont[N]->Height)



void MarlinUI::menu_light() {
  prevScreen = currentScreen;
   HAL_LTDC_SetAddress(&hltdc,LCD_FRAME_BUFFER(LAYER_LIGHT), 0);
}

void MarlinUI::draw_menu_light() {


  // uint8_t drawLayer = LAYER_LIGHT;
  // uint8_t backLayer = LAYER_LIGHT;

  
  TFT_FillScreen(0xFF231F20,LAYER_LIGHT);
  draw_right_panel(menu_light);



  MarlinUI::hotendActive = active_extruder;
  /*****************************************************************/
  // if(redrawall){
  //   RewriteMemory(&hsdram1,LCD_FRAME_BUFFER(1),LCD_FRAME_BUFFER(3));
  //   HAL_LTDC_SetAddress(&hltdc,LCD_FRAME_BUFFER(3), 0);
  //   RewriteMemory(&hsdram1,LCD_FRAME_BUFFER(1),LCD_FRAME_BUFFER(0));
  //   HAL_LTDC_SetAddress(&hltdc,LCD_FRAME_BUFFER(0), 0);
  //   DrawPhaseOne = true;
  //   DrawPhaseTwo = false;
  // }else{
  //   DrawPhaseOne = true; 
  //   DrawPhaseTwo = true;
  // }
}