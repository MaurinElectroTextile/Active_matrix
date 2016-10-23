/*******************************************************************************
* File Name: USBFS_std.c
* Version 1.50
*
* Description:
*  USB Standard request handler.
*
* Note:
*
********************************************************************************
* Copyright 2008-2010, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
********************************************************************************/

#include "USBFS.h"


/***************************************
* External references
***************************************/

extern uint8 CYCODE USBFS_DEVICE0_DESCR[];
extern uint8 CYCODE USBFS_DEVICE0_CONFIGURATION0_DESCR[];
extern uint8 CYCODE USBFS_STRING_DESCRIPTORS[];
extern uint8 CYCODE USBFS_MSOS_DESCRIPTOR[];
extern uint8 CYCODE USBFS_SN_STRING_DESCRIPTOR[];

extern volatile uint8 USBFS_epHalt;
extern volatile uint8 USBFS_device;
extern volatile uint8 USBFS_configuration;
extern volatile uint8 USBFS_interfaceSetting[];
extern volatile uint8 USBFS_deviceAddress;
extern volatile uint8 USBFS_deviceStatus;
extern T_USBFS_LUT CYCODE USBFS_TABLE[];
extern volatile T_USBFS_EP_CTL_BLOCK USBFS_EP[];
extern volatile T_USBFS_TD currentTD;


/***************************************
*         Forward references
***************************************/

uint8 USBFS_InitControlRead(void);
uint8 USBFS_InitControlWrite(void);
uint8 USBFS_InitNoDataControlTransfer(void);
uint8 USBFS_DispatchClassRqst(void);

void USBFS_Config(uint8 clearAltSetting);
T_USBFS_LUT *USBFS_GetConfigTablePtr(uint8 c) \
                                                            ;
T_USBFS_LUT *USBFS_GetDeviceTablePtr(void) \
                                                            ;
uint8 USBFS_ClearEndpointHalt(void);
uint8 USBFS_SetEndpointHalt(void);
uint8 USBFS_ValidateAlternateSetting(void);

/*DIE ID string descriptor for 8 bytes ID*/
#if defined(USBFS_ENABLE_IDSN_STRING)
    void USBFS_ReadDieID(uint8 *descr);
    uint8 USBFS_idSerialNumberStringDescriptor[0x22u]={0x22u, USBFS_DESCR_STRING};
#endif /* USBFS_ENABLE_IDSN_STRING */


/***************************************
* Global data allocation
***************************************/

uint8 tBuffer[2u];
uint8 *USBFS_fwSerialNumberStringDescriptor;
uint8 USBFS_snStringConfirm = USBFS_FALSE;


/*******************************************************************************
* Function Name: USBFS_SerialNumString
********************************************************************************
*
* Summary:
*  Application firmware may supply the source of the USB device descriptors 
*  serial number string during runtime.
*
* Parameters:
*  snString:  pointer to string.
*
* Return:
*  None.
*
* Reentrant:
*  No.
*
*******************************************************************************/
void  USBFS_SerialNumString(uint8 *snString)
{
    #if defined(USBFS_ENABLE_FWSN_STRING)
        USBFS_snStringConfirm = USBFS_FALSE;
        if(snString != USBFS_NULL)
        {
            USBFS_fwSerialNumberStringDescriptor = snString;
            /* check descriptor validation */
            if( (USBFS_fwSerialNumberStringDescriptor[0u] > 1u ) &&  \
                (USBFS_fwSerialNumberStringDescriptor[1u] == USBFS_DESCR_STRING) )
            {
                USBFS_snStringConfirm = USBFS_TRUE;
            }
        }
    #else
        snString = snString;
    #endif  /* USBFS_ENABLE_FWSN_STRING */
}


