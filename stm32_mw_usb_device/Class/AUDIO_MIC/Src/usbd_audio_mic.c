/**
******************************************************************************
* @file    usbd_audio_in.c
* @author  Central Labs
* @version V2.4.2 MOD
* @date    01-Sep-2016
* @brief   This file provides the Audio Input core functions.
******************************************************************************
* @attention
*
* <h2><center>&copy; COPYRIGHT(c) 2014 STMicroelectronics</center></h2>
*
* Licensed under MCD-ST Liberty SW License Agreement V2, (the "License");
* You may not use this file except in compliance with the License.
* You may obtain a copy of the License at:
*
*        http://www.st.com/software_license_agreement_liberty_v2
*
* Redistribution and use in source and binary forms, with or without modification,
* are permitted provided that the following conditions are met:
*   1. Redistributions of source code must retain the above copyright notice,
*      this list of conditions and the following disclaimer.
*   2. Redistributions in binary form must reproduce the above copyright notice,
*      this list of conditions and the following disclaimer in the documentation
*      and/or other materials provided with the distribution.
*   3. Neither the name of STMicroelectronics nor the names of its contributors
*      may be used to endorse or promote products derived from this software
*      without specific prior written permission.
*
* THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
* AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
* IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
* DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
* FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
* DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
* SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
* CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
* OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
* OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*
******************************************************************************
*/

/* Includes ------------------------------------------------------------------*/

#include "usbd_audio_mic.h"
#include "usbd_desc.h"
#include "usbd_ctlreq.h"

#define _AUDIO_MIC_EP 0x81
#define _AUDIO_MIC_AC_ITF_NBR 0x00U
#define _AUDIO_MIC_AS_ITF_NBR 0x01U

uint8_t AUDIO_MIC_EP = _AUDIO_MIC_EP;
uint8_t AUDIO_MIC_AC_ITF_NBR = _AUDIO_MIC_AC_ITF_NBR;
uint8_t AUDIO_MIC_AS_ITF_NBR = _AUDIO_MIC_AS_ITF_NBR;

/** @addtogroup STM32_USB_OTG_DEVICE_LIBRARY
* @{
*/

/** @defgroup USBD_AUDIO_IN
*
* 	This file provides the Audio Input core functions.
*
*           This driver implements the following aspects:
*             - Device descriptor management
*             - Configuration descriptor management
*             - Standard AC Interface Descriptor management
*             - 1 Audio Streaming Interface
*             - 1 Audio Streaming Endpoint
*             - 1 Audio Terminal Input
*             - Audio Class-Specific AC Interfaces
*             - Audio Class-Specific AS Interfaces
*             - AudioControl Requests: mute and volume control
*             - Audio Synchronization type: Asynchronous
*             - Multiple frequencies and channel number configurable using ad hoc
*               init function
*
*          The current audio class version supports the following audio features:
*             - Pulse Coded Modulation (PCM) format
*             - Configurable sampling rate
*             - Bit resolution: 16
*             - Configurable Number of channels
*             - Volume control
*             - Mute/Unmute capability
*             - Asynchronous Endpoints
*
* @note     This driver has been developed starting from the usbd_audio.c file
*           included within the standard Cube Package for STM32F4
* @{
*/

/** @defgroup USBD_AUDIO_IN_Private_TypesDefinitions
* @{
*/
/**
* @}
*/

/** @defgroup USBD_AUDIO_IN_Private_Defines
* @{
*/

/**
* @}
*/

/** @defgroup USBD_AUDIO_IN_Private_Macros
* @{
*/
/**
* @}
*/

/** @defgroup USBD_AUDIO_IN_Private_FunctionPrototypes
* @{
*/
static uint8_t USBD_AUDIO_Init(USBD_HandleTypeDef *pdev, uint8_t cfgidx);
static uint8_t USBD_AUDIO_DeInit(USBD_HandleTypeDef *pdev, uint8_t cfgidx);
static uint8_t USBD_AUDIO_Setup(USBD_HandleTypeDef *pdev, USBD_SetupReqTypedef *req);
static uint8_t *USBD_AUDIO_GetCfgDesc(uint16_t *length);
static uint8_t *USBD_AUDIO_GetDeviceQualifierDesc(uint16_t *length);
static uint8_t USBD_AUDIO_DataIn(USBD_HandleTypeDef *pdev, uint8_t epnum);
static uint8_t USBD_AUDIO_DataOut(USBD_HandleTypeDef *pdev, uint8_t epnum);
static uint8_t USBD_AUDIO_EP0_RxReady(USBD_HandleTypeDef *pdev);
static uint8_t USBD_AUDIO_EP0_TxReady(USBD_HandleTypeDef *pdev);
static uint8_t USBD_AUDIO_SOF(USBD_HandleTypeDef *pdev);
static uint8_t USBD_AUDIO_IsoINIncomplete(USBD_HandleTypeDef *pdev, uint8_t epnum);
static uint8_t USBD_AUDIO_IsoOutIncomplete(USBD_HandleTypeDef *pdev, uint8_t epnum);
static void AUDIO_REQ_GetCurrent(USBD_HandleTypeDef *pdev, USBD_SetupReqTypedef *req);
static void AUDIO_REQ_SetCurrent(USBD_HandleTypeDef *pdev, USBD_SetupReqTypedef *req);
static void AUDIO_REQ_GetMaximum(USBD_HandleTypeDef *pdev, USBD_SetupReqTypedef *req);
static void AUDIO_REQ_GetMinimum(USBD_HandleTypeDef *pdev, USBD_SetupReqTypedef *req);
static void AUDIO_REQ_GetResolution(USBD_HandleTypeDef *pdev, USBD_SetupReqTypedef *req);

