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


#if !defined(USBUART_H)
#define USBUART_H

#include "cytypes.h"

#if(CYDEV_CHIP_DIE_EXPECT == CYDEV_CHIP_DIE_LEOPARD)     
    #if(CYDEV_CHIP_REV_EXPECT <= CYDEV_CHIP_REV_LEOPARD_ES2)      
        #include <intrins.h>
        #define USBUART_ISR_PATCH() _nop_(); _nop_(); _nop_(); _nop_(); _nop_(); _nop_(); _nop_(); _nop_();
    #endif
#endif    

/************************************************
 *  General defines 
 ************************************************/
#if defined(__C51__) || defined(__CX51__)
#ifndef USBUART_CODE
#define USBUART_CODE code
#define USBUART_DATA data
#define USBUART_XDATA xdata
#define USBUART_FAR far
#define USBUART_NULL  ((void *) 0)
#endif
#else
#ifndef USBUART_CODE
#define USBUART_CODE
#define USBUART_DATA
#define USBUART_XDATA
#define USBUART_FAR
#define USBUART_NULL  ((void *) 0)
#endif
#endif

/************************************************
 *  Customizer Defines 
 ************************************************/

#define USBUART_MON_VBUS                   0
#define USBUART_EP0_ISR_REMOVE             0
#define USBUART_EP1_ISR_REMOVE             0
#define USBUART_EP2_ISR_REMOVE             0
#define USBUART_EP3_ISR_REMOVE             0
#define USBUART_EP4_ISR_REMOVE             0
#define USBUART_EP5_ISR_REMOVE             0
#define USBUART_EP6_ISR_REMOVE             0
#define USBUART_EP7_ISR_REMOVE             0
#define USBUART_EP8_ISR_REMOVE             0
#if (USBUART_MON_VBUS == 1)
#define USBUART_VBUS_DR                    ((reg8 *) USBUART_VBUS__DR)
#define USBUART_VBUS_PS                    ((reg8 *) USBUART_VBUS__PS)
#define USBUART_VBUS_MASK                  USBUART_VBUS__MASK
#endif

/************************************************
 *  Constants for USBUART API. 
 ************************************************/
#define USBUART_TRUE                       1
#define USBUART_FALSE                      0

#define USBUART_NO_EVENT_ALLOWED           2
#define USBUART_EVENT_PENDING              1
#define USBUART_NO_EVENT_PENDING           0

#define USBUART_IN_BUFFER_FULL             USBUART_NO_EVENT_PENDING
#define USBUART_IN_BUFFER_EMPTY            USBUART_EVENT_PENDING
#define USBUART_OUT_BUFFER_FULL            USBUART_EVENT_PENDING
#define USBUART_OUT_BUFFER_EMPTY           USBUART_NO_EVENT_PENDING

#define USBUART_FORCE_J                    0xA0
#define USBUART_FORCE_K                    0x80
#define USBUART_FORCE_SE0                  0xC0
#define USBUART_FORCE_NONE                 0x00

#define USBUART_IDLE_TIMER_RUNNING         0x02
#define USBUART_IDLE_TIMER_EXPIRED         0x01
#define USBUART_IDLE_TIMER_INDEFINITE      0x00

#define USBUART_DEVICE_STATUS_BUS_POWERED  0x00
#define USBUART_DEVICE_STATUS_SELF_POWERED 0x01

#define USBUART_3V_OPERATION               0x02
#define USBUART_5V_OPERATION               0x03

#define USBUART_MODE_DISABLE               0x00
#define	USBUART_MODE_NAK_IN_OUT            0x01
#define	USBUART_MODE_STATUS_OUT_ONLY       0x02
#define	USBUART_MODE_STALL_IN_OUT          0x03
#define	USBUART_MODE_RESERVED_0100         0x04
#define	USBUART_MODE_ISO_OUT               0x05
#define	USBUART_MODE_STATUS_IN_ONLY        0x06
#define	USBUART_MODE_ISO_IN                0x07
#define	USBUART_MODE_NAK_OUT               0x08
#define	USBUART_MODE_ACK_OUT               0x09
#define	USBUART_MODE_RESERVED_1010         0x0A
#define	USBUART_MODE_ACK_OUT_STATUS_IN     0x0B
#define	USBUART_MODE_NAK_IN                0x0C
#define	USBUART_MODE_ACK_IN                0x0D
#define	USBUART_MODE_RESERVED_1110         0x0E
#define	USBUART_MODE_ACK_IN_STATUS_OUT     0x0F
#define	USBUART_MODE_STALL_DATA_EP         0x80

