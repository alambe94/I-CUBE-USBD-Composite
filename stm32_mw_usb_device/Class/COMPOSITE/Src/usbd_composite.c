/**
  ******************************************************************************
  * @file    usbd_composite.c
  * @author  MCD Application Team
  * @brief   This file provides the HID core functions.
  *
  * @verbatim
  *
  *          ===================================================================
  *                                COMPOSITE Class  Description
  *          ===================================================================
  *
  *
  *
  *
  *
  *
  * @note     In HS mode and when the DMA is used, all variables and data structures
  *           dealing with the DMA during the transaction process should be 32-bit aligned.
  *
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

/* Includes ------------------------------------------------------------------*/
#include "usbd_composite.h"
#include "usbd_ctlreq.h"

/** @addtogroup STM32_USB_DEVICE_LIBRARY
  * @{
  */

/** @defgroup USBD_COMPOSITE
  * @brief usbd core module
  * @{
  */

/** @defgroup USBD_COMPOSITE_Private_TypesDefinitions
  * @{
  */
/**
  * @}
  */

/** @defgroup USBD_COMPOSITE_Private_Defines
  * @{
  */

/**
  * @}
  */

/** @defgroup USBD_COMPOSITE_Private_Macros
  * @{
  */

/**
  * @}
  */

/** @defgroup USBD_COMPOSITE_Private_FunctionPrototypes
  * @{
  */

static uint8_t USBD_COMPOSITE_Init(USBD_HandleTypeDef *pdev, uint8_t cfgidx);
static uint8_t USBD_COMPOSITE_DeInit(USBD_HandleTypeDef *pdev, uint8_t cfgidx);
static uint8_t USBD_COMPOSITE_Setup(USBD_HandleTypeDef *pdev, USBD_SetupReqTypedef *req);
static uint8_t USBD_COMPOSITE_DataIn(USBD_HandleTypeDef *pdev, uint8_t epnum);
static uint8_t USBD_COMPOSITE_DataOut(USBD_HandleTypeDef *pdev, uint8_t epnum);
static uint8_t USBD_COMPOSITE_EP0_RxReady(USBD_HandleTypeDef *pdev);
static uint8_t USBD_COMPOSITE_EP0_TxReady(USBD_HandleTypeDef *pdev);
static uint8_t USBD_COMPOSITE_SOF(USBD_HandleTypeDef *pdev);
static uint8_t USBD_COMPOSITE_IsoINIncomplete(USBD_HandleTypeDef *pdev, uint8_t epnum);
static uint8_t USBD_COMPOSITE_IsoOutIncomplete(USBD_HandleTypeDef *pdev, uint8_t epnum);

static uint8_t *USBD_COMPOSITE_GetHSCfgDesc(uint16_t *length);
static uint8_t *USBD_COMPOSITE_GetFSCfgDesc(uint16_t *length);
static uint8_t *USBD_COMPOSITE_GetOtherSpeedCfgDesc(uint16_t *length);
static uint8_t *USBD_COMPOSITE_GetDeviceQualifierDesc(uint16_t *length);
static uint8_t *USBD_COMPOSITE_GetUsrStringDesc(USBD_HandleTypeDef *pdev, uint8_t index, uint16_t *length);

/**
  * @}
  */

/** @defgroup USBD_COMPOSITE_Private_Variables
  * @{
  */

USBD_ClassTypeDef USBD_COMPOSITE =
    {
        USBD_COMPOSITE_Init,
        USBD_COMPOSITE_DeInit,
        USBD_COMPOSITE_Setup,
        USBD_COMPOSITE_EP0_TxReady,
        USBD_COMPOSITE_EP0_RxReady,
        USBD_COMPOSITE_DataIn,
        USBD_COMPOSITE_DataOut,
        USBD_COMPOSITE_SOF,
        USBD_COMPOSITE_IsoINIncomplete,
        USBD_COMPOSITE_IsoOutIncomplete,
        USBD_COMPOSITE_GetHSCfgDesc,
        USBD_COMPOSITE_GetFSCfgDesc,
        USBD_COMPOSITE_GetOtherSpeedCfgDesc,
        USBD_COMPOSITE_GetDeviceQualifierDesc,
        USBD_COMPOSITE_GetUsrStringDesc};

typedef struct USBD_COMPOSITE_CFG_DESC_t
{
  uint8_t CONFIG_DESC[USB_CONF_DESC_SIZE];
#if (USBD_USE_CDC_RNDIS == 1)
  uint8_t USBD_CDC_RNDIS_DESC[CDC_RNDIS_CONFIG_DESC_SIZE - 0x09];
#endif
#if (USBD_USE_CDC_ECM == 1)
  uint8_t USBD_CDC_ECM_DESC[CDC_ECM_CONFIG_DESC_SIZE - 0x09];
#endif
#if (USBD_USE_HID_MOUSE == 1)
  uint8_t USBD_HID_MOUSE_DESC[USB_HID_CONFIG_DESC_SIZ - 0x09];
#endif
#if (USBD_USE_HID_KEYBOARD == 1)
  uint8_t USBD_HID_KEYBOARD_DESC[HID_KEYBOARD_CONFIG_DESC_SIZE - 0x09];
#endif
#if (USBD_USE_HID_CUSTOM == 1)
  uint8_t USBD_HID_CUSTOM_DESC[USB_CUSTOM_HID_CONFIG_DESC_SIZ - 0x09];
#endif
#if (USBD_USE_UAC_MIC == 1)
  uint8_t USBD_UAC_MIC_DESC[USBD_AUDIO_MIC_CONFIG_DESC_SIZE - 0x09];
#endif
#if (USBD_USE_UAC_SPKR == 1)
  uint8_t USBD_AUDIO_SPKR_DESC[USBD_AUDIO_SPKR_CONFIG_DESC_SIZE - 0x09];
#endif
#if (USBD_USE_UVC == 1)
  uint8_t USBD_UVC_DESC[UVC_CONFIG_DESC_SIZE - 0x09];
#endif
#if (USBD_USE_MSC == 1)
  uint8_t USBD_MSC_DESC[USB_MSC_CONFIG_DESC_SIZ - 0x09];
#endif
#if (USBD_USE_DFU == 1)
  uint8_t USBD_DFU_DESC[USB_DFU_CONFIG_DESC_SIZ - 0x09];
#endif
#if (USBD_USE_PRNTR == 1)
  uint8_t USBD_PRNTR_DESC[USB_PRNT_CONFIG_DESC_SIZE - 0x09];
#endif
#if (USBD_USE_CDC_ACM == 1)
  uint8_t USBD_CDC_ACM_DESC[USB_CDC_CONFIG_DESC_SIZ - 0x09];
#endif

} __PACKED USBD_COMPOSITE_CFG_DESC_t;

