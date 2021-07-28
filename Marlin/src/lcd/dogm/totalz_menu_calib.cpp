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
#include "../../module/planner.h"
#include "../../core/w25qxx.h"



#define     FAN_INT(N)              (Temperature::fan_speed[N])
#define     FAN_PERC(N)             (Temperature::fanPercent(FAN_INT(N)))
#define     FAN_TO_INT(X)           (Temperature::fanUnits(X))
#define     FAN_TO_PERC(X)          (Temperature::fanPercent(X))


extern LCD_DrawPropTypeDef lcdprop;
// extern Planner planner;

void MarlinUI::edic_calib_cancel(){
  uint32_t now = millis();
  if(!ELAPSED(now,edited_timer_ms)){
    edited_timer_ms = now;
  }else{
    edited_timer_ms = now+10000;
    edited_settings = 0;
  }
}
void MarlinUI::edic_calib_back(){
  uint32_t now = millis();
  if(ELAPSED(now,edited_timer_ms)){
    switch(calibstate){
      case EXTR1: edited_settings = Temperature::temp_hotend[0].target;break;
      case EXTR2: edited_settings = Temperature::temp_hotend[1].target;break;
      case EXTR3: edited_settings = Temperature::temp_hotend[2].target;break;
      case BED:   edited_settings = Temperature::temp_bed.target;break;
      case CHAMBER: 
      #if HAS_TEMP_CHAMBER
        edited_settings = Temperature::temp_chamber.target;break;
      #endif
      break;
      case FEEDRATE: edited_settings = feedrate_percentage;break;
      case FILAMENT_FLOW1: edited_settings = Planner::flow_percentage[0];break;
      case FILAMENT_FLOW2: edited_settings = Planner::flow_percentage[1];break;
      case COOLER: edited_settings = thermalManager.fanPercent(thermalManager.fan_speed[0]);break;
      default: break;
    }
  }
  edited_settings = (uint16_t)((edited_settings/10));
  
  edited_timer_ms = now+10000;
}
void MarlinUI::edic_calib_ok(){
  uint32_t now = millis();
  if(!ELAPSED(now,edited_timer_ms)){
    switch(calibstate){
      case EXTR1: Temperature::temp_hotend[0].target = edited_settings;break;
      case EXTR2: Temperature::temp_hotend[1].target = edited_settings;break;
      case BED:   Temperature::temp_bed.target = edited_settings;break;
      case CHAMBER: 
      #if HAS_TEMP_CHAMBER
        Temperature::temp_chamber.target = edited_settings;break;
      #endif
      break;
      case FEEDRATE: feedrate_percentage = edited_settings;break;
      case FILAMENT_FLOW1: planner.flow_percentage[0] = edited_settings;planner.refresh_e_factor(0);break;
      case FILAMENT_FLOW2: planner.flow_percentage[1] = edited_settings;planner.refresh_e_factor(1);break;
      case COOLER:  Temperature::set_fan_speed(0, Temperature::fanUnits(edited_settings>100?100:edited_settings));
      default: break;
    }
    edited_timer_ms = now;
  }
}
void MarlinUI::pm_calib_val(int i) {
  if(!ELAPSED(millis(),edited_timer_ms)){
    edited_settings = edited_settings+i;
    if(edited_settings<0)edited_settings=0;
    if(edited_settings>999)edited_settings=999;
    edited_timer_ms = millis() + 10000;  
  }else{
    switch(calibstate){
      case EXTR1: Temperature::temp_hotend[0].target = Temperature::temp_hotend[0].target+i;if(Temperature::temp_hotend[0].target<0)Temperature::temp_hotend[0].target=0;break;
      case EXTR2: Temperature::temp_hotend[1].target = Temperature::temp_hotend[1].target+i;if(Temperature::temp_hotend[1].target<0)Temperature::temp_hotend[1].target=0;break;
      case BED:   Temperature::temp_bed.target = Temperature::temp_bed.target+i;if(Temperature::temp_bed.target<0)Temperature::temp_bed.target=0;break;
      case CHAMBER: 
      #if HAS_TEMP_CHAMBER
        Temperature::temp_chamber.target = Temperature::temp_chamber.target+i;if(Temperature::temp_chamber.target<0)Temperature::temp_chamber.target=0;break;
      #endif
      break;
      case FEEDRATE: feedrate_percentage = feedrate_percentage+i;if(feedrate_percentage<0)feedrate_percentage=0;break;
      case FILAMENT_FLOW1: Planner::flow_percentage[0] = Planner::flow_percentage[0]+i;if(Planner::flow_percentage[0]<0)Planner::flow_percentage[0]=0;break;
      case FILAMENT_FLOW2: Planner::flow_percentage[1] = Planner::flow_percentage[1]+i;if(Planner::flow_percentage[1]<0)Planner::flow_percentage[1]=0;break;
      case COOLER: Temperature::fan_speed[0] = FAN_TO_INT(FAN_PERC(0)+i>100?100:(FAN_PERC(0)+i<0?0:FAN_PERC(0)+i));break;
      default: break;
    }//fanUnits
  }
}
void MarlinUI::edit_calib_val(int i) {
  uint16_t current_val;
  char str[5];
  /**
   * Если время таймера 10с не истекло
   * то берем текущее значение с экрана - current_val
   * В противном случе
   * берем глобал из памяти - edited_settings
  */
  if(ELAPSED(millis(),edited_timer_ms)){
    current_val=0; 
    switch(calibstate){
      case EXTR1: current_val=Temperature::temp_hotend[0].target;break;
      case EXTR2: current_val=Temperature::temp_hotend[1].target;break;
      case BED:   current_val=Temperature::temp_bed.target;break;
      case CHAMBER: 
      #if HAS_TEMP_CHAMBER
        current_val=Temperature::temp_chamber.target;break;
      #endif
      break;
      case FEEDRATE: current_val=feedrate_percentage;break;
      case FILAMENT_FLOW1: current_val=Planner::flow_percentage[0];break;
      case FILAMENT_FLOW2: current_val=Planner::flow_percentage[1];break;
      case COOLER: current_val=Temperature::fanPercent(Temperature::fan_speed[0]);break;
      default: current_val=0; break;
    }
    sprintf(str,"%d%d",(int)current_val,i);
  }else{
    sprintf(str,"%d%d",(int)edited_settings,i);
  }
  char temp[5];
  uint8_t a = 0;
  if(int(str[3])>=48 && int(str[3])<=57)a = 1;
  temp[0] = str[0+a];
  temp[1] = str[1+a];
  temp[2] = str[2+a];
  temp[3] = str[3+a];
  edited_settings = (uint16_t)(atoi(temp));

  // switch(calibstate){
  //   case EXTR1: Temperature::temp_hotend[0].target = (int16_t)edited_settings;break;
  //   case EXTR2: Temperature::temp_hotend[1].target = (int16_t)edited_settings;break;
  //   case BED:   Temperature::temp_bed.target = (int16_t)edited_settings;break;
  //   case CHAMBER: 
  //   #if HAS_TEMP_CHAMBER
  //     Temperature::temp_chamber.target = (int16_t)edited_settings;break;
  //   #endif
  //   break;
  //   case FEEDRATE: feedrate_percentage = (int16_t)edited_settings;break;
  //   case FILAMENT_FLOW: current_val=0;break;
  //   default: current_val=0; break;
  // }
  edited_timer_ms = millis() + 10000;  
}
void MarlinUI::menu_calib() {
  prevScreen = currentScreen;
  menu_calib_refresh();
  HAL_LTDC_SetAddress(&hltdc,LCD_FRAME_BUFFER(LAYER_CAL), 0);
}