#define USBUART_MODE_ACKD                  0x10
#define USBUART_MODE_OUT_RCVD              0x20
#define USBUART_MODE_IN_RCVD               0x40
#define USBUART_MODE_SETUP_RCVD            0x80

#define USBUART_RQST_TYPE_MASK             0x60
#define USBUART_RQST_TYPE_STD              0x00
#define USBUART_RQST_TYPE_CLS              0x20
#define USBUART_RQST_TYPE_VND              0x40
#define USBUART_RQST_DIR_MASK              0x80
#define USBUART_RQST_DIR_D2H               0x80
#define USBUART_RQST_DIR_H2D               0x00
#define USBUART_RQST_RCPT_MASK             0x03
#define USBUART_RQST_RCPT_DEV              0x00
#define USBUART_RQST_RCPT_IFC              0x01
#define USBUART_RQST_RCPT_EP               0x02
#define USBUART_RQST_RCPT_OTHER            0x03

/************************************************
 *  Standard Request Types (Table 9-4) 
 ************************************************/
#define USBUART_GET_STATUS           0x00
#define USBUART_CLEAR_FEATURE        0x01
#define USBUART_SET_FEATURE          0x03
#define USBUART_SET_ADDRESS          0x05
#define USBUART_GET_DESCRIPTOR       0x06
#define USBUART_SET_DESCRIPTOR       0x07
#define USBUART_GET_CONFIGURATION    0x08
#define USBUART_SET_CONFIGURATION    0x09
#define USBUART_GET_INTERFACE        0x0A
#define USBUART_SET_INTERFACE        0x0B
#define USBUART_SYNCH_FRAME          0x0C

/************************************************
 *  Vendor Specific Request Types 
 ************************************************/
 /*Request for Microsoft OS String Descriptor*/
#define USBUART_GET_CONFIG_DESCRIPTOR 0x01
/************************************************
 *  Descriptor Types (Table 9-5) 
 ************************************************/

#define USBUART_DESCR_DEVICE           1
#define USBUART_DESCR_CONFIG           2
#define USBUART_DESCR_STRING           3
#define USBUART_DESCR_INTERFACE        4
#define USBUART_DESCR_ENDPOINT         5
#define USBUART_DESCR_DEVICE_QUALIFIER 6
#define USBUART_DESCR_OTHER_SPEED      7
#define USBUART_DESCR_INTERFACE_POWER  8

/************************************************
 *  Device Descriptor Shifts 
 ************************************************/
#define USBUART_DEVICE_DESCR_SN_SHIFT	16

/************************************************
 *  Feature Selectors (Table 9-6)
 ************************************************/
#define USBUART_DEVICE_REMOTE_WAKEUP    0x01
#define USBUART_ENDPOINT_HALT           0x00
#define USBUART_TEST_MODE               0x02

/************************************************
 *  USB Device Status (Figure 9-4)
 ************************************************/
#define USBUART_DEVICE_STATUS_BUS_POWERED   0x00
#define USBUART_DEVICE_STATUS_SELF_POWERED  0x01
#define USBUART_DEVICE_STATUS_REMOTE_WAKEUP 0x02

/************************************************
 *  USB Endpoint Status (Figure 9-4)
 ************************************************/
#define USBUART_ENDPOINT_STATUS_HALT        0x01

/************************************************
 *  USB Endpoint Directions 
 ************************************************/
#define USBUART_DIR_IN                          0x80
#define USBUART_DIR_OUT                         0x00
#define USBUART_DIR_UNUSED                      0x7F
/************************************************
 *  USB Endpoint Attributes 
 ************************************************/
#define USBUART_EP_TYPE_CTRL                    0x00
#define USBUART_EP_TYPE_ISOC                    0x01
#define USBUART_EP_TYPE_BULK                    0x02
#define USBUART_EP_TYPE_INT                     0x03
#define USBUART_EP_TYPE_MASK                    0x03

#define USBUART_EP_SYNC_TYPE_NO_SYNC            0x00
#define USBUART_EP_SYNC_TYPE_ASYNC              0x04
#define USBUART_EP_SYNC_TYPE_ADAPTIVE           0x08
#define USBUART_EP_SYNC_TYPE_SYNCHRONOUS        0x0C
#define USBUART_EP_SYNC_TYPE_MASK               0x0C

