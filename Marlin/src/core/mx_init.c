//#include "stm32f4xx_hal.h"
#include "mx_init.h"
// #include "../inc/MarlinConfig.h"
// #include "../inc/Conditionals_post.h"

#ifndef LCD_FRAME_BUFFER(N)
  #define LCD_FRAME_BUFFER(N) ((uint32_t)(0xC0000000 + (N)*0x1D4C00))
#endif

void MX_LTDC_Init(void)
{

  /* USER CODE BEGIN LTDC_Init 0 */

  /* USER CODE END LTDC_Init 0 */

  LTDC_LayerCfgTypeDef pLayerCfg = {0};

  /* USER CODE BEGIN LTDC_Init 1 */

  /* USER CODE END LTDC_Init 1 */
  hltdc.Instance = LTDC;
  hltdc.Init.HSPolarity = LTDC_HSPOLARITY_AL;
  hltdc.Init.VSPolarity = LTDC_VSPOLARITY_AL;
  hltdc.Init.DEPolarity = LTDC_DEPOLARITY_AL;
  hltdc.Init.PCPolarity = LTDC_PCPOLARITY_IPC;

  const int Horizontal_Synchronization_Width      = 4;//Horizontal Synchronization Width must be between 1 pixels and 4.095 Kpixels.
  const int Horizontal_Back_Porch	                =	2;// 	Horizontal Back Porch must be between 0 pixels and 4.095 Kpixels.
  const int Horizontal_Front_Porch                = 5;//    Horizontal Front Porch must be between 0 pixels and 4.095 Kpixels.
  const int Vertical_Synchronization_Height       = 5;//		  Vertical Synchronization Height must be between 1 lines and 2.047 Klines.
  const int Vertical_Back_Porch                   = 1;// 		Vertical Back Porch must be between 0 lines and 2.047 Klines.
  const int Vertical_Front_Porch                  = 1;//		Vertical Front Porch must be between 0 lines and 2.047 Klines.

  hltdc.Init.HorizontalSync = Horizontal_Synchronization_Width-1;//127;      //This register value is equal to (Horizontal Synchronization Width - 1)
  hltdc.Init.VerticalSync = Vertical_Synchronization_Height-1;          //This register value is equal to (Vertical Synchronization Width - 1)
  hltdc.Init.AccumulatedHBP = Horizontal_Synchronization_Width+Horizontal_Back_Porch-1;      //This register value is equal to the (Horizontal Synchronization Width + Horizontal Back Porch - 1)
  hltdc.Init.AccumulatedVBP = Vertical_Synchronization_Height+Vertical_Back_Porch-1;       //This register value is equal to the (Vertical Synchronization Width + Vertical Back Porch - 1)
  hltdc.Init.AccumulatedActiveW = 800+Horizontal_Synchronization_Width+Horizontal_Back_Porch-1; //This register value is equal to (Horizontal Synchronization Width + Horizontal Back Porch + Active Width - 1)
  hltdc.Init.AccumulatedActiveH = 600+Vertical_Synchronization_Height+Vertical_Back_Porch-1;  //This register value is equal to the (Vertical Synchronization Width + Vertical Back Porch + Active Height - 1)
  hltdc.Init.TotalWidth = 800+Horizontal_Synchronization_Width+Horizontal_Back_Porch+Horizontal_Front_Porch-1;          //This register value is equal to (Horizontal Synchronization Width + Horizontal Back Porch + Active Width + Horizontal Front Porch - 1)
  hltdc.Init.TotalHeigh = 600+Vertical_Synchronization_Height+Vertical_Back_Porch+Vertical_Front_Porch-1;          //This register value is equal to the (Vertical Synchronization Width + Vertical Back Porch + Active Height + Vertical Front Porch - 1)
  hltdc.Init.Backcolor.Blue = 0;
  hltdc.Init.Backcolor.Green = 0;
  hltdc.Init.Backcolor.Red = 0;
  if (HAL_LTDC_Init(&hltdc) != HAL_OK)
  {
    Error_Handler();
  }
  for(int n=0;n<MAX_LAYER;n++){
	  pLayerCfg.WindowX0 = 0;
	  pLayerCfg.WindowX1 = n==0?800:0;
	  pLayerCfg.WindowY0 = 0;
	  pLayerCfg.WindowY1 = n==0?600:0;
	  pLayerCfg.PixelFormat = LTDC_PIXEL_FORMAT_ARGB8888;
	  pLayerCfg.Alpha = 255;
	  pLayerCfg.Alpha0 = 0;
	  pLayerCfg.BlendingFactor1 = LTDC_BLENDING_FACTOR1_PAxCA;
	  pLayerCfg.BlendingFactor2 = LTDC_BLENDING_FACTOR2_PAxCA;
	  pLayerCfg.FBStartAdress = LCD_FRAME_BUFFER(n);
	  pLayerCfg.ImageWidth = 800;
	  pLayerCfg.ImageHeight = 600;
	  pLayerCfg.Backcolor.Blue = 0;
	  pLayerCfg.Backcolor.Green = 0;
	  pLayerCfg.Backcolor.Red = 0;
	  if (HAL_LTDC_ConfigLayer(&hltdc, &pLayerCfg, n) != HAL_OK)
	  {
		Error_Handler();
	  }
  }
}

