/**
  ******************************************************************************
  * @file    usbd_composite.h
  * @author  MCD Application Team
  * @brief   Header file for the usbd_composite.c file.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2015 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under Ultimate Liberty license
  * SLA0044, the "License"; You may not use this file except in compliance with
  * the License. You may obtain a copy of the License at:
  *                      www.st.com/SLA0044
  *
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __USB_COMPOSITE_H
#define __USB_COMPOSITE_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stdbool.h"
#include "usbd_ioreq.h"
#include "AL94.I-CUBE-USBD-COMPOSITE_conf.h"

/** @addtogroup STM32_USB_DEVICE_LIBRARY
  * @{
  */

/** @defgroup USBD_COMPOSITE
  * @brief This file is the header file for usbd_composite.c
  * @{
  */


/** @defgroup USBD_COMPOSITE_Exported_Defines
  * @{
  */

#define USBD_USE_HS                  _USBD_USE_HS
#define USBD_USE_CDC_ACM             _USBD_USE_CDC_ACM
#define USBD_CDC_ACM_COUNT           _USBD_CDC_ACM_COUNT
#define USBD_USE_CDC_RNDIS           _USBD_USE_CDC_RNDIS
#define USBD_USE_CDC_ECM             _USBD_USE_CDC_ECM
#define USBD_USE_HID_MOUSE           _USBD_USE_HID_MOUSE
#define USBD_USE_HID_KEYBOARD        _USBD_USE_HID_KEYBOARD
#define USBD_USE_HID_CUSTOM          _USBD_USE_HID_CUSTOM
#define USBD_USE_UAC_MIC             _USBD_USE_UAC_MIC
#define USBD_USE_UAC_SPKR            _USBD_USE_UAC_SPKR
#define USBD_USE_UVC                 _USBD_USE_UVC
#define USBD_USE_MSC                 _USBD_USE_MSC
#define USBD_USE_DFU                 _USBD_USE_DFU
#define USBD_USE_PRNTR               _USBD_USE_PRNTR

#define STM32F1_DEVICE               _STM32F1_DEVICE

#if(USBD_USE_CDC_ACM == 1)
#include "usbd_cdc_acm_if.h"
#endif
#if(USBD_USE_CDC_RNDIS == 1)
#include "usbd_cdc_rndis_if.h"
#endif
#if(USBD_USE_CDC_ECM == 1)
#include "usbd_cdc_ecm_if.h"
#endif
#if(USBD_USE_HID_MOUSE == 1)
#include "usbd_hid_mouse.h"
#endif
#if(USBD_USE_HID_KEYBOARD == 1)
#include "usbd_hid_keyboard.h"
#endif
#if(USBD_USE_HID_CUSTOM == 1)
#include "usbd_hid_custom_if.h"
#endif
#if(USBD_USE_UAC_MIC == 1)
#include "usbd_audio_mic_if.h"
#endif
#if(USBD_USE_UAC_SPKR == 1)
#include "usbd_audio_spkr_if.h"
#endif
#if(USBD_USE_UVC == 1)
#include "usbd_video_if.h"
#endif
#if(USBD_USE_MSC == 1)
#include "usbd_msc_if.h"
#endif
#if(USBD_USE_DFU == 1)
#include "usbd_dfu_if.h"
#endif
#if(USBD_USE_PRNTR == 1)
#include "usbd_printer_if.h"
#endif

/**
  * @}
  */


/** @defgroup USBD_COMPOSITE_Exported_TypesDefinitions
  * @{
  */

/**
  * @}
  */



/** @defgroup USBD_COMPOSITE_Exported_Macros
  * @{
  */

/**
  * @}
  */

/** @defgroup USBD_COMPOSITE_Exported_Variables
  * @{
  */

extern USBD_ClassTypeDef USBD_COMPOSITE;
/**
  * @}
  */

/** @defgroup USB_CORE_Exported_Functions
  * @{
  */
void USBD_COMPOSITE_Mount_Class(void);
/**
  * @}
  */

#ifdef __cplusplus
}
#endif

#endif  /* __USB_COMPOSITE_H */
/**
  * @}
  */

/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
