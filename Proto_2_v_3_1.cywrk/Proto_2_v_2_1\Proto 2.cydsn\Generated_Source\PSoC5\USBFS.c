/*******************************************************************************
* File Name: USBFS.c
* Version 1.50
*
* Description:
*  API for USBFS Component.
*
* Note:
*  Many of the functions use endpoint number.  RAM arrays are sized with 9
*  elements so they are indexed directly by epNumber.  The SIE and ARB
*  registers are indexed by variations of epNumber - 1.
*
********************************************************************************
* Copyright 2008-2010, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
*******************************************************************************/

#include "CyLib.h"
#include "USBFS.h"


/***************************************
* External data references
***************************************/

extern volatile uint8 USBFS_configuration;
extern volatile uint8 USBFS_interfaceSetting[];
extern volatile uint8 USBFS_deviceAddress;
extern volatile uint8 USBFS_deviceStatus;
extern volatile uint8 USBFS_device;
extern volatile uint8 USBFS_transferState;
extern volatile uint8 USBFS_lastPacketSize;

extern T_USBFS_EP_CTL_BLOCK USBFS_EP[];


/***************************************
* External function references
***************************************/

CY_ISR_PROTO(USBFS_EP_0_ISR);
#if(USBFS_EP1_ISR_REMOVE == 0u)
    CY_ISR_PROTO(USBFS_EP_1_ISR);
#endif /* End USBFS_EP1_ISR_REMOVE */
#if(USBFS_EP2_ISR_REMOVE == 0u)
    CY_ISR_PROTO(USBFS_EP_2_ISR);
#endif /* End USBFS_EP2_ISR_REMOVE */
#if(USBFS_EP3_ISR_REMOVE == 0u)
    CY_ISR_PROTO(USBFS_EP_3_ISR);
#endif /* End USBFS_EP3_ISR_REMOVE */
#if(USBFS_EP4_ISR_REMOVE == 0u)
    CY_ISR_PROTO(USBFS_EP_4_ISR);
#endif /* End USBFS_EP4_ISR_REMOVE */
#if(USBFS_EP5_ISR_REMOVE == 0u)
    CY_ISR_PROTO(USBFS_EP_5_ISR);
#endif /* End USBFS_EP5_ISR_REMOVE */
#if(USBFS_EP6_ISR_REMOVE == 0u)
    CY_ISR_PROTO(USBFS_EP_6_ISR);
#endif /* End USBFS_EP6_ISR_REMOVE */
#if(USBFS_EP7_ISR_REMOVE == 0u)
    CY_ISR_PROTO(USBFS_EP_7_ISR);
#endif /* End USBFS_EP7_ISR_REMOVE */
#if(USBFS_EP8_ISR_REMOVE == 0u)
    CY_ISR_PROTO(USBFS_EP_8_ISR);
#endif /* End USBFS_EP8_ISR_REMOVE */
CY_ISR_PROTO(USBFS_BUS_RESET_ISR);
CY_ISR_PROTO(USBFS_SOF_ISR);

uint8 USBFS_initVar = 0u;


/*******************************************************************************
* Function Name: USBFS_Start
********************************************************************************
*
* Summary:
*  This function initialize the USB SIE, arbiter and the
*  endpoint APIs, including setting the D+ Pullup
*
* Parameters:
*  device: Contains the device number from the desired device descriptor set 
*          entered with the USBFS customizer.
*  mode: The operating voltage. This determines whether the voltage regulator 
*        is enabled for 5V operation or if pass through mode is used for 3.3V 
*        operation. Symbolic names and their associated values are given in the 
*        following table.
*       USBFS_3V_OPERATION - Disable voltage regulator and pass-thru 
*                                       Vcc for pull-up
*       USBFS_5V_OPERATION - Enable voltage regulator and use 
*                                       regulator for pull-up
*       USBFS_DWR_VDDD_OPERATION - Enable or Disable voltage 
*                         regulator depend on Vddd Voltage configuration in DWR.
*
* Return:
*   None.
*
* Global variables:
*  The USBFS_intiVar variable is used to indicate initial 
*  configuration of this component. The variable is initialized to zero (0u) 
*  and set to one (1u) the first time USBFS_Start() is called. 
*  This allows for component Re-Start without unnecessary re-initialization 
*  in all subsequent calls to the USBFS_Start() routine. 
*  If re-initialization of the component is required the variable should be set 
*  to zero before call of UART_Start() routine, or the user may call 
*  USBFS_Init() and USBFS_InitComponent() as done 
*  in the USBFS_Start() routine.
*
* Side Effects:
*   This function will reset all communication states to default.
*
* Reentrant:
*  No.
*
*******************************************************************************/
void USBFS_Start(uint8 device, uint8 mode)
{
    /* If not Initialized then initialize all required hardware and software */
    if(USBFS_initVar == 0u)
    {
        USBFS_Init();
        USBFS_initVar = 1u;
    }
    USBFS_InitComponent(device, mode);
}