// void MX_RNG_Init(void)
// {

//   /* USER CODE BEGIN RNG_Init 0 */

//   /* USER CODE END RNG_Init 0 */

//   /* USER CODE BEGIN RNG_Init 1 */

//   /* USER CODE END RNG_Init 1 */
//   hrng.Instance = RNG;
//   if (HAL_RNG_Init(&hrng) != HAL_OK)
//   {
//     // Error_Handler();
//   }
//   /* USER CODE BEGIN RNG_Init 2 */

//   /* USER CODE END RNG_Init 2 */

// }

void MX_SPI3_Init(void)
{

  /* USER CODE BEGIN SPI3_Init 0 */

  /* USER CODE END SPI3_Init 0 */

  /* USER CODE BEGIN SPI3_Init 1 */

  /* USER CODE END SPI3_Init 1 */
  /* SPI3 parameter configuration*/
  // hspi3.Instance = SPI3;
  // hspi3.Init.Mode = SPI_MODE_MASTER;
  // hspi3.Init.Direction = SPI_DIRECTION_2LINES;
  // hspi3.Init.DataSize = SPI_DATASIZE_8BIT;
  // hspi3.Init.CLKPolarity = SPI_POLARITY_LOW;
  // hspi3.Init.CLKPhase = SPI_PHASE_1EDGE;
  // hspi3.Init.NSS = SPI_NSS_SOFT;
  // hspi3.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_4;
  // hspi3.Init.FirstBit = SPI_FIRSTBIT_MSB;
  // hspi3.Init.TIMode = SPI_TIMODE_DISABLE;
  // hspi3.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
  // hspi3.Init.CRCPolynomial = 10;
  // if (HAL_SPI_Init(&hspi3) != HAL_OK)
  // {
  //   Error_Handler();
  // }
  // /* USER CODE BEGIN SPI3_Init 2 */

  // /* USER CODE END SPI3_Init 2 */

}

void MX_USART2_UART_Init(void)
{

  /* USER CODE BEGIN USART2_Init 0 */

  /* USER CODE END USART2_Init 0 */

  /* USER CODE BEGIN USART2_Init 1 */

  /* USER CODE END USART2_Init 1 */
  huart2.Instance = USART2;
  huart2.Init.BaudRate = 256000;
  huart2.Init.WordLength = UART_WORDLENGTH_8B;
  huart2.Init.StopBits = UART_STOPBITS_1;
  huart2.Init.Parity = UART_PARITY_NONE;
  huart2.Init.Mode = UART_MODE_TX_RX;
  huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart2.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart2) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }
  /* USER CODE BEGIN USART2_Init 2 */

  /* USER CODE END USART2_Init 2 */

}

void MX_DMA_Init(void) 
{

  /* DMA controller clock enable */
  __HAL_RCC_DMA1_CLK_ENABLE();

  /* DMA interrupt init */
  /* DMA1_Stream5_IRQn interrupt configuration */
  // HAL_NVIC_SetPriority(DMA1_Stream5_IRQn, 0, 0);
  // HAL_NVIC_EnableIRQ(DMA1_Stream5_IRQn);
  // /* DMA1_Stream6_IRQn interrupt configuration */
  // HAL_NVIC_SetPriority(DMA1_Stream6_IRQn, 0, 0);
  // HAL_NVIC_EnableIRQ(DMA1_Stream6_IRQn);

}