#define USBUART_EP_USAGE_TYPE_DATA              0x00
#define USBUART_EP_USAGE_TYPE_FEEDBACK          0x10
#define USBUART_EP_USAGE_TYPE_IMPLICIT          0x20
#define USBUART_EP_USAGE_TYPE_RESERVED          0x30
#define USBUART_EP_USAGE_TYPE_MASK              0x30

/************************************************
 *  Transfer Completion Notification 
 ************************************************/
#define USBUART_XFER_IDLE                       0x00
#define USBUART_XFER_STATUS_ACK                 0x01
#define USBUART_XFER_PREMATURE                  0x02
#define USBUART_XFER_ERROR                      0x03

/*******************************************************************************
* Driver State defines
********************************************************************************/
#define USBUART_TRANS_STATE_IDLE             0x00
#define USBUART_TRANS_STATE_CONTROL_READ     0x02
#define USBUART_TRANS_STATE_CONTROL_WRITE    0x04
#define USBUART_TRANS_STATE_NO_DATA_CONTROL  0x06

/************************************************
 *  USB, ARB, SIE register definitions 
 ************************************************/
#define USBUART_ARB_CFG        ((reg8 *) USBUART_USB__ARB_CFG)

#define USBUART_ARB_EP1_CFG    ((reg8 *) USBUART_USB__ARB_EP1_CFG)
#define USBUART_ARB_EP1_INT_EN ((reg8 *) USBUART_USB__ARB_EP1_INT_EN)
#define USBUART_ARB_EP1_SR     ((reg8 *) USBUART_USB__ARB_EP1_SR)

#define USBUART_ARB_EP2_CFG    ((reg8 *) USBUART_USB__ARB_EP2_CFG)
#define USBUART_ARB_EP2_INT_EN ((reg8 *) USBUART_USB__ARB_EP2_INT_EN)
#define USBUART_ARB_EP2_SR     ((reg8 *) USBUART_USB__ARB_EP2_SR)

#define USBUART_ARB_EP3_CFG    ((reg8 *) USBUART_USB__ARB_EP3_CFG)
#define USBUART_ARB_EP3_INT_EN ((reg8 *) USBUART_USB__ARB_EP3_INT_EN)
#define USBUART_ARB_EP3_SR     ((reg8 *) USBUART_USB__ARB_EP3_SR)

#define USBUART_ARB_EP4_CFG    ((reg8 *) USBUART_USB__ARB_EP4_CFG)
#define USBUART_ARB_EP4_INT_EN ((reg8 *) USBUART_USB__ARB_EP4_INT_EN)
#define USBUART_ARB_EP4_SR     ((reg8 *) USBUART_USB__ARB_EP4_SR)

#define USBUART_ARB_EP5_CFG    ((reg8 *) USBUART_USB__ARB_EP5_CFG)
#define USBUART_ARB_EP5_INT_EN ((reg8 *) USBUART_USB__ARB_EP5_INT_EN)
#define USBUART_ARB_EP5_SR     ((reg8 *) USBUART_USB__ARB_EP5_SR)

#define USBUART_ARB_EP6_CFG    ((reg8 *) USBUART_USB__ARB_EP6_CFG)
#define USBUART_ARB_EP6_INT_EN ((reg8 *) USBUART_USB__ARB_EP6_INT_EN)
#define USBUART_ARB_EP6_SR     ((reg8 *) USBUART_USB__ARB_EP6_SR)

#define USBUART_ARB_EP7_CFG    ((reg8 *) USBUART_USB__ARB_EP7_CFG)
#define USBUART_ARB_EP7_INT_EN ((reg8 *) USBUART_USB__ARB_EP7_INT_EN)
#define USBUART_ARB_EP7_SR     ((reg8 *) USBUART_USB__ARB_EP7_SR)

#define USBUART_ARB_EP8_CFG    ((reg8 *) USBUART_USB__ARB_EP8_CFG)
#define USBUART_ARB_EP8_INT_EN ((reg8 *) USBUART_USB__ARB_EP8_INT_EN)
#define USBUART_ARB_EP8_SR     ((reg8 *) USBUART_USB__ARB_EP8_SR)

#define USBUART_ARB_INT_EN     ((reg8 *) USBUART_USB__ARB_INT_EN)
#define USBUART_ARB_INT_SR     ((reg8 *) USBUART_USB__ARB_INT_SR)

