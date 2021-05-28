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

#include "../inc/MarlinConfig.h"

#if ENABLED(BABYSTEPPING)

#include "babystep.h"
#include "../MarlinCore.h"
#include "../module/planner.h"
#include "../module/stepper.h"
#include "../core/w25qxx.h"
#include "../../libs/numtostr.h"
#include "../LTDC/ltdc.h"

#if ENABLED(BABYSTEP_ALWAYS_AVAILABLE)
  #include "../gcode/gcode.h"
#endif

Babystep babystep;

extern    int                               FL_sel;

volatile int16_t Babystep::steps[BS_AXIS_IND(Z_AXIS) + 1];
#if ENABLED(BABYSTEP_DISPLAY_TOTAL)
  int16_t Babystep::axis_total[BS_TOTAL_IND(Z_AXIS) + 1];
#endif
int16_t Babystep::accum;
// bool Babystep::bbst_homed = false;

// void Babystep::bbstp_reset() {bbst_homed = false;}
void Babystep::step_axis(const AxisEnum axis) {
  const int16_t curTodo = steps[BS_AXIS_IND(axis)]; // get rid of volatile for performance
  if (curTodo) {
    stepper.do_babystep((AxisEnum)axis, curTodo > 0);
    if (curTodo > 0) steps[BS_AXIS_IND(axis)]--; else steps[BS_AXIS_IND(axis)]++;
  }
}

void Babystep::add_mm(const AxisEnum axis, const float &mm) {
  current_position.pos[axis] -= mm;
  add_steps(axis, mm * planner.settings.axis_steps_per_mm[axis]);
}