/**
* @}
*/

/** @defgroup USBD_AUDIO_Private_Variables
* @{
*/
/* This dummy buffer with 0 values will be sent when there is no availble data */
static uint8_t IsocInBuffDummy[48 * 4 * 2];
static int16_t VOL_CUR;
static USBD_AUDIO_MIC_HandleTypeDef haudioInstance;

USBD_ClassTypeDef USBD_AUDIO_MIC =
    {
        USBD_AUDIO_Init,
        USBD_AUDIO_DeInit,
        USBD_AUDIO_Setup,
        USBD_AUDIO_EP0_TxReady,
        USBD_AUDIO_EP0_RxReady,
        USBD_AUDIO_DataIn,
        USBD_AUDIO_DataOut,
        USBD_AUDIO_SOF,
        USBD_AUDIO_IsoINIncomplete,
        USBD_AUDIO_IsoOutIncomplete,
        USBD_AUDIO_GetCfgDesc,
        USBD_AUDIO_GetCfgDesc,
        USBD_AUDIO_GetCfgDesc,
        USBD_AUDIO_GetDeviceQualifierDesc,
};

/* USB AUDIO device Configuration Descriptor */
__ALIGN_BEGIN static uint8_t USBD_AUDIO_CfgDesc[AUDIO_MIC_CONFIG_DESC_SIZE] __ALIGN_END =
    {
        0x09,                                                           /* bLength */
        0x02,                                                           /* bDescriptorType */
        ((AUDIO_MIC_CONFIG_DESC_SIZE + AUDIO_MIC_CHANNELS - 1) & 0xff), /* wTotalLength */
        ((AUDIO_MIC_CONFIG_DESC_SIZE + AUDIO_MIC_CHANNELS - 1) >> 8),
        0x02, /* bNumInterfaces */
        0x01, /* bConfigurationValue */
        0x00, /* iConfiguration */
#if (USBD_SELF_POWERED == 1U)
        0xC0, /* bmAttributes: Bus Powered according to user configuration */
#else
        0x80, /* bmAttributes: Bus Powered according to user configuration */
#endif
        USBD_MAX_POWER, /* bMaxPower = 100 mA */
        /* 09 byte*/

        /* USB Microphone Standard interface descriptor */
        0x09,                            /* bLength */
        USB_DESC_TYPE_INTERFACE,         /* bDescriptorType */
        _AUDIO_MIC_AC_ITF_NBR,           /* bInterfaceNumber */
        0x00,                            /* bAlternateSetting */
        0x00,                            /* bNumEndpoints */
        AUDIO_MIC_DEVICE_CLASS,          /* bInterfaceClass */
        AUDIO_MIC_SUBCLASS_AUDIOCONTROL, /* bInterfaceSubClass */
        AUDIO_MIC_PROTOCOL_UNDEFINED,    /* bInterfaceProtocol */
        0x00,                            /* iInterface */
        /* 18 byte*/

        /* USB Microphone Class-specific AC Interface Descriptor */
        0x09,                                /* bLength */
        AUDIO_MIC_INTERFACE_DESCRIPTOR_TYPE, /* bDescriptorType */
        AUDIO_MIC_CONTROL_HEADER,            /* bDescriptorSubtype */
        0x00,
        /* 1.00 */ /* bcdADC */
        0x01,
        0x25 + AUDIO_MIC_CHANNELS, /* wTotalLength = 37+AUDIO_CHANNELS*/
        0x00,
        0x01,                  /* bInCollection */
        _AUDIO_MIC_AS_ITF_NBR, /* baInterfaceNr */
        /* 27 byte*/

        /* USB Microphone Input Terminal Descriptor */
        AUDIO_MIC_INPUT_TERMINAL_DESC_SIZE,  /* bLength */
        AUDIO_MIC_INTERFACE_DESCRIPTOR_TYPE, /* bDescriptorType */
        AUDIO_MIC_CONTROL_INPUT_TERMINAL,    /* bDescriptorSubtype */
        0x01,                                /* bTerminalID */
        0x01,                                /* wTerminalType AUDIO_TERMINAL_USB_MICROPHONE   0x0201 */
        0x02,
        0x00,               /* bAssocTerminal */
        AUDIO_MIC_CHANNELS, /* bNrChannels */
#if (AUDIO_MIC_CHANNELS == 1)
        0x00, /* wChannelConfig 0x0000  Mono */
        0x00,
#else
        0x03, /* wChannelConfig 0x0003  Stereo */
        0x00,
#endif
        0x00, /* iChannelNames */
        0x00, /* iTerminal */
        /* 39 byte*/

        /* USB Microphone Audio Feature Unit Descriptor */
        0x07 + AUDIO_MIC_CHANNELS + 1,       /* bLength */
        AUDIO_MIC_INTERFACE_DESCRIPTOR_TYPE, /* bDescriptorType */
        AUDIO_MIC_CONTROL_FEATURE_UNIT,      /* bDescriptorSubtype */
        0x02,                                /* bUnitID */
        0x01,                                /* bSourceID */
        0x01,                                /* bControlSize */

#if (AUDIO_MIC_CHANNELS == 1)
        0x02,
        0x00,
#else
        0x00,
        0x02,
        0x02,
#endif

#if (AUDIO_MIC_CHANNELS > 2)
        0x02,
#endif

#if (AUDIO_MIC_CHANNELS > 3)
        0x02,
#endif

#if (AUDIO_MIC_CHANNELS > 4)
        0x02,
#endif

#if (AUDIO_MIC_CHANNELS > 5)
        0x02,
#endif

#if (AUDIO_MIC_CHANNELS > 6)
        0x02,
#endif

#if (AUDIO_MIC_CHANNELS > 7)
        0x02,
#endif
        0x00, /* iTerminal */
        //47 + AUDIO_MIC_CHANNELS byte

        /*USB Microphone Output Terminal Descriptor */
        0x09,                                /* bLength */
        AUDIO_MIC_INTERFACE_DESCRIPTOR_TYPE, /* bDescriptorType */
        AUDIO_MIC_CONTROL_OUTPUT_TERMINAL,   /* bDescriptorSubtype */
        0x03,                                /* bTerminalID */
        0x01,                                /* wTerminalType AUDIO_TERMINAL_USB_STREAMING 0x0101*/
        0x01,
        0x00,
        0x02,
        0x00,
        //56 + AUDIO_MIC_CHANNELS byte

        /* USB Microphone Standard AS Interface Descriptor - Audio Streaming Zero Bandwith */
        /* Interface 1, Alternate Setting 0                                             */
        0x09,                              /* bLength */
        USB_DESC_TYPE_INTERFACE,           /* bDescriptorType */
        _AUDIO_MIC_AS_ITF_NBR,             /* bInterfaceNumber */
        0x00,                              /* bAlternateSetting */
        0x00,                              /* bNumEndpoints */
        AUDIO_MIC_DEVICE_CLASS,            /* bInterfaceClass */
        AUDIO_MIC_SUBCLASS_AUDIOSTREAMING, /* bInterfaceSubClass */
        AUDIO_MIC_PROTOCOL_UNDEFINED,      /* bInterfaceProtocol */
        0x00,
        //65 + AUDIO_MIC_CHANNELS byte

        /* USB Microphone Standard AS Interface Descriptor - Audio Streaming Operational */
        /* Interface 1, Alternate Setting 1                                           */
        0x09,                              /* bLength */
        USB_DESC_TYPE_INTERFACE,           /* bDescriptorType */
        _AUDIO_MIC_AS_ITF_NBR,             /* bInterfaceNumber */
        0x01,                              /* bAlternateSetting */
        0x01,                              /* bNumEndpoints */
        AUDIO_MIC_DEVICE_CLASS,            /* bInterfaceClass */
        AUDIO_MIC_SUBCLASS_AUDIOSTREAMING, /* bInterfaceSubClass */
        AUDIO_MIC_PROTOCOL_UNDEFINED,      /* bInterfaceProtocol */
        0x00,                              /* iInterface */
        //74 + AUDIO_MIC_CHANNELS byte

        /* USB Microphone Audio Streaming Interface Descriptor */
        AUDIO_MIC_STREAMING_INTERFACE_DESC_SIZE, /* bLength */
        AUDIO_MIC_INTERFACE_DESCRIPTOR_TYPE,     /* bDescriptorType */
        AUDIO_MIC_STREAMING_GENERAL,             /* bDescriptorSubtype */
        0x03,                                    /* bTerminalLink */
        0x01,                                    /* bDelay */
        0x01,                                    /* wFormatTag AUDIO_FORMAT_PCM  0x0001*/
        0x00,
        //81 + AUDIO_MIC_CHANNELS byte

        /* USB Microphone Audio Type I Format Interface Descriptor */
        0x0B,                                /* bLength */
        AUDIO_MIC_INTERFACE_DESCRIPTOR_TYPE, /* bDescriptorType */
        AUDIO_MIC_STREAMING_FORMAT_TYPE,     /* bDescriptorSubtype */
        AUDIO_MIC_FORMAT_TYPE_I,             /* bFormatType */
        AUDIO_MIC_CHANNELS,                  /* bNrChannels */
        0x02,                                /* bSubFrameSize */
        16,                                  /* bBitResolution */
        0x01,                                /* bSamFreqType */
        AUDIO_MIC_SMPL_FREQ & 0xff,          /* tSamFreq 8000 = 0x1F40 */
        (AUDIO_MIC_SMPL_FREQ >> 8) & 0xff,
        AUDIO_MIC_SMPL_FREQ >> 16,
        //92 + AUDIO_MIC_CHANNELS byte

        /* Endpoint 1 - Standard Descriptor */
        AUDIO_MIC_STANDARD_ENDPOINT_DESC_SIZE,                              /* bLength */
        0x05,                                                               /* bDescriptorType */
        _AUDIO_MIC_EP,                                                      /* bEndpointAddress 1 in endpoint*/
        0x05,                                                               /* bmAttributes */
        ((AUDIO_MIC_SMPL_FREQ / 1000 + 2) * AUDIO_MIC_CHANNELS * 2) & 0xFF, /* wMaxPacketSize */
        ((AUDIO_MIC_SMPL_FREQ / 1000 + 2) * AUDIO_MIC_CHANNELS * 2) >> 8,
        0x01, /* bInterval */
        0x00, /* bRefresh */
        0x00, /* bSynchAddress */

        /* Endpoint - Audio Streaming Descriptor*/
        AUDIO_MIC_STREAMING_ENDPOINT_DESC_SIZE, /* bLength */
        AUDIO_MIC_ENDPOINT_DESCRIPTOR_TYPE,     /* bDescriptorType */
        AUDIO_MIC_ENDPOINT_GENERAL,             /* bDescriptor */
        0x00,                                   /* bmAttributes */
        0x00,                                   /* bLockDelayUnits */
        0x00,                                   /* wLockDelay */
        0x00,
};

