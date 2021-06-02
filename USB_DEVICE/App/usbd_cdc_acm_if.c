/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : usbd_cdc_if.c
  * @version        : v2.0_Cube
  * @brief          : Usb device for Virtual Com Port.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2020 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under Ultimate Liberty license
  * SLA0044, the "License"; You may not use this file except in compliance with
  * the License. You may obtain a copy of the License at:
  *                             www.st.com/SLA0044
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "usbd_cdc_acm_if.h"

/* USER CODE BEGIN INCLUDE */
#include "usart.h"
//#include "tim.h"
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

/** @addtogroup USBD_CDC_IF
  * @{
  */

/** @defgroup USBD_CDC_IF_Private_TypesDefinitions USBD_CDC_IF_Private_TypesDefinitions
  * @brief Private types.
  * @{
  */

/* USER CODE BEGIN PRIVATE_TYPES */

/* USER CODE END PRIVATE_TYPES */

/**
  * @}
  */

/** @defgroup USBD_CDC_IF_Private_Defines USBD_CDC_IF_Private_Defines
  * @brief Private defines.
  * @{
  */

/* USER CODE BEGIN PRIVATE_DEFINES */
/* USER CODE END PRIVATE_DEFINES */

/**
  * @}
  */

/** @defgroup USBD_CDC_IF_Private_Macros USBD_CDC_IF_Private_Macros
  * @brief Private macros.
  * @{
  */

/* USER CODE BEGIN PRIVATE_MACRO */

/* USER CODE END PRIVATE_MACRO */

/**
  * @}
  */

/** @defgroup USBD_CDC_IF_Private_Variables USBD_CDC_IF_Private_Variables
  * @brief Private variables.
  * @{
  */

/* USER CODE BEGIN PRIVATE_VARIABLES */

#define APP_RX_DATA_SIZE 128
#define APP_TX_DATA_SIZE 128

/** RX buffer for USB */
uint8_t RX_Buffer[NUMBER_OF_CDC][APP_RX_DATA_SIZE];

/** TX buffer for USB, RX buffer for UART */
uint8_t TX_Buffer[NUMBER_OF_CDC][APP_TX_DATA_SIZE];

USBD_CDC_ACM_LineCodingTypeDef Line_Coding[NUMBER_OF_CDC];

uint32_t Write_Index[NUMBER_OF_CDC]; /* keep track of received data over UART */
uint32_t Read_Index[NUMBER_OF_CDC];  /* keep track of sent data to USB */

/* USER CODE END PRIVATE_VARIABLES */

/**
  * @}
  */

/** @defgroup USBD_CDC_IF_Exported_Variables USBD_CDC_IF_Exported_Variables
  * @brief Public variables.
  * @{
  */

extern USBD_HandleTypeDef hUsbDeviceHS;

/* USER CODE BEGIN EXPORTED_VARIABLES */

/* USER CODE END EXPORTED_VARIABLES */

/**
  * @}
  */

/** @defgroup USBD_CDC_IF_Private_FunctionPrototypes USBD_CDC_IF_Private_FunctionPrototypes
  * @brief Private functions declaration.
  * @{
  */

static int8_t CDC_Init(uint8_t cdc_ch);
static int8_t CDC_DeInit(uint8_t cdc_ch);
static int8_t CDC_Control(uint8_t cdc_ch, uint8_t cmd, uint8_t *pbuf, uint16_t length);
static int8_t CDC_Receive(uint8_t cdc_ch, uint8_t *pbuf, uint32_t *Len);
static int8_t CDC_TransmitCplt(uint8_t cdc_ch, uint8_t *Buf, uint32_t *Len, uint8_t epnum);

/* USER CODE BEGIN PRIVATE_FUNCTIONS_DECLARATION */
UART_HandleTypeDef *CDC_CH_To_UART_Handle(uint8_t cdc_ch)
{
  UART_HandleTypeDef *handle = NULL;

  if (cdc_ch == 0)
  {
    handle = &huart1;
  }
#if (0)
  else if (cdc_ch == 1)
  {
    handle = &huart2;
  }
  else if (cdc_ch == 2)
  {
    handle = &huart3;
  }
#endif
  return handle;
}

uint8_t UART_Handle_TO_CDC_CH(UART_HandleTypeDef *handle)
{
  uint8_t cdc_ch = 0;

  if (handle == &huart1)
  {
    cdc_ch = 0;
  }
#if (0)
  else if (handle == &huart2)
  {
    cdc_ch = 1;
  }
  else if (handle == &huart3)
  {
    cdc_ch = 2;
  }
#endif
  return cdc_ch;
}

