/*******************************************************************************
* File Name: `$INSTANCE_NAME`_descr.c
* Version `$CY_MAJOR_VERSION`.`$CY_MINOR_VERSION`
*
*  Description:
*    USB descriptors and storage.
*
*   Note:
*
********************************************************************************
* Copyright 2008-2009, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions, 
* disclaimers, and limitations in the end user license agreement accompanying 
* the software package with which this file was provided.
********************************************************************************/

#include "cydevice.h"
#include "cyfitter.h"
#include "`$INSTANCE_NAME`.h"

/*******************************************************************************
*  User supplied descriptors.  If you want to specify your own descriptors,
*  remove the comments around the define USER_SUPPLIED_DESCRIPTORS below and
*  add your descriptors.
********************************************************************************/
/* `#START USER_DESCRIPTORS_DECLARATIONS` Place your declaration here */
/* `#END` */

/*******************************************************************************
*  USB Customizer Generated Descriptors
********************************************************************************/



/*********************************************************************
 Device Descriptors
 *********************************************************************/
uint8 `$INSTANCE_NAME`_CODE `$INSTANCE_NAME`_DEVICE0_DESCR[] = {
/* Descriptor Length                       */ 0x12u,
/* DescriptorType: DEVICE                  */ 0x01u,
/* bcdUSB (ver 2.0)                        */ 0x00u, 0x02u,
/* bDeviceClass: This is CDC class device  */ 0x02u,
/* bDeviceSubClass                         */ 0x00u,
/* bDeviceProtocol                         */ 0x00u,
/* bMaxPacketSize0                         */ 0x08u,
/* idVendor                                */ `$VID_LSB`, `$VID_MSB`,
/* idProduct                               */ `$PID_LSB`, `$PID_MSB`,
/* bcdDevice                               */ `$DeviceRelease_LSB`, `$DeviceRelease_MSB`,
/* iManufacturer                           */ 0x00u,
/* iProduct                                */ 0x00u,
/* iSerialNumber                           */ 0x00u,
/* bNumConfigurations                      */ 0x01u
};
/*********************************************************************
 Config Descriptor
 *********************************************************************/