/* USB Standard Device Descriptor */
__ALIGN_BEGIN static uint8_t USBD_AUDIO_DeviceQualifierDesc[USB_LEN_DEV_QUALIFIER_DESC] __ALIGN_END =
    {
        USB_LEN_DEV_QUALIFIER_DESC,
        USB_DESC_TYPE_DEVICE_QUALIFIER,
        0x00,
        0x02,
        0x00,
        0x00,
        0x00,
        0x40,
        0x01,
        0x00,
};

/**
* @}
*/

/** @defgroup USBD_AUDIO_IN_Private_Functions
* @{
*/

/**
* @brief  USBD_AUDIO_Init
*         Initialize the AUDIO interface
* @param  pdev: device instance
* @param  cfgidx: Configuration index
* @retval status
*/

static uint8_t USBD_AUDIO_Init(USBD_HandleTypeDef *pdev,
                               uint8_t cfgidx)
{
  if (haudioInstance.state != STATE_USB_WAITING_FOR_INIT)
  {
    return USBD_FAIL;
  }

  USBD_AUDIO_MIC_HandleTypeDef *haudio;
  pdev->pClassData_UAC_MIC = &haudioInstance;
  haudio = (USBD_AUDIO_MIC_HandleTypeDef *)pdev->pClassData_UAC_MIC;
  uint16_t packet_dim = haudio->paketDimension;
  uint16_t wr_rd_offset = (AUDIO_MIC_PACKET_NUM / 2) * haudio->dataAmount / haudio->paketDimension;
  haudio->wr_ptr = wr_rd_offset * packet_dim;
  haudio->rd_ptr = 0;
  haudio->timeout = 0;

  ((USBD_AUDIO_MIC_ItfTypeDef *)pdev->pUserData_UAC_MIC)->Init(haudio->frequency, 0, haudio->channels);

  USBD_LL_OpenEP(pdev,
                 AUDIO_MIC_EP,
                 USBD_EP_TYPE_ISOC,
                 AUDIO_MIC_PACKET);

  USBD_LL_FlushEP(pdev, AUDIO_MIC_EP);

  USBD_LL_Transmit(pdev, AUDIO_MIC_EP,
                   IsocInBuffDummy,
                   packet_dim);

  haudio->state = STATE_USB_IDLE;
  return USBD_OK;
}

