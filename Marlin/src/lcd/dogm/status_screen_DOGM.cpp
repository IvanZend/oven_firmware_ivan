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

#if HAS_GRAPHICAL_LCD && DISABLED(LIGHTWEIGHT_UI)

#include "ststusScreen_dimensions.h"
#include "dogm_Statusscreen.h"
#include "ultralcd_DOGM.h"
#include "../ultralcd.h"
#include "../LTDC/ltdc.h"
#include "../lcdprint.h"
#include "../../libs/numtostr.h"

#include "../../module/motion.h"
#include "../../module/temperature.h"
#include "../../module/LWIP/lwip.h"
#include "../../core/w25qxx.h"
#include "../../feature/babystep.h"

#if ENABLED(FILAMENT_LCD_DISPLAY)
  #include "../../feature/filwidth.h"
  #include "../../module/planner.h"
  #include "../../gcode/parser.h"
#endif

#if HAS_CUTTER
  #include "../../feature/spindle_laser.h"
#endif

#if ENABLED(SDSUPPORT)
  #include "../../sd/cardreader.h"
#endif

#if HAS_PRINT_PROGRESS
  #include "../../module/printcounter.h"
#endif

#if DUAL_MIXING_EXTRUDER
  #include "../../feature/mixing.h"
#endif

extern        LCD_DrawPropTypeDef               lcdprop;
extern        Babystep                          babystep;
extern 				ApplicationTypeDef                Appli_state;
extern        struct netif                      gnetif;
#define X_LABEL_POS      3
#define X_VALUE_POS     90
#define XYZ_SPACING     107
#define XYZ_BASELINE    396
#define EXTRAS_BASELINE (40 + INFO_FONT_ASCENT)
#define STATUS_BASELINE (LCD_PIXEL_HEIGHT - INFO_FONT_DESCENT)

#if ANIM_HBCC
  enum HeatBits : uint8_t {
    HEATBIT_HOTEND,
    HEATBIT_BED = HOTENDS,
    HEATBIT_CHAMBER,
    HEATBIT_CUTTER
  };
  IF<(HEATBIT_CUTTER > 7), uint16_t, uint8_t>::type heat_bits;
#endif

#if ANIM_HOTEND
  #define HOTEND_ALT(N) TEST(heat_bits, HEATBIT_HOTEND + N)
#else
  #define HOTEND_ALT(N) false
#endif
#if ANIM_BED
  #define BED_ALT() TEST(heat_bits, HEATBIT_BED)
#else
  #define BED_ALT() false
#endif
#if ANIM_CHAMBER
  #define CHAMBER_ALT() TEST(heat_bits, HEATBIT_CHAMBER)
#else
  #define CHAMBER_ALT() false
#endif
#if ANIM_CUTTER
  #define CUTTER_ALT(N) TEST(heat_bits, HEATBIT_CUTTER)
#else
  #define CUTTER_ALT() false
#endif

#if DO_DRAW_HOTENDS
  // #define MAX_HOTEND_DRAW _MIN(HOTENDS, ((LCD_PIXEL_WIDTH - (STATUS_LOGO_WIDTH + STATUS_FAN_WIDTH)) / (STATUS_HEATERS_XSPACE)))
  #define STATUS_HEATERS_BOT (STATUS_HEATERS_Y + STATUS_HEATERS_HEIGHT - 1)
#endif

#define PROGRESS_BAR_X 54
#define PROGRESS_BAR_WIDTH (LCD_PIXEL_WIDTH - PROGRESS_BAR_X)

// extern uint32_t image_data_bed_on;


FORCE_INLINE void _draw_centered_temp(const int16_t temp, const uint16_t tx, const uint16_t ty,uint8_t textlayer, uint8_t backLayer) {
  const char *str = i16tostr3rj(temp);
  const uint8_t len = str[0] != ' ' ? 3 : str[1] != ' ' ? 2 : 1;
  TFT_DrawText(TFT_FONT16,tx, ty, &str[3-len],3,textlayer, backLayer,0x0);
  TFT_DrawChar(TFT_FONT16,tx+FONT_W(TFT_FONT16)*strlen(&str[3-len]), ty, LCD_STR_DEGREE[0],3,textlayer, backLayer,0x0);
  // lcd_put_u8str(tx - len * (INFO_FONT_WIDTH) / 2 + 1, ty, &str[3-len]);
  // lcd_put_wchar(LCD_STR_DEGREE[0]);//tab character
}