/*******************************************************************************
* Function Name: USBFS_Init
********************************************************************************
*
* Summary:
*  Initialize component's hardware. Usually called in USBFS_Start().
*
* Parameters:
*  None.
*
* Return:
*  None.
*
* Reentrant:
*  No.
*
*******************************************************************************/
void USBFS_Init(void)
{
    uint16 i;
    uint8 *p;
    uint8 enableInterrupts;
    enableInterrupts = CyEnterCriticalSection();

    p = (uint8 *)&USBFS_ARB_RW1_DR_PTR[0u];
    
    /* Enable USB block  */
    USBFS_PM_ACT_CFG_REG |= USBFS_PM_ACT_EN_FSUSB;
    
    #if(USBFS_PSOC3_ES2 || USBFS_PSOC5_ES1)
        /* Disable USBIO for TO3 */
        USBFS_PM_AVAIL_CR_REG &= ~USBFS_PM_AVAIL_EN_FSUSBIO;
    #endif /* End USBFS_PSOC3_ES2 || USBFS_PSOC5_ES1 */

    /* Enable core clock */
    USBFS_USB_CLK_EN_REG = USBFS_USB_CLK_ENABLE;

    /* Bus Reset Length */
    USBFS_BUS_RST_CNT_REG = USBFS_BUS_RST_COUNT;
    
    #if(USBFS_PSOC3_ES2 || USBFS_PSOC5_ES1)
        /* Disable USBIO for TO3 */
        USBFS_PM_AVAIL_CR_REG |= USBFS_PM_AVAIL_EN_FSUSBIO;
    #endif /* End USBFS_PSOC3_ES2 || USBFS_PSOC5_ES1 */

    /* ENABLING USBIO PADS IN USB MODE FROM I/O MODE */
    #if(USBFS_PSOC3_ES3 || USBFS_PSOC5_ES2)
        /* Ensure USB transmit enable is low (USB_USBIO_CR0.ten). - Manual Transmission - Disabled */
        USBFS_USBIO_CR0_REG &= ~USBFS_USBIO_CR0_TEN;
        CyDelayUs(0);  /*~50ns delay */
        /* Disable the USBIO by asserting PM.USB_CR0.fsusbio_pd_n(Inverted) high. This will
           have been set low by the power manger out of reset. 
           Also confirm USBIO pull-up disabled*/
        USBFS_PM_USB_CR0_REG &= ~(USBFS_PM_USB_CR0_PD_N |USBFS_PM_USB_CR0_PD_PULLUP_N);
        /* Enable the USBIO reference by setting PM.USB_CR0.fsusbio_ref_en.*/
        USBFS_PM_USB_CR0_REG |= USBFS_PM_USB_CR0_REF_EN;
        /* The reference will be available 1 us after the regulator is enabled */
        CyDelayUs(1); 
        /* OR 40us after power restored */
        CyDelayUs(40); 
        /* Ensure the single ended disable bits are low (PRT15.INP_DIS[7:6])(input receiver enabled). */
        USBFS_DM_INP_DIS_REG &= ~USBFS_DM_MASK;
        USBFS_DP_INP_DIS_REG &= ~USBFS_DP_MASK;
        
        /* Enable USBIO */
        USBFS_PM_USB_CR0_REG |= USBFS_PM_USB_CR0_PD_N;
        CyDelayUs(2); 
        /* Set the USBIO pull-up enable */
        USBFS_PM_USB_CR0_REG |= USBFS_PM_USB_CR0_PD_PULLUP_N;
    
    #endif /* End USBFS_PSOC3_ES3 || USBFS_PSOC5_ES2 */

    /* If VBUS Monitoring is enable, setup the DR in the port reg */
    #if (USBFS_MON_VBUS == 1u)
        USBFS_VBUS_DR_REG &= ~USBFS_VBUS_MASK;
    #endif /* USBFS_MON_VBUS */

    /* Write WAx */
    CY_SET_REG8(&USBFS_ARB_RW1_WA_PTR[0u],     0u);
    CY_SET_REG8(&USBFS_ARB_RW1_WA_MSB_PTR[0u], 0u);

    /* Copy the data using the arbiter data register */
    for (i = 0u; i < USBFS_EPX_DATA_BUF_MAX; i++)
    {
        CY_SET_REG8(p, 0xFFu);
    }

    CyExitCriticalSection(enableInterrupts);

    /* Set the bus reset Interrupt. */
    CyIntSetVector(USBFS_BUS_RESET_VECT_NUM,   USBFS_BUS_RESET_ISR);
    CyIntSetPriority(USBFS_BUS_RESET_VECT_NUM, USBFS_BUS_RESET_PRIOR);

    /* Set the SOF Interrupt. */
    CyIntSetVector(USBFS_SOF_VECT_NUM,   USBFS_SOF_ISR);
    CyIntSetPriority(USBFS_SOF_VECT_NUM, USBFS_SOF_PRIOR);

    /* Set the Control Endpoint Interrupt. */
    CyIntSetVector(USBFS_EP_0_VECT_NUM,   USBFS_EP_0_ISR);
    CyIntSetPriority(USBFS_EP_0_VECT_NUM, USBFS_EP_0_PRIOR);

    /* Set the Data Endpoint 1 Interrupt. */
    #if(USBFS_EP1_ISR_REMOVE == 0u)
        CyIntSetVector(USBFS_EP_1_VECT_NUM,   USBFS_EP_1_ISR);
        CyIntSetPriority(USBFS_EP_1_VECT_NUM, USBFS_EP_1_PRIOR);
    #endif   /* End USBFS_EP1_ISR_REMOVE */

    /* Set the Data Endpoint 2 Interrupt. */
    #if(USBFS_EP2_ISR_REMOVE == 0u)
        CyIntSetVector(USBFS_EP_2_VECT_NUM,   USBFS_EP_2_ISR);
        CyIntSetPriority(USBFS_EP_2_VECT_NUM, USBFS_EP_2_PRIOR);
    #endif   /* End USBFS_EP2_ISR_REMOVE */

    /* Set the Data Endpoint 3 Interrupt. */
    #if(USBFS_EP3_ISR_REMOVE == 0u)
        CyIntSetVector(USBFS_EP_3_VECT_NUM,   USBFS_EP_3_ISR);
        CyIntSetPriority(USBFS_EP_3_VECT_NUM, USBFS_EP_3_PRIOR);
    #endif   /* End USBFS_EP3_ISR_REMOVE */

    /* Set the Data Endpoint 4 Interrupt. */
    #if(USBFS_EP4_ISR_REMOVE == 0u)
        CyIntSetVector(USBFS_EP_4_VECT_NUM,   USBFS_EP_4_ISR);
        CyIntSetPriority(USBFS_EP_4_VECT_NUM, USBFS_EP_4_PRIOR);
    #endif   /* End USBFS_EP4_ISR_REMOVE */

    /* Set the Data Endpoint 5 Interrupt. */
    #if(USBFS_EP5_ISR_REMOVE == 0u)
        CyIntSetVector(USBFS_EP_5_VECT_NUM,   USBFS_EP_5_ISR);
        CyIntSetPriority(USBFS_EP_5_VECT_NUM, USBFS_EP_5_PRIOR);
    #endif   /* End USBFS_EP5_ISR_REMOVE */

    /* Set the Data Endpoint 6 Interrupt. */
    #if(USBFS_EP6_ISR_REMOVE == 0u)
        CyIntSetVector(USBFS_EP_6_VECT_NUM,   USBFS_EP_6_ISR);
        CyIntSetPriority(USBFS_EP_6_VECT_NUM, USBFS_EP_6_PRIOR);
    #endif   /* End USBFS_EP6_ISR_REMOVE */

     /* Set the Data Endpoint 7 Interrupt. */
    #if(USBFS_EP7_ISR_REMOVE == 0u)
        CyIntSetVector(USBFS_EP_7_VECT_NUM,   USBFS_EP_7_ISR);
        CyIntSetPriority(USBFS_EP_7_VECT_NUM, USBFS_EP_7_PRIOR);
    #endif   /* End USBFS_EP7_ISR_REMOVE */

    /* Set the Data Endpoint 8 Interrupt. */
    #if(USBFS_EP8_ISR_REMOVE == 0u)
        CyIntSetVector(USBFS_EP_8_VECT_NUM,   USBFS_EP_8_ISR);
        CyIntSetPriority(USBFS_EP_8_VECT_NUM, USBFS_EP_8_PRIOR);
    #endif   /* End USBFS_EP8_ISR_REMOVE */
}