uint8 `$INSTANCE_NAME`_CODE `$INSTANCE_NAME`_DEVICE0_CONFIGURATION0_DESCR[] = {
/*  Config Descriptor Length               */ 0x09u,
/*  DescriptorType: CONFIG                 */ 0x02u,
/*  wTotalLength: "Hand Computed"  67B     */ 0x43u, 0x00u,
/*  bNumInterfaces                         */ 0x02u,
/*  bConfigurationValue                    */ 0x01u,
/*  iConfiguration                         */ 0x00u,
/*  bmAttributes                           */ 0x80u,
/*  bMaxPower                              */ 0x31u,
/*********************************************************************
 Interface Descriptor (CDC Communications Class Interface)
 *********************************************************************/
/*  Interface Descriptor Length            */ 0x09u,
/*  DescriptorType: INTERFACE              */ 0x04u,
/*  bInterfaceNumber                       */ 0x00u,
/*  bAlternateSetting                      */ 0x00u,
/*  bNumEndpoints                          */ 0x01u,
/*  bInterfaceClass : Communication i/f    */ 0x02u,
/*  bInterfaceSubClass : ACD               */ 0x02u,
/*  bInterfaceProtocol : V.25ter           */ 0x01u,
/*  iInterface                             */ 0x00u,

/*********************************************************************
 CDC Class-specific Descriptors
 Header Functional Descriptor
 *********************************************************************/
/*  Descriptor Length                      */ 0x05u,
/*  DescriptorType: CS_INTERFACE           */ 0x24u,
/*  DescriptorSubType: Header Functional   */ 0x00u,
/*  bcdCDC: CDC Release Number             */ 0x10u, 0x01u,

/*********************************************************************
 CDC Class-specific Descriptors
 Abstract Control Management Functional Descriptor
 *********************************************************************/
/*  Descriptor Length                      */ 0x04u,
/*  DescriptorType: CS_INTERFACE           */ 0x24u,
/*  DescriptorSubType: ACM				   */ 0x02u,
/*  bmCapabilities :  TODO !!! WARNING     */ 0x02u,
/*   Supports the request combination of               */
/*   Set_Line_Coding, Set_Control_Line_State,          */
/*   Get_Line_Coding and the notification Serial_State */

/*********************************************************************
 CDC Class-specific Descriptors
 Union Functional Descriptor
 *********************************************************************/
/*  Descriptor Length                      */ 0x05u,
/*  DescriptorType: CS_INTERFACE           */ 0x24u,
/*  DescriptorSubType: Union Functional    */ 0x06u,
/*  bMasterInteface                        */ 0x00u,
/*  bSlaveInterface                        */ 0x01u,

/*********************************************************************
 CDC Class-specific Descriptors
 Call Management Functional Descriptor
 *********************************************************************/
/*  Descriptor Length                      */ 0x05u,
/*  DescriptorType: CS_INTERFACE           */ 0x24u,
/*  DescriptorSubType: Call Management     */ 0x01u,
/*  bmCapabilities:                        */ 0x00u,
/*   Device sends/receives call management information */
/*   only over the Communication Class Interface.      */
/*  bInterface: i/f # of Data Class i/f    */ 0x01u,

/*********************************************************************
 Endpoint Descriptor
 *********************************************************************/
/*  Endpoint Descriptor Length             */ 0x07u,
/*  DescriptorType: ENDPOINT               */ 0x05u,
/*  bEndpointAddress                       */ 0x81u,
/*  bmAttributes                           */ 0x03u,
/*  wMaxPacketSize                         */ 0x08u, 0x00u,
/*  bInterval                              */ 0x02u,
/*********************************************************************
 Interface Descriptor (CDC Data Interface)
 *********************************************************************/
/*  Interface Descriptor Length            */ 0x09u,
/*  DescriptorType: INTERFACE              */ 0x04u,
/*  bInterfaceNumber                       */ 0x01u,
/*  bAlternateSetting                      */ 0x00u,
/*  bNumEndpoints                          */ 0x02u,
/*  bInterfaceClass : Data Interface       */ 0x0Au,
/*  bInterfaceSubClass                     */ 0x00u,
/*  bInterfaceProtocol                     */ 0x00u,
/*  iInterface                             */ 0x00u,
/*********************************************************************
 Endpoint Descriptor
 *********************************************************************/
/*  Endpoint Descriptor Length             */ 0x07u,
/*  DescriptorType: ENDPOINT               */ 0x05u,
/*  bEndpointAddress                       */ 0x82u,
/*  bmAttributes                           */ 0x02u,
/*  wMaxPacketSize                         */ 0x40u, 0x00u,
/*  bInterval                              */ 0x00u,
/*********************************************************************
 Endpoint Descriptor
 *********************************************************************/
/*  Endpoint Descriptor Length             */ 0x07u,
/*  DescriptorType: ENDPOINT               */ 0x05u,
/*  bEndpointAddress                       */ 0x03u,
/*  bmAttributes                           */ 0x02u,
/*  wMaxPacketSize                         */ 0x40u, 0x00u,
/*  bInterval                              */ 0x00u
};


/*********************************************************************
  Endpoint Setting Table -- This table contain the endpoint setting
                            for each endpoint in the configuration.  It
                            contains the necessary information to
                            configure the endpoint hardware for each
                            interface and alternate setting.
  *********************************************************************/
