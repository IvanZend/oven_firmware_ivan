// #include "main.h"
#include "USBflash.h"
#include "ltdc.h"
#include "stm32_def.h"


// int connectToUSB(int val){
// 	// while(Appli_state != APPLICATION_READY){
// 	// 	USBstatusNew = 0;
// 	// 	if(USBstatusOld != USBstatusNew){
// 	// 		sprintf(uintToHex,"NO USB  \r\n");
// 	// 		// USART_SendToCom(&uintToHex);
// 	// 		USBstatusOld = 0;
// 	// 		// TFT_DrawText(10, 50, "NO USB                             ",MAIN_LAYER);
// 	// 	}
// 	// 	MX_USB_HOST_Process();
// 	// }
// 	// USBstatusNew = 1;
// 	// //////////////////////////////////////////////////////////////////////////

// 	// //////////////////////////////////////////////////////////////////////////
// 	// //usb_result = f_mount(&USBDISKFatFs, USBHPath, 1);
// 	// if(f_mount(&USBDISKFatFs, (TCHAR const*)USBHPath, 0) != FR_OK)
// 	// {
// 	// 	USBstatusNew = 0;
// 	// 	USBstatusOld = 0;
// 	// 	sprintf(uintToHex,"USB MOUNT ERROR  \r\n");
// 	// 	// USART_SendToCom(&uintToHex);
// 	// 	_Error_Handler(__FILE__, __LINE__);
// 	// 	return 0;
// 	// }
// 	// if(USBstatusOld != USBstatusNew){
// 	// 	sprintf(uintToHex,"USB MOUNTED  \r\n");
// 	// 	// USART_SendToCom(&uintToHex);
// 	// 	USBstatusOld = 1;
// 	// 	// TFT_DrawText(10, 50, "USB MOUNTED                                  ",MAIN_LAYER);

// 	// 	//#if(FLASH_ENABLED == 1)
// 	// 	if(val == 0){
// 	// 		//getDirs();
// 	// 	    char buff[256];
// 	// 	    strcpy(buff, "/");
// 	// 	    scan_files(buff);
// 	// 	}
// 	// 	//#endif
// 	// 	return 1;
// 	// }
// 	// if((USBstatusNew == 1)&&(val == 1)){
// 	// 	#if(FLASH_ENABLED == 1)
// 	//     USBreadBmp();
// 	// 	#endif
// 	// }
// 	return 0;
// }
// void getDirs(void){
// // 	char 			pcBuffer[ 100 ];
// // 	unsigned char 	ucReturnValue;
// // 	ucReturnValue = f_getcwd(pcBuffer,  sizeof(pcBuffer));
// //     if( ucReturnValue != FR_OK )
// //     {
// //         /* The name of the current working directory was not copied into
// //         pcBuffer.  ucReturnValue holds the error code. */
// //     }
// //     else
// //     {
// //         /* The name of the current working directory was copied into
// //         pcBuffer. */
// // //		sprintf(uintToHex,"CURRENT DIR: %d  \r\n",ucReturnValue);
// // //		USART_SendToCom(&uintToHex);
// //     }
// }


// FRESULT scan_files (char* path){        /* Start node to be scanned (***also used as work area***) */
//     FRESULT res;
//     DIR dir;
//     UINT i;
//     static FILINFO fno;

//     sprintf(uintToHex,"====================== \r\n");
// 	// USART_SendToCom(&uintToHex);

//     res = f_opendir(&dir, path);                       /* Open the directory */
//     if (res == FR_OK) {
//         for (;;) {
//             res = f_readdir(&dir, &fno);                   /* Read a directory item */
//             if (res != FR_OK || fno.fname[0] == 0) break;  /* Break on error or end of dir */
//             if (fno.fattrib & AM_DIR) {                    /* It is a directory */
//                 i = strlen(path);
//                 sprintf(uintToHex,&path[i], "/%s \r\n", fno.fname);
//                 sprintf(&path[i], "/%s", (char*)fno.fname);
//         		// USART_SendToCom(&uintToHex);
//                 res = scan_files(path);                    /* Enter the directory */
//                 if (res != FR_OK) break;
//                 path[i] = 0;
//             } else {                                       /* It is a file. */
//             	// sprintf(uintToHex,"%s/%s \r\n", path, fno.fname);
//         		// USART_SendToCom(&uintToHex);
//             }
//         }
//         f_closedir(&dir);
//     }

//     return res;
// }
// void USBreadBmp(void){
// 	//                            /* File read buffer */
// 	// if(f_open(&FileFromSD, "testimg.bmp", FA_READ) != FR_OK){
// 	// 	_Error_Handler(__FILE__, __LINE__);
// 	// }else{
// 	// 	#if(FLASH_ENABLED == 1)
// 	// 	int n = FileFromSD.obj.objsize;
// 	// 	//sprintf(n, "%d", FileFromSD.obj.objsize);
// 	// 	// = uintToHex;			/* filesize */
// 	// 	if(n<=8388603){///*‭8388607, but 1 byte for size of sizelength, 3 bytes for sizelength at maximum*////