/*******************************************************************************
* Function Name: USBFS_HandleStandardRqst
********************************************************************************
*
* Summary:
*  This Routine dispatches standard requests
*
* Parameters:
*  None.
*
* Return:
*  TRUE if request handled.
*
* Reentrant:
*  No.
*
*******************************************************************************/
uint8 USBFS_HandleStandardRqst(void)
{
    uint8 requestHandled = USBFS_FALSE;
    #if defined(USBFS_ENABLE_STRINGS)
        uint8 *pStr = 0u;
        #if defined(USBFS_ENABLE_DESCRIPTOR_STRINGS)
            uint8 nStr;
        #endif /* USBFS_ENABLE_DESCRIPTOR_STRINGS */
    #endif /* USBFS_ENABLE_STRINGS */
    uint16 count;

    T_USBFS_LUT *pTmp;
    currentTD.count = 0u;

    if ((CY_GET_REG8(USBFS_bmRequestType) & USBFS_RQST_DIR_MASK) == USBFS_RQST_DIR_D2H)
    {
        /* Control Read */
        switch (CY_GET_REG8(USBFS_bRequest))
        {
            case USBFS_GET_DESCRIPTOR:
                if (CY_GET_REG8(USBFS_wValueHi) == USBFS_DESCR_DEVICE)
                {
                    pTmp = USBFS_GetDeviceTablePtr();
                    currentTD.pData = pTmp->p_list;
                    currentTD.count = USBFS_DEVICE_DESCR_LENGTH;
                    requestHandled  = USBFS_InitControlRead();
                }
                else if (CY_GET_REG8(USBFS_wValueHi) == USBFS_DESCR_CONFIG)
                {
                    pTmp = USBFS_GetConfigTablePtr(CY_GET_REG8(USBFS_wValueLo));
                    currentTD.pData = pTmp->p_list;
                    count = ((uint16)(currentTD.pData)[USBFS_CONFIG_DESCR_TOTAL_LENGTH_HI] << 8u) | \
                                                    (currentTD.pData)[USBFS_CONFIG_DESCR_TOTAL_LENGTH_LOW];
                    currentTD.count = count;
                    requestHandled  = USBFS_InitControlRead();
                }
                #if defined(USBFS_ENABLE_STRINGS)
                else if (CY_GET_REG8(USBFS_wValueHi) == USBFS_DESCR_STRING)
                {
                    /* Descriptor Strings*/
                    #if defined(USBFS_ENABLE_DESCRIPTOR_STRINGS)
                        nStr = 0u;
                        pStr = &USBFS_STRING_DESCRIPTORS[0u];
                        while ( (CY_GET_REG8(USBFS_wValueLo) > nStr) && (*pStr != 0u ))
                        {
                            pStr += *pStr;
                            nStr++;
                        };
                    #endif /* End USBFS_ENABLE_DESCRIPTOR_STRINGS */
                    /* Microsoft OS String*/
                    #if defined(USBFS_ENABLE_MSOS_STRING)
                        if( CY_GET_REG8(USBFS_wValueLo) == USBFS_STRING_MSOS )
                        {
                            pStr = &USBFS_MSOS_DESCRIPTOR[0u];
                        }
                    #endif /* End USBFS_ENABLE_MSOS_STRING*/
                    /* SN string*/
                    #if defined(USBFS_ENABLE_SN_STRING)
                        if( (CY_GET_REG8(USBFS_wValueLo) != 0 ) && \
                            (CY_GET_REG8(USBFS_wValueLo) == \
                            USBFS_DEVICE0_DESCR[USBFS_DEVICE_DESCR_SN_SHIFT]) )
                        {
                            pStr = &USBFS_SN_STRING_DESCRIPTOR[0u];
                            if(USBFS_snStringConfirm != USBFS_FALSE)
                            {
                                pStr = USBFS_fwSerialNumberStringDescriptor;
                            }
                            #if defined(USBFS_ENABLE_IDSN_STRING)
                                /* TODO: read DIE ID and genarete string descriptor in RAM*/
                                USBFS_ReadDieID(USBFS_idSerialNumberStringDescriptor);
                                pStr = USBFS_idSerialNumberStringDescriptor;
                            #endif    /* End USBFS_ENABLE_IDSN_STRING */
                        }
                    #endif    /* End USBFS_ENABLE_SN_STRING */
                    if (*pStr != 0u)
                    {
                        currentTD.count = *pStr;
                        currentTD.pData = pStr;
                        requestHandled  = USBFS_InitControlRead();
                    }
                }
                #endif /* End USBFS_ENABLE_STRINGS*/
                else
                {
                    requestHandled = USBFS_DispatchClassRqst();
                }
                break;
            case USBFS_GET_STATUS:
                switch ((CY_GET_REG8(USBFS_bmRequestType) & USBFS_RQST_RCPT_MASK))
                {
                    case USBFS_RQST_RCPT_EP:
                        currentTD.count = USBFS_EP_STATUS_LENGTH;
                        tBuffer[0] = USBFS_EP[ \
                                        CY_GET_REG8(USBFS_wIndexLo) & USBFS_DIR_UNUSED].hwEpState;
                        tBuffer[1] = 0u;
                        currentTD.pData = &tBuffer[0u];
                        requestHandled  = USBFS_InitControlRead();
                        break;
                    case USBFS_RQST_RCPT_DEV:
                        currentTD.count = USBFS_DEVICE_STATUS_LENGTH;
                        tBuffer[0u] = USBFS_deviceStatus;
                        tBuffer[1u] = 0u;
                        currentTD.pData = &tBuffer[0u];
                        requestHandled  = USBFS_InitControlRead();
                        break;
                    default:    /* requestHandled is initialezed as FALSE by default */
                        break;
                }
                break;
            case USBFS_GET_CONFIGURATION:
                currentTD.count = 1u;
                currentTD.pData = (uint8 *)&USBFS_configuration;
                requestHandled  = USBFS_InitControlRead();
                break;
            case USBFS_GET_INTERFACE:
                currentTD.count = 1u;
                currentTD.pData = (uint8 *)&USBFS_interfaceSetting[CY_GET_REG8(USBFS_wIndexLo)];
                requestHandled  = USBFS_InitControlRead();
                break;
            default: /* requestHandled is initialezed as FALSE by default */
                break;
        }
    }
    else {
        /* Control Write */
        switch (CY_GET_REG8(USBFS_bRequest))
        {
            case USBFS_SET_ADDRESS:
                USBFS_deviceAddress = CY_GET_REG8(USBFS_wValueLo);
                requestHandled = USBFS_InitNoDataControlTransfer();
                break;
            case USBFS_SET_CONFIGURATION:
                USBFS_configuration = CY_GET_REG8(USBFS_wValueLo);
                USBFS_Config(USBFS_TRUE);
                requestHandled = USBFS_InitNoDataControlTransfer();
                break;
            case USBFS_SET_INTERFACE:
                if (USBFS_ValidateAlternateSetting())
                {
                    USBFS_Config(USBFS_FALSE);
                    requestHandled = USBFS_InitNoDataControlTransfer();
                }
                break;
            case USBFS_CLEAR_FEATURE:
                switch (CY_GET_REG8(USBFS_bmRequestType) & USBFS_RQST_RCPT_MASK)
                {
                case USBFS_RQST_RCPT_EP:
                    if (CY_GET_REG8(USBFS_wValueLo) == USBFS_ENDPOINT_HALT)
                    {
                        requestHandled = USBFS_ClearEndpointHalt();
                    }
                    break;
                case USBFS_RQST_RCPT_DEV:
                    /* Clear device REMOTE_WAKEUP */
                    if (CY_GET_REG8(USBFS_wValueLo) == USBFS_DEVICE_REMOTE_WAKEUP)
                    {
                        USBFS_deviceStatus &= ~USBFS_DEVICE_STATUS_REMOTE_WAKEUP;
                        requestHandled = USBFS_InitNoDataControlTransfer();
                    }
                    break;
                }
                break;
            case USBFS_SET_FEATURE:
                switch (CY_GET_REG8(USBFS_bmRequestType) & USBFS_RQST_RCPT_MASK)
                {
                    case USBFS_RQST_RCPT_EP:
                        if (CY_GET_REG8(USBFS_wValueLo) == USBFS_ENDPOINT_HALT)
                        {
                            requestHandled = USBFS_SetEndpointHalt();
                        }
                        break;
                    case USBFS_RQST_RCPT_DEV:
                        /* Set device REMOTE_WAKEUP */
                        if (CY_GET_REG8(USBFS_wValueLo) == USBFS_DEVICE_REMOTE_WAKEUP)
                        {
                            USBFS_deviceStatus |= USBFS_DEVICE_STATUS_REMOTE_WAKEUP;
                            requestHandled = USBFS_InitNoDataControlTransfer();
                        }
                        break;
                    default:    /* requestHandled is initialezed as FALSE by default */
                        break;
                }
                break;
            default:    /* requestHandled is initialezed as FALSE by default */
                break;
        }
    }
    return(requestHandled);
}


