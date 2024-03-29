/**
  ******************************************************************************
  * @file    usbd_printer.h
  * @author  MCD Application Team
  * @brief   header file for the usbd_printer.c file.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2021 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under Ultimate Liberty license
  * SLA0044, the "License"; You may not use this file except in compliance with
  * the License. You may obtain a copy of the License at:
  *                     www.st.com/SLA0044
  *
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __USB_PRNT_H
#define __USB_PRNT_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include  "usbd_ioreq.h"

/** @addtogroup STM32_USB_DEVICE_LIBRARY
  * @{
  */

/** @defgroup usbd_PRNT
  * @brief This file is the Header file for usbd_PRNT.c
  * @{
  */


/** @defgroup usbd_PRNT_Exported_Defines
  * @{
  */
#define PRNT_STR_DESC           					 "STM32 USB PRINTER"

#ifndef PRNT_DATA_HS_MAX_PACKET_SIZE
#define PRNT_DATA_HS_MAX_PACKET_SIZE                 512U  /* Endpoint IN & OUT Packet size */
#endif /* PRNT_DATA_HS_MAX_PACKET_SIZE */

#ifndef PRNT_DATA_FS_MAX_PACKET_SIZE
#define PRNT_DATA_FS_MAX_PACKET_SIZE                 64U   /* Endpoint IN & OUT Packet size */
#endif /* PRNT_DATA_FS_MAX_PACKET_SIZE */

#define USB_PRNT_CONFIG_DESC_SIZE                    32U
#define PRNT_DATA_HS_IN_PACKET_SIZE                  PRNT_DATA_HS_MAX_PACKET_SIZE
#define PRNT_DATA_HS_OUT_PACKET_SIZE                 PRNT_DATA_HS_MAX_PACKET_SIZE

#define PRNT_DATA_FS_IN_PACKET_SIZE                  PRNT_DATA_FS_MAX_PACKET_SIZE
#define PRNT_DATA_FS_OUT_PACKET_SIZE                 PRNT_DATA_FS_MAX_PACKET_SIZE

/*---------------------------------------------------------------------*/
/*  PRNT definitions                                                    */
/*---------------------------------------------------------------------*/
#define PRNT_STATUS_PAPER_EMPTY                      0x10U
#define PRNT_STATUS_SELECTED                         0x08U
#define PRNT_STATUS_NO_ERROR                         0x00U

#define USB_PRNT_SUBCLASS                            0x01U

#define USB_PRNT_UNIDIRECTIONAL                      0x01U
#define USB_PRNT_BIDIRECTIONAL                       0x02U

/* USB PRNT Request types */
#define PRNT_GET_DEVICE_ID                           0x00U
#define PRNT_GET_PORT_STATUS                         0x01U
#define PRNT_SOFT_RESET                              0x02U
/**
  * @}
  */

/** @defgroup USBD_CORE_Exported_TypesDefinitions
  * @{
  */

/**
  * @}
  */

typedef struct _USBD_PRNT_Itf
{
  int8_t (*Init)(void);
  int8_t (*DeInit)(void);
  int8_t (*Control_req)(uint8_t req, uint8_t *pbuf, uint16_t *length);
  int8_t (*Receive)(uint8_t *Buf, uint32_t *Len);

} USBD_PRNT_ItfTypeDef;

typedef struct
{
  uint32_t data[PRNT_DATA_HS_MAX_PACKET_SIZE / 4U]; /* Force 32-bit alignment */
  uint8_t CmdOpCode;
  uint8_t CmdLength;
  uint8_t *RxBuffer;
  uint8_t *TxBuffer;
  uint32_t RxLength;
  uint32_t TxLength;

  __IO uint32_t TxState;
  __IO uint32_t RxState;
} USBD_PRNT_HandleTypeDef;

/** @defgroup USBD_CORE_Exported_Macros
  * @{
  */

/**
  * @}
  */

/** @defgroup USBD_CORE_Exported_Variables
  * @{
  */

extern USBD_ClassTypeDef USBD_PRNT;

extern uint8_t PRNT_IN_EP;
extern uint8_t PRNT_OUT_EP;
extern uint8_t PRNT_ITF_NBR;
extern uint8_t PRINTER_STR_DESC_IDX;

/**
  * @}
  */

/** @defgroup USB_CORE_Exported_Functions
  * @{
  */
uint8_t USBD_PRNT_RegisterInterface(USBD_HandleTypeDef *pdev, USBD_PRNT_ItfTypeDef *fops);
uint8_t USBD_PRNT_SetRxBuffer(USBD_HandleTypeDef *pdev, uint8_t *pbuff);
uint8_t USBD_PRNT_ReceivePacket(USBD_HandleTypeDef *pdev);

void USBD_Update_PRNT_DESC(uint8_t *desc, uint8_t itf_no, uint8_t in_ep, uint8_t out_ep, uint8_t str_idx);

/**
  * @}
  */

#ifdef __cplusplus
}
#endif

#endif /* __USB_PRNT_H */
/**
  * @}
  */

/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