#if DO_DRAW_HOTENDS

  // Draw hotend bitmap with current and target temperatures
  FORCE_INLINE void _draw_hotend_status(const heater_ind_t heater, const bool blink, bool drawPic, uint8_t textLayer, uint8_t backLayer) {
    #if !HEATER_IDLE_HANDLER
      UNUSED(blink);
    #endif

    // const bool isHeat = HOTEND_ALT(heater);

    const uint16_t tx = STATUS_HOTEND_TEXT_X(heater),
                   ty = STATUS_HOTEND_TEXT_Y(heater);

    const float temp = thermalManager.degHotend(heater),
              target = thermalManager.degTargetHotend(heater);

    #if DISABLED(STATUS_HOTEND_ANIM)
      #define STATIC_HOTEND true
      #define HOTEND_DOT    isHeat
    #else
      #define STATIC_HOTEND false
      #define HOTEND_DOT    false
    #endif

    #if ANIM_HOTEND && BOTH(STATUS_HOTEND_INVERTED, STATUS_HOTEND_NUMBERLESS)
      #define OFF_BMP(N) status_hotend_b_bmp
      #define ON_BMP(N)  status_hotend_a_bmp
    #elif ANIM_HOTEND && DISABLED(STATUS_HOTEND_INVERTED) && ENABLED(STATUS_HOTEND_NUMBERLESS)
      #define OFF_BMP(N) status_hotend_a_bmp
      #define ON_BMP(N)  status_hotend_b_bmp
    #elif ANIM_HOTEND && ENABLED(STATUS_HOTEND_INVERTED)
      // #define OFF_BMP(N) status_hotend##N##_b_bmp
      // #define ON_BMP(N)  status_hotend##N##_a_bmp
      #define OFF_BMP(N) image_data_whiteLens
      #define ON_BMP(N)  image_data_redLens
    #else
      #define OFF_BMP(N) status_hotend##N##_a_bmp
      #define ON_BMP(N)  status_hotend##N##_b_bmp
    #endif
    // #define HOTEND_ACTIVE(N) ((N) ? hotendActive1 : hotendActive0)
    #if STATUS_HOTEND_BITMAPS > 1
      // static const uint32_t* const status_hotend_gfx[STATUS_HOTEND_BITMAPS] PROGMEM = ARRAY_N(STATUS_HOTEND_BITMAPS, OFF_BMP(1), OFF_BMP(2), OFF_BMP(3), OFF_BMP(4), OFF_BMP(5), OFF_BMP(6));
      #if ANIM_HOTEND
        // static const uint32_t* const status_hotend_on_gfx[STATUS_HOTEND_BITMAPS] PROGMEM = ARRAY_N(STATUS_HOTEND_BITMAPS, ON_BMP(1), ON_BMP(2), ON_BMP(3), ON_BMP(4), ON_BMP(5), ON_BMP(6));
        #define HOTEND_BITMAP(N,S) (unsigned char*)pgm_read_ptr((S) ? &status_hotend_on_gfx[(N) % (STATUS_HOTEND_BITMAPS)] : &status_hotend_gfx[(N) % (STATUS_HOTEND_BITMAPS)])
      #else
        #define HOTEND_BITMAP(N,S) (unsigned char*)pgm_read_ptr(&status_hotend_gfx[(N) % (STATUS_HOTEND_BITMAPS)])
      #endif
    #elif ANIM_HOTEND
      #define HOTEND_BITMAP(N,S) ((S) ? ON_BMP() : OFF_BMP())
    #else
      #define HOTEND_BITMAP(N,S) status_hotend_a_bmp
    #endif

    // if (PAGE_CONTAINS(STATUS_HEATERS_Y, STATUS_HEATERS_BOT)) {

      #define BAR_TALL (STATUS_HEATERS_HEIGHT - 2)

      const float prop = target - 20,
                  perc = prop > 0 && temp >= 20 ? (temp - 20) / prop : 0;
      uint8_t tall = uint8_t(perc * BAR_TALL + 0.5f);
      NOMORE(tall, BAR_TALL);

      #if ANIM_HOTEND
        // Draw hotend bitmap, either whole or split by the heating percent
        const uint16_t hx = STATUS_HOTEND_X(heater),
                      hy = STATUS_HOTEND_Y(heater);
                      // bw = STATUS_HOTEND_BYTEWIDTH(heater);
        #if ENABLED(STATUS_HEAT_PERCENT)
          if (isHeat && tall <= BAR_TALL) {
            const uint8_t ph = STATUS_HEATERS_HEIGHT - 1 - tall;
            // u8g.drawBitmapP(hx, STATUS_HEATERS_Y, bw, ph, HOTEND_BITMAP(heater, false));
            // u8g.drawBitmapP(hx, STATUS_HEATERS_Y + ph, bw, tall + 1, HOTEND_BITMAP(heater, true) + ph * bw);
          }
          else
        #endif
            // u8g.drawBitmapP(hx, hy, bw, STATUS_HEATERS_HEIGHT, HOTEND_BITMAP(heater, isHeat));
            bool drawAct = false;
            if(active_extruder != MarlinUI::hotendActive){drawAct = true;}

            if(drawAct){
              LTDC_drawHomeDeviceRect(hx,hy, LAYER_HOME, active_extruder==heater?1:0);
              LTDC_IconFromLayer(hx+34, hy+12, HOTEND_PIC_WIDTH, HOTEND_PIC_HEIGHT, active_extruder==heater ?0xFFFFFFFF:TTZ_COL_ORAN,HOTEND_PIC_START, FLASH_LAYER1, LAYER_HOME,0,0);
              LTDC_drawHomeDeviceRect(hx,hy, LAYER_HOME, active_extruder==heater?1:0);
              LTDC_IconFromLayer(hx+34, hy+12, HOTEND_PIC_WIDTH, HOTEND_PIC_HEIGHT, active_extruder==heater ?0xFFFFFFFF:TTZ_COL_ORAN,HOTEND_PIC_START, FLASH_LAYER1, LAYER_HOME,0,0);

              LTDC_drawHomeDeviceRect(hx,hy, LAYER_HOME_2, active_extruder==heater?1:0);
              LTDC_IconFromLayer(hx+34, hy+12, HOTEND_PIC_WIDTH, HOTEND_PIC_HEIGHT, active_extruder==heater ?0xFFFFFFFF:TTZ_COL_ORAN,HOTEND_PIC_START, FLASH_LAYER1, LAYER_HOME_2,0,0);
              LTDC_drawHomeDeviceRect(hx,hy, LAYER_HOME_2, active_extruder==heater?1:0);
              LTDC_IconFromLayer(hx+34, hy+12, HOTEND_PIC_WIDTH, HOTEND_PIC_HEIGHT, active_extruder==heater ?0xFFFFFFFF:TTZ_COL_ORAN,HOTEND_PIC_START, FLASH_LAYER1, LAYER_HOME_2,0,0);
            }
            if(!drawAct && drawPic){           
              LTDC_drawHomeDeviceRect(hx,hy, LAYER_HOME, active_extruder==heater?1:0);
              LTDC_IconFromLayer(hx+34, hy+12, HOTEND_PIC_WIDTH, HOTEND_PIC_HEIGHT, active_extruder==heater ?0xFFFFFFFF:TTZ_COL_ORAN,HOTEND_PIC_START, FLASH_LAYER1, LAYER_HOME,0,0);
            }
            
      #endif

    // } // PAGE_CONTAINS

    // if (PAGE_UNDER(7)) {
      // #if HEATER_IDLE_HANDLER
      //   const bool dodraw = (blink || !thermalManager.hotend_idle[heater].timed_out);
      // #else
      //   constexpr bool dodraw = true;
      // #endif
      // if (dodraw) {
        _draw_centered_temp(target + 0.5, tx, ty,textLayer, backLayer);
      // }
    // }

    // if (PAGE_CONTAINS(28 - INFO_FONT_ASCENT, 28 - 1))
      _draw_centered_temp(temp + 0.5f, tx, ty+30,textLayer, backLayer);

    // if (STATIC_HOTEND && HOTEND_DOT && PAGE_CONTAINS(17, 19)) {
    //   u8g.setColorIndex(0); // set to white on black
    //   // u8g.drawBox(tx, 20 - 3, 2, 2);
    //   u8g.setColorIndex(1); // restore black on white
    // }

  }

#endif // DO_DRAW_HOTENDS

