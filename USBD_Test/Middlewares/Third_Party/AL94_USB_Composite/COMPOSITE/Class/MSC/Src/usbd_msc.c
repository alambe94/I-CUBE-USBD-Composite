/**
  ******************************************************************************
  * @file    usbd_msc.c
  * @author  MCD Application Team
  * @brief   This file provides all the MSC core functions.
  *
  * @verbatim
  *
  *          ===================================================================
  *                                MSC Class  Description
  *          ===================================================================
  *           This module manages the MSC class V1.0 following the "Universal
  *           Serial Bus Mass Storage Class (MSC) Bulk-Only Transport (BOT) Version 1.0
  *           Sep. 31, 1999".
  *           This driver implements the following aspects of the specification:
  *             - Bulk-Only Transport protocol
  *             - Subclass : SCSI transparent command set (ref. SCSI Primary Commands - 3 (SPC-3))
  *
  *  @endverbatim
  *
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

/* BSPDependencies
- "stm32xxxxx_{eval}{discovery}{nucleo_144}.c"
- "stm32xxxxx_{eval}{discovery}_io.c"
- "stm32xxxxx_{eval}{discovery}{adafruit}_sd.c"
EndBSPDependencies */

/* Includes ------------------------------------------------------------------*/
#include "usbd_msc.h"

#define _MSC_IN_EP 0x81U
#define _MSC_OUT_EP 0x01U
#define _MSC_ITF_NBR 0x00
#define _MSC_BOT_STR_DESC_IDX 0x00U

uint8_t MSC_IN_EP = _MSC_IN_EP;
uint8_t MSC_OUT_EP = _MSC_OUT_EP;
uint8_t MSC_ITF_NBR = _MSC_ITF_NBR;
uint8_t MSC_BOT_STR_DESC_IDX = _MSC_BOT_STR_DESC_IDX;

/** @addtogroup STM32_USB_DEVICE_LIBRARY
  * @{
  */

/** @defgroup MSC_CORE
  * @brief Mass storage core module
  * @{
  */

/** @defgroup MSC_CORE_Private_TypesDefinitions
  * @{
  */
/**
  * @}
  */

/** @defgroup MSC_CORE_Private_Defines
  * @{
  */

/**
  * @}
  */

/** @defgroup MSC_CORE_Private_Macros
  * @{
  */
/**
  * @}
  */

/** @defgroup MSC_CORE_Private_FunctionPrototypes
  * @{
  */
uint8_t USBD_MSC_Init(USBD_HandleTypeDef *pdev, uint8_t cfgidx);
uint8_t USBD_MSC_DeInit(USBD_HandleTypeDef *pdev, uint8_t cfgidx);
uint8_t USBD_MSC_Setup(USBD_HandleTypeDef *pdev, USBD_SetupReqTypedef *req);
uint8_t USBD_MSC_DataIn(USBD_HandleTypeDef *pdev, uint8_t epnum);
uint8_t USBD_MSC_DataOut(USBD_HandleTypeDef *pdev, uint8_t epnum);

uint8_t *USBD_MSC_GetHSCfgDesc(uint16_t *length);
uint8_t *USBD_MSC_GetFSCfgDesc(uint16_t *length);
uint8_t *USBD_MSC_GetOtherSpeedCfgDesc(uint16_t *length);
uint8_t *USBD_MSC_GetDeviceQualifierDescriptor(uint16_t *length);

/**
  * @}
  */

/** @defgroup MSC_CORE_Private_Variables
  * @{
  */

static USBD_MSC_BOT_HandleTypeDef USBD_MSC_Instance;

USBD_ClassTypeDef USBD_MSC =
    {
        USBD_MSC_Init,
        USBD_MSC_DeInit,
        USBD_MSC_Setup,
        NULL, /*EP0_TxSent*/
        NULL, /*EP0_RxReady*/
        USBD_MSC_DataIn,
        USBD_MSC_DataOut,
        NULL, /*SOF */
        NULL,
        NULL,
        USBD_MSC_GetHSCfgDesc,
        USBD_MSC_GetFSCfgDesc,
        USBD_MSC_GetOtherSpeedCfgDesc,
        USBD_MSC_GetDeviceQualifierDescriptor,
};