// 	// 		uint32_t addr = 0x000000;
// 	// 		uint8_t readBuff[256];
// 	// 		int firstSize = getFirstSize(readBuff,n);
// 	// 		read256fromUSB(readBuff,n,firstSize,0);

// 	// 		W25qxx_EraseChip();
// 	// 		W25qxx_WritePage(readBuff,0x000000,2,10);

// 	// 		uint8_t bufS1;
// 	// 		W25qxx_ReadPage(&bufS1,0x000000,5,10);
// 	// 		sprintf(uintToHex,"+++bytes: %h \r\n",bufS1);
// 	// 		// USART_SendToCom(&uintToHex);

// 	// 		int counter = 0;
// 	// 		for(int t=firstSize;t<n;t=t+256){
// 	// 			if((n-t)>=256){
// 	// 				read256fromUSB(readBuff,n,256,t);
// 	// 			}else{
// 	// 				read256fromUSB(readBuff,n,n-t,t);
// 	// 			}
// 	// 			write256(readBuff,addr);

// 	// 			counter++;
// 	// 		}
//     //     	sprintf(uintToHex,"i counted: %d pages \r\n",counter);
//     // 		// USART_SendToCom(&uintToHex);





// 	// 	}else{
//     //     	sprintf(uintToHex,"TOO BIG \r\n");
//     // 		// USART_SendToCom(&uintToHex);
// 	// 	}
// 	// 	#endif
// 	// }

// }
int getFirstSize(uint8_t *buf,int n){
	uint8_t b_counter = 0;
	uint8_t b0 = (n & 0xff0000) >> 16;
	uint8_t b1 = (n & 0x00ff00) >> 8;
	uint8_t b2 = (n & 0x0000ff);
	if(b0 != 0){ b_counter++; }
	if(b1 != 0){ b_counter++; }
	if(b2 != 0){ b_counter++; }
	int size = 256-b_counter-1;

	int i = 0;
	buf[i] = b_counter; i++;
	if(b0 != 0){ buf[i] = b0;i++; }
	if(b1 != 0){ buf[i] = b1;i++; }
	if(b2 != 0){ buf[i] = b2;i++; }
	return size;
}
// uint8_t read256fromUSB(uint8_t *buf,int n, int readSize, int offset){
// 	FRESULT res;                                          /* FatFs function common result code */
// 	uint32_t byteswritten, bytesread256;                     /* File write/read counts */

// 	int startingByte = 256-readSize;

// 	uint8_t readData[readSize];

// 	f_lseek(&FileFromSD,0);
// 	res = f_read(&FileFromSD, readData, sizeof(readSize), (void *)&bytesread256);

// 	if((bytesread256 == 0) || (res != FR_OK)){
// 		_Error_Handler(__FILE__, __LINE__);
// 	}else{
// 		for(int t=0;t<readSize;t++){
// 			buf[startingByte] = readData[t];startingByte++;
// 		}
// 		return buf;

// 		//f_close(&FileFromSD);
// 	}
// 	return 0;
// }


// uint32_t OpenBMP(FIL FileFromSD,uint8_t *ptr, const char* fname)
// {
//   uint32_t ind = 0, sz = 0, i1 = 0, ind1 = 0;
//   static uint32_t bmp_addr;
//   if(f_open(&FileFromSD, fname, FA_READ) != FR_OK)
//   {
//     TFT_FillScreen(0xFF00FF00,0); //в случае неудачи окрасим экран в красный цвет
//   }
//   else
//   {
//     if (f_read (&FileFromSD, sect, 30, (UINT *)bytesread) != FR_OK)
//     {
//       _Error_Handler(__FILE__, __LINE__);
//     }
//     else
//     {
//       bmp_addr = (uint32_t)sect;
//       sz = *(uint16_t *) (bmp_addr + 2);
//       sz |= (*(uint16_t *) (bmp_addr + 4)) << 16;
//       /* Get bitmap data address offset */
//       ind = *(uint16_t *) (bmp_addr + 10);
//       ind |= (*(uint16_t *) (bmp_addr + 12)) << 16;
//       f_close (&FileFromSD);
//       f_open (&FileFromSD, fname, FA_READ);
//       ind=0;
//       do
//       {
//         if (sz < 4096)
//         {
//           i1 = sz;
//         }
//         else
//         {
//           i1 = 4096;
//         }
//         sz -= i1;
//         f_lseek(&FileFromSD,ind1);
//         f_read (&FileFromSD, sect, i1, (UINT *)&bytesread);
//         // memcpy((void*)(bmp1+ind1), (void*)sect, i1);
//         ind1+=i1;
//       }
//       while (sz > 0);
//       f_close (&FileFromSD);
//     }
//     ind1=0;
//   }
//   return 0;
// }

// void loadImageToLCD(void){
// 	// OpenBMP(FileFromSD,(uint8_t *)bmp1,"copy_calibration_image_800x600.bmp");
// 	// TFT_DrawBitmap(0,0,(uint8_t *)bmp1,MAIN_LAYER);
// }