void MarlinUI::menu_calib_set_active(int item){
  /*extruder 1 and filament flow 1*/
  TFT_DrawRectangle(/*x,y*/37,28, /*width, height*/211,95, /*round corner*/0,/*color*/TTZ_COL_ORAN,/*back_color*/TTZ_COL_BAGR,LAYER_CAL,/*thikness in px*/3,/*fill?*/0,/*punktir?*/0);
  TFT_DrawRectangle(/*x,y*/37,130, /*width, height*/211,95, /*round corner*/0,/*color*/TTZ_COL_ORAN,/*back_color*/TTZ_COL_BAGR,LAYER_CAL,/*thikness in px*/3,/*fill?*/0,/*punktir?*/0);
  /*extruder 2 and filament flow 2*/
  TFT_DrawRectangle(/*x,y*/255,28, /*width, height*/211,95, /*round corner*/0,/*color*/TTZ_COL_ORAN,/*back_color*/TTZ_COL_BAGR,LAYER_CAL,/*thikness in px*/3,/*fill?*/0,/*punktir?*/0);
  TFT_DrawRectangle(/*x,y*/255,130, /*width, height*/211,95, /*round corner*/0,/*color*/TTZ_COL_ORAN,/*back_color*/TTZ_COL_BAGR,LAYER_CAL,/*thikness in px*/3,/*fill?*/0,/*punktir?*/0);
  /*bed*/
  TFT_DrawRectangle(/*x,y*/474,28, /*width, height*/211,95, /*round corner*/0,/*color*/TTZ_COL_ORAN,/*back_color*/TTZ_COL_BAGR,LAYER_CAL,/*thikness in px*/3,/*fill?*/0,/*punktir?*/0);
  /*chamber*/
  TFT_DrawRectangle(/*x,y*/474,130, /*width, height*/211,95, /*round corner*/0,/*color*/TTZ_COL_ORAN,/*back_color*/TTZ_COL_BAGR,LAYER_CAL,/*thikness in px*/3,/*fill?*/0,/*punktir?*/0);
  /*feedrate*/
  TFT_DrawRectangle(/*x,y*/474,232, /*width, height*/211,95, /*round corner*/0,/*color*/TTZ_COL_ORAN,/*back_color*/TTZ_COL_BAGR,LAYER_CAL,/*thikness in px*/3,/*fill?*/0,/*punktir?*/0);
  /*cooler*/
  TFT_DrawRectangle(/*x,y*/474,334, /*width, height*/211,95, /*round corner*/0,/*color*/TTZ_COL_ORAN,/*back_color*/TTZ_COL_BAGR,LAYER_CAL,/*thikness in px*/3,/*fill?*/0,/*punktir?*/0);
  
  switch(item){
    case CAL_EXTR1: TFT_DrawRectangle(/*x,y*/37,28, /*width, height*/211,95, /*round corner*/0,/*color*/0xFFFFFFFF,/*back_color*/TTZ_COL_BAGR,LAYER_CAL,/*thikness in px*/3,/*fill?*/0,/*punktir?*/0);
    break;
    case CAL_EXTR2:TFT_DrawRectangle(/*x,y*/255,28, /*width, height*/211,95, /*round corner*/0,/*color*/0xFFFFFFFF,/*back_color*/TTZ_COL_BAGR,LAYER_CAL,/*thikness in px*/3,/*fill?*/0,/*punktir?*/0);
    break;
    case CAL_BED: TFT_DrawRectangle(/*x,y*/474,28, /*width, height*/211,95, /*round corner*/0,/*color*/0xFFFFFFFF,/*back_color*/TTZ_COL_BAGR,LAYER_CAL,/*thikness in px*/3,/*fill?*/0,/*punktir?*/0);
    break;
    case CAL_CHAMBER: TFT_DrawRectangle(/*x,y*/474,130, /*width, height*/211,95, /*round corner*/0,/*color*/0xFFFFFFFF,/*back_color*/TTZ_COL_BAGR,LAYER_CAL,/*thikness in px*/3,/*fill?*/0,/*punktir?*/0);
    break;
    case CAL_FEEDRATE:TFT_DrawRectangle(/*x,y*/474,232, /*width, height*/211,95, /*round corner*/0,/*color*/0xFFFFFFFF,/*back_color*/TTZ_COL_BAGR,LAYER_CAL,/*thikness in px*/3,/*fill?*/0,/*punktir?*/0);
    break;
    case CAL_FILAMENT_FLOW1:  TFT_DrawRectangle(/*x,y*/37,130, /*width, height*/211,95, /*round corner*/0,/*color*/0xFFFFFFFF,/*back_color*/TTZ_COL_BAGR,LAYER_CAL,/*thikness in px*/3,/*fill?*/0,/*punktir?*/0);
    break;
    case CAL_FILAMENT_FLOW2:  TFT_DrawRectangle(/*x,y*/255,130, /*width, height*/211,95, /*round corner*/0,/*color*/0xFFFFFFFF,/*back_color*/TTZ_COL_BAGR,LAYER_CAL,/*thikness in px*/3,/*fill?*/0,/*punktir?*/0);
    break;
    case CAL_COOLER:  TFT_DrawRectangle(/*x,y*/474,334, /*width, height*/211,95, /*round corner*/0,/*color*/0xFFFFFFFF,/*back_color*/TTZ_COL_BAGR,LAYER_CAL,/*thikness in px*/3,/*fill?*/0,/*punktir?*/0);
    break;
  }
           
}
void MarlinUI::menu_calib_refresh() { 

  uint8_t drawLayer = LAYER_CAL;
  uint8_t backLayer = LAYER_CAL_2;  

  bool drawAct = false;
  if(active_extruder != MarlinUI::hotendActive){drawAct = true;}

  if(drawAct){
    /*EXTR 0*/
    LTDC_drawCalibDeviceRect(37,28, LAYER_CAL, active_extruder==0?1:0);
    LTDC_IconFromLayer(37+34, 28+12, HOTEND_PIC_WIDTH, HOTEND_PIC_HEIGHT, active_extruder==0?0xFFFFFFFF:TTZ_COL_ORAN,HOTEND_PIC_START, FLASH_LAYER1, LAYER_CAL,0,0);
   /*EXTR 1*/   
    #if EXTRUDERS > 1
    LTDC_drawCalibDeviceRect(255,28, LAYER_CAL, active_extruder==1?1:0);
    LTDC_IconFromLayer(255+34, 28+12, HOTEND_PIC_WIDTH, HOTEND_PIC_HEIGHT, active_extruder==1?0xFFFFFFFF:TTZ_COL_ORAN,HOTEND_PIC_START, FLASH_LAYER1, LAYER_CAL,0,0);
    #endif
    /*EXTR 0*/
    LTDC_drawCalibDeviceRect(37,28, LAYER_CAL_2, active_extruder==0?1:0);
    LTDC_IconFromLayer(37+34, 28+12, HOTEND_PIC_WIDTH, HOTEND_PIC_HEIGHT, active_extruder==0?0xFFFFFFFF:TTZ_COL_ORAN,HOTEND_PIC_START, FLASH_LAYER1, LAYER_CAL_2,0,0);
    /*EXTR 1*/
    #if EXTRUDERS > 1
    LTDC_drawCalibDeviceRect(255,28, LAYER_CAL_2, active_extruder==1?1:0);
    LTDC_IconFromLayer(255+34, 28+12, HOTEND_PIC_WIDTH, HOTEND_PIC_HEIGHT, active_extruder==1?0xFFFFFFFF:TTZ_COL_ORAN,HOTEND_PIC_START, FLASH_LAYER1, LAYER_CAL_2,0,0);
    #endif
  }
  char int16tostr[30] = "";
  switch(calibstate){
    case EXTR1: if(calibstate != calibstate_old)TFT_DrawText(TFT_FONT20,58, 252, "Температура 1 экструдера", 29, drawLayer, backLayer,0x0);sprintf(int16tostr,"%d",thermalManager.degTargetHotend(0));break;
    case EXTR2: if(calibstate != calibstate_old)TFT_DrawText(TFT_FONT20,58, 252, "Температура 2 экструдера", 29, drawLayer, backLayer,0x0);sprintf(int16tostr,"%d",thermalManager.degTargetHotend(1));break;
    case BED: if(calibstate != calibstate_old)TFT_DrawText(TFT_FONT20,58, 252, "Температура стола", 29, drawLayer, backLayer,0x0);sprintf(int16tostr,"%d",Temperature::temp_bed.target);break;
    case CHAMBER: if(calibstate != calibstate_old)TFT_DrawText(TFT_FONT20,58, 252, "Температура камеры", 29, drawLayer, backLayer,0x0);break;
    case FEEDRATE: if(calibstate != calibstate_old)TFT_DrawText(TFT_FONT20,58, 252, "Скорость печати", 29, drawLayer, backLayer,0x0);sprintf(int16tostr,"%d",feedrate_percentage);break;
    case FILAMENT_FLOW1: if(calibstate != calibstate_old)TFT_DrawText(TFT_FONT20,58, 252, "Скорость подачи 1 филамента", 29, drawLayer, backLayer,0x0);sprintf(int16tostr,"%d",Planner::flow_percentage[0]);break;
    case FILAMENT_FLOW2: if(calibstate != calibstate_old)TFT_DrawText(TFT_FONT20,58, 252, "Скорость подачи 2 филамента", 29, drawLayer, backLayer,0x0);sprintf(int16tostr,"%d",Planner::flow_percentage[1]);break;
    case COOLER: if(calibstate != calibstate_old)TFT_DrawText(TFT_FONT20,58, 252, "Скорость вентилятора %", 29, drawLayer, backLayer,0x0);sprintf(int16tostr,"%d",Temperature::fanPercent(Temperature::fan_speed[0]));break;
    default: break;
  }
  calibstate_old = calibstate;
  uint32_t col = 0xFFFFFFFF;
  if(!ELAPSED(millis(),edited_timer_ms)){
    sprintf(int16tostr,"%d",edited_settings);
    col = TTZ_COL_GREE;
  }
  TFT_DrawRectangle(/*x,y*/240,334, /*width, height*/64,24, /*round corner*/0,/*color*/TTZ_COL_ORAN,/*back_color*/TTZ_COL_ORAN,drawLayer,/*thikness in px*/13,/*fill?*/1,/*punktir?*/0);
      
  TFT_DrawNumers(300, 336, int16tostr, 4, drawLayer, backLayer,col);

  char txt[10];
  /*top-lane 3 items duals*/
  sprintf(txt,"%i",Temperature::temp_hotend[0].target);
  TFT_DrawText(TFT_FONT16,184, 53, txt, 4, drawLayer, backLayer,0x0);
  sprintf(txt,"%i",(int16_t)(Temperature::temp_hotend[0].celsius));
  TFT_DrawText(TFT_FONT16,184, 82, txt, 4, drawLayer, backLayer,0x0);

  sprintf(txt,"%d", Planner::flow_percentage[0]);
  TFT_DrawText(TFT_FONT16,184, 156, txt, 4, drawLayer, backLayer,0x0);
  
  #if EXTRUDERS > 1
  sprintf(txt,"%i",Temperature::temp_hotend[1].target);
  TFT_DrawText(TFT_FONT16,406, 53, txt, 4, drawLayer, backLayer,0x0);
  sprintf(txt,"%i",(int16_t)(Temperature::temp_hotend[1].celsius));
  TFT_DrawText(TFT_FONT16,406, 82, txt, 4, drawLayer, backLayer,0x0);

  sprintf(txt,"%d", Planner::flow_percentage[1]);
  TFT_DrawText(TFT_FONT16,406, 156, txt, 4, drawLayer, backLayer,0x0);
  #endif
  sprintf(txt,"%i",Temperature::temp_bed.target);
  TFT_DrawText(TFT_FONT16,624, 53, txt, 4, drawLayer, backLayer,0x0);
  sprintf(txt,"%i",(int16_t)(Temperature::temp_bed.celsius));
  TFT_DrawText(TFT_FONT16,624, 82, txt, 4, drawLayer, backLayer,0x0);
  
  
  #if HAS_TEMP_CHAMBER
    sprintf(txt,"%i",Temperature::temp_chamber.target);
    TFT_DrawText(TFT_FONT16,624, 156, txt, 4, drawLayer, backLayer,0x0);
    sprintf(txt,"%i",(int16_t)(Temperature::temp_chamber.celsius));
    TFT_DrawText(TFT_FONT16,624, 184, txt, 4, drawLayer, backLayer,0x0);
  #endif
  /*FEEDRATE*/
  sprintf(txt,"%d",feedrate_percentage);
  TFT_DrawText(TFT_FONT16,624, 259, txt, 4, drawLayer, backLayer,0x0);
  /*COOLER*/
  sprintf(txt,"%d",Temperature::fanPercent(Temperature::fan_speed[0]));
  TFT_DrawText(TFT_FONT16,624, 362, txt, 4, drawLayer, backLayer,0x0);


  // TFT_DrawRectangle(/*x,y*/255,28, /*width, height*/211,95, /*round corner*/0,/*color*/0xFFFFFFFF,/*back_color*/TTZ_COL_BAGR,LAYER_CAL,/*thikness in px*/3,/*fill?*/0,/*punktir?*/0);
  // TFT_DrawRectangle(/*x,y*/255,28, /*width, height*/211,95, /*round corner*/0,/*color*/0xFFFFFFFF,/*back_color*/TTZ_COL_BAGR,LAYER_CAL_2,/*thikness in px*/3,/*fill?*/0,/*punktir?*/0);
	
//  TFT_DrawRectangle(/*x,y*/259,32, /*width, height*/202,86, /*round corner*/2,/*color*/0xFFFFFFFF,/*back_color*/0xFFFFFFFF,LAYER_CAL,/*thikness in px*/2,/*fill?*/0,/*punktir?*/1);
//  TFT_DrawRectangle(/*x,y*/259,32, /*width, height*/202,86, /*round corner*/2,/*color*/0xFFFFFFFF,/*back_color*/0xFFFFFFFF,LAYER_CAL_2,/*thikness in px*/2,/*fill?*/0,/*punktir?*/1);
	
//flow_percentage


  MarlinUI::hotendActive = active_extruder;

}
void MarlinUI::draw_menu_calib() {

  TFT_FillScreen(0xFF231F20,LAYER_CAL);

  LTDC_drawCalibDeviceRect(474,28, LAYER_CAL, 0);
  LTDC_IconFromLayer(474+17, 28+11, 68, 60, TTZ_COL_ORAN,TABLE_PIC_START, FLASH_LAYER1, LAYER_CAL,0,0);
  /*mid-lane 3 items*/

  LTDC_drawCalibDeviceRect(474,130, LAYER_CAL, 0);
  LTDC_IconFromLayer(474+18, 130+12, 72, 62, TTZ_COL_ORAN,CHAMBER_PIC_START, FLASH_LAYER1, LAYER_CAL,0,0);

  // LTDC_IconFromLayer(474+22, 130+17, 60, 46, TTZ_COL_ORAN,RIGHT_PANEL_FILAMENT_PIC_START, FLASH_LAYER1, LAYER_CAL,0,0);

  /*bot-lane 1 item*/
  LTDC_drawCalibDeviceRect(474,232, LAYER_CAL, 0);
  LTDC_IconFromLayer(474+14, 232+21, 79, 42, TTZ_COL_ORAN,CALIB_SPEED_PIC_START, FLASH_LAYER1, LAYER_CAL,0,0);

  LTDC_drawCalibDeviceRect(474,334, LAYER_CAL, 0);
  LTDC_IconFromLayer(499, 345, 57, 58, TTZ_COL_ORAN,CALIB_COOLER_PIC_START, FLASH_LAYER1, LAYER_CAL,0,0);

      
  TFT_DrawRectangle(/*x,y*/38,240, /*width, height*/428,51, /*round corner*/7,/*color*/TTZ_COL_ORAN,/*back_color*/TTZ_COL_BAGR,LAYER_CAL,/*thikness in px*/26,/*fill?*/1,/*punktir?*/0);
  TFT_DrawRectangle(/*x,y*/37,306, /*width, height*/102,102, /*round corner*/7,/*color*/TTZ_COL_ORAN,/*back_color*/TTZ_COL_BAGR,LAYER_CAL,/*thikness in px*/4,/*fill?*/0,/*punktir?*/0);
  TFT_DrawRectangle(/*x,y*/365,306, /*width, height*/102,102, /*round corner*/7,/*color*/TTZ_COL_ORAN,/*back_color*/TTZ_COL_BAGR,LAYER_CAL,/*thikness in px*/4,/*fill?*/0,/*punktir?*/0);
  /* minus & plus*/
  TFT_DrawRectangle(/*x,y*/55,356, /*width, height*/64,5, /*round corner*/3,/*color*/TTZ_COL_ORAN,/*back_color*/TTZ_COL_BAGR,LAYER_CAL,/*thikness in px*/3,/*fill?*/1,/*punktir?*/0);
  TFT_DrawRectangle(/*x,y*/384,356, /*width, height*/64,5, /*round corner*/3,/*color*/TTZ_COL_ORAN,/*back_color*/TTZ_COL_BAGR,LAYER_CAL,/*thikness in px*/3,/*fill?*/1,/*punktir?*/0);
  TFT_DrawRectangle(/*x,y*/414,326, /*width, height*/5,64, /*round corner*/3,/*color*/TTZ_COL_ORAN,/*back_color*/TTZ_COL_BAGR,LAYER_CAL,/*thikness in px*/3,/*fill?*/1,/*punktir?*/0);
  
  TFT_DrawRectangle(/*x,y*/146,306, /*width, height*/358-146,408-306, /*round corner*/11,/*color*/TTZ_COL_ORAN,/*back_color*/TTZ_COL_BAGR,LAYER_CAL,/*thikness in px*/52,/*fill?*/1,/*punktir?*/0);
  TFT_DrawRectangle(/*x,y*/146,306, /*width, height*/358-146,408-306, /*round corner*/11,/*color*/0xFFFFFFFF,/*back_color*/TTZ_COL_ORAN,LAYER_CAL,/*thikness in px*/4,/*fill?*/0,/*punktir?*/0);

  TFT_DrawRectangle(/*x,y*/37,422, /*width, height*/467-37,573-422, /*round corner*/0,/*color*/0xFFFFFFFF,/*back_color*/TTZ_COL_BAGR,LAYER_CAL,/*thikness in px*/76,/*fill?*/1,/*punktir?*/0);
  TFT_DrawRectangle(/*x,y*/37,422, /*width, height*/467-37,573-422, /*round corner*/0,/*color*/TTZ_COL_ORAN,/*back_color*/TTZ_COL_BAGR,LAYER_CAL,/*thikness in px*/3,/*fill?*/0,/*punktir?*/0);
  
  TFT_DrawRectangle(/*x,y*/ 44,429, /*width, height*/65,43, /*round corner*/3,/*color*/TTZ_COL_ORAN,/*back_color*/0xFFFFFFFF,LAYER_CAL,/*thikness in px*/22,/*fill?*/1,/*punktir?*/0);
  TFT_DrawRectangle(/*x,y*/114,429, /*width, height*/65,43, /*round corner*/3,/*color*/TTZ_COL_ORAN,/*back_color*/0xFFFFFFFF,LAYER_CAL,/*thikness in px*/22,/*fill?*/1,/*punktir?*/0);
  TFT_DrawRectangle(/*x,y*/184,429, /*width, height*/65,43, /*round corner*/3,/*color*/TTZ_COL_ORAN,/*back_color*/0xFFFFFFFF,LAYER_CAL,/*thikness in px*/22,/*fill?*/1,/*punktir?*/0);
  TFT_DrawRectangle(/*x,y*/254,429, /*width, height*/65,43, /*round corner*/3,/*color*/TTZ_COL_ORAN,/*back_color*/0xFFFFFFFF,LAYER_CAL,/*thikness in px*/22,/*fill?*/1,/*punktir?*/0);
  TFT_DrawRectangle(/*x,y*/324,429, /*width, height*/65,43, /*round corner*/3,/*color*/TTZ_COL_ORAN,/*back_color*/0xFFFFFFFF,LAYER_CAL,/*thikness in px*/22,/*fill?*/1,/*punktir?*/0);
  TFT_DrawRectangle(/*x,y*/394,429, /*width, height*/67,138, /*round corner*/3,/*color*/TTZ_COL_ORAN,/*back_color*/0xFFFFFFFF,LAYER_CAL,/*thikness in px*/34,/*fill?*/1,/*punktir?*/0);
      
  TFT_DrawRectangle(/*x,y*/ 44,476, /*width, height*/65,43, /*round corner*/3,/*color*/TTZ_COL_ORAN,/*back_color*/0xFFFFFFFF,LAYER_CAL,/*thikness in px*/22,/*fill?*/1,/*punktir?*/0);
  TFT_DrawRectangle(/*x,y*/114,476, /*width, height*/65,43, /*round corner*/3,/*color*/TTZ_COL_ORAN,/*back_color*/0xFFFFFFFF,LAYER_CAL,/*thikness in px*/22,/*fill?*/1,/*punktir?*/0);
  TFT_DrawRectangle(/*x,y*/184,476, /*width, height*/65,43, /*round corner*/3,/*color*/TTZ_COL_ORAN,/*back_color*/0xFFFFFFFF,LAYER_CAL,/*thikness in px*/22,/*fill?*/1,/*punktir?*/0);
  TFT_DrawRectangle(/*x,y*/254,476, /*width, height*/65,43, /*round corner*/3,/*color*/TTZ_COL_ORAN,/*back_color*/0xFFFFFFFF,LAYER_CAL,/*thikness in px*/22,/*fill?*/1,/*punktir?*/0);
  TFT_DrawRectangle(/*x,y*/324,476, /*width, height*/65,43, /*round corner*/3,/*color*/TTZ_COL_ORAN,/*back_color*/0xFFFFFFFF,LAYER_CAL,/*thikness in px*/22,/*fill?*/1,/*punktir?*/0);
  
  TFT_DrawRectangle(/*x,y*/ 44,524, /*width, height*/136,43, /*round corner*/3,/*color*/TTZ_COL_ORAN,/*back_color*/0xFFFFFFFF,LAYER_CAL,/*thikness in px*/22,/*fill?*/1,/*punktir?*/0);
  TFT_DrawRectangle(/*x,y*/184,524, /*width, height*/136,43, /*round corner*/3,/*color*/TTZ_COL_ORAN,/*back_color*/0xFFFFFFFF,LAYER_CAL,/*thikness in px*/22,/*fill?*/1,/*punktir?*/0);
  TFT_DrawRectangle(/*x,y*/324,524, /*width, height*/137,43, /*round corner*/3,/*color*/TTZ_COL_ORAN,/*back_color*/0xFFFFFFFF,LAYER_CAL,/*thikness in px*/22,/*fill?*/1,/*punktir?*/0);
  TFT_DrawFLatLine(394,524, 460, 524, TTZ_COL_ORAN, LAYER_CAL);
  TFT_DrawPixel(460,525,TTZ_COL_ORAN,LAYER_CAL);

  LTDC_IconFromLayer( 71, 442, 13, 20, 0xFFFFFFFF,NUM_PIC_ONE_START, FLASH_LAYER1, LAYER_CAL,0,0);
  LTDC_IconFromLayer(139, 442, 16, 20, 0xFFFFFFFF,NUM_PIC_TWO_START, FLASH_LAYER1, LAYER_CAL,0,0);
  LTDC_IconFromLayer(208, 441, 16, 21, 0xFFFFFFFF,NUM_PIC_THREE_START, FLASH_LAYER1, LAYER_CAL,0,0);
  LTDC_IconFromLayer(279, 441, 18, 21, 0xFFFFFFFF,NUM_PIC_FOUR_START, FLASH_LAYER1, LAYER_CAL,0,0);
  LTDC_IconFromLayer(349, 442, 17, 20, 0xFFFFFFFF,NUM_PIC_FIVE_START, FLASH_LAYER1, LAYER_CAL,0,0);
  LTDC_IconFromLayer( 68, 487, 17, 21, 0xFFFFFFFF,NUM_PIC_SIX_START, FLASH_LAYER1, LAYER_CAL,0,0);
  LTDC_IconFromLayer(140, 487, 15, 20, 0xFFFFFFFF,NUM_PIC_SEVEN_START, FLASH_LAYER1, LAYER_CAL,0,0);
  LTDC_IconFromLayer(209, 487, 17, 20, 0xFFFFFFFF,NUM_PIC_EIGHT_START, FLASH_LAYER1, LAYER_CAL,0,0);
  LTDC_IconFromLayer(278, 487, 17, 20, 0xFFFFFFFF,NUM_PIC_NINE_START, FLASH_LAYER1, LAYER_CAL,0,0);
  LTDC_IconFromLayer(348, 487, 17, 20, 0xFFFFFFFF,NUM_PIC_ZERO_START, FLASH_LAYER1, LAYER_CAL,0,0);
  LTDC_IconFromLayer(408, 529, 34, 34, 0xFFFFFFFF,NUM_PIC_OK_START, FLASH_LAYER1, LAYER_CAL,0,0);
  LTDC_IconFromLayer(236, 529, 34, 34, 0xFFFFFFFF,NUM_PIC_CANCEL_START, FLASH_LAYER1, LAYER_CAL,0,0);
  LTDC_IconFromLayer( 87, 535, 51, 23, 0xFFFFFFFF,NUM_PIC_BACK_START, FLASH_LAYER1, LAYER_CAL,0,0);

  draw_right_panel(menu_calib);

  uint8_t drawLayer = LAYER_CAL_2;
  uint8_t backLayer = LAYER_CAL;  

  bool drawAct = false;
  if(active_extruder != MarlinUI::hotendActive){drawAct = true;}

  if(drawAct){
    /*EXTR 0*/
    LTDC_drawCalibDeviceRect(37,28, LAYER_CAL, active_extruder==0?1:0);
    LTDC_IconFromLayer(37+34, 28+12, HOTEND_PIC_WIDTH, HOTEND_PIC_HEIGHT, active_extruder==0?0xFFFFFFFF:TTZ_COL_ORAN,HOTEND_PIC_START, FLASH_LAYER1, LAYER_CAL,0,0);
    LTDC_drawCalibDeviceRect(37,130, LAYER_CAL, 0);
    LTDC_IconFromLayer(37+22, 130+17, 60, 46, TTZ_COL_ORAN,RIGHT_PANEL_FILAMENT_PIC_START, FLASH_LAYER1, LAYER_CAL,0,0);
    /*EXTR 1*/
    #if EXTRUDERS > 1
    LTDC_drawCalibDeviceRect(255,28, LAYER_CAL, active_extruder==1?1:0);
    LTDC_IconFromLayer(255+34, 28+12, HOTEND_PIC_WIDTH, HOTEND_PIC_HEIGHT, active_extruder==1?0xFFFFFFFF:TTZ_COL_ORAN,HOTEND_PIC_START, FLASH_LAYER1, LAYER_CAL,0,0);
    LTDC_drawCalibDeviceRect(255,130, LAYER_CAL, 0);
    LTDC_IconFromLayer(255+22, 130+17, 60, 46, TTZ_COL_ORAN,RIGHT_PANEL_FILAMENT_PIC_START, FLASH_LAYER1, LAYER_CAL,0,0);
    #endif
  }else{
    /*EXTR 0*/
    LTDC_drawCalibDeviceRect(37,28, LAYER_CAL, active_extruder==0?1:0);
    LTDC_IconFromLayer(37+34, 28+12, HOTEND_PIC_WIDTH, HOTEND_PIC_HEIGHT, active_extruder==0?0xFFFFFFFF:TTZ_COL_ORAN,HOTEND_PIC_START, FLASH_LAYER1, LAYER_CAL,0,0);
    LTDC_drawCalibDeviceRect(37,130, LAYER_CAL, 0);
    LTDC_IconFromLayer(37+22, 130+17, 60, 46, TTZ_COL_ORAN,RIGHT_PANEL_FILAMENT_PIC_START, FLASH_LAYER1, LAYER_CAL,0,0);
    /*EXTR 1*/
    #if EXTRUDERS > 1
    LTDC_drawCalibDeviceRect(255,28, LAYER_CAL, active_extruder==1?1:0);
    LTDC_IconFromLayer(255+34, 28+12, HOTEND_PIC_WIDTH, HOTEND_PIC_HEIGHT, active_extruder==1?0xFFFFFFFF:TTZ_COL_ORAN,HOTEND_PIC_START, FLASH_LAYER1, LAYER_CAL,0,0);
    LTDC_drawCalibDeviceRect(255,130, LAYER_CAL, 0);
    LTDC_IconFromLayer(255+22, 130+17, 60, 46, TTZ_COL_ORAN,RIGHT_PANEL_FILAMENT_PIC_START, FLASH_LAYER1, LAYER_CAL,0,0);
    #endif
  }


  RewriteMemory(&hsdram1,LCD_FRAME_BUFFER(LAYER_CAL),LCD_FRAME_BUFFER(LAYER_CAL_2));
}