/* USB Mass storage device Configuration Descriptor */
/*   All Descriptors (Configuration, Interface, Endpoint, Class, Vendor */
__ALIGN_BEGIN static uint8_t USBD_MSC_CfgHSDesc[USB_MSC_CONFIG_DESC_SIZ] __ALIGN_END =
    {
        0x09,                        /* bLength: Configuration Descriptor size */
        USB_DESC_TYPE_CONFIGURATION, /* bDescriptorType: Configuration */
        USB_MSC_CONFIG_DESC_SIZ,

        0x00,
        0x01, /* bNumInterfaces: 1 interface */
        0x01, /* bConfigurationValue */
        0x04, /* iConfiguration */
#if (USBD_SELF_POWERED == 1U)
        0xC0, /* bmAttributes: Bus Powered according to user configuration */
#else
        0x80, /* bmAttributes: Bus Powered according to user configuration */
#endif
        USBD_MAX_POWER, /* MaxPower (mA) */

        /********************  Mass Storage interface ********************/
        0x09,                  /* bLength: Interface Descriptor size */
        0x04,                  /* bDescriptorType: */
        _MSC_ITF_NBR,          /* bInterfaceNumber: Number of Interface */
        0x00,                  /* bAlternateSetting: Alternate setting */
        0x02,                  /* bNumEndpoints */
        0x08,                  /* bInterfaceClass: MSC Class */
        0x06,                  /* bInterfaceSubClass : SCSI transparent */
        0x50,                  /* nInterfaceProtocol */
        _MSC_BOT_STR_DESC_IDX, /* iInterface */
        /********************  Mass Storage Endpoints ********************/
        0x07,       /* Endpoint descriptor length = 7 */
        0x05,       /* Endpoint descriptor type */
        _MSC_IN_EP, /* Endpoint address (IN, address 1) */
        0x02,       /* Bulk endpoint type */
        LOBYTE(MSC_MAX_HS_PACKET),
        HIBYTE(MSC_MAX_HS_PACKET),
        0x00, /* Polling interval in milliseconds */

        0x07,        /* Endpoint descriptor length = 7 */
        0x05,        /* Endpoint descriptor type */
        _MSC_OUT_EP, /* Endpoint address (OUT, address 1) */
        0x02,        /* Bulk endpoint type */
        LOBYTE(MSC_MAX_HS_PACKET),
        HIBYTE(MSC_MAX_HS_PACKET),
        0x00 /* Polling interval in milliseconds */
};

/* USB Mass storage device Configuration Descriptor */
/* All Descriptors (Configuration, Interface, Endpoint, Class, Vendor */
__ALIGN_BEGIN static uint8_t USBD_MSC_CfgFSDesc[USB_MSC_CONFIG_DESC_SIZ] __ALIGN_END =
    {
        0x09,                        /* bLength: Configuration Descriptor size */
        USB_DESC_TYPE_CONFIGURATION, /* bDescriptorType: Configuration */
        USB_MSC_CONFIG_DESC_SIZ,

        0x00,
        0x01, /* bNumInterfaces: 1 interface */
        0x01, /* bConfigurationValue */
        0x04, /* iConfiguration */
#if (USBD_SELF_POWERED == 1U)
        0xC0, /* bmAttributes: Bus Powered according to user configuration */
#else
        0x80, /* bmAttributes: Bus Powered according to user configuration */
#endif
        USBD_MAX_POWER, /* MaxPower (mA) */

        /********************  Mass Storage interface ********************/
        0x09,                  /* bLength: Interface Descriptor size */
        0x04,                  /* bDescriptorType: */
        _MSC_ITF_NBR,          /* bInterfaceNumber: Number of Interface */
        0x00,                  /* bAlternateSetting: Alternate setting */
        0x02,                  /* bNumEndpoints*/
        0x08,                  /* bInterfaceClass: MSC Class */
        0x06,                  /* bInterfaceSubClass : SCSI transparent*/
        0x50,                  /* nInterfaceProtocol */
        _MSC_BOT_STR_DESC_IDX, /* iInterface: */
        /********************  Mass Storage Endpoints ********************/
        0x07,       /* Endpoint descriptor length = 7 */
        0x05,       /* Endpoint descriptor type */
        _MSC_IN_EP, /* Endpoint address (IN, address 1) */
        0x02,       /* Bulk endpoint type */
        LOBYTE(MSC_MAX_FS_PACKET),
        HIBYTE(MSC_MAX_FS_PACKET),
        0x00, /* Polling interval in milliseconds */

        0x07,        /* Endpoint descriptor length = 7 */
        0x05,        /* Endpoint descriptor type */
        _MSC_OUT_EP, /* Endpoint address (OUT, address 1) */
        0x02,        /* Bulk endpoint type */
        LOBYTE(MSC_MAX_FS_PACKET),
        HIBYTE(MSC_MAX_FS_PACKET),
        0x00 /* Polling interval in milliseconds */
};

