/**
  ******************************************************************************
  * @file    usbd_template_core.h
  * @author  MCD Application Team
  * @brief   Header file for the usbd_template_core.c file.
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
#ifndef __USB_COMPOSITE_CORE_H
#define __USB_COMPOSITE_CORE_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include  "usbd_ioreq.h"

/** @addtogroup STM32_USB_DEVICE_LIBRARY
  * @{
  */

/** @defgroup USBD_COMPOSITE
  * @brief This file is the header file for usbd_template_core.c
  * @{
  */


/** @defgroup USBD_COMPOSITE_Exported_Defines
  * @{
  */
#define COMPOSITE_CDC_RNDIS_IN_EP                                   0x81U  /* EP1 for data IN */
#define COMPOSITE_CDC_RNDIS_OUT_EP                                  0x01U  /* EP1 for data OUT */
#define COMPOSITE_CDC_RNDIS_CMD_EP                                  0x82U  /* EP2 for CDC_RNDIS commands */
#define COMPOSITE_UVC_IN_EP                                         0x83U
#define COMPOSITE_MSC_EPIN_ADDR                                     0x84U
#define COMPOSITE_MSC_EPOUT_ADDR                                    0x02U

#define COMPOSITE_CDC_RNDIS_CMD_ITF_NBR                             0x00U /* Command Interface Number 0 */
#define COMPOSITE_CDC_RNDIS_COM_ITF_NBR                             0x01U /* Communication Interface Number 0 */
#define COMPOSITE_UVC_VC_IF_NUM                                     0x02U
#define COMPOSITE_UVC_VS_IF_NUM                                     0x03U
#define COMPOSITE_MSC_ITF_NBR                                       0x04U

#define USB_COMPOSITE_CONFIG_DESC_SIZ                               (UVC_CONFIG_DESC_SIZ + CDC_RNDIS_CONFIG_DESC_SIZ + USB_MSC_CONFIG_DESC_SIZ - 0x09)

/**
  * @}
  */


/** @defgroup USBD_CORE_Exported_TypesDefinitions
  * @{
  */

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

#endif  /* __USB_COMPOSITE_CORE_H */
/**
  * @}
  */

/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