/**
* @brief  USBD_AUDIO_Init
*         DeInitialize the AUDIO layer
* @param  pdev: device instance
* @param  cfgidx: Configuration index
* @retval status
*/
static uint8_t USBD_AUDIO_DeInit(USBD_HandleTypeDef *pdev,
                                 uint8_t cfgidx)
{
  /* Close EP IN */
  USBD_LL_CloseEP(pdev, AUDIO_MIC_EP);
  /* DeInit  physical Interface components */
  if (pdev->pClassData_UAC_MIC != NULL)
  {
    ((USBD_AUDIO_MIC_ItfTypeDef *)pdev->pUserData_UAC_MIC)->DeInit(0);
    haudioInstance.state = STATE_USB_WAITING_FOR_INIT;
  }
  return USBD_OK;
}

/**
* @brief  USBD_AUDIO_Setup
*         Handle the AUDIO specific requests
* @param  pdev: instance
* @param  req: usb requests
* @retval status
*/
static uint8_t USBD_AUDIO_Setup(USBD_HandleTypeDef *pdev,
                                USBD_SetupReqTypedef *req)
{
  USBD_AUDIO_MIC_HandleTypeDef *haudio;
  uint16_t len;
  uint8_t *pbuf;
  uint8_t ret = USBD_OK;
  haudio = pdev->pClassData_UAC_MIC;

  switch (req->bmRequest & USB_REQ_TYPE_MASK)
  {
    /* AUDIO Class Requests -------------------------------*/
  case USB_REQ_TYPE_CLASS:
    switch (req->bRequest)
    {
    case AUDIO_MIC_REQ_GET_CUR:
      AUDIO_REQ_GetCurrent(pdev, req);
      break;

    case AUDIO_MIC_REQ_SET_CUR:
      AUDIO_REQ_SetCurrent(pdev, req);
      break;

    case AUDIO_MIC_REQ_GET_MIN:
      AUDIO_REQ_GetMinimum(pdev, req);
      break;

    case AUDIO_MIC_REQ_GET_MAX:
      AUDIO_REQ_GetMaximum(pdev, req);
      break;

    case AUDIO_MIC_REQ_GET_RES:
      AUDIO_REQ_GetResolution(pdev, req);
      break;

    default:
      USBD_CtlError(pdev, req);
      return USBD_FAIL;
    }
    break;

    /* Standard Requests -------------------------------*/
  case USB_REQ_TYPE_STANDARD:
    switch (req->bRequest)
    {
    case USB_REQ_GET_DESCRIPTOR:
      if ((req->wValue >> 8) == AUDIO_MIC_DESCRIPTOR_TYPE)
      {

        pbuf = USBD_AUDIO_CfgDesc + 18;
        len = MIN(AUDIO_MIC_DESC_SIZE, req->wLength);

        USBD_CtlSendData(pdev,
                         pbuf,
                         len);
      }
      break;

    case USB_REQ_GET_INTERFACE:
      USBD_CtlSendData(pdev,
                       (uint8_t *)haudio->alt_setting,
                       1);
      break;

    case USB_REQ_SET_INTERFACE:
      if ((uint8_t)(req->wValue) < USBD_MAX_NUM_INTERFACES)
      {
        haudio->alt_setting = (uint8_t)(req->wValue);
      }
      else
      {
        /* Call the error management function (command will be nacked */
        USBD_CtlError(pdev, req);
      }
      break;
    }
  }
  return ret;
}