__ALIGN_BEGIN static uint8_t USBD_MSC_OtherSpeedCfgDesc[USB_MSC_CONFIG_DESC_SIZ] __ALIGN_END =
    {
        0x09, /* bLength: Configuration Descriptor size */
        USB_DESC_TYPE_OTHER_SPEED_CONFIGURATION,
        USB_MSC_CONFIG_DESC_SIZ,

        0x00,
        0x01, /* bNumInterfaces: 1 interface */
        0x01, /* bConfigurationValue */
        0x04, /* iConfiguration */
#if (USBD_SELF_POWERED == 1U)
        0xC0, /* bmAttributes: Bus Powered according to user configuration */
#else
        0x80, /* bmAttributes: Bus Powered according to user configuration */
#endif
        USBD_MAX_POWER, /* MaxPower (mA) */

        /********************  Mass Storage interface ********************/
        0x09,                  /* bLength: Interface Descriptor size */
        0x04,                  /* bDescriptorType: */
        _MSC_ITF_NBR,          /* bInterfaceNumber: Number of Interface */
        0x00,                  /* bAlternateSetting: Alternate setting */
        0x02,                  /* bNumEndpoints */
        0x08,                  /* bInterfaceClass: MSC Class */
        0x06,                  /* bInterfaceSubClass : SCSI transparent command set */
        0x50,                  /* nInterfaceProtocol */
        _MSC_BOT_STR_DESC_IDX, /* iInterface */
        /********************  Mass Storage Endpoints ********************/
        0x07,       /* Endpoint descriptor length = 7 */
        0x05,       /* Endpoint descriptor type */
        _MSC_IN_EP, /* Endpoint address (IN, address 1) */
        0x02,       /* Bulk endpoint type */
        0x40,
        0x00,
        0x00, /* Polling interval in milliseconds */

        0x07,        /* Endpoint descriptor length = 7 */
        0x05,        /* Endpoint descriptor type */
        _MSC_OUT_EP, /* Endpoint address (OUT, address 1) */
        0x02,        /* Bulk endpoint type */
        0x40,
        0x00,
        0x00 /* Polling interval in milliseconds */
};

/* USB Standard Device Descriptor */
__ALIGN_BEGIN static uint8_t USBD_MSC_DeviceQualifierDesc[USB_LEN_DEV_QUALIFIER_DESC] __ALIGN_END =
    {
        USB_LEN_DEV_QUALIFIER_DESC,
        USB_DESC_TYPE_DEVICE_QUALIFIER,
        0x00,
        0x02,
        0x00,
        0x00,
        0x00,
        MSC_MAX_FS_PACKET,
        0x01,
        0x00,
};
/**
  * @}
  */

/** @defgroup MSC_CORE_Private_Functions
  * @{
  */

/**
  * @brief  USBD_MSC_Init
  *         Initialize  the mass storage configuration
  * @param  pdev: device instance
  * @param  cfgidx: configuration index
  * @retval status
  */
uint8_t USBD_MSC_Init(USBD_HandleTypeDef *pdev, uint8_t cfgidx)
{
  UNUSED(cfgidx);
  USBD_MSC_BOT_HandleTypeDef *hmsc;

  hmsc = &USBD_MSC_Instance;

  if (hmsc == NULL)
  {
    pdev->pClassData_MSC = NULL;
    return (uint8_t)USBD_EMEM;
  }

  pdev->pClassData_MSC = (void *)hmsc;

  if (pdev->dev_speed == USBD_SPEED_HIGH)
  {
    /* Open EP OUT */
    (void)USBD_LL_OpenEP(pdev, MSC_OUT_EP, USBD_EP_TYPE_BULK, MSC_MAX_HS_PACKET);
    pdev->ep_out[MSC_OUT_EP & 0xFU].is_used = 1U;

    /* Open EP IN */
    (void)USBD_LL_OpenEP(pdev, MSC_IN_EP, USBD_EP_TYPE_BULK, MSC_MAX_HS_PACKET);
    pdev->ep_in[MSC_IN_EP & 0xFU].is_used = 1U;
  }
  else
  {
    /* Open EP OUT */
    (void)USBD_LL_OpenEP(pdev, MSC_OUT_EP, USBD_EP_TYPE_BULK, MSC_MAX_FS_PACKET);
    pdev->ep_out[MSC_OUT_EP & 0xFU].is_used = 1U;

    /* Open EP IN */
    (void)USBD_LL_OpenEP(pdev, MSC_IN_EP, USBD_EP_TYPE_BULK, MSC_MAX_FS_PACKET);
    pdev->ep_in[MSC_IN_EP & 0xFU].is_used = 1U;
  }

  /* Init the BOT  layer */
  MSC_BOT_Init(pdev);

  return (uint8_t)USBD_OK;
}

