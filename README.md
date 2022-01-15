# I-CUBE-USBD-Composite
A wrapper class around ST USB stack to create STM32 USB Composite devices with ease.
## Modifications
* CDC supports multiple instance
* Removed ST use of malloc
* UAC Microphone & Speaker separated
* HID Keyboard & Mouse separated

## TODO
* Add CubeMX Templates
* Fix UAC combination with other classes

## How to use
### 1. Download AL94.I-CUBE-USBD-COMPOSITE.1.0.0.pack from repo.
![Download](pics/USE/1.Download.PNG)

### 2. Add AL94.I-CUBE-USBD-COMPOSITE.1.0.0.pack to CubeMX.
![ADD1](pics/USE/2.ADD1.png)

![LOCAL](pics/USE/3.LOCAL.png)

![ADD2](pics/USE/4.ADD2.png)

![INSTALLED](pics/USE/5.installed.png)

### 3. Inlude in project
Enable USB peripheral & NVIC
![NVIC](pics/USE/6.NVIC.png)

Add middleware to project
![INCLUDE](pics/USE/7.INCLUDE.png)

Add components to project, Selection of Core & COMPOSITE is must
![SELECT](pics/USE/8.SELECT.png)

Enable the classes you want
![ENABLE](pics/USE/9.ENABLE.png)

Dont forget!!!
![SELECT2](pics/USE/10.SELECT2.png)

### 4. Generate Code
Call MX_USB_DEVICE_Init(); manually
![SELECT2](pics/USE/11.CALL.png)


## Enumerated
### 1. USB CDC ACM.
![CDC_ACM](pics/ENUM/CDC_ACM.PNG)

### 2. CDC & HID.
![HID_CDC](pics/ENUM/HID_CDC.PNG)

### 3. RNDIS.
![RNDIS](pics/ENUM/RNDIS.PNG)

### 4. UAC Microphone & Speaker.
![UAC](pics/ENUM/UAC.PNG)

### 5. UVC Camera & Mass Storage.
![UVC_MSC](pics/ENUM/UVC_MSC.PNG)

# Troubleshooting
1. Cross check number of endpoints in MCU & consumed by application.
2. Adjust Endpont Size & PMA buffers in "Target/usbd_conf.c" accordingly.
3. For some classes "SOF" must be enabled!!!
4. Make sure MCU clock is configured properly & USB Interrupt is enabled.
5. For L5 HAL_PWREx_EnableVddUSB() needs to be called before enabling USB operation.
