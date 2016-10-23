/*******************************************************************************
* File Name: USBUART.c
* Version 0.0
*
*  Description:
*    API for USBFS Component.
*
*   Note:
*    6/16/10: Updated to USBFS_v1_30 version of files, fixing interrupt setup changes for PSoC 5
*   
*   Many of the functions use endpoint number.  RAM arrays are sized with 9
*   elements so they are indexed directly by bEPNumber.  The SIE and ARB
*   registers are indexed by variations of bEPNumber - 1.
*
********************************************************************************
* Copyright 2008-2009, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions, 
* disclaimers, and limitations in the end user license agreement accompanying 
* the software package with which this file was provided.
********************************************************************************/



#include "cydevice_trm.h"
#include "cyfitter.h"
#include "CyLib.h"
#include "USBUART.h"

/*******************************************************************************
* External data references
********************************************************************************/
extern uint8 USBUART_bConfiguration;
extern uint8 USBUART_bInterfaceSetting[];
extern uint8 USBUART_bDeviceAddress;
extern uint8 USBUART_bDeviceStatus;
extern uint8 USBUART_bEPHalt;
extern uint8 USBUART_bDevice;
extern uint8 USBUART_bTransferState;
extern T_USBUART_EP_CTL_BLOCK USBUART_EP[];

/*******************************************************************************
* Forward function references
********************************************************************************/
void USBUART_InitComponent(uint8 bDevice, uint8 bMode);

/*******************************************************************************
* External function references
********************************************************************************/
CY_ISR_PROTO(USBUART_EP_0_ISR);
#if(USBUART_EP1_ISR_REMOVE == 0)
CY_ISR_PROTO(USBUART_EP_1_ISR);
#endif   
#if(USBUART_EP2_ISR_REMOVE == 0)
CY_ISR_PROTO(USBUART_EP_2_ISR);
#endif   
#if(USBUART_EP3_ISR_REMOVE == 0)
CY_ISR_PROTO(USBUART_EP_3_ISR);
#endif   
#if(USBUART_EP4_ISR_REMOVE == 0)
CY_ISR_PROTO(USBUART_EP_4_ISR);
#endif   
#if(USBUART_EP5_ISR_REMOVE == 0)
CY_ISR_PROTO(USBUART_EP_5_ISR);
#endif   
#if(USBUART_EP6_ISR_REMOVE == 0)
CY_ISR_PROTO(USBUART_EP_6_ISR);
#endif   
#if(USBUART_EP7_ISR_REMOVE == 0)
CY_ISR_PROTO(USBUART_EP_7_ISR);
#endif   
#if(USBUART_EP8_ISR_REMOVE == 0)
CY_ISR_PROTO(USBUART_EP_8_ISR);
#endif   
CY_ISR_PROTO(USBUART_BUS_RESET_ISR);
CY_ISR_PROTO(USBUART_SOF_ISR);