#define USBUART_ARB_RW1_DR     ((reg8 *) USBUART_USB__ARB_RW1_DR)
#define USBUART_ARB_RW1_RA     ((reg8 *) USBUART_USB__ARB_RW1_RA)
#define USBUART_ARB_RW1_RA_MSB ((reg8 *) USBUART_USB__ARB_RW1_RA_MSB)
#define USBUART_ARB_RW1_WA     ((reg8 *) USBUART_USB__ARB_RW1_WA)
#define USBUART_ARB_RW1_WA_MSB ((reg8 *) USBUART_USB__ARB_RW1_WA_MSB)

#define USBUART_ARB_RW2_DR     ((reg8 *) USBUART_USB__ARB_RW2_DR)
#define USBUART_ARB_RW2_RA     ((reg8 *) USBUART_USB__ARB_RW2_RA)
#define USBUART_ARB_RW2_RA_MSB ((reg8 *) USBUART_USB__ARB_RW2_RA_MSB)
#define USBUART_ARB_RW2_WA     ((reg8 *) USBUART_USB__ARB_RW2_WA)
#define USBUART_ARB_RW2_WA_MSB ((reg8 *) USBUART_USB__ARB_RW2_WA_MSB)

#define USBUART_ARB_RW3_DR     ((reg8 *) USBUART_USB__ARB_RW3_DR)
#define USBUART_ARB_RW3_RA     ((reg8 *) USBUART_USB__ARB_RW3_RA)
#define USBUART_ARB_RW3_RA_MSB ((reg8 *) USBUART_USB__ARB_RW3_RA_MSB)
#define USBUART_ARB_RW3_WA     ((reg8 *) USBUART_USB__ARB_RW3_WA)
#define USBUART_ARB_RW3_WA_MSB ((reg8 *) USBUART_USB__ARB_RW3_WA_MSB)

#define USBUART_ARB_RW4_DR     ((reg8 *) USBUART_USB__ARB_RW4_DR)
#define USBUART_ARB_RW4_RA     ((reg8 *) USBUART_USB__ARB_RW4_RA)
#define USBUART_ARB_RW4_RA_MSB ((reg8 *) USBUART_USB__ARB_RW4_RA_MSB)
#define USBUART_ARB_RW4_WA     ((reg8 *) USBUART_USB__ARB_RW4_WA)
#define USBUART_ARB_RW4_WA_MSB ((reg8 *) USBUART_USB__ARB_RW4_WA_MSB)

#define USBUART_ARB_RW5_DR     ((reg8 *) USBUART_USB__ARB_RW5_DR)
#define USBUART_ARB_RW5_RA     ((reg8 *) USBUART_USB__ARB_RW5_RA)
#define USBUART_ARB_RW5_RA_MSB ((reg8 *) USBUART_USB__ARB_RW5_RA_MSB)
#define USBUART_ARB_RW5_WA     ((reg8 *) USBUART_USB__ARB_RW5_WA)
#define USBUART_ARB_RW5_WA_MSB ((reg8 *) USBUART_USB__ARB_RW5_WA_MSB)

#define USBUART_ARB_RW6_DR     ((reg8 *) USBUART_USB__ARB_RW6_DR)
#define USBUART_ARB_RW6_RA     ((reg8 *) USBUART_USB__ARB_RW6_RA)
#define USBUART_ARB_RW6_RA_MSB ((reg8 *) USBUART_USB__ARB_RW6_RA_MSB)
#define USBUART_ARB_RW6_WA     ((reg8 *) USBUART_USB__ARB_RW6_WA)
#define USBUART_ARB_RW6_WA_MSB ((reg8 *) USBUART_USB__ARB_RW6_WA_MSB)

#define USBUART_ARB_RW7_DR     ((reg8 *) USBUART_USB__ARB_RW7_DR)
#define USBUART_ARB_RW7_RA     ((reg8 *) USBUART_USB__ARB_RW7_RA)
#define USBUART_ARB_RW7_RA_MSB ((reg8 *) USBUART_USB__ARB_RW7_RA_MSB)
#define USBUART_ARB_RW7_WA     ((reg8 *) USBUART_USB__ARB_RW7_WA)
#define USBUART_ARB_RW7_WA_MSB ((reg8 *) USBUART_USB__ARB_RW7_WA_MSB)

