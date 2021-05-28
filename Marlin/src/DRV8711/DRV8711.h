#pragma once

#include "../../inc/MarlinConfig.h"
#include <SPI.h>
#include "drv8711_custom.h"


#define  ENABLE_STEPPER_X() do{driver_onoff(1,X_AXIS, X_ENABLE_GPIO_Port, X_ENABLE_Pin_MX, spiSend);}while(0)
#define DISABLE_STEPPER_X() do{driver_onoff(0,X_AXIS, X_ENABLE_GPIO_Port, X_ENABLE_Pin_MX, spiSend);}while(0)
#define  ENABLE_STEPPER_Y() do{driver_onoff(1,Y_AXIS, Y_ENABLE_GPIO_Port, Y_ENABLE_Pin_MX, spiSend);}while(0)
#define DISABLE_STEPPER_Y() do{driver_onoff(0,Y_AXIS, Y_ENABLE_GPIO_Port, Y_ENABLE_Pin_MX, spiSend);}while(0)
#define  ENABLE_STEPPER_Z() do{driver_onoff(1,Z_AXIS, Z_ENABLE_GPIO_Port, Z_ENABLE_Pin_MX, spiSend);}while(0)
#define DISABLE_STEPPER_Z() do{driver_onoff(0,Z_AXIS, Z_ENABLE_GPIO_Port, Z_ENABLE_Pin_MX, spiSend);}while(0)
#define  ENABLE_STEPPER_E0() do{driver_onoff(1,E0_AXIS, E0_ENABLE_GPIO_Port, E0_ENABLE_Pin_MX, spiSend);}while(0)
#define DISABLE_STEPPER_E0() do{driver_onoff(0,E0_AXIS, E0_ENABLE_GPIO_Port, E0_ENABLE_Pin_MX, spiSend);}while(0)
#define  ENABLE_STEPPER_E1() do{driver_onoff(1,E1_AXIS, E1_ENABLE_GPIO_Port, E1_ENABLE_Pin_MX, spiSend);}while(0)
#define DISABLE_STEPPER_E1() do{driver_onoff(0,E1_AXIS, E1_ENABLE_GPIO_Port, E1_ENABLE_Pin_MX, spiSend);}while(0)
#define  ENABLE_STEPPER_E2() do{driver_onoff(1,E2_AXIS, E2_ENABLE_GPIO_Port, E2_ENABLE_Pin_MX, spiSend);}while(0)
#define DISABLE_STEPPER_E2() do{driver_onoff(0,E2_AXIS, E2_ENABLE_GPIO_Port, E2_ENABLE_Pin_MX, spiSend);}while(0)




#define X_ENABLE_INIT() ENABLE_STEPPER_X()
#define X_ENABLE_WRITE(STATE) do{driver_onoff(STATE,X_AXIS, X_ENABLE_GPIO_Port, X_ENABLE_Pin_MX, spiSend);}while(0)
#define X_ENABLE_READ() 0//bool(READ(X_ENABLE_PIN))

#define Y_ENABLE_INIT() ENABLE_STEPPER_Y()
#define Y_ENABLE_WRITE(STATE) do{driver_onoff(STATE,Y_AXIS, Y_ENABLE_GPIO_Port, Y_ENABLE_Pin_MX, spiSend);}while(0)
#define Y_ENABLE_READ() 0//bool(READ(Y_ENABLE_PIN))

#define Z_ENABLE_INIT() ENABLE_STEPPER_Z()
#define Z_ENABLE_WRITE(STATE) do{driver_onoff(STATE,Z_AXIS, Z_ENABLE_GPIO_Port, Z_ENABLE_Pin_MX, spiSend);}while(0)
#define Z_ENABLE_READ() 0//bool(READ(Z_ENABLE_PIN))

#define E0_ENABLE_INIT() ENABLE_STEPPER_E0()
#define E0_ENABLE_WRITE(STATE) do{driver_onoff(STATE,E0_AXIS, E0_ENABLE_GPIO_Port, E0_ENABLE_Pin_MX, spiSend);}while(0)
#define E0_ENABLE_READ() 0//bool(READ(E0_ENABLE_PIN))

#define E1_ENABLE_INIT() ENABLE_STEPPER_E1()
#define E1_ENABLE_WRITE(STATE) do{driver_onoff(STATE,E1_AXIS, E1_ENABLE_GPIO_Port, E1_ENABLE_Pin_MX, spiSend);}while(0)
#define E1_ENABLE_READ() 0//bool(READ(E1_ENABLE_PIN))

#define E2_ENABLE_INIT() ENABLE_STEPPER_E2()
#define E2_ENABLE_WRITE(STATE) do{driver_onoff(STATE,E2_AXIS, E2_ENABLE_GPIO_Port, E2_ENABLE_Pin_MX, spiSend);}while(0)
#define E2_ENABLE_READ() 0//bool(READ(E2_ENABLE_PIN))
