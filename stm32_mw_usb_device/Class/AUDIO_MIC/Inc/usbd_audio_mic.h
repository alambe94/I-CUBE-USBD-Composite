/**
******************************************************************************
* @file    usbd_audio_in.h
* @author  Central Labs
* @version V2.4.2 MOD
* @date    01-Sep-2016
* @brief   header file for the usbd_audio_in.c file.
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

#ifndef __USBD_AUDIO_IN_H_
#define __USBD_AUDIO_IN_H_

#include "usbd_ioreq.h"

/** @addtogroup STM32_USB_OTG_DEVICE_LIBRARY
* @{
*/

/** @defgroup USBD_AUDIO_IN
* @{
*/

/** @defgroup USBD_AUDIO_IN_Exported_Defines
* @{
*/

/* Audio Data in endpoint */
#define AUDIO_MIC_CHANNELS                                0x01
#define AUDIO_MIC_SMPL_FREQ                               16000

#define AUDIO_MIC_CONFIG_DESC_SIZE                        (109 + AUDIO_MIC_CHANNELS - 1)

#define AUDIO_MIC_INTERFACE_DESC_SIZE                     0x09
#define AUDIO_MIC_DESC_SIZE                               0x09
#define AUDIO_MIC_STANDARD_ENDPOINT_DESC_SIZE             0x09
#define AUDIO_MIC_STREAMING_ENDPOINT_DESC_SIZE            0x07
#define AUDIO_MIC_DESCRIPTOR_TYPE                         0x21
#define AUDIO_MIC_DEVICE_CLASS                            0x01
#define AUDIO_MIC_SUBCLASS_AUDIOCONTROL                   0x01
#define AUDIO_MIC_SUBCLASS_AUDIOSTREAMING                 0x02
#define AUDIO_MIC_PROTOCOL_UNDEFINED                      0x00
#define AUDIO_MIC_STREAMING_GENERAL                       0x01
#define AUDIO_MIC_STREAMING_FORMAT_TYPE                   0x02
/* Audio Descriptor Types */
#define AUDIO_MIC_INTERFACE_DESCRIPTOR_TYPE               0x24
#define AUDIO_MIC_ENDPOINT_DESCRIPTOR_TYPE                0x25
/* Audio Control Interface Descriptor Subtypes */
#define AUDIO_MIC_CONTROL_HEADER                          0x01
#define AUDIO_MIC_CONTROL_INPUT_TERMINAL                  0x02
#define AUDIO_MIC_CONTROL_OUTPUT_TERMINAL                 0x03
#define AUDIO_MIC_CONTROL_FEATURE_UNIT                    0x06
#define AUDIO_MIC_INPUT_TERMINAL_DESC_SIZE                0x0C
#define AUDIO_MIC_OUTPUT_TERMINAL_DESC_SIZE               0x09
#define AUDIO_MIC_STREAMING_INTERFACE_DESC_SIZE           0x07
#define AUDIO_MIC_CONTROL_MUTE                            0x0002
#define AUDIO_MIC_FORMAT_TYPE_I                           0x01
#define AUDIO_MIC_FORMAT_TYPE_III                         0x03
#define AUDIO_MIC_ENDPOINT_GENERAL                        0x01
#define AUDIO_MIC_REQ_GET_CUR                             0x81
#define AUDIO_MIC_REQ_GET_MIN                             0x82
#define AUDIO_MIC_REQ_GET_MAX                             0x83
#define AUDIO_MIC_REQ_GET_RES                             0x84
#define AUDIO_MIC_REQ_SET_CUR                             0x01
#define AUDIO_MIC_OUT_STREAMING_CTRL                      0x02

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

/* Audio Commands enmueration */
typedef enum
{
  AUDIO_CMD_START = 1,
  AUDIO_CMD_PLAY,
  AUDIO_CMD_STOP,
} AUDIO_MIC_CMD_TypeDef;

typedef enum
{
  AUDIO_OFFSET_NONE = 0,
  AUDIO_OFFSET_HALF,
  AUDIO_OFFSET_FULL,
  AUDIO_OFFSET_UNKNOWN,
} AUDIO_MIC_OffsetTypeDef;

/**
* @}
*/

/** @defgroup USBD_AUDIO_IN_Exported_TypesDefinitions
* @{
*/
typedef struct
{
  uint8_t cmd;
  uint8_t data[USB_MAX_EP0_SIZE];
  uint8_t len;
  uint8_t unit;
} USBD_AUDIO_ControlTypeDef;

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

/**
* @}
*/

/** @defgroup USBD_AUDIO_IN_Exported_Functions
* @{
*/
uint8_t USBD_AUDIO_MIC_RegisterInterface(USBD_HandleTypeDef *pdev, USBD_AUDIO_MIC_ItfTypeDef *fops);
void USBD_AUDIO_MICInit_Microphone_Descriptor(USBD_HandleTypeDef *pdev, uint32_t samplingFrequency, uint8_t Channels);
uint8_t USBD_AUDIO_MICData_Transfer(USBD_HandleTypeDef *pdev, int16_t *audioData, uint16_t dataAmount);

void USBD_Update_Audio_MIC_DESC(uint8_t *desc, uint8_t ac_itf, uint8_t as_itf, uint8_t in_ep);

/**
* @}
*/

/**
* @}
*/

/**
* @}
*/
#endif // __USBD_AUDIO_IN_H_
/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