/**
* @brief  USBD_AUDIO_GetCfgDesc
*         return configuration descriptor
* @param  length : pointer data length
* @retval pointer to descriptor buffer
*/
static uint8_t *USBD_AUDIO_GetCfgDesc(uint16_t *length)
{
  *length = sizeof(USBD_AUDIO_CfgDesc);
  return USBD_AUDIO_CfgDesc;
}

uint16_t app;
uint16_t rd_ptr;
uint16_t wr_ptr;
/**
* @brief  USBD_AUDIO_DataIn
*         handle data IN Stage
* @param  pdev: device instance
* @param  epnum: endpoint index
* @retval status
*/
static uint8_t USBD_AUDIO_DataIn(USBD_HandleTypeDef *pdev,
                                 uint8_t epnum)
{

  USBD_AUDIO_MIC_HandleTypeDef *haudio;

  haudio = pdev->pClassData_UAC_MIC;

  rd_ptr = haudio->rd_ptr;
  wr_ptr = haudio->wr_ptr;

  uint32_t length_usb_pck;
  // uint16_t app;
  uint16_t IsocInWr_app = haudio->wr_ptr;
  uint16_t true_dim = haudio->buffer_length;
  uint16_t packet_dim = haudio->paketDimension;
  uint16_t channels = haudio->channels;
  length_usb_pck = packet_dim;
  haudio->timeout = 0;
  if (epnum == (AUDIO_MIC_EP & 0x7F))
  {
    if (haudio->state == STATE_USB_IDLE)
    {
      haudio->state = STATE_USB_REQUESTS_STARTED;
      ((USBD_AUDIO_MIC_ItfTypeDef *)pdev->pUserData_UAC_MIC)->Record();
    }
    if (haudio->state == STATE_USB_BUFFER_WRITE_STARTED)
    {
      haudio->rd_ptr = haudio->rd_ptr % (true_dim);
      if (IsocInWr_app < haudio->rd_ptr)
      {
        app = ((true_dim)-haudio->rd_ptr) + IsocInWr_app;
      }
      else
      {
        app = IsocInWr_app - haudio->rd_ptr;
      }
      if (app >= (packet_dim * haudio->upper_treshold))
      {
        length_usb_pck += channels * 2;
      }
      else if (app <= (packet_dim * haudio->lower_treshold))
      {
        length_usb_pck -= channels * 2;
      }
      USBD_LL_Transmit(pdev, AUDIO_MIC_EP,
                       (uint8_t *)(&haudio->buffer[haudio->rd_ptr]),
                       length_usb_pck);
      haudio->rd_ptr += length_usb_pck;

      if (app < haudio->buffer_length / 10)
      {
        ((USBD_AUDIO_MIC_ItfTypeDef *)pdev->pUserData_UAC_MIC)->Stop();
        haudio->state = STATE_USB_IDLE;
        haudio->timeout = 0;
        memset(haudio->buffer, 0, (haudio->buffer_length + haudio->dataAmount));
      }
    }
    else
    {
      USBD_LL_Transmit(pdev, AUDIO_MIC_EP,
                       IsocInBuffDummy,
                       length_usb_pck);
    }
  }
  return USBD_OK;
}