#if defined(__ICCARM__) /*!< IAR Compiler */
#pragma data_alignment = 4
#endif
__ALIGN_BEGIN USBD_COMPOSITE_CFG_DESC_t USBD_COMPOSITE_FSCfgDesc, USBD_COMPOSITE_HSCfgDesc __ALIGN_END;
uint8_t USBD_Track_String_Index = (USBD_IDX_INTERFACE_STR + 1);

#if defined(__ICCARM__) /*!< IAR Compiler */
#pragma data_alignment = 4
#endif
/* USB Standard Device Descriptor */
__ALIGN_BEGIN static uint8_t USBD_COMPOSITE_DeviceQualifierDesc[USB_LEN_DEV_QUALIFIER_DESC] __ALIGN_END =
    {
        USB_LEN_DEV_QUALIFIER_DESC,
        USB_DESC_TYPE_DEVICE_QUALIFIER,
        0x00,
        0x02,
        0xEF,
        0x02,
        0x01,
        0x40,
        0x01,
        0x00,
};

/**
  * @}
  */

/** @defgroup USBD_COMPOSITE_Private_Functions
  * @{
  */

/**
  * @brief  USBD_COMPOSITE_Init
  *         Initialize the COMPOSITE interface
  * @param  pdev: device instance
  * @param  cfgidx: Configuration index
  * @retval status
  */
static uint8_t USBD_COMPOSITE_Init(USBD_HandleTypeDef *pdev, uint8_t cfgidx)
{
#if (USBD_USE_CDC_ACM == 1)
  USBD_CDC_ACM.Init(pdev, cfgidx);
#endif
#if (USBD_USE_CDC_ECM == 1)
  USBD_CDC_ECM.Init(pdev, cfgidx);
#endif
#if (USBD_USE_CDC_RNDIS == 1)
  USBD_CDC_RNDIS.Init(pdev, cfgidx);
#endif
#if (USBD_USE_HID_MOUSE == 1)
  USBD_HID_MOUSE.Init(pdev, cfgidx);
#endif
#if (USBD_USE_HID_KEYBOARD == 1)
  USBD_HID_KEYBOARD.Init(pdev, cfgidx);
#endif
#if (USBD_USE_HID_CUSTOM == 1)
  USBD_HID_CUSTOM.Init(pdev, cfgidx);
#endif
#if (USBD_USE_UAC_MIC == 1)
  USBD_AUDIO_MIC.Init(pdev, cfgidx);
#endif
#if (USBD_USE_UAC_SPKR == 1)
  USBD_AUDIO_SPKR.Init(pdev, cfgidx);
#endif
#if (USBD_USE_UVC == 1)
  USBD_VIDEO.Init(pdev, cfgidx);
#endif
#if (USBD_USE_MSC == 1)
  USBD_MSC.Init(pdev, cfgidx);
#endif
#if (USBD_USE_DFU == 1)
  USBD_DFU.Init(pdev, cfgidx);
#endif
#if (USBD_USE_PRNTR == 1)
  USBD_PRNT.Init(pdev, cfgidx);
#endif

  return (uint8_t)USBD_OK;
}

/**
  * @brief  USBD_COMPOSITE_Init
  *         DeInitialize the COMPOSITE layer
  * @param  pdev: device instance
  * @param  cfgidx: Configuration index
  * @retval status
  */
static uint8_t USBD_COMPOSITE_DeInit(USBD_HandleTypeDef *pdev, uint8_t cfgidx)
{
#if (USBD_USE_CDC_ACM == 1)
  USBD_CDC_ACM.DeInit(pdev, cfgidx);
#endif
#if (USBD_USE_CDC_ECM == 1)
  USBD_CDC_ECM.DeInit(pdev, cfgidx);
#endif
#if (USBD_USE_CDC_RNDIS == 1)
  USBD_CDC_RNDIS.DeInit(pdev, cfgidx);
#endif
#if (USBD_USE_HID_MOUSE == 1)
  USBD_HID_MOUSE.DeInit(pdev, cfgidx);
#endif
#if (USBD_USE_HID_KEYBOARD == 1)
  USBD_HID_KEYBOARD.DeInit(pdev, cfgidx);
#endif
#if (USBD_USE_HID_CUSTOM == 1)
  USBD_HID_CUSTOM.DeInit(pdev, cfgidx);
#endif
#if (USBD_USE_UAC_MIC == 1)
  USBD_AUDIO_MIC.DeInit(pdev, cfgidx);
#endif
#if (USBD_USE_UAC_SPKR == 1)
  USBD_AUDIO_SPKR.DeInit(pdev, cfgidx);
#endif
#if (USBD_USE_UVC == 1)
  USBD_VIDEO.DeInit(pdev, cfgidx);
#endif
#if (USBD_USE_MSC == 1)
  USBD_MSC.DeInit(pdev, cfgidx);
#endif
#if (USBD_USE_DFU == 1)
  USBD_DFU.DeInit(pdev, cfgidx);
#endif
#if (USBD_USE_PRNTR == 1)
  USBD_PRNT.DeInit(pdev, cfgidx);
#endif

  return (uint8_t)USBD_OK;
}

/**
  * @brief  USBD_COMPOSITE_Setup
  *         Handle the COMPOSITE specific requests
  * @param  pdev: instance
  * @param  req: usb requests
  * @retval status
  */