T_`$INSTANCE_NAME`_EP_SETTINGS_BLOCK `$INSTANCE_NAME`_CODE `$INSTANCE_NAME`_DEVICE0_CONFIGURATION0_EP_SETTINGS_TABLE[] = {
/* IFC  ALT    EPAddr bmAttr MaxPktSize BufferType ********************/
{0x00u, 0x00u, 0x81u, 0x03u, 0x0008u,   0x00u},
{0x01u, 0x00u, 0x82u, 0x02u, 0x0040u,   0x00u},
{0x01u, 0x00u, 0x03u, 0x02u, 0x0040u,   0x00u}
};
/*********************************************************************
 Config Dispatch Table -- Points to the Config Descriptor and each of
                          and endpoint setup table and to each interface
                          table if it specifies a USB Class
 *********************************************************************/
T_`$INSTANCE_NAME`_LUT `$INSTANCE_NAME`_CODE `$INSTANCE_NAME`_DEVICE0_CONFIGURATION0_TABLE[] = {
    {0x01u,     &`$INSTANCE_NAME`_DEVICE0_CONFIGURATION0_DESCR},
    {0x03u,     &`$INSTANCE_NAME`_DEVICE0_CONFIGURATION0_EP_SETTINGS_TABLE},
    {0x00u,    `$INSTANCE_NAME`_NULL},
    {0x00u,    `$INSTANCE_NAME`_NULL}
};
/*********************************************************************
 Device Dispatch Table -- Points to the Device Descriptor and each of
                          and Configuration Tables for this Device
 *********************************************************************/
T_`$INSTANCE_NAME`_LUT `$INSTANCE_NAME`_CODE `$INSTANCE_NAME`_DEVICE0_TABLE[] = {
    {0x01u,     &`$INSTANCE_NAME`_DEVICE0_DESCR},
    {0x02u,     &`$INSTANCE_NAME`_DEVICE0_CONFIGURATION0_TABLE}
};
/*********************************************************************
 Device Table -- Indexed by the device number.
 *********************************************************************/
T_`$INSTANCE_NAME`_LUT `$INSTANCE_NAME`_CODE `$INSTANCE_NAME`_TABLE[] = {
    {0x01u,     &`$INSTANCE_NAME`_DEVICE0_TABLE}
};

#if defined(`$INSTANCE_NAME`_ENABLE_MSOS_STRING)
/*******************************************************************************
*  USB Microsoft OS String Descriptor
********************************************************************************/

uint8 `$INSTANCE_NAME`_CODE `$INSTANCE_NAME`_MSOS_DESCRIPTOR[] = {
/* Descriptor Length                       */ 0x12u,
/* DescriptorType: STRING                  */ 0x03u,
/* qwSignature */  'M', 0, 'S', 0, 'F', 0, 'T', 0, '1', 0, '0', 0, '0', 0,
/* bMS_VendorCode:                         */ `$INSTANCE_NAME`_GET_CONFIG_DESCRIPTOR,
/* bPad                				   	   */ 0x00u
};

/* Extended Compat ID Descriptor */

uint8 `$INSTANCE_NAME`_CODE `$INSTANCE_NAME`_MSOS_CONFIGURATION_DESCR[] = {
/*  Length of the descriptor 4 bytes       */ 0x28u, 0x00u, 0x00u, 0x00u,
/*  Version of the descriptor 2 bytes      */ 0x00u, 0x01u,
/*  wIndex - Fixed:INDEX_CONFIG_DESCRIPTOR */ 0x04u, 0x00u,
/*  bCount - Count of device functions.    */ 0x01u,
/*  Reserved : 7 bytes                     */ 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u,
/*  bFirstInterfaceNumber                  */ 0x02u,   /*TODO or 0 ?*/
/*  Reserved                           	   */ 0x01u,
/*  compatibleID    - "USBCIR\0\0"         */ 0x55u, 0x53u, 0x42u, 0x43u, 0x49u, 0x52u, 0x00u, 0x00u,
/*  subcompatibleID - "IR2CMPT\0"          */ 0x49u, 0x52u, 0x32u, 0x43u, 0x4Du, 0x50u, 0x54u, 0x00u,
/*  Reserved : 6 bytes                     */ 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u
}; 
#endif