#define USBUART_ARB_RW8_DR     ((reg8 *) USBUART_USB__ARB_RW8_DR)
#define USBUART_ARB_RW8_RA     ((reg8 *) USBUART_USB__ARB_RW8_RA)
#define USBUART_ARB_RW8_RA_MSB ((reg8 *) USBUART_USB__ARB_RW8_RA_MSB)
#define USBUART_ARB_RW8_WA     ((reg8 *) USBUART_USB__ARB_RW8_WA)
#define USBUART_ARB_RW8_WA_MSB ((reg8 *) USBUART_USB__ARB_RW8_WA_MSB)

#define USBUART_BUF_SIZE       ((reg8 *) USBUART_USB__BUF_SIZE)
#define USBUART_BUS_RST_CNT    ((reg8 *) USBUART_USB__BUS_RST_CNT)
#define USBUART_CR0            ((reg8 *) USBUART_USB__CR0)
#define USBUART_CR1            ((reg8 *) USBUART_USB__CR1)
#define USBUART_CWA            ((reg8 *) USBUART_USB__CWA)
#define USBUART_CWA_MSB        ((reg8 *) USBUART_USB__CWA_MSB)

#define USBUART_DMA_THRES      ((reg8 *) USBUART_USB__DMA_THRES)
#define USBUART_DMA_THRES_MSB  ((reg8 *) USBUART_USB__DMA_THRES_MSB)

#define USBUART_EP_ACTIVE      ((reg8 *) USBUART_USB__EP_ACTIVE)
#define USBUART_EP_TYPE        ((reg8 *) USBUART_USB__EP_TYPE)

#define USBUART_EP0_CNT        ((reg8 *) USBUART_USB__EP0_CNT)
#define USBUART_EP0_CR         ((reg8 *) USBUART_USB__EP0_CR)
#define USBUART_EP0_DR0        ((reg8 *) USBUART_USB__EP0_DR0)
#define USBUART_EP0_DR1        ((reg8 *) USBUART_USB__EP0_DR1)
#define USBUART_EP0_DR2        ((reg8 *) USBUART_USB__EP0_DR2)
#define USBUART_EP0_DR3        ((reg8 *) USBUART_USB__EP0_DR3)
#define USBUART_EP0_DR4        ((reg8 *) USBUART_USB__EP0_DR4)
#define USBUART_EP0_DR5        ((reg8 *) USBUART_USB__EP0_DR5)
#define USBUART_EP0_DR6        ((reg8 *) USBUART_USB__EP0_DR6)
#define USBUART_EP0_DR7        ((reg8 *) USBUART_USB__EP0_DR7)

#define USBUART_OSCLK_DR0      ((reg8 *) USBUART_USB__OSCLK_DR0)
#define USBUART_OSCLK_DR1      ((reg8 *) USBUART_USB__OSCLK_DR1)

#define USBUART_PM_ACT_CFG     ((reg8 *) USBUART_USB__PM_ACT_CFG)
#define USBUART_PM_ACT_MSK     ((reg8 *) USBUART_USB__PM_ACT_MSK)
#define USBUART_PM_STBY_CFG    ((reg8 *) USBUART_USB__PM_STBY_CFG)
#define USBUART_PM_STBY_MSK    ((reg8 *) USBUART_USB__PM_STBY_MSK)

#define USBUART_SIE_EP_INT_EN  ((reg8 *) USBUART_USB__SIE_EP_INT_EN)
#define USBUART_SIE_EP_INT_SR  ((reg8 *) USBUART_USB__SIE_EP_INT_SR)

#define USBUART_SIE_EP1_CNT0   ((reg8 *) USBUART_USB__SIE_EP1_CNT0)
#define USBUART_SIE_EP1_CNT1   ((reg8 *) USBUART_USB__SIE_EP1_CNT1)
#define USBUART_SIE_EP1_CR0    ((reg8 *) USBUART_USB__SIE_EP1_CR0)

#define USBUART_SIE_EP2_CNT0   ((reg8 *) USBUART_USB__SIE_EP2_CNT0)
#define USBUART_SIE_EP2_CNT1   ((reg8 *) USBUART_USB__SIE_EP2_CNT1)
#define USBUART_SIE_EP2_CR0    ((reg8 *) USBUART_USB__SIE_EP2_CR0)