/*******************************************************************************
* Function Name: USBFS_InitComponent
********************************************************************************
*
* Summary:
*  Initialize the component, except for the HW which is done one time in
*  the Start function.  This function pulls up D+.
*
* Parameters:
*  device: Contains the device number from the desired device descriptor set 
*          entered with the USBFS customizer.
*  mode: The operating voltage. This determines whether the voltage regulator 
*        is enabled for 5V operation or if pass through mode is used for 3.3V 
*        operation. Symbolic names and their associated values are given in the 
*        following table.
*       USBFS_3V_OPERATION - Disable voltage regulator and pass-thru 
*                                       Vcc for pull-up
*       USBFS_5V_OPERATION - Enable voltage regulator and use 
*                                       regulator for pull-up
*       USBFS_DWR_VDDD_OPERATION - Enable or Disable voltage 
*                         regulator depend on Vddd Voltage configuration in DWR.
*
* Return:
*   None.
*
* Global variables:
*   USBFS_device: Contains the started device number from the 
*       desired device descriptor set entered with the USBFS customizer.
*   USBFS_transferState: This variable used by the communication 
*       functions to handle current transfer state. Initialized to 
*       TRANS_STATE_IDLE in this API. 
*   USBFS_configuration: Contains current configuration number 
*       which is set by the Host using SET_CONFIGURATION request. 
*       Initialized to zero in this API.
*   USBFS_deviceAddress: Contains current device address. This 
*       variable is initialized to zero in this API. Host starts to communicate 
*      to device with address 0 and then set it to whatever value using 
*      SET_ADDRESS request.  
*   USBFS_deviceStatus: initialized to 0.
*       This is two bit variable which contain power status in first bit 
*       (DEVICE_STATUS_BUS_POWERED or DEVICE_STATUS_SELF_POWERED) and remote 
*       wakeup status (DEVICE_STATUS_REMOTE_WAKEUP) in second bit. 
*   USBFS_lastPacketSize initialized to 0;
*
* Reentrant:
*  No.
*
*******************************************************************************/
void USBFS_InitComponent(uint8 device, uint8 mode)
{
    /* Enable Interrupts. */
    CyIntEnable(USBFS_BUS_RESET_VECT_NUM);
    CyIntEnable(USBFS_EP_0_VECT_NUM);
    #if(USBFS_EP1_ISR_REMOVE == 0u)
        CyIntEnable(USBFS_EP_1_VECT_NUM);
    #endif   /* End USBFS_EP1_ISR_REMOVE */
    #if(USBFS_EP2_ISR_REMOVE == 0u)
        CyIntEnable(USBFS_EP_2_VECT_NUM);
    #endif   /* End USBFS_EP2_ISR_REMOVE */
    #if(USBFS_EP3_ISR_REMOVE == 0u)
        CyIntEnable(USBFS_EP_3_VECT_NUM);
    #endif   /* End USBFS_EP3_ISR_REMOVE */
    #if(USBFS_EP4_ISR_REMOVE == 0u)
        CyIntEnable(USBFS_EP_4_VECT_NUM);
    #endif   /* End USBFS_EP4_ISR_REMOVE */
    #if(USBFS_EP5_ISR_REMOVE == 0u)
        CyIntEnable(USBFS_EP_5_VECT_NUM);
    #endif   /* End USBFS_EP5_ISR_REMOVE */
    #if(USBFS_EP6_ISR_REMOVE == 0u)
        CyIntEnable(USBFS_EP_6_VECT_NUM);
    #endif   /* End USBFS_EP6_ISR_REMOVE */
    #if(USBFS_EP7_ISR_REMOVE == 0u)
        CyIntEnable(USBFS_EP_7_VECT_NUM);
    #endif   /* End USBFS_EP7_ISR_REMOVE */
    #if(USBFS_EP8_ISR_REMOVE == 0u)
        CyIntEnable(USBFS_EP_8_VECT_NUM);
    #endif   /* End USBFS_EP8_ISR_REMOVE */

    USBFS_transferState = USBFS_TRANS_STATE_IDLE;

    /* USB Locking: Enabled, VRegulator: depend on mode or DWR Voltage configuration*/
    switch(mode)
    {
        case USBFS_3V_OPERATION:
            USBFS_CR1_REG = USBFS_CR1_ENABLE_LOCK;
            break;
        case USBFS_5V_OPERATION:
            USBFS_CR1_REG = USBFS_CR1_ENABLE_LOCK | USBFS_CR1_REG_ENABLE;
            break;
        default:   /*USBFS_DWR_VDDD_OPERATION */    
            if(USBFS_VDDD_MV < USBFS_3500MV)
            {        
                USBFS_CR1_REG = USBFS_CR1_ENABLE_LOCK;
            }
            else
            {
                USBFS_CR1_REG = USBFS_CR1_ENABLE_LOCK | USBFS_CR1_REG_ENABLE;
            }
            break;
    }

    /* Record the descriptor selection */
    USBFS_device = device;

    /* Clear all of the component data */
    USBFS_configuration = 0u;
    USBFS_deviceAddress  = 0u;
    USBFS_deviceStatus = 0u;

    USBFS_lastPacketSize = 0u;

    /*  ACK Setup, Stall IN/OUT */
    CY_SET_REG8(USBFS_EP0_CR_PTR, USBFS_MODE_STALL_IN_OUT);

    /* Enable the SIE with an address 0 */
    CY_SET_REG8(USBFS_CR0_PTR, USBFS_CR0_ENABLE);

    /* Finally, Enable d+ pullup and select iomode to USB mode*/
    CY_SET_REG8(USBFS_USBIO_CR1_PTR, USBFS_USBIO_CR1_USBPUEN);
}