#if defined(USBFS_ENABLE_IDSN_STRING)

    /***************************************************************************
    * Function Name: USBFS_ReadDieID
    ****************************************************************************
    *
    * Summary:
    *  This routine read Die ID and genarete Serian Number string descriptor.
    *
    * Parameters:
    *  descr:  pointer on string descriptor.
    *
    * Return:
    *  None.
    *
    * Reentrant:
    *  No.
    *
    ***************************************************************************/
    void USBFS_ReadDieID(uint8 *descr)
    {
        uint8 i,j;
        uint8 value;
        static char8 const hex[16u] = "0123456789ABCDEF";

        /* check descriptor validation */
        if( (descr[0u] > 1u ) && (descr[1u] == USBFS_DESCR_STRING) )
        {
            /* fill descriptor */
            for(j = 0u, i = 2u; i < descr[0u]; i += 2u)
            {
                value = CY_GET_XTND_REG8((void CYFAR *)(USBFS_DIE_ID + j++));
                descr[i] = (uint8)hex[value >> 4u];
                i += 2u;
                descr[i] = (uint8)hex[value & 0x0Fu];
            }
        }
    }

#endif /* End $INSTANCE_NAME`_ENABLE_IDSN_STRING*/


/*******************************************************************************
* Function Name: USBFS_Config
********************************************************************************
*
* Summary:
*  This routine configures endpoints for the entire configuration by scanning
*  the configuration descriptor.
*
* Parameters:
*  clearAltSetting: It configures the bAlternateSetting 0 for each interface.
*
* Return:
*  None.
*
* Reentrant:
*  No.
*
*******************************************************************************/
void USBFS_Config(uint8 clearAltSetting)
{
    uint8 ep,cur_ep,i;
    uint8 iso;
    uint16 count;
    uint8 *pDescr;
    T_USBFS_LUT *pTmp;
    T_USBFS_EP_SETTINGS_BLOCK *pEP;

    /* Clear all of the endpoints */
    for (ep = 0u; ep < USBFS_MAX_EP; ep++)
    {
        USBFS_EP[ep].attrib = 0u;
        USBFS_EP[ep].hwEpState = 0u;
        USBFS_EP[ep].apiEpState = USBFS_NO_EVENT_PENDING;
        USBFS_EP[ep].epToggle = 0u;
        USBFS_EP[ep].epMode = USBFS_MODE_DISABLE;
        USBFS_EP[ep].bufferSize = 0u;
    }

    /* Clear Alternate settings for all interfaces */
    if(clearAltSetting != 0u)
    {
        for (i = 0u; i < USBFS_MAX_INTERFACES_NUMBER; i++)
        {
            USBFS_interfaceSetting[i] = 0x00u;
        }
    }

    /* Init Endpoints and Device Status if configured */
    if(USBFS_configuration > 0u)
    {
        pTmp = USBFS_GetConfigTablePtr(USBFS_configuration - 1u);
        /* Set Power status for current configuration */
        pDescr = (uint8 *)pTmp->p_list;
        USBFS_SetPowerStatus(pDescr[USBFS_CONFIG_DESCR_ATTRIB] & \
                                        USBFS_CONFIG_DESCR_ATTRIB_SELF_POWERED);
        pTmp++;
        ep = pTmp->c;  /* For this table, c is the number of endpoints  */
        /* and p_list points the endpoint setting table. */
        pEP = (T_USBFS_EP_SETTINGS_BLOCK *) pTmp->p_list;   

        for (i = 0u; i < ep; i++, pEP++)
        {
            /* compate current Alternate setting with EP Alt*/
            if(USBFS_interfaceSetting[pEP->interface] == pEP->altSetting)
            {
                cur_ep = pEP->addr;
                cur_ep &= 0x0Fu;
                iso  = ((pEP->attributes & USBFS_EP_TYPE_MASK) == USBFS_EP_TYPE_ISOC);
                if (pEP->addr & USBFS_DIR_IN)
                {
                    /* IN Endpoint */
                    USBFS_EP[cur_ep].apiEpState = USBFS_EVENT_PENDING;
                    USBFS_EP[cur_ep].epMode = \
                                            (iso ? USBFS_MODE_ISO_IN : USBFS_MODE_ACK_IN);
                }
                else
                {
                    /* OUT Endpoint */
                    USBFS_EP[cur_ep].apiEpState = USBFS_NO_EVENT_PENDING;
                    USBFS_EP[cur_ep].epMode = \
                                            (iso ? USBFS_MODE_ISO_OUT : USBFS_MODE_ACK_OUT);
                }
                USBFS_EP[cur_ep].bufferSize = pEP->bufferSize;
                USBFS_EP[cur_ep].addr = pEP->addr;
            }
        }

        /* Set the endpoint buffer addresses */
        count = 0u;
        ep = 1u;
        for (i = 0u; i < 0x80u; i+= 0x10u)
        {
            CY_SET_REG8(&USBFS_ARB_EP1_CFG_PTR[i], USBFS_ARB_EPX_CFG_CRC_BYPASS);

            if(USBFS_EP[ep].epMode != USBFS_MODE_DISABLE)
            {
                if((USBFS_EP[ep].addr & USBFS_DIR_IN) != 0u ) 
                {
                    CY_SET_REG8(&USBFS_SIE_EP1_CR0_PTR[i], USBFS_MODE_NAK_IN);
                }
                else
                {
                    CY_SET_REG8(&USBFS_SIE_EP1_CR0_PTR[i], USBFS_MODE_NAK_OUT);
                }
            }
            else
            {
                CY_SET_REG8(&USBFS_SIE_EP1_CR0_PTR[i], USBFS_MODE_STALL_DATA_EP);
            }    

            USBFS_EP[ep].buffOffset = count;
            count += USBFS_EP[ep].bufferSize;
            CY_SET_REG8(&USBFS_SIE_EP1_CNT0_PTR[i],   USBFS_EP[ep].bufferSize >> 8u);
            CY_SET_REG8(&USBFS_SIE_EP1_CNT1_PTR[i],   USBFS_EP[ep].bufferSize & 0xFFu);

            CY_SET_REG8(&USBFS_ARB_RW1_RA_PTR[i],     USBFS_EP[ep].buffOffset & 0xFFu);
            CY_SET_REG8(&USBFS_ARB_RW1_RA_MSB_PTR[i], USBFS_EP[ep].buffOffset >> 8u);
            CY_SET_REG8(&USBFS_ARB_RW1_WA_PTR[i],     USBFS_EP[ep].buffOffset & 0xFFu);
            CY_SET_REG8(&USBFS_ARB_RW1_WA_MSB_PTR[i], USBFS_EP[ep].buffOffset >> 8u);

            ep++;
        }
    } /* USBFS_configuration > 0 */
    CY_SET_REG8(USBFS_SIE_EP_INT_EN_PTR, 0xFFu);
}


