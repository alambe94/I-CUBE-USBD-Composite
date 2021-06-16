/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : usb_device.c
  * @version        : v1.0_Cube
  * @brief          : This file implements the USB Device
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
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/

#include "usb_device.h"
#include "usbd_desc.h"

/* USER CODE BEGIN Includes */
#include "usbd_composite.h"
/* USER CODE END Includes */

/* USER CODE BEGIN PV */
/* Private variables ---------------------------------------------------------*/

/* USER CODE END PV */

/* USER CODE BEGIN PFP */
/* Private function prototypes -----------------------------------------------*/

/* USER CODE END PFP */

/* USB Device Core handle declaration. */
USBD_HandleTypeDef hUsbDeviceHS;

/*
 * -- Insert your variables declaration here --
 */
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/*
 * -- Insert your external function declaration here --
 */
/* USER CODE BEGIN 1 */

/* USER CODE END 1 */

/**
  * Init USB device Library, add supported class and start the library
  * @retval None
  */
void MX_USB_DEVICE_Init(void)
{
  /* USER CODE BEGIN USB_DEVICE_Init_PreTreatment */

  /* USER CODE END USB_DEVICE_Init_PreTreatment */

  /* Init Device Library, add supported class and start the library. */
  USBD_COMPOSITE_Mount_Class();

  if (USBD_Init(&hUsbDeviceHS, &HS_Desc, DEVICE_HS) != USBD_OK)
  {
    Error_Handler();
  }
  if (USBD_RegisterClass(&hUsbDeviceHS, &USBD_COMPOSITE) != USBD_OK)
  {
    Error_Handler();
  }
#if (USBD_USE_CDC_ACM == 1)
  if (USBD_CDC_ACM_RegisterInterface(&hUsbDeviceHS, &USBD_CDC_ACM_fops) != USBD_OK)
  {
    Error_Handler();
  }
#endif
#if (USBD_USE_CDC_RNDIS == 1)
  if (USBD_CDC_RNDIS_RegisterInterface(&hUsbDeviceHS, &USBD_CDC_RNDIS_fops) != USBD_OK)
  {
    Error_Handler();
  }
#endif
#if (USBD_USE_CDC_ECM == 1)
  if (USBD_CDC_ECM_RegisterInterface(&hUsbDeviceHS, &USBD_CDC_ECM_fops) != USBD_OK)
  {
    Error_Handler();
  }
#endif
#if (USBD_USE_HID_MOUSE == 1)
#endif
#if (USBD_USE_HID_KEYBOARD == 1)
#endif
#if (USBD_USE_HID_CUSTOM == 1)
  if (USBD_CUSTOM_HID_RegisterInterface(&hUsbDeviceHS, &USBD_CustomHID_fops) != USBD_OK)
  {
    Error_Handler();
  }
#endif
#if (USBD_USE_UAC_MIC == 1)
  if (USBD_AUDIO_MIC_RegisterInterface(&hUsbDeviceHS, &USBD_AUDIO_MIC_fops_FS) != USBD_OK)
  {
    Error_Handler();
  }
#endif
#if (USBD_USE_UAC_SPKR == 1)
  if (USBD_AUDIO_SPKR_RegisterInterface(&hUsbDeviceHS, &USBD_AUDIO_SPKR_fops) != USBD_OK)
  {
    Error_Handler();
  }
#endif
#if (USBD_USE_UVC == 1)
  if (USBD_VIDEO_RegisterInterface(&hUsbDeviceHS, &USBD_VIDEO_fops_FS) != USBD_OK)
  {
    Error_Handler();
  }
#endif
#if (USBD_USE_MSC == 1)
  if (USBD_MSC_RegisterStorage(&hUsbDeviceHS, &USBD_Storage_Interface_fops) != USBD_OK)
  {
    Error_Handler();
  }
#endif
#if (USBD_USE_DFU == 1)
  if (USBD_DFU_RegisterMedia(&hUsbDeviceHS, &USBD_DFU_fops) != USBD_OK)
  {
    Error_Handler();
  }
#endif
#if (USBD_USE_PRNTR == 1)
  if (USBD_PRNT_RegisterInterface(&hUsbDeviceHS, &USBD_PRNT_fops) != USBD_OK)
  {
    Error_Handler();
  }
#endif
  if (USBD_Start(&hUsbDeviceHS) != USBD_OK)
  {
    Error_Handler();
  }

  /* USER CODE BEGIN USB_DEVICE_Init_PostTreatment */

  /* USER CODE END USB_DEVICE_Init_PostTreatment */
}

/**
  * @}
  */

/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