static uint8_t USBD_COMPOSITE_Setup(USBD_HandleTypeDef *pdev,
                                    USBD_SetupReqTypedef *req)
{
#if (USBD_USE_CDC_ACM == 1)
  for (uint8_t i = 0; i < USBD_CDC_ACM_COUNT; i++)
  {
    if (LOBYTE(req->wIndex) == CDC_CMD_ITF_NBR[i] || LOBYTE(req->wIndex) == CDC_COM_ITF_NBR[i])
    {
      return USBD_CDC_ACM.Setup(pdev, req);
    }
  }
#endif
#if (USBD_USE_CDC_ECM == 1)
  if (LOBYTE(req->wIndex) == CDC_ECM_CMD_ITF_NBR || LOBYTE(req->wIndex) == CDC_ECM_COM_ITF_NBR)
  {
    return USBD_CDC_ECM.Setup(pdev, req);
  }
#endif
#if (USBD_USE_CDC_RNDIS == 1)
  if (LOBYTE(req->wIndex) == CDC_RNDIS_CMD_ITF_NBR || LOBYTE(req->wIndex) == CDC_RNDIS_COM_ITF_NBR)
  {
    return USBD_CDC_RNDIS.Setup(pdev, req);
  }
#endif
#if (USBD_USE_HID_MOUSE == 1)
  if (LOBYTE(req->wIndex) == HID_MOUSE_ITF_NBR)
  {
    return USBD_HID_MOUSE.Setup(pdev, req);
  }
#endif
#if (USBD_USE_HID_KEYBOARD == 1)
  if (LOBYTE(req->wIndex) == HID_KEYBOARD_ITF_NBR)
  {
    return USBD_HID_KEYBOARD.Setup(pdev, req);
  }
#endif
#if (USBD_USE_HID_CUSTOM == 1)
  if (LOBYTE(req->wIndex) == CUSTOM_HID_ITF_NBR)
  {
    return USBD_HID_CUSTOM.Setup(pdev, req);
  }
#endif
#if (USBD_USE_UAC_MIC == 1)
  if (LOBYTE(req->wIndex) == AUDIO_MIC_AC_ITF_NBR || LOBYTE(req->wIndex) == AUDIO_MIC_AS_ITF_NBR)
  {
    return USBD_AUDIO_MIC.Setup(pdev, req);
  }
#endif
#if (USBD_USE_UAC_SPKR == 1)
  if (LOBYTE(req->wIndex) == AUDIO_SPKR_AC_ITF_NBR || LOBYTE(req->wIndex) == AUDIO_SPKR_AS_ITF_NBR)
  {
    return USBD_AUDIO_SPKR.Setup(pdev, req);
  }
#endif
#if (USBD_USE_UVC == 1)
  if (LOBYTE(req->wIndex) == UVC_VC_IF_NUM || LOBYTE(req->wIndex) == UVC_VS_IF_NUM)
  {
    return USBD_VIDEO.Setup(pdev, req);
  }
#endif
#if (USBD_USE_MSC == 1)
  if (LOBYTE(req->wIndex) == MSC_ITF_NBR)
  {
    return USBD_MSC.Setup(pdev, req);
  }
#endif
#if (USBD_USE_DFU == 1)
  if (LOBYTE(req->wIndex) == DFU_ITF_NBR)
  {
    return USBD_DFU.Setup(pdev, req);
  }
#endif
#if (USBD_USE_PRNTR == 1)
  if (LOBYTE(req->wIndex) == PRNT_ITF_NBR)
  {
    USBD_PRNT.Setup(pdev, req);
  }
#endif

  return USBD_FAIL;
}

/**
  * @brief  USBD_COMPOSITE_DataIn
  *         handle data IN Stage
  * @param  pdev: device instance
  * @param  epnum: endpoint index
  * @retval status
  */
static uint8_t USBD_COMPOSITE_DataIn(USBD_HandleTypeDef *pdev, uint8_t epnum)
{
#if (USBD_USE_CDC_ACM == 1)
  for (uint8_t i = 0; i < USBD_CDC_ACM_COUNT; i++)
  {
    if (epnum == (CDC_IN_EP[i] & 0x7F) || epnum == (CDC_CMD_EP[i] & 0x7F))
    {
      return USBD_CDC_ACM.DataIn(pdev, epnum);
    }
  }
#endif
#if (USBD_USE_CDC_ECM == 1)
  if (epnum == (CDC_ECM_IN_EP & 0x7F) || epnum == (CDC_ECM_CMD_EP & 0x7F))
  {
    return USBD_CDC_ECM.DataIn(pdev, epnum);
  }
#endif
#if (USBD_USE_CDC_RNDIS == 1)
  if (epnum == (CDC_RNDIS_IN_EP & 0x7F) || epnum == (CDC_RNDIS_CMD_EP & 0x7F))
  {
    return USBD_CDC_RNDIS.DataIn(pdev, epnum);
  }
#endif
#if (USBD_USE_HID_MOUSE == 1)
  if (epnum == (HID_MOUSE_IN_EP & 0x7F))
  {
    return USBD_HID_MOUSE.DataIn(pdev, epnum);
  }
#endif
#if (USBD_USE_HID_KEYBOARD == 1)
  if (epnum == (HID_KEYBOARD_IN_EP & 0x7F))
  {
    return USBD_HID_KEYBOARD.DataIn(pdev, epnum);
  }
#endif
#if (USBD_USE_HID_CUSTOM == 1)
  if (epnum == (CUSTOM_HID_IN_EP & 0x7F))
  {
    return USBD_HID_CUSTOM.DataIn(pdev, epnum);
  }
#endif
#if (USBD_USE_UAC_MIC == 1)
  if (epnum == (AUDIO_MIC_EP & 0x7F))
  {
    return USBD_AUDIO_MIC.DataIn(pdev, epnum);
  }
#endif
#if (USBD_USE_UAC_SPKR == 1)
#endif
#if (USBD_USE_UVC == 1)
  if (epnum == (UVC_IN_EP & 0x7F))
  {
    return USBD_VIDEO.DataIn(pdev, epnum);
  }
#endif
#if (USBD_USE_MSC == 1)
  if (epnum == (MSC_IN_EP & 0x7F))
  {
    return USBD_MSC.DataIn(pdev, epnum);
  }
#endif
#if (USBD_USE_DFU == 1)
#endif
#if (USBD_USE_PRNTR == 1)
  if (epnum == (PRNT_IN_EP & 0x7F))
  {
    USBD_PRNT.DataIn(pdev, epnum);
  }
#endif

  return USBD_FAIL;
}