/*******************************************************************************
* Function Name: USBFS_GetConfigTablePtr
********************************************************************************
*
* Summary:
*  This routine returns a pointer a configuration table entry
*
* Parameters:
*  c:  Configuration Index
*
* Return:
*  Device Descriptor pointer.
*
*******************************************************************************/
T_USBFS_LUT *USBFS_GetConfigTablePtr(uint8 c) \
                                                        
{
    /* Device Table */
    T_USBFS_LUT *pTmp;

    pTmp = USBFS_GetDeviceTablePtr();
    
    /* The first entry points to the Device Descriptor, 
       the the configuration entries  */    
    return(pTmp[c + 1u].p_list);
}


/*******************************************************************************
* Function Name: USBFS_GetDeviceTablePtr
********************************************************************************
*
* Summary:
*  This routine returns a pointer to the Device table
*
* Parameters:
*  None.
*
* Return:
*  Device Table pointer
*
*******************************************************************************/
T_USBFS_LUT *USBFS_GetDeviceTablePtr(void) 
{
    /* Device Table */
    return(USBFS_TABLE[USBFS_device].p_list);
}


/*******************************************************************************
* Function Name: USBFS_SetEndpointHalt
********************************************************************************
*
* Summary:
*  This routine handles set endpoint halt.
*
* Parameters:
*  None.
*
* Return:
*  requestHandled.
*
* Reentrant:
*  No.
*
*******************************************************************************/
uint8 USBFS_SetEndpointHalt(void)
{
    uint8 ep, ri;
    uint8 requestHandled = USBFS_FALSE;

    /* Clear endpoint halt */
    ep = CY_GET_REG8(USBFS_wIndexLo) & USBFS_DIR_UNUSED;
    ri = ((ep - USBFS_EP1) << USBFS_EPX_CNTX_ADDR_SHIFT);

    if ((ep > USBFS_EP0) && (ep < USBFS_MAX_EP))
    {
        /* Set the endpoint Halt */
        USBFS_EP[ep].hwEpState |= (USBFS_ENDPOINT_STATUS_HALT);

        /* Clear the data toggle */
        USBFS_EP[ep].epToggle = 0u;

        if (USBFS_EP[ep].addr & USBFS_DIR_IN)
        {
            /* IN Endpoint */
            USBFS_EP[ep].apiEpState = USBFS_NO_EVENT_ALLOWED;
            CY_SET_REG8(&USBFS_SIE_EP1_CR0_PTR[ri], (USBFS_MODE_STALL_DATA_EP | \
                                                            USBFS_MODE_ACK_IN));
        }
        else
        {
            /* OUT Endpoint */
            USBFS_EP[ep].apiEpState = USBFS_NO_EVENT_ALLOWED;
            CY_SET_REG8(&USBFS_SIE_EP1_CR0_PTR[ri], (USBFS_MODE_STALL_DATA_EP | \
                                                            USBFS_MODE_ACK_OUT));
        }
        requestHandled = USBFS_InitNoDataControlTransfer();
    }

    return(requestHandled);
}