void Babystep::bbstp_home(const AxisEnum axis) {
  FL_sel = 1;
  int bbstp;
  uint8_t bytes[4];
  W25qxx_ReadBytes(bytes,W25ADDR_BABYSTEPS,4);
  bbstp = bytes[1]<<16 | bytes[2]<<8 | bytes[3];//current_position.pos.z
  if(bytes[0] == 0){
    
  }else if(bytes[0] == 1){
    bbstp = -bbstp;
  }else{
    bbstp = 0;
  }
  // const xyz_pos_t lpos = current_position.asLogical();//planner.position
  
  float saved_steps_mm    = (int)bbstp*0.01;//from current to d-home mm
  float to_move           = current_position.pos[axis]+saved_steps_mm;
  if(saved_steps_mm >  0.5)saved_steps_mm =  0.5;
  if(saved_steps_mm < -5.0)saved_steps_mm = -5.0;
  current_position.pos[axis] = -saved_steps_mm;
  set_home_offset(axis,saved_steps_mm);
  babystep.bbst_homed = false;
  if (DISABLED(BABYSTEP_WITHOUT_HOMING) && !TEST(axis_known_position, axis))return;
  steps[axis] += to_move*planner.settings.axis_steps_per_mm[axis];
  bbstp_save((int)(0));
  babystep.bbst_homed = true;
  LTDC_drawOptionsRectBot(35,37,TTZ_COL_ORAN,TTZ_COL_BAGR,LAYER_AXIS);
  LTDC_IconFromLayer(54, 41, 51, 31, TTZ_COL_ORAN, AXIS_ARROW_LARGE_TOP_PIC_START, FLASH_LAYER1, LAYER_AXIS,0,0);
  LTDC_IconFromLayer(54, 72, 51, 31, TTZ_COL_ORAN, AXIS_ARROW_LARGE_TOP_PIC_START, FLASH_LAYER1, LAYER_AXIS,0,0);
  TFT_DrawText(TFT_FONT16,54, 113, PSTR("0,1 мм"),0,LAYER_AXIS, LAYER_AXIS,0x0);
  LTDC_drawOptionsRectBot(35,134,TTZ_COL_ORAN,TTZ_COL_BAGR,LAYER_AXIS);  
  LTDC_IconFromLayer(54, 142, 51, 61, TTZ_COL_ORAN, AXIS_ARROW_EXLARGE_TOP_PIC_START, FLASH_LAYER1, LAYER_AXIS,0,0);  
  TFT_DrawText(TFT_FONT16,44, 210, PSTR("0,01 мм"),0,LAYER_AXIS, LAYER_AXIS,0x0);
  LTDC_drawOptionsRectTop(35,330,TTZ_COL_ORAN,TTZ_COL_BAGR,LAYER_AXIS);
  LTDC_IconFromLayer(54, 353, 51, 61, TTZ_COL_ORAN, AXIS_ARROW_EXLARGE_TOP_PIC_START, FLASH_LAYER1, LAYER_AXIS,2,0);
  TFT_DrawText(TFT_FONT16,44, 332, PSTR("0,01 мм"),0,LAYER_AXIS, LAYER_AXIS,0x0);
  LTDC_drawOptionsRectTop(35,425,TTZ_COL_ORAN,TTZ_COL_BAGR,LAYER_AXIS);
  LTDC_IconFromLayer(52, 448, 51, 31, TTZ_COL_ORAN, AXIS_ARROW_LARGE_TOP_PIC_START, FLASH_LAYER1, LAYER_AXIS,2,0);
  LTDC_IconFromLayer(52, 479, 51, 31, TTZ_COL_ORAN, AXIS_ARROW_LARGE_TOP_PIC_START, FLASH_LAYER1, LAYER_AXIS,2,0);
  TFT_DrawText(TFT_FONT16,54, 427, PSTR("0,1 мм"),0,LAYER_AXIS, LAYER_AXIS,0x0);



}
void Babystep::add_steps(const AxisEnum axis, const int16_t distance) {  

  if (DISABLED(BABYSTEP_WITHOUT_HOMING) && !TEST(axis_known_position, axis)) return;

  accum += (distance/planner.settings.axis_steps_per_mm[axis] - current_position.pos[axis])*planner.settings.axis_steps_per_mm[axis];


  // accum += distance; // Count up babysteps for the UI
  #if ENABLED(BABYSTEP_DISPLAY_TOTAL)
    axis_total[BS_TOTAL_IND(axis)] += distance;
  #endif

  #if ENABLED(BABYSTEP_ALWAYS_AVAILABLE)
    #define BSA_ENABLE(AXIS) do{ switch (AXIS) { case X_AXIS: ENABLE_AXIS_X(); break; case Y_AXIS: ENABLE_AXIS_Y(); break; case Z_AXIS: ENABLE_AXIS_Z(); break; default: break; } }while(0)
  #else
    #define BSA_ENABLE(AXIS) NOOP
  #endif

  #if IS_CORE
    #if ENABLED(BABYSTEP_XY)
      switch (axis) {
        case CORE_AXIS_1: // X on CoreXY and CoreXZ, Y on CoreYZ
          BSA_ENABLE(CORE_AXIS_1);
          BSA_ENABLE(CORE_AXIS_2);
          steps[CORE_AXIS_1] += distance * 2;
          steps[CORE_AXIS_2] += distance * 2;
          break;
        case CORE_AXIS_2: // Y on CoreXY, Z on CoreXZ and CoreYZ
          BSA_ENABLE(CORE_AXIS_1);
          BSA_ENABLE(CORE_AXIS_2);
          steps[CORE_AXIS_1] += CORESIGN(distance * 2);
          steps[CORE_AXIS_2] -= CORESIGN(distance * 2);
          break;
        case NORMAL_AXIS: // Z on CoreXY, Y on CoreXZ, X on CoreYZ
        default:
          BSA_ENABLE(NORMAL_AXIS);
          steps[NORMAL_AXIS] += distance;
          break;
      }
    #elif CORE_IS_XZ || CORE_IS_YZ
      // Only Z stepping needs to be handled here
      BSA_ENABLE(CORE_AXIS_1);
      BSA_ENABLE(CORE_AXIS_2);
      steps[CORE_AXIS_1] += CORESIGN(distance * 2);
      steps[CORE_AXIS_2] -= CORESIGN(distance * 2);
    #else
      BSA_ENABLE(Z_AXIS);
      steps[Z_AXIS] += distance;
      bbstp_save((int)(accum));
    #endif
  #else
    #if ENABLED(BABYSTEP_XY)
      BSA_ENABLE(axis);
    #else
      BSA_ENABLE(Z_AXIS);
    #endif
    steps[BS_AXIS_IND(axis)] += distance;
  #endif
  #if ENABLED(BABYSTEP_ALWAYS_AVAILABLE)
    gcode.reset_stepper_timeout();
  #endif

  #if ENABLED(INTEGRATED_BABYSTEPPING)
    if (has_steps()) stepper.initiateBabystepping();
  #endif
}
void Babystep::bbstp_save(int accum){
  // float mm = accum/planner.settings.axis_steps_per_mm[Z_AXIS];
  float mm = home_offset.pos[Z_AXIS];
  int steps = mm*100;
  uint8_t sgn = 0;
  if(steps<0){steps = -steps;sgn = 1;}
  uint8_t bbstp[4];
  bbstp[0] = ((uint8_t)(sgn)) & 0xFF;
  bbstp[1] = ((uint32_t)(steps)>>16) & 0xFF;
  bbstp[2] = ((uint32_t)(steps)>>8) & 0xFF;
  bbstp[3] = ((uint32_t)(steps)) & 0xFF;

  FL_sel = 1;
  W25qxx_EraseSector(W25ADDR_BABYSTEPS/0x1000);
  W25qxx_WriteSector(bbstp,W25ADDR_BABYSTEPS/0x1000,0,4);
}
#endif // BABYSTEPPING
