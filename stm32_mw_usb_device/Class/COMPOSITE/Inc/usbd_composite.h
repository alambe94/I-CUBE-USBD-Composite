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
#include  "usbd_ioreq.h"

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

#define USBD_USE_CDC_ACM                                            1
#define USBD_USE_CDC_RNDIS                                          1
#define USBD_USE_CDC_ECM                                            1

#define USBD_USE_HID_MOUSE                                          1
#define USBD_USE_HID_KEYBOARD                                       1
#define USBD_USE_HID_CUSTOM                                         1

#define USBD_USE_UVC                                                1

#define USBD_USE_MSC                                                1

#define USBD_USE_UAC_SPKR                                           1
#define USBD_USE_UAC_MIC                                            1

#define USBD_USE_DFU                                                1


#define COMPOSITE_CDC_ACM_CMD_EP                                    0x81
#define COMPOSITE_CDC_ACM_IN_EP                                     0x82
#define COMPOSITE_CDC_ACM_OUT_EP                                    0x01

#define COMPOSITE_CDC_RNDIS_IN_EP                                   0x81U
#define COMPOSITE_CDC_RNDIS_OUT_EP                                  0x01U
#define COMPOSITE_CDC_RNDIS_CMD_EP                                  0x82U

#define COMPOSITE_CDC_ECM_IN_EP                                     0x81U
#define COMPOSITE_CDC_ECM_OUT_EP                                    0x01U
#define COMPOSITE_CDC_ECM_CMD_EP                                    0x82U

#define COMPOSITE_HID_MOUSE_IN_EP                                   0x81U
#define COMPOSITE_HID_KEYBOARD_IN_EP                                0x81U
#define COMPOSITE_HID_CUSTOM_IN_EP                                  0x81U
#define COMPOSITE_HID_CUSTOM_OUT_EP                                 0x01U

#define COMPOSITE_UVC_IN_EP                                         0x83U

#define COMPOSITE_MSC_IN_EP                                         0x84U
#define COMPOSITE_MSC_OUT_EP                                        0x02U

#define COMPOSITE_UAC_SPKR_OUT_EP                                   0x02U

#define COMPOSITE_UAC_MIC_IN_EP                                     0x84U

/////////////////////////////////// Interfaces /////////////////////////
#define COMPOSITE_CDC_ACM_CMD_ITF_NBR                               0x00
#define COMPOSITE_CDC_ACM_COM_ITF_NBR                               0x01

#define COMPOSITE_CDC_RNDIS_CMD_ITF_NBR                             0x00U
#define COMPOSITE_CDC_RNDIS_COM_ITF_NBR                             0x01U

#define COMPOSITE_CDC_ECM_CMD_ITF_NBR                               0x00U
#define COMPOSITE_CDC_ECM_COM_ITF_NBR                               0x01U

#define COMPOSITE_HID_MOUSE_ITF_NBR                                 0x00U

#define COMPOSITE_UVC_VC_IF_NUM                                     0x02U
#define COMPOSITE_UVC_VS_IF_NUM                                     0x03U

#define COMPOSITE_MSC_ITF_NBR                                       0x04U

#define COMPOSITE_HID_KEYBOARD_ITF_NBR                              0x04U

#define COMPOSITE_HID_CUSTOM_ITF_NBR                                0x04U

#define COMPOSITE_UAC_SPKR_AC_IF_NUM                                0x04U
#define COMPOSITE_UAC_SPKR_AS_IF_NUM                                0x04U

#define COMPOSITE_UAC_MIC_AC_IF_NUM                                 0x04U
#define COMPOSITE_UAC_MIC_AS_IF_NUM                                 0x04U

#define COMPOSITE_DFU_ITF_NBR                                       0x04U

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
