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

void MarlinUI::menu_filament() {
	// draw_menu_filament();
  prevScreen = currentScreen;
  HAL_LTDC_SetAddress(&hltdc,LCD_FRAME_BUFFER(LAYER_MAT), 0);
}

void MarlinUI::draw_menu_filament() {  

  uint8_t drawLayer = LAYER_MAT;
  uint8_t backLayer = LAYER_MAT;

  /*****************************************************************/

    TFT_FillScreen(TTZ_COL_BAGR,LAYER_MAT);
    
    TFT_DrawRectangle(/*x,y*/ 34,85,  /*width, height*/319,34, /*round corner*/5,/*color*/TTZ_COL_ORAN,/*back_color*/TTZ_COL_BAGR,LAYER_MAT,/*thikness in px*/2,/*fill?*/0,/*punktir?*/0);
    TFT_DrawRectangle(/*x,y*/367,85,  /*width, height*/319,34, /*round corner*/5,/*color*/TTZ_COL_ORAN,/*back_color*/TTZ_COL_BAGR,LAYER_MAT,/*thikness in px*/2,/*fill?*/0,/*punktir?*/0);
   
    TFT_DrawRectangle(/*x,y*/ 34,129, /*width, height*/105,23, /*round corner*/4,/*color*/TTZ_COL_ORAN,/*back_color*/TTZ_COL_BAGR,LAYER_MAT,/*thikness in px*/2,/*fill?*/0,/*punktir?*/0);
    TFT_DrawRectangle(/*x,y*/141,129, /*width, height*/105,23, /*round corner*/4,/*color*/TTZ_COL_ORAN,/*back_color*/TTZ_COL_BAGR,LAYER_MAT,/*thikness in px*/2,/*fill?*/0,/*punktir?*/0);
    TFT_DrawRectangle(/*x,y*/248,129, /*width, height*/105,23, /*round corner*/4,/*color*/TTZ_COL_ORAN,/*back_color*/TTZ_COL_BAGR,LAYER_MAT,/*thikness in px*/2,/*fill?*/0,/*punktir?*/0);
       
    TFT_DrawRectangle(/*x,y*/367,129, /*width, height*/105,23, /*round corner*/4,/*color*/TTZ_COL_ORAN,/*back_color*/TTZ_COL_BAGR,LAYER_MAT,/*thikness in px*/2,/*fill?*/0,/*punktir?*/0);
    TFT_DrawRectangle(/*x,y*/474,129, /*width, height*/105,23, /*round corner*/4,/*color*/TTZ_COL_ORAN,/*back_color*/TTZ_COL_BAGR,LAYER_MAT,/*thikness in px*/2,/*fill?*/0,/*punktir?*/0);
    TFT_DrawRectangle(/*x,y*/581,129, /*width, height*/105,23, /*round corner*/4,/*color*/TTZ_COL_ORAN,/*back_color*/TTZ_COL_BAGR,LAYER_MAT,/*thikness in px*/2,/*fill?*/0,/*punktir?*/0);
    
    TFT_DrawRectangle(/*x,y*/ 51,163, /*width, height*/43,43, /*round corner*/4,/*color*/TTZ_COL_ORAN,/*back_color*/TTZ_COL_BAGR,LAYER_MAT,/*thikness in px*/2,/*fill?*/0,/*punktir?*/0);
    TFT_DrawRectangle(/*x,y*/295,163, /*width, height*/43,43, /*round corner*/4,/*color*/TTZ_COL_ORAN,/*back_color*/TTZ_COL_BAGR,LAYER_MAT,/*thikness in px*/2,/*fill?*/0,/*punktir?*/0);
    TFT_DrawRectangle(/*x,y*/385,163, /*width, height*/43,43, /*round corner*/4,/*color*/TTZ_COL_ORAN,/*back_color*/TTZ_COL_BAGR,LAYER_MAT,/*thikness in px*/2,/*fill?*/0,/*punktir?*/0);
    TFT_DrawRectangle(/*x,y*/629,163, /*width, height*/43,43, /*round corner*/4,/*color*/TTZ_COL_ORAN,/*back_color*/TTZ_COL_BAGR,LAYER_MAT,/*thikness in px*/2,/*fill?*/0,/*punktir?*/0);
    
    TFT_DrawRectangle(/*x,y*/ 34,216, /*width, height*/319,34, /*round corner*/3,/*color*/TTZ_COL_ORAN,/*back_color*/TTZ_COL_BAGR,LAYER_MAT,/*thikness in px*/18,/*fill?*/1,/*punktir?*/0);
    TFT_DrawRectangle(/*x,y*/367,216, /*width, height*/319,34, /*round corner*/3,/*color*/TTZ_COL_ORAN,/*back_color*/TTZ_COL_BAGR,LAYER_MAT,/*thikness in px*/18,/*fill?*/1,/*punktir?*/0);
    
    TFT_DrawRectangle(/*x,y*/ 34,260, /*width, height*/319,308, /*round corner*/5,/*color*/TTZ_COL_ORAN,/*back_color*/TTZ_COL_BAGR,LAYER_MAT,/*thikness in px*/2,/*fill?*/0,/*punktir?*/0);
    TFT_DrawRectangle(/*x,y*/367,260, /*width, height*/319,308, /*round corner*/5,/*color*/TTZ_COL_ORAN,/*back_color*/TTZ_COL_BAGR,LAYER_MAT,/*thikness in px*/2,/*fill?*/0,/*punktir?*/0);
    

  	LTDC_drawSetParams(249,267,LAYER_MAT, 0);
  	LTDC_drawSetParams(583,267,LAYER_MAT, 0);
  
    TFT_DrawRectangle(/*x,y*/54,339, /*width, height*/137,43, /*round corner*/5,/*color*/TTZ_COL_ORAN,/*back_color*/TTZ_COL_BAGR,LAYER_MAT,/*thikness in px*/2,/*fill?*/0,/*punktir?*/0);
    TFT_DrawRectangle(/*x,y*/54,392, /*width, height*/137,43, /*round corner*/5,/*color*/TTZ_COL_ORAN,/*back_color*/TTZ_COL_BAGR,LAYER_MAT,/*thikness in px*/22,/*fill?*/1,/*punktir?*/0);
    TFT_DrawRectangle(/*x,y*/54,457, /*width, height*/137,43, /*round corner*/5,/*color*/TTZ_COL_ORAN,/*back_color*/TTZ_COL_BAGR,LAYER_MAT,/*thikness in px*/22,/*fill?*/1,/*punktir?*/0);
    TFT_DrawRectangle(/*x,y*/54,509, /*width, height*/137,43, /*round corner*/5,/*color*/TTZ_COL_ORAN,/*back_color*/TTZ_COL_BAGR,LAYER_MAT,/*thikness in px*/22,/*fill?*/1,/*punktir?*/0);
  
    TFT_DrawRectangle(/*x,y*/388,339, /*width, height*/137,43, /*round corner*/5,/*color*/TTZ_COL_ORAN,/*back_color*/TTZ_COL_BAGR,LAYER_MAT,/*thikness in px*/2,/*fill?*/0,/*punktir?*/0);
    TFT_DrawRectangle(/*x,y*/388,392, /*width, height*/137,43, /*round corner*/5,/*color*/TTZ_COL_ORAN,/*back_color*/TTZ_COL_BAGR,LAYER_MAT,/*thikness in px*/22,/*fill?*/1,/*punktir?*/0);
    TFT_DrawRectangle(/*x,y*/388,457, /*width, height*/137,43, /*round corner*/5,/*color*/TTZ_COL_ORAN,/*back_color*/TTZ_COL_BAGR,LAYER_MAT,/*thikness in px*/22,/*fill?*/1,/*punktir?*/0);
    TFT_DrawRectangle(/*x,y*/388,509, /*width, height*/137,43, /*round corner*/5,/*color*/TTZ_COL_ORAN,/*back_color*/TTZ_COL_BAGR,LAYER_MAT,/*thikness in px*/22,/*fill?*/1,/*punktir?*/0);
    
    TFT_DrawRectangle(/*x,y*/249,361, /*width, height*/90,90, /*round corner*/5,/*color*/TTZ_COL_ORAN,/*back_color*/TTZ_COL_BAGR,LAYER_MAT,/*thikness in px*/2,/*fill?*/0,/*punktir?*/0);
    TFT_DrawRectangle(/*x,y*/249,464, /*width, height*/90,90, /*round corner*/5,/*color*/TTZ_COL_ORAN,/*back_color*/TTZ_COL_BAGR,LAYER_MAT,/*thikness in px*/2,/*fill?*/0,/*punktir?*/0);
    TFT_DrawRectangle(/*x,y*/583,361, /*width, height*/90,90, /*round corner*/5,/*color*/TTZ_COL_ORAN,/*back_color*/TTZ_COL_BAGR,LAYER_MAT,/*thikness in px*/2,/*fill?*/0,/*punktir?*/0);
    TFT_DrawRectangle(/*x,y*/583,464, /*width, height*/90,90, /*round corner*/5,/*color*/TTZ_COL_ORAN,/*back_color*/TTZ_COL_BAGR,LAYER_MAT,/*thikness in px*/2,/*fill?*/0,/*punktir?*/0);
    
    draw_right_panel(menu_filament);

  bool drawAct = false;
  if(active_extruder!=MarlinUI::hotendActive)drawAct = true;
  MarlinUI::hotendActive = active_extruder;
  if(drawAct){
    /*EXTR 0*/
    LTDC_drawFilExtruder(54,32, LAYER_MAT, hotendActive==0);
    LTDC_drawFilExtruder(54,32, MAIN_LAYER, hotendActive==0);
    /*EXTR 1*/
    LTDC_drawFilExtruder(630,32, LAYER_MAT, hotendActive==1);
    LTDC_drawFilExtruder(630,32, MAIN_LAYER, hotendActive==1);
  }
  if(!drawAct /*&& redrawPics*/){
    /*EXTR 0*/    
    LTDC_drawFilExtruder(54,32, LAYER_MAT, hotendActive==0);
    /*EXTR 1*/ 
    LTDC_drawFilExtruder(630,32, LAYER_MAT, hotendActive==1);
  }
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