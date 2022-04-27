/**
  ******************************************************************************
  * File Name          : AL94.I-CUBE-USBD-COMPOSITE_conf.h
  * Description        : This file provides code for the configuration
  *                      of the AL94.I-CUBE-USBD-COMPOSITE_conf.h instances.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2022 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __AL94__I_CUBE_USBD_COMPOSITE_CONF__H__
#define __AL94__I_CUBE_USBD_COMPOSITE_CONF__H__

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/

/**
	MiddleWare name : AL94.I-CUBE-USBD-COMPOSITE.1.0.3
	MiddleWare fileName : AL94.I-CUBE-USBD-COMPOSITE_conf.h
	MiddleWare version :
*/
/*---------- _USBD_USE_HS  -----------*/
#define _USBD_USE_HS      true

/*---------- _USBD_USE_CDC_ACM  -----------*/
#define _USBD_USE_CDC_ACM      true

/*---------- _USBD_CDC_ACM_COUNT  -----------*/
#define _USBD_CDC_ACM_COUNT      1

/*---------- _USBD_USE_CDC_RNDIS  -----------*/
#define _USBD_USE_CDC_RNDIS      true

/*---------- _USBD_USE_CDC_ECM  -----------*/
#define _USBD_USE_CDC_ECM      true

/*---------- _USBD_USE_HID_MOUSE  -----------*/
#define _USBD_USE_HID_MOUSE      true

/*---------- _USBD_USE_HID_KEYBOARD  -----------*/
#define _USBD_USE_HID_KEYBOARD      true

/*---------- _USBD_USE_HID_CUSTOM  -----------*/
#define _USBD_USE_HID_CUSTOM      true

/*---------- _USBD_USE_UAC_MIC  -----------*/
#define _USBD_USE_UAC_MIC      true

/*---------- _USBD_USE_UAC_SPKR  -----------*/
#define _USBD_USE_UAC_SPKR      true

/*---------- _USBD_USE_UVC  -----------*/
#define _USBD_USE_UVC      true

/*---------- _USBD_USE_MSC  -----------*/
#define _USBD_USE_MSC      true

/*---------- _USBD_USE_DFU  -----------*/
#define _USBD_USE_DFU      true

/*---------- _USBD_USE_PRNTR  -----------*/
#define _USBD_USE_PRNTR      true

/*---------- _STM32F1_DEVICE  -----------*/
#define _STM32F1_DEVICE      false

#ifdef __cplusplus
}
#endif
#endif /*__ AL94__I_CUBE_USBD_COMPOSITE_CONF__H_H */

/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
