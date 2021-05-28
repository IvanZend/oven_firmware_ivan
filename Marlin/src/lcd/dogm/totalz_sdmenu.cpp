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
#include "../../sd/cardreader.h"

extern LCD_DrawPropTypeDef lcdprop;


void MarlinUI::menu_sdmain() {
	draw_menu_sdmain();
}

void MarlinUI::draw_menu_sdmain() {  
  bool redrawall = (prevScreen != currentScreen);  
  if(redrawall){
    DrawPhaseOne = false; 
    DrawPhaseTwo = false;
  }
  prevScreen = currentScreen;
  uint8_t drawLayer = MAIN_LAYER;
  uint8_t backLayer = EXTRA_LAYER;
  if(!DrawPhaseOne){
    drawLayer = EXTRA_LAYER;
    backLayer = EXTRA_LAYER;
  }
  bool redrawPics = (!DrawPhaseOne);
  /*****************************************************************/
  if(redrawPics){
    // TFT_DrawRectangle(/*x,y*/150,100, /*width, height*/500,400, /*round corner*/8,/*color*/TTZ_COL_ORAN,/*back_color*/0x0,MAIN_LAYER,/*thikness in px*/201,/*fill?*/1,/*punktir?*/0);
    TFT_DrawRectangle(/*x,y*/150,502, /*width, height*/100,50, /*round corner*/8,/*color*/TTZ_COL_ORAN,/*back_color*/0x0,MAIN_LAYER,/*thikness in px*/38,/*fill?*/1,/*punktir?*/0);
    TFT_DrawRectangle(/*x,y*/252,502, /*width, height*/100,50, /*round corner*/8,/*color*/TTZ_COL_ORAN,/*back_color*/0x0,MAIN_LAYER,/*thikness in px*/38,/*fill?*/1,/*punktir?*/0);
    TFT_DrawRectangle(/*x,y*/448,502, /*width, height*/100,50, /*round corner*/8,/*color*/TTZ_COL_ORAN,/*back_color*/0x0,MAIN_LAYER,/*thikness in px*/38,/*fill?*/1,/*punktir?*/0);
    TFT_DrawRectangle(/*x,y*/550,502, /*width, height*/100,50, /*round corner*/8,/*color*/TTZ_COL_ORAN,/*back_color*/0x0,MAIN_LAYER,/*thikness in px*/38,/*fill?*/1,/*punktir?*/0);
    LTDC_IconFromLayer(530, 522, 51, 31, LCD_COLOR_WHITE, AXIS_ARROW_LARGE_TOP_PIC_START, FLASH_LAYER1, MAIN_LAYER,1,0);
    LTDC_IconFromLayer(180, 502, 51, 31, LCD_COLOR_WHITE, AXIS_ARROW_LARGE_TOP_PIC_START, FLASH_LAYER1, MAIN_LAYER,3,0);
    TFT_DrawText(TFT_FONT24,449, 515, "Превью", 0, MAIN_LAYER, MAIN_LAYER,0x0);
    TFT_DrawText(TFT_FONT24,557, 515, "Старт",  0, MAIN_LAYER, MAIN_LAYER,0x0);
    card.reset_page();
    card.ls(true,sdFilesPosition);
    for(int i=0;i<15;i++){
      // TFT_DrawFLatLine(155,125+i*24, 645, 125+i*24, TTZ_COL_D_GR, MAIN_LAYER);
    }
      
  }


  if(redrawall){
    // RewriteMemory(&hsdram1,LCD_FRAME_BUFFER(1),LCD_FRAME_BUFFER(3));
    // HAL_LTDC_SetAddress(&hltdc,LCD_FRAME_BUFFER(3), 0);
    // RewriteMemory(&hsdram1,LCD_FRAME_BUFFER(1),LCD_FRAME_BUFFER(0));
    // HAL_LTDC_SetAddress(&hltdc,LCD_FRAME_BUFFER(0), 0);
    DrawPhaseOne = true;
    DrawPhaseTwo = true;
  }else{
    DrawPhaseOne = true; 
    DrawPhaseTwo = true;
  }
}