/* FMC initialization function */
void MX_FMC_Init(void)
{

  /* USER CODE BEGIN FMC_Init 0 */

  /* USER CODE END FMC_Init 0 */

  FMC_SDRAM_TimingTypeDef SdramTiming = {0};

  /* USER CODE BEGIN FMC_Init 1 */

  /* USER CODE END FMC_Init 1 */

  /** Perform the SDRAM1 memory initialization sequence
  */
  hsdram1.Instance = FMC_SDRAM_DEVICE;
  /* hsdram1.Init */
  hsdram1.Init.SDBank = FMC_SDRAM_BANK1;
  #ifdef W9825G6KH
    hsdram1.Init.ColumnBitsNumber = FMC_SDRAM_COLUMN_BITS_NUM_9;
    hsdram1.Init.RowBitsNumber = FMC_SDRAM_ROW_BITS_NUM_13;
  #else
    hsdram1.Init.ColumnBitsNumber = FMC_SDRAM_COLUMN_BITS_NUM_8;
    hsdram1.Init.RowBitsNumber = FMC_SDRAM_ROW_BITS_NUM_12;
  #endif
  hsdram1.Init.MemoryDataWidth = FMC_SDRAM_MEM_BUS_WIDTH_16;
  hsdram1.Init.InternalBankNumber = FMC_SDRAM_INTERN_BANKS_NUM_4;
  hsdram1.Init.CASLatency = FMC_SDRAM_CAS_LATENCY_2;
  hsdram1.Init.WriteProtection = FMC_SDRAM_WRITE_PROTECTION_DISABLE;
  hsdram1.Init.SDClockPeriod = FMC_SDRAM_CLOCK_PERIOD_2;
  hsdram1.Init.ReadBurst = FMC_SDRAM_RBURST_ENABLE;
  hsdram1.Init.ReadPipeDelay = FMC_SDRAM_RPIPE_DELAY_0;
  /* SdramTiming */
  SdramTiming.LoadToActiveDelay = 1;
  SdramTiming.ExitSelfRefreshDelay = 1;
  SdramTiming.SelfRefreshTime = 1;
  SdramTiming.RowCycleDelay = 4;
  SdramTiming.WriteRecoveryTime = 1;
  SdramTiming.RPDelay = 1;
  SdramTiming.RCDDelay = 1;

  if (HAL_SDRAM_Init(&hsdram1, &SdramTiming) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

  /* USER CODE BEGIN FMC_Init 2 */

  /* USER CODE END FMC_Init 2 */
}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOE_CLK_ENABLE();
  __HAL_RCC_GPIOI_CLK_ENABLE();
  __HAL_RCC_GPIOF_CLK_ENABLE();
  __HAL_RCC_GPIOH_CLK_ENABLE();
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOG_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();
  __HAL_RCC_GPIOD_CLK_ENABLE();
 
/*--------------------FOR STEPPERS--------------------*/
/*XYZ*/
  GPIO_InitStruct.Pin = X_STEP_Pin_MX;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(X_STEP_GPIO_Port, &GPIO_InitStruct);

  GPIO_InitStruct.Pin = X_ENABLE_Pin_MX;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(X_ENABLE_GPIO_Port, &GPIO_InitStruct);

  GPIO_InitStruct.Pin = Y_STEP_Pin_MX;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(Y_STEP_GPIO_Port, &GPIO_InitStruct);

  GPIO_InitStruct.Pin = Y_ENABLE_Pin_MX;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(Y_ENABLE_GPIO_Port, &GPIO_InitStruct);

  GPIO_InitStruct.Pin = Z_STEP_Pin_MX;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(Z_STEP_GPIO_Port, &GPIO_InitStruct);

  GPIO_InitStruct.Pin = Z_ENABLE_Pin_MX;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(Z_ENABLE_GPIO_Port, &GPIO_InitStruct);

/*EXTRUDERS*/
  GPIO_InitStruct.Pin = E0_STEP_Pin_MX;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(E0_STEP_GPIO_Port, &GPIO_InitStruct);

  GPIO_InitStruct.Pin = E0_ENABLE_Pin_MX;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(E0_ENABLE_GPIO_Port, &GPIO_InitStruct);

  GPIO_InitStruct.Pin = E1_STEP_Pin_MX;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
  HAL_GPIO_Init(E1_STEP_GPIO_Port, &GPIO_InitStruct);

  GPIO_InitStruct.Pin = E1_ENABLE_Pin_MX;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
  HAL_GPIO_Init(E1_ENABLE_GPIO_Port, &GPIO_InitStruct);

  GPIO_InitStruct.Pin = E2_STEP_Pin_MX;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(E2_STEP_GPIO_Port, &GPIO_InitStruct);

  GPIO_InitStruct.Pin = E2_ENABLE_Pin_MX;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(E2_ENABLE_GPIO_Port, &GPIO_InitStruct);

/*--------------------OUTPUTS, CHIP-SELECT--------------------*/
/*SPIs-TALK*/

  GPIO_InitStruct.Pin = ADS_CS1_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(ADS_CS1_GPIO_Port, &GPIO_InitStruct);

  GPIO_InitStruct.Pin = ADS_CS2_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(ADS_CS2_GPIO_Port, &GPIO_InitStruct);

  GPIO_InitStruct.Pin = FLASH_SPI1_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(FLASH_SPI1_GPIO_Port, &GPIO_InitStruct);

  GPIO_InitStruct.Pin = FLASH_SPI2_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(FLASH_SPI2_GPIO_Port, &GPIO_InitStruct);

/*TOUCH*/

  GPIO_InitStruct.Pin = TS_SCK_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(TS_SCK_GPIO_Port, &GPIO_InitStruct);

  GPIO_InitStruct.Pin = TS_MISO_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(TS_MISO_GPIO_Port, &GPIO_InitStruct);

  GPIO_InitStruct.Pin = TS_MOSI_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(TS_MOSI_GPIO_Port, &GPIO_InitStruct);

  GPIO_InitStruct.Pin = TS_CS_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(TS_CS_GPIO_Port, &GPIO_InitStruct);

  GPIO_InitStruct.Pin = TS_IRQ_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_FALLING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(TS_IRQ_GPIO_Port, &GPIO_InitStruct);

/*TRIGGERS-LOW-FOR-ENABLE*/

  GPIO_InitStruct.Pin = DIEN1_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
  HAL_GPIO_Init(DIEN1_GPIO_Port, &GPIO_InitStruct);

  GPIO_InitStruct.Pin = DIEN2_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(DIEN2_GPIO_Port, &GPIO_InitStruct);
  
/*OTHERS*/
  GPIO_InitStruct.Pin = LCD_PWM_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
  HAL_GPIO_Init(LCD_PWM_GPIO_Port, &GPIO_InitStruct);

  GPIO_InitStruct.Pin = LCD_RES_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
  HAL_GPIO_Init(LCD_RES_GPIO_Port, &GPIO_InitStruct);

  GPIO_InitStruct.Pin = USB_PWM_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(USB_PWM_GPIO_Port, &GPIO_InitStruct);

  GPIO_InitStruct.Pin = BUZZER_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(BUZZER_GPIO_Port, &GPIO_InitStruct);

/*--------------------LED-PINS--------------------*/

    GPIO_InitStruct.Pin = SRV_PWM_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    // GPIO_InitStruct.Alternate = GPIO_AF9_TIM12;
    HAL_GPIO_Init(SRV_PWM_GPIO_Port, &GPIO_InitStruct);

  // GPIO_InitStruct.Pin = RED_PWM_Pin;
  // GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  // GPIO_InitStruct.Pull = GPIO_NOPULL;
  // GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  // HAL_GPIO_Init(RED_PWM_GPIO_Port, &GPIO_InitStruct);

  GPIO_InitStruct.Pin = GRN_PWM_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GRN_PWM_GPIO_Port, &GPIO_InitStruct);

  GPIO_InitStruct.Pin = BLUE_PWM_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(BLUE_PWM_GPIO_Port, &GPIO_InitStruct);