/*******************************************************************************
* Function Name: USBUART_Start
********************************************************************************
* Summary:
*   This function initialize the USB SIE, arbiter and the
*   endpoint APIs, including setting the D+ Pullup
*
* Parameters:  
*   None
*******************************************************************************/
void USBUART_Start(uint8 bDevice, uint8 bMode)
{
    uint16 i;
    uint8 *p = (uint8 *)&USBUART_ARB_RW1_DR[0];
    
    /* Enable USB block */
    CY_SET_REG8(CYDEV_PM_ACT_CFG5, CY_GET_REG8(CYDEV_PM_ACT_CFG5) | 0x01);
    //TODO: pbvr: disable PM available for USB
    CY_SET_REG8(CYDEV_PM_AVAIL_CR6, CY_GET_REG8(CYDEV_PM_AVAIL_CR6) & ~0x10);
    /* Enable core clock */
    CY_SET_REG8(USBUART_USB_CLK_EN, 0x01);
    
    /* Bus Reset Length */
    CY_SET_REG8(USBUART_BUS_RST_CNT, 3);
    // TODO: pbvr: Enable PM available for USB
    CY_SET_REG8(CYDEV_PM_AVAIL_CR6, CY_GET_REG8(CYDEV_PM_AVAIL_CR6) | 0x10);
    /* If VBUS Monitoring is enable, setup the DR in the port reg */
#if (USBUART_MON_VBUS == 1)
    CY_SET_REG8(USBUART_VBUS_DR, CY_GET_REG8(USBUART_VBUS_DR) & ~USBUART_VBUS_MASK);
#endif

    /* Write WAx */
    CY_SET_REG8(&USBUART_ARB_RW1_WA[0],     0);
    CY_SET_REG8(&USBUART_ARB_RW1_WA_MSB[0], 0);
    
    /* Copy the data using the arbiter data register */
    for (i = 0; i < 512; i++)
    {
        CY_SET_REG8(p, 0xFF);
    }

    /* Set the bus reset Interrupt. */
    CyIntSetVector(USBUART_BUS_RESET_VECT_NUM,   USBUART_BUS_RESET_ISR);
    CyIntSetPriority(USBUART_BUS_RESET_VECT_NUM, USBUART_BUS_RESET_PRIOR);
    CyIntEnable(USBUART_BUS_RESET_VECT_NUM);

    /* Set the SOF Interrupt. */
    CyIntSetVector(USBUART_SOF_VECT_NUM,   USBUART_SOF_ISR);
    CyIntSetPriority(USBUART_SOF_VECT_NUM, USBUART_SOF_PRIOR);
    CyIntEnable(USBUART_SOF_VECT_NUM);

    /* Set the Control Endpoint Interrupt. */
    CyIntSetVector(USBUART_EP_0_VECT_NUM,   USBUART_EP_0_ISR);
    CyIntSetPriority(USBUART_EP_0_VECT_NUM, USBUART_EP_0_PRIOR);
    CyIntEnable(USBUART_EP_0_VECT_NUM);

    /* Set the Data Endpoint 1 Interrupt. */
#if(USBUART_EP1_ISR_REMOVE == 0)
    CyIntSetVector(USBUART_EP_1_VECT_NUM,   USBUART_EP_1_ISR);
    CyIntSetPriority(USBUART_EP_1_VECT_NUM, USBUART_EP_1_PRIOR);
    CyIntEnable(USBUART_EP_1_VECT_NUM);
#endif   // End USBUART_EP1_ISR_REMOVE

    /* Set the Data Endpoint 2 Interrupt. */
#if(USBUART_EP2_ISR_REMOVE == 0)
    CyIntSetVector(USBUART_EP_2_VECT_NUM,   USBUART_EP_2_ISR);
    CyIntSetPriority(USBUART_EP_2_VECT_NUM, USBUART_EP_2_PRIOR);
    CyIntEnable(USBUART_EP_2_VECT_NUM);
#endif   // End USBUART_EP2_ISR_REMOVE

    /* Set the Data Endpoint 3 Interrupt. */
#if(USBUART_EP3_ISR_REMOVE == 0)
    CyIntSetVector(USBUART_EP_3_VECT_NUM,   USBUART_EP_3_ISR);
    CyIntSetPriority(USBUART_EP_3_VECT_NUM, USBUART_EP_3_PRIOR);
    CyIntEnable(USBUART_EP_3_VECT_NUM);
#endif   // End USBUART_EP3_ISR_REMOVE

    /* Set the Data Endpoint 4 Interrupt. */
#if(USBUART_EP4_ISR_REMOVE == 0)
    CyIntSetVector(USBUART_EP_4_VECT_NUM,   USBUART_EP_4_ISR);
    CyIntSetPriority(USBUART_EP_4_VECT_NUM, USBUART_EP_4_PRIOR);
    CyIntEnable(USBUART_EP_4_VECT_NUM);
#endif   // End USBUART_EP4_ISR_REMOVE

    /* Set the Data Endpoint 5 Interrupt. */
#if(USBUART_EP5_ISR_REMOVE == 0)
    CyIntSetVector(USBUART_EP_5_VECT_NUM,   USBUART_EP_5_ISR);
    CyIntSetPriority(USBUART_EP_5_VECT_NUM, USBUART_EP_5_PRIOR);
    CyIntEnable(USBUART_EP_5_VECT_NUM);
#endif   // End USBUART_EP5_ISR_REMOVE

    /* Set the Data Endpoint 6 Interrupt. */
#if(USBUART_EP6_ISR_REMOVE == 0)
    CyIntSetVector(USBUART_EP_6_VECT_NUM,   USBUART_EP_6_ISR);
    CyIntSetPriority(USBUART_EP_6_VECT_NUM, USBUART_EP_6_PRIOR);
    CyIntEnable(USBUART_EP_6_VECT_NUM);
#endif   // End USBUART_EP6_ISR_REMOVE

     /* Set the Data Endpoint 7 Interrupt. */
#if(USBUART_EP7_ISR_REMOVE == 0)
    CyIntSetVector(USBUART_EP_7_VECT_NUM,   USBUART_EP_7_ISR);
    CyIntSetPriority(USBUART_EP_7_VECT_NUM, USBUART_EP_7_PRIOR);
    CyIntEnable(USBUART_EP_7_VECT_NUM);
#endif   // End USBUART_EP7_ISR_REMOVE

    /* Set the Data Endpoint 8 Interrupt. */
#if(USBUART_EP8_ISR_REMOVE == 0)
    CyIntSetVector(USBUART_EP_8_VECT_NUM,   USBUART_EP_8_ISR);
    CyIntSetPriority(USBUART_EP_8_VECT_NUM, USBUART_EP_8_PRIOR);
    CyIntEnable(USBUART_EP_8_VECT_NUM);
#endif   // End USBUART_EP8_ISR_REMOVE
    
    USBUART_InitComponent(bDevice, bMode);
}
/*******************************************************************************
* Function Name: USBUART_InitComponent
********************************************************************************
* Summary:
*   Initialize the component, except for the HW which is done one time in
*	the Start function.  This function pulls up D+.
*
* Parameters:  
*   None
*******************************************************************************/
void USBUART_InitComponent(uint8 bDevice, uint8 bMode)
{
    /* USB Locking: Enabled, VRegulator: Disabled */
    CY_SET_REG8(USBUART_CR1, (bMode | 0x02));

    /* Record the descriptor selection */
    USBUART_bDevice = bDevice;

    /* Clear all of the component data */
    USBUART_bConfiguration = 0;
    USBUART_bDeviceAddress  = 0;
    USBUART_bEPHalt = 0;
    USBUART_bDeviceStatus = 0;
    USBUART_bDeviceStatus = 0;
	/*TODO Add hid var*/
    USBUART_bTransferState = USBUART_TRANS_STATE_IDLE;

    /* STALL_IN_OUT */
    CY_SET_REG8(USBUART_EP0_CR, USBUART_MODE_STALL_IN_OUT);
    /* Enable the SIE with an address 0 */
    CY_SET_REG8(USBUART_CR0, USBUART_CR0_ENABLE );
    /* Finally, Enable d+ pullup */
    CY_SET_REG8(USBUART_USBIO_CR1, USBUART_USBIO_CR1_USBPUEN);
}
/*******************************************************************************
* Function Name: USBUART_Stop
********************************************************************************
* Summary:
*   This function shuts down the USB function including to release
*   the D+ Pullup and disabling the SIE.
*
* Parameters:  
*   None
*******************************************************************************/
void USBUART_Stop(void)
{
    /* Disable the SIE with address 0 */
    CY_SET_REG8(USBUART_CR0, 0x00);
    /* Disable the d+ pullup */
    CY_SET_REG8(USBUART_USBIO_CR1, 0x00);
    /* Disable the reset interrupt */
    CyIntDisable(USBUART_BUS_RESET_VECT_NUM);
    CyIntDisable(USBUART_EP_0_VECT_NUM);

    USBUART_bConfiguration = 0;
    USBUART_bDeviceAddress  = 0;
    USBUART_bEPHalt = 0;
    USBUART_bDeviceStatus = 0;
}
/*******************************************************************************
* Function Name: USBUART_bCheckActivity
********************************************************************************
* Summary:
*   Returns the activity status of the bus.  Clears the status hardware to
*   provide fresh activity status on the next call of this routine.
*
* Parameters:  
*   None
*******************************************************************************/
uint8  USBUART_bCheckActivity(void)
{
    uint8 r = ((CY_GET_REG8(USBUART_CR1) >> 2) & USBUART_CR1_BUS_ACTIVITY);

    CY_SET_REG8(USBUART_CR1, (CY_GET_REG8(USBUART_CR1) & USBUART_CR1_BUS_ACTIVITY));

    return r;
}
/*******************************************************************************
* Function Name: USBUART_bGetConfiguration
********************************************************************************
* Summary:
*   Returns the current configuration setting
*
* Parameters:  
*   None
*******************************************************************************/
uint8  USBUART_bGetConfiguration(void)
{
    return USBUART_bConfiguration;
}
/*******************************************************************************
* Function Name: USBUART_bGetInterfaceSetting
********************************************************************************
* Summary:
*   Returns the current interface setting
*
* Parameters:  
*   uint8 ifc, interface number
*
*******************************************************************************/
uint8  USBUART_bGetInterfaceSetting(uint8 ifc)
{
    return USBUART_bInterfaceSetting[ifc];
}
/*******************************************************************************
* Function Name: USBUART_bGetEPState
********************************************************************************
* Summary:
*   Returned the state of the requested endpoint.
*
* Parameters:  
*   bEPNumber: Endpoint Number
*******************************************************************************/
uint8 USBUART_bGetEPState(uint8 bEPNumber)
{
    return USBUART_EP[bEPNumber].bAPIEPState;
}
/*******************************************************************************
* Function Name: USBUART_wGetEPCount
********************************************************************************
* Summary:
*   Returns the transfer count for the requested endpoint.  The value from
*   the count registers includes 2 counts for the two byte checksum of the
*   packet.  This function subtracts the two counts.
*
* Parameters:  
*   bEPNumber: Endpoint Number
*******************************************************************************/
uint16 USBUART_wGetEPCount(uint8 bEPNumber)
{
    uint8 ri = ((bEPNumber - 1) << 4);
    return ( (uint16)(CY_GET_REG8(&USBUART_SIE_EP1_CNT0[ri]) & USBUART_EPX_CNT0_MASK) << 8 | 
                      CY_GET_REG8(&USBUART_SIE_EP1_CNT1[ri]) - 2 );

}
/*******************************************************************************
* Function Name: USBUART_LoadEP
********************************************************************************
* Summary:  Load the endpoint buffer, set up the address pointers and go.
*
* Parameters:  
*   None
*******************************************************************************/
void   USBUART_LoadEP(uint8 bEPNumber, uint8 *pData, uint16 wLength)
{
    uint8 i;
    uint8 ri = ((bEPNumber - 1) << 4);
    uint8 *p = (uint8 *)&USBUART_ARB_RW1_DR[ri];
    
    /* Write WAx */
    CY_SET_REG8(&USBUART_ARB_RW1_WA[ri],     USBUART_EP[bEPNumber].wBuffOffset & 0xFFu);
    CY_SET_REG8(&USBUART_ARB_RW1_WA_MSB[ri], (USBUART_EP[bEPNumber].wBuffOffset >> 8));
    
    /* Copy the data using the arbiter data register */
    for (i = 0; i < wLength; i++)
    {
        CY_SET_REG8(p, *pData++);
    }
    /* Set the count and data toggle */
    CY_SET_REG8(&USBUART_SIE_EP1_CNT0[ri], (wLength >> 8) | (USBUART_EP[bEPNumber].bEPToggle));
    CY_SET_REG8(&USBUART_SIE_EP1_CNT1[ri],  wLength & 0xFFu);
    /* Write the RAx */
    CY_SET_REG8(&USBUART_ARB_RW1_RA[ri],     USBUART_EP[bEPNumber].wBuffOffset & 0xFFu);
    CY_SET_REG8(&USBUART_ARB_RW1_RA_MSB[ri], (USBUART_EP[bEPNumber].wBuffOffset >> 8));

    /* Mark the event pending */
    USBUART_EP[bEPNumber].bAPIEPState = USBUART_NO_EVENT_PENDING;
    /* Write the Mode register */
    CY_SET_REG8(&USBUART_SIE_EP1_CR0[ri], USBUART_EP[bEPNumber].bEPMode);
}