/**
  * @brief  USBD_MSC_DeInit
  *         DeInitialize  the mass storage configuration
  * @param  pdev: device instance
  * @param  cfgidx: configuration index
  * @retval status
  */
uint8_t USBD_MSC_DeInit(USBD_HandleTypeDef *pdev, uint8_t cfgidx)
{
  UNUSED(cfgidx);

  /* Close MSC EPs */
  (void)USBD_LL_CloseEP(pdev, MSC_OUT_EP);
  pdev->ep_out[MSC_OUT_EP & 0xFU].is_used = 0U;

  /* Close EP IN */
  (void)USBD_LL_CloseEP(pdev, MSC_IN_EP);
  pdev->ep_in[MSC_IN_EP & 0xFU].is_used = 0U;

  /* Free MSC Class Resources */
  if (pdev->pClassData_MSC != NULL)
  {
    /* De-Init the BOT layer */
    MSC_BOT_DeInit(pdev);
#if (0)
    (void)USBD_free(pdev->pClassData_MSC);
#endif
    pdev->pClassData_MSC = NULL;
  }

  return (uint8_t)USBD_OK;
}
/**
  * @brief  USBD_MSC_Setup
  *         Handle the MSC specific requests
  * @param  pdev: device instance
  * @param  req: USB request
  * @retval status
  */
uint8_t USBD_MSC_Setup(USBD_HandleTypeDef *pdev, USBD_SetupReqTypedef *req)
{
  USBD_MSC_BOT_HandleTypeDef *hmsc = (USBD_MSC_BOT_HandleTypeDef *)pdev->pClassData_MSC;
  USBD_StatusTypeDef ret = USBD_OK;
  uint16_t status_info = 0U;

  if (hmsc == NULL)
  {
    return (uint8_t)USBD_FAIL;
  }

  switch (req->bmRequest & USB_REQ_TYPE_MASK)
  {
  /* Class request */
  case USB_REQ_TYPE_CLASS:
    switch (req->bRequest)
    {
    case BOT_GET_MAX_LUN:
      if ((req->wValue == 0U) && (req->wLength == 1U) &&
          ((req->bmRequest & 0x80U) == 0x80U))
      {
        hmsc->max_lun = (uint32_t)((USBD_StorageTypeDef *)pdev->pUserData_MSC)->GetMaxLun();
        (void)USBD_CtlSendData(pdev, (uint8_t *)&hmsc->max_lun, 1U);
      }
      else
      {
        USBD_CtlError(pdev, req);
        ret = USBD_FAIL;
      }
      break;

    case BOT_RESET:
      if ((req->wValue == 0U) && (req->wLength == 0U) &&
          ((req->bmRequest & 0x80U) != 0x80U))
      {
        MSC_BOT_Reset(pdev);
      }
      else
      {
        USBD_CtlError(pdev, req);
        ret = USBD_FAIL;
      }
      break;

    default:
      USBD_CtlError(pdev, req);
      ret = USBD_FAIL;
      break;
    }
    break;
  /* Interface & Endpoint request */
  case USB_REQ_TYPE_STANDARD:
    switch (req->bRequest)
    {
    case USB_REQ_GET_STATUS:
      if (pdev->dev_state == USBD_STATE_CONFIGURED)
      {
        (void)USBD_CtlSendData(pdev, (uint8_t *)&status_info, 2U);
      }
      else
      {
        USBD_CtlError(pdev, req);
        ret = USBD_FAIL;
      }
      break;

    case USB_REQ_GET_INTERFACE:
      if (pdev->dev_state == USBD_STATE_CONFIGURED)
      {
        (void)USBD_CtlSendData(pdev, (uint8_t *)&hmsc->interface, 1U);
      }
      else
      {
        USBD_CtlError(pdev, req);
        ret = USBD_FAIL;
      }
      break;

    case USB_REQ_SET_INTERFACE:
      if (pdev->dev_state == USBD_STATE_CONFIGURED)
      {
        hmsc->interface = (uint8_t)(req->wValue);
      }
      else
      {
        USBD_CtlError(pdev, req);
        ret = USBD_FAIL;
      }
      break;

    case USB_REQ_CLEAR_FEATURE:
      if (pdev->dev_state == USBD_STATE_CONFIGURED)
      {
        if (req->wValue == USB_FEATURE_EP_HALT)
        {
          /* Flush the FIFO */
          (void)USBD_LL_FlushEP(pdev, (uint8_t)req->wIndex);

          /* Handle BOT error */
          MSC_BOT_CplClrFeature(pdev, (uint8_t)req->wIndex);
        }
      }
      break;

    default:
      USBD_CtlError(pdev, req);
      ret = USBD_FAIL;
      break;
    }
    break;

  default:
    USBD_CtlError(pdev, req);
    ret = USBD_FAIL;
    break;
  }

  return (uint8_t)ret;
}

