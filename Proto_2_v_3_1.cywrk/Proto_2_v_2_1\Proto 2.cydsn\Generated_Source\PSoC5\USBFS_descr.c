/*******************************************************************************
* File Name: USBFS_descr.c
* Version 1.50
*
* Description:
*  USB descriptors and storage.
*
*  Note:
*
********************************************************************************
* Copyright 2008-2010, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
********************************************************************************/

#include "USBFS.h"


/*******************************************************************************
*  User supplied descriptors.  If you want to specify your own descriptors,
*  remove the comments around the define USER_SUPPLIED_DESCRIPTORS below and
*  add your descriptors.
********************************************************************************/
/* `#START USER_DESCRIPTORS_DECLARATIONS` Place your declaration here */

/* `#END` */


/***************************************
*  USB Customizer Generated Descriptors
***************************************/

#if !defined(USER_SUPPLIED_DESCRIPTORS)
    /*********************************************************************
 Device Descriptors
 *********************************************************************/
uint8 CYCODE USBFS_DEVICE0_DESCR[] = {
/* Descriptor Length                       */ 0x12u,
/* DescriptorType: DEVICE                  */ 0x01u,
/* bcdUSB (ver 2.0)                        */ 0x00u, 0x02u,
/* bDeviceClass                            */ 0x00u,
/* bDeviceSubClass                         */ 0x00u,
/* bDeviceProtocol                         */ 0x00u,
/* bMaxPacketSize0                         */ 0x08u,
/* idVendor                                */ 0x00u, 0xEEu,
/* idProduct                               */ 0x02u, 0x00u,
/* bcdDevice                               */ 0x00u, 0x00u,
/* iManufacturer                           */ 0x01u,
/* iProduct                                */ 0x02u,
/* iSerialNumber                           */ 0x00u,
/* bNumConfigurations                      */ 0x01u
};
/*********************************************************************
 Config Descriptor  
 *********************************************************************/
uint8 CYCODE USBFS_DEVICE0_CONFIGURATION0_DESCR[] = {
/*  Config Descriptor Length               */ 0x09u,
/*  DescriptorType: CONFIG                 */ 0x02u,
/*  wTotalLength                           */ 0x20u, 0x00u,
/*  bNumInterfaces                         */ 0x01u,
/*  bConfigurationValue                    */ 0x01u,
/*  iConfiguration                         */ 0x04u,
/*  bmAttributes                           */ 0x80u,
/*  bMaxPower                              */ 0x32u,
/*********************************************************************
 Interface Descriptor
 *********************************************************************/
/*  Interface Descriptor Length            */ 0x09u,
/*  DescriptorType: INTERFACE              */ 0x04u,
/*  bInterfaceNumber                       */ 0x00u,
/*  bAlternateSetting                      */ 0x00u,
/*  bNumEndpoints                          */ 0x02u,
/*  bInterfaceClass                        */ 0x00u,
/*  bInterfaceSubClass                     */ 0x00u,
/*  bInterfaceProtocol                     */ 0x00u,
/*  iInterface                             */ 0x03u,
/*********************************************************************
 Endpoint Descriptor
 *********************************************************************/
/*  Endpoint Descriptor Length             */ 0x07u,
/*  DescriptorType: ENDPOINT               */ 0x05u,
/*  bEndpointAddress                       */ 0x82u,
/*  bmAttributes                           */ 0x03u,
/*  wMaxPacketSize                         */ 0x2Au, 0x00u,
/*  bInterval                              */ 0x64u,
/*********************************************************************
 Endpoint Descriptor
 *********************************************************************/
/*  Endpoint Descriptor Length             */ 0x07u,
/*  DescriptorType: ENDPOINT               */ 0x05u,
/*  bEndpointAddress                       */ 0x81u,
/*  bmAttributes                           */ 0x02u,
/*  wMaxPacketSize                         */ 0x80u, 0x00u,
/*  bInterval                              */ 0x01u
};

    /*********************************************************************
 String Descriptor Table
 *********************************************************************/
uint8 CYCODE USBFS_STRING_DESCRIPTORS[] = {
/*********************************************************************
 Language ID Descriptor
 *********************************************************************/
/* Descriptor Length                       */ 0x04u,
 /* DescriptorType: STRING                  */ 0x03u,
 /* Language Id                             */ 0x09u, 0x04u,
/*********************************************************************
 String Descriptor: "XY Interaction"
 *********************************************************************/
/* Descriptor Length                       */ 0x1Eu,
/* DescriptorType: STRING                  */ 0x03u,
 'X', 0,'Y', 0,' ', 0,'I', 0,'n', 0,'t', 0,'e', 0,'r', 0,'a', 0,'c', 0
,'t', 0,'i', 0,'o', 0,'n', 0,
/*********************************************************************
 String Descriptor: "Active Matrix Driver"
 *********************************************************************/
/* Descriptor Length                       */ 0x2Au,
/* DescriptorType: STRING                  */ 0x03u,
 'A', 0,'c', 0,'t', 0,'i', 0,'v', 0,'e', 0,' ', 0,'M', 0,'a', 0,'t', 0
,'r', 0,'i', 0,'x', 0,' ', 0,'D', 0,'r', 0,'i', 0,'v', 0,'e', 0,'r', 0,
/*********************************************************************
 String Descriptor: "Position"
 *********************************************************************/
/* Descriptor Length                       */ 0x12u,
/* DescriptorType: STRING                  */ 0x03u,
 'P', 0,'o', 0,'s', 0,'i', 0,'t', 0,'i', 0,'o', 0,'n', 0,
/*********************************************************************
 String Descriptor: "Cypress default touchpad"
 *********************************************************************/
/* Descriptor Length                       */ 0x32u,
/* DescriptorType: STRING                  */ 0x03u,
 'C', 0,'y', 0,'p', 0,'r', 0,'e', 0,'s', 0,'s', 0,' ', 0,'d', 0,'e', 0
,'f', 0,'a', 0,'u', 0,'l', 0,'t', 0,' ', 0,'t', 0,'o', 0,'u', 0,'c', 0
,'h', 0,'p', 0,'a', 0,'d', 0,
/*********************************************************************/
/* Marks the end of the list.              */ 0x00u};
/*********************************************************************/

    /*********************************************************************
 Serial Number String Descriptor
 *********************************************************************/