#if DO_DRAW_BED

  // Draw bed bitmap with current and target temperatures
  FORCE_INLINE void _draw_bed_status(const bool blink,bool drawPic, uint8_t textLayer, uint8_t backLayer) {
    #if !HEATER_IDLE_HANDLER
      UNUSED(blink);
    #endif

    const uint16_t tx = STATUS_BED_TEXT_X,
                   ty = STATUS_BED_TEXT_Y;

    const float temp = thermalManager.degBed(),
              target = thermalManager.degTargetBed();

    #if ENABLED(STATUS_HEAT_PERCENT) || DISABLED(STATUS_BED_ANIM)
      const bool isHeat = BED_ALT();
    #endif

    #if DISABLED(STATUS_BED_ANIM)
      #define STATIC_BED    true
      #define BED_DOT       isHeat
    #else
      #define STATIC_BED    false
      #define BED_DOT       false
    #endif

    if (PAGE_CONTAINS(STATUS_HEATERS_Y, STATUS_HEATERS_BOT)) {

      #define BAR_TALL (STATUS_HEATERS_HEIGHT - 2)

      const float prop = target - 20,
                  perc = prop > 0 && temp >= 20 ? (temp - 20) / prop : 0;
      uint8_t tall = uint8_t(perc * BAR_TALL + 0.5f);
      NOMORE(tall, BAR_TALL);

      // Draw a heating progress bar, if specified
      #if ENABLED(STATUS_HEAT_PERCENT)

        if (isHeat) {
          const uint8_t bx = STATUS_BED_X + STATUS_BED_WIDTH;
          u8g.drawFrame(bx, STATUS_HEATERS_Y, 3, STATUS_HEATERS_HEIGHT);
          if (tall) {
            const uint8_t ph = STATUS_HEATERS_HEIGHT - 1 - tall;
            if (PAGE_OVER(STATUS_HEATERS_Y + ph))
              u8g.drawVLine(bx + 1, STATUS_HEATERS_Y + ph, tall);
          }
        }

      #endif

    } // PAGE_CONTAINS

    #define BED_BITMAP(S) ((S) ? (uint8_t*)image_data_bed_on/*ON*/ : (uint8_t*)image_data_bed_off)/*OFF*/ 
    // u8g.drawBitmapP(STATUS_BED_X, STATUS_BED_Y, STATUS_BED_BYTEWIDTH, STATUS_BED_HEIGHT(BED_ALT()),  BED_BITMAP(BED_ALT()));
    
    if(BED_ALT() != MarlinUI::bedActive){
      drawPic = true;
      LTDC_drawHomeDeviceRect(STATUS_BED_X,STATUS_BED_Y, LAYER_HOME,0);
      LTDC_IconFromLayer(STATUS_BED_X+27, STATUS_BED_Y+11, TABLE_PIC_WIDTH, TABLE_PIC_HEIGHT, TTZ_COL_ORAN,TABLE_PIC_START, FLASH_LAYER1, LAYER_HOME,0,0);
    }
    MarlinUI::bedActive = BED_ALT();
    if(drawPic){
      LTDC_drawHomeDeviceRect(STATUS_BED_X,STATUS_BED_Y,LAYER_HOME,0);
      LTDC_IconFromLayer(STATUS_BED_X+27, STATUS_BED_Y+11, TABLE_PIC_WIDTH, TABLE_PIC_HEIGHT, TTZ_COL_ORAN,TABLE_PIC_START, FLASH_LAYER1, LAYER_HOME,0,0);
    }
    // if (PAGE_UNDER(7)) {
      // #if HEATER_IDLE_HANDLER
      //   const bool dodraw = (blink || !thermalManager.bed_idle.timed_out);
      // #else
      //   constexpr bool dodraw = true;
      // #endif
      // if (dodraw) {
        _draw_centered_temp(target + 0.5, tx, ty,textLayer, backLayer);
      // }
    // }

    // if (PAGE_CONTAINS(28 - INFO_FONT_ASCENT, 28 - 1))
      _draw_centered_temp(temp + 0.5f, tx, ty+30,textLayer, backLayer);

    if (STATIC_BED && BED_DOT && PAGE_CONTAINS(17, 19)) {
      u8g.setColorIndex(0); // set to white on black
      // u8g.drawBox(tx, 20 - 2, 2, 2);
      u8g.setColorIndex(1); // restore black on white
    }

  }

#endif // DO_DRAW_BED

#if DO_DRAW_CHAMBER

  FORCE_INLINE void _draw_chamber_status(const bool blink,bool drawPic, uint8_t textLayer, uint8_t backLayer) {
    UNUSED(blink);
      const uint16_t  tx = STATUS_CHAMBER_TEXT_X,
                      ty = STATUS_CHAMBER_TEXT_Y;
    #if HAS_HEATED_CHAMBER
      // if (PAGE_UNDER(7))
        _draw_centered_temp(thermalManager.degTargetChamber() + 0.5f, tx, ty,textLayer, backLayer);
    #endif

    if(drawPic){
      LTDC_drawHomeDeviceRect(STATUS_CHAMBER_X,STATUS_CHAMBER_Y, LAYER_HOME,0);
      LTDC_IconFromLayer(STATUS_CHAMBER_X+27, STATUS_CHAMBER_Y+11, CHAMBER_PIC_WIDTH, CHAMBER_PIC_HEIGHT, TTZ_COL_ORAN,CHAMBER_PIC_START, FLASH_LAYER1, LAYER_HOME,0,0);
    }

// _draw_centered_temp(target + 0.5, tx, ty,textLayer, backLayer);
    // if (PAGE_CONTAINS(28 - INFO_FONT_ASCENT, 28 - 1))
      _draw_centered_temp(thermalManager.degChamber() + 0.5f, tx, ty+30,textLayer, backLayer);
  }

#endif // DO_DRAW_CHAMBER

//
// Before homing, blink '123' <-> '???'.
// Homed but unknown... '123' <-> '   '.
// Homed and known, display constantly.
//
FORCE_INLINE void _draw_axis_value(const AxisEnum axis, const char *value, const bool blink, bool drawPic, uint8_t textLayer, uint8_t backLayer) {
  const AxisEnum a = (
    #if ENABLED(LCD_SHOW_E_TOTAL)
      axis == E_AXIS ? X_AXIS :
    #endif
    axis
  );
  const uint8_t offs = (XYZ_SPACING) * a;
  TFT_DrawChar(TFT_FONT16,(X_VALUE_POS + offs), XYZ_BASELINE, axis_codes[axis],3,LAYER_HOME_2, LAYER_HOME_2,0x0);
  // lcd_put_wchar(X_LABEL_POS + offs, XYZ_BASELINE, axis_codes[axis]);
  lcd_moveto(X_VALUE_POS + offs, XYZ_BASELINE);
  if (blink)
    TFT_DrawText(TFT_FONT16,(X_VALUE_POS + offs)+FONT_W(TFT_FONT16), XYZ_BASELINE, value,3,textLayer, backLayer,0x0);
    // lcd_put_u8str(value);
  else {
    if (!TEST(axis_homed, axis)){
      uint8_t i=1;
      while (const char c = *value++) {
        // lcd_put_wchar(c <= '.' ? c : '?');
        TFT_DrawChar(TFT_FONT16,(X_VALUE_POS + offs)+(FONT_W(TFT_FONT16)*i++), XYZ_BASELINE, c <= '.' ? c : '?',3,textLayer, backLayer,0x0);
      }
    }else {
      #if NONE(HOME_AFTER_DEACTIVATE, DISABLE_REDUCED_ACCURACY_WARNING)
        if (!TEST(axis_known_position, axis))
         TFT_DrawText(TFT_FONT16,(X_VALUE_POS + offs)+FONT_W(TFT_FONT16), XYZ_BASELINE, axis == Z_AXIS ? PSTR("       ") : PSTR("    "),3,textLayer, backLayer,0x0);
          // lcd_put_u8str_P(axis == Z_AXIS ? PSTR("       ") : PSTR("    "));
        else
      #endif
        TFT_DrawText(TFT_FONT16,(X_VALUE_POS + offs)+FONT_W(TFT_FONT16), XYZ_BASELINE, value,3,textLayer, backLayer,0x0);
          // lcd_put_u8str(value);
    }
  }
}

