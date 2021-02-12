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

#include "usbd_cdc_rndis.h"
#include "usbd_video.h"
#include "usbd_msc.h"

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

#if defined(__ICCARM__) /*!< IAR Compiler */
#pragma data_alignment = 4
#endif
/* USB COMPOSITE device Configuration Descriptor */
__ALIGN_BEGIN static uint8_t USBD_COMPOSITE_HSCfgDesc[USB_COMPOSITE_CONFIG_DESC_SIZ] __ALIGN_END =
    {
        /* Configuration Descriptor */
        USB_CONF_DESC_SIZE,                    /* bLength: Configuration Descriptor size */
        USB_DESC_TYPE_CONFIGURATION,           /* bDescriptorType: Configuration */
        LOBYTE(USB_COMPOSITE_CONFIG_DESC_SIZ), /* wTotalLength: no of returned bytes */
        HIBYTE(USB_COMPOSITE_CONFIG_DESC_SIZ),
        0x05, /* bNumInterfaces: 5 interface */
        0x01, /* bConfigurationValue: Configuration value */
        0x00, /* iConfiguration: Index of string descriptor describing the configuration */
#if (USBD_SELF_POWERED == 1U)
        0xC0, /* bmAttributes: Bus Powered according to user configuration */
#else
        0x80, /* bmAttributes: Bus Powered according to user configuration */
#endif
        USBD_MAX_POWER, /* bMaxPower in mA according to user configuration */

#if (NUMBER_OF_CDC > 0)
        /********************  CDC0 block ********************/
        /******** IAD to associate the two CDC interfaces */
        0x08,                 /* bLength */
        0x0B,                 /* bDescriptorType */
        CDC_ACM0_CMD_ITF_NBR, /* bFirstInterface */
        0x02,                 /* bInterfaceCount */
        0x02,                 /* bFunctionClass */
        0x02,                 /* bFunctionSubClass */
        0x01,                 /* bFunctionProtocol */
        0x00,                 /* iFunction (Index of string descriptor describing this function) */

        /* Interface Descriptor */
        0x09,                    /* bLength: Interface Descriptor size */
        USB_DESC_TYPE_INTERFACE, /* bDescriptorType: Interface */
        CDC_ACM0_CMD_ITF_NBR,    /* bInterfaceNumber: Number of Interface */
        0x00,                    /* bAlternateSetting: Alternate setting */
        0x01,                    /* bNumEndpoints: One endpoints used */
        0x02,                    /* bInterfaceClass: Communication Interface Class */
        0x02,                    /* bInterfaceSubClass: Abstract Control Model */
        0x01,                    /* bInterfaceProtocol: Common AT commands */
        0x00,                    /* iInterface: */

        /* Header Functional Descriptor */
        0x05, /* bLength: Endpoint Descriptor size */
        0x24, /* bDescriptorType: CS_INTERFACE */
        0x00, /* bDescriptorSubtype: Header Func Desc */
        0x10, /* bcdCDC: spec release number */
        0x01,

        /* Call Management Functional Descriptor */
        0x05,                 /* bFunctionLength */
        0x24,                 /* bDescriptorType: CS_INTERFACE */
        0x01,                 /* bDescriptorSubtype: Call Management Func Desc */
        0x00,                 /* bmCapabilities: D0+D1 */
        CDC_ACM0_COM_ITF_NBR, /* bDataInterface: 1 */

        /* ACM Functional Descriptor */
        0x04, /* bFunctionLength */
        0x24, /* bDescriptorType: CS_INTERFACE */
        0x02, /* bDescriptorSubtype: Abstract Control Management desc */
        0x02, /* bmCapabilities */

        /* Union Functional Descriptor */
        0x05,                 /* bFunctionLength */
        0x24,                 /* bDescriptorType: CS_INTERFACE */
        0x06,                 /* bDescriptorSubtype: Union func desc */
        CDC_ACM0_CMD_ITF_NBR, /* bMasterInterface: Communication class interface */
        CDC_ACM0_COM_ITF_NBR, /* bSlaveInterface0: Data Class Interface */

        /* Endpoint 2 Descriptor */
        0x07,                        /* bLength: Endpoint Descriptor size */
        USB_DESC_TYPE_ENDPOINT,      /* bDescriptorType: Endpoint */
        CDC_ACM0_CMD_EP,             /* bEndpointAddress */
        0x03,                        /* bmAttributes: Interrupt */
        LOBYTE(CDC_CMD_PACKET_SIZE), /* wMaxPacketSize: */
        HIBYTE(CDC_CMD_PACKET_SIZE),
        CDC_FS_BINTERVAL, /* bInterval: */
        /*---------------------------------------------------------------------------*/

        /* Data class interface descriptor */
        0x09,                    /* bLength: Endpoint Descriptor size */
        USB_DESC_TYPE_INTERFACE, /* bDescriptorType: */
        CDC_ACM0_COM_ITF_NBR,    /* bInterfaceNumber: Number of Interface */
        0x00,                    /* bAlternateSetting: Alternate setting */
        0x02,                    /* bNumEndpoints: Two endpoints used */
        0x0A,                    /* bInterfaceClass: CDC */
        0x00,                    /* bInterfaceSubClass: */
        0x00,                    /* bInterfaceProtocol: */
        0x00,                    /* iInterface: */

        /* Endpoint OUT Descriptor */
        0x07,                                /* bLength: Endpoint Descriptor size */
        USB_DESC_TYPE_ENDPOINT,              /* bDescriptorType: Endpoint */
        CDC_ACM0_OUT_EP,                     /* bEndpointAddress */
        0x02,                                /* bmAttributes: Bulk */
        LOBYTE(CDC_DATA_FS_MAX_PACKET_SIZE), /* wMaxPacketSize: */
        HIBYTE(CDC_DATA_FS_MAX_PACKET_SIZE),
        0x00, /* bInterval: ignore for Bulk transfer */

        /* Endpoint IN Descriptor */
        0x07,                                /* bLength: Endpoint Descriptor size */
        USB_DESC_TYPE_ENDPOINT,              /* bDescriptorType: Endpoint */
        CDC_ACM0_IN_EP,                      /* bEndpointAddress */
        0x02,                                /* bmAttributes: Bulk */
        LOBYTE(CDC_DATA_FS_MAX_PACKET_SIZE), /* wMaxPacketSize: */
        HIBYTE(CDC_DATA_FS_MAX_PACKET_SIZE),
        0x00,
#endif
#if (NUMBER_OF_CDC > 1)
        /********************  CDC1 block ********************/
        /******** IAD to associate the two CDC interfaces */
        0x08,                 /* bLength */
        0x0B,                 /* bDescriptorType */
        CDC_ACM1_CMD_ITF_NBR, /* bFirstInterface */
        0x02,                 /* bInterfaceCount */
        0x02,                 /* bFunctionClass */
        0x02,                 /* bFunctionSubClass */
        0x01,                 /* bFunctionProtocol */
        0x00,                 /* iFunction (Index of string descriptor describing this function) */

        /* Interface Descriptor */
        0x09,                    /* bLength: Interface Descriptor size */
        USB_DESC_TYPE_INTERFACE, /* bDescriptorType: Interface */
        CDC_ACM1_CMD_ITF_NBR,    /* bInterfaceNumber: Number of Interface */
        0x00,                    /* bAlternateSetting: Alternate setting */
        0x01,                    /* bNumEndpoints: One endpoints used */
        0x02,                    /* bInterfaceClass: Communication Interface Class */
        0x02,                    /* bInterfaceSubClass: Abstract Control Model */
        0x01,                    /* bInterfaceProtocol: Common AT commands */
        0x00,                    /* iInterface: */

        /* Header Functional Descriptor */
        0x05, /* bLength: Endpoint Descriptor size */
        0x24, /* bDescriptorType: CS_INTERFACE */
        0x00, /* bDescriptorSubtype: Header Func Desc */
        0x10, /* bcdCDC: spec release number */
        0x01,

        /* Call Management Functional Descriptor */
        0x05,                 /* bFunctionLength */
        0x24,                 /* bDescriptorType: CS_INTERFACE */
        0x01,                 /* bDescriptorSubtype: Call Management Func Desc */
        0x00,                 /* bmCapabilities: D0+D1 */
        CDC_ACM1_COM_ITF_NBR, /* bDataInterface: 1 */

        /* ACM Functional Descriptor */
        0x04, /* bFunctionLength */
        0x24, /* bDescriptorType: CS_INTERFACE */
        0x02, /* bDescriptorSubtype: Abstract Control Management desc */
        0x02, /* bmCapabilities */

        /* Union Functional Descriptor */
        0x05,                 /* bFunctionLength */
        0x24,                 /* bDescriptorType: CS_INTERFACE */
        0x06,                 /* bDescriptorSubtype: Union func desc */
        CDC_ACM1_CMD_ITF_NBR, /* bMasterInterface: Communication class interface */
        CDC_ACM1_COM_ITF_NBR, /* bSlaveInterface0: Data Class Interface */

        /* Endpoint 2 Descriptor */
        0x07,                        /* bLength: Endpoint Descriptor size */
        USB_DESC_TYPE_ENDPOINT,      /* bDescriptorType: Endpoint */
        CDC_ACM1_CMD_EP,             /* bEndpointAddress */
        0x03,                        /* bmAttributes: Interrupt */
        LOBYTE(CDC_CMD_PACKET_SIZE), /* wMaxPacketSize: */
        HIBYTE(CDC_CMD_PACKET_SIZE),
        CDC_FS_BINTERVAL, /* bInterval: */
        /*---------------------------------------------------------------------------*/

        /* Data class interface descriptor */
        0x09,                    /* bLength: Endpoint Descriptor size */
        USB_DESC_TYPE_INTERFACE, /* bDescriptorType: */
        CDC_ACM1_COM_ITF_NBR,    /* bInterfaceNumber: Number of Interface */
        0x00,                    /* bAlternateSetting: Alternate setting */
        0x02,                    /* bNumEndpoints: Two endpoints used */
        0x0A,                    /* bInterfaceClass: CDC */
        0x00,                    /* bInterfaceSubClass: */
        0x00,                    /* bInterfaceProtocol: */
        0x00,                    /* iInterface: */

        /* Endpoint OUT Descriptor */
        0x07,                                /* bLength: Endpoint Descriptor size */
        USB_DESC_TYPE_ENDPOINT,              /* bDescriptorType: Endpoint */
        CDC_ACM1_OUT_EP,                     /* bEndpointAddress */
        0x02,                                /* bmAttributes: Bulk */
        LOBYTE(CDC_DATA_FS_MAX_PACKET_SIZE), /* wMaxPacketSize: */
        HIBYTE(CDC_DATA_FS_MAX_PACKET_SIZE),
        0x00, /* bInterval: ignore for Bulk transfer */

        /* Endpoint IN Descriptor */
        0x07,                                /* bLength: Endpoint Descriptor size */
        USB_DESC_TYPE_ENDPOINT,              /* bDescriptorType: Endpoint */
        CDC_ACM1_IN_EP,                      /* bEndpointAddress */
        0x02,                                /* bmAttributes: Bulk */
        LOBYTE(CDC_DATA_FS_MAX_PACKET_SIZE), /* wMaxPacketSize: */
        HIBYTE(CDC_DATA_FS_MAX_PACKET_SIZE),
        0x00, /* bInterval: ignore for Bulk transfer */
#endif
#if (NUMBER_OF_CDC > 2)
        /********************  CDC2 block ********************/
        /******** IAD to associate the two CDC interfaces */
        0x08,                 /* bLength */
        0x0B,                 /* bDescriptorType */
        CDC_ACM2_CMD_ITF_NBR, /* bFirstInterface */
        0x02,                 /* bInterfaceCount */
        0x02,                 /* bFunctionClass */
        0x02,                 /* bFunctionSubClass */
        0x01,                 /* bFunctionProtocol */
        0x00,                 /* iFunction (Index of string descriptor describing this function) */

        /* Interface Descriptor */
        0x09,                    /* bLength: Interface Descriptor size */
        USB_DESC_TYPE_INTERFACE, /* bDescriptorType: Interface */
        CDC_ACM2_CMD_ITF_NBR,    /* bInterfaceNumber: Number of Interface */
        0x00,                    /* bAlternateSetting: Alternate setting */
        0x01,                    /* bNumEndpoints: One endpoints used */
        0x02,                    /* bInterfaceClass: Communication Interface Class */
        0x02,                    /* bInterfaceSubClass: Abstract Control Model */
        0x01,                    /* bInterfaceProtocol: Common AT commands */
        0x00,                    /* iInterface: */

        /* Header Functional Descriptor */
        0x05, /* bLength: Endpoint Descriptor size */
        0x24, /* bDescriptorType: CS_INTERFACE */
        0x00, /* bDescriptorSubtype: Header Func Desc */
        0x10, /* bcdCDC: spec release number */
        0x01,

        /* Call Management Functional Descriptor */
        0x05,                 /* bFunctionLength */
        0x24,                 /* bDescriptorType: CS_INTERFACE */
        0x01,                 /* bDescriptorSubtype: Call Management Func Desc */
        0x00,                 /* bmCapabilities: D0+D1 */
        CDC_ACM2_COM_ITF_NBR, /* bDataInterface: 1 */

        /* ACM Functional Descriptor */
        0x04, /* bFunctionLength */
        0x24, /* bDescriptorType: CS_INTERFACE */
        0x02, /* bDescriptorSubtype: Abstract Control Management desc */
        0x02, /* bmCapabilities */

        /* Union Functional Descriptor */
        0x05,                 /* bFunctionLength */
        0x24,                 /* bDescriptorType: CS_INTERFACE */
        0x06,                 /* bDescriptorSubtype: Union func desc */
        CDC_ACM2_CMD_ITF_NBR, /* bMasterInterface: Communication class interface */
        CDC_ACM2_COM_ITF_NBR, /* bSlaveInterface0: Data Class Interface */

        /* Endpoint 2 Descriptor */
        0x07,                        /* bLength: Endpoint Descriptor size */
        USB_DESC_TYPE_ENDPOINT,      /* bDescriptorType: Endpoint */
        CDC_ACM2_CMD_EP,             /* bEndpointAddress */
        0x03,                        /* bmAttributes: Interrupt */
        LOBYTE(CDC_CMD_PACKET_SIZE), /* wMaxPacketSize: */
        HIBYTE(CDC_CMD_PACKET_SIZE),
        CDC_FS_BINTERVAL, /* bInterval: */
        /*---------------------------------------------------------------------------*/

        /* Data class interface descriptor */
        0x09,                    /* bLength: Endpoint Descriptor size */
        USB_DESC_TYPE_INTERFACE, /* bDescriptorType: */
        CDC_ACM2_COM_ITF_NBR,    /* bInterfaceNumber: Number of Interface */
        0x00,                    /* bAlternateSetting: Alternate setting */
        0x02,                    /* bNumEndpoints: Two endpoints used */
        0x0A,                    /* bInterfaceClass: CDC */
        0x00,                    /* bInterfaceSubClass: */
        0x00,                    /* bInterfaceProtocol: */
        0x00,                    /* iInterface: */

        /* Endpoint OUT Descriptor */
        0x07,                                /* bLength: Endpoint Descriptor size */
        USB_DESC_TYPE_ENDPOINT,              /* bDescriptorType: Endpoint */
        CDC_ACM2_OUT_EP,                     /* bEndpointAddress */
        0x02,                                /* bmAttributes: Bulk */
        LOBYTE(CDC_DATA_FS_MAX_PACKET_SIZE), /* wMaxPacketSize: */
        HIBYTE(CDC_DATA_FS_MAX_PACKET_SIZE),
        0x00, /* bInterval: ignore for Bulk transfer */

        /* Endpoint IN Descriptor */
        0x07,                                /* bLength: Endpoint Descriptor size */
        USB_DESC_TYPE_ENDPOINT,              /* bDescriptorType: Endpoint */
        CDC_ACM2_IN_EP,                      /* bEndpointAddress */
        0x02,                                /* bmAttributes: Bulk */
        LOBYTE(CDC_DATA_FS_MAX_PACKET_SIZE), /* wMaxPacketSize: */
        HIBYTE(CDC_DATA_FS_MAX_PACKET_SIZE),
        0x00,
#endif
#if (USBD_USE_CDC_RNDIS == 1)
        /**********  RNDIS START **************/
        /* IAD descriptor */
        0x08,                  /* bLength */
        0x0B,                  /* bDescriptorType */
        CDC_RNDIS_CMD_ITF_NBR, /* bFirstInterface */
        0x02,                  /* bInterfaceCount */
        0xE0,                  /* bFunctionClass (Wireless Controller) */
        0x01,                  /* bFunctionSubClass */
        0x03,                  /* bFunctionProtocol */
        0x00,                  /* iFunction */

        /*---------------------------------------------------------------------------*/
        /* Interface Descriptor */
        0x09,                    /* bLength: Interface Descriptor size */
        USB_DESC_TYPE_INTERFACE, /* bDescriptorType: Interface descriptor type */
        CDC_RNDIS_CMD_ITF_NBR,   /* bInterfaceNumber: Number of Interface */
        0x00,                    /* bAlternateSetting: Alternate setting */
        0x01,                    /* bNumEndpoints: One endpoint used */
        0x02,                    /* bInterfaceClass: Communication Interface Class */
        0x02,                    /* bInterfaceSubClass:Abstract Control Model */
        0xFF,                    /* bInterfaceProtocol: Common AT commands */
        0x00,                    /* iInterface: */

        /* Header Functional Descriptor */
        0x05, /* bLength: Endpoint Descriptor size */
        0x24, /* bDescriptorType: CS_INTERFACE */
        0x00, /* bDescriptorSubtype: Header functional descriptor */
        0x10, /* bcdCDC: spec release number: 1.20 */
        0x01,

        /* Call Management Functional Descriptor */
        0x05,                  /* bFunctionLength */
        0x24,                  /* bDescriptorType: CS_INTERFACE */
        0x01,                  /* bDescriptorSubtype: Call Management Func Desc */
        0x00,                  /* bmCapabilities: D0+D1 */
        CDC_RNDIS_COM_ITF_NBR, /* bDataInterface: 1 */

        /* ACM Functional Descriptor */
        0x04, /* bFunctionLength */
        0x24, /* bDescriptorType: CS_INTERFACE */
        0x02, /* bDescriptorSubtype: Abstract Control Management desc */
        0x00, /* bmCapabilities */

        /* Union Functional Descriptor */
        0x05,                  /* bFunctionLength */
        0x24,                  /* bDescriptorType: CS_INTERFACE */
        0x06,                  /* bDescriptorSubtype: Union functional descriptor */
        CDC_RNDIS_CMD_ITF_NBR, /* bMasterInterface: Communication class interface */
        CDC_RNDIS_COM_ITF_NBR, /* bSlaveInterface0: Data Class Interface */

        /* Notification Endpoint Descriptor */
        0x07,                              /* bLength: Endpoint Descriptor size */
        USB_DESC_TYPE_ENDPOINT,            /* bDescriptorType: Endpoint */
        CDC_RNDIS_CMD_EP,                  /* bEndpointAddress */
        0x03,                              /* bmAttributes: Interrupt */
        LOBYTE(CDC_RNDIS_CMD_PACKET_SIZE), /* wMaxPacketSize: */
        HIBYTE(CDC_RNDIS_CMD_PACKET_SIZE),
        CDC_RNDIS_HS_BINTERVAL, /* bInterval */

        /*---------------------------------------------------------------------------*/
        /* Data class interface descriptor */
        0x09,                    /* bLength: Endpoint Descriptor size */
        USB_DESC_TYPE_INTERFACE, /* bDescriptorType: */
        CDC_RNDIS_COM_ITF_NBR,   /* bInterfaceNumber: Number of Interface */
        0x00,                    /* bAlternateSetting: Alternate setting */
        0x02,                    /* bNumEndpoints: Two endpoints used */
        0x0A,                    /* bInterfaceClass: CDC */
        0x00,                    /* bInterfaceSubClass: */
        0x00,                    /* bInterfaceProtocol: */
        0x00,                    /* iInterface: */

        /* Endpoint OUT Descriptor */
        0x07,                                      /* bLength: Endpoint Descriptor size */
        USB_DESC_TYPE_ENDPOINT,                    /* bDescriptorType: Endpoint */
        CDC_RNDIS_OUT_EP,                          /* bEndpointAddress */
        0x02,                                      /* bmAttributes: Bulk */
        LOBYTE(CDC_RNDIS_DATA_HS_MAX_PACKET_SIZE), /* wMaxPacketSize: */
        HIBYTE(CDC_RNDIS_DATA_HS_MAX_PACKET_SIZE),
        0xFF, /* bInterval: ignore for Bulk transfer */

        /* Endpoint IN Descriptor */
        0x07,                                      /* bLength: Endpoint Descriptor size */
        USB_DESC_TYPE_ENDPOINT,                    /* bDescriptorType: Endpoint */
        CDC_RNDIS_IN_EP,                           /* bEndpointAddress */
        0x02,                                      /* bmAttributes: Bulk */
        LOBYTE(CDC_RNDIS_DATA_HS_MAX_PACKET_SIZE), /* wMaxPacketSize: */
        HIBYTE(CDC_RNDIS_DATA_HS_MAX_PACKET_SIZE),
        0xFF, /* bInterval: ignore for Bulk transfer */
              /**********  RNDIS END **************/
#endif
#if (USBD_USE_UVC == 1)
        /**********  UVC START **************/
        /* Interface Association Descriptor */
        USB_IAD_DESC_SIZE,             /* bLength: Interface Association Descriptor size */
        USB_DESC_TYPE_IAD,             /* bDescriptorType: interface association */
        UVC_VC_IF_NUM,                 /* bFirstInterface */
        0x02,                          /* bInterfaceCount */
        UVC_CC_VIDEO,                  /* bFunctionClass: Video class */
        SC_VIDEO_INTERFACE_COLLECTION, /* bFunctionSubClass: Video Interface Collection */
        PC_PROTOCOL_UNDEFINED,         /* bInterfaceProtocol: protocol undefined */
        0x00,                          /* iFunction */

        /* Standard VC (Video Control) Interface Descriptor  = interface 0 */
        USB_IF_DESC_SIZE,        /* bLength: interface descriptor size */
        USB_DESC_TYPE_INTERFACE, /* bDescriptorType: interface */
        UVC_VC_IF_NUM,           /* bInterfaceNumber: interface number */
        0x00,                    /* bAlternateSetting: index of this alternate setting */
        0x00,                    /* bNumEndpoints: No endpoints used for this interface */
        UVC_CC_VIDEO,            /* bInterfaceClass: Video Class */
        SC_VIDEOCONTROL,         /* bInterfaceSubClass: Video Control */
        PC_PROTOCOL_UNDEFINED,   /* bInterfaceProtocol: protocol is undefined */
        0x00,                    /* iFunction: index of string descriptor relative to this item */

        /* Class-specific VC Interface Descriptor */
        VIDEO_VC_IF_HEADER_DESC_SIZE, /* bLength */
        CS_INTERFACE,                 /* bDescriptorType */
        VC_HEADER,                    /* bDescriptorSubtype */
        LOBYTE(UVC_VERSION),
        HIBYTE(UVC_VERSION), /* bcdUVC: UVC1.0 or UVC1.1 revision */
        VS_FRAME_DESC_SIZE,  /* wTotalLength: total size of class-specific descriptors */
        0x00,
        0x00, /* dwClockFrequency: not used. 48 Mhz value is set, but not used */
        0x6C,
        0xDC,
        0x02,
        0x01,          /* bInCollection: number of streaming interfaces */
        UVC_VS_IF_NUM, /* baInterfaceNr(1): VideoStreaming interface 1 is part of VC interface */

        /* Input Terminal Descriptor */
        VIDEO_IN_TERMINAL_DESC_SIZE, /* bLength: Input terminal descriptor size */
        CS_INTERFACE,                /* bDescriptorType: INTERFACE */
        VC_INPUT_TERMINAL,           /* bDescriptorSubtype: INPUT_TERMINAL */
        0x01,                        /* bTerminalID: ID of this Terminal */
        LOBYTE(ITT_VENDOR_SPECIFIC), /* wTerminalType: 0x0200 ITT_VENDOR_SPECIFIC */
        HIBYTE(ITT_VENDOR_SPECIFIC),
        0x00, /* bAssocTerminal: no Terminal is associated */
        0x00, /* iTerminal: index of string descriptor relative to this item */

        /* Output Terminal Descriptor */
        VIDEO_OUT_TERMINAL_DESC_SIZE, /* bLength: output terminal descriptor size */
        CS_INTERFACE,                 /* bDescriptorType */
        VC_OUTPUT_TERMINAL,           /* bDescriptorSubtype */
        0x02,                         /* bTerminalID */
        LOBYTE(TT_STREAMING),         /* wTerminalType: USB streaming terminal */
        HIBYTE(TT_STREAMING),
        0x00, /* bAssocTerminal: no Terminal is associated */
        0x01, /* bSourceID: input is connected to output unit ID 1 */
        0x00, /* iTerminal: index of string descriptor relative to this item */

        /* Standard VS (Video Streaming) Interface Descriptor = interface 1, alternate setting 0 = Zero Bandwidth (when no data are sent from the device) */
        USB_IF_DESC_SIZE,        /* bLength: interface descriptor size */
        USB_DESC_TYPE_INTERFACE, /* bDescriptorType */
        UVC_VS_IF_NUM,           /* bInterfaceNumber */
        0x00,                    /* bAlternateSetting */
        0x00,                    /* bNumEndpoints: no endpoints used for alternate setting 0 */
        UVC_CC_VIDEO,            /* bInterfaceClass */
        SC_VIDEOSTREAMING,       /* bInterfaceSubClass */
        PC_PROTOCOL_UNDEFINED,   /* bInterfaceProtocol */
        0x00,                    /* iInterface: index of string descriptor relative to this item */

        /* Class-specific VS Header Descriptor (Input) */
        VIDEO_VS_IF_IN_HEADER_DESC_SIZE, /* bLength */
        CS_INTERFACE,                    /* bDescriptorType */
        VS_INPUT_HEADER,                 /* bDescriptorSubtype */
        0x01,                            /* bNumFormats: 1 format descriptor is used */
        VC_HEADER_SIZE,
        0x00,      /* Total size of Video Control Specific Descriptors */
        UVC_IN_EP, /* bEndPointAddress: In endpoint is used for the alternate setting */
        0x00,      /* bmInfo: dynamic format change not supported */
        0x02,      /* bTerminalLink: output to terminal ID 2 */
        0x00,      /* bStillCaptureMethod: not supported */
        0x00,      /* bTriggerSupport: not supported */
        0x00,      /* bTriggerUsage: not supported */
        0x01,      /* bControlSize: 1 byte field size */
        0x00,      /* bmaControls: No specific controls used */

        /* Payload Format Descriptor */
        VS_FORMAT_DESC_SIZE, /* blength */
        CS_INTERFACE,        /* bDescriptorType */
        VS_FORMAT_SUBTYPE,   /* bDescriptorSubType */
        0x01,                /* bFormatIndex */
        0x01,                /* bNumFrameDescriptor */
#ifdef USBD_UVC_FORMAT_UNCOMPRESSED
        DBVAL(UVC_UNCOMPRESSED_GUID), /* Giud Format: YUY2 {32595559-0000-0010-8000-00AA00389B71} */
        0x00,
        0x00,
        0x10,
        0x00,
        0x80,
        0x00,
        0x00,
        0xAA,
        0x00,
        0x38,
        0x9B,
        0x71,
        UVC_BITS_PER_PIXEL, /* bBitsPerPixel : Number of bits per pixel */
#else
        0x01, /* bmFlags: FixedSizeSamples */
#endif
        0x01, /* bDefaultFrameIndex: default frame used is frame 1 (only one frame used) */
        0x00, /* bAspectRatioX: not required by specification */
        0x00, /* bAspectRatioY: not required by specification */
        0x00, /* bInterlaceFlags: non interlaced stream */
        0x00, /* bCopyProtect: no protection restrictions */

        /* Class-specific VS (Video Streaming) Frame Descriptor */
        VS_FRAME_DESC_SIZE,                      /* bLength */
        CS_INTERFACE,                            /* bDescriptorType */
        VS_FRAME_SUBTYPE,                        /* bDescriptorSubType */
        0x01,                                    /* bFrameIndex */
        0x02,                                    /* bmCapabilities: fixed frame rate supported */
        WBVAL(UVC_WIDTH),                        /* wWidth: Image Frame Width */
        WBVAL(UVC_HEIGHT),                       /* wHeight: Image Frame Height */
        DBVAL(UVC_MIN_BIT_RATE(UVC_CAM_FPS_HS)), /* dwMinBitRate: Minimum supported bit rate in bits/s  */
        DBVAL(UVC_MAX_BIT_RATE(UVC_CAM_FPS_HS)), /* dwMaxBitRate: Maximum supported bit rate in bits/s  */
        DBVAL(UVC_MAX_FRAME_SIZE),               /* dwMaxVideoFrameBufSize: Maximum video frame size, in bytes */
        DBVAL(UVC_INTERVAL(UVC_CAM_FPS_HS)),     /* dwDefaultFrameInterval: following number of FPS */
        0x01,                                    /* bFrameIntervalType: Discrete frame interval type */
        DBVAL(UVC_INTERVAL(UVC_CAM_FPS_HS)),     /* dwMinFrameInterval: One supported value of interval (FPS) */

#ifdef USBD_UVC_FORMAT_UNCOMPRESSED
        /* Color Matching Descriptor */
        VS_COLOR_MATCHING_DESC_SIZE, /* bLength */
        CS_INTERFACE,                /* bDescriptorType: CS_INTERFACE */
        VS_COLORFORMAT,              /* bDescriptorSubType: VS_COLORFORMAT */
        UVC_COLOR_PRIMARIE,          /* bColorPrimarie: 1: BT.709, sRGB (default) */
        UVC_TFR_CHARACTERISTICS,     /* bTransferCharacteristics: 1: BT.709 (default) */
        UVC_MATRIX_COEFFICIENTS,     /* bMatrixCoefficients: 4: BT.601, (default) */
#endif

        /* Standard VS Interface Descriptor  = interface 1, alternate setting 1 = data transfer mode  */
        USB_IF_DESC_SIZE,        /* bLength */
        USB_DESC_TYPE_INTERFACE, /* bDescriptorType */
        UVC_VS_IF_NUM,           /* bInterfaceNumber */
        0x01,                    /* bAlternateSetting */
        0x01,                    /* bNumEndpoints: one endpoint is used */
        UVC_CC_VIDEO,            /* bInterfaceClass */
        SC_VIDEOSTREAMING,       /* bInterfaceSubClass */
        PC_PROTOCOL_UNDEFINED,   /* bInterfaceProtocol */
        0x00,                    /* iInterface: index of string descriptor relative to this item */

        /* Standard VS (Video Streaming) data Endpoint */
        USB_EP_DESC_SIZE,       /* bLength */
        USB_DESC_TYPE_ENDPOINT, /* bDescriptorType */
        UVC_IN_EP,              /* bEndpointAddress */
        0x05,                   /* bmAttributes: ISO transfer */
        LOBYTE(UVC_ISO_HS_MPS), /* wMaxPacketSize */
        HIBYTE(UVC_ISO_HS_MPS),
        0x01, /* bInterval: 1 frame interval */
              /**********  UVC END **************/
#endif
#if (USBD_USE_MSC == 1)
        /**********  MSC START ************/
        /********************  Mass Storage interface ********************/
        0x09,        /* bLength: Interface Descriptor size */
        0x04,        /* bDescriptorType: */
        MSC_ITF_NBR, /* bInterfaceNumber: Number of Interface */
        0x00,        /* bAlternateSetting: Alternate setting */
        0x02,        /* bNumEndpoints */
        0x08,        /* bInterfaceClass: MSC Class */
        0x06,        /* bInterfaceSubClass : SCSI transparent */
        0x50,        /* nInterfaceProtocol */
        0x05,        /* iInterface: */
        /********************  Mass Storage Endpoints ********************/
        0x07,          /* Endpoint descriptor length = 7 */
        0x05,          /* Endpoint descriptor type */
        MSC_EPIN_ADDR, /* Endpoint address (IN, address 1) */
        0x02,          /* Bulk endpoint type */
        LOBYTE(MSC_MAX_HS_PACKET),
        HIBYTE(MSC_MAX_HS_PACKET),
        0x00, /* Polling interval in milliseconds */

        0x07,           /* Endpoint descriptor length = 7 */
        0x05,           /* Endpoint descriptor type */
        MSC_EPOUT_ADDR, /* Endpoint address (OUT, address 1) */
        0x02,           /* Bulk endpoint type */
        LOBYTE(MSC_MAX_HS_PACKET),
        HIBYTE(MSC_MAX_HS_PACKET),
        0x00 /* Polling interval in milliseconds */
             /**********  MSC END **************/
#endif

#if (USBD_USE_UAC_MIC == 1)
        /* USB Microphone Standard interface descriptor */
        0x09,                          /* bLength */
        USB_INTERFACE_DESCRIPTOR_TYPE, /* bDescriptorType */
        0x00,                          /* bInterfaceNumber */
        0x00,                          /* bAlternateSetting */
        0x00,                          /* bNumEndpoints */
        USB_DEVICE_CLASS_AUDIO,        /* bInterfaceClass */
        AUDIO_SUBCLASS_AUDIOCONTROL,   /* bInterfaceSubClass */
        AUDIO_PROTOCOL_UNDEFINED,      /* bInterfaceProtocol */
        0x00,                          /* iInterface */

        /* USB Microphone Class-specific AC Interface Descriptor */
        0x09,                            /* bLength */
        AUDIO_INTERFACE_DESCRIPTOR_TYPE, /* bDescriptorType */
        AUDIO_CONTROL_HEADER,            /* bDescriptorSubtype */
        0x00,                            /* 1.00, bcdADC */
        0x01,
        0x25 + USBD_AUDIO_IN_CHANNELS, /* wTotalLength = 37+AUDIO_CHANNELS*/
        0x00,
        0x01, /* bInCollection */
        0x01, /* baInterfaceNr */

        /* USB Microphone Input Terminal Descriptor */
        AUDIO_INPUT_TERMINAL_DESC_SIZE,  /* bLength */
        AUDIO_INTERFACE_DESCRIPTOR_TYPE, /* bDescriptorType */
        AUDIO_CONTROL_INPUT_TERMINAL,    /* bDescriptorSubtype */
        0x01,                            /* bTerminalID */
        0x01,                            /* wTerminalType AUDIO_TERMINAL_USB_MICROPHONE   0x0201 */
        0x02,
        0x00,                   /* bAssocTerminal */
        USBD_AUDIO_IN_CHANNELS, /* bNrChannels */
#if (USBD_AUDIO_IN_CHANNELS == 1)
        0x00, /* wChannelConfig 0x0000  Mono */
        0x00,
#else
        0x03, /* wChannelConfig 0x0003  Stereo */
        0x00,
#endif
        0x00, /* iChannelNames */
        0x00, /* iTerminal */

        /* USB Microphone Audio Feature Unit Descriptor */
        0x07 + USBD_AUDIO_IN_CHANNELS + 1, /* bLength */
        AUDIO_INTERFACE_DESCRIPTOR_TYPE,   /* bDescriptorType */
        AUDIO_CONTROL_FEATURE_UNIT,        /* bDescriptorSubtype */
        0x02,                              /* bUnitID */
        0x01,                              /* bSourceID */
        0x01,                              /* bControlSize */
        index = 47,

#if (USBD_AUDIO_IN_CHANNELS == 1)
        0x02,
        0x00,
#else
        0x00,
        0x02,
        0x02,
#endif

#if (USBD_AUDIO_IN_CHANNELS > 2)
        0x02,
#endif

#if (USBD_AUDIO_IN_CHANNELS > 3)
        0x02,
#endif

#if (USBD_AUDIO_IN_CHANNELS > 4)
        0x02,
#endif

#if (USBD_AUDIO_IN_CHANNELS > 5)
        0x02,
#endif

#if (USBD_AUDIO_IN_CHANNELS > 6)
        0x02,
#endif

#if (USBD_AUDIO_IN_CHANNELS > 7)
        0x02,
#endif
        0x00, /* iTerminal */

        /*USB Microphone Output Terminal Descriptor */
        0x09,                            /* bLength */
        AUDIO_INTERFACE_DESCRIPTOR_TYPE, /* bDescriptorType */
        AUDIO_CONTROL_OUTPUT_TERMINAL,   /* bDescriptorSubtype */
        0x03,                            /* bTerminalID */
        0x01,                            /* wTerminalType AUDIO_TERMINAL_USB_STREAMING 0x0101*/
        0x01,
        0x00,
        0x02,
        0x00,

        /* USB Microphone Standard AS Interface Descriptor - Audio Streaming Zero Bandwith */
        /* Interface 1, Alternate Setting 0                                             */
        0x09,                          /* bLength */
        USB_INTERFACE_DESCRIPTOR_TYPE, /* bDescriptorType */
        0x01,                          /* bInterfaceNumber */
        0x00,                          /* bAlternateSetting */
        0x00,                          /* bNumEndpoints */
        USB_DEVICE_CLASS_AUDIO,        /* bInterfaceClass */
        AUDIO_SUBCLASS_AUDIOSTREAMING, /* bInterfaceSubClass */
        AUDIO_PROTOCOL_UNDEFINED,      /* bInterfaceProtocol */
        0x00,
        /* USB Microphone Standard AS Interface Descriptor - Audio Streaming Operational */

        /* Interface 1, Alternate Setting 1                                           */
        0x09,                          /* bLength */
        USB_INTERFACE_DESCRIPTOR_TYPE, /* bDescriptorType */
        0x01,                          /* bInterfaceNumber */
        0x01,                          /* bAlternateSetting */
        0x01,                          /* bNumEndpoints */
        USB_DEVICE_CLASS_AUDIO,        /* bInterfaceClass */
        AUDIO_SUBCLASS_AUDIOSTREAMING, /* bInterfaceSubClass */
        AUDIO_PROTOCOL_UNDEFINED,      /* bInterfaceProtocol */
        0x00,                          /* iInterface */

        /* USB Microphone Audio Streaming Interface Descriptor */
        AUDIO_STREAMING_INTERFACE_DESC_SIZE, /* bLength */
        AUDIO_INTERFACE_DESCRIPTOR_TYPE,     /* bDescriptorType */
        AUDIO_STREAMING_GENERAL,             /* bDescriptorSubtype */
        0x03,                                /* bTerminalLink */
        0x01,                                /* bDelay */
        0x01,                                /* wFormatTag AUDIO_FORMAT_PCM  0x0001*/
        0x00,

        /* USB Microphone Audio Type I Format Interface Descriptor */
        0x0B,                            /* bLength */
        AUDIO_INTERFACE_DESCRIPTOR_TYPE, /* bDescriptorType */
        AUDIO_STREAMING_FORMAT_TYPE,     /* bDescriptorSubtype */
        AUDIO_FORMAT_TYPE_I,             /* bFormatType */
        USBD_AUDIO_IN_CHANNELS,          /* bNrChannels */
        0x02,                            /* bSubFrameSize */
        16,                              /* bBitResolution */
        0x01,                            /* bSamFreqType */
        samplingFrequency & 0xff,        /* tSamFreq 8000 = 0x1F40 */
        (samplingFrequency >> 8) & 0xff,
        samplingFrequency >> 16,

        /* Endpoint 1 - Standard Descriptor */
        AUDIO_STANDARD_ENDPOINT_DESC_SIZE,                                    /* bLength */
        0x05,                                                                 /* bDescriptorType */
        AUDIO_IN_EP,                                                          /* bEndpointAddress 1 in endpoint*/
        0x05,                                                                 /* bmAttributes */
        ((samplingFrequency / 1000 + 2) * USBD_AUDIO_IN_CHANNELS * 2) & 0xFF, /* wMaxPacketSize */
        ((samplingFrequency / 1000 + 2) * USBD_AUDIO_IN_CHANNELS * 2) >> 8,
        0x01, /* bInterval */
        0x00, /* bRefresh */
        0x00, /* bSynchAddress */

        /* Endpoint - Audio Streaming Descriptor*/
        AUDIO_STREAMING_ENDPOINT_DESC_SIZE, /* bLength */
        AUDIO_ENDPOINT_DESCRIPTOR_TYPE,     /* bDescriptorType */
        AUDIO_ENDPOINT_GENERAL,             /* bDescriptor */
        0x00,                               /* bmAttributes */
        0x00,                               /* bLockDelayUnits */
        0x00,                               /* wLockDelay */
        0x00,
#endif

#if (USBD_USE_UAC_SPKR == 1)
        /* USB Speaker Standard interface descriptor */
        AUDIO_INTERFACE_DESC_SIZE,   /* bLength */
        USB_DESC_TYPE_INTERFACE,     /* bDescriptorType */
        AUDIO_OUT_AC_ITF_NBR,        /* bInterfaceNumber */
        0x00,                        /* bAlternateSetting */
        0x00,                        /* bNumEndpoints */
        USB_DEVICE_CLASS_AUDIO,      /* bInterfaceClass */
        AUDIO_SUBCLASS_AUDIOCONTROL, /* bInterfaceSubClass */
        AUDIO_PROTOCOL_UNDEFINED,    /* bInterfaceProtocol */
        0x00,                        /* iInterface */
        /* 09 byte*/

        /* USB Speaker Class-specific AC Interface Descriptor */
        AUDIO_INTERFACE_DESC_SIZE,       /* bLength */
        AUDIO_INTERFACE_DESCRIPTOR_TYPE, /* bDescriptorType */
        AUDIO_CONTROL_HEADER,            /* bDescriptorSubtype */
        0x00,                            /* 1.00, bcdADC */
        0x01,
        0x27, /* wTotalLength = 39*/
        0x00,
        0x01,                 /* bInCollection */
        AUDIO_OUT_AS_ITF_NBR, /* baInterfaceNr */
        /* 09 byte*/

        /* USB Speaker Input Terminal Descriptor */
        AUDIO_INPUT_TERMINAL_DESC_SIZE,  /* bLength */
        AUDIO_INTERFACE_DESCRIPTOR_TYPE, /* bDescriptorType */
        AUDIO_CONTROL_INPUT_TERMINAL,    /* bDescriptorSubtype */
        0x01,                            /* bTerminalID */
        0x01,                            /* wTerminalType AUDIO_TERMINAL_USB_STREAMING   0x0101 */
        0x01,
        0x00, /* bAssocTerminal */
        0x01, /* bNrChannels */
        0x00, /* wChannelConfig 0x0000  Mono */
        0x00,
        0x00, /* iChannelNames */
        0x00, /* iTerminal */
        /* 12 byte*/

        /* USB Speaker Audio Feature Unit Descriptor */
        0x09,                            /* bLength */
        AUDIO_INTERFACE_DESCRIPTOR_TYPE, /* bDescriptorType */
        AUDIO_CONTROL_FEATURE_UNIT,      /* bDescriptorSubtype */
        AUDIO_OUT_STREAMING_CTRL,        /* bUnitID */
        0x01,                            /* bSourceID */
        0x01,                            /* bControlSize */
        AUDIO_CONTROL_MUTE,              /* bmaControls(0) */
        0x00,                            /* bmaControls(1) */
        0x00,                            /* iTerminal */
        /* 09 byte*/

        /*USB Speaker Output Terminal Descriptor */
        0x09,                            /* bLength */
        AUDIO_INTERFACE_DESCRIPTOR_TYPE, /* bDescriptorType */
        AUDIO_CONTROL_OUTPUT_TERMINAL,   /* bDescriptorSubtype */
        0x03,                            /* bTerminalID */
        0x01,                            /* wTerminalType  0x0301*/
        0x03,
        0x00, /* bAssocTerminal */
        0x02, /* bSourceID */
        0x00, /* iTerminal */
        /* 09 byte*/

        /* USB Speaker Standard AS Interface Descriptor - Audio Streaming Zero Bandwidth */
        /* Interface 1, Alternate Setting 0                                             */
        AUDIO_INTERFACE_DESC_SIZE,     /* bLength */
        USB_DESC_TYPE_INTERFACE,       /* bDescriptorType */
        AUDIO_OUT_AS_ITF_NBR,          /* bInterfaceNumber */
        0x00,                          /* bAlternateSetting */
        0x00,                          /* bNumEndpoints */
        USB_DEVICE_CLASS_AUDIO,        /* bInterfaceClass */
        AUDIO_SUBCLASS_AUDIOSTREAMING, /* bInterfaceSubClass */
        AUDIO_PROTOCOL_UNDEFINED,      /* bInterfaceProtocol */
        0x00,                          /* iInterface */
        /* 09 byte*/

        /* USB Speaker Standard AS Interface Descriptor - Audio Streaming Operational */
        /* Interface 1, Alternate Setting 1                                           */
        AUDIO_INTERFACE_DESC_SIZE,     /* bLength */
        USB_DESC_TYPE_INTERFACE,       /* bDescriptorType */
        AUDIO_OUT_AS_ITF_NBR,          /* bInterfaceNumber */
        0x01,                          /* bAlternateSetting */
        0x01,                          /* bNumEndpoints */
        USB_DEVICE_CLASS_AUDIO,        /* bInterfaceClass */
        AUDIO_SUBCLASS_AUDIOSTREAMING, /* bInterfaceSubClass */
        AUDIO_PROTOCOL_UNDEFINED,      /* bInterfaceProtocol */
        0x00,                          /* iInterface */
        /* 09 byte*/

        /* USB Speaker Audio Streaming Interface Descriptor */
        AUDIO_STREAMING_INTERFACE_DESC_SIZE, /* bLength */
        AUDIO_INTERFACE_DESCRIPTOR_TYPE,     /* bDescriptorType */
        AUDIO_STREAMING_GENERAL,             /* bDescriptorSubtype */
        0x01,                                /* bTerminalLink */
        0x01,                                /* bDelay */
        0x01,                                /* wFormatTag AUDIO_FORMAT_PCM  0x0001 */
        0x00,
        /* 07 byte*/

        /* USB Speaker Audio Type III Format Interface Descriptor */
        0x0B,                               /* bLength */
        AUDIO_INTERFACE_DESCRIPTOR_TYPE,    /* bDescriptorType */
        AUDIO_STREAMING_FORMAT_TYPE,        /* bDescriptorSubtype */
        AUDIO_FORMAT_TYPE_I,                /* bFormatType */
        0x02,                               /* bNrChannels */
        0x02,                               /* bSubFrameSize :  2 Bytes per frame (16bits) */
        16,                                 /* bBitResolution (16-bits per sample) */
        0x01,                               /* bSamFreqType only one frequency supported */
        AUDIO_SAMPLE_FREQ(USBD_AUDIO_FREQ), /* Audio sampling frequency coded on 3 bytes */
        /* 11 byte*/

        /* Endpoint 1 - Standard Descriptor */
        AUDIO_STANDARD_ENDPOINT_DESC_SIZE, /* bLength */
        USB_DESC_TYPE_ENDPOINT,            /* bDescriptorType */
        AUDIO_OUT_EP,                      /* bEndpointAddress 1 out endpoint */
        USBD_EP_TYPE_ISOC,                 /* bmAttributes */
        AUDIO_PACKET_SZE(USBD_AUDIO_FREQ), /* wMaxPacketSize in Bytes (Freq(Samples)*2(Stereo)*2(HalfWord)) */
        AUDIO_FS_BINTERVAL,                /* bInterval */
        0x00,                              /* bRefresh */
        0x00,                              /* bSynchAddress */
        /* 09 byte*/

        /* Endpoint - Audio Streaming Descriptor*/
        AUDIO_STREAMING_ENDPOINT_DESC_SIZE, /* bLength */
        AUDIO_ENDPOINT_DESCRIPTOR_TYPE,     /* bDescriptorType */
        AUDIO_ENDPOINT_GENERAL,             /* bDescriptor */
        0x00,                               /* bmAttributes */
        0x00,                               /* bLockDelayUnits */
        0x00,                               /* wLockDelay */
        0x00,
/* 07 byte*/
#endif
};

