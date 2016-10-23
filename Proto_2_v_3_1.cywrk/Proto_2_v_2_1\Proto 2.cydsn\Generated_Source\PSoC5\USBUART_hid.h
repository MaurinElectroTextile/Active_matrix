/*******************************************************************************
* File Name: USBUART.h
* Version 0.0
*
*  Description:
*    Header File for the USFS component. Contains prototypes and constant values. 
*
********************************************************************************
* Copyright 2008-2009, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions, 
* disclaimers, and limitations in the end user license agreement accompanying 
* the software package with which this file was provided.
********************************************************************************/


#if !defined(USBUART_hid_H)
#define USBUART_hid_H

#include "cytypes.h"
/************************************************
 *  Prototypes of the USBUART API. 
 ************************************************/
extern uint8 USBFS_UpdateHIDTimer(uint8 bInterface);
extern uint8 USBFS_bGetProtocol(uint8 bInterface);

/************************************************
 *  Constants for USBUART API. 
 ************************************************/

#define USBUART_IDLE_TIMER_RUNNING         0x02
#define USBUART_IDLE_TIMER_EXPIRED         0x01
#define USBUART_IDLE_TIMER_INDEFINITE      0x00

#define USBUART_PROTOCOL_BOOT              0x00
#define USBUART_PROTOCOL_REPORT            0x01

/************************************************
 *  Request Types (HID Chapter 7.2) 
 ************************************************/

#define USBUART_HID_GET_REPORT       0x01
#define USBUART_HID_GET_IDLE         0x02
#define USBUART_HID_GET_PROTOCOL     0x03
#define USBUART_HID_SET_REPORT       0x09
#define USBUART_HID_SET_IDLE         0x0A
#define USBUART_HID_SET_PROTOCOL     0x0B

/************************************************
 *  Descriptor Types (HID Chapter 7.1) 
 ************************************************/

#define USBUART_DESCR_HID_CLASS         0x21
#define USBUART_DESCR_HID_REPORT        0x22
#define USBUART_DESCR_HID_PHYSICAL      0x23

#endif
