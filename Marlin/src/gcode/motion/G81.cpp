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

#include "../../inc/MarlinConfigPre.h"

// #if ENABLED(GCODE_MOTION_MODES)

#include "../gcode.h"

#include "../../HAL/shared/Delay.h"
// #include "../../core/mx_init.h"
// #include "../../DRV8711/drv8711_custom.h"
/**
 * G81: Change motor stepper settings
 */

struct drvDefSettings2{
	uint16_t ctrl;
	uint16_t torque;
	uint16_t off;
	uint16_t blank;
	uint16_t decay;
	uint16_t stall;
	uint16_t drive;
};

struct drvDefSettings2 test1 = {
	0x0E31,//ctrl
	0x1170,//torque
	0x2032,//off
	0x3100,//blank
	0x4510,//decay
	0x5A02,//stall
	0x6000//drive
};

void starterSettings2(struct drvDefSettings2 motor, GPIO_TypeDef* port, uint16_t pin, void (*f)(uint8_t));
void RegisterTxRx2(uint16_t, char * s, GPIO_TypeDef* port, uint16_t pin, void (*f)(uint8_t));
void GcodeSuite::G81() {


  //   // spiBegin();
  //   // spiInit(2);

    
  //   // uint8_t s0 = 0x80;
  //   uint8_t s1 = 0x90;
  //   // uint8_t s2 = 0xA0;
  //   // uint8_t s3 = 0xB0;
  //   // uint8_t s4 = 0xC0;
  //   // uint8_t s5 = 0xD0;
  //   // uint8_t s6 = 0xE0;
  //   // uint8_t s7 = 0xF0;
  //   uint8_t s00 = 0x00;

  //   // uint8_t buf00;
  //   // uint8_t buf01;
  //   uint8_t buf10;
  //   uint8_t buf11;
  //   // uint8_t buf20;
  //   // uint8_t buf21;
  //   // uint8_t buf30;
  //   // uint8_t buf31;
  //   // uint8_t buf40;
  //   // uint8_t buf41;
  //   // uint8_t buf50;
  //   // uint8_t buf51;
  //   // uint8_t buf60;
  //   // uint8_t buf61;
  //   // uint8_t buf70;
  //   // uint8_t buf71;
  // // struct drvDefSettings test1 = {
  // //   0x0E31,//ctrl
  // //   0x1170,//torque
  // //   0x2032,//off
  // //   0x3100,//blank
  // //   0x4510,//decay
  // //   0x5A02,//stall
  // //   0x6000//drive
  // // };
  // spiInit(6);
  // // starterSettings2(test1, Z_ENABLE_GPIO_Port, Z_ENABLE_Pin_MX, spiSend);
  //   // HAL_GPIO_WritePin(Z_ENABLE_GPIO_Port, Z_ENABLE_Pin_MX, GPIO_PIN_SET);    
  //   // // spiSend(0x80);
  //   // // spiSend(0x00);  
  //   // // // spiSend(0x90);
  //   // // // spiSend(0x00);  
  //   // spiSend(0xA0);
  //   // spiSend(0x00);  
  //   // spiSend(0xB0);
  //   // spiSend(0x00);  
  //   // spiSend(0xC0);
  //   // spiSend(0x00);  
  //   // spiSend(0xD0);
  //   // spiSend(0x00);  
  //   // spiSend(0xE0);
  //   // spiSend(0x00);  
  //   // spiSend(0xF0);
  //   // spiSend(0x00);  
  //   // HAL_GPIO_WritePin(Z_ENABLE_GPIO_Port, Z_ENABLE_Pin_MX, GPIO_PIN_RESET);

  //   // HAL_GPIO_WritePin(Z_ENABLE_GPIO_Port, Z_ENABLE_Pin_MX, GPIO_PIN_SET);    
  //   // HAL_SPI_TransmitReceive(&hspi3, &s0, &buf00, 1, 20);
  //   // HAL_SPI_TransmitReceive(&hspi3, &s00, &buf01, 1, 20);
  //   // HAL_GPIO_WritePin(Z_ENABLE_GPIO_Port, Z_ENABLE_Pin_MX, GPIO_PIN_RESET);
  //   // delay(10);
  //   HAL_GPIO_WritePin(Z_ENABLE_GPIO_Port, Z_ENABLE_Pin_MX, GPIO_PIN_SET);   
  //   HAL_SPI_TransmitReceive(&hspi3, &s1, &buf10, 1, 20);
  //   HAL_SPI_TransmitReceive(&hspi3, &s00, &buf11, 1, 20);
  //   HAL_GPIO_WritePin(Z_ENABLE_GPIO_Port, Z_ENABLE_Pin_MX, GPIO_PIN_RESET);
  //   // delay(10);
  //   // HAL_GPIO_WritePin(Z_ENABLE_GPIO_Port, Z_ENABLE_Pin_MX, GPIO_PIN_SET);
  //   // HAL_SPI_TransmitReceive(&hspi3, &s2, &buf20, 1, 20);
  //   // HAL_SPI_TransmitReceive(&hspi3, &s00, &buf21, 1, 20);
  //   // HAL_GPIO_WritePin(Z_ENABLE_GPIO_Port, Z_ENABLE_Pin_MX, GPIO_PIN_RESET);
  //   // delay(10);
  //   // HAL_GPIO_WritePin(Z_ENABLE_GPIO_Port, Z_ENABLE_Pin_MX, GPIO_PIN_SET);
  //   // HAL_SPI_TransmitReceive(&hspi3, &s3, &buf30, 1, 20);
  //   // HAL_SPI_TransmitReceive(&hspi3, &s00, &buf31, 1, 20);
  //   // HAL_GPIO_WritePin(Z_ENABLE_GPIO_Port, Z_ENABLE_Pin_MX, GPIO_PIN_RESET);
  //   // delay(10);
  //   // HAL_GPIO_WritePin(Z_ENABLE_GPIO_Port, Z_ENABLE_Pin_MX, GPIO_PIN_SET);
  //   // HAL_SPI_TransmitReceive(&hspi3, &s4, &buf40, 1, 20);
  //   // HAL_SPI_TransmitReceive(&hspi3, &s00, &buf41, 1, 20);
  //   // HAL_GPIO_WritePin(Z_ENABLE_GPIO_Port, Z_ENABLE_Pin_MX, GPIO_PIN_RESET);
  //   // delay(10);
  //   // HAL_GPIO_WritePin(Z_ENABLE_GPIO_Port, Z_ENABLE_Pin_MX, GPIO_PIN_SET);
  //   // HAL_SPI_TransmitReceive(&hspi3, &s5, &buf50, 1, 20);
  //   // HAL_SPI_TransmitReceive(&hspi3, &s00, &buf51, 1, 20);
  //   // HAL_GPIO_WritePin(Z_ENABLE_GPIO_Port, Z_ENABLE_Pin_MX, GPIO_PIN_RESET);
  //   // delay(10);
  //   // HAL_GPIO_WritePin(Z_ENABLE_GPIO_Port, Z_ENABLE_Pin_MX, GPIO_PIN_SET);
  //   // HAL_SPI_TransmitReceive(&hspi3, &s6, &buf60, 1, 20);
  //   // HAL_SPI_TransmitReceive(&hspi3, &s00, &buf61, 1, 20);
  //   // HAL_GPIO_WritePin(Z_ENABLE_GPIO_Port, Z_ENABLE_Pin_MX, GPIO_PIN_RESET);
  //   // delay(10);
  //   // HAL_GPIO_WritePin(Z_ENABLE_GPIO_Port, Z_ENABLE_Pin_MX, GPIO_PIN_SET);
  //   // HAL_SPI_TransmitReceive(&hspi3, &s7, &buf70, 1, 20);
  //   // HAL_SPI_TransmitReceive(&hspi3, &s00, &buf71, 1, 20);
  //   // HAL_GPIO_WritePin(Z_ENABLE_GPIO_Port, Z_ENABLE_Pin_MX, GPIO_PIN_RESET);


  //   /*char txData[100];
  //   sprintf(txData,"echo: G81 responce:  %02X%02X  %02X%02X  %02X%02X  %02X%02X  %02X%02X  %02X%02X  %02X%02X \r\n",\
  //   buf00,buf01,\
  //   buf10,buf11,\
  //   buf20,buf21,\
  //   buf30,buf31,\
  //   buf40,buf41,\
  //   buf50,buf51,\
  //   buf60,buf61,\
  //   buf70,buf71);
  //   HAL_UART_Transmit(&huart2, (uint8_t *)txData, strlen(txData), 10);
  //   */


  // // parser.cancel_motion_mode();

}