/*--------------------INPUTS-FOR-TRIGGERS--------------------*/
  GPIO_InitStruct.Pin = DIOD0_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLDOWN;
  HAL_GPIO_Init(DIOD0_GPIO_Port, &GPIO_InitStruct);

  GPIO_InitStruct.Pin = DIOD1_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLDOWN;
  HAL_GPIO_Init(DIOD1_GPIO_Port, &GPIO_InitStruct);

  GPIO_InitStruct.Pin = DID2_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLDOWN;
  HAL_GPIO_Init(DID2_GPIO_Port, &GPIO_InitStruct);

  GPIO_InitStruct.Pin = DID3_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLDOWN;
  HAL_GPIO_Init(DID3_GPIO_Port, &GPIO_InitStruct);

  GPIO_InitStruct.Pin = DID4_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLDOWN;
  HAL_GPIO_Init(DID4_GPIO_Port, &GPIO_InitStruct);

  GPIO_InitStruct.Pin = DID5_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLDOWN;
  HAL_GPIO_Init(DID5_GPIO_Port, &GPIO_InitStruct);


  
/*XYZ*/
  HAL_GPIO_WritePin(X_STEP_GPIO_Port,     X_STEP_Pin_MX,        GPIO_PIN_RESET);
  HAL_GPIO_WritePin(X_ENABLE_GPIO_Port,   X_ENABLE_Pin_MX,      GPIO_PIN_RESET);
  HAL_GPIO_WritePin(Y_STEP_GPIO_Port,     Y_STEP_Pin_MX,        GPIO_PIN_RESET);
  HAL_GPIO_WritePin(Y_ENABLE_GPIO_Port,   Y_ENABLE_Pin_MX,      GPIO_PIN_RESET);
  HAL_GPIO_WritePin(Z_STEP_GPIO_Port,     Z_STEP_Pin_MX,        GPIO_PIN_RESET);
  HAL_GPIO_WritePin(Z_ENABLE_GPIO_Port,   Z_ENABLE_Pin_MX,      GPIO_PIN_RESET);
