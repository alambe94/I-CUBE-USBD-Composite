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
	MiddleWare fileName : AL94.I-CUBE-USBD-COMPOSITE_conf.h
	MiddleWare version :
*/
/*---------- ENABLE_USB_CDC_ACM  -----------*/
#define ENABLE_USB_CDC_ACM      true

/*---------- USB_CDC_ACM_CMD_ITF_NBR  -----------*/
#define USB_CDC_ACM_CMD_ITF_NBR      0

/*---------- USB_CDC_ACM_COM_ITF_NBR  -----------*/
#define USB_CDC_ACM_COM_ITF_NBR      1

/*---------- USB_CDC_ACM_CMD_EP_NBR  -----------*/
#define USB_CDC_ACM_CMD_EP_NBR      1

/*---------- USB_CDC_ACM_IN_EP_NBR  -----------*/
#define USB_CDC_ACM_IN_EP_NBR      2

/*---------- USB_CDC_ACM_OUT_EP_NBR  -----------*/
#define USB_CDC_ACM_OUT_EP_NBR      1

/*---------- USB_CDC_ACM_CMD_EP_SIZE  -----------*/
#define USB_CDC_ACM_CMD_EP_SIZE      64

/*---------- USB_CDC_ACM_IN_EP_SIZE  -----------*/
#define USB_CDC_ACM_IN_EP_SIZE      512

/*---------- USB_CDC_ACM_OUT_EP_SIZE  -----------*/
#define USB_CDC_ACM_OUT_EP_SIZE      512

/*---------- ENABLE_USB_CDC_RNDIS  -----------*/
#define ENABLE_USB_CDC_RNDIS      false

/*---------- USB_CDC_RNDIS_CMD_ITF_NBR  -----------*/
#define USB_CDC_RNDIS_CMD_ITF_NBR      2

/*---------- USB_CDC_RNDIS_COM_ITF_NBR  -----------*/
#define USB_CDC_RNDIS_COM_ITF_NBR      3

/*---------- USB_CDC_RNDIS_CMD_EP_NBR  -----------*/
#define USB_CDC_RNDIS_CMD_EP_NBR      1

/*---------- USB_CDC_RNDIS_IN_EP_NBR  -----------*/
#define USB_CDC_RNDIS_IN_EP_NBR      1

/*---------- USB_CDC_RNDIS_OUT_EP_NBR  -----------*/
#define USB_CDC_RNDIS_OUT_EP_NBR      1

/*---------- USB_CDC_RNDIS_CMD_EP_SIZE  -----------*/
#define USB_CDC_RNDIS_CMD_EP_SIZE      64

/*---------- USB_CDC_RNDIS_IN_EP_SIZE  -----------*/
#define USB_CDC_RNDIS_IN_EP_SIZE      512

/*---------- USB_CDC_RNDIS_OUT_EP_SIZE  -----------*/
#define USB_CDC_RNDIS_OUT_EP_SIZE      512

#ifdef __cplusplus
}
#endif
#endif /*__ AL94__I_CUBE_USBD_COMPOSITE_CONF__H_H */

/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
