/**
  ******************************************************************************
  * @file    usbd_video_if_template.c
  * @author  MCD Application Team
  * @brief   Template file for Video Interface application layer functions
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2020 STMicroelectronics.
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
#include "usbd_video_if.h"

/* Include you image binary file here
    Binary image template shall provide:
     - tImagesList: table containing pointers to all images
     - tImagesSizes: table containing sizes of each image respectively
     - img_count: global image counter variable
     - IMG_NBR: Total image number

     To generate such file, it is possible to use tools converting video to MJPEG then to JPEG images.
 */
/* #include "img_bin.h" */

/* USER CODE BEGIN INCLUDE */

/* USER CODE END INCLUDE */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/

/* USER CODE BEGIN PV */
/* Private variables ---------------------------------------------------------*/

/* USER CODE END PV */

/** @addtogroup STM32_USB_OTG_DEVICE_LIBRARY
  * @brief Usb device library.
  * @{
  */

/** @addtogroup USBD_VIDEO_IF
  * @{
  */

/** @defgroup USBD_VIDEO_IF_Private_TypesDefinitions
  * @brief Private types.
  * @{
  */

/* USER CODE BEGIN PRIVATE_TYPES */

/* USER CODE END PRIVATE_TYPES */

/**
  * @}
  */

/** @defgroup USBD_VIDEO_IF_Private_Defines
  * @brief Private defines.
  * @{
  */

/* USER CODE BEGIN PRIVATE_DEFINES */

/* USER CODE END PRIVATE_DEFINES */

/**
  * @}
  */

/** @defgroup USBD_VIDEO_IF_Private_Macros
  * @brief Private macros.
  * @{
  */

/* USER CODE BEGIN PRIVATE_MACRO */

/* USER CODE END PRIVATE_MACRO */

/**
  * @}
  */

/** @defgroup USBD_VIDEO_IF_Private_Variables
  * @brief Private variables.
  * @{
  */

/* USER CODE BEGIN PRIVATE_VARIABLES */

/* USER CODE END PRIVATE_VARIABLES */

/**
  * @}
  */

/** @defgroup USBD_VIDEO_IF_Exported_Variables
  * @brief Public variables.
  * @{
  */



/* USER CODE BEGIN EXPORTED_VARIABLES */

/* USER CODE END EXPORTED_VARIABLES */

/**
  * @}
  */

/** @defgroup USBD_VIDEO_IF_Private_FunctionPrototypes
  * @brief Private functions declaration.
  * @{
  */

static int8_t VIDEO_Itf_Init(void);
static int8_t VIDEO_Itf_DeInit(void);
static int8_t VIDEO_Itf_Control(uint8_t cmd, uint8_t *pbuf, uint16_t length);
static int8_t VIDEO_Itf_Data(uint8_t **pbuf, uint16_t *psize, uint16_t *pcktidx);


/* USER CODE BEGIN PRIVATE_FUNCTIONS_DECLARATION */

/* USER CODE END PRIVATE_FUNCTIONS_DECLARATION */

/**
  * @}
  */

USBD_VIDEO_ItfTypeDef USBD_VIDEO_fops_FS =
{
  VIDEO_Itf_Init,
  VIDEO_Itf_DeInit,
  VIDEO_Itf_Control,
  VIDEO_Itf_Data,
};

/* Private functions ---------------------------------------------------------*/
/**
  * @brief  Initializes the VIDEO media low layer over USB FS IP
  * @param  VIDEOFreq: VIDEO frequency used to play the VIDEO stream.
  * @param  Volume: Initial volume level (from 0 (Mute) to 100 (Max))
  * @param  options: Reserved for future use
  * @retval USBD_OK if all operations are OK else USBD_FAIL
  */
static int8_t VIDEO_Itf_Init(void)
{
  /*
     Add your initialization code here
  */

  return (0);
}

/**
  * @brief  TEMPLATE_DeInit
  *         DeInitializes the UVC media low layer
  * @param  None
  * @retval Result of the operation: USBD_OK if all operations are OK else USBD_FAIL
  */
static int8_t VIDEO_Itf_DeInit(void)
{
  /*
     Add your deinitialization code here
  */
  return (0);
}


/**
  * @brief  TEMPLATE_Control
  *         Manage the UVC class requests
  * @param  Cmd: Command code
  * @param  Buf: Buffer containing command data (request parameters)
  * @param  Len: Number of data to be sent (in bytes)
  * @retval Result of the operation: USBD_OK if all operations are OK else USBD_FAIL
  */
static int8_t VIDEO_Itf_Control(uint8_t cmd, uint8_t *pbuf, uint16_t length)
{

  return (0);
}

/**
  * @brief  TEMPLATE_Data
  *         Manage the UVC data packets
  * @param  pbuf: pointer to the buffer data to be filled
  * @param  psize: pointer tot he current packet size to be filled
  * @param  pcktidx: pointer to the current packet index in the current image
  * @retval Result of the operation: USBD_OK if all operations are OK else USBD_FAIL
  */
static int8_t VIDEO_Itf_Data(uint8_t **pbuf, uint16_t *psize, uint16_t *pcktidx)
{

  return (0);
}

/* USER CODE BEGIN PRIVATE_FUNCTIONS_IMPLEMENTATION */

/* USER CODE END PRIVATE_FUNCTIONS_IMPLEMENTATION */

/**
  * @}
  */

/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
