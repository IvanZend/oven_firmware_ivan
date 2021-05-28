// #include "main.h"
#pragma once

#ifndef __USBFLASH_H_
#define __USBFLASH_H_
#include "fatfs.h"
#include "usb_host.h"

#ifdef __cplusplus
 extern "C" {
#endif 

// extern 				ApplicationTypeDef Appli_state;
// extern 				USBH_HandleTypeDef hUsbHostFS;
// // extern				bmp1;
// extern 		char 	SD_Path[4]; 										/* SD logical drive path */
// extern 		char 	uintToHex[50];
// // int 				USBstatusOld;
// // int 				USBstatusNew;
// extern 		FATFS 				USBDISKFatFs;           							/* File system object for USB disk logical drive */
// extern 		FIL 				FileFromSD;                   							/* File object */
// extern 		DIR 				usb_directory;
// extern 		FRESULT 			usb_result;
// extern 		uint8_t 			sect[4096];
// // uint32_t 			bytesread;


// // extern 		void 	USART_SendToCom(char (*s)[]);
// int 				connectToUSB(int val);
int 				getFirstSize(uint8_t *buf, int n);
// void 				loadImageToLCD(void);
// void 				USBreadBmp(void);
// void 				getDirs(void);
// //void 				getFiles(void);
// FRESULT 			scan_files(char* path);
// uint8_t 			read256fromUSB(uint8_t *buf, int n, int firstSize, int offset);
// uint32_t 			OpenBMP(FIL FileFromSD,uint8_t *ptr, const char* fname);


#ifdef __cplusplus
}
#endif

#endif /* __USBFLASH_H_ */