/**
* @brief  USBD_AUDIO_EP0_RxReady
*         handle EP0 Rx Ready event
* @param  pdev: device instance
* @retval status
*/

static uint8_t USBD_AUDIO_EP0_RxReady(USBD_HandleTypeDef *pdev)
{
  USBD_AUDIO_MIC_HandleTypeDef *haudio;
  haudio = pdev->pClassData_UAC_MIC;
  if (haudio->control.cmd == AUDIO_MIC_REQ_SET_CUR)
  {
    if (haudio->control.unit == AUDIO_MIC_OUT_STREAMING_CTRL)
    {
      ((USBD_AUDIO_MIC_ItfTypeDef *)pdev->pUserData_UAC_MIC)->VolumeCtl(VOL_CUR);

      haudio->control.cmd = 0;
      haudio->control.len = 0;
      haudio->control.unit = 0;
      haudio->control.data[0] = 0;
      haudio->control.data[0] = 0;
    }
  }
  return USBD_OK;
}
/**
* @brief  USBD_AUDIO_EP0_TxReady
*         handle EP0 TRx Ready event
* @param  pdev: device instance
* @retval status
*/
static uint8_t USBD_AUDIO_EP0_TxReady(USBD_HandleTypeDef *pdev)
{
  /* Only OUT control data are processed */
  return USBD_OK;
}
/**
* @brief  USBD_AUDIO_SOF
*         handle SOF event
* @param  pdev: device instance
* @retval status
*/
static uint8_t USBD_AUDIO_SOF(USBD_HandleTypeDef *pdev)
{
  return USBD_OK;
}

/**
* @brief  USBD_AUDIO_IsoINIncomplete
*         handle data ISO IN Incomplete event
* @param  pdev: device instance
* @param  epnum: endpoint index
* @retval status
*/
static uint8_t USBD_AUDIO_IsoINIncomplete(USBD_HandleTypeDef *pdev, uint8_t epnum)
{
  return USBD_OK;
}
/**
* @brief  USBD_AUDIO_IsoOutIncomplete
*         handle data ISO OUT Incomplete event
* @param  pdev: device instance
* @param  epnum: endpoint index
* @retval status
*/
static uint8_t USBD_AUDIO_IsoOutIncomplete(USBD_HandleTypeDef *pdev, uint8_t epnum)
{
  return USBD_OK;
}
/**
* @brief  USBD_AUDIO_DataOut
*         handle data OUT Stage
* @param  pdev: device instance
* @param  epnum: endpoint index
* @retval status
*/
static uint8_t USBD_AUDIO_DataOut(USBD_HandleTypeDef *pdev,
                                  uint8_t epnum)
{
  return USBD_OK;
}

/**
* @brief  DeviceQualifierDescriptor
*         return Device Qualifier descriptor
* @param  length : pointer data length
* @retval pointer to descriptor buffer
*/
static uint8_t *USBD_AUDIO_GetDeviceQualifierDesc(uint16_t *length)
{
  *length = sizeof(USBD_AUDIO_DeviceQualifierDesc);
  return USBD_AUDIO_DeviceQualifierDesc;
}

