/**
 * Marlin 3D Printer Firmware
 * Copyright (c) 2020 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
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

#include "../../inc/MarlinConfigPre.h"

#if ENABLED(TOUCH_BUTTONS)

#include "xpt2046.h"
#include "../../inc/MarlinConfig.h"
#include "../../lcd/menu/menu.h"
extern void menu_motion();

#ifndef TOUCH_INT_PIN
  #define TOUCH_INT_PIN  T_IRQ_PIN //US_DIR 151
#endif
#ifndef TOUCH_MISO_PIN
  #define TOUCH_MISO_PIN T_MISO_PIN //T_CK1  54
#endif
#ifndef TOUCH_MOSI_PIN
  #define TOUCH_MOSI_PIN T_MOSI_PIN //T_EN1  54
#endif
#ifndef TOUCH_SCK_PIN
  #define TOUCH_SCK_PIN  T_SCK_PIN //TS_EN2 54
#endif
#ifndef TOUCH_CS_PIN
  #define TOUCH_CS_PIN   T_CS_PIN //TS_CK2 55
#endif

struct touchEXTI touch_btn = {
	false,//pressed?
	0,//x
	0,//y
  0,//btn code
  0//time
};

XPT2046 touch;
extern int8_t encoderDiff;
// static screenFunc_t;
// extern screenFunc_t currentScreen;
// extern  screenFunc_t MarlinUI::currentScreen; 
// extern  MarlinUI::status_screen(); 

static touchOrienation _orient = XPT2046_LANDSCAPE_180;
static uint16_t   _width = 800,
                 _height = 600;

/* Калибровочные значения дисплея */
#define XPT2046_MIN_RAW_X 105
#define XPT2046_MAX_RAW_X 2010
#define XPT2046_MIN_RAW_Y 50
#define XPT2046_MAX_RAW_Y 1980
uint16_t a_x, a_y, a_z1, a_z2;
uint32_t r_x, r_y, r_z1, r_z2;

void XPT2046::init() {
  SET_INPUT(TOUCH_MISO_PIN);
  SET_OUTPUT(TOUCH_MOSI_PIN);
  SET_OUTPUT(TOUCH_SCK_PIN);
  OUT_WRITE(TOUCH_CS_PIN, HIGH);

  #if PIN_EXISTS(TOUCH_INT)
    // Optional Pendrive interrupt pin
    SET_INPUT(TOUCH_INT_PIN);
  #endif

  // Read once to enable pendrive status pin
  getInTouch(XPT2046_X);
}

#include "../../lcd/ultralcd.h" // For EN_C bit mask

uint8_t XPT2046::returnBtn(screenFunc_t currentScreen) {

  return 0;
}
uint16_t XPT2046::read_buttons(screenFunc_t currentScreen) {
    // char testPing[10];
    // sprintf(testPing,"echo:1");
    // HAL_UART_Transmit(&huart2, (uint8_t *)testPing, strlen(testPing), 10);

  int16_t tsoffsets[4] = { 0 };
  // uint8_t testVar = 0;

  if (tsoffsets[0] + tsoffsets[1] == 0) {
    // Not yet set, so use defines as fallback...
    tsoffsets[0] = XPT2046_X_CALIBRATION;
    tsoffsets[1] = XPT2046_X_OFFSET;
    tsoffsets[2] = XPT2046_Y_CALIBRATION;
    tsoffsets[3] = XPT2046_Y_OFFSET;
  }

  // We rely on XPT2046 compatible mode to ADS7843, hence no Z1 and Z2 measurements possible.

  if (!isTouched()) return 0;
    touch_t t = get16samples();

    uint16_t x_t = t.x;
    uint16_t y_t = t.y;
    if(x_t < 10) x_t = 10;
    if(x_t > 790) x_t = 790;
    if(y_t > 590) y_t = 590;
    if(y_t < 10) y_t = 10;
    uint16_t x = x_t;
    uint16_t y = y_t;


    // char txData[100];
    // sprintf(txData,"echo: xy = %d %d  \r\n",x,y);
    // HAL_UART_Transmit(&huart2, (uint8_t *)txData, strlen(txData), 10);



  // const uint16_t y =  uint16_t(((uint32_t(getInTouch(XPT2046_X))) * tsoffsets[0]) ) + tsoffsets[1],
  //                x =  uint16_t(((uint32_t(getInTouch(XPT2046_Y))) * tsoffsets[2]) ) + tsoffsets[3];
  // if (!isTouched()) return 0; // Fingers must still be on the TS for a valid read.

		//  TFT_DrawFilledCircle(y,x,10,LCD_COLOR_RANDOM);
		  // *(uint32_t*) (hltdc.LayerCfg[0].FBStartAdress + (4*(y*hltdc.LayerCfg[0].ImageWidth + x))) = (uint32_t)((rand() & 0xff) | (rand() & 0xff)<<8 | (rand() & 0xff)<<16 | ( 0xff)<<24);
  
  
  /*
  for (vector<Widget>::size_type i = 0; i != oven_display.widgets_vector.size(); i++)
  {
    if (oven_display.widgets_vector[i].button_name != NO_BUTTON)
    {
      if ((WITHIN(x, oven_display.widgets_vector[i].wgt_coord_x, (oven_display.widgets_vector[i].wgt_coord_x + oven_display.widgets_vector[i].wgt_width))) && \
          (WITHIN(y, oven_display.widgets_vector[i].wgt_coord_y, (oven_display.widgets_vector[i].wgt_coord_y + oven_display.widgets_vector[i].wgt_height))))
      {
        return oven_display.widgets_vector[i].button_name;
      }
    }
  }
  */

  return oven_display.identify_pressed_btn(x, y);
}