/*******************************************************************************
* Function Name: USBFS_ReInitComponent
********************************************************************************
*
* Summary:
*  This function reinitialize the component configuration and is 
*  intend to be called from the Reset interrupt.
*
* Parameters:
*  None.
*
* Return:
*   None.
*
* Global variables:
*   USBFS_device: Contains the started device number from the 
*       desired device descriptor set entered with the USBFS customizer.
*   USBFS_transferState: This variable used by the communication 
*       functions to handle current transfer state. Initialized to 
*       TRANS_STATE_IDLE in this API. 
*   USBFS_configuration: Contains current configuration number 
*       which is set by the Host using SET_CONFIGURATION request. 
*       Initialized to zero in this API.
*   USBFS_deviceAddress: Contains current device address. This 
*       variable is initialized to zero in this API. Host starts to communicate 
*      to device with address 0 and then set it to whatever value using 
*      SET_ADDRESS request.  
*   USBFS_deviceStatus: initialized to 0.
*       This is two bit variable which contain power status in first bit 
*       (DEVICE_STATUS_BUS_POWERED or DEVICE_STATUS_SELF_POWERED) and remote 
*       wakeup status (DEVICE_STATUS_REMOTE_WAKEUP) in second bit. 
*   USBFS_lastPacketSize initialized to 0;
*
* Reentrant:
*  No.
*
*******************************************************************************/
void USBFS_ReInitComponent()
{
    USBFS_transferState = USBFS_TRANS_STATE_IDLE;

    /* Clear all of the component data */
    USBFS_configuration = 0u;
    USBFS_deviceAddress  = 0u;
    USBFS_deviceStatus = 0u;

    USBFS_lastPacketSize = 0u;

    /*  ACK Setup, Stall IN/OUT */
    CY_SET_REG8(USBFS_EP0_CR_PTR, USBFS_MODE_STALL_IN_OUT);

    /* Enable the SIE with an address 0 */
    CY_SET_REG8(USBFS_CR0_PTR, USBFS_CR0_ENABLE);

}