/*******************************************************************************
* Function Name: USBUART_ReadOutEP
********************************************************************************
* Summary:
*   Read data from an endpoint.  The application must call USBUART_bGetEPState
*   to see if an event is pending.
*
* Parameters:  
*   bEPNumber   Endpoint Number
*   pData       Pointer to the destination buffer
*   wLength     Length of the destination buffer
*
* Returns:
*   Number of bytes received
*
*******************************************************************************/
uint16 USBUART_ReadOutEP(uint8 bEPNumber, uint8 *pData, uint16 wLength)
{
    uint8 i;
    uint8 ri = ((bEPNumber - 1) << 4);
    uint8 *p = (uint8 *)&USBUART_ARB_RW1_DR[ri];
/*
	USB_bEP0Count = (CY_GET_REG8(USB_EP0_CNT ) & 0x0F) - 2;
    
    USB_TransferByteCount += USB_bEP0Count;
    
    while ((CurrentTD.wCount > 0) && (USB_bEP0Count > 0))
	{
        *CurrentTD.pData++ = CY_GET_REG8(pReg++);
        USB_bEP0Count--;
		CurrentTD.wCount--;
	}*/
    
    /* Write the RAx */
    CY_SET_REG8(&USBUART_ARB_RW1_RA[ri],     USBUART_EP[bEPNumber].wBuffOffset & 0xFFu);
    CY_SET_REG8(&USBUART_ARB_RW1_RA_MSB[ri], (USBUART_EP[bEPNumber].wBuffOffset >> 8));
    
    /* Copy the data using the arbiter data register */
    for (i = 0; i < wLength; i++)
    {
        *pData++ = CY_GET_REG8(p);
    }
    /* Write the WAx */
    CY_SET_REG8(&USBUART_ARB_RW1_WA[ri],     USBUART_EP[bEPNumber].wBuffOffset & 0xFFu);
    CY_SET_REG8(&USBUART_ARB_RW1_WA_MSB[ri], (USBUART_EP[bEPNumber].wBuffOffset >> 8));

    /* (re)arming of OUT endpoint */
    USBUART_EnableOutEP(bEPNumber);

    return wLength;
}
/*******************************************************************************
* Function Name: USBUART_EnableOutEP
********************************************************************************
* Summary:
*    Enable an OUT endpoint
*
* Parameters:  
*   bEPNumber   Endpoint Number
*
*******************************************************************************/
void USBUART_EnableOutEP(uint8 bEPNumber)
{
    uint8 ri = ((bEPNumber - 1) << 4);
    /* Mark the event pending */
    USBUART_EP[bEPNumber].bAPIEPState = USBUART_NO_EVENT_PENDING;
    /* Write the Mode register */
    CY_SET_REG8(&USBUART_SIE_EP1_CR0[ri], USBUART_EP[bEPNumber].bEPMode);
}
/*******************************************************************************
* Function Name: USBUART_DisableOutEP
********************************************************************************
* Summary:
*
* Parameters:  
*   bEPNumber   Endpoint Number
*
*******************************************************************************/
void USBUART_DisableOutEP(uint8 bEPNumber)
{
    uint8 ri = ((bEPNumber - 1) << 4);

	/* Write the Mode register */
    CY_SET_REG8(&USBUART_SIE_EP1_CR0[ri], USBUART_EP[bEPNumber].bEPMode);
}
/*******************************************************************************
* Function Name: USBUART_Force
********************************************************************************
* Summary:  Forces the bus state
*
* Parameters:  
*   bState 
*    USBUART_FORCE_J 
*    USBUART_FORCE_K 
*    USBUART_FORCE_SE0 
*    USBUART_FORCE_NONE
*
*******************************************************************************/
void USBUART_Force(uint8 bState)
{
    CY_SET_REG8(USBUART_USBIO_CR0, bState);
}
/*******************************************************************************
* Function Name: USBUART_bGetEPAckState
********************************************************************************
* Summary:  Returns the ACK of the CR0 Register (ACKD)
*
* Parameters:  
*   bEPNumber   Endpoint Number
*
* Returns
*   0 if nothing has been ACKD, non-=zero something has been ACKD
*******************************************************************************/
uint8 USBUART_bGetEPAckState(uint8 bEPNumber)
{
    uint8 ri = ((bEPNumber - 1) << 4);
  
    return (CY_GET_REG8(&USBUART_SIE_EP1_CR0[ri]) & USBUART_MODE_ACKD );
}
/*******************************************************************************
* Function Name: USBUART_SetPowerStatus
********************************************************************************
* Summary:
*   Sets the device power status for reporting in the Get Device Status
*   request
*
* Parameters:  
*   bPowerStaus 0 = Bus Powered, 1 = Self Powered
*
*******************************************************************************/
void USBUART_SetPowerStatus(uint8 bPowerStaus)
{
    if (bPowerStaus)
    {
        USBUART_bDeviceStatus |=  USBUART_DEVICE_STATUS_SELF_POWERED;
    }
    else
    {
        USBUART_bDeviceStatus &=  ~USBUART_DEVICE_STATUS_SELF_POWERED;
    }
}

/*******************************************************************************
* Function Name: USBUART_bVBusPresent
********************************************************************************
* Summary:
*   Determines VBUS presense for Self Powered Devices.  Returns 1 if VBUS
*   is present, otherwise 0.
*
* Parameters:  
*   None
*******************************************************************************/
#if (USBUART_MON_VBUS == 1)
uint8 USBUART_bVBusPresent()
{
    return ((CY_GET_REG8(USBUART_VBUS_PS) & USBUART_VBUS_MASK) ? 1 : 0);
}
#endif