void Change_UART_Setting(uint8_t cdc_ch)
{
  UART_HandleTypeDef *handle = CDC_CH_To_UART_Handle(cdc_ch);

  if (HAL_UART_DeInit(handle) != HAL_OK)
  {
    /* Initialization Error */
    Error_Handler();
  }
  /* set the Stop bit */
  switch (Line_Coding[cdc_ch].format)
  {
  case 0:
    handle->Init.StopBits = UART_STOPBITS_1;
    break;
  case 2:
    handle->Init.StopBits = UART_STOPBITS_2;
    break;
  default:
    handle->Init.StopBits = UART_STOPBITS_1;
    break;
  }

  /* set the parity bit*/
  switch (Line_Coding[cdc_ch].paritytype)
  {
  case 0:
    handle->Init.Parity = UART_PARITY_NONE;
    break;
  case 1:
    handle->Init.Parity = UART_PARITY_ODD;
    break;
  case 2:
    handle->Init.Parity = UART_PARITY_EVEN;
    break;
  default:
    handle->Init.Parity = UART_PARITY_NONE;
    break;
  }

  /*set the data type : only 8bits and 9bits is supported */
  switch (Line_Coding[cdc_ch].datatype)
  {
  case 0x07:
    /* With this configuration a parity (Even or Odd) must be set */
    handle->Init.WordLength = UART_WORDLENGTH_8B;
    break;
  case 0x08:
    if (handle->Init.Parity == UART_PARITY_NONE)
    {
      handle->Init.WordLength = UART_WORDLENGTH_8B;
    }
    else
    {
      handle->Init.WordLength = UART_WORDLENGTH_9B;
    }

    break;
  default:
    handle->Init.WordLength = UART_WORDLENGTH_8B;
    break;
  }

  if (Line_Coding[cdc_ch].bitrate == 0)
  {
    Line_Coding[cdc_ch].bitrate = 115200;
  }

  handle->Init.BaudRate = Line_Coding[cdc_ch].bitrate;
  handle->Init.HwFlowCtl = UART_HWCONTROL_NONE;
  handle->Init.Mode = UART_MODE_TX_RX;
  handle->Init.OverSampling = UART_OVERSAMPLING_16;

  if (HAL_UART_Init(handle) != HAL_OK)
  {
    /* Initialization Error */
    Error_Handler();
  }

  /** rx for uart and tx buffer of usb */
  if (HAL_UART_Receive_IT(handle, TX_Buffer[cdc_ch], 1) != HAL_OK)
  {
    /* Transfer error in reception process */
    Error_Handler();
  }
}
/* USER CODE END PRIVATE_FUNCTIONS_DECLARATION */

/**
  * @}
  */

USBD_CDC_ACM_ItfTypeDef USBD_CDC_ACM_fops = {CDC_Init,
                                             CDC_DeInit,
                                             CDC_Control,
                                             CDC_Receive,
											 CDC_TransmitCplt};

/* Private functions ---------------------------------------------------------*/
/**
  * @brief  Initializes the CDC media low layer over the FS USB IP
  * @retval USBD_OK if all operations are OK else USBD_FAIL
  */
static int8_t CDC_Init(uint8_t cdc_ch)
{
  /* USER CODE BEGIN 3 */

  /* ##-1- Set Application Buffers */
  USBD_CDC_SetRxBuffer(cdc_ch, &hUsbDeviceHS, RX_Buffer[cdc_ch]);

  //  /*##-2- Start the TIM Base generation in interrupt mode ####################*/
  //  /* Start Channel1 */
  //  if (HAL_TIM_Base_Start_IT(&htim4) != HAL_OK)
  //  {
  //    /* Starting Error */
  //    Error_Handler();
  //  }

  return (USBD_OK);
  /* USER CODE END 3 */
}

/**
  * @brief  DeInitializes the CDC media low layer
  * @retval USBD_OK if all operations are OK else USBD_FAIL
  */
static int8_t CDC_DeInit(uint8_t cdc_ch)
{
  /* USER CODE BEGIN 4 */
  /* DeInitialize the UART peripheral */
  if (HAL_UART_DeInit(CDC_CH_To_UART_Handle(cdc_ch)) != HAL_OK)
  {
    /* Initialization Error */
    Error_Handler();
  }
  return (USBD_OK);
  /* USER CODE END 4 */
}

/**
  * @brief  Manage the CDC class requests
  * @param  cmd: Command code
  * @param  pbuf: Buffer containing command data (request parameters)
  * @param  length: Number of data to be sent (in bytes)
  * @retval Result of the operation: USBD_OK if all operations are OK else USBD_FAIL
  */
