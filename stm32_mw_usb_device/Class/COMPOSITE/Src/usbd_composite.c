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

#include "usbd_cdc_acm.h"
#include "usbd_cdc_ecm.h"
#include "usbd_cdc_rndis.h"
#include "usbd_hid_mouse.h"
#include "usbd_hid_keyboard.h"
#include "usbd_hid_custom.h"
#include "usbd_audio_mic.h"
#include "usbd_audio_spkr.h"
#include "usbd_video.h"
#include "usbd_msc.h"
#include "usbd_dfu.h"
#include "usbd_printer.h"

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
};

/////////////////////////////////// Keep track of assigned Interfaces and end points /////////////////////////
uint8_t COMPOSITE_CDC_ACM_CMD_ITF_NBR = 0x00;
uint8_t COMPOSITE_CDC_ACM_COM_ITF_NBR = 0x01;
uint8_t COMPOSITE_CDC_RNDIS_CMD_ITF_NBR = 0x00U;
uint8_t COMPOSITE_CDC_RNDIS_COM_ITF_NBR = 0x01U;
uint8_t COMPOSITE_CDC_ECM_CMD_ITF_NBR = 0x00U;
uint8_t COMPOSITE_CDC_ECM_COM_ITF_NBR = 0x01U;
uint8_t COMPOSITE_HID_MOUSE_ITF_NBR = 0x00U;
uint8_t COMPOSITE_HID_KEYBOARD_ITF_NBR = 0x00U;
uint8_t COMPOSITE_HID_CUSTOM_ITF_NBR = 0x00U;
uint8_t COMPOSITE_UAC_MIC_AC_IF_NUM = 0x00U;
uint8_t COMPOSITE_UAC_MIC_AS_IF_NUM = 0x01U;
uint8_t COMPOSITE_UAC_SPKR_AC_IF_NUM = 0x00U;
uint8_t COMPOSITE_UAC_SPKR_AS_IF_NUM = 0x01U;
uint8_t COMPOSITE_UVC_VC_IF_NUM = 0x00U;
uint8_t COMPOSITE_UVC_VS_IF_NUM = 0x01U;
uint8_t COMPOSITE_MSC_ITF_NBR = 0x00U;
uint8_t COMPOSITE_DFU_ITF_NBR = 0x00U;
uint8_t COMPOSITE_PRNTR_ITF_NBR = 0x00U;

uint8_t COMPOSITE_CDC_ACM_CMD_EP = 0x81;
uint8_t COMPOSITE_CDC_ACM_IN_EP = 0x82;
uint8_t COMPOSITE_CDC_ACM_OUT_EP = 0x01;

uint8_t COMPOSITE_CDC_RNDIS_IN_EP = 0x81U;
uint8_t COMPOSITE_CDC_RNDIS_OUT_EP = 0x01U;
uint8_t COMPOSITE_CDC_RNDIS_CMD_EP = 0x82U;

uint8_t COMPOSITE_CDC_ECM_IN_EP = 0x81U;
uint8_t COMPOSITE_CDC_ECM_OUT_EP = 0x01U;
uint8_t COMPOSITE_CDC_ECM_CMD_EP = 0x82U;

uint8_t COMPOSITE_HID_MOUSE_IN_EP = 0x81U;
uint8_t COMPOSITE_HID_KEYBOARD_IN_EP = 0x81U;
uint8_t COMPOSITE_HID_CUSTOM_IN_EP = 0x81U;
uint8_t COMPOSITE_HID_CUSTOM_OUT_EP = 0x01U;

uint8_t COMPOSITE_UAC_MIC_IN_EP = 0x84U;
uint8_t COMPOSITE_UAC_SPKR_OUT_EP = 0x02U;

uint8_t COMPOSITE_UVC_IN_EP = 0x83U;

uint8_t COMPOSITE_MSC_IN_EP = 0x84U;
uint8_t COMPOSITE_MSC_OUT_EP = 0x02U;

