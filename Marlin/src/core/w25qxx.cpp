
#include "w25qxx.h"
#include "w25commands.h"
#include "w25qxxConf.h"
// #include "SPI.h"

#include "../lcd/dogm/icons/all_pics_w.h"

#if (_W25QXX_DEBUG==1)
#include <stdio.h>
#endif

w25qxx_t	w25qxx;
char uintToHex[50];
extern LTDC_HandleTypeDef hltdc;
extern int FL_sel;
// extern USART_SendToCom(char (*s)[]);


#if (_W25QXX_USE_FREERTOS==1)
#define	W25qxx_Delay(delay)		osDelay(delay)
//#include "cmsis_os.h"
#else
#define	W25qxx_Delay(delay)		delay_us(delay*1000)
#endif
//###################################################################################################################
uint8_t	W25qxx_Spi(uint8_t	Data)
{
	uint8_t	ret;
	SPI.transfer(FL_sel==1?PA11:PA12, &Data, &ret, 1, SPI_CONTINUE);
	// ret = transfer(uint8_t pin, uint8_t _data, SPITransferMode _mode = SPI_LAST);
	// HAL_SPI_TransmitReceive(&_W25QXX_SPI,&Data,&ret,1,100);
	return ret;	
}
//###################################################################################################################
uint32_t W25qxx_ReadID(void)
{
  uint32_t Temp = 0, Temp0 = 0, Temp1 = 0, Temp2 = 0;
//   //HAL_GPIO_WritePin(_W25QXX_CS_GPIO,FL_sel==1?_W25QXX_CS_PIN1:_W25QXX_CS_PIN2,GPIO_PIN_RESET);
  W25qxx_Spi(JEDEC_ID);
  Temp0 = W25qxx_Spi(W25QXX_DUMMY_BYTE);
  Temp1 = W25qxx_Spi(W25QXX_DUMMY_BYTE);
  Temp2 = W25qxx_Spi(W25QXX_DUMMY_BYTE);
  HAL_GPIO_WritePin(_W25QXX_CS_GPIO,FL_sel==1?_W25QXX_CS_PIN1:_W25QXX_CS_PIN2,GPIO_PIN_SET);
  Temp = (Temp0 << 16) | (Temp1 << 8) | Temp2;
  return Temp;
}
//###################################################################################################################
void W25qxx_ReadUniqID(void)
{
  //HAL_GPIO_WritePin(_W25QXX_CS_GPIO,FL_sel==1?_W25QXX_CS_PIN1:_W25QXX_CS_PIN2,GPIO_PIN_RESET);
  W25qxx_Spi(UNIQUE_ID);
	for(uint8_t	i=0;i<4;i++)
		W25qxx_Spi(W25QXX_DUMMY_BYTE);
	for(uint8_t	i=0;i<8;i++)
		w25qxx.UniqID[i] = W25qxx_Spi(W25QXX_DUMMY_BYTE);
  HAL_GPIO_WritePin(_W25QXX_CS_GPIO,FL_sel==1?_W25QXX_CS_PIN1:_W25QXX_CS_PIN2,GPIO_PIN_SET);
}
//###################################################################################################################
void W25qxx_WriteEnable(void)
{
  //HAL_GPIO_WritePin(_W25QXX_CS_GPIO,FL_sel==1?_W25QXX_CS_PIN1:_W25QXX_CS_PIN2,GPIO_PIN_RESET);
  W25qxx_Spi(WRITE_ENABLE);
  HAL_GPIO_WritePin(_W25QXX_CS_GPIO,FL_sel==1?_W25QXX_CS_PIN1:_W25QXX_CS_PIN2,GPIO_PIN_SET);
	W25qxx_Delay(1);
}
//###################################################################################################################
void W25qxx_WriteDisable(void)
{
  //HAL_GPIO_WritePin(_W25QXX_CS_GPIO,FL_sel==1?_W25QXX_CS_PIN1:_W25QXX_CS_PIN2,GPIO_PIN_RESET);
  W25qxx_Spi(WRITE_DISABLE);
  HAL_GPIO_WritePin(_W25QXX_CS_GPIO,FL_sel==1?_W25QXX_CS_PIN1:_W25QXX_CS_PIN2,GPIO_PIN_SET);
	W25qxx_Delay(1);
}
//###################################################################################################################
uint8_t W25qxx_ReadStatusRegister(uint8_t	SelectStatusRegister_1_2_3)
{
	uint8_t	status=0;
  //HAL_GPIO_WritePin(_W25QXX_CS_GPIO,FL_sel==1?_W25QXX_CS_PIN1:_W25QXX_CS_PIN2,GPIO_PIN_RESET);
	if(SelectStatusRegister_1_2_3==1)
	{
		W25qxx_Spi(READ_STATUS_REG1);
		status=W25qxx_Spi(W25QXX_DUMMY_BYTE);	
		w25qxx.StatusRegister1 = status;
	}
	else if(SelectStatusRegister_1_2_3==2)
	{
		W25qxx_Spi(READ_STATUS_REG2);
		status=W25qxx_Spi(W25QXX_DUMMY_BYTE);	
		w25qxx.StatusRegister2 = status;
	}
	else
	{
		W25qxx_Spi(0x15);
		status=W25qxx_Spi(W25QXX_DUMMY_BYTE);	
		w25qxx.StatusRegister3 = status;
	}	
  HAL_GPIO_WritePin(_W25QXX_CS_GPIO,FL_sel==1?_W25QXX_CS_PIN1:_W25QXX_CS_PIN2,GPIO_PIN_SET);
	return status;
}
//###################################################################################################################
void W25qxx_WriteStatusRegister(uint8_t	SelectStatusRegister_1_2_3,uint8_t Data)
{
  //HAL_GPIO_WritePin(_W25QXX_CS_GPIO,FL_sel==1?_W25QXX_CS_PIN1:_W25QXX_CS_PIN2,GPIO_PIN_RESET);
	if(SelectStatusRegister_1_2_3==1)
	{
		W25qxx_Spi(WRITE_STATUS_REG);
		w25qxx.StatusRegister1 = Data;
	}
	else if(SelectStatusRegister_1_2_3==2)
	{
		W25qxx_Spi(0x31);
		w25qxx.StatusRegister2 = Data;
	}
	else
	{
		W25qxx_Spi(0x11);
		w25qxx.StatusRegister3 = Data;
	}
	W25qxx_Spi(Data);
  HAL_GPIO_WritePin(_W25QXX_CS_GPIO,FL_sel==1?_W25QXX_CS_PIN1:_W25QXX_CS_PIN2,GPIO_PIN_SET);
}
//###################################################################################################################
void W25qxx_WaitForWriteEnd(void)
{
	W25qxx_Delay(2);
	//HAL_GPIO_WritePin(_W25QXX_CS_GPIO,FL_sel==1?_W25QXX_CS_PIN1:_W25QXX_CS_PIN2,GPIO_PIN_RESET);
	W25qxx_Spi(READ_STATUS_REG1);
  do
  {
    w25qxx.StatusRegister1 = W25qxx_Spi(W25QXX_DUMMY_BYTE);
		W25qxx_Delay(1);
  }
  while ((w25qxx.StatusRegister1 & WRITE_STATUS_REG) == WRITE_STATUS_REG);
 HAL_GPIO_WritePin(_W25QXX_CS_GPIO,FL_sel==1?_W25QXX_CS_PIN1:_W25QXX_CS_PIN2,GPIO_PIN_SET);
}
//###################################################################################################################
bool	W25qxx_Init(void)
{
	w25qxx.Lock=1;	
	while(HAL_GetTick()<100)
		W25qxx_Delay(1);
  HAL_GPIO_WritePin(_W25QXX_CS_GPIO,FL_sel==1?_W25QXX_CS_PIN1:_W25QXX_CS_PIN2,GPIO_PIN_SET);
  W25qxx_Delay(100);
	uint32_t	id;
	#if (_W25QXX_DEBUG==1)
	sprintf(uintToHex,"w25qxx Init Begin...\r\n");
	USART_SendToCom(&uintToHex);
	#endif
	id=W25qxx_ReadID();
	
	#if (_W25QXX_DEBUG==1)
	sprintf(uintToHex,"w25qxx ID:0x%X\r\n",id);
	USART_SendToCom(&uintToHex);
	#endif
	switch(id&0x0000FFFF)
	{
		case 0x401A:	// 	w25q512
			w25qxx.ID=W25Q512;
			w25qxx.BlockCount=1024;
			#if (_W25QXX_DEBUG==1)
			sprintf(uintToHex,"w25qxx Chip: w25q512\r\n");
			USART_SendToCom(&uintToHex);
			#endif
			return false;
		break;
		case 0x4019:	// 	w25q256
			w25qxx.ID=W25Q256;
			w25qxx.BlockCount=512;
			#if (_W25QXX_DEBUG==1)
			sprintf(uintToHex,"w25qxx Chip: w25q256\r\n");
			USART_SendToCom(&uintToHex);
			#endif
			return false;
		break;
		case 0x4018:	// 	w25q128
			w25qxx.ID=W25Q128;
			w25qxx.BlockCount=256;
			#if (_W25QXX_DEBUG==1)
			sprintf(uintToHex,"w25qxx Chip: w25q128\r\n");
			USART_SendToCom(&uintToHex);
			#endif
			return false;
		break;
		case 0x4017:	//	w25q64
			w25qxx.ID=W25Q64;
			w25qxx.BlockCount=128;
			#if (_W25QXX_DEBUG==1)
			sprintf(uintToHex,"w25qxx Chip: w25q64\r\n");
			USART_SendToCom(&uintToHex);
			#endif
		break;
		case 0x4016:	//	w25q32
			w25qxx.ID=W25Q32;
			w25qxx.BlockCount=64;
			#if (_W25QXX_DEBUG==1)
			sprintf(uintToHex,"w25qxx Chip: w25q32\r\n");
			USART_SendToCom(&uintToHex);
			#endif
			return false;
		break;
		case 0x4015:	//	w25q16
			w25qxx.ID=W25Q16;
			w25qxx.BlockCount=32;
			#if (_W25QXX_DEBUG==1)
			sprintf(uintToHex,"w25qxx Chip: w25q16\r\n");
			USART_SendToCom(&uintToHex);
			#endif
			return false;
		break;
		case 0x4014:	//	w25q80
			w25qxx.ID=W25Q80;
			w25qxx.BlockCount=16;
			#if (_W25QXX_DEBUG==1)
			sprintf(uintToHex,"w25qxx Chip: w25q80\r\n");
			USART_SendToCom(&uintToHex);
			#endif
			return false;
		break;
		case 0x4013:	//	w25q40
			w25qxx.ID=W25Q40;
			w25qxx.BlockCount=8;
			#if (_W25QXX_DEBUG==1)
			sprintf(uintToHex,"w25qxx Chip: w25q40\r\n");
			USART_SendToCom(&uintToHex);
			#endif
			return false;
		break;
		case 0x4012:	//	w25q20
			w25qxx.ID=W25Q20;
			w25qxx.BlockCount=4;
			#if (_W25QXX_DEBUG==1)
			sprintf(uintToHex,"w25qxx Chip: w25q20\r\n");
			USART_SendToCom(&uintToHex);
			#endif
			return false;
		break;
		case 0x4011:	//	w25q10
			w25qxx.ID=W25Q10;
			w25qxx.BlockCount=2;
			#if (_W25QXX_DEBUG==1)
			sprintf(uintToHex,"w25qxx Chip: w25q10\r\n");
			USART_SendToCom(&uintToHex);
			#endif
			return false;
		break;
		default:
				#if (_W25QXX_DEBUG==1)
				sprintf(uintToHex,"w25qxx Unknown ID\r\n");
				USART_SendToCom(&uintToHex);
				#endif
			w25qxx.Lock=0;	
			// kill(GET_TEXT(MSG_NOT_ENOUGH_LAYERS));
			return false;
				
	}		
	w25qxx.PageSize=256;
	w25qxx.SectorSize=0x1000;
	w25qxx.SectorCount=w25qxx.BlockCount*16;
	w25qxx.PageCount=(w25qxx.SectorCount*w25qxx.SectorSize)/w25qxx.PageSize;
	w25qxx.BlockSize=w25qxx.SectorSize*16;
	w25qxx.CapacityInKiloByte=(w25qxx.SectorCount*w25qxx.SectorSize)/1024;
	W25qxx_ReadUniqID();
	W25qxx_ReadStatusRegister(1);
	W25qxx_ReadStatusRegister(2);
	W25qxx_ReadStatusRegister(3);
	#if (_W25QXX_DEBUG==1)
	sprintf(uintToHex,"w25qxx Page Size: %d Bytes\r\n",w25qxx.PageSize);
	USART_SendToCom(&uintToHex);
	sprintf(uintToHex,"w25qxx Page Count: %d\r\n",w25qxx.PageCount);
	USART_SendToCom(&uintToHex);
	sprintf(uintToHex,"w25qxx Sector Size: %d Bytes\r\n",w25qxx.SectorSize);
	USART_SendToCom(&uintToHex);
	sprintf(uintToHex,"w25qxx Sector Count: %d\r\n",w25qxx.SectorCount);
	USART_SendToCom(&uintToHex);
	sprintf(uintToHex,"w25qxx Block Size: %d Bytes\r\n",w25qxx.BlockSize);
	USART_SendToCom(&uintToHex);
	sprintf(uintToHex,"w25qxx Block Count: %d\r\n",w25qxx.BlockCount);
	USART_SendToCom(&uintToHex);
	sprintf(uintToHex,"w25qxx Capacity: %d KiloBytes\r\n",w25qxx.CapacityInKiloByte);
	USART_SendToCom(&uintToHex);
	sprintf(uintToHex,"w25qxx Init Done\r\n");
	USART_SendToCom(&uintToHex);
	#endif
	w25qxx.Lock=0;	
	return true;
}	
//###################################################################################################################
void	W25qxx_EraseChip(void)
{
	while(w25qxx.Lock==1)
		W25qxx_Delay(1);
	w25qxx.Lock=1;	
	#if (_W25QXX_DEBUG==1)
	uint32_t	StartTime=HAL_GetTick();	
	sprintf(uintToHex,"w25qxx EraseChip Begin...\r\n");
	USART_SendToCom(&uintToHex);
	#endif
	W25qxx_WriteEnable();
	//HAL_GPIO_WritePin(_W25QXX_CS_GPIO,FL_sel==1?_W25QXX_CS_PIN1:_W25QXX_CS_PIN2,GPIO_PIN_RESET);
  W25qxx_Spi(CHIP_ERASE);
  HAL_GPIO_WritePin(_W25QXX_CS_GPIO,FL_sel==1?_W25QXX_CS_PIN1:_W25QXX_CS_PIN2,GPIO_PIN_SET);
	W25qxx_WaitForWriteEnd();
	#if (_W25QXX_DEBUG==1)
	sprintf(uintToHex,"w25qxx EraseBlock done after %d ms!\r\n",HAL_GetTick()-StartTime);
	USART_SendToCom(&uintToHex);
	#endif
	W25qxx_Delay(10);
	w25qxx.Lock=0;	
}
//###################################################################################################################
void W25qxx_EraseSector(uint32_t SectorAddr)
{
	while(w25qxx.Lock==1)
		W25qxx_Delay(1);
	w25qxx.Lock=1;	
	#if (_W25QXX_DEBUG==1)
	uint32_t	StartTime=HAL_GetTick();	
	sprintf(uintToHex,"w25qxx EraseSector %d Begin...\r\n",SectorAddr);
	USART_SendToCom(&uintToHex);
	#endif
	W25qxx_WaitForWriteEnd();
	SectorAddr = SectorAddr * w25qxx.SectorSize;
  W25qxx_WriteEnable();
  //HAL_GPIO_WritePin(_W25QXX_CS_GPIO,FL_sel==1?_W25QXX_CS_PIN1:_W25QXX_CS_PIN2,GPIO_PIN_RESET);
  W25qxx_Spi(SECTOR_ERASE);
	if(w25qxx.ID>=W25Q256)
		W25qxx_Spi((SectorAddr & 0xFF000000) >> 24);
  W25qxx_Spi((SectorAddr & 0xFF0000) >> 16);
  W25qxx_Spi((SectorAddr & 0xFF00) >> 8);
  W25qxx_Spi(SectorAddr & 0xFF);
	HAL_GPIO_WritePin(_W25QXX_CS_GPIO,FL_sel==1?_W25QXX_CS_PIN1:_W25QXX_CS_PIN2,GPIO_PIN_SET);
  W25qxx_WaitForWriteEnd();
	#if (_W25QXX_DEBUG==1)
	sprintf(uintToHex,"w25qxx EraseSector done after %d ms\r\n",HAL_GetTick()-StartTime);
	USART_SendToCom(&uintToHex);
	#endif
	W25qxx_Delay(1);
	w25qxx.Lock=0;
}
//###################################################################################################################
void W25qxx_EraseBlock(uint32_t BlockAddr)
{
	while(w25qxx.Lock==1)
		W25qxx_Delay(1);
	w25qxx.Lock=1;	
	#if (_W25QXX_DEBUG==1)
	sprintf(uintToHex,"w25qxx EraseBlock %d Begin...\r\n",BlockAddr);
	USART_SendToCom(&uintToHex);
	W25qxx_Delay(100);
	uint32_t	StartTime=HAL_GetTick();	
	#endif
	W25qxx_WaitForWriteEnd();
	BlockAddr = BlockAddr * w25qxx.SectorSize*16;
  W25qxx_WriteEnable();
  //HAL_GPIO_WritePin(_W25QXX_CS_GPIO,FL_sel==1?_W25QXX_CS_PIN1:_W25QXX_CS_PIN2,GPIO_PIN_RESET);
  W25qxx_Spi(BLOCK_ERASE64K);
	if(w25qxx.ID>=W25Q256)
		W25qxx_Spi((BlockAddr & 0xFF000000) >> 24);
  W25qxx_Spi((BlockAddr & 0xFF0000) >> 16);
  W25qxx_Spi((BlockAddr & 0xFF00) >> 8);
  W25qxx_Spi(BlockAddr & 0xFF);
	HAL_GPIO_WritePin(_W25QXX_CS_GPIO,FL_sel==1?_W25QXX_CS_PIN1:_W25QXX_CS_PIN2,GPIO_PIN_SET);
  W25qxx_WaitForWriteEnd();
	#if (_W25QXX_DEBUG==1)
	sprintf(uintToHex,"w25qxx EraseBlock done after %d ms\r\n",HAL_GetTick()-StartTime);
	USART_SendToCom(&uintToHex);
	W25qxx_Delay(100);
	#endif
	W25qxx_Delay(1);
	w25qxx.Lock=0;
}
//###################################################################################################################
uint32_t	W25qxx_PageToSector(uint32_t	PageAddress)
{
	return ((PageAddress*w25qxx.PageSize)/w25qxx.SectorSize);
}
//###################################################################################################################
uint32_t	W25qxx_PageToBlock(uint32_t	PageAddress)
{
	return ((PageAddress*w25qxx.PageSize)/w25qxx.BlockSize);
}
//###################################################################################################################
uint32_t	W25qxx_SectorToBlock(uint32_t	SectorAddress)
{
	return ((SectorAddress*w25qxx.SectorSize)/w25qxx.BlockSize);
}
//###################################################################################################################
uint32_t	W25qxx_SectorToPage(uint32_t	SectorAddress)
{
	return (SectorAddress*w25qxx.SectorSize)/w25qxx.PageSize;
}
//###################################################################################################################
uint32_t	W25qxx_BlockToPage(uint32_t	BlockAddress)
{
	return (BlockAddress*w25qxx.BlockSize)/w25qxx.PageSize;
}
//###################################################################################################################
bool 	W25qxx_IsEmptyPage(uint32_t Page_Address,uint32_t OffsetInByte,uint32_t NumByteToCheck_up_to_PageSize)
{
	while(w25qxx.Lock==1)
	W25qxx_Delay(1);
	w25qxx.Lock=1;	
	if(((NumByteToCheck_up_to_PageSize+OffsetInByte)>w25qxx.PageSize)||(NumByteToCheck_up_to_PageSize==0))
		NumByteToCheck_up_to_PageSize=w25qxx.PageSize-OffsetInByte;
	#if (_W25QXX_DEBUG==1)
	sprintf(uintToHex,"w25qxx CheckPage:%d, Offset:%d, Bytes:%d begin...\r\n",Page_Address,OffsetInByte,NumByteToCheck_up_to_PageSize);
	USART_SendToCom(&uintToHex);
	W25qxx_Delay(100);
	uint32_t	StartTime=HAL_GetTick();
	#endif		
	uint8_t	pBuffer[32];
	uint8_t	p_dummy[32] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
	uint32_t	WorkAddress;
	uint32_t	i;
	for(i=OffsetInByte; i<w25qxx.PageSize; i+=sizeof(pBuffer))
	{
		//HAL_GPIO_WritePin(_W25QXX_CS_GPIO,FL_sel==1?_W25QXX_CS_PIN1:_W25QXX_CS_PIN2,GPIO_PIN_RESET);
		WorkAddress=(i+Page_Address*w25qxx.PageSize);
		W25qxx_Spi(FAST_READ);
		if(w25qxx.ID>=W25Q256)
			W25qxx_Spi((WorkAddress & 0xFF000000) >> 24);
		W25qxx_Spi((WorkAddress & 0xFF0000) >> 16);
		W25qxx_Spi((WorkAddress & 0xFF00) >> 8);
		W25qxx_Spi(WorkAddress & 0xFF);
		W25qxx_Spi(0);
		SPI.transfer(FL_sel==1?PA11:PA12, p_dummy, pBuffer, sizeof(pBuffer), SPI_CONTINUE);
		// HAL_SPI_Receive(&_W25QXX_SPI,pBuffer,sizeof(pBuffer),100);	
		HAL_GPIO_WritePin(_W25QXX_CS_GPIO,FL_sel==1?_W25QXX_CS_PIN1:_W25QXX_CS_PIN2,GPIO_PIN_SET);
		for(uint8_t x=0;x<sizeof(pBuffer);x++)
		{
			if(pBuffer[x]!=0xFF)
				goto NOT_EMPTY;		
		}			
	}	
	if((w25qxx.PageSize+OffsetInByte)%sizeof(pBuffer)!=0)
	{
		i-=sizeof(pBuffer);
		for( ; i<w25qxx.PageSize; i++)
		{
			//HAL_GPIO_WritePin(_W25QXX_CS_GPIO,FL_sel==1?_W25QXX_CS_PIN1:_W25QXX_CS_PIN2,GPIO_PIN_RESET);
			WorkAddress=(i+Page_Address*w25qxx.PageSize);
			W25qxx_Spi(FAST_READ);
			if(w25qxx.ID>=W25Q256)
				W25qxx_Spi((WorkAddress & 0xFF000000) >> 24);
			W25qxx_Spi((WorkAddress & 0xFF0000) >> 16);
			W25qxx_Spi((WorkAddress & 0xFF00) >> 8);
			W25qxx_Spi(WorkAddress & 0xFF);
			W25qxx_Spi(0);
			SPI.transfer(FL_sel==1?PA11:PA12, p_dummy, pBuffer, 1, SPI_CONTINUE);
			// HAL_SPI_Receive(&_W25QXX_SPI,pBuffer,1,100);	
			HAL_GPIO_WritePin(_W25QXX_CS_GPIO,FL_sel==1?_W25QXX_CS_PIN1:_W25QXX_CS_PIN2,GPIO_PIN_SET);
			if(pBuffer[0]!=0xFF)
				goto NOT_EMPTY;
		}
	}	
	#if (_W25QXX_DEBUG==1)
	sprintf(uintToHex,"w25qxx CheckPage is Empty in %d ms\r\n",HAL_GetTick()-StartTime);
	USART_SendToCom(&uintToHex);
	W25qxx_Delay(100);
	#endif	
	w25qxx.Lock=0;
	return true;	
	NOT_EMPTY:
	#if (_W25QXX_DEBUG==1)
	sprintf(uintToHex,"w25qxx CheckPage is Not Empty in %d ms\r\n",HAL_GetTick()-StartTime);
	USART_SendToCom(&uintToHex);
	W25qxx_Delay(100);
	#endif	
	w25qxx.Lock=0;
	return false;
}
//###################################################################################################################
bool 	W25qxx_IsEmptySector(uint32_t Sector_Address,uint32_t OffsetInByte,uint32_t NumByteToCheck_up_to_SectorSize)
{
	while(w25qxx.Lock==1)
	W25qxx_Delay(1);
	w25qxx.Lock=1;	
	if((NumByteToCheck_up_to_SectorSize>w25qxx.SectorSize)||(NumByteToCheck_up_to_SectorSize==0))
		NumByteToCheck_up_to_SectorSize=w25qxx.SectorSize;
	#if (_W25QXX_DEBUG==1)
	sprintf(uintToHex,"w25qxx CheckSector:%d, Offset:%d, Bytes:%d begin...\r\n",Sector_Address,OffsetInByte,NumByteToCheck_up_to_SectorSize);
	USART_SendToCom(&uintToHex);
	W25qxx_Delay(100);
	uint32_t	StartTime=HAL_GetTick();
	#endif		
	uint8_t	pBuffer[32];
	uint8_t	p_dummy[32] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
	uint32_t	WorkAddress;
	uint32_t	i;
	for(i=OffsetInByte; i<w25qxx.SectorSize; i+=sizeof(pBuffer))
	{
		//HAL_GPIO_WritePin(_W25QXX_CS_GPIO,FL_sel==1?_W25QXX_CS_PIN1:_W25QXX_CS_PIN2,GPIO_PIN_RESET);
		WorkAddress=(i+Sector_Address*w25qxx.SectorSize);
		W25qxx_Spi(FAST_READ);
		if(w25qxx.ID>=W25Q256)
			W25qxx_Spi((WorkAddress & 0xFF000000) >> 24);
		W25qxx_Spi((WorkAddress & 0xFF0000) >> 16);
		W25qxx_Spi((WorkAddress & 0xFF00) >> 8);
		W25qxx_Spi(WorkAddress & 0xFF);
		W25qxx_Spi(0);
		SPI.transfer(FL_sel==1?PA11:PA12, p_dummy, pBuffer, sizeof(pBuffer), SPI_CONTINUE);
		// HAL_SPI_Receive(&_W25QXX_SPI,pBuffer,sizeof(pBuffer),100);	
		HAL_GPIO_WritePin(_W25QXX_CS_GPIO,FL_sel==1?_W25QXX_CS_PIN1:_W25QXX_CS_PIN2,GPIO_PIN_SET);
		for(uint8_t x=0;x<sizeof(pBuffer);x++)
		{
			if(pBuffer[x]!=0xFF)
				goto NOT_EMPTY;		
		}			
	}	
	if((w25qxx.SectorSize+OffsetInByte)%sizeof(pBuffer)!=0)
	{
		i-=sizeof(pBuffer);
		for( ; i<w25qxx.SectorSize; i++)
		{
			//HAL_GPIO_WritePin(_W25QXX_CS_GPIO,FL_sel==1?_W25QXX_CS_PIN1:_W25QXX_CS_PIN2,GPIO_PIN_RESET);
			WorkAddress=(i+Sector_Address*w25qxx.SectorSize);
			W25qxx_Spi(FAST_READ);
			if(w25qxx.ID>=W25Q256)
				W25qxx_Spi((WorkAddress & 0xFF000000) >> 24);
			W25qxx_Spi((WorkAddress & 0xFF0000) >> 16);
			W25qxx_Spi((WorkAddress & 0xFF00) >> 8);
			W25qxx_Spi(WorkAddress & 0xFF);
			W25qxx_Spi(0);
			SPI.transfer(FL_sel==1?PA11:PA12, p_dummy, pBuffer, 1, SPI_CONTINUE);
			// HAL_SPI_Receive(&_W25QXX_SPI,pBuffer,1,100);	
			HAL_GPIO_WritePin(_W25QXX_CS_GPIO,FL_sel==1?_W25QXX_CS_PIN1:_W25QXX_CS_PIN2,GPIO_PIN_SET);
			if(pBuffer[0]!=0xFF)
				goto NOT_EMPTY;
		}
	}	
	#if (_W25QXX_DEBUG==1)
	sprintf(uintToHex,"w25qxx CheckSector is Empty in %d ms\r\n",HAL_GetTick()-StartTime);
	USART_SendToCom(&uintToHex);
	W25qxx_Delay(100);
	#endif	
	w25qxx.Lock=0;
	return true;	
	NOT_EMPTY:
	#if (_W25QXX_DEBUG==1)
	sprintf(uintToHex,"w25qxx CheckSector is Not Empty in %d ms\r\n",HAL_GetTick()-StartTime);
	USART_SendToCom(&uintToHex);
	W25qxx_Delay(100);
	#endif	
	w25qxx.Lock=0;
	return false;
}
//###################################################################################################################
bool 	W25qxx_IsEmptyBlock(uint32_t Block_Address,uint32_t OffsetInByte,uint32_t NumByteToCheck_up_to_BlockSize)
{
	while(w25qxx.Lock==1)
	W25qxx_Delay(1);
	w25qxx.Lock=1;	
	if((NumByteToCheck_up_to_BlockSize>w25qxx.BlockSize)||(NumByteToCheck_up_to_BlockSize==0))
		NumByteToCheck_up_to_BlockSize=w25qxx.BlockSize;
	#if (_W25QXX_DEBUG==1)
	sprintf(uintToHex,"w25qxx CheckBlock:%d, Offset:%d, Bytes:%d begin...\r\n",Block_Address,OffsetInByte,NumByteToCheck_up_to_BlockSize);
	USART_SendToCom(&uintToHex);
	W25qxx_Delay(100);
	uint32_t	StartTime=HAL_GetTick();
	#endif		
	uint8_t	pBuffer[32];
	uint8_t	p_dummy[32] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
	uint32_t	WorkAddress;
	uint32_t	i;
	for(i=OffsetInByte; i<w25qxx.BlockSize; i+=sizeof(pBuffer))
	{
		//HAL_GPIO_WritePin(_W25QXX_CS_GPIO,FL_sel==1?_W25QXX_CS_PIN1:_W25QXX_CS_PIN2,GPIO_PIN_RESET);
		WorkAddress=(i+Block_Address*w25qxx.BlockSize);
		W25qxx_Spi(FAST_READ);
		if(w25qxx.ID>=W25Q256)
			W25qxx_Spi((WorkAddress & 0xFF000000) >> 24);
		W25qxx_Spi((WorkAddress & 0xFF0000) >> 16);
		W25qxx_Spi((WorkAddress & 0xFF00) >> 8);
		W25qxx_Spi(WorkAddress & 0xFF);
		W25qxx_Spi(0);
		SPI.transfer(FL_sel==1?PA11:PA12, p_dummy, pBuffer, sizeof(pBuffer), SPI_CONTINUE);
		// HAL_SPI_Receive(&_W25QXX_SPI,pBuffer,sizeof(pBuffer),100);	
		HAL_GPIO_WritePin(_W25QXX_CS_GPIO,FL_sel==1?_W25QXX_CS_PIN1:_W25QXX_CS_PIN2,GPIO_PIN_SET);
		for(uint8_t x=0;x<sizeof(pBuffer);x++)
		{
			if(pBuffer[x]!=0xFF)
				goto NOT_EMPTY;		
		}			
	}	
	if((w25qxx.BlockSize+OffsetInByte)%sizeof(pBuffer)!=0)
	{
		i-=sizeof(pBuffer);
		for( ; i<w25qxx.BlockSize; i++)
		{
			//HAL_GPIO_WritePin(_W25QXX_CS_GPIO,FL_sel==1?_W25QXX_CS_PIN1:_W25QXX_CS_PIN2,GPIO_PIN_RESET);
			WorkAddress=(i+Block_Address*w25qxx.BlockSize);
			W25qxx_Spi(FAST_READ);
			if(w25qxx.ID>=W25Q256)
				W25qxx_Spi((WorkAddress & 0xFF000000) >> 24);
			W25qxx_Spi((WorkAddress & 0xFF0000) >> 16);
			W25qxx_Spi((WorkAddress & 0xFF00) >> 8);
			W25qxx_Spi(WorkAddress & 0xFF);
			W25qxx_Spi(0);
			SPI.transfer(FL_sel==1?PA11:PA12, p_dummy, pBuffer, 1, SPI_CONTINUE);
			// HAL_SPI_Receive(&_W25QXX_SPI,pBuffer,1,100);	
			HAL_GPIO_WritePin(_W25QXX_CS_GPIO,FL_sel==1?_W25QXX_CS_PIN1:_W25QXX_CS_PIN2,GPIO_PIN_SET);
			if(pBuffer[0]!=0xFF)
				goto NOT_EMPTY;
		}
	}	
	#if (_W25QXX_DEBUG==1)
	sprintf(uintToHex,"w25qxx CheckBlock is Empty in %d ms\r\n",HAL_GetTick()-StartTime);
	USART_SendToCom(&uintToHex);
	W25qxx_Delay(100);
	#endif	
	w25qxx.Lock=0;
	return true;	
	NOT_EMPTY:
	#if (_W25QXX_DEBUG==1)
	sprintf(uintToHex,"w25qxx CheckBlock is Not Empty in %d ms\r\n",HAL_GetTick()-StartTime);
	USART_SendToCom(&uintToHex);
	W25qxx_Delay(100);
	#endif	
	w25qxx.Lock=0;
	return false;
}
//###################################################################################################################
void W25qxx_WriteByte(uint8_t pBuffer, uint32_t WriteAddr_inBytes)
{
	while(w25qxx.Lock==1)
		W25qxx_Delay(1);
	w25qxx.Lock=1;
	#if (_W25QXX_DEBUG==1)
	uint32_t	StartTime=HAL_GetTick();
	sprintf(uintToHex,"w25qxx WriteByte 0x%02X at address %d begin...",pBuffer,WriteAddr_inBytes);
	USART_SendToCom(&uintToHex);
	#endif
	W25qxx_WaitForWriteEnd();
  W25qxx_WriteEnable();
  //HAL_GPIO_WritePin(_W25QXX_CS_GPIO,FL_sel==1?_W25QXX_CS_PIN1:_W25QXX_CS_PIN2,GPIO_PIN_RESET);
  W25qxx_Spi(PAGE_PROGRAM);
	if(w25qxx.ID>=W25Q256)
		W25qxx_Spi((WriteAddr_inBytes & 0xFF000000) >> 24);
  W25qxx_Spi((WriteAddr_inBytes & 0xFF0000) >> 16);
  W25qxx_Spi((WriteAddr_inBytes & 0xFF00) >> 8);
  W25qxx_Spi(WriteAddr_inBytes & 0xFF);
  W25qxx_Spi(pBuffer);
	HAL_GPIO_WritePin(_W25QXX_CS_GPIO,FL_sel==1?_W25QXX_CS_PIN1:_W25QXX_CS_PIN2,GPIO_PIN_SET);
  W25qxx_WaitForWriteEnd();
	#if (_W25QXX_DEBUG==1)
	sprintf(uintToHex,"w25qxx WriteByte done after %d ms\r\n",HAL_GetTick()-StartTime);
	USART_SendToCom(&uintToHex);
	#endif
	w25qxx.Lock=0;
}
//###################################################################################################################
void 	W25qxx_WritePage(uint8_t *pBuffer	,uint32_t Page_Address,uint32_t OffsetInByte,uint32_t NumByteToWrite_up_to_PageSize)
{
	while(w25qxx.Lock==1)
		W25qxx_Delay(1);
	w25qxx.Lock=1;
	if(((NumByteToWrite_up_to_PageSize+OffsetInByte)>w25qxx.PageSize)||(NumByteToWrite_up_to_PageSize==0))
		NumByteToWrite_up_to_PageSize=w25qxx.PageSize-OffsetInByte;
	if((OffsetInByte+NumByteToWrite_up_to_PageSize) > w25qxx.PageSize)
		NumByteToWrite_up_to_PageSize = w25qxx.PageSize-OffsetInByte;
	#if ((_W25QXX_DEBUG==1)&&(_CHIP_MONITOR == 1))
	sprintf(uintToHex,"w25qxx WritePage:%d, Offset:%d ,Writes %d Bytes, begin...\r\n",Page_Address,OffsetInByte,NumByteToWrite_up_to_PageSize);
	USART_SendToCom(&uintToHex);
	W25qxx_Delay(100);
	uint32_t	StartTime=HAL_GetTick();
	#endif	
	W25qxx_WaitForWriteEnd();
  W25qxx_WriteEnable();
  //HAL_GPIO_WritePin(_W25QXX_CS_GPIO,FL_sel==1?_W25QXX_CS_PIN1:_W25QXX_CS_PIN2,GPIO_PIN_RESET);
  W25qxx_Spi(PAGE_PROGRAM);
	Page_Address = (Page_Address*w25qxx.PageSize)+OffsetInByte;	
	if(w25qxx.ID>=W25Q256)
		W25qxx_Spi((Page_Address & 0xFF000000) >> 24);
  W25qxx_Spi((Page_Address & 0xFF0000) >> 16);
  W25qxx_Spi((Page_Address & 0xFF00) >> 8);
  W25qxx_Spi(Page_Address&0xFF);
  	SPI.transfer(FL_sel==1?PA11:PA12, pBuffer, NumByteToWrite_up_to_PageSize, SPI_CONTINUE);
	// HAL_SPI_Transmit(&_W25QXX_SPI,pBuffer,NumByteToWrite_up_to_PageSize,100);	
	HAL_GPIO_WritePin(_W25QXX_CS_GPIO,FL_sel==1?_W25QXX_CS_PIN1:_W25QXX_CS_PIN2,GPIO_PIN_SET);
  W25qxx_WaitForWriteEnd();
	#if ((_W25QXX_DEBUG==1)&&(_CHIP_MONITOR == 1))
	StartTime = HAL_GetTick()-StartTime; 
	for(uint32_t i=0;i<NumByteToWrite_up_to_PageSize ; i++)
	{
		if((i%8==0)&&(i>2))
		{
			sprintf(uintToHex,"\r\n");
			USART_SendToCom(&uintToHex);
			W25qxx_Delay(10);			
		}
		sprintf(uintToHex,"0x%02X,",pBuffer[i]);
		USART_SendToCom(&uintToHex);
	}	
	sprintf(uintToHex,"\r\n");
	USART_SendToCom(&uintToHex);
	sprintf(uintToHex,"w25qxx WritePage done after %d ms\r\n",StartTime);
	USART_SendToCom(&uintToHex);
	W25qxx_Delay(100);
	#endif	
	W25qxx_Delay(1);
	w25qxx.Lock=0;
}
//###################################################################################################################
void 	W25qxx_WriteSector(uint8_t *pBuffer	,uint32_t Sector_Address,uint32_t OffsetInByte	,uint32_t NumByteToWrite_up_to_SectorSize)
{
	if((NumByteToWrite_up_to_SectorSize>w25qxx.SectorSize)||(NumByteToWrite_up_to_SectorSize==0))
		NumByteToWrite_up_to_SectorSize=w25qxx.SectorSize;
	#if (_W25QXX_DEBUG==1)
	sprintf(uintToHex,"+++w25qxx WriteSector:%d, Offset:%d ,Write %d Bytes, begin...\r\n",Sector_Address,OffsetInByte,NumByteToWrite_up_to_SectorSize);
	USART_SendToCom(&uintToHex);
	W25qxx_Delay(100);
	#endif	
	if(OffsetInByte>=w25qxx.SectorSize)
	{
		#if (_W25QXX_DEBUG==1)
		sprintf(uintToHex,"---w25qxx WriteSector Faild!\r\n");
		USART_SendToCom(&uintToHex);
		W25qxx_Delay(100);
		#endif	
		return;
	}	
	uint32_t	StartPage;
	int32_t		BytesToWrite;
	uint32_t	LocalOffset;	
	if((OffsetInByte+NumByteToWrite_up_to_SectorSize) > w25qxx.SectorSize)
		BytesToWrite = w25qxx.SectorSize-OffsetInByte;
	else
		BytesToWrite = NumByteToWrite_up_to_SectorSize;	
	StartPage = W25qxx_SectorToPage(Sector_Address)+(OffsetInByte/w25qxx.PageSize);
	LocalOffset = OffsetInByte%w25qxx.PageSize;	
	do
	{		
		W25qxx_WritePage(pBuffer,StartPage,LocalOffset,BytesToWrite);
		StartPage++;
		BytesToWrite-=w25qxx.PageSize-LocalOffset;
		pBuffer+=w25qxx.PageSize;	
		LocalOffset=0;
	}while(BytesToWrite>0);		
	#if (_W25QXX_DEBUG==1)
	sprintf(uintToHex,"---w25qxx WriteSector Done\r\n");
	USART_SendToCom(&uintToHex);
	W25qxx_Delay(100);
	#endif	
}
//###################################################################################################################
void 	W25qxx_WriteBlock	(uint8_t* pBuffer ,uint32_t Block_Address	,uint32_t OffsetInByte	,uint32_t	NumByteToWrite_up_to_BlockSize)
{
	if((NumByteToWrite_up_to_BlockSize>w25qxx.BlockSize)||(NumByteToWrite_up_to_BlockSize==0))
		NumByteToWrite_up_to_BlockSize=w25qxx.BlockSize;
	#if (_W25QXX_DEBUG==1)
	sprintf(uintToHex,"+++w25qxx WriteBlock:%d, Offset:%d ,Write %d Bytes, begin...\r\n",Block_Address,OffsetInByte,NumByteToWrite_up_to_BlockSize);
	USART_SendToCom(&uintToHex);
	W25qxx_Delay(100);
	#endif	
	if(OffsetInByte>=w25qxx.BlockSize)
	{
		#if (_W25QXX_DEBUG==1)
		sprintf(uintToHex,"---w25qxx WriteBlock Faild!\r\n");
		USART_SendToCom(&uintToHex);
		W25qxx_Delay(100);
		#endif	
		return;
	}	
	uint32_t	StartPage;
	int32_t		BytesToWrite;
	uint32_t	LocalOffset;	
	if((OffsetInByte+NumByteToWrite_up_to_BlockSize) > w25qxx.BlockSize)
		BytesToWrite = w25qxx.BlockSize-OffsetInByte;
	else
		BytesToWrite = NumByteToWrite_up_to_BlockSize;	
	StartPage = W25qxx_BlockToPage(Block_Address)+(OffsetInByte/w25qxx.PageSize);
	LocalOffset = OffsetInByte%w25qxx.PageSize;	
	do
	{		
		W25qxx_WritePage(pBuffer,StartPage,LocalOffset,BytesToWrite);
		StartPage++;
		BytesToWrite-=w25qxx.PageSize-LocalOffset;
		pBuffer+=w25qxx.PageSize;	
		LocalOffset=0;
	}while(BytesToWrite>0);		
	#if (_W25QXX_DEBUG==1)
	sprintf(uintToHex,"---w25qxx WriteBlock Done\r\n");
	USART_SendToCom(&uintToHex);
	W25qxx_Delay(100);
	#endif	
}
//###################################################################################################################
void 	W25qxx_ReadByte(uint8_t *pBuffer,uint32_t Bytes_Address)
{
	while(w25qxx.Lock==1)
		W25qxx_Delay(1);
	w25qxx.Lock=1;
	#if (_W25QXX_DEBUG==1)
	uint32_t	StartTime=HAL_GetTick();
	sprintf(uintToHex,"w25qxx ReadByte at address %d begin...\r\n",Bytes_Address);
	USART_SendToCom(&uintToHex);
	#endif
	//HAL_GPIO_WritePin(_W25QXX_CS_GPIO,FL_sel==1?_W25QXX_CS_PIN1:_W25QXX_CS_PIN2,GPIO_PIN_RESET);
  W25qxx_Spi(FAST_READ);
	if(w25qxx.ID>=W25Q256)
		W25qxx_Spi((Bytes_Address & 0xFF000000) >> 24);
  W25qxx_Spi((Bytes_Address & 0xFF0000) >> 16);
  W25qxx_Spi((Bytes_Address& 0xFF00) >> 8);
  W25qxx_Spi(Bytes_Address & 0xFF);
	W25qxx_Spi(0);
	*pBuffer = W25qxx_Spi(W25QXX_DUMMY_BYTE);
	HAL_GPIO_WritePin(_W25QXX_CS_GPIO,FL_sel==1?_W25QXX_CS_PIN1:_W25QXX_CS_PIN2,GPIO_PIN_SET);
	#if (_W25QXX_DEBUG==1)
	sprintf(uintToHex,"w25qxx ReadByte 0x%02X done after %d ms\r\n",*pBuffer,HAL_GetTick()-StartTime);
	USART_SendToCom(&uintToHex);
	#endif
	w25qxx.Lock=0;
}
//###################################################################################################################
void W25qxx_ReadBytes(uint8_t* pBuffer, uint32_t ReadAddr, const uint32_t NumByteToRead)
{
	while(w25qxx.Lock==1)
		W25qxx_Delay(1);
	w25qxx.Lock=1;
	#if (_W25QXX_DEBUG==1)
	uint32_t	StartTime=HAL_GetTick();
	sprintf(uintToHex,"w25qxx ReadBytes at Address:%d, %d Bytes  begin...\r\n",ReadAddr,NumByteToRead);
	USART_SendToCom(&uintToHex);
	#endif	
	//HAL_GPIO_WritePin(_W25QXX_CS_GPIO,FL_sel==1?_W25QXX_CS_PIN1:_W25QXX_CS_PIN2,GPIO_PIN_RESET);
	W25qxx_Spi(FAST_READ);
	if(w25qxx.ID>=W25Q256)
		W25qxx_Spi((ReadAddr & 0xFF000000) >> 24);
  W25qxx_Spi((ReadAddr & 0xFF0000) >> 16);
  W25qxx_Spi((ReadAddr& 0xFF00) >> 8);
  W25qxx_Spi(ReadAddr & 0xFF);
	W25qxx_Spi(0);
	uint8_t	p_dummy[NumByteToRead];
	for(uint32_t i=0;i<NumByteToRead;i++){
		p_dummy[i] = 0;
	}
	SPI.transfer(FL_sel==1?PA11:PA12, p_dummy, pBuffer, NumByteToRead, SPI_CONTINUE);
	// HAL_SPI_Receive(&_W25QXX_SPI,pBuffer,NumByteToRead,2000);	
	HAL_GPIO_WritePin(_W25QXX_CS_GPIO,FL_sel==1?_W25QXX_CS_PIN1:_W25QXX_CS_PIN2,GPIO_PIN_SET);
	#if (_W25QXX_DEBUG==1)
	StartTime = HAL_GetTick()-StartTime; 
	for(uint32_t i=0;i<NumByteToRead ; i++)
	{
		if((i%8==0)&&(i>2))
		{
			sprintf(uintToHex,"\r\n");
			USART_SendToCom(&uintToHex);
			W25qxx_Delay(10);
		}
		sprintf(uintToHex,"0x%02X,",pBuffer[i]);
		USART_SendToCom(&uintToHex);
	}
	sprintf(uintToHex,"\r\n");
	USART_SendToCom(&uintToHex);
	sprintf(uintToHex,"w25qxx ReadBytes done after %d ms\r\n",StartTime);
	USART_SendToCom(&uintToHex);
	W25qxx_Delay(100);
	#endif	
	W25qxx_Delay(1);
	w25qxx.Lock=0;
}
//###################################################################################################################
void 	W25qxx_ReadPage(uint8_t *pBuffer,uint32_t Page_Address,uint32_t OffsetInByte,uint32_t NumByteToRead_up_to_PageSize)
{
	while(w25qxx.Lock==1)
		W25qxx_Delay(1);
	w25qxx.Lock=1;
	if((NumByteToRead_up_to_PageSize>w25qxx.PageSize)||(NumByteToRead_up_to_PageSize==0))
		NumByteToRead_up_to_PageSize=w25qxx.PageSize;
	if((OffsetInByte+NumByteToRead_up_to_PageSize) > w25qxx.PageSize)
		NumByteToRead_up_to_PageSize = w25qxx.PageSize-OffsetInByte;
	#if (_W25QXX_DEBUG==1)
	sprintf(uintToHex,"w25qxx ReadPage:%d, Offset:%d ,Read %d Bytes, begin...\r\n",Page_Address,OffsetInByte,NumByteToRead_up_to_PageSize);
	USART_SendToCom(&uintToHex);
	W25qxx_Delay(100);
	uint32_t	StartTime=HAL_GetTick();
	#endif	
	Page_Address = Page_Address*w25qxx.PageSize+OffsetInByte;
	//HAL_GPIO_WritePin(_W25QXX_CS_GPIO,FL_sel==1?_W25QXX_CS_PIN1:_W25QXX_CS_PIN2,GPIO_PIN_RESET);
	W25qxx_Spi(FAST_READ);
	if(w25qxx.ID>=W25Q256)
		W25qxx_Spi((Page_Address & 0xFF000000) >> 24);
  W25qxx_Spi((Page_Address & 0xFF0000) >> 16);
  W25qxx_Spi((Page_Address& 0xFF00) >> 8);
  W25qxx_Spi(Page_Address & 0xFF);
	W25qxx_Spi(0);
	const uint32_t dummy_s = NumByteToRead_up_to_PageSize;
	uint8_t	p_dummy[dummy_s];
	for(uint32_t i=0;i<dummy_s;i++){
		p_dummy[i] = 0;
	}
	SPI.transfer(FL_sel==1?PA11:PA12, p_dummy, pBuffer, NumByteToRead_up_to_PageSize, SPI_CONTINUE);
	// HAL_SPI_Receive(&_W25QXX_SPI,pBuffer,NumByteToRead_up_to_PageSize,100);	
	HAL_GPIO_WritePin(_W25QXX_CS_GPIO,FL_sel==1?_W25QXX_CS_PIN1:_W25QXX_CS_PIN2,GPIO_PIN_SET);
	#if (_W25QXX_DEBUG==1)
	StartTime = HAL_GetTick()-StartTime; 
	for(uint32_t i=0;i<NumByteToRead_up_to_PageSize ; i++)
	{
		if((i%8==0)&&(i>2))
		{
			sprintf(uintToHex,"\r\n");
			USART_SendToCom(&uintToHex);
			W25qxx_Delay(10);
		}
		sprintf(uintToHex,"0x%02X,",pBuffer[i]);
		USART_SendToCom(&uintToHex);
	}	
	sprintf(uintToHex,"\r\n");
	USART_SendToCom(&uintToHex);
	sprintf(uintToHex,"w25qxx ReadPage done after %d ms\r\n",StartTime);
	USART_SendToCom(&uintToHex);
	W25qxx_Delay(100);
	#endif	
	W25qxx_Delay(1);
	w25qxx.Lock=0;
}
//###################################################################################################################
void 	W25qxx_ReadSector(uint8_t *pBuffer,uint32_t Sector_Address,uint32_t OffsetInByte,uint32_t NumByteToRead_up_to_SectorSize)
{	
	if((NumByteToRead_up_to_SectorSize>w25qxx.SectorSize)||(NumByteToRead_up_to_SectorSize==0))
		NumByteToRead_up_to_SectorSize=w25qxx.SectorSize;
	#if (_W25QXX_DEBUG==1)
	sprintf(uintToHex,"+++w25qxx ReadSector:%d, Offset:%d ,Read %d Bytes, begin...\r\n",Sector_Address,OffsetInByte,NumByteToRead_up_to_SectorSize);
	USART_SendToCom(&uintToHex);
	W25qxx_Delay(100);
	#endif	
	if(OffsetInByte>=w25qxx.SectorSize)
	{
		#if (_W25QXX_DEBUG==1)
		sprintf(uintToHex,"---w25qxx ReadSector Faild!\r\n");
		USART_SendToCom(&uintToHex);
		W25qxx_Delay(100);
		#endif	
		return;
	}	
	uint32_t	StartPage;
	int32_t		BytesToRead;
	uint32_t	LocalOffset;	
	if((OffsetInByte+NumByteToRead_up_to_SectorSize) > w25qxx.SectorSize)
		BytesToRead = w25qxx.SectorSize-OffsetInByte;
	else
		BytesToRead = NumByteToRead_up_to_SectorSize;	
	StartPage = W25qxx_SectorToPage(Sector_Address)+(OffsetInByte/w25qxx.PageSize);
	LocalOffset = OffsetInByte%w25qxx.PageSize;	
	do
	{		
		W25qxx_ReadPage(pBuffer,StartPage,LocalOffset,BytesToRead);
		StartPage++;
		BytesToRead-=w25qxx.PageSize-LocalOffset;
		pBuffer+=w25qxx.PageSize;	
		LocalOffset=0;
	}while(BytesToRead>0);		
	#if (_W25QXX_DEBUG==1)
	sprintf(uintToHex,"---w25qxx ReadSector Done\r\n");
	USART_SendToCom(&uintToHex);
	W25qxx_Delay(100);
	#endif	
}
//###################################################################################################################
void 	W25qxx_ReadBlock(uint8_t* pBuffer,uint32_t Block_Address,uint32_t OffsetInByte,uint32_t	NumByteToRead_up_to_BlockSize)
{
	if((NumByteToRead_up_to_BlockSize>w25qxx.BlockSize)||(NumByteToRead_up_to_BlockSize==0))
		NumByteToRead_up_to_BlockSize=w25qxx.BlockSize;
	#if (_W25QXX_DEBUG==1)
	sprintf(uintToHex,"+++w25qxx ReadBlock:%d, Offset:%d ,Read %d Bytes, begin...\r\n",Block_Address,OffsetInByte,NumByteToRead_up_to_BlockSize);
	USART_SendToCom(&uintToHex);
	W25qxx_Delay(100);
	#endif	
	if(OffsetInByte>=w25qxx.BlockSize)
	{
		#if (_W25QXX_DEBUG==1)
		sprintf(uintToHex,"w25qxx ReadBlock Faild!\r\n");
		USART_SendToCom(&uintToHex);
		W25qxx_Delay(100);
		#endif	
		return;
	}	
	uint32_t	StartPage;
	int32_t		BytesToRead;
	uint32_t	LocalOffset;	
	if((OffsetInByte+NumByteToRead_up_to_BlockSize) > w25qxx.BlockSize)
		BytesToRead = w25qxx.BlockSize-OffsetInByte;
	else
		BytesToRead = NumByteToRead_up_to_BlockSize;	
	StartPage = W25qxx_BlockToPage(Block_Address)+(OffsetInByte/w25qxx.PageSize);
	LocalOffset = OffsetInByte%w25qxx.PageSize;	
	do
	{		
		W25qxx_ReadPage(pBuffer,StartPage,LocalOffset,BytesToRead);
		StartPage++;
		BytesToRead-=w25qxx.PageSize-LocalOffset;
		pBuffer+=w25qxx.PageSize;	
		LocalOffset=0;
	}while(BytesToRead>0);		
	#if (_W25QXX_DEBUG==1)
	sprintf(uintToHex,"---w25qxx ReadBlock Done\r\n");
	USART_SendToCom(&uintToHex);
	W25qxx_Delay(100);
	#endif	
}
//###################################################################################################################

