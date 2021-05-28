// #include "stm32f4xx_hal.h"

#include "drv8711_custom.h"
#include "SPI.h"
#include "../pins/stm32/pins_TOTALZ_PRINTER.h"
// #include <SPI.h>

// // #include "img_argb32bit.h"
// #define USART_DIR_GPIO_Port GPIOD
// #define USART_DIR_Pin GPIO_PIN_7
extern SPI_HandleTypeDef            hspi3;

extern SPIClass SPI;

char msg0_8711REG[40] = "000200";
char msg1_8711REG[40] = "000201";
char msg2_8711REG[40] = "000202";
char msg3_8711REG[40] = "000203";
char msg4_8711REG[40] = "000204";
char msg5_8711REG[40] = "000205";
char msg6_8711REG[40] = "000206";
char msg7_8711REG[40] = "000207";
char msg8_8711REG[40] = "000208";
char msg9_8711REG[40] = "000209";
char usart_string_8711REG[50];
char hexvalue_8711REG[10];
char regname_8711REG[40];



uint16_t XmotorCTRL = 0x0;
uint16_t YmotorCTRL = 0x0;
uint16_t ZmotorCTRL = 0x0;
uint16_t E0motorCTRL = 0x0;
uint16_t E1motorCTRL = 0x0;
uint16_t E2motorCTRL = 0x0;

struct drvDefSettings SLIMZ = {
	0x0E3B,//ctrl
	0x113A,//torque
	0x2032,//off
	0x30AA,//blank
	0x4310,//decay
	0x5A02,//stall
	0x6000//drive
};
struct drvDefSettings SLIME = {	
	0x0E33,//ctrl
	0x113A,//torque
	0x2001,//off
	0x300A,//blank
	0x4210,//decay
	0x5A02,//stall
	0x6000//drive
};



struct drvDefSettings M_17HS2408 = {
	0x0C29,//ctrl
	0x11FF,//torque
	0x2032,//off
	0x3100,//blank
	0x4510,//decay
	0x5A02,//stall
	0x6000//drive
};
struct drvDefSettings M_FL42STH331334A = {
	0x0E33,//ctrl
	0x1190,//torque
	0x2032,//off
	0x3100,//blank
	0x4510,//decay
	0x5A02,//stall
	0x6000//drive
};

struct drvDefSettings M_FL42STH381684A = {	
	0x0E33,//ctrl
	0x113C,//torque
	0x2001,//off
	0x300A,//blank
	0x4301,//decay
	0x5A02,//stall
	0x6000//drive
};
struct drvDefSettings M_42HD402701 = {
	0x0C29,//ctrl
	0x1118,//torque
	0x2032,//off
	0x3100,//blank
	0x4510,//decay
	0x5A02,//stall
	0x6555//drive
};
struct drvDefSettings M_KS42STH40 = {
	0x0C39,//ctrl
	0x1118,//torque
	0x2032,//off
	0x3100,//blank
	0x4510,//decay
	0x5A02,//stall
	0x6555//drive
};

struct drvDefSettings DRV_TEST = {
	0x0C39,//ctrl
	0x1118,//torque
	0x2032,//off
	0x3100,//blank
	0x4510,//decay
	0x5A02,//stall
	0x6555//drive
};


// extern drvDefSettings DRV_TEST;

#define DRV8711_X   M_FL42STH381684A
#define DRV8711_Y   M_FL42STH381684A
#define DRV8711_Z   SLIMZ
#define DRV8711_E0  SLIME
#define DRV8711_E1  M_FL42STH331334A
#define DRV8711_E2  M_FL42STH331334A



struct motorRegs CTRL_REG = {	
	DRV8711_X.ctrl,//
	DRV8711_Y.ctrl,//
	DRV8711_Z.ctrl,//
	DRV8711_E0.ctrl,//
	DRV8711_E1.ctrl,//
	DRV8711_E2.ctrl,//
};

