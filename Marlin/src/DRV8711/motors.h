// #pragma once
// #include "stm32f4xx_hal.h"
// // #include <string.h>
// // #include <stdlib.h>

// struct drvDefSettings{
// 	uint16_t ctrl;
// 	uint16_t torque;
// 	uint16_t off;
// 	uint16_t blank;
// 	uint16_t decay;
// 	uint16_t stall;
// 	uint16_t drive;
// };


// struct drvDefSettings M_17HS2408 = {
// 	0x0F01,//ctrl
// 	0x11FF,//torque
// 	0x2032,//off
// 	0x3100,//blank
// 	0x4510,//decay
// 	0x5A02,//stall
// 	0x6000//drive
// };
// struct drvDefSettings M_FL42STH331334A = {
// 	0x0F01,//ctrl
// 	0x11FF,//torque
// 	0x2032,//off
// 	0x3100,//blank
// 	0x4510,//decay
// 	0x5A02,//stall
// 	0x6000//drive
// };

// struct drvDefSettings M_FL42STH381684A = {
// 	0x0F01,//ctrl
// 	0x11FF,//torque
// 	0x2032,//off
// 	0x3100,//blank
// 	0x4510,//decay
// 	0x5A02,//stall
// 	0x6000//drive
// };
// struct drvDefSettings M_42HD402701 = {
// 	0x0C11,//ctrl
// 	0x111C,//torque
// 	0x2032,//off
// 	0x3100,//blank
// 	0x4510,//decay
// 	0x5A02,//stall
// 	0x6000//drive
// };


// void starterSettings(struct drvDefSettings motor);