/*EXTRUDERS*/
  HAL_GPIO_WritePin(E0_STEP_GPIO_Port,    E0_STEP_Pin_MX,       GPIO_PIN_RESET);
  HAL_GPIO_WritePin(E0_ENABLE_GPIO_Port,  E0_ENABLE_Pin_MX,     GPIO_PIN_RESET);
  HAL_GPIO_WritePin(E1_STEP_GPIO_Port,    E1_STEP_Pin_MX,       GPIO_PIN_RESET);
  HAL_GPIO_WritePin(E1_ENABLE_GPIO_Port,  E1_ENABLE_Pin_MX,     GPIO_PIN_RESET);
  HAL_GPIO_WritePin(E2_STEP_GPIO_Port,    E2_STEP_Pin_MX,       GPIO_PIN_RESET);
  HAL_GPIO_WritePin(E2_ENABLE_GPIO_Port,  E2_ENABLE_Pin_MX,     GPIO_PIN_RESET);
/*SPIs-TALK*/	
  HAL_GPIO_WritePin(ADS_CS1_GPIO_Port,    ADS_CS1_Pin,          GPIO_PIN_SET);
  HAL_GPIO_WritePin(ADS_CS2_GPIO_Port,    ADS_CS2_Pin,          GPIO_PIN_SET);
  HAL_GPIO_WritePin(FLASH_SPI1_GPIO_Port, FLASH_SPI1_Pin,       GPIO_PIN_SET);
  HAL_GPIO_WritePin(FLASH_SPI2_GPIO_Port, FLASH_SPI2_Pin,       GPIO_PIN_SET);
/*TRIGGERS-LOW-FOR-ENABLE*/
  HAL_GPIO_WritePin(DIEN1_GPIO_Port,      DIEN1_Pin,            GPIO_PIN_SET);
  HAL_GPIO_WritePin(DIEN2_GPIO_Port,      DIEN2_Pin,            GPIO_PIN_SET);
/*TOUCH*/
  // HAL_GPIO_WritePin(TS_SCK_GPIO_Port,     TS_SCK_Pin,           GPIO_PIN_RESET);
  // HAL_GPIO_WritePin(TS_MOSI_GPIO_Port,    TS_MOSI_Pin,          GPIO_PIN_RESET);
  // HAL_GPIO_WritePin(TS_MISO_GPIO_Port,    TS_MOSI_Pin,          GPIO_PIN_RESET);
  // HAL_GPIO_WritePin(TS_IRQ_GPIO_Port,     TS_IRQ_Pin,           GPIO_PIN_RESET);
/*LED-PINS*/
  // HAL_GPIO_WritePin(RED_PWM_GPIO_Port,    RED_PWM_Pin,          GPIO_PIN_SET);
  HAL_GPIO_WritePin(GRN_PWM_GPIO_Port,    GRN_PWM_Pin,          GPIO_PIN_SET);
  HAL_GPIO_WritePin(BLUE_PWM_GPIO_Port,   BLUE_PWM_Pin,         GPIO_PIN_RESET);//<-временно за кулер

