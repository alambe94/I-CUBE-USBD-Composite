/**
  ******************************************************************************
  * @file    usbd_hid.h
  * @author  MCD Application Team
  * @brief   Header file for the usbd_hid_core.c file.
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
#ifndef __USB_HID_KEYBAORD_H
#define __USB_HID_KEYBAORD_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include  "usbd_ioreq.h"

/** @addtogroup STM32_USB_DEVICE_LIBRARY
  * @{
  */

/** @defgroup USBD_HID
  * @brief This file is the Header file for usbd_hid.c
  * @{
  */


/** @defgroup USBD_HID_Exported_Defines
  * @{
  */

#define HID_KEYBOARD_STR_DESC                               "STM32 USB KEYBOARD"

#define HID_KEYBOARD_EPIN_SIZE                              0x08U

#define HID_KEYBOARD_CONFIG_DESC_SIZE                       34U
#define HID_KEYBOARD_DESC_SIZE                              9U

#define HID_KEYBOARD_REPORT_DESC_SIZE                       187U

#define HID_KEYBOARD_DESCRIPTOR_TYPE                        0x21U
#define HID_KEYBOARD_REPORT_DESC                            0x22U

#ifndef HID_KEYBOARD_HS_BINTERVAL
#define HID_KEYBOARD_HS_BINTERVAL                           0x07U
#endif /* HID_KEYBOARD_HS_BINTERVAL */

#ifndef HID_KEYBOARD_FS_BINTERVAL
#define HID_KEYBOARD_FS_BINTERVAL                           0x0AU
#endif /* HID_KEYBOARD_FS_BINTERVAL */

#define HID_KEYBOARD_REQ_SET_PROTOCOL                       0x0BU
#define HID_KEYBOARD_REQ_GET_PROTOCOL                       0x03U

#define HID_KEYBOARD_REQ_SET_IDLE                           0x0AU
#define HID_KEYBOARD_REQ_GET_IDLE                           0x02U

#define HID_KEYBOARD_REQ_SET_REPORT                         0x09U
#define HID_KEYBOARD_REQ_GET_REPORT                         0x01U
/**
  * @}
  */

/** @defgroup USBD_CORE_Exported_TypesDefinitions
  * @{
  */
typedef enum
{
  KEYBOARD_HID_IDLE = 0,
  KEYBOARD_HID_BUSY,
} HID_Keyboard_StateTypeDef;

typedef struct
{
  uint32_t Protocol;
  uint32_t IdleState;
  uint32_t AltSetting;
  HID_Keyboard_StateTypeDef state;
} USBD_HID_Keyboard_HandleTypeDef;
/**
  * @}
  */

/** @defgroup USBD_CORE_Exported_Macros
  * @{
  */

/**
  * @}
  */

/** @defgroup USBD_CORE_Exported_Variables
  * @{
  */

extern USBD_ClassTypeDef USBD_HID_KEYBOARD;

extern uint8_t HID_KEYBOARD_IN_EP;
extern uint8_t HID_KEYBOARD_ITF_NBR;
extern uint8_t HID_KEYBOARD_STR_DESC_IDX;

/**
  * @}
  */

/** @defgroup USB_CORE_Exported_Functions
  * @{
  */
uint8_t USBD_HID_Keybaord_SendReport(USBD_HandleTypeDef *pdev, uint8_t *report, uint16_t len);
uint32_t USBD_HID_Keyboard_GetPollingInterval(USBD_HandleTypeDef *pdev);

void USBD_Update_HID_KBD_DESC(uint8_t *desc, uint8_t itf_no, uint8_t in_ep, uint8_t str_idx);

/**
  * @}
  */

#ifdef __cplusplus
}
#endif

#endif /* __USB_HID_KEYBAORD_H */
/**
  * @}
  */

/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
