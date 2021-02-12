/**
  ******************************************************************************
  * @file    ltdc.c
  * @brief   This file provides code for the configuration
  *          of the LTDC instances.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2021 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under Ultimate Liberty license
  * SLA0044, the "License"; You may not use this file except in compliance with
  * the License. You may obtain a copy of the License at:
  *                             www.st.com/SLA0044
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "ltdc.h"

/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

LTDC_HandleTypeDef hltdc;

/* LTDC init function */
void MX_LTDC_Init(void)
{
  LTDC_LayerCfgTypeDef pLayerCfg = {0};

  hltdc.Instance = LTDC;
  hltdc.Init.HSPolarity = LTDC_HSPOLARITY_AL;
  hltdc.Init.VSPolarity = LTDC_VSPOLARITY_AL;
  hltdc.Init.DEPolarity = LTDC_DEPOLARITY_AL;
  hltdc.Init.PCPolarity = LTDC_PCPOLARITY_IPC;
  hltdc.Init.HorizontalSync = 9;
  hltdc.Init.VerticalSync = 9;
  hltdc.Init.AccumulatedHBP = 52;
  hltdc.Init.AccumulatedVBP = 21;
  hltdc.Init.AccumulatedActiveW = 532;
  hltdc.Init.AccumulatedActiveH = 293;
  hltdc.Init.TotalWidth = 540;
  hltdc.Init.TotalHeigh = 297;
  hltdc.Init.Backcolor.Blue = 0;
  hltdc.Init.Backcolor.Green = 0;
  hltdc.Init.Backcolor.Red = 0;
  if (HAL_LTDC_Init(&hltdc) != HAL_OK)
  {
    Error_Handler();
  }
  pLayerCfg.WindowX0 = 0;
  pLayerCfg.WindowX1 = 480;
  pLayerCfg.WindowY0 = 0;
  pLayerCfg.WindowY1 = 272;
  pLayerCfg.PixelFormat = LTDC_PIXEL_FORMAT_RGB565;
  pLayerCfg.Alpha = 255;
  pLayerCfg.Alpha0 = 0;
  pLayerCfg.BlendingFactor1 = LTDC_BLENDING_FACTOR1_PAxCA;
  pLayerCfg.BlendingFactor2 = LTDC_BLENDING_FACTOR2_PAxCA;
  pLayerCfg.FBStartAdress = 0;
  pLayerCfg.ImageWidth = 480;
  pLayerCfg.ImageHeight = 272;
  pLayerCfg.Backcolor.Blue = 0;
  pLayerCfg.Backcolor.Green = 0;
  pLayerCfg.Backcolor.Red = 0;
  if (HAL_LTDC_ConfigLayer(&hltdc, &pLayerCfg, 0) != HAL_OK)
  {
    Error_Handler();
  }

}

