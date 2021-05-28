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


void MarlinUI::menu_recovery() {
	draw_menu_recovery();
}

void MarlinUI::draw_menu_recovery() {  
  bool redrawall = (prevScreen != currentScreen);  
  if(redrawall){
    DrawPhaseOne = false; 
    DrawPhaseTwo = false;
  }
  prevScreen = currentScreen;
  uint8_t drawLayer = MAIN_LAYER;
  uint8_t backLayer = MAIN_LAYER;
  if(!DrawPhaseOne){
    drawLayer = MAIN_LAYER;
    backLayer = MAIN_LAYER;
  }
  bool redrawPics = (!DrawPhaseOne || !DrawPhaseTwo);
  /*****************************************************************/  
  if(redrawall)TFT_FillScreen(0xFF231F20,MAIN_LAYER);

  if(redrawall){
    TFT_DrawRectangle(/*x,y*/100,200, /*width, height*/250,200, /*round corner*/5,/*color*/TTZ_COL_ORAN,/*back_color*/TTZ_COL_BAGR,MAIN_LAYER,/*thikness in px*/2,/*fill?*/1,/*punktir?*/0);
	  TFT_DrawRectangle(/*x,y*/450,200, /*width, height*/250,200, /*round corner*/5,/*color*/TTZ_COL_ORAN,/*back_color*/TTZ_COL_BAGR,MAIN_LAYER,/*thikness in px*/2,/*fill?*/1,/*punktir?*/0);
	
    
    TFT_DrawText(TFT_FONT24,120, 300, "RESUME", 20, drawLayer, backLayer,0x0);
    TFT_DrawText(TFT_FONT24,470, 300, "CANCEL", 20, drawLayer, backLayer,0x0);














    // draw_right_panel(currentScreen);
  }


  // ACTION_ITEM(MSG_RESUME_PRINT, lcd_power_loss_recovery_resume);
  // ACTION_ITEM(MSG_STOP_PRINT, lcd_power_loss_recovery_cancel);

  MarlinUI::hotendActive = active_extruder;
  /*****************************************************************/
  if(redrawall){
    // RewriteMemory(&hsdram1,LCD_FRAME_BUFFER(1),LCD_FRAME_BUFFER(3));
    // HAL_LTDC_SetAddress(&hltdc,LCD_FRAME_BUFFER(3), 0);
    // RewriteMemory(&hsdram1,LCD_FRAME_BUFFER(1),LCD_FRAME_BUFFER(0));
    // HAL_LTDC_SetAddress(&hltdc,LCD_FRAME_BUFFER(0), 0);
    DrawPhaseOne = true;
    DrawPhaseTwo = false;
  }else{
    DrawPhaseOne = true; 
    DrawPhaseTwo = true;
  }
}