#define USBUART_SIE_EP3_CNT0   ((reg8 *) USBUART_USB__SIE_EP3_CNT0)
#define USBUART_SIE_EP3_CNT1   ((reg8 *) USBUART_USB__SIE_EP3_CNT1)
#define USBUART_SIE_EP3_CR0    ((reg8 *) USBUART_USB__SIE_EP3_CR0)

#define USBUART_SIE_EP4_CNT0   ((reg8 *) USBUART_USB__SIE_EP4_CNT0)
#define USBUART_SIE_EP4_CNT1   ((reg8 *) USBUART_USB__SIE_EP4_CNT1)
#define USBUART_SIE_EP4_CR0    ((reg8 *) USBUART_USB__SIE_EP4_CR0)

#define USBUART_SIE_EP5_CNT0   ((reg8 *) USBUART_USB__SIE_EP5_CNT0)
#define USBUART_SIE_EP5_CNT1   ((reg8 *) USBUART_USB__SIE_EP5_CNT1)
#define USBUART_SIE_EP5_CR0    ((reg8 *) USBUART_USB__SIE_EP5_CR0)

#define USBUART_SIE_EP6_CNT0   ((reg8 *) USBUART_USB__SIE_EP6_CNT0)
#define USBUART_SIE_EP6_CNT1   ((reg8 *) USBUART_USB__SIE_EP6_CNT1)
#define USBUART_SIE_EP6_CR0    ((reg8 *) USBUART_USB__SIE_EP6_CR0)

#define USBUART_SIE_EP7_CNT0   ((reg8 *) USBUART_USB__SIE_EP7_CNT0)
#define USBUART_SIE_EP7_CNT1   ((reg8 *) USBUART_USB__SIE_EP7_CNT1)
#define USBUART_SIE_EP7_CR0    ((reg8 *) USBUART_USB__SIE_EP7_CR0)

#define USBUART_SIE_EP8_CNT0   ((reg8 *) USBUART_USB__SIE_EP8_CNT0)
#define USBUART_SIE_EP8_CNT1   ((reg8 *) USBUART_USB__SIE_EP8_CNT1)
#define USBUART_SIE_EP8_CR0    ((reg8 *) USBUART_USB__SIE_EP8_CR0)

#define USBUART_SOF0           ((reg8 *) USBUART_USB__SOF0)
#define USBUART_SOF1           ((reg8 *) USBUART_USB__SOF1)

#define USBUART_USB_CLK_EN     ((reg8 *) USBUART_USB__USB_CLK_EN)

#define USBUART_USBIO_CR0      ((reg8 *) USBUART_USB__USBIO_CR0)
#define USBUART_USBIO_CR1      ((reg8 *) USBUART_USB__USBIO_CR1)
#define USBUART_USBIO_CR2      ((reg8 *) USBUART_USB__USBIO_CR2)

#define USBUART_USB_MEM        ((reg8 *) CYDEV_USB_MEM_BASE)

#if(CYDEV_CHIP_DIE_EXPECT == CYDEV_CHIP_DIE_LEOPARD)
    /* PSoC3 interrupt registers*/
    #define USBUART_USB_ISR_PRIOR  ((reg8 *) CYDEV_INTC_PRIOR0)
    #define USBUART_USB_ISR_SET_EN ((reg8 *) CYDEV_INTC_SET_EN0)
    #define USBUART_USB_ISR_CLR_EN ((reg8 *) CYDEV_INTC_CLR_EN0)
    #define USBUART_USB_ISR_VECT   ((cyisraddress *) CYDEV_INTC_VECT_MBASE)
#else
#if(CYDEV_CHIP_DIE_EXPECT == CYDEV_CHIP_DIE_PANTHER)
    /* PSoC5 interrupt registers*/ 
    #define USBUART_USB_ISR_PRIOR  ((reg8 *) CYDEV_NVIC_PRI_0)
    #define USBUART_USB_ISR_SET_EN ((reg8 *) CYDEV_NVIC_SETENA0)
    #define USBUART_USB_ISR_CLR_EN ((reg8 *) CYDEV_NVIC_CLRENA0)
    #define USBUART_USB_ISR_VECT   ((cyisraddress *) CYDEV_NVIC_VECT_OFFSET)
#endif
#endif

/************************************************
 *  Interrupt vectors, masks and priorities 
 ************************************************/