void MarlinUI::draw_status_screen() {


  TFT_FillScreen(TTZ_COL_BAGR,LAYER_HOME);
  
    TFT_DrawRectangle(/*x,y*/23,28, /*width, height*/422,392, /*round corner*/0,/*color*/TTZ_COL_GREY,/*back_color*/TTZ_COL_BAGR,LAYER_HOME,/*thikness in px*/97,/*fill?*/1,/*punktir?*/0);
	  TFT_DrawRectangle(/*x,y*/23,28, /*width, height*/422,392, /*round corner*/0,/*color*/TTZ_COL_ORAN,/*back_color*/TTZ_COL_ORAN,LAYER_HOME,/*thikness in px*/3,/*fill?*/0,/*punktir?*/0);
  
    TFT_DrawRectangle(/*x,y*/89,82, /*width, height*/294,251, /*round corner*/8,/*color*/0xFFFFFFFF,/*back_color*/TTZ_COL_BAGR,LAYER_HOME,/*thikness in px*/126,/*fill?*/1,/*punktir?*/0);
    
    
    // LTDC_IconFromLayer(GEAR_PIC_X, GEAR_PIC_Y, GEAR_PIC_WIDTH, GEAR_PIC_HEIGHT,0xFFFFFFFF, GEAR_PIC_START, FLASH_LAYER1, LAYER_HOME,0,0);
    LTDC_IconFromLayer(41, 43, 22, 28,0xFFFFFFFF, FILE_CURRENT_PIC_START, FLASH_LAYER1, LAYER_HOME,0,0);


    LTDC_IconFromLayer(90, 363, 24, 25,0xFFFFFFFF, CHECK_MARK_START, FLASH_LAYER1, LAYER_HOME,0,0);
    TFT_DrawRectangle(/*x,y*/125,362, /*width, height*/42,24, /*round corner*/10,/*color*/TTZ_COL_ORAN,/*back_color*/TTZ_COL_GREY,LAYER_HOME,/*thikness in px*/13,/*fill?*/1,/*punktir?*/0);
    TFT_DrawRectangle(/*x,y*/251,362, /*width, height*/42,24, /*round corner*/10,/*color*/TTZ_COL_ORAN,/*back_color*/TTZ_COL_GREY,LAYER_HOME,/*thikness in px*/13,/*fill?*/1,/*punktir?*/0);
    TFT_DrawRectangle(/*x,y*/319,362, /*width, height*/42,24, /*round corner*/10,/*color*/TTZ_COL_ORAN,/*back_color*/TTZ_COL_GREY,LAYER_HOME,/*thikness in px*/13,/*fill?*/1,/*punktir?*/0);
    LTDC_IconFromLayer(134, 368, 25, 14,0xFFFFFFFF, DZ_SMALL_START, FLASH_LAYER1, LAYER_HOME,0,0);

    
    LTDC_IconFromLayer(264, 365, 20, 20,0xFFFFFFFF, AXIS_EXTRUDER_EXSMALL_PIC_START, FLASH_LAYER1, LAYER_HOME,0,0);
    LTDC_IconFromLayer(332, 364, 18, 22,0xFFFFFFFF, CUBE_SMALL_START, FLASH_LAYER1, LAYER_HOME,0,0);

    TFT_DrawRectangle(/*x,y*/23,430, /*width, height*/676,145, /*round corner*/0,/*color*/TTZ_COL_GREY,/*back_color*/TTZ_COL_BAGR,LAYER_HOME,/*thikness in px*/74,/*fill?*/1,/*punktir?*/0);
    TFT_DrawRectangle(/*x,y*/23,430, /*width, height*/676,145, /*round corner*/0,/*color*/TTZ_COL_ORAN,/*back_color*/TTZ_COL_ORAN,LAYER_HOME,/*thikness in px*/3,/*fill?*/0,/*punktir?*/0);
    
    TFT_DrawRectangle(/*x,y*/50,466, /*width, height*/619,18, /*round corner*/9,/*color*/TTZ_COL_ORAN,/*back_color*/TTZ_COL_GREY,LAYER_HOME,/*thikness in px*/10,/*fill?*/1,/*punktir?*/0);
    TFT_DrawRectangle(/*x,y*/52,468, /*width, height*/615,14, /*round corner*/7,/*color*/0xFFFFFFFF,/*back_color*/TTZ_COL_ORAN,LAYER_HOME,/*thikness in px*/8,/*fill?*/1,/*punktir?*/0);
    
    /*BOTTOM PANEL
    * 1 SD folder button
    * 2 play button
    * 3 pause button from 2 parts [][]
    * 4 stop button
    */
    LTDC_IconFromLayer(58, 507, 61, 43,Appli_state==APPLICATION_READY?TTZ_COL_ORAN:TTZ_COL_D_GR, BOTTOM_PANEL_FOLDER_START, FLASH_LAYER1, LAYER_HOME,0,0);
    LTDC_IconFromLayer(240, 508, 58, 41,TTZ_COL_ORAN, BOTTOM_PANEL_PLAY_START, FLASH_LAYER1, LAYER_HOME,0,0);
    TFT_DrawRectangle(/*x,y*/432,509, /*width, height*/14,38, /*round corner*/0,/*color*/TTZ_COL_ORAN,/*back_color*/TTZ_COL_GREY,LAYER_HOME,/*thikness in px*/3,/*fill?*/0,/*punktir?*/0);
    TFT_DrawRectangle(/*x,y*/450,509, /*width, height*/14,38, /*round corner*/0,/*color*/TTZ_COL_ORAN,/*back_color*/TTZ_COL_GREY,LAYER_HOME,/*thikness in px*/3,/*fill?*/0,/*punktir?*/0);
    TFT_DrawRectangle(/*x,y*/609,509, /*width, height*/38,38, /*round corner*/0,/*color*/TTZ_COL_ORAN,/*back_color*/TTZ_COL_GREY,LAYER_HOME,/*thikness in px*/3,/*fill?*/0,/*punktir?*/0);




    const bool blink = get_blink();
    #if DO_DRAW_HOTENDS
      LOOP_L_N(e, EXTRUDERS)
        _draw_hotend_status((heater_ind_t)e, blink, true, LAYER_HOME_2, LAYER_HOME);
    #endif
    MarlinUI::hotendActive = active_extruder;
    // Laser / Spindle
    #if DO_DRAW_CUTTER
      if (cutter.power && PAGE_CONTAINS(STATUS_CUTTER_TEXT_Y - INFO_FONT_ASCENT, STATUS_CUTTER_TEXT_Y - 1)) {
        lcd_put_u8str(STATUS_CUTTER_TEXT_X, STATUS_CUTTER_TEXT_Y, i16tostr3rj(cutter.powerPercent(cutter.power)));
        lcd_put_wchar('%');
      }
    #endif

    // Heated Bed
    #if DO_DRAW_BED
      _draw_bed_status(blink,true, LAYER_HOME_2, LAYER_HOME);
    #endif

    // Heated Chamber
    #if DO_DRAW_CHAMBER
      _draw_chamber_status(blink,true,LAYER_HOME_2, LAYER_HOME);
    #endif







  draw_right_panel(status_screen);
  
  RewriteMemory(&hsdram1,LCD_FRAME_BUFFER(LAYER_HOME),LCD_FRAME_BUFFER(LAYER_HOME_2));
  // RewriteMemory(&hsdram1,LCD_FRAME_BUFFER(LAYER_HOME),LCD_FRAME_BUFFER(MAIN_LAYER));
}

