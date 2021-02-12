/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2021 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32h7xx_hal.h"
#include "stm32h7xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define WIFI_GPIO_Pin GPIO_PIN_4
#define WIFI_GPIO_GPIO_Port GPIOI
#define SDNCAS_Pin GPIO_PIN_15
#define SDNCAS_GPIO_Port GPIOG
#define LCD_B6_Pin GPIO_PIN_5
#define LCD_B6_GPIO_Port GPIOK
#define OCSPI1_IO6_Pin GPIO_PIN_9
#define OCSPI1_IO6_GPIO_Port GPIOG
#define OCSPI1_IO7_Pin GPIO_PIN_7
#define OCSPI1_IO7_GPIO_Port GPIOD
#define D3_Pin GPIO_PIN_1
#define D3_GPIO_Port GPIOD
#define SDIO1_D2_Pin GPIO_PIN_10
#define SDIO1_D2_GPIO_Port GPIOC
#define WIFI_BOOT_Pin GPIO_PIN_3
#define WIFI_BOOT_GPIO_Port GPIOI
#define WIFI_DATRDY_Pin GPIO_PIN_5
#define WIFI_DATRDY_GPIO_Port GPIOI
#define FMC_NBL0_Pin GPIO_PIN_0
#define FMC_NBL0_GPIO_Port GPIOE
#define LCD_B7_Pin GPIO_PIN_6
#define LCD_B7_GPIO_Port GPIOK
#define LCD_B4_Pin GPIO_PIN_3
#define LCD_B4_GPIO_Port GPIOK
#define USER_LED1_Pin GPIO_PIN_11
#define USER_LED1_GPIO_Port GPIOG
#define LCD_B3_Pin GPIO_PIN_15
#define LCD_B3_GPIO_Port GPIOJ
#define SDIO1_CMD_Pin GPIO_PIN_2
#define SDIO1_CMD_GPIO_Port GPIOD
#define SDIO1_CK_Pin GPIO_PIN_12
#define SDIO1_CK_GPIO_Port GPIOC
#define JTCK_Pin GPIO_PIN_14
#define JTCK_GPIO_Port GPIOA
#define uSD_Detect_Pin GPIO_PIN_8
#define uSD_Detect_GPIO_Port GPIOI
#define FMC_NBL1_Pin GPIO_PIN_1
#define FMC_NBL1_GPIO_Port GPIOE
#define JTDO_TRACESWO_Pin GPIO_PIN_3
#define JTDO_TRACESWO_GPIO_Port GPIOB
#define LCD_B5_Pin GPIO_PIN_4
#define LCD_B5_GPIO_Port GPIOK
#define LCD_B2_Pin GPIO_PIN_14
#define LCD_B2_GPIO_Port GPIOJ
#define D2_Pin GPIO_PIN_0
#define D2_GPIO_Port GPIOD
#define JTDI_Pin GPIO_PIN_15
#define JTDI_GPIO_Port GPIOA
#define SPI2_SCK_Pin GPIO_PIN_12
#define SPI2_SCK_GPIO_Port GPIOA
#define SPI2_NSS_Pin GPIO_PIN_11
#define SPI2_NSS_GPIO_Port GPIOA
#define LCD_DE_Pin GPIO_PIN_7
#define LCD_DE_GPIO_Port GPIOK
#define LCD_B1_Pin GPIO_PIN_13
#define LCD_B1_GPIO_Port GPIOJ
#define SDIO1_D3_Pin GPIO_PIN_11
#define SDIO1_D3_GPIO_Port GPIOC
#define WIFI_WKUP_Pin GPIO_PIN_2
#define WIFI_WKUP_GPIO_Port GPIOI
#define LCD_B0_Pin GPIO_PIN_12
#define LCD_B0_GPIO_Port GPIOJ
#define WIFI_RST_Pin GPIO_PIN_1
#define WIFI_RST_GPIO_Port GPIOI
#define JTMS_Pin GPIO_PIN_13
#define JTMS_GPIO_Port GPIOA
#define VCP_RX_Pin GPIO_PIN_10
#define VCP_RX_GPIO_Port GPIOA
#define SDIO1_D1_Pin GPIO_PIN_9
#define SDIO1_D1_GPIO_Port GPIOC
#define WAKEUP_Pin GPIO_PIN_13
#define WAKEUP_GPIO_Port GPIOC
#define VCP_TX_Pin GPIO_PIN_9
#define VCP_TX_GPIO_Port GPIOA
#define SDIO1_D0_Pin GPIO_PIN_8
#define SDIO1_D0_GPIO_Port GPIOC
#define SDCLK_Pin GPIO_PIN_8
#define SDCLK_GPIO_Port GPIOG
#define A1_Pin GPIO_PIN_1
#define A1_GPIO_Port GPIOF
#define A0_Pin GPIO_PIN_0
#define A0_GPIO_Port GPIOF
#define MCO_Pin GPIO_PIN_8
#define MCO_GPIO_Port GPIOA
#define OCSPI1_NCS_Pin GPIO_PIN_6
#define OCSPI1_NCS_GPIO_Port GPIOG
#define A15_Pin GPIO_PIN_5
#define A15_GPIO_Port GPIOG
#define AUDIO_NRST_Pin GPIO_PIN_3
#define AUDIO_NRST_GPIO_Port GPIOG
#define A2_Pin GPIO_PIN_2
#define A2_GPIO_Port GPIOF
#define LCD_HSYNC_Pin GPIO_PIN_12
#define LCD_HSYNC_GPIO_Port GPIOI
#define A4_Pin GPIO_PIN_4
#define A4_GPIO_Port GPIOF
#define LCD_CLK_Pin GPIO_PIN_14
#define LCD_CLK_GPIO_Port GPIOI
#define LCD_VSYNC_Pin GPIO_PIN_13
#define LCD_VSYNC_GPIO_Port GPIOI
#define A14_Pin GPIO_PIN_4
#define A14_GPIO_Port GPIOG
#define USER_LED2_Pin GPIO_PIN_2
#define USER_LED2_GPIO_Port GPIOG
#define LCD_G7_Pin GPIO_PIN_2
#define LCD_G7_GPIO_Port GPIOK
#define LCD_G6_Pin GPIO_PIN_1
#define LCD_G6_GPIO_Port GPIOK
#define A3_Pin GPIO_PIN_3
#define A3_GPIO_Port GPIOF
#define A5_Pin GPIO_PIN_5
#define A5_GPIO_Port GPIOF
#define SPI2_MISO_Pin GPIO_PIN_2
#define SPI2_MISO_GPIO_Port GPIOC
#define LCD_G4_Pin GPIO_PIN_11
#define LCD_G4_GPIO_Port GPIOJ
#define LCD_G5_Pin GPIO_PIN_0
#define LCD_G5_GPIO_Port GPIOK
#define LCD_G3_Pin GPIO_PIN_10
#define LCD_G3_GPIO_Port GPIOJ
#define LCD_G2_Pin GPIO_PIN_9
#define LCD_G2_GPIO_Port GPIOJ
#define LCD_G1_Pin GPIO_PIN_8
#define LCD_G1_GPIO_Port GPIOJ
#define I2C4_SDA_Pin GPIO_PIN_13
#define I2C4_SDA_GPIO_Port GPIOD
#define D0_Pin GPIO_PIN_14
#define D0_GPIO_Port GPIOD
#define D1_Pin GPIO_PIN_15
#define D1_GPIO_Port GPIOD
#define R7_Pin GPIO_PIN_6
#define R7_GPIO_Port GPIOJ
#define G0_Pin GPIO_PIN_7
#define G0_GPIO_Port GPIOJ
#define D9_Pin GPIO_PIN_12
#define D9_GPIO_Port GPIOE
#define D13_Pin GPIO_PIN_8
#define D13_GPIO_Port GPIOD
#define D15_Pin GPIO_PIN_10
#define D15_GPIO_Port GPIOD
#define I2C4_SCL_Pin GPIO_PIN_12
#define I2C4_SCL_GPIO_Port GPIOD
#define OCSPI1_IO4_Pin GPIO_PIN_1
#define OCSPI1_IO4_GPIO_Port GPIOC
#define LCD_INT_Pin GPIO_PIN_2
#define LCD_INT_GPIO_Port GPIOH
#define LCD_R0_Pin GPIO_PIN_15
#define LCD_R0_GPIO_Port GPIOI
#define A7_Pin GPIO_PIN_13
#define A7_GPIO_Port GPIOF
#define D4_Pin GPIO_PIN_7
#define D4_GPIO_Port GPIOE
#define D10_Pin GPIO_PIN_13
#define D10_GPIO_Port GPIOE
#define SDNE1_Pin GPIO_PIN_6
#define SDNE1_GPIO_Port GPIOH
#define SPI2_MOSI_Pin GPIO_PIN_3
#define SPI2_MOSI_GPIO_Port GPIOC
#define OCSPI1_IO5_Pin GPIO_PIN_3
#define OCSPI1_IO5_GPIO_Port GPIOH
#define OCSPI1_DQS_Pin GPIO_PIN_5
#define OCSPI1_DQS_GPIO_Port GPIOC
#define LCD_R1_Pin GPIO_PIN_0
#define LCD_R1_GPIO_Port GPIOJ
#define SDNRAS_Pin GPIO_PIN_11
#define SDNRAS_GPIO_Port GPIOF
#define A9_Pin GPIO_PIN_15
#define A9_GPIO_Port GPIOF
#define D11_Pin GPIO_PIN_14
#define D11_GPIO_Port GPIOE
#define D7_Pin GPIO_PIN_10
#define D7_GPIO_Port GPIOE
#define LCD_R6_Pin GPIO_PIN_5
#define LCD_R6_GPIO_Port GPIOJ
#define D14_Pin GPIO_PIN_9
#define D14_GPIO_Port GPIOD
#define LCD_BL_CTRL_Pin GPIO_PIN_1
#define LCD_BL_CTRL_GPIO_Port GPIOA
#define SDNWE_Pin GPIO_PIN_5
#define SDNWE_GPIO_Port GPIOH
#define LCD_R2_Pin GPIO_PIN_1
#define LCD_R2_GPIO_Port GPIOJ
#define LCD_R5_Pin GPIO_PIN_4
#define LCD_R5_GPIO_Port GPIOJ
#define A8_Pin GPIO_PIN_14
#define A8_GPIO_Port GPIOF
#define A11_Pin GPIO_PIN_1
#define A11_GPIO_Port GPIOG
#define D6_Pin GPIO_PIN_9
#define D6_GPIO_Port GPIOE
#define D12_Pin GPIO_PIN_15
#define D12_GPIO_Port GPIOE
#define LCD_ON_OFF_Pin GPIO_PIN_2
#define LCD_ON_OFF_GPIO_Port GPIOA
#define OCSPI1_CLK_Pin GPIO_PIN_2
#define OCSPI1_CLK_GPIO_Port GPIOB
#define R3_Pin GPIO_PIN_2
#define R3_GPIO_Port GPIOJ
#define R4_Pin GPIO_PIN_3
#define R4_GPIO_Port GPIOJ
#define A6_Pin GPIO_PIN_12
#define A6_GPIO_Port GPIOF
#define A10_Pin GPIO_PIN_0
#define A10_GPIO_Port GPIOG
#define D5_Pin GPIO_PIN_8
#define D5_GPIO_Port GPIOE
#define D8_Pin GPIO_PIN_11
#define D8_GPIO_Port GPIOE
#define SDCKE1_Pin GPIO_PIN_7
#define SDCKE1_GPIO_Port GPIOH
/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