void HAL_LTDC_MspInit(LTDC_HandleTypeDef* ltdcHandle)
{

  GPIO_InitTypeDef GPIO_InitStruct = {0};
  if(ltdcHandle->Instance==LTDC)
  {
  /* USER CODE BEGIN LTDC_MspInit 0 */

  /* USER CODE END LTDC_MspInit 0 */
    /* LTDC clock enable */
    __HAL_RCC_LTDC_CLK_ENABLE();

    __HAL_RCC_GPIOK_CLK_ENABLE();
    __HAL_RCC_GPIOJ_CLK_ENABLE();
    __HAL_RCC_GPIOI_CLK_ENABLE();
    /**LTDC GPIO Configuration
    PK5     ------> LTDC_B6
    PK6     ------> LTDC_B7
    PK3     ------> LTDC_B4
    PJ15     ------> LTDC_B3
    PK4     ------> LTDC_B5
    PJ14     ------> LTDC_B2
    PK7     ------> LTDC_DE
    PJ13     ------> LTDC_B1
    PJ12     ------> LTDC_B0
    PI12     ------> LTDC_HSYNC
    PI14     ------> LTDC_CLK
    PI13     ------> LTDC_VSYNC
    PK2     ------> LTDC_G7
    PK1     ------> LTDC_G6
    PJ11     ------> LTDC_G4
    PK0     ------> LTDC_G5
    PJ10     ------> LTDC_G3
    PJ9     ------> LTDC_G2
    PJ8     ------> LTDC_G1
    PJ6     ------> LTDC_R7
    PJ7     ------> LTDC_G0
    PI15     ------> LTDC_R0
    PJ0     ------> LTDC_R1
    PJ5     ------> LTDC_R6
    PJ1     ------> LTDC_R2
    PJ4     ------> LTDC_R5
    PJ2     ------> LTDC_R3
    PJ3     ------> LTDC_R4
    */
    GPIO_InitStruct.Pin = LCD_B6_Pin|LCD_B7_Pin|LCD_B4_Pin|LCD_B5_Pin
                          |LCD_DE_Pin|LCD_G7_Pin|LCD_G6_Pin|LCD_G5_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    GPIO_InitStruct.Alternate = GPIO_AF14_LTDC;
    HAL_GPIO_Init(GPIOK, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = LCD_B3_Pin|LCD_B2_Pin|LCD_B1_Pin|LCD_B0_Pin
                          |LCD_G4_Pin|LCD_G3_Pin|LCD_G2_Pin|LCD_G1_Pin
                          |R7_Pin|G0_Pin|LCD_R1_Pin|LCD_R6_Pin
                          |LCD_R2_Pin|LCD_R5_Pin|R3_Pin|R4_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    GPIO_InitStruct.Alternate = GPIO_AF14_LTDC;
    HAL_GPIO_Init(GPIOJ, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = LCD_HSYNC_Pin|LCD_CLK_Pin|LCD_VSYNC_Pin|LCD_R0_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    GPIO_InitStruct.Alternate = GPIO_AF14_LTDC;
    HAL_GPIO_Init(GPIOI, &GPIO_InitStruct);

  /* USER CODE BEGIN LTDC_MspInit 1 */

  /* USER CODE END LTDC_MspInit 1 */
  }
}

void HAL_LTDC_MspDeInit(LTDC_HandleTypeDef* ltdcHandle)
{

  if(ltdcHandle->Instance==LTDC)
  {
  /* USER CODE BEGIN LTDC_MspDeInit 0 */

  /* USER CODE END LTDC_MspDeInit 0 */
    /* Peripheral clock disable */
    __HAL_RCC_LTDC_CLK_DISABLE();

    /**LTDC GPIO Configuration
    PK5     ------> LTDC_B6
    PK6     ------> LTDC_B7
    PK3     ------> LTDC_B4
    PJ15     ------> LTDC_B3
    PK4     ------> LTDC_B5
    PJ14     ------> LTDC_B2
    PK7     ------> LTDC_DE
    PJ13     ------> LTDC_B1
    PJ12     ------> LTDC_B0
    PI12     ------> LTDC_HSYNC
    PI14     ------> LTDC_CLK
    PI13     ------> LTDC_VSYNC
    PK2     ------> LTDC_G7
    PK1     ------> LTDC_G6
    PJ11     ------> LTDC_G4
    PK0     ------> LTDC_G5
    PJ10     ------> LTDC_G3
    PJ9     ------> LTDC_G2
    PJ8     ------> LTDC_G1
    PJ6     ------> LTDC_R7
    PJ7     ------> LTDC_G0
    PI15     ------> LTDC_R0
    PJ0     ------> LTDC_R1
    PJ5     ------> LTDC_R6
    PJ1     ------> LTDC_R2
    PJ4     ------> LTDC_R5
    PJ2     ------> LTDC_R3
    PJ3     ------> LTDC_R4
    */
    HAL_GPIO_DeInit(GPIOK, LCD_B6_Pin|LCD_B7_Pin|LCD_B4_Pin|LCD_B5_Pin
                          |LCD_DE_Pin|LCD_G7_Pin|LCD_G6_Pin|LCD_G5_Pin);

    HAL_GPIO_DeInit(GPIOJ, LCD_B3_Pin|LCD_B2_Pin|LCD_B1_Pin|LCD_B0_Pin
                          |LCD_G4_Pin|LCD_G3_Pin|LCD_G2_Pin|LCD_G1_Pin
                          |R7_Pin|G0_Pin|LCD_R1_Pin|LCD_R6_Pin
                          |LCD_R2_Pin|LCD_R5_Pin|R3_Pin|R4_Pin);

    HAL_GPIO_DeInit(GPIOI, LCD_HSYNC_Pin|LCD_CLK_Pin|LCD_VSYNC_Pin|LCD_R0_Pin);

  /* USER CODE BEGIN LTDC_MspDeInit 1 */

  /* USER CODE END LTDC_MspDeInit 1 */
  }
}

/* USER CODE BEGIN 1 */

/* USER CODE END 1 */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