/**
  * @brief  USBD_COMPOSITE_EP0_RxReady
  *         handle EP0 Rx Ready event
  * @param  pdev: device instance
  * @retval status
  */
static uint8_t USBD_COMPOSITE_EP0_RxReady(USBD_HandleTypeDef *pdev)
{
#if (USBD_USE_CDC_ACM == 1)
  USBD_CDC_ACM.EP0_RxReady(pdev);
#endif
#if (USBD_USE_CDC_ECM == 1)
  USBD_CDC_ECM.EP0_RxReady(pdev);
#endif
#if (USBD_USE_CDC_RNDIS == 1)
  USBD_CDC_RNDIS.EP0_RxReady(pdev);
#endif
#if (USBD_USE_HID_MOUSE == 1)
#endif
#if (USBD_USE_HID_KEYBOARD == 1)
#endif
#if (USBD_USE_HID_CUSTOM == 1)
  USBD_HID_CUSTOM.EP0_RxReady(pdev);
#endif
#if (USBD_USE_UAC_MIC == 1)
  USBD_AUDIO_MIC.EP0_RxReady(pdev);
#endif
#if (USBD_USE_UAC_SPKR == 1)
  USBD_AUDIO_SPKR.EP0_RxReady(pdev);
#endif
#if (USBD_USE_UVC == 1)
#endif
#if (USBD_USE_MSC == 1)
#endif
#if (USBD_USE_DFU == 1)
  USBD_DFU.EP0_RxReady(pdev);
#endif
#if (USBD_USE_PRNTR == 1)
#endif

  return (uint8_t)USBD_OK;
}

/**
  * @brief  USBD_COMPOSITE_EP0_TxReady
  *         handle EP0 TRx Ready event
  * @param  pdev: device instance
  * @retval status
  */
static uint8_t USBD_COMPOSITE_EP0_TxReady(USBD_HandleTypeDef *pdev)
{
#if (USBD_USE_CDC_ACM == 1)
#endif
#if (USBD_USE_CDC_ECM == 1)
#endif
#if (USBD_USE_CDC_RNDIS == 1)
#endif
#if (USBD_USE_HID_MOUSE == 1)
#endif
#if (USBD_USE_HID_KEYBOARD == 1)
#endif
#if (USBD_USE_HID_CUSTOM == 1)
#endif
#if (USBD_USE_UAC_MIC == 1)
  USBD_AUDIO_MIC.EP0_TxSent(pdev);
#endif
#if (USBD_USE_UAC_SPKR == 1)
  USBD_AUDIO_SPKR.EP0_TxSent(pdev);
#endif
#if (USBD_USE_UVC == 1)
#endif
#if (USBD_USE_MSC == 1)
#endif
#if (USBD_USE_DFU == 1)
  USBD_DFU.EP0_TxSent(pdev);
#endif
#if (USBD_USE_PRNTR == 1)
#endif

  return (uint8_t)USBD_OK;
}

/**
  * @brief  USBD_COMPOSITE_SOF
  *         handle SOF event
  * @param  pdev: device instance
  * @retval status
  */
static uint8_t USBD_COMPOSITE_SOF(USBD_HandleTypeDef *pdev)
{
#if (USBD_USE_CDC_ACM == 1)
#endif
#if (USBD_USE_CDC_ECM == 1)
#endif
#if (USBD_USE_CDC_RNDIS == 1)
#endif
#if (USBD_USE_HID_MOUSE == 1)
#endif
#if (USBD_USE_HID_KEYBOARD == 1)
#endif
#if (USBD_USE_HID_CUSTOM == 1)
#endif
#if (USBD_USE_UAC_MIC == 1)
  USBD_AUDIO_MIC.SOF(pdev);
#endif
#if (USBD_USE_UAC_SPKR == 1)
  USBD_AUDIO_SPKR.SOF(pdev);
#endif
#if (USBD_USE_UVC == 1)
  USBD_VIDEO.SOF(pdev);
#endif
#if (USBD_USE_MSC == 1)
#endif
#if (USBD_USE_DFU == 1)
  USBD_DFU.SOF(pdev);
#endif
#if (USBD_USE_PRNTR == 1)
#endif

  return (uint8_t)USBD_OK;
}

/**
  * @brief  USBD_COMPOSITE_IsoINIncomplete
  *         handle data ISO IN Incomplete event
  * @param  pdev: device instance
  * @param  epnum: endpoint index
  * @retval status
  */
static uint8_t USBD_COMPOSITE_IsoINIncomplete(USBD_HandleTypeDef *pdev, uint8_t epnum)
{
#if (USBD_USE_CDC_ACM == 1)
#endif
#if (USBD_USE_CDC_ECM == 1)
#endif
#if (USBD_USE_CDC_RNDIS == 1)
#endif
#if (USBD_USE_HID_MOUSE == 1)
#endif
#if (USBD_USE_HID_KEYBOARD == 1)
#endif
#if (USBD_USE_HID_CUSTOM == 1)
#endif
#if (USBD_USE_UAC_MIC == 1)
  if (epnum == (AUDIO_MIC_EP & 0x7F))
  {
    USBD_AUDIO_MIC.IsoINIncomplete(pdev, epnum);
  }
#endif
#if (USBD_USE_UAC_SPKR == 1)
#endif
#if (USBD_USE_UVC == 1)
  if (epnum == (UVC_IN_EP & 0x7F))
  {
    USBD_VIDEO.IsoINIncomplete(pdev, epnum);
  }
#endif
#if (USBD_USE_MSC == 1)
#endif
#if (USBD_USE_DFU == 1)
#endif
#if (USBD_USE_PRNTR == 1)
#endif

  return (uint8_t)USBD_OK;
}

