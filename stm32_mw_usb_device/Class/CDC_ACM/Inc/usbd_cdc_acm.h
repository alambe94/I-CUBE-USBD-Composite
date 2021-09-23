/**
  ******************************************************************************
  * @file    usbd_cdc.h
  * @author  MCD Application Team
  * @brief   header file for the usbd_cdc.c file.
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
#ifndef __USB_CDC_H
#define __USB_CDC_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include  "usbd_ioreq.h"
#include "AL94.I-CUBE-USBD-COMPOSITE_conf.h"

/** @addtogroup STM32_USB_DEVICE_LIBRARY
  * @{
  */

/** @defgroup usbd_cdc
  * @brief This file is the Header file for usbd_cdc.c
  * @{
  */


/** @defgroup usbd_cdc_Exported_Defines
  * @{
  */

#define CDC_ACM_STR_DESC                            "STM32 CDC ACM%d"

#define NUMBER_OF_CDC                               _USBD_CDC_ACM_COUNT
#ifndef CDC_HS_BINTERVAL
#define CDC_HS_BINTERVAL                            0x10U
#endif /* CDC_HS_BINTERVAL */

#ifndef CDC_FS_BINTERVAL
#define CDC_FS_BINTERVAL                            0x10U
#endif /* CDC_FS_BINTERVAL */

/* CDC Endpoints parameters: you can fine tune these values depending on the needed baudrates and performance. */
#define CDC_DATA_HS_MAX_PACKET_SIZE                 512U  /* Endpoint IN & OUT Packet size */
#define CDC_DATA_FS_MAX_PACKET_SIZE                 64U  /* Endpoint IN & OUT Packet size */
#define CDC_CMD_PACKET_SIZE                         8U  /* Control Endpoint Packet size */

#define USB_CDC_CONFIG_DESC_SIZ                     (9 + 66 * NUMBER_OF_CDC)
#define CDC_DATA_HS_IN_PACKET_SIZE                  CDC_DATA_HS_MAX_PACKET_SIZE
#define CDC_DATA_HS_OUT_PACKET_SIZE                 CDC_DATA_HS_MAX_PACKET_SIZE

#define CDC_DATA_FS_IN_PACKET_SIZE                  CDC_DATA_FS_MAX_PACKET_SIZE
#define CDC_DATA_FS_OUT_PACKET_SIZE                 CDC_DATA_FS_MAX_PACKET_SIZE

#define CDC_REQ_MAX_DATA_SIZE                       0x7U
/*---------------------------------------------------------------------*/
/*  CDC definitions                                                    */
/*---------------------------------------------------------------------*/
#define CDC_SEND_ENCAPSULATED_COMMAND               0x00U
#define CDC_GET_ENCAPSULATED_RESPONSE               0x01U
#define CDC_SET_COMM_FEATURE                        0x02U
#define CDC_GET_COMM_FEATURE                        0x03U
#define CDC_CLEAR_COMM_FEATURE                      0x04U
#define CDC_SET_LINE_CODING                         0x20U
#define CDC_GET_LINE_CODING                         0x21U
#define CDC_SET_CONTROL_LINE_STATE                  0x22U
#define CDC_SEND_BREAK                              0x23U

  /**
  * @}
  */

  /** @defgroup USBD_CORE_Exported_TypesDefinitions
  * @{
  */

  /**
  * @}
  */
  typedef struct
  {
    uint32_t bitrate;
    uint8_t format;
    uint8_t paritytype;
    uint8_t datatype;
  } USBD_CDC_ACM_LineCodingTypeDef;

  typedef struct _USBD_CDC_Itf
  {
    int8_t (*Init)(uint8_t cdc_ch);
    int8_t (*DeInit)(uint8_t cdc_ch);
    int8_t (*Control)(uint8_t cdc_ch, uint8_t cmd, uint8_t *pbuf, uint16_t length);
    int8_t (*Receive)(uint8_t cdc_ch, uint8_t *Buf, uint32_t *Len);
    int8_t (*TransmitCplt)(uint8_t cdc_ch, uint8_t *Buf, uint32_t *Len, uint8_t epnum);
  } USBD_CDC_ACM_ItfTypeDef;

  typedef struct
  {
    uint32_t data[NUMBER_OF_CDC][CDC_DATA_HS_MAX_PACKET_SIZE / 4U]; /* Force 32bits alignment */
    uint8_t CmdOpCode;
    uint8_t CmdLength;
    uint8_t *RxBuffer;
    uint8_t *TxBuffer;
    uint32_t RxLength;
    uint32_t TxLength;

    __IO uint32_t TxState;
    __IO uint32_t RxState;
  } USBD_CDC_ACM_HandleTypeDef;

  /** @defgroup USBD_CORE_Exported_Macros
  * @{
  */

  /**
  * @}
  */

  /** @defgroup USBD_CORE_Exported_Variables
  * @{
  */

  extern USBD_ClassTypeDef USBD_CDC_ACM;

  extern uint8_t CDC_IN_EP[NUMBER_OF_CDC];  /* EP1 for data IN */
  extern uint8_t CDC_OUT_EP[NUMBER_OF_CDC]; /* EP1 for data OUT */
  extern uint8_t CDC_CMD_EP[NUMBER_OF_CDC]; /* EP2 for CDC commands */

  extern uint8_t CDC_CMD_ITF_NBR[NUMBER_OF_CDC]; /* Command Interface Number */
  extern uint8_t CDC_COM_ITF_NBR[NUMBER_OF_CDC]; /* Communication Interface Number */

  extern uint8_t CDC_STR_DESC_IDX[NUMBER_OF_CDC];

  /**
  * @}
  */

  /** @defgroup USB_CORE_Exported_Functions
  * @{
  */
  uint8_t USBD_CDC_ACM_RegisterInterface(USBD_HandleTypeDef *pdev,
                                         USBD_CDC_ACM_ItfTypeDef *fops);

  uint8_t USBD_CDC_SetTxBuffer(uint8_t ch, USBD_HandleTypeDef *pdev, uint8_t *pbuff,
                                   uint32_t length);

  uint8_t USBD_CDC_SetRxBuffer(uint8_t ch, USBD_HandleTypeDef *pdev, uint8_t *pbuff);
  uint8_t USBD_CDC_ReceivePacket(uint8_t ch, USBD_HandleTypeDef *pdev);
  uint8_t USBD_CDC_TransmitPacket(uint8_t ch, USBD_HandleTypeDef *pdev);

  void USBD_Update_CDC_ACM_DESC(uint8_t *desc,
                                uint8_t cmd_itf,
                                uint8_t com_itf,
                                uint8_t in_ep,
                                uint8_t cmd_ep,
                                uint8_t out_ep,
                                uint8_t str_idx);
  /**
  * @}
  */

#ifdef __cplusplus
}
#endif

#endif /* __USB_CDC_H */
/**
  * @}
  */

/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