// uint8_t imageToData(uint8_t *buf, int i, uint32_t length, uint8_t (*img)[]){
// 	int completeLength = length+i;
// 	int completePercent = 0;
// 	int onePercent = length/100;
// 	int m = 1;
// 	for(int n=i;n<completeLength;n++){
// 		buf[n] = (*img)[n];
// 		if((n/100) >= (onePercent*m)){
// 			sprintf(uintToHex,"---w25qxx Ready: %d \r\n",m);
// 			USART_SendToCom(&uintToHex);
// 			m++;
// 		}
// 	}
// 	return buf;
// }

void sendPageToMem(void){

}
void read256fromFile(void){

}

void write256(uint8_t *buf, uint32_t addr){

}


void FlashReadFile(uint8_t layerID){

	uint8_t flsizbuf[10];
	W25qxx_ReadBytes(flsizbuf,0,10);

	// #define BMPFILE true
	#ifdef BMPFILE
		uint32_t b = 0;
		if(flsizbuf[0] == 3) b = (flsizbuf[1] << 16) + (flsizbuf[2] << 8) + (flsizbuf[3]);
		if(flsizbuf[0] == 2) b = (flsizbuf[1] << 8)  + (flsizbuf[2]);
		if(flsizbuf[0] == 1) b = (flsizbuf[1]);


		int imgDataStart = b-(800*600*3);

		uint16_t i,j;
		uint32_t k = 0;
	//	uint8_t testVar = 0;
	//	uint32_t endAdr = 800*600*4;
	//	uint32_t n = hltdc.LayerCfg[0].ImageHeight*hltdc.LayerCfg[0].ImageWidth;
		for(j=hltdc.LayerCfg[layerID].ImageHeight;j>0;j--){
			uint8_t firstData[2400];
	//		if(j==249){
	//			HAL_GPIO_WritePin(BUZZER_GPIO_Port,BUZZER_Pin,GPIO_PIN_RESET);
	//			HAL_GPIO_WritePin(BUZZER_GPIO_Port,BUZZER_Pin,GPIO_PIN_SET);
	//		}
			W25qxx_ReadBytes(firstData,imgDataStart+(flsizbuf[0]+1)+k*3,800*3);
			for(i=0;i<hltdc.LayerCfg[layerID].ImageWidth;i++){
				*(uint32_t*) (hltdc.LayerCfg[layerID].FBStartAdress + ((j-1)*hltdc.LayerCfg[layerID].ImageWidth + i)*4)  = (uint32_t)(0xFF000000 | (firstData[i*3+2] << 16) | (firstData[i*3+1] << 8) | (firstData[i*3]));
				k++;
			}
		}
	#else
		// uint16_t i,j;
		uint32_t k = 0;
		uint32_t allpicsize = sizeof(all_pic);
		for(k=0;k<allpicsize;k++){
			*(uint8_t*) (hltdc.LayerCfg[layerID].FBStartAdress +k) = (uint8_t)(all_pic[k]);
		}
		// for(j=0;j<(allpicsize/4/800 +1);j++){
		// 	uint8_t firstData[3200];
		// 	W25qxx_ReadBytes(firstData,(flsizbuf[0]+1)+k*4,4*800);
		// 	for(i=0;i<hltdc.LayerCfg[layerID].ImageWidth;i++){
		// 		*(uint32_t*) (hltdc.LayerCfg[layerID].FBStartAdress + ((j)*hltdc.LayerCfg[layerID].ImageWidth + i)*4)  = (uint32_t)((firstData[i*4+3] << 24) | (firstData[i*4+2] << 16) | (firstData[i*4+1] << 8) | (firstData[i*4]));
		// 		k++;
		// 	}
		// }
	#endif

}