///////////////////////////////////////// FS /////////////////////////////////////////////////////
///////////////////////////////////////// FS /////////////////////////////////////////////////////

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
  USBD_VIDEO.Init(pdev, cfgidx);
  USBD_CDC_RNDIS.Init(pdev, cfgidx);
  USBD_MSC.Init(pdev, cfgidx);
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
  USBD_VIDEO.DeInit(pdev, cfgidx);
  USBD_CDC_RNDIS.DeInit(pdev, cfgidx);
  USBD_MSC.DeInit(pdev, cfgidx);
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
  /** if interface is msc */
  if (req->wIndex == COMPOSITE_CDC_RNDIS_CMD_ITF_NBR || req->wIndex == COMPOSITE_CDC_RNDIS_COM_ITF_NBR)
  {
    return USBD_CDC_RNDIS.Setup(pdev, req);
  }
  else if (req->wIndex == COMPOSITE_UVC_VC_IF_NUM || req->wIndex == COMPOSITE_UVC_VS_IF_NUM)
  {
    return USBD_VIDEO.Setup(pdev, req);
  }

  return USBD_MSC.Setup(pdev, req);
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
  if (epnum == (COMPOSITE_CDC_RNDIS_IN_EP & 0x7F) || epnum == (COMPOSITE_CDC_RNDIS_CMD_EP & 0x7F))
  {
    return USBD_CDC_RNDIS.DataIn(pdev, epnum);
  }
  else if (epnum == (COMPOSITE_UVC_IN_EP & 0x7F))
  {
    return USBD_VIDEO.DataIn(pdev, epnum);
  }

  return USBD_MSC.DataIn(pdev, epnum);
}