uint8_t COMPOSITE_PRNTR_IN_EP = 0x84U;
uint8_t COMPOSITE_PRNTR_OUT_EP = 0x02U;

#if defined(__ICCARM__) /*!< IAR Compiler */
#pragma data_alignment = 4
#endif
typedef struct __attribute__((packed, aligned(4))) USBD_COMPOSITE_CFG_DESC_t
{
  uint8_t CONFIG_DESC[USB_CONF_DESC_SIZE];
#if (USBD_USE_CDC_ACM == 1)
  uint8_t USBD_CDC_ACM0_DESC[USB_CDC_CONFIG_DESC_SIZ - 0x09];
#endif
#if (USBD_USE_CDC_ECM == 1)
  uint8_t USBD_CDC_ECM_DESC[CDC_ECM_CONFIG_DESC_SIZE - 0x09];
#endif
#if (USBD_USE_CDC_RNDIS == 1)
  uint8_t USBD_CDC_RNDIS_DESC[CDC_RNDIS_CONFIG_DESC_SIZE - 0x09];
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
  uint8_t USBD_UAC_MIC_DESC[AUDIO_MIC_CONFIG_DESC_SIZE - 0x09];
#endif
#if (USBD_USE_UAC_SPKR == 1)
  uint8_t USBD_UAC_SPKR_DESC[USB_AUDIO_CONFIG_DESC_SIZ - 0x09];
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

} USBD_COMPOSITE_CFG_DESC_t;

USBD_COMPOSITE_CFG_DESC_t USBD_COMPOSITE_FSCfgDesc, USBD_COMPOSITE_HSCfgDesc;

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
  if (req->wIndex == COMPOSITE_CDC_ACM_CMD_ITF_NBR || req->wIndex == COMPOSITE_CDC_ACM_COM_ITF_NBR)
  {
    return USBD_CDC_ACM.Setup(pdev, req);
  }
#endif
#if (USBD_USE_CDC_ECM == 1)
  if (req->wIndex == COMPOSITE_CDC_ECM_CMD_ITF_NBR || req->wIndex == COMPOSITE_CDC_ECM_COM_ITF_NBR)
  {
    return USBD_CDC_ECM.Setup(pdev, req);
  }
#endif
#if (USBD_USE_CDC_RNDIS == 1)
  if (req->wIndex == COMPOSITE_CDC_RNDIS_CMD_ITF_NBR || req->wIndex == COMPOSITE_CDC_RNDIS_COM_ITF_NBR)
  {
    return USBD_CDC_RNDIS.Setup(pdev, req);
  }
#endif
#if (USBD_USE_HID_MOUSE == 1)
  if (req->wIndex == COMPOSITE_HID_MOUSE_ITF_NBR)
  {
    return USBD_HID_MOUSE.Setup(pdev, req);
  }
#endif
#if (USBD_USE_HID_KEYBOARD == 1)
  if (req->wIndex == COMPOSITE_HID_KEYBOARD_ITF_NBR)
  {
    return USBD_HID_KEYBOARD.Setup(pdev, req);
  }
#endif
#if (USBD_USE_HID_CUSTOM == 1)
  if (req->wIndex == COMPOSITE_HID_CUSTOM_ITF_NBR)
  {
    return USBD_HID_CUSTOM.Setup(pdev, req);
  }
#endif
#if (USBD_USE_UAC_MIC == 1)
  if (req->wIndex == COMPOSITE_UAC_MIC_AC_IF_NUM || req->wIndex == COMPOSITE_UAC_MIC_AS_IF_NUM)
  {
    return USBD_AUDIO_MIC.Setup(pdev, req);
  }
#endif
#if (USBD_USE_UAC_SPKR == 1)
  if (req->wIndex == COMPOSITE_UAC_SPKR_AC_IF_NUM || req->wIndex == COMPOSITE_UAC_SPKR_AS_IF_NUM)
  {
    return USBD_AUDIO_SPKR.Setup(pdev, req);
  }
