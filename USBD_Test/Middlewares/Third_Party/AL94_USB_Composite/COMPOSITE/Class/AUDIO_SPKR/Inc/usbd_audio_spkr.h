/**
  ******************************************************************************
  * @file    usbd_audio.h
  * @author  MCD Application Team
  * @brief   header file for the usbd_audio.c file.
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
#ifndef __USB_AUDIO_SPKR_H
#define __USB_AUDIO_SPKR_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include  "usbd_ioreq.h"
#include  "usbd_audio.h"

/** @addtogroup STM32_USB_DEVICE_LIBRARY
  * @{
  */

/** @defgroup USBD_AUDIO
  * @brief This file is the Header file for usbd_audio.c
  * @{
  */


/** @defgroup USBD_AUDIO_Exported_Defines
  * @{
  */

#define AUDIO_SPKR_STR_DESC                          "STM32 SPEAKER"

#define USBD_AUDIO_FREQ                               16000U

#define USBD_AUDIO_SPKR_CONFIG_DESC_SIZE              117

#define AUDIO_OUT_TC                                  0x01U
#define AUDIO_IN_TC                                   0x02U


#define AUDIO_OUT_PACKET                              (uint16_t)(((USBD_AUDIO_FREQ * 2U * 2U) / 1000U))
#define AUDIO_DEFAULT_VOLUME                          70U

/* Number of sub-packets in the audio transfer buffer. You can modify this value but always make sure
  that it is an even number and higher than 3 */
#define AUDIO_OUT_PACKET_NUM                          80U
/* Total size of the audio transfer buffer */
#define AUDIO_TOTAL_BUF_SIZE                          ((uint16_t)(AUDIO_OUT_PACKET * AUDIO_OUT_PACKET_NUM))

  typedef struct
  {
    uint32_t alt_setting;
    uint8_t buffer[AUDIO_TOTAL_BUF_SIZE];
    AUDIO_OffsetTypeDef offset;
    uint8_t rd_enable;
    uint16_t rd_ptr;
    uint16_t wr_ptr;
    USBD_AUDIO_ControlTypeDef control;
  } USBD_AUDIO_SPKR_HandleTypeDef;

  typedef struct
  {
    int8_t (*Init)(uint32_t AudioFreq, uint32_t Volume, uint32_t options);
    int8_t (*DeInit)(uint32_t options);
    int8_t (*AudioCmd)(uint8_t *pbuf, uint32_t size, uint8_t cmd);
    int8_t (*VolumeCtl)(uint8_t vol);
    int8_t (*MuteCtl)(uint8_t cmd);
    int8_t (*PeriodicTC)(uint8_t *pbuf, uint32_t size, uint8_t cmd);
    int8_t (*GetState)(void);
  } USBD_AUDIO_SPKR_ItfTypeDef;
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

  extern USBD_ClassTypeDef USBD_AUDIO_SPKR;

  extern uint8_t AUDIO_SPKR_EP;
  extern uint8_t AUDIO_SPKR_AC_ITF_NBR;
  extern uint8_t AUDIO_SPKR_AS_ITF_NBR;
  extern uint8_t AUDIO_SPKR_STR_DESC_IDX;

  /**
  * @}
  */

  /** @defgroup USB_CORE_Exported_Functions
  * @{
  */
  uint8_t USBD_AUDIO_SPKR_RegisterInterface(USBD_HandleTypeDef *pdev,
                                            USBD_AUDIO_SPKR_ItfTypeDef *fops);

  void USBD_AUDIO_SPKR_Sync(USBD_HandleTypeDef *pdev, AUDIO_OffsetTypeDef offset);

  void USBD_Update_Audio_SPKR_DESC(uint8_t *desc,
                                   uint8_t ac_itf,
                                   uint8_t as_itf,
                                   uint8_t out_ep,
                                   uint8_t str_idx);
  /**
  * @}
  */

#ifdef __cplusplus
}
#endif

#endif /* __USB_AUDIO_SPKR_H */
/**
  * @}
  */

/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