/*OTHERS*/
  HAL_GPIO_WritePin(BUZZER_GPIO_Port,     BUZZER_Pin,           GPIO_PIN_RESET);
  HAL_GPIO_WritePin(LCD_PWM_GPIO_Port,    LCD_PWM_Pin,          GPIO_PIN_RESET);
  HAL_GPIO_WritePin(LCD_RES_GPIO_Port,    LCD_RES_Pin,          GPIO_PIN_RESET);
  HAL_GPIO_WritePin(USB_PWM_GPIO_Port,    USB_PWM_Pin,          GPIO_PIN_RESET);

}

void MX_TIM2_Init(void)
{

  /* USER CODE BEGIN TIM2_Init 0 */

  /* USER CODE END TIM2_Init 0 */

  TIM_MasterConfigTypeDef sMasterConfig = {0};
  TIM_OC_InitTypeDef sConfigOC = {0};

  /* USER CODE BEGIN TIM2_Init 1 */

  /* USER CODE END TIM2_Init 1 */
  htim2.Instance = TIM2;
  htim2.Init.Prescaler = 0;
  htim2.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim2.Init.Period = 0;
  htim2.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim2.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_PWM_Init(&htim2) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim2, &sMasterConfig) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }
  sConfigOC.OCMode = TIM_OCMODE_PWM1;
  sConfigOC.Pulse = 0;
  sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
  sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
  if (HAL_TIM_PWM_ConfigChannel(&htim2, &sConfigOC, TIM_CHANNEL_1) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }
  if (HAL_TIM_PWM_ConfigChannel(&htim2, &sConfigOC, TIM_CHANNEL_2) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }
  if (HAL_TIM_PWM_ConfigChannel(&htim2, &sConfigOC, TIM_CHANNEL_3) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }
  if (HAL_TIM_PWM_ConfigChannel(&htim2, &sConfigOC, TIM_CHANNEL_4) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }
  /* USER CODE BEGIN TIM2_Init 2 */

  /* USER CODE END TIM2_Init 2 */
  HAL_TIM_MspPostInit(&htim2);

}

/**
  * @brief TIM3 Initialization Function
  * @param None
  * @retval None
  */
void MX_TIM3_Init(void)
{

  /* USER CODE BEGIN TIM3_Init 0 */

  /* USER CODE END TIM3_Init 0 */

  TIM_MasterConfigTypeDef sMasterConfig = {0};
  TIM_OC_InitTypeDef sConfigOC = {0};

  /* USER CODE BEGIN TIM3_Init 1 */

  /* USER CODE END TIM3_Init 1 */
  htim3.Instance = TIM3;
  htim3.Init.Prescaler = 0;
  htim3.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim3.Init.Period = 0;
  htim3.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim3.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_PWM_Init(&htim3) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim3, &sMasterConfig) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }
  sConfigOC.OCMode = TIM_OCMODE_PWM1;
  sConfigOC.Pulse = 0;
  sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
  sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
  if (HAL_TIM_PWM_ConfigChannel(&htim3, &sConfigOC, TIM_CHANNEL_1) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }
  if (HAL_TIM_PWM_ConfigChannel(&htim3, &sConfigOC, TIM_CHANNEL_2) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }
  if (HAL_TIM_PWM_ConfigChannel(&htim3, &sConfigOC, TIM_CHANNEL_3) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }
  if (HAL_TIM_PWM_ConfigChannel(&htim3, &sConfigOC, TIM_CHANNEL_4) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }
  /* USER CODE BEGIN TIM3_Init 2 */

  /* USER CODE END TIM3_Init 2 */
  HAL_TIM_MspPostInit(&htim3);

}

/**
  * @brief TIM4 Initialization Function
  * @param None
  * @retval None
  */