/**
  * @brief  USBD_COMPOSITE_IsoOutIncomplete
  *         handle data ISO OUT Incomplete event
  * @param  pdev: device instance
  * @param  epnum: endpoint index
  * @retval status
  */
static uint8_t USBD_COMPOSITE_IsoOutIncomplete(USBD_HandleTypeDef *pdev, uint8_t epnum)
{
#if (USBD_USE_CDC_ACM == 1)
#endif
#if (USBD_USE_CDC_ECM == 1)
#endif
#if (USBD_USE_CDC_RNDIS == 1)
#endif
#if (USBD_USE_HID_MOUSE == 1)
#endif
#if (USBD_USE_HID_KEYBOARD == 1)
#endif
#if (USBD_USE_HID_CUSTOM == 1)
#endif
#if (USBD_USE_UAC_MIC == 1)
#endif
#if (USBD_USE_UAC_SPKR == 1)
  if (epnum == AUDIO_SPKR_EP)
  {
    USBD_AUDIO_SPKR.IsoOUTIncomplete(pdev, epnum);
  }
#endif
#if (USBD_USE_UVC == 1)
#endif
#if (USBD_USE_MSC == 1)
#endif
#if (USBD_USE_DFU == 1)
#endif
#if (USBD_USE_PRNTR == 1)
#endif

  return (uint8_t)USBD_OK;
}
/**
  * @brief  USBD_COMPOSITE_DataOut
  *         handle data OUT Stage
  * @param  pdev: device instance
  * @param  epnum: endpoint index
  * @retval status
  */
static uint8_t USBD_COMPOSITE_DataOut(USBD_HandleTypeDef *pdev, uint8_t epnum)
{
#if (USBD_USE_CDC_ACM == 1)
  for (uint8_t i = 0; i < USBD_CDC_ACM_COUNT; i++)
  {
    if (epnum == CDC_OUT_EP[i])
    {
      return USBD_CDC_ACM.DataOut(pdev, epnum);
    }
  }
#endif
#if (USBD_USE_CDC_ECM == 1)
  if (epnum == CDC_ECM_OUT_EP)
  {
    return USBD_CDC_ECM.DataOut(pdev, epnum);
  }
#endif
#if (USBD_USE_CDC_RNDIS == 1)
  if (epnum == CDC_RNDIS_OUT_EP)
  {
    return USBD_CDC_RNDIS.DataOut(pdev, epnum);
  }
#endif
#if (USBD_USE_HID_MOUSE == 1)
#endif
#if (USBD_USE_HID_KEYBOARD == 1)
#endif
#if (USBD_USE_HID_CUSTOM == 1)
  if (epnum == CUSTOM_HID_OUT_EP)
  {
    return USBD_HID_CUSTOM.DataOut(pdev, epnum);
  }
#endif
#if (USBD_USE_UAC_MIC == 1)
#endif
#if (USBD_USE_UAC_SPKR == 1)
  if (epnum == AUDIO_SPKR_EP)
  {
    return USBD_AUDIO_SPKR.DataOut(pdev, epnum);
  }
#endif
#if (USBD_USE_UVC == 1)
#endif
#if (USBD_USE_MSC == 1)
  if (epnum == MSC_OUT_EP)
  {
    return USBD_MSC.DataOut(pdev, epnum);
  }
#endif
#if (USBD_USE_DFU == 1)
#endif
#if (USBD_USE_PRNTR == 1)
  if (epnum == PRNT_OUT_EP)
  {
    USBD_PRNT.DataOut(pdev, epnum);
  }
#endif

  return USBD_FAIL;
}

/**
  * @brief  USBD_COMPOSITE_GetHSCfgDesc
  *         return configuration descriptor
  * @param  length : pointer data length
  * @retval pointer to descriptor buffer
  */
static uint8_t *USBD_COMPOSITE_GetHSCfgDesc(uint16_t *length)
{
  *length = (uint16_t)sizeof(USBD_COMPOSITE_HSCfgDesc);
  return (uint8_t *)&USBD_COMPOSITE_HSCfgDesc;
}

/**
  * @brief  USBD_COMPOSITE_GetFSCfgDesc
  *         return configuration descriptor
  * @param  length : pointer data length
  * @retval pointer to descriptor buffer
  */
static uint8_t *USBD_COMPOSITE_GetFSCfgDesc(uint16_t *length)
{
  *length = (uint16_t)sizeof(USBD_COMPOSITE_FSCfgDesc);
  return (uint8_t *)&USBD_COMPOSITE_FSCfgDesc;
}

/**
  * @brief  USBD_COMPOSITE_GetOtherSpeedCfgDesc
  *         return configuration descriptor
  * @param  length : pointer data length
  * @retval pointer to descriptor buffer
  */
static uint8_t *USBD_COMPOSITE_GetOtherSpeedCfgDesc(uint16_t *length)
{
#if (USBD_USE_HS == 1)
  *length = (uint16_t)sizeof(USBD_COMPOSITE_FSCfgDesc);
  return (uint8_t *)&USBD_COMPOSITE_FSCfgDesc;
#else
  *length = (uint16_t)sizeof(USBD_COMPOSITE_HSCfgDesc);
  return (uint8_t *)&USBD_COMPOSITE_HSCfgDesc;
#endif
}

/**
  * @brief  DeviceQualifierDescriptor
  *         return Device Qualifier descriptor
  * @param  length : pointer data length
  * @retval pointer to descriptor buffer
  */
uint8_t *USBD_COMPOSITE_GetDeviceQualifierDesc(uint16_t *length)
{
  *length = (uint16_t)sizeof(USBD_COMPOSITE_DeviceQualifierDesc);
  return USBD_COMPOSITE_DeviceQualifierDesc;
}

/**
  * @brief  USBD_COMPOSITE_GetUsrStringDesc
  *         Manages the transfer of memory interfaces string descriptors.
  * @param  speed : current device speed
  * @param  index: descriptor index
  * @param  length : pointer data length
  * @retval pointer to the descriptor table or NULL if the descriptor is not supported.
  */