/**
* @brief  AUDIO_REQ_GetMaximum
*         Handles the VOL_MAX Audio control request.
* @param  pdev: instance
* @param  req: setup class request
* @retval status
*/
static void AUDIO_REQ_GetMaximum(USBD_HandleTypeDef *pdev, USBD_SetupReqTypedef *req)
{
  USBD_AUDIO_MIC_HandleTypeDef *haudio;
  haudio = pdev->pClassData_UAC_MIC;

  (haudio->control.data)[0] = (uint16_t)AUDIO_MIC_VOL_MAX & 0xFF;
  (haudio->control.data)[1] = ((uint16_t)AUDIO_MIC_VOL_MAX & 0xFF00) >> 8;

  USBD_CtlSendData(pdev,
                   haudio->control.data,
                   req->wLength);
}

/**
* @brief  AUDIO_REQ_GetMinimum
*         Handles the VOL_MIN Audio control request.
* @param  pdev: instance
* @param  req: setup class request
* @retval status
*/
static void AUDIO_REQ_GetMinimum(USBD_HandleTypeDef *pdev, USBD_SetupReqTypedef *req)
{
  USBD_AUDIO_MIC_HandleTypeDef *haudio;
  haudio = pdev->pClassData_UAC_MIC;
  (haudio->control.data)[0] = (uint16_t)AUDIO_MIC_VOL_MIN & 0xFF;
  (haudio->control.data)[1] = ((uint16_t)AUDIO_MIC_VOL_MIN & 0xFF00) >> 8;
  /* Send the current mute state */
  USBD_CtlSendData(pdev,
                   haudio->control.data,
                   req->wLength);
}

/**
* @brief  AUDIO_Req_GetResolution
*         Handles the VOL_RES Audio control request.
* @param  pdev: instance
* @param  req: setup class request
* @retval status
*/
static void AUDIO_REQ_GetResolution(USBD_HandleTypeDef *pdev, USBD_SetupReqTypedef *req)
{
  USBD_AUDIO_MIC_HandleTypeDef *haudio;
  haudio = pdev->pClassData_UAC_MIC;
  (haudio->control.data)[0] = (uint16_t)AUDIO_MIC_VOL_RES & 0xFF;
  (haudio->control.data)[1] = ((uint16_t)AUDIO_MIC_VOL_RES & 0xFF00) >> 8;
  USBD_CtlSendData(pdev,
                   haudio->control.data,
                   req->wLength);
}

/**
* @brief  AUDIO_Req_GetCurrent
*         Handles the GET_CUR Audio control request.
* @param  pdev: instance
* @param  req: setup class request
* @retval status
*/
static void AUDIO_REQ_GetCurrent(USBD_HandleTypeDef *pdev, USBD_SetupReqTypedef *req)
{
  USBD_AUDIO_MIC_HandleTypeDef *haudio;
  haudio = pdev->pClassData_UAC_MIC;

  (haudio->control.data)[0] = (uint16_t)VOL_CUR & 0xFF;
  (haudio->control.data)[1] = ((uint16_t)VOL_CUR & 0xFF00) >> 8;

  USBD_CtlSendData(pdev,
                   haudio->control.data,
                   req->wLength);
}

/**
* @brief  AUDIO_Req_SetCurrent
*         Handles the SET_CUR Audio control request.
* @param  pdev: instance
* @param  req: setup class request
* @retval status
*/
static void AUDIO_REQ_SetCurrent(USBD_HandleTypeDef *pdev, USBD_SetupReqTypedef *req)
{
  USBD_AUDIO_MIC_HandleTypeDef *haudio;
  haudio = pdev->pClassData_UAC_MIC;
  if (req->wLength)
  {
    /* Prepare the reception of the buffer over EP0 */
    USBD_CtlPrepareRx(pdev,
                      (uint8_t *)&VOL_CUR,
                      req->wLength);

    haudio->control.cmd = AUDIO_MIC_REQ_SET_CUR; /* Set the request value */
    haudio->control.len = req->wLength;          /* Set the request data length */
    haudio->control.unit = HIBYTE(req->wIndex);  /* Set the request target unit */
  }
}

/**
* @}
*/

/** @defgroup USBD_AUDIO_IN_Exported_Functions
* @{
*/