void MX_TIM4_Init(void)
{
  /*

  если мы хотим таймер на каждые 2.3 секунды:
  (PSC+1)*(ARR+1) = TIMclk/Updatefrequency
  Updatefrequency = 1/2.3
  TIMclk = 84000000 // - частота APB1, 168/2
  (PSC+1) * (ARR+1) = 84000000 / (1 / 2.3) = 84000000 * 2.3 = 193200000
  берем круглый результат PSC+1 = 10000, делитель будет 9999
  соответственно ARR+1 = 19320, период будет 19320
  
  */

  /* USER CODE BEGIN TIM4_Init 0 */

  /* USER CODE END TIM4_Init 0 */

  TIM_ClockConfigTypeDef sClockSourceConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};
  TIM_OC_InitTypeDef sConfigOC = {0};

  /* USER CODE BEGIN TIM4_Init 1 */

  /* USER CODE END TIM4_Init 1 */
  htim4.Instance = TIM4;
  htim4.Init.Prescaler = 9999;
  htim4.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim4.Init.Period = 19319;
  htim4.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim4.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim4) != HAL_OK)
  {
    Error_Handler();
  }
  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&htim4, &sClockSourceConfig) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_TIM_PWM_Init(&htim4) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim4, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sConfigOC.OCMode = TIM_OCMODE_PWM1;
  sConfigOC.Pulse = 0;
  sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
  sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
  if (HAL_TIM_PWM_ConfigChannel(&htim4, &sConfigOC, TIM_CHANNEL_3) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_TIM_PWM_ConfigChannel(&htim4, &sConfigOC, TIM_CHANNEL_4) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM4_Init 2 */

  /* USER CODE END TIM4_Init 2 */
  HAL_TIM_MspPostInit(&htim4);

}


/**
  * @brief TIM12 Initialization Function
  * @param None
  * @retval None
  */
void MX_TIM12_Init(void)
{

  /* USER CODE BEGIN TIM12_Init 0 */

  /* USER CODE END TIM12_Init 0 */

  TIM_OC_InitTypeDef sConfigOC = {0};

  /* USER CODE BEGIN TIM12_Init 1 */

  /* USER CODE END TIM12_Init 1 */
  htim12.Instance = TIM12;
  htim12.Init.Prescaler = 0;
  htim12.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim12.Init.Period = 0;
  htim12.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim12.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_PWM_Init(&htim12) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }
  sConfigOC.OCMode = TIM_OCMODE_PWM1;
  sConfigOC.Pulse = 0;
  sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
  sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
  if (HAL_TIM_PWM_ConfigChannel(&htim12, &sConfigOC, TIM_CHANNEL_1) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }
  /* USER CODE BEGIN TIM12_Init 2 */

  /* USER CODE END TIM12_Init 2 */
  HAL_TIM_MspPostInit(&htim12);

}
/* USER CODE BEGIN 4 */
void delay_us(uint32_t delay)
{
	uint32_t i=0;
	//byte j=0;
	for (i=0;i<(delay*10);i++)
	{
		asm("NOP");
	}
}


int check_SPI_enables(){
  /*DRIVERS: reset to off*/
  //GPIO_PinState HAL_GPIO_ReadPin
  if(HAL_GPIO_ReadPin(X_ENABLE_GPIO_Port,   X_ENABLE_Pin_MX) == GPIO_PIN_SET) return 1;
  if(HAL_GPIO_ReadPin(Y_ENABLE_GPIO_Port,   Y_ENABLE_Pin_MX) == GPIO_PIN_SET) return 1;
  if(HAL_GPIO_ReadPin(Z_ENABLE_GPIO_Port,   Z_ENABLE_Pin_MX) == GPIO_PIN_SET) return 1;
  if(HAL_GPIO_ReadPin(E0_ENABLE_GPIO_Port,  E0_ENABLE_Pin_MX) == GPIO_PIN_SET) return 1;
  if(HAL_GPIO_ReadPin(E1_ENABLE_GPIO_Port,  E1_ENABLE_Pin_MX) == GPIO_PIN_SET) return 1;
  if(HAL_GPIO_ReadPin(E2_ENABLE_GPIO_Port,  E2_ENABLE_Pin_MX) == GPIO_PIN_SET) return 1;
  /*DRIVERS: ads and flsh set off*/
  if(HAL_GPIO_ReadPin(ADS_CS1_GPIO_Port,    ADS_CS1_Pin) == GPIO_PIN_RESET) return 1;
  if(HAL_GPIO_ReadPin(ADS_CS2_GPIO_Port,    ADS_CS2_Pin) == GPIO_PIN_RESET) return 1;
  if(HAL_GPIO_ReadPin(FLASH_SPI1_GPIO_Port, FLASH_SPI1_Pin) == GPIO_PIN_RESET) return 1;
  if(HAL_GPIO_ReadPin(FLASH_SPI2_GPIO_Port, FLASH_SPI2_Pin) == GPIO_PIN_RESET) return 1;

  return 0;
}

void TestFuncForReference(int x){
	// int y = x;

  char txdata[50];
  sprintf(txdata,"echo: counter = %d \r\n",x);
  HAL_UART_Transmit(&huart2, (uint8_t *)txdata, strlen(txdata), 10);
};