bool XPT2046::isTouched() {
  return (

    // HAL_GPIO_ReadPin(TS_IRQ_GPIO_Port, TS_IRQ_Pin) == GPIO_PIN_SET
    // #if PIN_EXISTS(TOUCH_INT)
      READ(TOUCH_INT_PIN) != HIGH
    // #else
    //   getInTouch(XPT2046_Z1) >= XPT2046_Z1_THRESHOLD
    // #endif
  );
}

uint16_t XPT2046::SPI_Soft_Getdata(uint8_t coordinate){
  const uint8_t coord = uint8_t(coordinate) | XPT2046_CONTROL | XPT2046_DFR_MODE;
	HAL_GPIO_WritePin(TS_SCK_GPIO_Port, TS_SCK_Pin, GPIO_PIN_RESET);
	for(uint8_t j = 0x80; j; j >>= 1){
		HAL_GPIO_WritePin(TS_SCK_GPIO_Port, TS_SCK_Pin, GPIO_PIN_RESET);
		GPIO_PinState bit = GPIO_PIN_RESET;
		if((coord & j)!=0)bit = GPIO_PIN_SET;
		HAL_GPIO_WritePin(TS_MOSI_GPIO_Port, TS_MOSI_Pin, bit);
	  delay_us(30);
		HAL_GPIO_WritePin(TS_SCK_GPIO_Port, TS_SCK_Pin, GPIO_PIN_SET);
	  delay_us(30);
	}
	HAL_GPIO_WritePin(TS_MOSI_GPIO_Port, TS_MOSI_Pin, GPIO_PIN_RESET);
	uint16_t data = 0;
	for(uint8_t i=0;i<16;i++){
		HAL_GPIO_WritePin(TS_SCK_GPIO_Port, TS_SCK_Pin, GPIO_PIN_RESET);
		data<<=1;
		if(HAL_GPIO_ReadPin(TS_MISO_GPIO_Port, TS_MISO_Pin))data|=1;
    delay_us(30);
		HAL_GPIO_WritePin(TS_SCK_GPIO_Port, TS_SCK_Pin, GPIO_PIN_SET);
	  delay_us(30);
	}
	HAL_GPIO_WritePin(TS_SCK_GPIO_Port, TS_SCK_Pin, GPIO_PIN_RESET);
	return data;
}

uint16_t XPT2046::get1sample(const XPTCoordinate coordinate) {
  uint16_t data;
  const uint8_t coord = uint8_t(coordinate) | XPT2046_CONTROL | XPT2046_DFR_MODE;
  
    for (uint8_t j = 0x80; j; j >>= 1) {
      WRITE(TOUCH_SCK_PIN, LOW);
      WRITE(TOUCH_MOSI_PIN, bool(coord & j));
      WRITE(TOUCH_SCK_PIN, HIGH);
    }

    data = 0;
    for (uint16_t j = 0x8000; j; j >>= 1) {
      WRITE(TOUCH_SCK_PIN, LOW);
      if (READ(TOUCH_MISO_PIN)) data |= j;
      WRITE(TOUCH_SCK_PIN, HIGH);
    }
    WRITE(TOUCH_SCK_PIN, LOW);
    data >>= 4;
 
  return data;
}