/*******************************************************************************
* Function Name: USBFS_Stop
********************************************************************************
*
* Summary:
*  This function shuts down the USB function including to release
*  the D+ Pullup and disabling the SIE.
*
* Parameters:
*  None.
*
* Return:
*  None.
*
*******************************************************************************/
void USBFS_Stop(void) 
{
    /* Disable the SIE */
    USBFS_CR0_REG &= ~USBFS_CR0_ENABLE;
    /* Disable the d+ pullup */
    USBFS_USBIO_CR1_REG &= ~USBFS_USBIO_CR1_USBPUEN;
    
    /* Disable the reset and EP interrupts */
    CyIntDisable(USBFS_BUS_RESET_VECT_NUM);
    CyIntDisable(USBFS_EP_0_VECT_NUM);
    #if(USBFS_EP1_ISR_REMOVE == 0u)
        CyIntDisable(USBFS_EP_1_VECT_NUM);
    #endif   /* End USBFS_EP1_ISR_REMOVE */
    #if(USBFS_EP2_ISR_REMOVE == 0u)
        CyIntDisable(USBFS_EP_2_VECT_NUM);
    #endif   /* End USBFS_EP2_ISR_REMOVE */
    #if(USBFS_EP3_ISR_REMOVE == 0u)
        CyIntDisable(USBFS_EP_3_VECT_NUM);
    #endif   /* End USBFS_EP3_ISR_REMOVE */
    #if(USBFS_EP4_ISR_REMOVE == 0u)
        CyIntDisable(USBFS_EP_4_VECT_NUM);
    #endif   /* End USBFS_EP4_ISR_REMOVE */
    #if(USBFS_EP5_ISR_REMOVE == 0u)
        CyIntDisable(USBFS_EP_5_VECT_NUM);
    #endif   /* End USBFS_EP5_ISR_REMOVE */
    #if(USBFS_EP6_ISR_REMOVE == 0u)
        CyIntDisable(USBFS_EP_6_VECT_NUM);
    #endif   /* End USBFS_EP6_ISR_REMOVE */
    #if(USBFS_EP7_ISR_REMOVE == 0u)
        CyIntDisable(USBFS_EP_7_VECT_NUM);
    #endif   /* End USBFS_EP7_ISR_REMOVE */
    #if(USBFS_EP8_ISR_REMOVE == 0u)
        CyIntDisable(USBFS_EP_8_VECT_NUM);
    #endif   /* End USBFS_EP8_ISR_REMOVE */
}