void driver_onoff(uint8_t onoff, uint8_t axis, GPIO_TypeDef* port, uint16_t pin, void (*f)(uint8_t) ){
    
	struct drvDefSettings motor = DRV8711_X;
	uint8_t pin_stm = X_ENABLE_PIN;
	uint16_t ctrl_old = 0x0;
	uint16_t ctrl_new = 0x0;
	switch(axis){
		case 0/*X_AXIS*/: 	motor = DRV8711_X; 	pin_stm = X_ENABLE_PIN;		ctrl_old = CTRL_REG.Xreg; 	ctrl_new = (ctrl_old & 0xFFFE) | onoff; CTRL_REG.Xreg = ctrl_new; 	break;
		case 1/*Y_AXIS*/: 	motor = DRV8711_Y; 	pin_stm = Y_ENABLE_PIN; 	ctrl_old = CTRL_REG.Yreg; 	ctrl_new = (ctrl_old & 0xFFFE) | onoff; CTRL_REG.Yreg = ctrl_new;	break;
		case 2/*Z_AXIS*/: 	motor = DRV8711_Z; 	pin_stm = Z_ENABLE_PIN; 	ctrl_old = CTRL_REG.Zreg; 	ctrl_new = (ctrl_old & 0xFFFE) | onoff; CTRL_REG.Zreg = ctrl_new;	break;
		case 3/*E0_AXIS*/: 	motor = DRV8711_E0; pin_stm = E0_ENABLE_PIN; 	ctrl_old = CTRL_REG.E0reg; 	ctrl_new = (ctrl_old & 0xFFFE) | onoff; CTRL_REG.E0reg = ctrl_new;	break;
		case 4/*E1_AXIS*/: 	motor = DRV8711_E1; pin_stm = E1_ENABLE_PIN; 	ctrl_old = CTRL_REG.E1reg; 	ctrl_new = (ctrl_old & 0xFFFE) | onoff; CTRL_REG.E1reg = ctrl_new;	break;
		case 5/*E2_AXIS*/: 	motor = DRV8711_E2; pin_stm = E2_ENABLE_PIN; 	ctrl_old = CTRL_REG.E2reg; 	ctrl_new = (ctrl_old & 0xFFFE) | onoff; CTRL_REG.E2reg = ctrl_new;	break;
	}
	if(ctrl_new != ctrl_old){RegisterTxRx(ctrl_new, "000200", port, pin, f, pin_stm);}
}
void starterSettings(uint8_t axis, GPIO_TypeDef* port, uint16_t pin, void (*f)(uint8_t) ){
	struct drvDefSettings motor = DRV8711_X;
	uint8_t pin_stm = X_ENABLE_PIN;
	switch(axis){
		case 0/*X_AXIS*/: motor = DRV8711_X; pin_stm = PI8; break;
		case 1/*Y_AXIS*/: motor = DRV8711_Y; pin_stm = PC15; break;
		case 2/*Z_AXIS*/: motor = DRV8711_Z; pin_stm = PA3; break;
		case 3/*E0_AXIS*/: motor = DRV8711_E0; pin_stm = E0_ENABLE_PIN; break;
		case 4/*E1_AXIS*/: motor = DRV8711_E1; pin_stm = E1_ENABLE_PIN; break;
		case 5/*E2_AXIS*/: motor = DRV8711_E2; pin_stm = E2_ENABLE_PIN; break;
	}
	
	RegisterTxRx(motor.ctrl, 	"000200", port, pin, f, pin_stm);
	RegisterTxRx(motor.torque, 	"000201", port, pin, f, pin_stm);
	RegisterTxRx(motor.off, 	"000202", port, pin, f, pin_stm);
	RegisterTxRx(motor.blank, 	"000203", port, pin, f, pin_stm);
	RegisterTxRx(motor.decay, 	"000204", port, pin, f, pin_stm);
	RegisterTxRx(motor.stall, 	"000205", port, pin, f, pin_stm);
	RegisterTxRx(motor.drive, 	"000206", port, pin, f, pin_stm);
	RegisterTxRx(0x7000,        "000207", port, pin, f, pin_stm);
}

