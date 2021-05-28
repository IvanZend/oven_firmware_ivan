#include "W9812G6.h"
#include "mx_init.h"

FMC_SDRAM_CommandTypeDef command;
HAL_StatusTypeDef hal_stat;

void w9812g6_init(SDRAM_HandleTypeDef *hsdram)
{
	uint32_t tmpmrd =0;
	command.CommandMode = FMC_SDRAM_CMD_CLK_ENABLE;
	command.CommandTarget = FMC_SDRAM_CMD_TARGET_BANK1;
	command.AutoRefreshNumber = 1;
	command.ModeRegisterDefinition = 0;
	hal_stat = HAL_SDRAM_SendCommand(hsdram, &command, SDRAM_TIMEOUT);
	delay_us(10);
	command.CommandMode = FMC_SDRAM_CMD_PALL;
	command.CommandTarget = FMC_SDRAM_CMD_TARGET_BANK1;
	command.AutoRefreshNumber = 1;
	command.ModeRegisterDefinition = 0;
	hal_stat = HAL_SDRAM_SendCommand(hsdram, &command, SDRAM_TIMEOUT);
	delay_us(10);
	command.CommandMode = FMC_SDRAM_CMD_AUTOREFRESH_MODE;
	command.CommandTarget = FMC_SDRAM_CMD_TARGET_BANK1;
	command.AutoRefreshNumber = 2;
	command.ModeRegisterDefinition = 0;
	hal_stat = HAL_SDRAM_SendCommand(hsdram, &command, SDRAM_TIMEOUT);
	delay_us(10);
	tmpmrd = (uint32_t)SDRAM_MODEREG_BURST_LENGTH_1|SDRAM_MODEREG_BURST_TYPE_SEQUENTIAL|SDRAM_MODEREG_CAS_LATENCY_2|
             SDRAM_MODEREG_OPERATING_MODE_STANDARD |SDRAM_MODEREG_WRITEBURST_MODE_SINGLE;
	command.CommandMode = FMC_SDRAM_CMD_LOAD_MODE;
	command.CommandTarget = FMC_SDRAM_CMD_TARGET_BANK1;
	command.AutoRefreshNumber = 1;
	command.ModeRegisterDefinition = tmpmrd;
	hal_stat = HAL_SDRAM_SendCommand(hsdram, &command, SDRAM_TIMEOUT);
	delay_us(10);
	/* Step 8: Set the refresh rate counter 
	64msec / 8192 fresh = 7.81 us
	refresh count = 7.81usec * 84MHz  - 20 =  636.04 
	 Set the device refresh counter */
	//hsdram->Instance->SDRTR |= ((uint32_t)((1292)<< 1));
	HAL_SDRAM_ProgramRefreshRate(hsdram, REFRESH_COUNT);

}
