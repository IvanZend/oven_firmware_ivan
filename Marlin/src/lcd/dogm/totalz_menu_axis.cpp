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
#include "../../feature/babystep.h"

extern LCD_DrawPropTypeDef lcdprop;
extern Babystep babystep;


FORCE_INLINE void _draw_opt_axis_values(const AxisEnum axis, uint16_t xpos,uint16_t ypos, const char *value, const bool blink, bool drawPic, uint8_t textLayer, uint8_t backLayer) {
  // uint8_t len = t_shortlen(value);
  // xpos = xpos-(len-4)*lcdprop.pFont[TFT_FONT24]->Width;
  if (blink)
    TFT_DrawText(TFT_FONT24,xpos-40+FONT_W(TFT_FONT24), ypos, value,3,textLayer, backLayer,0x0);
  else {
    if (!TEST(axis_homed, axis)){
      uint8_t i=1;
      while (const char c = *value++) {
        TFT_DrawChar(TFT_FONT24,(xpos-40)+(FONT_W(TFT_FONT24)*i++), ypos, c <= '.' ? c : '?',3,textLayer, backLayer,0x0);
      }
    }else {
        if (!TEST(axis_known_position, axis))
          TFT_DrawText(TFT_FONT24,(xpos-40)+FONT_W(TFT_FONT24), ypos, axis == Z_AXIS ? PSTR("       ") : PSTR("    "),3,textLayer, backLayer,0x0);
        else
          TFT_DrawText(TFT_FONT24,(xpos-40)+FONT_W(TFT_FONT24), ypos, value,3,textLayer, backLayer,0x0);
    }
  }
}

void MarlinUI::menu_axis() {
	// draw_menu_options();
  prevScreen = currentScreen;
  HAL_LTDC_SetAddress(&hltdc,LCD_FRAME_BUFFER(LAYER_AXIS), 0);
  menu_axis_refresh();
}