#endif
#if (USBD_USE_UVC == 1)
  if (req->wIndex == COMPOSITE_UVC_VC_IF_NUM || req->wIndex == COMPOSITE_UVC_VS_IF_NUM)
  {
    return USBD_VIDEO.Setup(pdev, req);
  }
#endif
#if (USBD_USE_MSC == 1)
  if (req->wIndex == COMPOSITE_MSC_ITF_NBR)
  {
    return USBD_MSC.Setup(pdev, req);
  }
#endif
#if (USBD_USE_DFU == 1)
  if (req->wIndex == COMPOSITE_DFU_ITF_NBR)
  {
    return USBD_DFU.Setup(pdev, req);
  }
#endif
#if (USBD_USE_PRNTR == 1)
  if (req->wIndex == COMPOSITE_PRNTR_ITF_NBR)
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
  if (epnum == (COMPOSITE_CDC_ACM_IN_EP & 0x7F) || epnum == (COMPOSITE_CDC_ACM_CMD_EP & 0x7F))
  {
    return USBD_CDC_ACM.DataIn(pdev, epnum);
  }
#endif
#if (USBD_USE_CDC_ECM == 1)
  if (epnum == (COMPOSITE_CDC_ECM_IN_EP & 0x7F) || epnum == (COMPOSITE_CDC_ECM_CMD_EP & 0x7F))
  {
    return USBD_CDC_ECM.DataIn(pdev, epnum);
  }
#endif
#if (USBD_USE_CDC_RNDIS == 1)
  if (epnum == (COMPOSITE_CDC_RNDIS_IN_EP & 0x7F) || epnum == (COMPOSITE_CDC_RNDIS_CMD_EP & 0x7F))
  {
    return USBD_CDC_RNDIS.DataIn(pdev, epnum);
  }
#endif
#if (USBD_USE_HID_MOUSE == 1)
  if (epnum == (COMPOSITE_HID_MOUSE_IN_EP & 0x7F))
  {
    return USBD_HID_MOUSE.DataIn(pdev, epnum);
  }
#endif
#if (USBD_USE_HID_KEYBOARD == 1)
  if (epnum == (COMPOSITE_HID_KEYBOARD_IN_EP & 0x7F))
  {
    return USBD_HID_KEYBOARD.DataIn(pdev, epnum);
  }
#endif
#if (USBD_USE_HID_CUSTOM == 1)
  if (epnum == (COMPOSITE_HID_CUSTOM_IN_EP & 0x7F))
  {
    return USBD_HID_CUSTOM.DataIn(pdev, epnum);
  }
#endif
#if (USBD_USE_UAC_MIC == 1)
  if (epnum == (COMPOSITE_UAC_MIC_IN_EP & 0x7F))
  {
    return USBD_AUDIO_MIC.DataIn(pdev, epnum);
  }
#endif
#if (USBD_USE_UAC_SPKR == 1)
#endif
#if (USBD_USE_UVC == 1)
  if (epnum == (COMPOSITE_UVC_IN_EP & 0x7F))
  {
    return USBD_VIDEO.DataIn(pdev, epnum);
  }
#endif
#if (USBD_USE_MSC == 1)
  if (epnum == (COMPOSITE_MSC_IN_EP & 0x7F))
  {
    return USBD_MSC.DataIn(pdev, epnum);
  }
#endif
#if (USBD_USE_DFU == 1)
#endif
#if (USBD_USE_PRNTR == 1)
  if (epnum == (COMPOSITE_PRNTR_IN_EP & 0x7F))
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
  USBD_AUDIO_MIC.IsoINIncomplete(pdev, epnum);
#endif
#if (USBD_USE_UAC_SPKR == 1)
  USBD_AUDIO_SPKR.IsoINIncomplete(pdev, epnum);