/**
* @brief  USBD_AUDIO_Data_Transfer
*         Fills the USB internal buffer with audio data from user
* @param pdev: device instance
* @param audioData: audio data to be sent via USB
* @param dataAmount: number of PCM samples to be copyed
* @note Depending on the calling frequency, a coherent amount of samples must be passed to
*       the function. E.g.: assuming a Sampling frequency of 16 KHz and 1 channel,
*       you can pass 16 PCM samples if the function is called each millisecond,
*       32 samples if called every 2 milliseconds and so on.
* @retval status
*/
uint8_t USBD_AUDIO_Data_Transfer(USBD_HandleTypeDef *pdev, int16_t *audioData, uint16_t PCMSamples)
{

  USBD_AUDIO_MIC_HandleTypeDef *haudio;
  haudio = (USBD_AUDIO_MIC_HandleTypeDef *)pdev->pClassData_UAC_MIC;

  if (haudioInstance.state == STATE_USB_WAITING_FOR_INIT)
  {
    return USBD_BUSY;
  }
  uint16_t dataAmount = PCMSamples * 2; /*Bytes*/
  uint16_t true_dim = haudio->buffer_length;
  uint16_t current_data_Amount = haudio->dataAmount;
  uint16_t packet_dim = haudio->paketDimension;

  if (haudio->state == STATE_USB_REQUESTS_STARTED || current_data_Amount != dataAmount)
  {
    /*USB parameters definition, based on the amount of data passed*/
    haudio->dataAmount = dataAmount;
    uint16_t wr_rd_offset = (AUDIO_MIC_PACKET_NUM / 2) * dataAmount / packet_dim;
    haudio->wr_ptr = wr_rd_offset * packet_dim;
    haudio->rd_ptr = 0;
    haudio->upper_treshold = wr_rd_offset + 1;
    haudio->lower_treshold = wr_rd_offset - 1;
    haudio->buffer_length = (packet_dim * (dataAmount / packet_dim) * AUDIO_MIC_PACKET_NUM);

    /*Memory allocation for data buffer, depending (also) on data amount passed to the transfer function*/
    if (haudio->buffer != NULL)
    {
      USBD_free(haudio->buffer);
    }
    haudio->buffer = USBD_malloc(haudio->buffer_length + haudio->dataAmount);
    if (haudio->buffer == NULL)
    {
      return USBD_FAIL;
    }
    memset(haudio->buffer, 0, (haudio->buffer_length + haudio->dataAmount));
    haudio->state = STATE_USB_BUFFER_WRITE_STARTED;
  }
  else if (haudio->state == STATE_USB_BUFFER_WRITE_STARTED)
  {
    if (haudio->timeout++ == TIMEOUT_VALUE)
    {
      haudio->state = STATE_USB_IDLE;
      ((USBD_AUDIO_MIC_ItfTypeDef *)pdev->pUserData_UAC_MIC)->Stop();
      haudio->timeout = 0;
    }
    memcpy((uint8_t *)&haudio->buffer[haudio->wr_ptr], (uint8_t *)(audioData), dataAmount);
    haudio->wr_ptr += dataAmount;
    haudio->wr_ptr = haudio->wr_ptr % (true_dim);
    if ((haudio->wr_ptr - dataAmount) == 0)
    {
      memcpy((uint8_t *)(((uint8_t *)haudio->buffer) + true_dim), (uint8_t *)haudio->buffer, dataAmount);
    }
  }
  return USBD_OK;
}

/**
* @brief  USBD_AUDIO_RegisterInterface
* @param  fops: Audio interface callback
* @retval status
*/
uint8_t USBD_AUDIO_MIC_RegisterInterface(USBD_HandleTypeDef *pdev,
                                         USBD_AUDIO_MIC_ItfTypeDef *fops)
{
  if (fops != NULL)
  {
    pdev->pUserData_UAC_MIC = fops;
  }
  return 0;
}

void USBD_Update_Audio_MIC_DESC(uint8_t *desc, uint8_t ac_itf, uint8_t as_itf, uint8_t in_ep)
{
  desc[11] = ac_itf;
  desc[26] = as_itf;
  desc[58 + AUDIO_MIC_CHANNELS] = as_itf;
  desc[67 + AUDIO_MIC_CHANNELS] = as_itf;
  desc[95 + AUDIO_MIC_CHANNELS] = in_ep;

  AUDIO_MIC_EP = in_ep;
  AUDIO_MIC_AC_ITF_NBR = ac_itf;
  AUDIO_MIC_AS_ITF_NBR = as_itf;

  haudioInstance.paketDimension = (AUDIO_MIC_SMPL_FREQ / 1000 * AUDIO_MIC_CHANNELS * 2);
  haudioInstance.frequency = AUDIO_MIC_SMPL_FREQ;
  haudioInstance.buffer_length = haudioInstance.paketDimension * AUDIO_MIC_PACKET_NUM;
  haudioInstance.channels = AUDIO_MIC_CHANNELS;
  haudioInstance.upper_treshold = 5;
  haudioInstance.lower_treshold = 2;
  haudioInstance.state = STATE_USB_WAITING_FOR_INIT;
  haudioInstance.wr_ptr = 3 * haudioInstance.paketDimension;
  haudioInstance.rd_ptr = 0;
  haudioInstance.dataAmount = 0;
  haudioInstance.buffer = 0;
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
