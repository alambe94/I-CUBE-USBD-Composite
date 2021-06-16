/**
  ******************************************************************************
  * File Name          : AL94.I-CUBE-USBD-COMPOSITE_conf.h
  * Description        : This file provides code for the configuration
  *                      of the AL94.I-CUBE-USBD-COMPOSITE_conf.h instances.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2021 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under Ultimate Liberty license
  * SLA0044, the "License"; You may not use this file except in compliance with
  * the License. You may obtain a copy of the License at:
  *                             www.st.com/SLA0044
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
	MiddleWare name : AL94.I-CUBE-USBD-COMPOSITE.1.0.0
	MiddleWare fileName : ./AL94.I-CUBE-USBD-COMPOSITE_conf.h
	MiddleWare version :
*/
/*---------- _USBD_USE_HS  -----------*/
#define _USBD_USE_HS      true

/*---------- _USBD_USE_CDC_ACM  -----------*/
#define _USBD_USE_CDC_ACM      false

/*---------- _USBD_CDC_ACM_COUNT  -----------*/
#define _USBD_CDC_ACM_COUNT      2

/*---------- _USBD_USE_CDC_RNDIS  -----------*/
#define _USBD_USE_CDC_RNDIS      false

/*---------- _USBD_USE_CDC_ECM  -----------*/
#define _USBD_USE_CDC_ECM      false

/*---------- _USBD_USE_HID_MOUSE  -----------*/
#define _USBD_USE_HID_MOUSE      false

/*---------- _USBD_USE_HID_KEYBOARD  -----------*/
#define _USBD_USE_HID_KEYBOARD      false

/*---------- _USBD_USE_HID_CUSTOM  -----------*/
#define _USBD_USE_HID_CUSTOM      false

/*---------- _USBD_USE_UAC_MIC  -----------*/
#define _USBD_USE_UAC_MIC      true

/*---------- _USBD_USE_UAC_SPKR  -----------*/
#define _USBD_USE_UAC_SPKR      false

/*---------- _USBD_USE_UVC  -----------*/
#define _USBD_USE_UVC      false

/*---------- _USBD_USE_MSC  -----------*/
#define _USBD_USE_MSC      true

/*---------- _USBD_USE_DFU  -----------*/
#define _USBD_USE_DFU      false

/*---------- _USBD_USE_PRNTR  -----------*/
#define _USBD_USE_PRNTR      false

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