static int8_t CDC_Control(uint8_t cdc_ch, uint8_t cmd, uint8_t *pbuf, uint16_t length)
{
  /* USER CODE BEGIN 5 */
  switch (cmd)
  {
  case CDC_SEND_ENCAPSULATED_COMMAND:

    break;

  case CDC_GET_ENCAPSULATED_RESPONSE:

    break;

  case CDC_SET_COMM_FEATURE:

    break;

  case CDC_GET_COMM_FEATURE:

    break;

  case CDC_CLEAR_COMM_FEATURE:

    break;

    /*******************************************************************************/
    /* Line Coding Structure                                                       */
    /*-----------------------------------------------------------------------------*/
    /* Offset | Field       | Size | Value  | Description                          */
    /* 0      | dwDTERate   |   4  | Number |Data terminal rate, in bits per second*/
    /* 4      | bCharFormat |   1  | Number | Stop bits                            */
    /*                                        0 - 1 Stop bit                       */
    /*                                        1 - 1.5 Stop bits                    */
    /*                                        2 - 2 Stop bits                      */
    /* 5      | bParityType |  1   | Number | Parity                               */
    /*                                        0 - None                             */
    /*                                        1 - Odd                              */
    /*                                        2 - Even                             */
    /*                                        3 - Mark                             */
    /*                                        4 - Space                            */
    /* 6      | bDataBits  |   1   | Number Data bits (5, 6, 7, 8 or 16).          */
    /*******************************************************************************/
  case CDC_SET_LINE_CODING:
    Line_Coding[cdc_ch].bitrate = (uint32_t)(pbuf[0] | (pbuf[1] << 8) |
                                             (pbuf[2] << 16) | (pbuf[3] << 24));
    Line_Coding[cdc_ch].format = pbuf[4];
    Line_Coding[cdc_ch].paritytype = pbuf[5];
    Line_Coding[cdc_ch].datatype = pbuf[6];

    Change_UART_Setting(cdc_ch);
    break;

  case CDC_GET_LINE_CODING:
    pbuf[0] = (uint8_t)(Line_Coding[cdc_ch].bitrate);
    pbuf[1] = (uint8_t)(Line_Coding[cdc_ch].bitrate >> 8);
    pbuf[2] = (uint8_t)(Line_Coding[cdc_ch].bitrate >> 16);
    pbuf[3] = (uint8_t)(Line_Coding[cdc_ch].bitrate >> 24);
    pbuf[4] = Line_Coding[cdc_ch].format;
    pbuf[5] = Line_Coding[cdc_ch].paritytype;
    pbuf[6] = Line_Coding[cdc_ch].datatype;
    break;

  case CDC_SET_CONTROL_LINE_STATE:

    break;

  case CDC_SEND_BREAK:

    break;

  default:
    break;
  }

  return (USBD_OK);
  /* USER CODE END 5 */
}

/**
  * @brief  Data received over USB OUT endpoint are sent over CDC interface
  *         through this function.
  *
  *         @note
  *         This function will issue a NAK packet on any OUT packet received on
  *         USB endpoint until exiting this function. If you exit this function
  *         before transfer is complete on CDC interface (ie. using DMA controller)
  *         it will result in receiving more data while previous ones are still
  *         not sent.
  *
  * @param  Buf: Buffer of data to be received
  * @param  Len: Number of data received (in bytes)
  * @retval Result of the operation: USBD_OK if all operations are OK else USBD_FAIL
  */
static int8_t CDC_Receive(uint8_t cdc_ch, uint8_t *Buf, uint32_t *Len)
{
  /* USER CODE BEGIN 6 */
  HAL_UART_Transmit_DMA(CDC_CH_To_UART_Handle(cdc_ch), Buf, *Len);
  return (USBD_OK);
  /* USER CODE END 6 */
}

static int8_t CDC_TransmitCplt(uint8_t cdc_ch, uint8_t *Buf, uint32_t *Len, uint8_t epnum)
{
	return (USBD_OK);
}

/* USER CODE BEGIN PRIVATE_FUNCTIONS_IMPLEMENTATION */
void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart)
{
  /* Initiate next USB packet transfer once UART completes transfer (transmitting data over Tx line) */
  USBD_CDC_ReceivePacket(UART_Handle_TO_CDC_CH(huart), &hUsbDeviceHS);
}

//void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
//{
//  for (uint8_t i = 0; i < NUMBER_OF_CDC; i++)
//  {
//    uint32_t buffptr;
//    uint32_t buffsize;
//
//    if (Read_Index[i] != Write_Index[i])
//    {
//      if (Read_Index[i] > Write_Index[i]) /* Rollback */
//      {
//        buffsize = APP_TX_DATA_SIZE - Read_Index[i];
//      }
//      else
//      {
//        buffsize = Write_Index[i] - Read_Index[i];
//      }
//
//      buffptr = Read_Index[i];
//
//      USBD_CDC_SetTxBuffer(i, &hUsbDeviceHS, &TX_Buffer[i][buffptr], buffsize);
//
//      if (USBD_CDC_TransmitPacket(i, &hUsbDeviceHS) == USBD_OK)
//      {
//        Read_Index[i] += buffsize;
//        if (Read_Index[i] == APP_RX_DATA_SIZE)
//        {
//          Read_Index[i] = 0;
//        }
//      }
//    }
//  }
//}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
  uint8_t cdc_ch = UART_Handle_TO_CDC_CH(huart);
  /* Increment Index for buffer writing */
  Write_Index[cdc_ch]++;

  /* To avoid buffer overflow */
  if (Write_Index[cdc_ch] == APP_RX_DATA_SIZE)
  {
    Write_Index[cdc_ch] = 0;
  }

  /* Start another reception: provide the buffer pointer with offset and the buffer size */
  HAL_UART_Receive_IT(huart, (TX_Buffer[cdc_ch] + Write_Index[cdc_ch]), 1);
}
/* USER CODE END PRIVATE_FUNCTIONS_IMPLEMENTATION */

/**
  * @}
  */

/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