uint8 CYCODE USBFS_SN_STRING_DESCRIPTOR[] = {
/* Descriptor Length                       */ 0x0Au,
 /* DescriptorType: STRING                  */ 0x03u,
 '0', 0,'0', 0,'0', 0,'1', 0
};

    
    
    /*********************************************************************
  Endpoint Setting Table -- This table contain the endpoint setting
                            for each endpoint in the configuration.  It
                            contains the necessary information to
                            configure the endpoint hardware for each
                            interface and alternate setting.
  *********************************************************************/
T_USBFS_EP_SETTINGS_BLOCK CYCODE USBFS_DEVICE0_CONFIGURATION0_EP_SETTINGS_TABLE[] = {
/* IFC  ALT    EPAddr bmAttr MaxPktSize BufferType ********************/
{0x00u, 0x00u, 0x82u, 0x03u, 0x002Au,   0x00u},
{0x00u, 0x00u, 0x81u, 0x02u, 0x0080u,   0x00u}
};
/*********************************************************************
 Config Dispatch Table -- Points to the Config Descriptor and each of
                          and endpoint setup table and to each interface 
                          table if it specifies a USB Class 
 *********************************************************************/
T_USBFS_LUT CYCODE USBFS_DEVICE0_CONFIGURATION0_TABLE[] = {
    {0x01u,     &USBFS_DEVICE0_CONFIGURATION0_DESCR},
    {0x02u,     &USBFS_DEVICE0_CONFIGURATION0_EP_SETTINGS_TABLE},
    {0x00u,    USBFS_NULL}
};
/*********************************************************************
 Device Dispatch Table -- Points to the Device Descriptor and each of
                          and Configuration Tables for this Device 
 *********************************************************************/
T_USBFS_LUT CYCODE USBFS_DEVICE0_TABLE[] = {
    {0x01u,     &USBFS_DEVICE0_DESCR},
    {0x01u,     &USBFS_DEVICE0_CONFIGURATION0_TABLE}
};
/*********************************************************************
 Device Table -- Indexed by the device number.
 *********************************************************************/
T_USBFS_LUT CYCODE USBFS_TABLE[] = {
    {0x01u,     &USBFS_DEVICE0_TABLE}
};

#endif /* USER_SUPPLIED_DESCRIPTORS */

#if defined(USBFS_ENABLE_MSOS_STRING)

    /*******************************************************************************
    *  USB Microsoft OS String Descriptor
    *  "MSFT" identifies a Microsoft host
    *  "100" specifies version 1.00
    *  USBFS_GET_EXTENDED_CONFIG_DESCRIPTOR becomes the bRequest value
    *  in a host vendor device/class request
    *******************************************************************************/

    uint8 CYCODE USBFS_MSOS_DESCRIPTOR[] = {
    /* Descriptor Length                       */   0x12u,
    /* DescriptorType: STRING                  */   0x03u,
    /* qwSignature */                               'M', 0, 'S', 0, 'F', 0, 'T', 0, '1', 0, '0', 0, '0', 0,
    /* bMS_VendorCode:                         */   USBFS_GET_EXTENDED_CONFIG_DESCRIPTOR,
    /* bPad                                    */   0x00u
    };

    /* Extended Configuration Descriptor */

    uint8 CYCODE USBFS_MSOS_CONFIGURATION_DESCR[] = {
    /*  Length of the descriptor 4 bytes       */   0x28u, 0x00u, 0x00u, 0x00u,
    /*  Version of the descriptor 2 bytes      */   0x00u, 0x01u,
    /*  wIndex - Fixed:INDEX_CONFIG_DESCRIPTOR */   0x04u, 0x00u,
    /*  bCount - Count of device functions.    */   0x01u,
    /*  Reserved : 7 bytes                     */   0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u,
    /*  bFirstInterfaceNumber                  */   0x00u,
    /*  Reserved                               */   0x01u,
    /*  compatibleID    - "CYUSB\0\0"          */   'C', 'Y', 'U', 'S', 'B', 0x00, 0x00u, 0x00u,
    /*  subcompatibleID - "00001\0\0"          */   '0', '0', '0', '0', '1', 0x00, 0x00u, 0x00u,
    /*  Reserved : 6 bytes                     */   0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u
    };
    
#endif /* USBFS_ENABLE_MSOS_STRING */


/* [] END OF FILE */