#if (USBD_SUPPORT_USER_STRING_DESC == 1U)
static uint8_t *USBD_COMPOSITE_GetUsrStringDesc(USBD_HandleTypeDef *pdev, uint8_t index, uint16_t *length)
{
  static uint8_t USBD_StrDesc[64];

  /* Check if the requested string interface is supported */
  if (index <= USBD_Track_String_Index)
  {
#if (USBD_USE_CDC_ACM == 1)
    char str_buffer[16] = "";
    for (uint8_t i = 0; i < USBD_CDC_ACM_COUNT; i++)
    {
      if (index == CDC_STR_DESC_IDX[i])
      {
        snprintf(str_buffer, sizeof(str_buffer), CDC_ACM_STR_DESC, i);
        USBD_GetString((uint8_t *)str_buffer, USBD_StrDesc, length);
      }
    }
#endif
#if (USBD_USE_CDC_ECM == 1)
    if (index == CDC_ECM_STR_DESC_IDX)
    {
      USBD_GetString((uint8_t *)CDC_ECM_STR_DESC, USBD_StrDesc, length);
    }
#endif
#if (USBD_USE_CDC_RNDIS == 1)
    if (index == CDC_RNDIS_STR_DESC_IDX)
    {
      USBD_GetString((uint8_t *)CDC_RNDIS_STR_DESC, USBD_StrDesc, length);
    }
#endif
#if (USBD_USE_HID_MOUSE == 1)
    if (index == HID_MOUSE_STR_DESC_IDX)
    {
      USBD_GetString((uint8_t *)HID_MOUSE_STR_DESC, USBD_StrDesc, length);
    }
#endif
#if (USBD_USE_HID_KEYBOARD == 1)
    if (index == HID_KEYBOARD_STR_DESC_IDX)
    {
      USBD_GetString((uint8_t *)HID_KEYBOARD_STR_DESC, USBD_StrDesc, length);
    }
#endif
#if (USBD_USE_HID_CUSTOM == 1)
    if (index == CUSTOM_HID_STR_DESC_IDX)
    {
      USBD_GetString((uint8_t *)CUSTOM_HID_STR_DESC, USBD_StrDesc, length);
    }
#endif
#if (USBD_USE_UAC_MIC == 1)
    if (index == AUDIO_MIC_STR_DESC_IDX)
    {
      USBD_GetString((uint8_t *)AUDIO_MIC_STR_DESC, USBD_StrDesc, length);
    }
#endif
#if (USBD_USE_UAC_SPKR == 1)
    if (index == AUDIO_SPKR_STR_DESC_IDX)
    {
      USBD_GetString((uint8_t *)AUDIO_SPKR_STR_DESC, USBD_StrDesc, length);
    }
#endif
#if (USBD_USE_UVC == 1)
    if (index == UVC_STR_DESC_IDX)
    {
      USBD_GetString((uint8_t *)UVC_STR_DESC, USBD_StrDesc, length);
    }
#endif
#if (USBD_USE_MSC == 1)
    if (index == MSC_BOT_STR_DESC_IDX)
    {
      USBD_GetString((uint8_t *)MSC_BOT_STR_DESC, USBD_StrDesc, length);
    }
#endif
#if (USBD_USE_DFU == 1)
    if (index == DFU_STR_DESC_IDX)
    {
      USBD_GetString((uint8_t *)DFU_STR_DESC, USBD_StrDesc, length);
    }
#endif
#if (USBD_USE_PRNTR == 1)
    if (index == PRINTER_STR_DESC_IDX)
    {
      USBD_GetString((uint8_t *)PRNT_STR_DESC, USBD_StrDesc, length);
    }
#endif
    return USBD_StrDesc;
  }
  else
  {
    /* Not supported Interface Descriptor index */
    return NULL;
  }
}
#endif