/*******************************************************************************
* Function Name: USBFS_ClearEndpointHalt
********************************************************************************
*
* Summary:
*  This routine handles clear endpoint halt.
*
* Parameters:
*  None.
*
* Return:
*  requestHandled.
*
* Reentrant:
*  No.
*
*******************************************************************************/
uint8 USBFS_ClearEndpointHalt(void)
{
    uint8 ep, ri;
    uint8 requestHandled = USBFS_FALSE;

    /* Clear endpoint halt */
    ep = CY_GET_REG8(USBFS_wIndexLo) & USBFS_DIR_UNUSED;
    ri = ((ep - USBFS_EP1) << USBFS_EPX_CNTX_ADDR_SHIFT);

    if ((ep > USBFS_EP0) && (ep < USBFS_MAX_EP))
    {
        /* Set the endpoint Halt */
        USBFS_EP[ep].hwEpState &= ~(USBFS_ENDPOINT_STATUS_HALT);

        /* Clear the data toggle */
        USBFS_EP[ep].epToggle = 0u;

        if (USBFS_EP[ep].addr & USBFS_DIR_IN)
        {
            /* IN Endpoint */
            USBFS_EP[ep].apiEpState = USBFS_NO_EVENT_PENDING;
            CY_SET_REG8(&USBFS_SIE_EP1_CR0_PTR[ri], USBFS_MODE_NAK_IN);
        }
        else
        {
            /* OUT Endpoint */
            USBFS_EP[ep].apiEpState = USBFS_EVENT_PENDING;
            CY_SET_REG8(&USBFS_SIE_EP1_CR0_PTR[ri], USBFS_MODE_ACK_OUT);
        }
        requestHandled = USBFS_InitNoDataControlTransfer();
    }

    return(requestHandled);
}