void starterSettings2(struct drvDefSettings2 motor, GPIO_TypeDef* port, uint16_t pin, void (*f)(uint8_t) ){
  /*
	RegisterTxRx2(motor.ctrl, 	"000200", port, pin, f);
	RegisterTxRx2(motor.torque, 	"000201", port, pin, f);
	RegisterTxRx2(motor.off, 	"000202", port, pin, f);
	RegisterTxRx2(motor.blank, 	"000203", port, pin, f);
	RegisterTxRx2(motor.decay, 	"000204", port, pin, f);
	RegisterTxRx2(motor.stall, 	"000205", port, pin, f);
	RegisterTxRx2(motor.drive, 	"000206", port, pin, f);
	RegisterTxRx2(0x7000,        "000207", port, pin, f);
  */
}
// #endif // GCODE_MOTION_MODES
void RegisterTxRx2(uint16_t DataByte1, char * s, GPIO_TypeDef* port, uint16_t pin,  void (*f)(uint8_t)){

	uint8_t part1,part2;

	part1 = DataByte1 >> 8;
	part2 = DataByte1 & 0xFF;

	HAL_GPIO_WritePin(port, pin,GPIO_PIN_SET);//включаем пин драйвера на общение
	(*f)(part1);
	(*f)(part2);
	HAL_GPIO_WritePin(port, pin,GPIO_PIN_RESET);//выключаем пин драйвера

}