/*******************************************************************************
* Function Name: USBFS_CheckActivity
********************************************************************************
*
* Summary:
*  Returns the activity status of the bus.  Clears the status hardware to
*  provide fresh activity status on the next call of this routine.
*
* Parameters:
*  None.
*
* Return:
*  1 - If bus activity was detected since the last call to this function
*  0 - If bus activity not was detected since the last call to this function
*
*******************************************************************************/
uint8 USBFS_CheckActivity(void) 
{
    uint8 r;
    
    r = CY_GET_REG8(USBFS_CR1_PTR);
    CY_SET_REG8(USBFS_CR1_PTR, (r & ~USBFS_CR1_BUS_ACTIVITY));

    return((r & USBFS_CR1_BUS_ACTIVITY) >> USBFS_CR1_BUS_ACTIVITY_SHIFT);
}


/*******************************************************************************
* Function Name: USBFS_GetConfiguration
********************************************************************************
*
* Summary:
*  Returns the current configuration setting
*
* Parameters:
*  None.
*
* Return:
*  configuration.
*
*******************************************************************************/
uint8 USBFS_GetConfiguration(void) 
{
    return(USBFS_configuration);
}


/*******************************************************************************
* Function Name: USBFS_GetInterfaceSetting
********************************************************************************
*
* Summary:
*  Returns the alternate setting from current interface
*
* Parameters:
*  uint8 interfaceNumber, interface number
*
* Return:
*  Alternate setting.
*
*******************************************************************************/
uint8  USBFS_GetInterfaceSetting(uint8 interfaceNumber) \
                                                    
{
    return(USBFS_interfaceSetting[interfaceNumber]);
}


/*******************************************************************************
* Function Name: USBFS_GetEPState
********************************************************************************
*
* Summary:
*  Returned the state of the requested endpoint.
*
* Parameters:
*  epNumber: Endpoint Number
*
* Return:
*  State of the requested endpoint.
*
*******************************************************************************/
uint8 USBFS_GetEPState(uint8 epNumber) 
{
    return(USBFS_EP[epNumber].apiEpState);
}


/*******************************************************************************
* Function Name: USBFS_GetEPCount
********************************************************************************
*
* Summary:
*  This function supports Data Endpoints only(EP1-EP8).
*  Returns the transfer count for the requested endpoint.  The value from
*  the count registers includes 2 counts for the two byte checksum of the
*  packet.  This function subtracts the two counts. 
*
* Parameters:
*  epNumber: Data Endpoint Number.
*            Valid values are between 1 and 8.
*
* Return:
*  Returns the current byte count from the specified endpoin or 0 for an 
*  invalid endpoint.
*
*******************************************************************************/
uint16 USBFS_GetEPCount(uint8 epNumber) 
{
    uint8 ri;
    uint16 count = 0u;
    
    if((epNumber > USBFS_EP0) && (epNumber < USBFS_MAX_EP))
    {
        ri = ((epNumber - USBFS_EP1) << USBFS_EPX_CNTX_ADDR_SHIFT);
    
        count = (uint16)((CY_GET_REG8(&USBFS_SIE_EP1_CNT0_PTR[ri]) & USBFS_EPX_CNT0_MASK) << 8u) \
                        | CY_GET_REG8(&USBFS_SIE_EP1_CNT1_PTR[ri]);
        count -= USBFS_EPX_CNTX_CRC_COUNT;
    }
    return(count); 
}


/*******************************************************************************
* Function Name: USBFS_LoadInEP
********************************************************************************
*
* Summary:
*  Loads and enables the specified USB data endpoint for an IN interrupt or bulk 
*  transfer.
*
* Parameters:
*  epNumber: Contains the data endpoint number.
*            Valid values are between 1 and 8.
*  *pData: A pointer to a data array from which the data for the endpoint space 
*          is loaded.
*  length: The number of bytes to transfer from the array and then send as a 
*          result of an IN request. Valid values are between 0 and 512.
*
* Return:
*  None.
*
* Reentrant:
*  No.
*
*******************************************************************************/
void USBFS_LoadInEP(uint8 epNumber, uint8 *pData, uint16 length)
{
    uint16 i;
    uint8 ri;
    uint8 *p;

    if((epNumber > USBFS_EP0) && (epNumber < USBFS_MAX_EP) && (pData != USBFS_NULL))
    {
        ri = ((epNumber - USBFS_EP1) << USBFS_EPX_CNTX_ADDR_SHIFT);
        p = (uint8 *)&USBFS_ARB_RW1_DR_PTR[ri];
        /* Write WAx */
        CY_SET_REG8(&USBFS_ARB_RW1_WA_PTR[ri],     USBFS_EP[epNumber].buffOffset & 0xFFu);
        CY_SET_REG8(&USBFS_ARB_RW1_WA_MSB_PTR[ri], (USBFS_EP[epNumber].buffOffset >> 8u));
        
        /* Limits length to available buffer space */
        if(length > USBFS_EPX_DATA_BUF_MAX - USBFS_EP[epNumber].buffOffset)
        {
            length = USBFS_EPX_DATA_BUF_MAX - USBFS_EP[epNumber].buffOffset;
        }
        /* Copy the data using the arbiter data register */
        for (i = 0u; i < length; i++)
        {
            CY_SET_REG8(p, pData[i]);
        }
        /* Set the count and data toggle */
        CY_SET_REG8(&USBFS_SIE_EP1_CNT0_PTR[ri], (length >> 8u) | (USBFS_EP[epNumber].epToggle));
        CY_SET_REG8(&USBFS_SIE_EP1_CNT1_PTR[ri],  length & 0xFFu);
        /* Write the RAx */
        CY_SET_REG8(&USBFS_ARB_RW1_RA_PTR[ri],     USBFS_EP[epNumber].buffOffset & 0xFFu);
        CY_SET_REG8(&USBFS_ARB_RW1_RA_MSB_PTR[ri], (USBFS_EP[epNumber].buffOffset >> 8u));
    
        USBFS_EP[epNumber].apiEpState = USBFS_NO_EVENT_PENDING;
        /* Write the Mode register */
        CY_SET_REG8(&USBFS_SIE_EP1_CR0_PTR[ri], USBFS_EP[epNumber].epMode);
    }
}