void RegisterTxRx(uint16_t DataByte1, char * s, GPIO_TypeDef* port, uint16_t pin,  void (*f)(uint8_t), uint8_t pin_stm){

	uint8_t part1,part2;

	part1 = DataByte1 >> 8;
	part2 = DataByte1 & 0xFF;
	// SPI.transfer();
	// HAL_GPIO_WritePin(port, pin,GPIO_PIN_SET);//включаем пин драйвера на общение
		// HAL_SPI_Transmit(&hspi3, (uint8_t*)(&part1), 1, 20);
		// HAL_SPI_Transmit(&hspi3, (uint8_t*)(&part2), 1, 20);
		SPI.transfer(pin_stm, part1, SPI_CONTINUE);
		SPI.transfer(pin_stm, part2, SPI_CONTINUE);
	// (*f)(part1);
	// (*f)(part2);
	HAL_GPIO_WritePin(port, pin,GPIO_PIN_RESET);//выключаем пин драйвера


	// for(int i=0;i<40;i++){
	// 	regname_8711REG[i] = s[i];
	// }

	// uint16_t read_DRV_SPI,addr_DRV_SPI;
	// uint16_t command_DRV_SPI;

	// read_DRV_SPI		= (DataByte1 & 0x8000);
	// command_DRV_SPI 	= (DataByte1 & 0x0fff);
	// addr_DRV_SPI 		= (DataByte1 & 0x7000);

	// setSingleDriver(DataByte1,addr_DRV_SPI,command_DRV_SPI,read_DRV_SPI, port, pin);
}


void setSingleDriver(uint16_t DataByte1, uint16_t addr, uint16_t command, uint16_t read, GPIO_TypeDef* port, uint16_t pin){	
	// SPI_HandleTypeDef hspi3;
	// MX_SPI3_Init();
// 	if(!read){
// 		uint8_t buf1;
		
// 	uint8_t part1 = DataByte1 >> 8;
// 		HAL_GPIO_WritePin(port, pin,GPIO_PIN_SET);//включаем пин драйвера на общение
// 		HAL_SPI_TransmitReceive(&hspi3, (uint8_t*)(&part1), &buf1, 1, 20);
// 		HAL_GPIO_WritePin(port, pin,GPIO_PIN_RESET);//выключаем пин драйвера
// 	}

// 	uint16_t buf1;
// 	command = (command | addr | 0x8000);
// 	HAL_GPIO_WritePin(port, pin,GPIO_PIN_SET);//включаем пин драйвера на общение
// 	HAL_SPI_TransmitReceive(&hspi3, (uint8_t*)(&command), &buf1, 1, 20);
// 	HAL_GPIO_WritePin(port, pin,GPIO_PIN_RESET);//выключаем пин драйвера

// 	buf1 = (buf1 & 0x0fff) | addr;
// 	//sprintf(usart_string_8711REG, "%X\n", buf1);
// 	char buf2[10];
// 	getRegFromAddr(&buf2,addr);

// 	// HAL_GPIO_WritePin(USART_DIR_GPIO_Port, USART_DIR_Pin, GPIO_PIN_SET);
// //	sprintf(usart_string_8711REG,"%s",&regname_8711REG);
// //	USART_SendToCom(&usart_string_8711REG);
// 	sprintf(usart_string_8711REG," %X",buf1);

// 	char stringtransfer[strlen(regname_8711REG)+strlen(usart_string_8711REG)-1];
// 	for(int i=0;i<strlen(regname_8711REG);i++){
// 		stringtransfer[i] = regname_8711REG[i];
// 	}
// 	for(int i=0;i<strlen(usart_string_8711REG);i++){
// 		stringtransfer[strlen(regname_8711REG)+i] = usart_string_8711REG[i+1];
// 	}
// 	char *newstr1 = malloc(strlen(stringtransfer) + 2);
// 	strcpy(newstr1, stringtransfer);
// 	strcat(newstr1, "\r");
// 	char *newstr2 = malloc(strlen(newstr1) + 2);
// 	strcpy(newstr2, newstr1);
// 	strcat(newstr2, "\n");
//	stringtransfer[strlen(regname_8711REG)+strlen(usart_string_8711REG)] = 0x13;
//	stringtransfer[strlen(regname_8711REG)+strlen(usart_string_8711REG)+1] = 0x10;

	// USART_SendToCom(newstr2);
	// HAL_GPIO_WritePin(USART_DIR_GPIO_Port, USART_DIR_Pin,GPIO_PIN_RESET);
}

void getRegFromAddr(char *buf2, uint16_t addr){
	if(addr == 0x0000){buf2 = "CTRL";}
	if(addr == 0x1000){buf2 = "TORQUE";}
	if(addr == 0x2000){buf2 = "OFF";}
	if(addr == 0x3000){buf2 = "BLANK";}
	if(addr == 0x4000){buf2 = "DECAY";}
	if(addr == 0x5000){buf2 = "STALL";}
	if(addr == 0x6000){buf2 = "DRIVE";}
}