void MarlinUI::menu_axis_refresh() {
  /*
  *
  * dynamic objects
  *
  */
  uint8_t drawLayer = LAYER_AXIS;
  uint8_t backLayer = LAYER_AXIS_2;


  const bool blink = get_blink();

  static char xstring[5], ystring[5], zstring[8];
  const xyz_pos_t lpos = current_position.asLogical();
  strcpy(xstring, ftostr4sign(lpos.x));
  strcpy(ystring, ftostr4sign(lpos.y));
  strcpy(zstring, ftostr52sp(lpos.z));
  TFT_SetTextColor(LCD_COLOR_BLACK);
  _draw_opt_axis_values(X_AXIS, 76,537, xstring, blink,true, drawLayer, backLayer);
  _draw_opt_axis_values(Y_AXIS,232,537, ystring, blink,true, drawLayer, backLayer);
  _draw_opt_axis_values(Z_AXIS,388,537, zstring, blink,true, drawLayer, backLayer);  



  /*drawing Z-offset (babysteps)*/  
  char str[10];
  const char *tmpSign = (home_offset.pos[Z_AXIS] < 0) ? "-" : "";
  float tmpVal = (home_offset.pos[Z_AXIS] < 0) ? -home_offset.pos[Z_AXIS] : home_offset.pos[Z_AXIS];

  int tmpInt = tmpVal;                  // Get the integer (678).
  float tmpFrac = tmpVal - tmpInt;      // Get fraction (0.0123).

  float tmpFrac1 = tmpFrac*10;           // Fraction x10 (0.123)
  int n1 = trunc(tmpFrac1);  // Turn into integer (0).

  float tmpFrac2 = (tmpFrac1-n1)*10;          // Fraction x10 (1.23)
  int n2 = roundf(tmpFrac2);  // Turn into integer (1).
  if(n2>9){n1++;n2=0;}
  if(n1>9){tmpInt++;n1=0;}

  // Print as parts, note that you need 0-padding for fractional bit.

  sprintf (str, "%s%d.%d%d", tmpSign, tmpInt, n1, n2);
  // sprintf (str, "%.2f", home_offset.pos[Z_AXIS]);
  delta_z_available_new = (DISABLED(BABYSTEP_WITHOUT_HOMING) && !TEST(axis_known_position, Z_AXIS));
  if(delta_z_available_new != delta_z_available_old){
    TFT_DrawRectangle(/*x,y*/35,232, /*width, height*/90,90, /*round corner*/8,/*color*/delta_z_available_new?LCD_COLOR_DARKGRAY:TTZ_COL_ORAN,/*back_color*/TTZ_COL_BAGR,LAYER_AXIS,/*thikness in px*/46,/*fill?*/1,/*punktir?*/0);
    LTDC_IconFromLayer(56, 266, 44, 25, 0xFFFFFFFF, AXIS_LETTER_DZ_LARGE_PIC_START, FLASH_LAYER1, LAYER_AXIS,0,0);
    delta_z_available_old = delta_z_available_new;
  }
  
  
  TFT_SetTextColor(babystep.bbst_homed?LCD_COLOR_BLACK:LCD_COLOR_GRAY);
  TFT_DrawText(TFT_FONT24,586, 537, str, 4, drawLayer, backLayer,0x0);
  TFT_SetTextColor(LCD_COLOR_WHITE);

    
  
  bool drawAct = false;
  if(active_extruder!=MarlinUI::hotendActive)drawAct = true;
  MarlinUI::hotendActive = active_extruder;
  if(drawAct){
    /*EXTR 0*/
    LTDC_drawOptExtruder(596,232, LAYER_AXIS, hotendActive==0);
    // LTDC_drawOptExtruder(596,232, MAIN_LAYER, hotendActive==0);
    /*EXTR 1*/
    LTDC_drawOptExtruder(596,277, LAYER_AXIS, hotendActive==1);
    // LTDC_drawOptExtruder(596,277, MAIN_LAYER, hotendActive==1);
  }
  if(!drawAct /* && redrawPics*/){
    /*EXTR 0*/    
    LTDC_drawOptExtruder(596,232, LAYER_AXIS, hotendActive==0);
    /*EXTR 1*/ 
    LTDC_drawOptExtruder(596,277, LAYER_AXIS, hotendActive==1);
  }




}
void MarlinUI::draw_menu_options() {



  /*****************************************************************/

  TFT_FillScreen(TTZ_COL_BAGR,LAYER_AXIS);

  delta_z_available_old = (DISABLED(BABYSTEP_WITHOUT_HOMING) && !TEST(axis_known_position, Z_AXIS));
  delta_z_available_new = delta_z_available_old;
  /*left screen*/
  LTDC_drawOptionsRectBot(35,37,babystep.bbst_homed?TTZ_COL_ORAN:LCD_COLOR_DARKGRAY,TTZ_COL_BAGR,LAYER_AXIS);
  LTDC_IconFromLayer(54, 41, 51, 31, babystep.bbst_homed?TTZ_COL_ORAN:LCD_COLOR_DARKGRAY, AXIS_ARROW_LARGE_TOP_PIC_START, FLASH_LAYER1, LAYER_AXIS,0,0);
  LTDC_IconFromLayer(54, 72, 51, 31, babystep.bbst_homed?TTZ_COL_ORAN:LCD_COLOR_DARKGRAY, AXIS_ARROW_LARGE_TOP_PIC_START, FLASH_LAYER1, LAYER_AXIS,0,0); 
  TFT_DrawText(TFT_FONT16,54, 113, PSTR("0,1 мм"),0,LAYER_AXIS, LAYER_AXIS,0x0);

  LTDC_drawOptionsRectBot(35,134,babystep.bbst_homed?TTZ_COL_ORAN:LCD_COLOR_DARKGRAY,TTZ_COL_BAGR,LAYER_AXIS);    
  LTDC_IconFromLayer(54, 142, 51, 61, babystep.bbst_homed?TTZ_COL_ORAN:LCD_COLOR_DARKGRAY, AXIS_ARROW_EXLARGE_TOP_PIC_START, FLASH_LAYER1, LAYER_AXIS,0,0);
  TFT_DrawText(TFT_FONT16,44, 210, PSTR("0,01 мм"),0,LAYER_AXIS, LAYER_AXIS,0x0);

  TFT_DrawRectangle(/*x,y*/35,232, /*width, height*/90,90, /*round corner*/8,/*color*/delta_z_available_new?LCD_COLOR_DARKGRAY:TTZ_COL_ORAN,/*back_color*/TTZ_COL_BAGR,LAYER_AXIS,/*thikness in px*/46,/*fill?*/1,/*punktir?*/0);
  LTDC_IconFromLayer(56, 266, 44, 25, 0xFFFFFFFF, AXIS_LETTER_DZ_LARGE_PIC_START, FLASH_LAYER1, LAYER_AXIS,0,0);
  
  LTDC_drawOptionsRectTop(35,330,babystep.bbst_homed?TTZ_COL_ORAN:LCD_COLOR_DARKGRAY,TTZ_COL_BAGR,LAYER_AXIS);
  LTDC_IconFromLayer(54, 353, 51, 61, babystep.bbst_homed?TTZ_COL_ORAN:LCD_COLOR_DARKGRAY, AXIS_ARROW_EXLARGE_TOP_PIC_START, FLASH_LAYER1, LAYER_AXIS,2,0);
  TFT_DrawText(TFT_FONT16,44, 332, PSTR("0,01 мм"),0,LAYER_AXIS, LAYER_AXIS,0x0);

  LTDC_drawOptionsRectTop(35,425,babystep.bbst_homed?TTZ_COL_ORAN:LCD_COLOR_DARKGRAY,TTZ_COL_BAGR,LAYER_AXIS);
  LTDC_IconFromLayer(52, 448, 51, 31, babystep.bbst_homed?TTZ_COL_ORAN:LCD_COLOR_DARKGRAY, AXIS_ARROW_LARGE_TOP_PIC_START, FLASH_LAYER1, LAYER_AXIS,2,0);
  LTDC_IconFromLayer(52, 479, 51, 31, babystep.bbst_homed?TTZ_COL_ORAN:LCD_COLOR_DARKGRAY, AXIS_ARROW_LARGE_TOP_PIC_START, FLASH_LAYER1, LAYER_AXIS,2,0);
  TFT_DrawText(TFT_FONT16,54, 427, PSTR("0,1 мм"),0,LAYER_AXIS, LAYER_AXIS,0x0);
  /*center screen*/
  TFT_DrawRectangle(/*x,y*/156,56, /*width, height*/286,37, /*round corner*/8,/*color*/TTZ_COL_ORAN,/*back_color*/TTZ_COL_BAGR,LAYER_AXIS,/*thikness in px*/2,/*fill?*/0,/*punktir?*/0);
  TFT_DrawFLatLine(257, 62,257, 86, TTZ_COL_ORAN, LAYER_AXIS);    
  TFT_DrawFLatLine(345, 62,345, 86, TTZ_COL_ORAN, LAYER_AXIS);    
  TFT_DrawRectangle(/*x,y*/163,67, /*width, height*/14,14, /*round corner*/7,/*color*/(opt_disctance == 1) ? TTZ_COL_ORAN : 0xFFFFFFFF,/*back_color*/TTZ_COL_BAGR,LAYER_AXIS,/*thikness in px*/7,/*fill?*/1,/*punktir?*/0);
  TFT_DrawRectangle(/*x,y*/262,67, /*width, height*/14,14, /*round corner*/7,/*color*/(opt_disctance == 2) ? TTZ_COL_ORAN : 0xFFFFFFFF,/*back_color*/TTZ_COL_BAGR,LAYER_AXIS,/*thikness in px*/7,/*fill?*/1,/*punktir?*/0);
  TFT_DrawRectangle(/*x,y*/351,67, /*width, height*/14,14, /*round corner*/7,/*color*/(opt_disctance == 3) ? TTZ_COL_ORAN : 0xFFFFFFFF,/*back_color*/TTZ_COL_BAGR,LAYER_AXIS,/*thikness in px*/7,/*fill?*/1,/*punktir?*/0);
  

  TFT_DrawRectangle(/*x,y*/156,232, /*width, height*/90,90, /*round corner*/8,/*color*/TTZ_COL_ORAN,/*back_color*/TTZ_COL_BAGR,LAYER_AXIS,/*thikness in px*/2,/*fill?*/0,/*punktir?*/0);
  LTDC_IconFromLayer(164, 261, 33, 33, TTZ_COL_ORAN, AXIS_ARROW_SMALL_TOP_PIC_START, FLASH_LAYER1, LAYER_AXIS,3,0);  
  LTDC_IconFromLayer(205, 262, 29, 35, TTZ_COL_ORAN, AXIS_LETTER_X_LARGE_PIC_START, FLASH_LAYER1, LAYER_AXIS,0,0);  
  TFT_DrawRectangle(/*x,y*/255,134, /*width, height*/90,90, /*round corner*/8,/*color*/TTZ_COL_ORAN,/*back_color*/TTZ_COL_BAGR,LAYER_AXIS,/*thikness in px*/2,/*fill?*/0,/*punktir?*/0);
  LTDC_IconFromLayer(284, 141, 33, 33, TTZ_COL_ORAN, AXIS_ARROW_SMALL_TOP_PIC_START, FLASH_LAYER1, LAYER_AXIS,0,0);
  LTDC_IconFromLayer(285, 181, 31, 35, TTZ_COL_ORAN, AXIS_LETTER_Y_LARGE_PIC_START, FLASH_LAYER1, LAYER_AXIS,0,0);  
  TFT_DrawRectangle(/*x,y*/354,232, /*width, height*/90,90, /*round corner*/8,/*color*/TTZ_COL_ORAN,/*back_color*/TTZ_COL_BAGR,LAYER_AXIS,/*thikness in px*/2,/*fill?*/0,/*punktir?*/0);
  LTDC_IconFromLayer(401, 261, 33, 33, TTZ_COL_ORAN, AXIS_ARROW_SMALL_TOP_PIC_START, FLASH_LAYER1, LAYER_AXIS,1,0);    
  LTDC_IconFromLayer(363, 262, 29, 35, TTZ_COL_ORAN, AXIS_LETTER_X_LARGE_PIC_START, FLASH_LAYER1, LAYER_AXIS,0,0);  
  TFT_DrawRectangle(/*x,y*/255,330, /*width, height*/90,90, /*round corner*/8,/*color*/TTZ_COL_ORAN,/*back_color*/TTZ_COL_BAGR,LAYER_AXIS,/*thikness in px*/2,/*fill?*/0,/*punktir?*/0);
  LTDC_IconFromLayer(284, 379, 33, 33, TTZ_COL_ORAN, AXIS_ARROW_SMALL_TOP_PIC_START, FLASH_LAYER1, LAYER_AXIS,2,0);
  LTDC_IconFromLayer(285, 338, 31, 35, TTZ_COL_ORAN, AXIS_LETTER_Y_LARGE_PIC_START, FLASH_LAYER1, LAYER_AXIS,0,0);  
  TFT_DrawRectangle(/*x,y*/255,232, /*width, height*/90,90, /*round corner*/8,/*color*/TTZ_COL_ORAN,/*back_color*/TTZ_COL_BAGR,LAYER_AXIS,/*thikness in px*/46,/*fill?*/1,/*punktir?*/0);
  LTDC_IconFromLayer(276, 238, 48, 52, 0xFFFFFFFF, RIGHT_PANEL_HOME_PIC_START, FLASH_LAYER1, LAYER_AXIS,0,0); 
  
  TFT_DrawRectangle(/*x,y*/277,294, /*width, height*/19,19, /*round corner*/9,/*color*/0xFFFFFFFF,/*back_color*/TTZ_COL_ORAN,LAYER_AXIS,/*thikness in px*/10,/*fill?*/1,/*punktir?*/0);
  TFT_DrawRectangle(/*x,y*/302,294, /*width, height*/19,19, /*round corner*/9,/*color*/0xFFFFFFFF,/*back_color*/TTZ_COL_ORAN,LAYER_AXIS,/*thikness in px*/10,/*fill?*/1,/*punktir?*/0);
  
  TFT_DrawChar(TFT_FONT16,196, 69, ',', 0, LAYER_AXIS, LAYER_AXIS,0x0);
  TFT_DrawText(TFT_FONT16,180, 68, PSTR("100 мм"),0,LAYER_AXIS, LAYER_AXIS,0x0);
  TFT_DrawChar(TFT_FONT16,286, 69, ',', 0, LAYER_AXIS, LAYER_AXIS,0x0);
  TFT_DrawText(TFT_FONT16,280, 68, PSTR("10 мм"),0,LAYER_AXIS, LAYER_AXIS,0x0);
  TFT_DrawChar(TFT_FONT16,375, 69, ',', 0, LAYER_AXIS, LAYER_AXIS,0x0);
  TFT_DrawText(TFT_FONT16,370, 68, PSTR("01 мм"),0,LAYER_AXIS, LAYER_AXIS,0x0);
  TFT_DrawText(TFT_FONT16,231, 101, PSTR("Шаг перемещения"),0,LAYER_AXIS, LAYER_AXIS,0x0);
  TFT_SetTextColor(TTZ_COL_ORAN);
  TFT_DrawChar(TFT_FONT16,281, 297, 'X',0,LAYER_AXIS, LAYER_AXIS,0x0);
  TFT_DrawChar(TFT_FONT16,306, 297, 'Y',0,LAYER_AXIS, LAYER_AXIS,0x0);
  TFT_SetTextColor(LCD_COLOR_WHITE);

  
  
  // LTDC_IconFromLayer(277, 294, 20, 20, AXIS_LETTER_X_SMALL_PIC_START, FLASH_LAYER1, LAYER_AXIS,0,0);
  // LTDC_IconFromLayer(302, 294, 21, 20, AXIS_LETTER_Y_SMALL_PIC_START, FLASH_LAYER1, LAYER_AXIS,0,0);     
  
  TFT_DrawRectangle(/*x,y*/255,425, /*width, height*/90,90, /*round corner*/8,/*color*/TTZ_COL_ORAN,/*back_color*/TTZ_COL_BAGR,LAYER_AXIS,/*thikness in px*/46,/*fill?*/1,/*punktir?*/0);
  LTDC_IconFromLayer(276, 432, 48, 52, 0xFFFFFFFF, RIGHT_PANEL_HOME_PIC_START, FLASH_LAYER1, LAYER_AXIS,0,0);  
  TFT_DrawRectangle(/*x,y*/262,490, /*width, height*/19,19, /*round corner*/9,/*color*/0xFFFFFFFF,/*back_color*/TTZ_COL_ORAN,LAYER_AXIS,/*thikness in px*/10,/*fill?*/1,/*punktir?*/0);
  TFT_DrawRectangle(/*x,y*/288,490, /*width, height*/19,19, /*round corner*/9,/*color*/0xFFFFFFFF,/*back_color*/TTZ_COL_ORAN,LAYER_AXIS,/*thikness in px*/10,/*fill?*/1,/*punktir?*/0);
  TFT_DrawRectangle(/*x,y*/314,490, /*width, height*/19,19, /*round corner*/9,/*color*/0xFFFFFFFF,/*back_color*/TTZ_COL_ORAN,LAYER_AXIS,/*thikness in px*/10,/*fill?*/1,/*punktir?*/0);
  TFT_SetTextColor(TTZ_COL_ORAN);
  TFT_DrawChar(TFT_FONT16,266, 493, 'X',0,LAYER_AXIS, LAYER_AXIS,0x0);
  TFT_DrawChar(TFT_FONT16,292, 493, 'Y',0,LAYER_AXIS, LAYER_AXIS,0x0);
  TFT_DrawChar(TFT_FONT16,318, 493, 'Z',0,LAYER_AXIS, LAYER_AXIS,0x0);
  TFT_SetTextColor(LCD_COLOR_WHITE);

  
  // LTDC_IconFromLayer(261, 490, 20, 20, AXIS_LETTER_X_SMALL_PIC_START, FLASH_LAYER1, LAYER_AXIS,0,0);   
  // LTDC_IconFromLayer(288, 490, 21, 20, AXIS_LETTER_Y_SMALL_PIC_START, FLASH_LAYER1, LAYER_AXIS,0,0);   
  // LTDC_IconFromLayer(314, 490, 21, 20, AXIS_LETTER_Z_SMALL_PIC_START, FLASH_LAYER1, LAYER_AXIS,0,0);   
  

  /*right screen*/
  /*top1 line*/
  LTDC_drawOptionsRectBot(474,37,TTZ_COL_ORAN,TTZ_COL_BAGR,LAYER_AXIS);
  LTDC_IconFromLayer(483, 44, 28, 17, TTZ_COL_ORAN, AXIS_ARROW_EXSMALL_TOP_PIC_START, FLASH_LAYER1, LAYER_AXIS,0,0);
  LTDC_IconFromLayer(483, 61, 28, 17, TTZ_COL_ORAN, AXIS_ARROW_EXSMALL_TOP_PIC_START, FLASH_LAYER1, LAYER_AXIS,0,0);  
  LTDC_IconFromLayer(523, 66, 29, 35, TTZ_COL_ORAN, AXIS_LETTER_Z_LARGE_PIC_START, FLASH_LAYER1, LAYER_AXIS,0,0);  
  LTDC_drawOptionsRectBot(596,37,TTZ_COL_ORAN,TTZ_COL_BAGR,LAYER_AXIS);
  LTDC_IconFromLayer(615, 41, 51, 31, TTZ_COL_ORAN, AXIS_ARROW_LARGE_TOP_PIC_START, FLASH_LAYER1, LAYER_AXIS,0,0);
  LTDC_IconFromLayer(615, 72, 51, 31, TTZ_COL_ORAN, AXIS_ARROW_LARGE_TOP_PIC_START, FLASH_LAYER1, LAYER_AXIS,0,0);
  TFT_DrawText(TFT_FONT16,483, 113, PSTR("10,0 мм"),0,LAYER_AXIS, LAYER_AXIS,0x0);
  TFT_DrawText(TFT_FONT16,605, 113, PSTR("10,0 мм"),0,LAYER_AXIS, LAYER_AXIS,0x0);
  /*top2 line*/
  LTDC_drawOptionsRectBot(474,134,TTZ_COL_ORAN,TTZ_COL_BAGR,LAYER_AXIS);
  LTDC_IconFromLayer(480, 141, 33, 33, TTZ_COL_ORAN, AXIS_ARROW_SMALL_TOP_PIC_START, FLASH_LAYER1, LAYER_AXIS,0,0);
  LTDC_IconFromLayer(523, 162, 29, 35, TTZ_COL_ORAN, AXIS_LETTER_Z_LARGE_PIC_START, FLASH_LAYER1, LAYER_AXIS,0,0);  
  LTDC_drawOptionsRectBot(596,134,TTZ_COL_ORAN,TTZ_COL_BAGR,LAYER_AXIS);
  LTDC_IconFromLayer(615, 142, 51, 61, TTZ_COL_ORAN, AXIS_ARROW_EXLARGE_TOP_PIC_START, FLASH_LAYER1, LAYER_AXIS,0,0);
  TFT_DrawText(TFT_FONT16,493, 210, PSTR("1,0 мм"),0,LAYER_AXIS, LAYER_AXIS,0x0);
  TFT_DrawText(TFT_FONT16,615, 210, PSTR("1,0 мм"),0,LAYER_AXIS, LAYER_AXIS,0x0);
  /*z-home*/
  TFT_DrawRectangle(/*x,y*/474,232, /*width, height*/90,90, /*round corner*/8,/*color*/TTZ_COL_ORAN,/*back_color*/TTZ_COL_BAGR,LAYER_AXIS,/*thikness in px*/46,/*fill?*/1,/*punktir?*/0);
  LTDC_IconFromLayer(495, 238, 48, 52, 0xFFFFFFFF, RIGHT_PANEL_HOME_PIC_START, FLASH_LAYER1, LAYER_AXIS,0,0);    
  TFT_DrawRectangle(/*x,y*/510,294, /*width, height*/19,19, /*round corner*/9,/*color*/0xFFFFFFFF,/*back_color*/TTZ_COL_ORAN,LAYER_AXIS,/*thikness in px*/10,/*fill?*/1,/*punktir?*/0);
  TFT_SetTextColor(TTZ_COL_ORAN);
  TFT_DrawChar(TFT_FONT16,514, 297, 'Z',0,LAYER_AXIS, LAYER_AXIS,0x0);
  TFT_SetTextColor(LCD_COLOR_WHITE);

  
  // LTDC_IconFromLayer(510, 294, 21, 20, AXIS_LETTER_Z_SMALL_PIC_START, FLASH_LAYER1, LAYER_AXIS,0,0);   
  /*middle extruders*/
  // TFT_DrawRectangle(/*x,y*/596,232, /*width, height*/90,43, /*round corner*/8,/*color*/TTZ_COL_ORAN,/*back_color*/TTZ_COL_BAGR,LAYER_AXIS,/*thikness in px*/(hotendActive0 == 1) ? 22 : 2,/*fill?*/(hotendActive0 == 1) ? 1 : 0,/*punktir?*/0);
  // TFT_DrawRectangle(/*x,y*/596,277, /*width, height*/90,43, /*round corner*/8,/*color*/TTZ_COL_ORAN,/*back_color*/TTZ_COL_BAGR,LAYER_AXIS,/*thikness in px*/(hotendActive1 == 1) ? 22 : 2,/*fill?*/(hotendActive1 == 1) ? 1 : 0,/*punktir?*/0);
  /*bottom1 line*/
  LTDC_drawOptionsRectTop(474,330,TTZ_COL_ORAN,TTZ_COL_BAGR,LAYER_AXIS);
  LTDC_IconFromLayer(480, 379, 33, 33, TTZ_COL_ORAN, AXIS_ARROW_SMALL_TOP_PIC_START, FLASH_LAYER1, LAYER_AXIS,2,0);
  LTDC_IconFromLayer(523, 356, 29, 35, TTZ_COL_ORAN, AXIS_LETTER_Z_LARGE_PIC_START, FLASH_LAYER1, LAYER_AXIS,0,0);  
  LTDC_drawOptionsRectTop(596,330,TTZ_COL_ORAN,TTZ_COL_BAGR,LAYER_AXIS);
  LTDC_IconFromLayer(615, 353, 51, 61, TTZ_COL_ORAN, AXIS_ARROW_EXLARGE_TOP_PIC_START, FLASH_LAYER1, LAYER_AXIS,2,0);
  TFT_DrawText(TFT_FONT16,493, 332, PSTR("1,0 мм"),0,LAYER_AXIS, LAYER_AXIS,0x0);
  TFT_DrawText(TFT_FONT16,615, 332, PSTR("1,0 мм"),0,LAYER_AXIS, LAYER_AXIS,0x0);
  /*bottom2 line*/
  LTDC_drawOptionsRectTop(474,425,TTZ_COL_ORAN,TTZ_COL_BAGR,LAYER_AXIS);
  LTDC_IconFromLayer(483, 474, 28, 17, TTZ_COL_ORAN, AXIS_ARROW_EXSMALL_TOP_PIC_START, FLASH_LAYER1, LAYER_AXIS,2,0);
  LTDC_IconFromLayer(483, 491, 28, 17, TTZ_COL_ORAN, AXIS_ARROW_EXSMALL_TOP_PIC_START, FLASH_LAYER1, LAYER_AXIS,2,0);
  LTDC_IconFromLayer(523, 451, 29, 35, TTZ_COL_ORAN, AXIS_LETTER_Z_LARGE_PIC_START, FLASH_LAYER1, LAYER_AXIS,0,0);  
  LTDC_drawOptionsRectTop(596,425,TTZ_COL_ORAN,TTZ_COL_BAGR,LAYER_AXIS);
  LTDC_IconFromLayer(615, 448, 51, 31, TTZ_COL_ORAN, AXIS_ARROW_LARGE_TOP_PIC_START, FLASH_LAYER1, LAYER_AXIS,2,0);
  LTDC_IconFromLayer(615, 479, 51, 31, TTZ_COL_ORAN, AXIS_ARROW_LARGE_TOP_PIC_START, FLASH_LAYER1, LAYER_AXIS,2,0);
  TFT_DrawText(TFT_FONT16,483, 427, PSTR("10,0 мм"),0,LAYER_AXIS, LAYER_AXIS,0x0);
  TFT_DrawText(TFT_FONT16,605, 427, PSTR("10,0 мм"),0,LAYER_AXIS, LAYER_AXIS,0x0);
  /*bottom screen*/
  TFT_DrawRectangle(/*x,y*/35,527, /*width, height*/466,41, /*round corner*/8,/*color*/TTZ_COL_ORAN,/*back_color*/TTZ_COL_BAGR,LAYER_AXIS,/*thikness in px*/21,/*fill?*/1,/*punktir?*/0);
  TFT_DrawFilledRect(72, 535, 173, 559, 0xFFFFFFFF, LAYER_AXIS);
  LTDC_IconFromLayer(47, 540, 20, 17, 0xFFFFFFFF, AXIS_LETTER_X_MEDIUM_PIC_START, FLASH_LAYER1, LAYER_AXIS,0,0);
  TFT_DrawFilledRect(228, 535, 330, 559, 0xFFFFFFFF, LAYER_AXIS);
  LTDC_IconFromLayer(203, 540, 21, 17, 0xFFFFFFFF, AXIS_LETTER_Y_MEDIUM_PIC_START, FLASH_LAYER1, LAYER_AXIS,0,0);
  TFT_DrawFilledRect(385, 535, 486, 559, 0xFFFFFFFF, LAYER_AXIS);    
  LTDC_IconFromLayer(360, 540, 20, 17, 0xFFFFFFFF, AXIS_LETTER_Z_MEDIUM_PIC_START, FLASH_LAYER1, LAYER_AXIS,0,0);
  
  TFT_DrawRectangle(/*x,y*/512,527, /*width, height*/173,41, /*round corner*/8,/*color*/TTZ_COL_ORAN,/*back_color*/TTZ_COL_BAGR,LAYER_AXIS,/*thikness in px*/21,/*fill?*/1,/*punktir?*/0);
  TFT_DrawFilledRect(568, 535, 669, 559, 0xFFFFFFFF, LAYER_AXIS);
  LTDC_IconFromLayer(526, 540, 35, 17, 0xFFFFFFFF, AXIS_LETTER_DZ_MEDIUM_PIC_START, FLASH_LAYER1, LAYER_AXIS,0,0);

  draw_right_panel(menu_options);

  RewriteMemory(&hsdram1,LCD_FRAME_BUFFER(LAYER_AXIS),LCD_FRAME_BUFFER(LAYER_AXIS_2));

}