void USBD_COMPOSITE_Mount_Class(void)
{
  uint16_t len = 0;
  uint8_t *ptr = NULL;

  uint8_t in_ep_track = 0x81;
  uint8_t out_ep_track = 0x01;
  uint8_t interface_no_track = 0x00;

#if (USBD_USE_CDC_RNDIS == 1)
  ptr = USBD_CDC_RNDIS.GetFSConfigDescriptor(&len);
  USBD_Update_CDC_RNDIS_DESC(ptr,
                             interface_no_track,
                             interface_no_track + 1,
                             in_ep_track,
                             in_ep_track + 1,
                             out_ep_track,
                             USBD_Track_String_Index);
  memcpy(USBD_COMPOSITE_FSCfgDesc.USBD_CDC_RNDIS_DESC, ptr + 0x09, len - 0x09);

  ptr = USBD_CDC_RNDIS.GetHSConfigDescriptor(&len);
  USBD_Update_CDC_RNDIS_DESC(ptr,
                             interface_no_track,
                             interface_no_track + 1,
                             in_ep_track,
                             in_ep_track + 1,
                             out_ep_track,
                             USBD_Track_String_Index);
  memcpy(USBD_COMPOSITE_HSCfgDesc.USBD_CDC_RNDIS_DESC, ptr + 0x09, len - 0x09);

  in_ep_track += 2;
  out_ep_track += 1;
  interface_no_track += 2;
  USBD_Track_String_Index += 1;
#endif

#if (USBD_USE_CDC_ECM == 1)
  ptr = USBD_CDC_ECM.GetFSConfigDescriptor(&len);
  USBD_Update_CDC_ECM_DESC(ptr,
                           interface_no_track,
                           interface_no_track + 1,
                           in_ep_track,
                           in_ep_track + 1,
                           out_ep_track,
                           USBD_Track_String_Index);
  memcpy(USBD_COMPOSITE_FSCfgDesc.USBD_CDC_ECM_DESC, ptr + 0x09, len - 0x09);

  ptr = USBD_CDC_ECM.GetHSConfigDescriptor(&len);
  USBD_Update_CDC_ECM_DESC(ptr,
                           interface_no_track,
                           interface_no_track + 1,
                           in_ep_track,
                           in_ep_track + 1,
                           out_ep_track,
                           USBD_Track_String_Index);
  memcpy(USBD_COMPOSITE_HSCfgDesc.USBD_CDC_ECM_DESC, ptr + 0x09, len - 0x09);

  in_ep_track += 2;
  out_ep_track += 1;
  interface_no_track += 2;
  USBD_Track_String_Index += 1;
#endif

#if (USBD_USE_HID_MOUSE == 1)
  ptr = USBD_HID_MOUSE.GetFSConfigDescriptor(&len);
  USBD_Update_HID_Mouse_DESC(ptr, interface_no_track, in_ep_track, USBD_Track_String_Index);
  memcpy(USBD_COMPOSITE_FSCfgDesc.USBD_HID_MOUSE_DESC, ptr + 0x09, len - 0x09);

  ptr = USBD_HID_MOUSE.GetHSConfigDescriptor(&len);
  USBD_Update_HID_Mouse_DESC(ptr, interface_no_track, in_ep_track, USBD_Track_String_Index);
  memcpy(USBD_COMPOSITE_HSCfgDesc.USBD_HID_MOUSE_DESC, ptr + 0x09, len - 0x09);

  in_ep_track += 1;
  interface_no_track += 1;
  USBD_Track_String_Index += 1;
#endif

#if (USBD_USE_HID_KEYBOARD == 1)
  ptr = USBD_HID_KEYBOARD.GetFSConfigDescriptor(&len);
  USBD_Update_HID_KBD_DESC(ptr, interface_no_track, in_ep_track, USBD_Track_String_Index);
  memcpy(USBD_COMPOSITE_FSCfgDesc.USBD_HID_KEYBOARD_DESC, ptr + 0x09, len - 0x09);

  ptr = USBD_HID_KEYBOARD.GetHSConfigDescriptor(&len);
  USBD_Update_HID_KBD_DESC(ptr, interface_no_track, in_ep_track, USBD_Track_String_Index);
  memcpy(USBD_COMPOSITE_HSCfgDesc.USBD_HID_KEYBOARD_DESC, ptr + 0x09, len - 0x09);

  in_ep_track += 1;
  interface_no_track += 1;
  USBD_Track_String_Index += 1;
#endif

#if (USBD_USE_HID_CUSTOM == 1)
  ptr = USBD_HID_CUSTOM.GetFSConfigDescriptor(&len);
  USBD_Update_HID_Custom_DESC(ptr, interface_no_track, in_ep_track, out_ep_track, USBD_Track_String_Index);
  memcpy(USBD_COMPOSITE_FSCfgDesc.USBD_HID_CUSTOM_DESC, ptr + 0x09, len - 0x09);

  ptr = USBD_HID_CUSTOM.GetHSConfigDescriptor(&len);
  USBD_Update_HID_Custom_DESC(ptr, interface_no_track, in_ep_track, out_ep_track, USBD_Track_String_Index);
  memcpy(USBD_COMPOSITE_HSCfgDesc.USBD_HID_CUSTOM_DESC, ptr + 0x09, len - 0x09);

  in_ep_track += 1;
  out_ep_track += 1;
  interface_no_track += 1;
  USBD_Track_String_Index += 1;
#endif

#if (USBD_USE_UAC_MIC == 1)
  ptr = USBD_AUDIO_MIC.GetFSConfigDescriptor(&len);
  USBD_Update_Audio_MIC_DESC(ptr,
                             interface_no_track,
                             interface_no_track + 1,
                             in_ep_track,
                             USBD_Track_String_Index);
  memcpy(USBD_COMPOSITE_FSCfgDesc.USBD_UAC_MIC_DESC, ptr + 0x09, len - 0x09);

  ptr = USBD_AUDIO_MIC.GetHSConfigDescriptor(&len);
  USBD_Update_Audio_MIC_DESC(ptr,
                             interface_no_track,
                             interface_no_track + 1,
                             in_ep_track,
                             USBD_Track_String_Index);

  memcpy(USBD_COMPOSITE_HSCfgDesc.USBD_UAC_MIC_DESC, ptr + 0x09, len - 0x09);
  in_ep_track += 1;
  interface_no_track += 2;
  USBD_Track_String_Index += 1;
#endif

#if (USBD_USE_UAC_SPKR == 1)
  ptr = USBD_AUDIO_SPKR.GetFSConfigDescriptor(&len);
  USBD_Update_Audio_SPKR_DESC(ptr, interface_no_track, interface_no_track + 1, out_ep_track, USBD_Track_String_Index);
  memcpy(USBD_COMPOSITE_FSCfgDesc.USBD_AUDIO_SPKR_DESC, ptr + 0x09, len - 0x09);

  ptr = USBD_AUDIO_SPKR.GetHSConfigDescriptor(&len);
  USBD_Update_Audio_SPKR_DESC(ptr, interface_no_track, interface_no_track + 1, out_ep_track, USBD_Track_String_Index);
  memcpy(USBD_COMPOSITE_HSCfgDesc.USBD_AUDIO_SPKR_DESC, ptr + 0x09, len - 0x09);

  out_ep_track += 1;
  interface_no_track += 2;
  USBD_Track_String_Index += 1;
#endif

#if (USBD_USE_UVC == 1)
  ptr = USBD_VIDEO.GetFSConfigDescriptor(&len);
  USBD_Update_UVC_DESC(ptr, interface_no_track, interface_no_track + 1, in_ep_track, USBD_Track_String_Index);
  memcpy(USBD_COMPOSITE_FSCfgDesc.USBD_UVC_DESC, ptr + 0x09, len - 0x09);

  ptr = USBD_VIDEO.GetHSConfigDescriptor(&len);
  USBD_Update_UVC_DESC(ptr, interface_no_track, interface_no_track + 1, in_ep_track, USBD_Track_String_Index);
  memcpy(USBD_COMPOSITE_HSCfgDesc.USBD_UVC_DESC, ptr + 0x09, len - 0x09);

  in_ep_track += 1;
  interface_no_track += 2;
  USBD_Track_String_Index += 1;
#endif

#if (USBD_USE_MSC == 1)
  ptr = USBD_MSC.GetFSConfigDescriptor(&len);
  USBD_Update_MSC_DESC(ptr, interface_no_track, in_ep_track, out_ep_track, USBD_Track_String_Index);
  memcpy(USBD_COMPOSITE_FSCfgDesc.USBD_MSC_DESC, ptr + 0x09, len - 0x09);

  ptr = USBD_MSC.GetHSConfigDescriptor(&len);
  USBD_Update_MSC_DESC(ptr, interface_no_track, in_ep_track, out_ep_track, USBD_Track_String_Index);
  memcpy(USBD_COMPOSITE_HSCfgDesc.USBD_MSC_DESC, ptr + 0x09, len - 0x09);

  in_ep_track += 1;
  out_ep_track += 1;
  interface_no_track += 1;
  USBD_Track_String_Index += 1;
#endif

#if (USBD_USE_DFU == 1)
  ptr = USBD_DFU.GetFSConfigDescriptor(&len);
  USBD_Update_DFU_DESC(ptr, interface_no_track, USBD_Track_String_Index);
  memcpy(USBD_COMPOSITE_FSCfgDesc.USBD_DFU_DESC, ptr + 0x09, len - 0x09);

  ptr = USBD_DFU.GetHSConfigDescriptor(&len);
  USBD_Update_DFU_DESC(ptr, interface_no_track, USBD_Track_String_Index);
  memcpy(USBD_COMPOSITE_HSCfgDesc.USBD_DFU_DESC, ptr + 0x09, len - 0x09);

  interface_no_track += USBD_DFU_MAX_ITF_NUM;
  USBD_Track_String_Index += USBD_DFU_MAX_ITF_NUM;
#endif

#if (USBD_USE_PRNTR == 1)
  ptr = USBD_PRNT.GetFSConfigDescriptor(&len);
  USBD_Update_PRNT_DESC(ptr, interface_no_track, in_ep_track, out_ep_track, USBD_Track_String_Index);
  memcpy(USBD_COMPOSITE_FSCfgDesc.USBD_PRNTR_DESC, ptr + 0x09, len - 0x09);

  ptr = USBD_PRNT.GetHSConfigDescriptor(&len);
  USBD_Update_PRNT_DESC(ptr, interface_no_track, in_ep_track, out_ep_track, USBD_Track_String_Index);
  memcpy(USBD_COMPOSITE_HSCfgDesc.USBD_PRNTR_DESC, ptr + 0x09, len - 0x09);
  
  in_ep_track += 1;
  out_ep_track += 1;
  interface_no_track += 1;
  USBD_Track_String_Index += 1;
#endif

#if (USBD_USE_CDC_ACM == 1)
  ptr = USBD_CDC_ACM.GetFSConfigDescriptor(&len);
  USBD_Update_CDC_ACM_DESC(ptr,
                           interface_no_track,
                           interface_no_track + 1,
                           in_ep_track,
                           in_ep_track + 1,
                           out_ep_track,
                           USBD_Track_String_Index);
  memcpy(USBD_COMPOSITE_FSCfgDesc.USBD_CDC_ACM_DESC, ptr + 0x09, len - 0x09);

  ptr = USBD_CDC_ACM.GetHSConfigDescriptor(&len);
  USBD_Update_CDC_ACM_DESC(ptr,
                           interface_no_track,
                           interface_no_track + 1,
                           in_ep_track,
                           in_ep_track + 1,
                           out_ep_track,
                           USBD_Track_String_Index);
  memcpy(USBD_COMPOSITE_HSCfgDesc.USBD_CDC_ACM_DESC, ptr + 0x09, len - 0x09);

  in_ep_track += 2 * USBD_CDC_ACM_COUNT;
  out_ep_track += 1 * USBD_CDC_ACM_COUNT;
  interface_no_track += 2 * USBD_CDC_ACM_COUNT;
  USBD_Track_String_Index += USBD_CDC_ACM_COUNT;
#endif

  uint16_t CFG_SIZE = sizeof(USBD_COMPOSITE_CFG_DESC_t);
  ptr = USBD_COMPOSITE_HSCfgDesc.CONFIG_DESC;
  /* Configuration Descriptor */
  ptr[0] = 0x09;                        /* bLength: Configuration Descriptor size */
  ptr[1] = USB_DESC_TYPE_CONFIGURATION; /* bDescriptorType: Configuration */
  ptr[2] = LOBYTE(CFG_SIZE);            /* wTotalLength:no of returned bytes */
  ptr[3] = HIBYTE(CFG_SIZE);
  ptr[4] = interface_no_track; /* bNumInterfaces: 2 interface */
  ptr[5] = 0x01;               /* bConfigurationValue: Configuration value */
  ptr[6] = 0x00;               /* iConfiguration: Index of string descriptor describing the configuration */
#if (USBD_SELF_POWERED == 1U)
  ptr[7] = 0xC0; /* bmAttributes: Bus Powered according to user configuration */
#else
  ptr[7] = 0x80; /* bmAttributes: Bus Powered according to user configuration */
#endif
  ptr[8] = USBD_MAX_POWER; /* MaxPower 100 mA */

  ptr = USBD_COMPOSITE_FSCfgDesc.CONFIG_DESC;
  /* Configuration Descriptor */
  ptr[0] = 0x09;                        /* bLength: Configuration Descriptor size */
  ptr[1] = USB_DESC_TYPE_CONFIGURATION; /* bDescriptorType: Configuration */
  ptr[2] = LOBYTE(CFG_SIZE);            /* wTotalLength:no of returned bytes */
  ptr[3] = HIBYTE(CFG_SIZE);
  ptr[4] = interface_no_track; /* bNumInterfaces: 2 interface */
  ptr[5] = 0x01;               /* bConfigurationValue: Configuration value */
  ptr[6] = 0x00;               /* iConfiguration: Index of string descriptor describing the configuration */
#if (USBD_SELF_POWERED == 1U)
  ptr[7] = 0xC0; /* bmAttributes: Bus Powered according to user configuration */
#else
  ptr[7] = 0x80; /* bmAttributes: Bus Powered according to user configuration */
#endif
  ptr[8] = USBD_MAX_POWER; /* MaxPower 100 mA */

  (void)out_ep_track;
  (void)in_ep_track;
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