/*******************************************************************************
* Function Name: USBFS_ReadOutEP
********************************************************************************
*
* Summary:
*  Read data from an endpoint.  The application must call 
*  USBFS_GetEPState to see if an event is pending.
*
* Parameters:
*  epNumber: Contains the data endpoint number. 
*            Valid values are between 1 and 8.
*  pData: A pointer to a data array from which the data for the endpoint space 
*         is loaded.
*  length: The number of bytes to transfer from the USB Out enpoint and loads 
*          it into data array. Valid values are between 0 and 256. The function
*          moves fewer than the requested number of bytes if the host sends 
*          fewer bytes than requested.
*
* Returns:
*  Number of bytes received, 0 for an invalid endpoint.
*
* Reentrant:
*  No.
*
*******************************************************************************/
uint16 USBFS_ReadOutEP(uint8 epNumber, uint8 *pData, uint16 length)
{
    uint16 i, xferCount;
    uint8 ri;
    uint8 *p;

    if((epNumber > USBFS_EP0) && (epNumber < USBFS_MAX_EP) && (pData != USBFS_NULL))
    {
        ri = ((epNumber - USBFS_EP1) << USBFS_EPX_CNTX_ADDR_SHIFT);
        p = (uint8 *)&USBFS_ARB_RW1_DR_PTR[ri];
        /* Write the RAx */
        CY_SET_REG8(&USBFS_ARB_RW1_RA_PTR[ri],     USBFS_EP[epNumber].buffOffset & 0xFFu);
        CY_SET_REG8(&USBFS_ARB_RW1_RA_MSB_PTR[ri], (USBFS_EP[epNumber].buffOffset >> 8u));

        /* Determine which is smaller the requested data or the available data */
        xferCount = USBFS_GetEPCount(epNumber);
        if (length > xferCount)
        {
            length = xferCount;
        }
        /* Copy the data using the arbiter data register */
        for (i = 0u; i < length; i++)
        {
            pData[i] = CY_GET_REG8(p);
        }
        /* Write the WAx */
        CY_SET_REG8(&USBFS_ARB_RW1_WA_PTR[ri],     USBFS_EP[epNumber].buffOffset & 0xFFu);
        CY_SET_REG8(&USBFS_ARB_RW1_WA_MSB_PTR[ri], (USBFS_EP[epNumber].buffOffset >> 8u));
    
        /* (re)arming of OUT endpoint */
        USBFS_EnableOutEP(epNumber);
    }
    else
    {
        length = 0;
    }

    return(length);
}


/*******************************************************************************
* Function Name: USBFS_EnableOutEP
********************************************************************************
*
* Summary:
*  This function enables an OUT endpoint.  It should not be
*  called for an IN endpoint.
*
* Parameters:
*  epNumber: Endpoint Number
*            Valid values are between 1 and 8.
*
* Return:
*   None.
*
* Global variables:
*  USBFS_EP[epNumber].apiEpState - set to NO_EVENT_PENDING
*
* Reentrant:
*  No.
*
*******************************************************************************/
void USBFS_EnableOutEP(uint8 epNumber)
{
    uint8 ri;

    if((epNumber > USBFS_EP0) && (epNumber < USBFS_MAX_EP))
    {
        ri = ((epNumber - USBFS_EP1) << USBFS_EPX_CNTX_ADDR_SHIFT);
        USBFS_EP[epNumber].apiEpState = USBFS_NO_EVENT_PENDING;
        /* Write the Mode register */
        CY_SET_REG8(&USBFS_SIE_EP1_CR0_PTR[ri], USBFS_EP[epNumber].epMode);
    }
}