void MarlinUI::home_recovery_rewrite() {
  for(int y=200;y<401;y++){
    for(int x=100;x<351;x++){
      *(uint32_t*) (hltdc.LayerCfg[LAYER_HOME].FBStartAdress + (4*(y*hltdc.LayerCfg[LAYER_HOME].ImageWidth + x))) = *(uint32_t*) (hltdc.LayerCfg[LAYER_HOME_2].FBStartAdress + (4*(y*hltdc.LayerCfg[LAYER_HOME_2].ImageWidth + x)));
    }
    for(int x=450;x<701;x++){
      *(uint32_t*) (hltdc.LayerCfg[LAYER_HOME].FBStartAdress + (4*(y*hltdc.LayerCfg[LAYER_HOME].ImageWidth + x))) = *(uint32_t*) (hltdc.LayerCfg[LAYER_HOME_2].FBStartAdress + (4*(y*hltdc.LayerCfg[LAYER_HOME_2].ImageWidth + x)));
    }
  }
}
void MarlinUI::home_sd_rewrite() {
  for(int y=100;y<553;y++){
    for(int x=150;x<651;x++){
      *(uint32_t*) (hltdc.LayerCfg[LAYER_HOME].FBStartAdress + (4*(y*hltdc.LayerCfg[LAYER_HOME].ImageWidth + x))) = *(uint32_t*) (hltdc.LayerCfg[LAYER_HOME_2].FBStartAdress + (4*(y*hltdc.LayerCfg[LAYER_HOME_2].ImageWidth + x)));
    }
  }
}
void MarlinUI::status_screen_refresh() {


  uint8_t drawLayer = LAYER_HOME;
  uint8_t backLayer = LAYER_HOME_2;

  /*****************************************************************/
  // TFT_FillScreen(TTZ_COL_BAGR,LAYER_HOME);

  static char xstring[5
    #if ENABLED(LCD_SHOW_E_TOTAL)
      + 7
    #endif
  ], ystring[5], zstring[8];
  #if ENABLED(FILAMENT_LCD_DISPLAY)
    static char wstring[5], mstring[4];
  #endif

  #if HAS_PRINT_PROGRESS
    #if DISABLED(DOGM_SD_PERCENT)
      #define _SD_INFO_X(len) (PROGRESS_BAR_X + (PROGRESS_BAR_WIDTH) / 2 - (len) * (MENU_FONT_WIDTH) / 2)
    #else
      #define _SD_INFO_X(len) (LCD_PIXEL_WIDTH - (len) * (MENU_FONT_WIDTH))
    #endif

    #if ENABLED(DOGM_SD_PERCENT)
      static char progress_string[5];
    #endif
    static uint8_t lastElapsed = 0xFF, lastProgress = 0xFF;
    // static u8g_uint_t elapsed_x_pos = 0, progress_bar_solid_width = 0;
    static char elapsed_string[16];
    #if ENABLED(SHOW_REMAINING_TIME)
      static u8g_uint_t estimation_x_pos = 0;
      static char estimation_string[10];
      #if BOTH(DOGM_SD_PERCENT, ROTATE_PROGRESS_DISPLAY)
        static u8g_uint_t progress_x_pos = 0;
        static uint8_t progress_state = 0;
        static bool prev_blink = 0;
      #endif
    #endif
  #endif

  const bool showxy = (true
    #if ENABLED(LCD_SHOW_E_TOTAL)
      && !printingIsActive()
    #endif
  );

  // At the first page, generate new display values
  // if (first_page) {
    #if ANIM_HBCC
      uint8_t new_bits = 0;
      #if ANIM_HOTEND
        HOTEND_LOOP() if (thermalManager.isHeatingHotend(e)) SBI(new_bits, HEATBIT_HOTEND + e);
      #endif
      #if ANIM_BED
        if (thermalManager.isHeatingBed()) SBI(new_bits, HEATBIT_BED);
      #endif
      #if DO_DRAW_CHAMBER && HAS_HEATED_CHAMBER
        if (thermalManager.isHeatingChamber()) SBI(new_bits, HEATBIT_CHAMBER);
      #endif
      #if ANIM_CUTTER
        if (cutter.enabled()) SBI(new_bits, HEATBIT_CUTTER);
      #endif
      heat_bits = new_bits;
    #endif

    const xyz_pos_t lpos = current_position.asLogical();
    strcpy(zstring, ftostr52sp(lpos.z));

    if (showxy) {
      strcpy(xstring, ftostr4sign(lpos.x));
      strcpy(ystring, ftostr4sign(lpos.y));
    }
    else {
      #if ENABLED(LCD_SHOW_E_TOTAL)
        const uint8_t escale = e_move_accumulator >= 100000.0f ? 10 : 1; // After 100m switch to cm
        sprintf_P(xstring, PSTR("%ld%cm"), uint32_t(_MAX(e_move_accumulator, 0.0f)) / escale, escale == 10 ? 'c' : 'm'); // 1234567mm
      #endif
    }

    #if ENABLED(FILAMENT_LCD_DISPLAY)
      strcpy(wstring, ftostr12ns(filwidth.measured_mm));
      strcpy(mstring, i16tostr3rj(planner.volumetric_percent(parser.volumetric_enabled)));
    #endif

    // Progress / elapsed / estimation updates and string formatting to avoid float math on each LCD draw
    #if HAS_PRINT_PROGRESS
      const progress_t progress =
        #if HAS_PRINT_PROGRESS_PERMYRIAD
          get_progress_permyriad()
        #else
          get_progress_percent()
        #endif
      ;
      duration_t elapsed = print_job_timer.duration();
      const uint8_t p = progress & 0xFF, ev = elapsed.value & 0xFF;
      if (p != lastProgress) {
        lastProgress = p;

        // progress_bar_solid_width = u8g_uint_t((PROGRESS_BAR_WIDTH - 2) * (progress / (PROGRESS_SCALE)) * 0.01f);

        #if ENABLED(DOGM_SD_PERCENT)
          if (progress == 0) {
            progress_string[0] = '\0';
            #if ENABLED(SHOW_REMAINING_TIME)
              estimation_string[0] = '\0';
              estimation_x_pos = _SD_INFO_X(0);
            #endif
          }
          else {
            strcpy(progress_string, (
              #if ENABLED(PRINT_PROGRESS_SHOW_DECIMALS)
                permyriadtostr4(progress)
              #else
                ui8tostr3rj(progress / (PROGRESS_SCALE))
              #endif
            ));
          }
          #if BOTH(SHOW_REMAINING_TIME, ROTATE_PROGRESS_DISPLAY) // Tri-state progress display mode
            progress_x_pos = _SD_INFO_X(strlen(progress_string) + 1);
          #endif
        #endif
      }

      // constexpr bool can_show_days = DISABLED(DOGM_SD_PERCENT) || ENABLED(ROTATE_PROGRESS_DISPLAY);
      if (ev != lastElapsed) {
        lastElapsed = ev;
        // const uint8_t len = elapsed.toDigital(elapsed_string, can_show_days && elapsed.value >= 60*60*24L);
        // elapsed_x_pos = _SD_INFO_X(len);

        #if ENABLED(SHOW_REMAINING_TIME)
          if (!(ev & 0x3)) {
            uint32_t timeval = (0
              #if BOTH(LCD_SET_PROGRESS_MANUALLY, USE_M73_REMAINING_TIME)
                + get_remaining_time()
              #endif
            );
            if (!timeval && progress > 0) timeval = elapsed.value * (100 * (PROGRESS_SCALE) - progress) / progress;
            if (!timeval) {
              estimation_string[0] = '\0';
              estimation_x_pos = _SD_INFO_X(0);
            }
            else {
              duration_t estimation = timeval;
              const uint8_t len = estimation.toDigital(estimation_string, can_show_days && estimation.value >= 60*60*24L);
              estimation_x_pos = _SD_INFO_X(len
                #if !BOTH(DOGM_SD_PERCENT, ROTATE_PROGRESS_DISPLAY)
                  + 1
                #endif
              );
            }
          }
        #endif
      }
    #endif
  // }

  const bool blink = get_blink();

  // Status Menu Font
  // set_font(FONT_STATUSMENU);

  #if DO_DRAW_LOGO
    if (PAGE_CONTAINS(STATUS_LOGO_Y, STATUS_LOGO_Y + STATUS_LOGO_HEIGHT - 1))
      u8g.drawBitmapP(STATUS_LOGO_X, STATUS_LOGO_Y, STATUS_LOGO_BYTEWIDTH, STATUS_LOGO_HEIGHT, status_logo_bmp);
  #endif

  #if STATUS_HEATERS_WIDTH
    // Draw all heaters (and maybe the bed) in one go
    if (PAGE_CONTAINS(STATUS_HEATERS_Y, STATUS_HEATERS_Y + STATUS_HEATERS_HEIGHT - 1))
      u8g.drawBitmapP(STATUS_HEATERS_X, STATUS_HEATERS_Y, STATUS_HEATERS_BYTEWIDTH, STATUS_HEATERS_HEIGHT, status_heaters_bmp);
  #endif

  #if DO_DRAW_CUTTER && DISABLED(STATUS_COMBINE_HEATERS)
    #if ANIM_CUTTER
      #define CUTTER_BITMAP(S) ((S) ? status_cutter_on_bmp : status_cutter_bmp)
    #else
      #define CUTTER_BITMAP(S) status_cutter_bmp
    #endif
    const uint8_t cuttery = STATUS_CUTTER_Y(CUTTER_ALT()),
                  cutterh = STATUS_CUTTER_HEIGHT(CUTTER_ALT());
    if (PAGE_CONTAINS(cuttery, cuttery + cutterh - 1))
      u8g.drawBitmapP(STATUS_CUTTER_X, cuttery, STATUS_CUTTER_BYTEWIDTH, cutterh, CUTTER_BITMAP(CUTTER_ALT()));
  #endif


  char str[10];
  const char *tmpSign = (home_offset.pos[Z_AXIS] < 0) ? "-" : "";
  float tmpVal = (home_offset.pos[Z_AXIS] < 0) ? -home_offset.pos[Z_AXIS] : home_offset.pos[Z_AXIS];
  int tmpInt1 = tmpVal;                  // Get the integer (678).
  float tmpFrac = tmpVal - tmpInt1;      // Get fraction (0.0123).
  int tmpInt2 = trunc(tmpFrac * 100);  // Turn into integer (123).
  // Print as parts, note that you need 0-padding for fractional bit.
  sprintf (str, "%s%d.%d", tmpSign, tmpInt1, tmpInt2);
  TFT_DrawText(TFT_FONT16,175, 369, str, 4, drawLayer, backLayer,0x0);

  const ip_addr_t *ip;
  ip = netif_ip_addr4(&gnetif);
  char ipaddr[16];
  const uint8_t ip1 = ((uint32_t)(ip->addr) & 0x000000FF);
  const uint8_t ip2 = ((uint32_t)(ip->addr) & 0x0000FF00)>>8;
  const uint8_t ip3 = ((uint32_t)(ip->addr) & 0x00FF0000)>>16;
  const uint8_t ip4 = ((uint32_t)(ip->addr) & 0xFF000000)>>24;
  sprintf(ipaddr,"%i.%i.%i.%i",ip1,ip2,ip3,ip4);
  TFT_DrawText(TFT_FONT16,5, 5, ipaddr, 16, LAYER_HOME, LAYER_HOME_2,0x0);


  // #if DO_DRAW_CHAMBER && DISABLED(STATUS_COMBINE_HEATERS)
  //   #if ANIM_CHAMBER
  //     #define CHAMBER_BITMAP(S) ((S) ? status_chamber_on_bmp : status_chamber_bmp)
  //   #else
  //     #define CHAMBER_BITMAP(S) status_chamber_bmp
  //   #endif
  //   const uint8_t chambery = STATUS_CHAMBER_Y(CHAMBER_ALT()),
  //                 chamberh = STATUS_CHAMBER_HEIGHT(CHAMBER_ALT());
  //   if (PAGE_CONTAINS(chambery, chambery + chamberh - 1))
  //     u8g.drawBitmapP(STATUS_CHAMBER_X, chambery, STATUS_CHAMBER_BYTEWIDTH, chamberh, CHAMBER_BITMAP(CHAMBER_ALT()));
  // #endif



  //
  // Temperature Graphics and Info
  //

    #if DO_DRAW_HOTENDS
      LOOP_L_N(e, EXTRUDERS)
        _draw_hotend_status((heater_ind_t)e, blink, false, drawLayer, backLayer);
    #endif
    MarlinUI::hotendActive = active_extruder;
    // Laser / Spindle
    #if DO_DRAW_CUTTER
      if (cutter.power && PAGE_CONTAINS(STATUS_CUTTER_TEXT_Y - INFO_FONT_ASCENT, STATUS_CUTTER_TEXT_Y - 1)) {
        lcd_put_u8str(STATUS_CUTTER_TEXT_X, STATUS_CUTTER_TEXT_Y, i16tostr3rj(cutter.powerPercent(cutter.power)));
        lcd_put_wchar('%');
      }
    #endif

    // Heated Bed
    #if DO_DRAW_BED
      _draw_bed_status(blink,false, drawLayer, backLayer);
    #endif

    // Heated Chamber
    #if DO_DRAW_CHAMBER
      _draw_chamber_status(blink,false, drawLayer, backLayer);
    #endif


  #if ENABLED(SDSUPPORT)
    //
    // SD Card Symbol
    //
    if (card.isFileOpen() && PAGE_CONTAINS(42, 51)) {
      // Upper box
      // u8g.drawBox(42, 42, 8, 7);     // 42-48 (or 41-47)
      // Right edge
      // u8g.drawBox(50, 44, 2, 5);     // 44-48 (or 43-47)
      // Bottom hollow box
      // u8g.drawFrame(42, 49, 10, 4);  // 49-52 (or 48-51)
      // Corner pixel
      // u8g.drawPixel(50, 43);         // 43 (or 42)
    }
  #endif // SDSUPPORT

  //
  // XYZ Coordinates
  //

  #if ENABLED(XYZ_HOLLOW_FRAME)
    #define XYZ_FRAME_TOP 29
    #define XYZ_FRAME_HEIGHT INFO_FONT_ASCENT + 3
  #else
    #define XYZ_FRAME_TOP 30
    #define XYZ_FRAME_HEIGHT INFO_FONT_ASCENT + 1
  #endif

  // if (PAGE_CONTAINS(XYZ_FRAME_TOP, XYZ_FRAME_TOP + XYZ_FRAME_HEIGHT - 1)) {

  #if ENABLED(XYZ_HOLLOW_FRAME)
    // u8g.drawFrame(0, XYZ_FRAME_TOP, LCD_PIXEL_WIDTH, XYZ_FRAME_HEIGHT); // 8: 29-40  7: 29-39
  #else
    u8g.drawBox(0, XYZ_FRAME_TOP, LCD_PIXEL_WIDTH, XYZ_FRAME_HEIGHT);   // 8: 30-39  7: 30-37
  #endif

  // if (PAGE_CONTAINS(XYZ_BASELINE - (INFO_FONT_ASCENT - 1), XYZ_BASELINE)) {

  #if DISABLED(XYZ_HOLLOW_FRAME)
    u8g.setColorIndex(0); // white on black
  #endif

  #if DUAL_MIXING_EXTRUDER

    // Two-component mix / gradient instead of XY

    char mixer_messages[12];
    const char *mix_label;
    #if ENABLED(GRADIENT_MIX)
      if (mixer.gradient.enabled) {
        mixer.update_mix_from_gradient();
        mix_label = "Gr";
      }
      else
    #endif
      {
        mixer.update_mix_from_vtool();
        mix_label = "Mx";
      }
    sprintf_P(mixer_messages, PSTR("%s %d;%d%% "), mix_label, int(mixer.mix[0]), int(mixer.mix[1]));
    lcd_put_u8str(X_LABEL_POS, XYZ_BASELINE, mixer_messages);

  #else

    if (showxy) {
      _draw_axis_value(X_AXIS, xstring, blink,true, drawLayer, backLayer);
      _draw_axis_value(Y_AXIS, ystring, blink,true, drawLayer, backLayer);
    }
    else {
      _draw_axis_value(E_AXIS, xstring, true,true, drawLayer, backLayer);
      // TFT_DrawText((elapsed_x_pos)*2, 195, PSTR("       "),LAYER_HOME,0x0);
      // lcd_put_u8str_P(PSTR("       "));
    }

  #endif

  _draw_axis_value(Z_AXIS, zstring, blink,true, drawLayer, backLayer);

  #if DISABLED(XYZ_HOLLOW_FRAME)
    u8g.setColorIndex(1); // black on white
  #endif

  #define EXTRAS_2_BASELINE (EXTRAS_BASELINE + 3)

  set_font(FONT_MENU);
  TFT_DrawText(TFT_FONT16,280, 443, PSTR("Скорость:"),0,drawLayer, backLayer,0x0);

  set_font(FONT_STATUSMENU);
  TFT_DrawText(TFT_FONT16,280+FONT_W(TFT_FONT16)*UTF_strlen(PSTR("Скорость:")), 443, i16tostr3rj(feedrate_percentage),3,drawLayer, backLayer,0x0);
  TFT_DrawChar(TFT_FONT16,280+FONT_W(TFT_FONT16)*(UTF_strlen(PSTR("Скорость:"))+strlen(i16tostr3rj(feedrate_percentage))), 443, '%',1,drawLayer, backLayer,0x0);


  uint8_t percent_old = percent;
  percent = card.percentDone();
  /*        */
  if(card.flag.sdprinting){
    if(percent>percent_old){
      
      for(uint8_t dr = percent_old+1;dr<=percent;dr++){

        
        
        
        uint32_t color = TTZ_COL_GREE;
        if(percent-percent_old>2)color = LCD_COLOR_CYAN;
        if(dr == 1){
          TFT_DrawFLatLine(46+dr*6 + 0, 473, 46+dr*6 + 0, 477, color, drawLayer);
          TFT_DrawFLatLine(46+dr*6 + 1, 471, 46+dr*6 + 1, 479, color, drawLayer);
          TFT_DrawFLatLine(46+dr*6 + 2, 470, 46+dr*6 + 2, 480, color, drawLayer);
          TFT_DrawFLatLine(46+dr*6 + 3, 469, 46+dr*6 + 3, 481, color, drawLayer);
          TFT_DrawFLatLine(46+dr*6 + 4, 469, 46+dr*6 + 4, 481, color, drawLayer);
          TFT_DrawFLatLine(46+dr*6 + 5, 468, 46+dr*6 + 5, 482, color, drawLayer);
          
        }else{
          for(int i=0;i<6;i++){
            TFT_DrawFLatLine(46+dr*6 + i, 468, 46+dr*6 + i, 482, color, drawLayer);
          }
        }

      }

    }

  }

  // TFT_DrawRectangle(/*x,y*/50,466, /*width, height*/619,18, /*round corner*/9,/*color*/TTZ_COL_ORAN,/*back_color*/TTZ_COL_GREY,LAYER_HOME,/*thikness in px*/10,/*fill?*/1,/*punktir?*/0);
  // TFT_DrawRectangle(/*x,y*/52,468, /*width, height*/615,14, /*round corner*/7,/*color*/0xFFFFFFFF,/*back_color*/TTZ_COL_ORAN,LAYER_HOME,/*thikness in px*/8,/*fill?*/1,/*punktir?*/0);
    

  //
  // Filament sensor display if SD is disabled
  //
  #if ENABLED(FILAMENT_LCD_DISPLAY) && DISABLED(SDSUPPORT)
    lcd_put_u8str(56, EXTRAS_2_BASELINE, wstring);
    lcd_put_u8str(102, EXTRAS_2_BASELINE, mstring);
    lcd_put_wchar('%');
    set_font(FONT_MENU);
    lcd_put_wchar(47, EXTRAS_2_BASELINE, LCD_STR_FILAM_DIA[0]); // lcd_put_u8str_P(PSTR(LCD_STR_FILAM_DIA));
    lcd_put_wchar(93, EXTRAS_2_BASELINE, LCD_STR_FILAM_MUL[0]);
  #endif

  LTDC_IconFromLayer(58, 507, 61, 43,Appli_state==APPLICATION_READY?TTZ_COL_ORAN:TTZ_COL_D_GR, BOTTOM_PANEL_FOLDER_START, FLASH_LAYER1, LAYER_HOME,0,0);
  
  TFT_DrawText(TFT_FONT16,50, 580, status_message,status_new==1?50:0, LAYER_HOME, LAYER_HOME_2,0x0);
  if(status_new)status_new=0;
}
void MarlinUI::draw_right_panel(screenFunc_t screen) {//MarlinUI::menu_options()

  uint8_t layer_index = 0;
  if(screen == status_screen)    {layer_index = LAYER_HOME;}
  if(screen == menu_calib)       {layer_index = LAYER_CAL;}
  if(screen == menu_options)     {layer_index = LAYER_OPT;}
  if(screen == menu_light)       {layer_index = LAYER_LIGHT;}
  if(screen == menu_filament)    {layer_index = LAYER_MAT;}
  if(screen == menu_camera)      {layer_index = LAYER_CAM;}
  if(screen == menu_axis)        {layer_index = LAYER_AXIS;}

  LTDC_IconFromLayer(RIGHT_PANEL_HOME_X, RIGHT_PANEL_HOME_Y, RIGHT_PANEL_HOME_WIDTH, RIGHT_PANEL_HOME_HEIGHT,(screen==status_screen) ? 0xFFFFFFFF:TTZ_COL_ORAN, RIGHT_PANEL_HOME_PIC_START, FLASH_LAYER1, layer_index,0,0);
  TFT_DrawText(TFT_FONT12,737, 92, PSTR("Принт"),0,layer_index, layer_index,0x0);
  
  LTDC_IconFromLayer(RIGHT_PANEL_CALIB_X, RIGHT_PANEL_CALIB_Y, RIGHT_PANEL_CALIB_WIDTH, RIGHT_PANEL_CALIB_HEIGHT,(screen==menu_calib) ? 0xFFFFFFFF:TTZ_COL_ORAN, RIGHT_PANEL_CALIB_PIC_START, FLASH_LAYER1, layer_index,0,0);
  TFT_DrawText(TFT_FONT12,718, 148, PSTR("Калибровка"),0,layer_index, layer_index,0x0);
  
  LTDC_IconFromLayer(RIGHT_PANEL_OPTIONS_X, RIGHT_PANEL_OPTIONS_Y, RIGHT_PANEL_OPTIONS_WIDTH, RIGHT_PANEL_OPTIONS_HEIGHT,(screen==menu_options) ? 0xFFFFFFFF:TTZ_COL_D_GR, RIGHT_PANEL_OPTIONS_PIC_START, FLASH_LAYER1, layer_index,0,0);
  #ifdef MENU_OPTS
  TFT_DrawText(TFT_FONT12,717, 233, PSTR("Настройки"),0,layer_index, layer_index,0x0);
  #else
  TFT_DrawText(TFT_FONT12,717, 233, PSTR("---------"),0,layer_index, layer_index,0x0);
  #endif 
  
  LTDC_IconFromLayer(RIGHT_PANEL_LIGHT_X, RIGHT_PANEL_LIGHT_Y, RIGHT_PANEL_LIGHT_WIDTH, RIGHT_PANEL_LIGHT_HEIGHT,(screen==menu_light) ? 0xFFFFFFFF:TTZ_COL_D_GR, RIGHT_PANEL_LIGHT_PIC_START, FLASH_LAYER1, layer_index,0,0);
  #ifdef MENU_LIGHT
  TFT_DrawText(TFT_FONT12,717, 316, PSTR("Подсветка"),0,layer_index, layer_index,0x0); 
  #else
  TFT_DrawText(TFT_FONT12,717, 316, PSTR("---------"),0,layer_index, layer_index,0x0); 
  #endif 
  
  LTDC_IconFromLayer(RIGHT_PANEL_CAMERA_X, RIGHT_PANEL_CAMERA_Y, RIGHT_PANEL_CAMERA_WIDTH, RIGHT_PANEL_CAMERA_HEIGHT,(screen==menu_camera) ? 0xFFFFFFFF:TTZ_COL_D_GR, RIGHT_PANEL_CAMERA_PIC_START, FLASH_LAYER1, layer_index,0,0);
  #ifdef MENU_CAMERA
  TFT_DrawText(TFT_FONT12,717, 395, PSTR("Камера"),0,layer_index, layer_index,0x0); 
  #else
  TFT_DrawText(TFT_FONT12,717, 395, PSTR("---------"),0,layer_index, layer_index,0x0); 
  #endif 
  
  LTDC_IconFromLayer(RIGHT_PANEL_FILAMENT_X, RIGHT_PANEL_FILAMENT_Y, RIGHT_PANEL_FILAMENT_WIDTH, RIGHT_PANEL_FILAMENT_HEIGHT,(screen==menu_filament) ? 0xFFFFFFFF:TTZ_COL_ORAN, RIGHT_PANEL_FILAMENT_PIC_START, FLASH_LAYER1, layer_index,0,0);
  TFT_DrawText(TFT_FONT12,716, 476, PSTR("Материалы"),0,layer_index, layer_index,0x0);
  
  LTDC_IconFromLayer(RIGHT_PANEL_AXIS_X, RIGHT_PANEL_AXIS_Y, RIGHT_PANEL_AXIS_WIDTH, RIGHT_PANEL_AXIS_HEIGHT,(screen==menu_axis) ? 0xFFFFFFFF:TTZ_COL_ORAN, RIGHT_PANEL_AXIS_PIC_START, FLASH_LAYER1, layer_index,0,0);
  TFT_DrawText(TFT_FONT12,717, 563, PSTR("Движение"),0,layer_index, layer_index,0x0); 
  TFT_DrawText(TFT_FONT12,721, 575, PSTR("по осям"),0,layer_index, layer_index,0x0); 
}