touch_t XPT2046::get16samples() {

	//Переменная, возвращаемая из функции
	static touch_t touch = {0,0,T_noTouch}; //Переменная сохраняет предыдущие координаты
	
	uint32_t avg_x = 0; //Среднее значение X
	uint32_t avg_y = 0;	//Среднее значение Y
        uint32_t avg_z1 = 0;	//Среднее значение Z1
        uint32_t avg_z2 = 0;	//Среднее значение Z2
	uint8_t nsamples = 0; //Количество сделанных выборок
        a_x = 0;
	      a_y = 0;
        a_z1 = 0;
        a_z2 = 0;
  HAL_GPIO_WritePin(TS_CS_GPIO_Port,     TS_CS_Pin,           GPIO_PIN_RESET);
	//Цикл выборки
	for(uint8_t i = 0; i < XPT2046_SAMPLES; i++)	{
		// if(HAL_GPIO_ReadPin(XPT2046_IRQ_GPIO_Port, XPT2046_IRQ_Pin) == GPIO_PIN_SET) break; //Если тачскрин был отпущен, то выборка не происходит
		//Получение значений по Y
    a_y = SPI_Soft_Getdata(XPT2046_Y)>>4;
    //Получение значений по X
    a_x = SPI_Soft_Getdata(XPT2046_X)>>4;
    //Получение значений по Z1
    // a_z1 = SPI_Soft_Getdata(XPT2046_Z1)>>4;
    // //Получение значений по Z2
    // a_z2 = SPI_Soft_Getdata(XPT2046_Z2)>>4;
    ///////////////////
		avg_y += a_y; //(((uint16_t)y_raw[0]) << 8) | ((uint16_t)y_raw[1]);
		avg_x += a_x; //(((uint16_t)x_raw[0]) << 8) | ((uint16_t)x_raw[1]);
                avg_z1 += a_z1;
                avg_z2 += a_z2;
		//Инкримент счётчика выборок
		nsamples++; 
	}

  HAL_GPIO_WritePin(TS_CS_GPIO_Port,     TS_CS_Pin,           GPIO_PIN_SET);

  /* Вычисление значений координат */
	//Усреднение значений 
	uint32_t raw_x = (avg_x / nsamples);
	uint32_t raw_y = (avg_y / nsamples);
        uint32_t raw_z1 = (avg_z1 / nsamples);
        uint32_t raw_z2 = (avg_z2 / nsamples);
        r_x = raw_x;
        r_y = raw_y;
        r_z1 = raw_z1;
        r_z2 = raw_z2;
        touch.z1 = raw_z1;
        touch.z2 = raw_z2;



    if(_orient == XPT2046_LANDSCAPE) {
		touch.y = _height-(raw_y - XPT2046_MIN_RAW_Y) * _height / (XPT2046_MAX_RAW_Y - XPT2046_MIN_RAW_Y);
		touch.x = (raw_x - XPT2046_MIN_RAW_X) * _width / (XPT2046_MAX_RAW_X - XPT2046_MIN_RAW_X);
	}
	if(_orient == XPT2046_LANDSCAPE_180) {
		touch.y = (raw_y - XPT2046_MIN_RAW_Y) * _height / (XPT2046_MAX_RAW_Y - XPT2046_MIN_RAW_Y);
		touch.x = _width-(raw_x - XPT2046_MIN_RAW_X) * _width / (XPT2046_MAX_RAW_X - XPT2046_MIN_RAW_X);
	}
	if(_orient == XPT2046_PORTRAIT) {
		touch.x = (raw_y - XPT2046_MIN_RAW_Y) * _height / (XPT2046_MAX_RAW_Y - XPT2046_MIN_RAW_Y);
		touch.y = (raw_x - XPT2046_MIN_RAW_X) * _width / (XPT2046_MAX_RAW_X - XPT2046_MIN_RAW_X);
	}
	if(_orient == XPT2046_PORTRAIT_180) {
		touch.x = _height - (raw_y - XPT2046_MIN_RAW_Y) * _height / (XPT2046_MAX_RAW_Y - XPT2046_MIN_RAW_Y);
		touch.y = _width - (raw_x - XPT2046_MIN_RAW_X) * _width / (XPT2046_MAX_RAW_X - XPT2046_MIN_RAW_X);
	}

	return touch;

}

uint16_t XPT2046::getInTouch(const XPTCoordinate coordinate) {
  uint16_t data[3];

  OUT_WRITE(TOUCH_CS_PIN, LOW);

  const uint8_t coord = uint8_t(coordinate) | XPT2046_CONTROL | XPT2046_DFR_MODE;
  for (uint16_t i = 0; i < 3 ; i++) {
    for (uint8_t j = 0x80; j; j >>= 1) {
      WRITE(TOUCH_SCK_PIN, LOW);
      WRITE(TOUCH_MOSI_PIN, bool(coord & j));
      WRITE(TOUCH_SCK_PIN, HIGH);
    }

    data[i] = 0;
    for (uint16_t j = 0x8000; j; j >>= 1) {
      WRITE(TOUCH_SCK_PIN, LOW);
      if (READ(TOUCH_MISO_PIN)) data[i] |= j;
      WRITE(TOUCH_SCK_PIN, HIGH);
    }
    WRITE(TOUCH_SCK_PIN, LOW);
    data[i] >>= 4;
  }

  WRITE(TOUCH_CS_PIN, HIGH);

  uint16_t delta01 = _MAX(data[0], data[1]) - _MIN(data[0], data[1]),
           delta02 = _MAX(data[0], data[2]) - _MIN(data[0], data[2]),
           delta12 = _MAX(data[1], data[2]) - _MIN(data[1], data[2]);

  if (delta01 <= delta02 && delta01 <= delta12)
    return (data[0] + data[1]) >> 1;

  if (delta02 <= delta12)
    return (data[0] + data[2]) >> 1;

  return (data[1] + data[2]) >> 1;
}

bool XPT2046::getTouchPoint(uint16_t &x, uint16_t &y) {
  if (isTouched()) {
    x = getInTouch(XPT2046_X);
    y = getInTouch(XPT2046_Y);
  }
  return isTouched();
}

#endif // TOUCH_BUTTONS