/**
  * @brief  USBD_MSC_DataIn
  *         handle data IN Stage
  * @param  pdev: device instance
  * @param  epnum: endpoint index
  * @retval status
  */
uint8_t USBD_MSC_DataIn(USBD_HandleTypeDef *pdev, uint8_t epnum)
{
  MSC_BOT_DataIn(pdev, epnum);

  return (uint8_t)USBD_OK;
}

/**
  * @brief  USBD_MSC_DataOut
  *         handle data OUT Stage
  * @param  pdev: device instance
  * @param  epnum: endpoint index
  * @retval status
  */
uint8_t USBD_MSC_DataOut(USBD_HandleTypeDef *pdev, uint8_t epnum)
{
  MSC_BOT_DataOut(pdev, epnum);

  return (uint8_t)USBD_OK;
}

/**
  * @brief  USBD_MSC_GetHSCfgDesc
  *         return configuration descriptor
  * @param  length : pointer data length
  * @retval pointer to descriptor buffer
  */
uint8_t *USBD_MSC_GetHSCfgDesc(uint16_t *length)
{
  *length = (uint16_t)sizeof(USBD_MSC_CfgHSDesc);

  return USBD_MSC_CfgHSDesc;
}

/**
  * @brief  USBD_MSC_GetFSCfgDesc
  *         return configuration descriptor
  * @param  length : pointer data length
  * @retval pointer to descriptor buffer
  */
uint8_t *USBD_MSC_GetFSCfgDesc(uint16_t *length)
{
  *length = (uint16_t)sizeof(USBD_MSC_CfgFSDesc);

  return USBD_MSC_CfgFSDesc;
}

/**
  * @brief  USBD_MSC_GetOtherSpeedCfgDesc
  *         return other speed configuration descriptor
  * @param  length : pointer data length
  * @retval pointer to descriptor buffer
  */
uint8_t *USBD_MSC_GetOtherSpeedCfgDesc(uint16_t *length)
{
  *length = (uint16_t)sizeof(USBD_MSC_OtherSpeedCfgDesc);

  return USBD_MSC_OtherSpeedCfgDesc;
}
/**
  * @brief  DeviceQualifierDescriptor
  *         return Device Qualifier descriptor
  * @param  length : pointer data length
  * @retval pointer to descriptor buffer
  */
uint8_t *USBD_MSC_GetDeviceQualifierDescriptor(uint16_t *length)
{
  *length = (uint16_t)sizeof(USBD_MSC_DeviceQualifierDesc);

  return USBD_MSC_DeviceQualifierDesc;
}

/**
  * @brief  USBD_MSC_RegisterStorage
  * @param  fops: storage callback
  * @retval status
  */
uint8_t USBD_MSC_RegisterStorage(USBD_HandleTypeDef *pdev, USBD_StorageTypeDef *fops)
{
  if (fops == NULL)
  {
    return (uint8_t)USBD_FAIL;
  }

  pdev->pUserData_MSC = fops;

  return (uint8_t)USBD_OK;
}

void USBD_Update_MSC_DESC(uint8_t *desc, uint8_t itf_no, uint8_t in_ep, uint8_t out_ep, uint8_t str_idx)
{
  desc[11] = itf_no;
  desc[17] = str_idx;
  desc[20] = in_ep;
  desc[27] = out_ep;

  MSC_IN_EP = in_ep;
  MSC_OUT_EP = out_ep;
  MSC_ITF_NBR = itf_no;
  MSC_BOT_STR_DESC_IDX = str_idx;
}

/**
  * @}
  */

/**
  * @}
  */

/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