#endif
#if (USBD_USE_UVC == 1)
  USBD_VIDEO.IsoINIncomplete(pdev, epnum);
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
  USBD_AUDIO_MIC.IsoOUTIncomplete(pdev, epnum);
#endif
#if (USBD_USE_UAC_SPKR == 1)
  USBD_AUDIO_SPKR.IsoOUTIncomplete(pdev, epnum);
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
  if (epnum == COMPOSITE_CDC_ACM_OUT_EP)
  {
    return USBD_CDC_ACM.DataOut(pdev, epnum);
  }
#endif
#if (USBD_USE_CDC_ECM == 1)
  if (epnum == COMPOSITE_CDC_ECM_OUT_EP)
  {
    return USBD_CDC_ECM.DataOut(pdev, epnum);
  }
#endif
#if (USBD_USE_CDC_RNDIS == 1)
  if (epnum == COMPOSITE_CDC_RNDIS_OUT_EP)
  {
    return USBD_CDC_RNDIS.DataOut(pdev, epnum);
  }
#endif
#if (USBD_USE_HID_MOUSE == 1)
#endif
#if (USBD_USE_HID_KEYBOARD == 1)
#endif
#if (USBD_USE_HID_CUSTOM == 1)
  if (epnum == COMPOSITE_HID_CUSTOM_OUT_EP)
  {
    return USBD_HID_CUSTOM.DataOut(pdev, epnum);
  }
#endif
#if (USBD_USE_UAC_MIC == 1)
#endif
#if (USBD_USE_UAC_SPKR == 1)
  if (epnum == COMPOSITE_UAC_SPKR_OUT_EP)
  {
    return USBD_AUDIO_SPKR.DataOut(pdev, epnum);
  }
#endif
#if (USBD_USE_UVC == 1)
#endif
#if (USBD_USE_MSC == 1)
  if (epnum == COMPOSITE_MSC_OUT_EP)
  {
    return USBD_MSC.DataOut(pdev, epnum);
  }
#endif
#if (USBD_USE_DFU == 1)
#endif
#if (USBD_USE_PRNTR == 1)
  if (epnum == COMPOSITE_PRNTR_OUT_EP)
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
  //TODOif (pdev->dev_speed == USBD_SPEED_HIGH)
  {
    *length = (uint16_t)sizeof(USBD_COMPOSITE_FSCfgDesc);
    return (uint8_t *)&USBD_COMPOSITE_FSCfgDesc;
  }

  *length = (uint16_t)sizeof(USBD_COMPOSITE_HSCfgDesc);
  return (uint8_t *)&USBD_COMPOSITE_HSCfgDesc;
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