/*******************************************************************************
* Function Name: USBFS_DisableOutEP
********************************************************************************
*
* Summary: 
*  This function disables an OUT endpoint.  It should not be
*  called for an IN endpoint.
*
* Parameters:
*  epNumber: Endpoint Number
*            Valid values are between 1 and 8.
*
* Return:
*  None.
*
*******************************************************************************/
void USBFS_DisableOutEP(uint8 epNumber) 
{
    uint8 ri ;
    
    if((epNumber > USBFS_EP0) && (epNumber < USBFS_MAX_EP))
    {
        ri = ((epNumber - USBFS_EP1) << USBFS_EPX_CNTX_ADDR_SHIFT);
        /* Write the Mode register */
        CY_SET_REG8(&USBFS_SIE_EP1_CR0_PTR[ri], USBFS_MODE_NAK_OUT);
    }    
}


/*******************************************************************************
* Function Name: USBFS_Force
********************************************************************************
*
* Summary:
*  Forces the bus state
*
* Parameters:
*  bState
*    USBFS_FORCE_J
*    USBFS_FORCE_K
*    USBFS_FORCE_SE0
*    USBFS_FORCE_NONE
*
* Return:
*  None.
*
*******************************************************************************/
void USBFS_Force(uint8 bState) 
{
    CY_SET_REG8(USBFS_USBIO_CR0_PTR, bState);
}


/*******************************************************************************
* Function Name: USBFS_GetEPAckState
********************************************************************************
*
* Summary:
*  Returns the ACK of the CR0 Register (ACKD)
*
* Parameters:
*  epNumber: Endpoint Number
*            Valid values are between 1 and 8.
*
* Returns
*  0 if nothing has been ACKD, non-=zero something has been ACKD
*
*******************************************************************************/
uint8 USBFS_GetEPAckState(uint8 epNumber) 
{
    uint8 ri;
    uint8 cr = 0;
    
    if((epNumber > USBFS_EP0) && (epNumber < USBFS_MAX_EP))
    {
        ri = ((epNumber - USBFS_EP1) << USBFS_EPX_CNTX_ADDR_SHIFT);
        cr = CY_GET_REG8(&USBFS_SIE_EP1_CR0_PTR[ri]) & USBFS_MODE_ACKD; 
    }
    
    return(cr);
}


/*******************************************************************************
* Function Name: USBFS_SetPowerStatus
********************************************************************************
*
* Summary:
*  Sets the device power status for reporting in the Get Device Status
*  request
*
* Parameters:
*  powerStatus: USBFS_DEVICE_STATUS_BUS_POWERED(0) - Bus Powered, 
*               USBFS_DEVICE_STATUS_SELF_POWERED(1) - Self Powered
*
* Return:
*   None.
*
* Global variables:
*  USBFS_deviceStatus - set power status
*
* Reentrant:
*  No.
*
*******************************************************************************/
void USBFS_SetPowerStatus(uint8 powerStatus)
{
    if (powerStatus != USBFS_DEVICE_STATUS_BUS_POWERED)
    {
        USBFS_deviceStatus |=  USBFS_DEVICE_STATUS_SELF_POWERED;
    }
    else
    {
        USBFS_deviceStatus &=  ~USBFS_DEVICE_STATUS_SELF_POWERED;
    }
}


#if (USBFS_MON_VBUS == 1u)

    /*******************************************************************************
    * Function Name: USBFS_VBusPresent
    ********************************************************************************
    *
    * Summary:
    *  Determines VBUS presense for Self Powered Devices.  
    *
    * Parameters:
    *  None.
    *
    * Return:
    *  1 if VBUS is present, otherwise 0.
    *
    *******************************************************************************/
    uint8 USBFS_VBusPresent(void) 
    {
        return((CY_GET_REG8(USBFS_VBUS_PS_PTR) & USBFS_VBUS_MASK) ? 1u : 0u);
    }

#endif /* USBFS_MON_VBUS */


/*******************************************************************************
* Function Name: USBFS_RWUEnabled
********************************************************************************
*
* Summary:
*  Returns TRUE if Remote Wake Up is enabled, otherwise FALSE
*
* Parameters:
*   None.
*
* Return:
*  TRUE -  Remote Wake Up Enabled
*  FALSE - Remote Wake Up Disabled
*
* Global variables:
*  USBFS_deviceStatus - checked to determine remote status
*
*******************************************************************************/
uint8 USBFS_RWUEnabled(void) 
{
    uint8 result = USBFS_FALSE;
    if((USBFS_deviceStatus & USBFS_DEVICE_STATUS_REMOTE_WAKEUP) != 0u)
    {
        result = USBFS_TRUE;
    }

    return(result);
}


/* [] END OF FILE */