#define USBUART_BUS_RESET_PRIOR     USBUART_bus_reset__INTC_PRIOR_NUM
#define USBUART_BUS_RESET_MASK      USBUART_bus_reset__INTC_MASK
#define USBUART_BUS_RESET_VECT_NUM  USBUART_bus_reset__INTC_NUMBER
 
#define USBUART_SOF_PRIOR     USBUART_sof_int__INTC_PRIOR_NUM 
#define USBUART_SOF_MASK      USBUART_sof_int__INTC_MASK
#define USBUART_SOF_VECT_NUM  USBUART_sof_int__INTC_NUMBER
 
#define USBUART_EP_0_PRIOR     USBUART_ep_0__INTC_PRIOR_NUM
#define USBUART_EP_0_MASK      USBUART_ep_0__INTC_MASK
#define USBUART_EP_0_VECT_NUM  USBUART_ep_0__INTC_NUMBER
 
#define USBUART_EP_1_PRIOR     USBUART_ep_1__INTC_PRIOR_NUM 
#define USBUART_EP_1_MASK      USBUART_ep_1__INTC_MASK
#define USBUART_EP_1_VECT_NUM  USBUART_ep_1__INTC_NUMBER
 
#define USBUART_EP_2_PRIOR     USBUART_ep_2__INTC_PRIOR_NUM 
#define USBUART_EP_2_MASK      USBUART_ep_2__INTC_MASK
#define USBUART_EP_2_VECT_NUM  USBUART_ep_2__INTC_NUMBER
 
#define USBUART_EP_3_PRIOR     USBUART_ep_3__INTC_PRIOR_NUM
#define USBUART_EP_3_MASK      USBUART_ep_3__INTC_MASK
#define USBUART_EP_3_VECT_NUM  USBUART_ep_3__INTC_NUMBER
 
#define USBUART_EP_4_PRIOR     USBUART_ep_4__INTC_PRIOR_NUM 
#define USBUART_EP_4_MASK      USBUART_ep_4__INTC_MASK
#define USBUART_EP_4_VECT_NUM  USBUART_ep_4__INTC_NUMBER
 
#define USBUART_EP_5_PRIOR     USBUART_ep_5__INTC_PRIOR_NUM
#define USBUART_EP_5_MASK      USBUART_ep_5__INTC_MASK
#define USBUART_EP_5_VECT_NUM  USBUART_ep_5__INTC_NUMBER
 
#define USBUART_EP_6_PRIOR     USBUART_ep_6__INTC_PRIOR_NUM
#define USBUART_EP_6_MASK      USBUART_ep_6__INTC_MASK
#define USBUART_EP_6_VECT_NUM  USBUART_ep_6__INTC_NUMBER
 
#define USBUART_EP_7_PRIOR     USBUART_ep_7__INTC_PRIOR_NUM 
#define USBUART_EP_7_MASK      USBUART_ep_7__INTC_MASK
#define USBUART_EP_7_VECT_NUM  USBUART_ep_7__INTC_NUMBER
 
#define USBUART_EP_8_PRIOR     USBUART_ep_8__INTC_PRIOR_NUM
#define USBUART_EP_8_MASK      USBUART_ep_8__INTC_MASK
#define USBUART_EP_8_VECT_NUM  USBUART_ep_8__INTC_NUMBER
 
/************************************************
 *  Register Masks 
 ************************************************/
#define USBUART_CR1_REG_ENABLE             0x01u
#define USBUART_CR1_ENABLE_LOCK            0x02u
#define USBUART_CR1_BUS_ACTIVITY           0x04u
#define USBUART_EP0_CNT_DATA_TOGGLE        0x80u
#define USBUART_EPX_CNT_DATA_TOGGLE        0x80u
#define USBUART_EPX_CNT0_MASK              0x07u
#define USBUART_CR0_ENABLE                 0x80u

#define USBUART_USBIO_CR1_IOMODE           0x80u
#define USBUART_USBIO_CR1_DRIVE_MODE       0x40u
#define USBUART_USBIO_CR1_DPI              0x20u
#define USBUART_USBIO_CR1_DMI              0x10u
#define USBUART_USBIO_CR1_P2PUEN           0x08u
#define USBUART_USBIO_CR1_USBPUEN          0x04u
#define USBUART_USBIO_CR1_DP0              0x02u
#define USBUART_USBIO_CR1_DM0              0x01u

#define USBUART_FASTCLK_IMO_CR_USBCLK_ON   0x40u
#define USBUART_FASTCLK_IMO_CR_XCLKEN      0x20u
#define USBUART_FASTCLK_IMO_CR_FX2ON       0x10u