void USBD_COMPOSITE_Mount_Class(void)
{
  uint16_t len = 0;
  uint8_t *ptr = NULL;

  uint8_t in_ep_count = 0x81;
  uint8_t out_ep_count = 0x01;
  uint8_t interface_count = 0x00;

#if (USBD_USE_CDC_ACM == 1)
  ptr = USBD_CDC_ACM.GetFSConfigDescriptor(&len);
  USBD_Update_CDC_ACM_DESC(ptr,
                           interface_count,
                           interface_count + 1,
                           in_ep_count,
                           in_ep_count + 1,
                           out_ep_count);
  memcpy(USBD_COMPOSITE_FSCfgDesc.USBD_CDC_ACM0_DESC, ptr + 0x09, len - 0x09);

  ptr = USBD_CDC_ACM.GetHSConfigDescriptor(&len);
  USBD_Update_CDC_ACM_DESC(USBD_COMPOSITE_HSCfgDesc.USBD_CDC_ACM0_DESC,
                           interface_count,
                           interface_count + 1,
                           in_ep_count,
                           in_ep_count + 1,
                           out_ep_count);
  memcpy(USBD_COMPOSITE_HSCfgDesc.USBD_CDC_ACM0_DESC, ptr + 0x09, len - 0x09);

  in_ep_count += 2 * USBD_CDC_ACM_COUNT;
  out_ep_count += 1 * USBD_CDC_ACM_COUNT;
  interface_count += 2 * USBD_CDC_ACM_COUNT;
#endif

#if (USBD_USE_CDC_RNDIS == 1)
  ptr = USBD_CDC_RNDIS.GetFSConfigDescriptor(&len);
  USBD_Update_CDC_RNDIS_DESC(ptr,
                             interface_count,
                             interface_count + 1,
                             in_ep_count,
                             in_ep_count + 1,
                             out_ep_count);
  memcpy(USBD_COMPOSITE_FSCfgDesc.USBD_CDC_RNDIS_DESC, ptr + 0x09, len - 0x09);

  ptr = USBD_CDC_RNDIS.GetHSConfigDescriptor(&len);
  USBD_Update_CDC_RNDIS_DESC(ptr,
                             interface_count,
                             interface_count + 1,
                             in_ep_count,
                             in_ep_count + 1,
                             out_ep_count);
  memcpy(USBD_COMPOSITE_HSCfgDesc.USBD_CDC_RNDIS_DESC, ptr + 0x09, len - 0x09);

  in_ep_count += 2;
  out_ep_count += 1;
  interface_count += 2;
#endif
#if (USBD_USE_CDC_ECM == 1)
  ptr = USBD_CDC_ECM.GetFSConfigDescriptor(&len);
  USBD_Update_CDC_ECM_DESC(ptr,
                           interface_count,
                           interface_count + 1,
                           in_ep_count,
                           in_ep_count + 1,
                           out_ep_count);
  memcpy(USBD_COMPOSITE_FSCfgDesc.USBD_CDC_ECM_DESC, ptr + 0x09, len - 0x09);

  ptr = USBD_CDC_ECM.GetHSConfigDescriptor(&len);
  USBD_Update_CDC_ECM_DESC(ptr,
                           interface_count,
                           interface_count + 1,
                           in_ep_count,
                           in_ep_count + 1,
                           out_ep_count);
  memcpy(USBD_COMPOSITE_HSCfgDesc.USBD_CDC_ECM_DESC, ptr + 0x09, len - 0x09);

  in_ep_count += 2;
  out_ep_count += 1;
  interface_count += 2;
#endif
#if (USBD_USE_HID_MOUSE == 1)
  ptr = USBD_HID_MOUSE.GetFSConfigDescriptor(&len);
  USBD_Update_HID_Mouse_DESC(ptr, interface_count, in_ep_count);
  memcpy(USBD_COMPOSITE_FSCfgDesc.USBD_HID_MOUSE_DESC, ptr + 0x09, len - 0x09);

  ptr = USBD_HID_MOUSE.GetHSConfigDescriptor(&len);
  USBD_Update_HID_Mouse_DESC(ptr, interface_count, in_ep_count);
  memcpy(USBD_COMPOSITE_HSCfgDesc.USBD_HID_MOUSE_DESC, ptr + 0x09, len - 0x09);
  in_ep_count += 1;
  interface_count += 1;
#endif
#if (USBD_USE_HID_KEYBOARD == 1)
  ptr = USBD_HID_KEYBOARD.GetFSConfigDescriptor(&len);
  USBD_Update_HID_KBD_DESC(ptr, interface_count, in_ep_count);
  memcpy(USBD_COMPOSITE_FSCfgDesc.USBD_HID_KEYBOARD_DESC, ptr + 0x09, len - 0x09);

  ptr = USBD_HID_KEYBOARD.GetHSConfigDescriptor(&len);
  USBD_Update_HID_KBD_DESC(ptr, interface_count, in_ep_count);
  memcpy(USBD_COMPOSITE_HSCfgDesc.USBD_HID_KEYBOARD_DESC, ptr + 0x09, len - 0x09);
  in_ep_count += 1;
  interface_count += 1;
#endif
#if (USBD_USE_HID_CUSTOM == 1)
  ptr = USBD_HID_CUSTOM.GetFSConfigDescriptor(&len);
  USBD_Update_HID_Custom_DESC(ptr, interface_count, in_ep_count, out_ep_count);
  memcpy(USBD_COMPOSITE_FSCfgDesc.USBD_HID_CUSTOM_DESC, ptr + 0x09, len - 0x09);

  ptr = USBD_HID_CUSTOM.GetHSConfigDescriptor(&len);
  USBD_Update_HID_Custom_DESC(ptr, interface_count, in_ep_count, out_ep_count);
  memcpy(USBD_COMPOSITE_HSCfgDesc.USBD_HID_CUSTOM_DESC, ptr + 0x09, len - 0x09);
  in_ep_count += 1;
  out_ep_count += 1;
  interface_count += 1;
#endif
#if (USBD_USE_UAC_MIC == 1)
  ptr = USBD_AUDIO_MIC.GetFSConfigDescriptor(&len);
  USBD_Update_Audio_MIC_DESC(ptr, interface_count, interface_count + 1, in_ep_count);
  memcpy(USBD_COMPOSITE_FSCfgDesc.USBD_UAC_MIC_DESC, ptr + 0x09, len - 0x09);

  ptr = USBD_AUDIO_MIC.GetHSConfigDescriptor(&len);
  USBD_Update_Audio_MIC_DESC(ptr, interface_count, interface_count + 1, in_ep_count);
  memcpy(USBD_COMPOSITE_HSCfgDesc.USBD_UAC_MIC_DESC, ptr + 0x09, len - 0x09);
  in_ep_count += 1;
  interface_count += 2;
#endif
#if (USBD_USE_UAC_SPKR == 1)
  ptr = USBD_AUDIO_SPKR.GetFSConfigDescriptor(&len);
  USBD_Update_Audio_SPKR_DESC(ptr, interface_count, interface_count + 1, out_ep_count);
  memcpy(USBD_COMPOSITE_FSCfgDesc.USBD_UAC_SPKR_DESC, ptr + 0x09, len - 0x09);

  ptr = USBD_AUDIO_SPKR.GetHSConfigDescriptor(&len);
  USBD_Update_Audio_SPKR_DESC(ptr, interface_count, interface_count + 1, out_ep_count);
  memcpy(USBD_COMPOSITE_HSCfgDesc.USBD_UAC_SPKR_DESC, ptr + 0x09, len - 0x09);
  out_ep_count += 1;
  interface_count += 2;
#endif
#if (USBD_USE_UVC == 1)
  ptr = USBD_VIDEO.GetFSConfigDescriptor(&len);
  USBD_Update_UVC_DESC(ptr, interface_count, interface_count + 1, in_ep_count);
  memcpy(USBD_COMPOSITE_FSCfgDesc.USBD_UVC_DESC, ptr + 0x09, len - 0x09);

  ptr = USBD_VIDEO.GetHSConfigDescriptor(&len);
  USBD_Update_UVC_DESC(ptr, interface_count, interface_count + 1, in_ep_count);
  memcpy(USBD_COMPOSITE_HSCfgDesc.USBD_UVC_DESC, ptr + 0x09, len - 0x09);
  in_ep_count += 1;
  interface_count += 2;
#endif
#if (USBD_USE_MSC == 1)
  ptr = USBD_MSC.GetFSConfigDescriptor(&len);
  USBD_Update_MSC_DESC(ptr, interface_count, in_ep_count, out_ep_count);
  memcpy(USBD_COMPOSITE_FSCfgDesc.USBD_MSC_DESC, ptr + 0x09, len - 0x09);

  ptr = USBD_MSC.GetHSConfigDescriptor(&len);
  USBD_Update_MSC_DESC(ptr, interface_count, in_ep_count, out_ep_count);
  memcpy(USBD_COMPOSITE_HSCfgDesc.USBD_MSC_DESC, ptr + 0x09, len - 0x09);
  in_ep_count += 1;
  out_ep_count += 1;
  interface_count += 1;
#endif
#if (USBD_USE_DFU == 1)
  ptr = USBD_DFU.GetFSConfigDescriptor(&len);
  USBD_Update_DFU_DESC(ptr, interface_count);
  memcpy(USBD_COMPOSITE_FSCfgDesc.USBD_DFU_DESC, ptr + 0x09, len - 0x09);

  ptr = USBD_DFU.GetHSConfigDescriptor(&len);
  USBD_Update_DFU_DESC(ptr, interface_count);
  memcpy(USBD_COMPOSITE_HSCfgDesc.USBD_DFU_DESC, ptr + 0x09, len - 0x09);
  interface_count += 1;
#endif
#if (USBD_USE_PRNTR == 1)
  ptr = USBD_PRNT.GetFSConfigDescriptor(&len);
  USBD_Update_PRNT_DESC(ptr, interface_count, in_ep_count, out_ep_count);
  memcpy(USBD_COMPOSITE_FSCfgDesc.USBD_PRNTR_DESC, ptr + 0x09, len - 0x09);

  ptr = USBD_PRNT.GetHSConfigDescriptor(&len);
  USBD_Update_PRNT_DESC(ptr, interface_count, in_ep_count, out_ep_count);
  memcpy(USBD_COMPOSITE_HSCfgDesc.USBD_PRNTR_DESC, ptr + 0x09, len - 0x09);
  interface_count += 1;
  out_ep_count += 1;
  interface_count += 1;
#endif

  uint16_t CFG_SIZE = sizeof(USBD_COMPOSITE_CFG_DESC_t);
  ptr = USBD_COMPOSITE_HSCfgDesc.CONFIG_DESC;
  /* Configuration Descriptor */
  ptr[0] = 0x09;                            /* bLength: Configuration Descriptor size */
  ptr[1] = USB_DESC_TYPE_CONFIGURATION;     /* bDescriptorType: Configuration */
  ptr[2] = LOBYTE(CFG_SIZE); /* wTotalLength:no of returned bytes */
  ptr[3] = HIBYTE(CFG_SIZE);
  ptr[4] = (2 * NUMBER_OF_CDC); /* bNumInterfaces: 2 interface */
  ptr[5] = 0x01;                /* bConfigurationValue: Configuration value */
  ptr[6] = 0x00;                /* iConfiguration: Index of string descriptor describing the configuration */
#if (USBD_SELF_POWERED == 1U)
  ptr[7] = 0xC0; /* bmAttributes: Bus Powered according to user configuration */
#else
  ptr[7] = 0x80; /* bmAttributes: Bus Powered according to user configuration */
#endif
  ptr[8] = USBD_MAX_POWER; /* MaxPower 100 mA */

  ptr = USBD_COMPOSITE_FSCfgDesc.CONFIG_DESC;
  /* Configuration Descriptor */
  ptr[0] = 0x09;                            /* bLength: Configuration Descriptor size */
  ptr[1] = USB_DESC_TYPE_CONFIGURATION;     /* bDescriptorType: Configuration */
  ptr[2] = LOBYTE(CFG_SIZE); /* wTotalLength:no of returned bytes */
  ptr[3] = HIBYTE(CFG_SIZE);
  ptr[4] = (2 * NUMBER_OF_CDC); /* bNumInterfaces: 2 interface */
  ptr[5] = 0x01;                /* bConfigurationValue: Configuration value */
  ptr[6] = 0x00;                /* iConfiguration: Index of string descriptor describing the configuration */
#if (USBD_SELF_POWERED == 1U)
  ptr[7] = 0xC0; /* bmAttributes: Bus Powered according to user configuration */
#else
  ptr[7] = 0x80; /* bmAttributes: Bus Powered according to user configuration */
#endif
  ptr[8] = USBD_MAX_POWER; /* MaxPower 100 mA */
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