/*******************************************************************************
* Function Name: USBFS_ValidateAlternateSetting
********************************************************************************
*
* Summary:
*  Validates (and records) a SET INTERFACE request.
*
* Parameters:
*  None.
*
* Return:
*  requestHandled.
*
* Reentrant:
*  No.
*
*******************************************************************************/
uint8 USBFS_ValidateAlternateSetting(void)
{
    uint8 requestHandled = USBFS_TRUE;
    uint8 interfaceNum;
    T_USBFS_LUT *pTmp;
    uint8 currentInterfacesNum;

    interfaceNum = CY_GET_REG8(USBFS_wIndexLo);
    /* Validate interface setting, stall if invalid. */
    pTmp = USBFS_GetConfigTablePtr(USBFS_configuration - 1u);
    currentInterfacesNum  = ((uint8 *) pTmp->p_list)[USBFS_CONFIG_DESCR_NUM_INTERFACES];

    if((interfaceNum >= currentInterfacesNum) || (interfaceNum >= USBFS_MAX_INTERFACES_NUMBER))
    {   /* wrong interface number */
        requestHandled = USBFS_FALSE;
    }
    else
    {
        USBFS_interfaceSetting[interfaceNum] = CY_GET_REG8(USBFS_wValueLo);
    }

    return (requestHandled);
}


/* [] END OF FILE */