#define USBUART_ARB_EPX_CFG_RESET_PTR      0x08u
#define USBUART_ARB_EPX_CFG_CRC_BYPASS     0x04u
#define USBUART_ARB_EPX_CFG_DMA_REQ        0x02u
#define USBUART_ARB_EPX_CFG_IN_DATA_RDY    0x01u

/************************************************
 *  Endpoint 0 offsets (Table 9-2) 
 ************************************************/

#define USBUART_bmRequestType   USBUART_EP0_DR0
#define USBUART_bRequest        USBUART_EP0_DR1
#define USBUART_wValue          USBUART_EP0_DR2
#define USBUART_wValueHi        USBUART_EP0_DR3
#define USBUART_wValueLo        USBUART_EP0_DR2
#define USBUART_wIndex          USBUART_EP0_DR4
#define USBUART_wIndexHi        USBUART_EP0_DR5
#define USBUART_wIndexLo        USBUART_EP0_DR4
#define USBUART_wLength         USBUART_EP0_DR6
#define USBUART_wLengthHi       USBUART_EP0_DR7
#define USBUART_wLengthLo       USBUART_EP0_DR6

#define TRUE  1
#define FALSE 0
/************************************************
 *  Optional Features 
 ************************************************/


#if (USBUART_MON_VBUS == 1)
#define USBUART_VBUS_DR        ((reg8 *) USBUART_VBUS__DR)
#define USBUART_VBUS_PS        ((reg8 *) USBUART_VBUS__PS)
#define USBUART_VBUS_MASK      USBUART_VBUS__MASK
#endif

/*******************************************************************************
* typedefs
********************************************************************************/
typedef struct {
    uint8  bAttrib;
    uint8  bAPIEPState;
    uint8  bHWEPState;
    uint8  bEPToggle;
    uint8  bAddr;
    uint8  bEPMode;
    uint16 wBuffOffset;
    uint16 wBufferSize;
} T_USBUART_EP_CTL_BLOCK;

typedef struct {
    uint8  bInterface;
    uint8  bAltSetting;
    uint8  bAddr;
    uint8  bmAttributes;
    uint16 wBufferSize;
	uint8  bMisc;
} T_USBUART_EP_SETTINGS_BLOCK;

typedef struct {
    uint8  bStatus;
    uint16 wLength;
} T_USBUART_XFER_STATUS_BLOCK;

typedef struct {
    uint16  wCount;
    uint8   *pData;
    T_USBUART_XFER_STATUS_BLOCK *pStatusBlock;
} T_USBUART_TD;

typedef struct {
    uint8   c;
    void    *p_list;
} T_USBUART_LUT;

/************************************************
 *  Prototypes of the USBUART API. 
 ************************************************/
extern void   USBUART_Start(uint8 bDevice, uint8 bMode);
extern void   USBUART_Stop(void);
extern uint8  USBUART_bCheckActivity(void);
extern uint8  USBUART_bGetConfiguration(void);
uint8  USBUART_bGetInterfaceSetting(uint8 ifc);
extern uint8  USBUART_bGetEPState(uint8 bEPNumber);
extern uint16 USBUART_wGetEPCount(uint8 bEPNumber);
extern void   USBUART_LoadEP(uint8 bEPNumber, uint8 *pData, uint16 wLength);
extern uint16 USBUART_ReadOutEP(uint8 bEPNumber, uint8 *pData, uint16 wLength);
extern void   USBUART_EnableOutEP(uint8 bEPNumber);
extern void   USBUART_DisableOutEP(uint8 bEPNumber);
extern void   USBUART_Force(uint8 bState);
extern uint8  USBUART_bGetEPAckState(uint8 bEPNumber);
extern void   USBUART_SetPowerStatus(uint8 bPowerStaus);
extern uint8  USBUART_bRWUEnabled(void);
extern void  USBUART_SerialNumString(uint8 *SNstring);


#define USBUART_LoadInEP(e, p, l)        USBUART_LoadEP(e, p, l)
#define USBUART_LoadInISOCEP(e, p, l)    USBUART_LoadEP(e, p, l)
#define USBUART_EnableOutISOCEP(e)       USBUART_EnableOutEP(e)

#if (USBUART_MON_VBUS == 1)
extern uint8  USBUART_bVBusPresent(void);
#endif

#endif