/**
  * @brief  USBD_COMPOSITE_EP0_RxReady
  *         handle EP0 Rx Ready event
  * @param  pdev: device instance
  * @retval status
  */
static uint8_t USBD_COMPOSITE_EP0_RxReady(USBD_HandleTypeDef *pdev)
{
  return USBD_CDC_RNDIS.EP0_RxReady(pdev);
}

/**
  * @brief  USBD_COMPOSITE_EP0_TxReady
  *         handle EP0 TRx Ready event
  * @param  pdev: device instance
  * @retval status
  */
static uint8_t USBD_COMPOSITE_EP0_TxReady(USBD_HandleTypeDef *pdev)
{
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
  return USBD_VIDEO.SOF(pdev);
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
  return USBD_VIDEO.IsoINIncomplete(pdev, epnum);
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
  if (epnum == COMPOSITE_CDC_RNDIS_OUT_EP)
  {
    return USBD_CDC_RNDIS.DataOut(pdev, epnum);
  }
  return USBD_MSC.DataOut(pdev, epnum);
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
  return USBD_COMPOSITE_HSCfgDesc;
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
  return USBD_COMPOSITE_FSCfgDesc;
}

/**
  * @brief  USBD_COMPOSITE_GetOtherSpeedCfgDesc
  *         return configuration descriptor
  * @param  length : pointer data length
  * @retval pointer to descriptor buffer
  */
static uint8_t *USBD_COMPOSITE_GetOtherSpeedCfgDesc(uint16_t *length)
{
  if (pdev->dev_speed == USBD_SPEED_HIGH)
  {
    *length = (uint16_t)sizeof(USBD_COMPOSITE_FSCfgDesc);
    return USBD_COMPOSITE_FSCfgDesc;
  }

  *length = (uint16_t)sizeof(USBD_COMPOSITE_HSCfgDesc);
  return USBD_COMPOSITE_HSCfgDesc;
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
  * @}
  */

/**
  * @}
  */

/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
