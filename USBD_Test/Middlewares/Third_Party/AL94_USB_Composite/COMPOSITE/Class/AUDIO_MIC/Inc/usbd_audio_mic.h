/**
******************************************************************************
* @file    usbd_audio_in.h
* @author  Central Labs
* @version V2.4.2 MOD
* @date    01-Sep-2016
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
#ifndef __USBD_AUDIO_MIC_H_
#define __USBD_AUDIO_MIC_H_

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

#define AUDIO_MIC_STR_DESC                                "STM32 MICROPHONE"

/* AUDIO Class Config */
#define AUDIO_MIC_CHANNELS                                0x01
#define AUDIO_MIC_SMPL_FREQ                               16000U
#define USBD_AUDIO_MIC_CONFIG_DESC_SIZE                   (116 + AUDIO_MIC_CHANNELS)



#define AUDIO_MIC_VOL_MIN                                 0xDBE0
#define AUDIO_MIC_VOL_RES                                 0x0023
#define AUDIO_MIC_VOL_MAX                                 0x0000
#define AUDIO_MIC_PACKET                                  (uint32_t)((((AUDIO_MIC_SMPL_FREQ/1000)+2)*8)*2)
#define AUDIO_MIC_TERMINAL_ID                             1
#define AUDIO_MIC_FU_ID                                   2
#define AUDIO_MIC_OUT_TERMINAL_ID                         3

/* Buffering state definitions */
typedef enum
{
  STATE_USB_WAITING_FOR_INIT = 0,
  STATE_USB_IDLE = 1,
  STATE_USB_REQUESTS_STARTED = 2,
  STATE_USB_BUFFER_WRITE_STARTED = 3,
} AUDIO_MIC_StatesTypeDef;

/* Number of sub-packets in the audio transfer buffer.*/
#define AUDIO_MIC_PACKET_NUM 20

#define TIMEOUT_VALUE 200


typedef struct
{
  __IO uint32_t alt_setting;
  uint8_t channels;
  uint32_t frequency;
  __IO int16_t timeout;
  uint16_t buffer_length;
  uint16_t dataAmount;
  uint16_t paketDimension;
  uint8_t state;
  uint16_t rd_ptr;
  uint16_t wr_ptr;
  uint8_t upper_treshold;
  uint8_t lower_treshold;
  USBD_AUDIO_ControlTypeDef control;
  uint8_t *buffer;
} USBD_AUDIO_MIC_HandleTypeDef;

typedef struct
{
  int8_t (*Init)(uint32_t AudioFreq, uint32_t BitRes, uint32_t ChnlNbr);
  int8_t (*DeInit)(uint32_t options);
  int8_t (*Record)(void);
  int8_t (*VolumeCtl)(int16_t Volume);
  int8_t (*MuteCtl)(uint8_t cmd);
  int8_t (*Stop)(void);
  int8_t (*Pause)(void);
  int8_t (*Resume)(void);
  int8_t (*CommandMgr)(uint8_t cmd);
} USBD_AUDIO_MIC_ItfTypeDef;
/**
* @}
*/

/** @defgroup USBD_AUDIO_IN_Exported_Macros
* @{
*/

/**
* @}
*/

/** @defgroup USBD_AUDIO_IN_Exported_Variables
* @{
*/

extern USBD_ClassTypeDef USBD_AUDIO_MIC;

extern uint8_t AUDIO_MIC_EP;
extern uint8_t AUDIO_MIC_AC_ITF_NBR;
extern uint8_t AUDIO_MIC_AS_ITF_NBR;
extern uint8_t AUDIO_MIC_STR_DESC_IDX;

/**
* @}
*/

/** @defgroup USBD_AUDIO_IN_Exported_Functions
* @{
*/
uint8_t USBD_AUDIO_MIC_RegisterInterface(USBD_HandleTypeDef *pdev, 
                                         USBD_AUDIO_MIC_ItfTypeDef *fops);
uint8_t USBD_AUDIO_MIC_Data_Transfer(USBD_HandleTypeDef *pdev, int16_t *audioData, uint16_t dataAmount);

  void USBD_Update_Audio_MIC_DESC(uint8_t *desc,
                                  uint8_t ac_itf,
                                  uint8_t as_itf,
                                  uint8_t in_ep,
                                  uint8_t str_idx);
  /**
  * @}
  */

#ifdef __cplusplus
}
#endif


/**
  * @}
  */

/**
  * @}
  */
#endif // __USBD_AUDIO_MIC_H_
/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