void MarlinUI::draw_status_message(const bool blink, bool drawPic, uint8_t textLayer, uint8_t backLayer) {

  // Get the UTF8 character count of the string

  #if ENABLED(STATUS_MESSAGE_SCROLLING)

  uint8_t slen = utf8_strlen(status_message);
    static bool last_blink = false;

    if (slen <= LCD_WIDTH) {
      // The string fits within the line. Print with no scrolling
      lcd_put_u8str(status_message);
      while (slen < LCD_WIDTH) { lcd_put_wchar(' '); ++slen; }
    }
    else {
      // String is longer than the available space

      // Get a pointer to the next valid UTF8 character
      // and the string remaining length
      uint8_t rlen;
      const char *stat = status_and_len(rlen);
      lcd_put_u8str_max(stat, LCD_PIXEL_WIDTH);

      // If the remaining string doesn't completely fill the screen
      if (rlen < LCD_WIDTH) {
        lcd_put_wchar('.');                     // Always at 1+ spaces left, draw a dot
        uint8_t chars = LCD_WIDTH - rlen;       // Amount of space left in characters
        if (--chars) {                          // Draw a second dot if there's space
          lcd_put_wchar('.');
          if (--chars) {                        // Print a second copy of the message
            lcd_put_u8str_max(status_message, LCD_PIXEL_WIDTH - (rlen + 2) * (MENU_FONT_WIDTH));
            lcd_put_wchar(' ');
          }
        }
      }
      if (last_blink != blink) {
        last_blink = blink;
        advance_status_scroll();
      }
    }

  #else // !STATUS_MESSAGE_SCROLLING

    UNUSED(blink);

    // Just print the string to the LCD
    // TFT_DrawText(50, 580, status_message,40,textLayer, backLayer,0x0);
    
    // lcd_put_u8str_max(status_message, LCD_PIXEL_WIDTH);

    // Fill the rest with spaces
    // for (; slen < LCD_WIDTH; ++slen) lcd_put_wchar(' ');

  #endif // !STATUS_MESSAGE_SCROLLING
}

#endif // HAS_GRAPHICAL_LCD && !LIGHTWEIGHT_UI
