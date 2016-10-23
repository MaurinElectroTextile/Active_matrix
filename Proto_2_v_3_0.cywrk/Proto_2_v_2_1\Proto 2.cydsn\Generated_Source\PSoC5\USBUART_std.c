/*******************************************************************************
* File Name: USBUART_std.c
* Version 0.0
*
*  Description:
*    USB Standard request handler.
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
#include "USBUART.h"

/*******************************************************************************
* External references
********************************************************************************/
uint8 USBUART_InitControlRead(void);
uint8 USBUART_InitControlWrite(void);
uint8 USBUART_InitNoDataControlTransfer(void);
uint8 USBUART_DispatchClassRqst(void);

extern uint8 USBUART_CODE USBUART_DEVICE0_DESCR[];
extern uint8 USBUART_CODE USBUART_DEVICE0_CONFIGURATION0_DESCR[]; 
extern uint8 USBUART_CODE USBUART_STRING_DESCRIPTORS[];
extern uint8 USBUART_CODE USBUART_MSOS_DESCRIPTOR[];
extern uint8 USBUART_CODE USBUART_SN_STRING_DESCRIPTOR[];


extern uint8 USBUART_bEPHalt;
extern uint8 USBUART_bDevice;
extern uint8 USBUART_bConfiguration;
extern uint8 USBUART_bInterfaceSetting[];
extern uint8 USBUART_bDeviceAddress;
extern uint8 USBUART_bDeviceStatus;
extern T_USBUART_LUT USBUART_CODE USBUART_TABLE[];
extern T_USBUART_EP_CTL_BLOCK USBUART_EP[];
extern T_USBUART_TD CurrentTD;
uint8 tBuffer[2];
uint8 *USBUART_bFWSerialNumberStringDescriptor;
uint8 USBUART_bSNStringConfirm = 0;

/*DIE ID string descriptor for 8 bytes ID*/
#if defined(USBUART_ENABLE_IDSN_STRING)
void USBUART_ReadDieID(uint8 *descr);
uint8 USBUART_bIDSerialNumberStringDescriptor[0x22]={0x22,0x03};
#endif	

/*******************************************************************************
* Forward references
********************************************************************************/
void USBUART_Config(void);
T_USBUART_LUT *USBUART_GetConfigTablePtr(uint8 c);
T_USBUART_LUT *USBUART_GetDeviceTablePtr(void);
uint8 USBUART_ClearEndpointHalt(void);
uint8 USBUART_SetEndpointHalt(void);
uint8 USBUART_ValidateAlternateSetting(void);
//uint8 USBUART_GetDeviceStatus(void);

/*******************************************************************************
* Function Name: USBUART_SerialNumString
********************************************************************************
* Summary:
*   Application firmware may supply the source of the USB device descriptors serial 
*   number string during runtime.
*   
* Parameters:  
*   SNstring - pointer to string
*******************************************************************************/
void  USBUART_SerialNumString(uint8 *SNstring)
{
#if defined(USBUART_ENABLE_FWSN_STRING)
    USBUART_bFWSerialNumberStringDescriptor = SNstring;
	/* check descriptor validation */
	if( (USBUART_bFWSerialNumberStringDescriptor[0] > 1 ) &&   /* Descriptor Length */
	    (USBUART_bFWSerialNumberStringDescriptor[1] == 3) )    /* DescriptorType: STRING */    
	{
	    USBUART_bSNStringConfirm = 1;
	}
    else
    {
	    USBUART_bSNStringConfirm = 0;
    }
#else
    SNstring = SNstring;
#endif     
}

/*******************************************************************************
* Function Name: USBUART_HandleStandardRqst
********************************************************************************
* Summary:
*   This Routine dispatches standard requests
*   
* Parameters:  
*   None
*******************************************************************************/
uint8 USBUART_HandleStandardRqst()
{
    uint8 bRequestHandled = USBUART_FALSE;
#if defined(USBUART_ENABLE_STRINGS)
    uint8 *pStr;
  #if defined(USBUART_ENABLE_DESCRIPTOR_STRINGS)
    uint8 nStr;
  #endif
#endif
    uint16 wCount;
    
    T_USBUART_LUT *pTmp;
    CurrentTD.wCount = 0;
    
    if ((CY_GET_REG8(USBUART_bmRequestType) & USBUART_RQST_DIR_MASK) == USBUART_RQST_DIR_D2H)
    {
        /* Control Read */
        switch (CY_GET_REG8(USBUART_bRequest)) 
        {
        case USBUART_GET_DESCRIPTOR:
            if (CY_GET_REG8(USBUART_wValueHi) == USBUART_DESCR_DEVICE)  
            {
                pTmp = USBUART_GetDeviceTablePtr();
                CurrentTD.pData     = pTmp->p_list;
                CurrentTD.wCount    = 18;
                bRequestHandled     = USBUART_InitControlRead();
                }
            else if (CY_GET_REG8(USBUART_wValueHi) == USBUART_DESCR_CONFIG)  {
                pTmp = USBUART_GetConfigTablePtr(CY_GET_REG8(USBUART_wValueLo));
                CurrentTD.pData     = pTmp->p_list;
                wCount    = ((uint16)(CurrentTD.pData)[3] << 8) | (CurrentTD.pData)[2];
                CurrentTD.wCount    = wCount;
                bRequestHandled     = USBUART_InitControlRead();
            }
#if defined(USBUART_ENABLE_STRINGS)
            else if (CY_GET_REG8(USBUART_wValueHi) == USBUART_DESCR_STRING)
            {
			/* Descriptor Strings*/	
			#if defined(USBUART_ENABLE_DESCRIPTOR_STRINGS)
                nStr = 0;
                pStr = &USBUART_STRING_DESCRIPTORS[0];
                while ((CY_GET_REG8(USBUART_wValueLo) > nStr) && (*pStr != 0)){
                    pStr += *pStr;
                    nStr++;
                };
			#endif
			/* Microsoft OS String*/	
			#if defined(USBUART_ENABLE_MSOS_STRING)
				if( CY_GET_REG8(USBUART_wValueLo) == 0xEE )
				{
	                pStr = &USBUART_MSOS_DESCRIPTOR[0];
				 }
			#endif
			/* SN string*/
			#if defined(USBUART_ENABLE_SN_STRING)
				if( CY_GET_REG8(USBUART_wValueLo) == USBUART_DEVICE0_DESCR[USBUART_DEVICE_DESCR_SN_SHIFT] )
				{
	                pStr = &USBUART_SN_STRING_DESCRIPTOR[0];
					if(USBUART_bSNStringConfirm != 0)
                    {
						pStr = USBUART_bFWSerialNumberStringDescriptor;
					}	
					#if defined(USBUART_ENABLE_IDSN_STRING)
					/* TODO: read DIE ID and genarete string descriptor in RAM*/
					    USBUART_ReadDieID(USBUART_bIDSerialNumberStringDescriptor);
					    pStr = USBUART_bIDSerialNumberStringDescriptor;
					#endif	
				 }
			#endif	 
                if (*pStr != 0)
                {
                    CurrentTD.wCount    = *pStr;
                    CurrentTD.pData     = pStr;
                    bRequestHandled     = USBUART_InitControlRead();
                }
            }
#endif
            else
            {
                bRequestHandled = USBUART_DispatchClassRqst();
            }
            break;
        case USBUART_GET_STATUS:
            switch ((CY_GET_REG8(USBUART_bmRequestType) & USBUART_RQST_RCPT_MASK))
            {
            case USBUART_RQST_RCPT_EP:
                    CurrentTD.wCount    = 1;
                    CurrentTD.pData     = &USBUART_EP[CY_GET_REG8(USBUART_wIndexLo) & 0x07].bHWEPState;
                    bRequestHandled     = USBUART_InitControlRead();
                break;
            case USBUART_RQST_RCPT_DEV:
                CurrentTD.wCount    = 2;
                tBuffer[0] = USBUART_bDeviceStatus;
                tBuffer[1] = 0;
                CurrentTD.pData     = &tBuffer[0];
                bRequestHandled     = USBUART_InitControlRead();
                break;
            }
            break;
        case USBUART_GET_CONFIGURATION:
            CurrentTD.wCount    = 1;
            CurrentTD.pData     = &USBUART_bConfiguration;
            bRequestHandled     = USBUART_InitControlRead();
            break;
        default:
            break;
        }
    }
    else {
        /* Control Write */
        switch (CY_GET_REG8(USBUART_bRequest)) 
        {
        case USBUART_SET_ADDRESS:
            USBUART_bDeviceAddress = (CY_GET_REG8(USBUART_wValueLo));
            bRequestHandled = USBUART_InitNoDataControlTransfer();
            break;
        case USBUART_SET_CONFIGURATION:
            USBUART_bConfiguration	= CY_GET_REG8(USBUART_wValueLo);
            USBUART_Config();
            bRequestHandled	= USBUART_InitNoDataControlTransfer();
            break;
		case USBUART_SET_INTERFACE:
			if (USBUART_ValidateAlternateSetting())
			{
	            bRequestHandled	= USBUART_InitNoDataControlTransfer();
			}
			break;
		case USBUART_CLEAR_FEATURE:
            switch ((CY_GET_REG8(USBUART_bmRequestType) & USBUART_RQST_RCPT_MASK))
            {
            case USBUART_RQST_RCPT_EP:
                if (CY_GET_REG8(USBUART_wValueLo) ==USBUART_ENDPOINT_HALT)
                {
                    bRequestHandled	= USBUART_ClearEndpointHalt();
                }
                break;
            case USBUART_RQST_RCPT_DEV:
                /* Clear device REMOTE_WAKEUP */
                USBUART_bDeviceStatus &= ~USBUART_DEVICE_STATUS_REMOTE_WAKEUP;
                bRequestHandled	= USBUART_InitNoDataControlTransfer();
                break;
            }
            break;
		case USBUART_SET_FEATURE:
            switch ((CY_GET_REG8(USBUART_bmRequestType) & USBUART_RQST_RCPT_MASK))
            {
            case USBUART_RQST_RCPT_EP:
                if (CY_GET_REG8(USBUART_wValueLo) ==USBUART_ENDPOINT_HALT)
                {
                    bRequestHandled	= USBUART_SetEndpointHalt();
                }
                break;
            case USBUART_RQST_RCPT_DEV:
                /* Clear device REMOTE_WAKEUP */
                USBUART_bDeviceStatus &= ~USBUART_DEVICE_STATUS_REMOTE_WAKEUP;
                bRequestHandled	= USBUART_InitNoDataControlTransfer();
                break;
            }
        default:
            break;
        }
	}
	return bRequestHandled;
}    

#if defined(USBUART_ENABLE_IDSN_STRING)

/*******************************************************************************
* Function Name: USBUART_ReadDieID
********************************************************************************
* Summary:
*   This routine read Die ID and genarete Serian Number string descriptor
*   
* Parameters:  
*   uint8 *descr - pointer on string descriptor
*******************************************************************************/
void USBUART_ReadDieID(uint8 *descr)
{
  uint8 i;
	/* check descriptor validation */
	if( (descr[0] > 1 ) &&   /* Descriptor Length */
	    (descr[1] == 3) )    /* DescriptorType: STRING */    
	{
		/* fill descriptor by counter for test only*/
	    for(i=2; i < descr[0]; i+=2)
		  descr[i] = 0x30 + ((i/2) & 0x7);
    }
	
}

#endif

/*******************************************************************************
* Function Name: USBUART_Config
********************************************************************************
* Summary:
*   This routine configures endpoints for the entire configuration by scanning
*   the configuration descriptor.  It configures the bAlternateSetting 0 for
*   each interface.
*   
* Parameters:  
*   None
*******************************************************************************/
void USBUART_Config()
{
    uint8 ep,cur_ep,i;
    uint8 iso, bCount;
    T_USBUART_LUT *pTmp;
    T_USBUART_EP_SETTINGS_BLOCK *pEP;

    /* Clear all of the endpoints */
    for (ep = 0; ep < 9; ep++)
    {
        USBUART_EP[ep].bAttrib = 0;
        USBUART_EP[ep].bHWEPState = 0;
        USBUART_EP[ep].bAPIEPState = USBUART_NO_EVENT_PENDING;
        USBUART_EP[ep].bEPToggle = 0;
        USBUART_EP[ep].bEPMode = 0;
        USBUART_EP[ep].wBufferSize = 0;
		USBUART_bInterfaceSetting[ep] = 0x00;
    }

    pTmp = USBUART_GetConfigTablePtr(USBUART_bConfiguration - 1);
    pTmp++;
    ep = pTmp->c;                                       /* For this table, c is the number of endpoints  */
    pEP = (T_USBUART_EP_SETTINGS_BLOCK *) pTmp->p_list;     /* and p_list points the endpoint setting table. */
    
    for (i = 0; i < ep; i++, pEP++)
    {
        cur_ep = pEP->bAddr; 
        cur_ep &= 0x0F;
        iso  = ((pEP->bmAttributes & USBUART_EP_TYPE_MASK) == USBUART_EP_TYPE_ISOC);
        if (pEP->bAddr & USBUART_DIR_IN)
        {
        /* IN Endpoint */
            USBUART_EP[cur_ep].bAPIEPState = USBUART_EVENT_PENDING;
            USBUART_EP[cur_ep].bEPMode = (iso ? USBUART_MODE_ISO_IN : USBUART_MODE_ACK_IN);
        }
        else
        {
        /* OUT Endpoint */
            USBUART_EP[cur_ep].bAPIEPState = USBUART_NO_EVENT_PENDING;
            USBUART_EP[cur_ep].bEPMode = (iso ? USBUART_MODE_ISO_OUT : USBUART_MODE_ACK_OUT);
        }
        USBUART_EP[cur_ep].wBufferSize = pEP->wBufferSize;
        USBUART_EP[cur_ep].bAddr = pEP->bAddr;
    } 

    /* Set the endpoint buffer addresses */
    bCount = 0;
    ep = 1;
    for (i = 0; i < 0x80; i+= 0x10)
    {
        CY_SET_REG8(&USBUART_ARB_EP1_CFG[i], USBUART_ARB_EPX_CFG_CRC_BYPASS);
        
        
        CY_SET_REG8(&USBUART_SIE_EP1_CR0[i], USBUART_MODE_NAK_IN_OUT);

        USBUART_EP[ep].wBuffOffset = bCount;
        bCount += USBUART_EP[ep].wBufferSize;
        CY_SET_REG8(&USBUART_SIE_EP1_CNT0[i],  (USBUART_EP[ep].wBufferSize >> 8));
        CY_SET_REG8(&USBUART_SIE_EP1_CNT1[i],  USBUART_EP[ep].wBufferSize & 0xFFu);

        CY_SET_REG8(&USBUART_ARB_RW1_RA[i],        USBUART_EP[ep].wBuffOffset & 0xFFu);
        CY_SET_REG8(&USBUART_ARB_RW1_RA_MSB[i],    (USBUART_EP[ep].wBuffOffset >> 8));
        CY_SET_REG8(&USBUART_ARB_RW1_WA[i],        USBUART_EP[ep].wBuffOffset & 0xFFu);
        CY_SET_REG8(&USBUART_ARB_RW1_WA_MSB[i],    (USBUART_EP[ep].wBuffOffset >> 8));
        
        ep++;
    }
    CY_SET_REG8(USBUART_SIE_EP_INT_EN, 0xFF);
}
/*******************************************************************************
* Function Name: USBUART_GetConfigTablePtr
********************************************************************************
* Summary:
*   This routine returns a pointer a configuration table entry
*   
* Parameters:  
*   c Configuration Index
*******************************************************************************/
T_USBUART_LUT *USBUART_GetConfigTablePtr(uint8 c)
{
    /* Device Table */
    T_USBUART_LUT *pTmp = USBUART_GetDeviceTablePtr();

    /* The first entry points to the Device Descriptor, the the configuration entries  */
    return pTmp[c + 1].p_list;
}
/*******************************************************************************
* Function Name: USBUART_GetDeviceTablePtr
********************************************************************************
* Summary:
*   This routine returns a pointer to the current config table based on the
*   selector in the request wValueLo
*   
* Parameters:  
*   None
*******************************************************************************/
T_USBUART_LUT *USBUART_GetDeviceTablePtr()
{
    /* Device Table */
    return USBUART_TABLE[USBUART_bDevice].p_list;
}

/*******************************************************************************
* Function Name: USBUART_SetEndpointHalt
********************************************************************************
* Summary:
*   This routine handles set endpoint halt
*   
* Parameters:  
*   None
*******************************************************************************/
uint8 USBUART_SetEndpointHalt()
{
    uint8 ep, ri;
    uint8 bRequestHandled = USBUART_FALSE;

    /* Clear endpoint halt */
    ep = (CY_GET_REG8(USBUART_wIndexLo) & 0x07);
    ri = ((ep - 1) << 4);
    if (ep != 0)
    {
        /* Set the endpoint Halt */
        USBUART_EP[ep].bHWEPState |= (USBUART_ENDPOINT_STATUS_HALT);
        
        /* Clear the data toggle */
        USBUART_EP[ep].bEPToggle = 0;
        
        if (USBUART_EP[ep].bAddr & USBUART_DIR_IN)
        {
            /* IN Endpoint */
            USBUART_EP[ep].bAPIEPState = USBUART_NO_EVENT_ALLOWED;
            CY_SET_REG8(&USBUART_SIE_EP1_CR0[ri], (USBUART_MODE_STALL_DATA_EP | USBUART_MODE_ACK_IN));
        }
        else
        {
            /* OUT Endpoint */
            USBUART_EP[ep].bAPIEPState = USBUART_NO_EVENT_ALLOWED;
            CY_SET_REG8(&USBUART_SIE_EP1_CR0[ri], (USBUART_MODE_STALL_DATA_EP | USBUART_MODE_ACK_OUT));
        }
        bRequestHandled = USBUART_InitNoDataControlTransfer();
    }
    return bRequestHandled;
}
/*******************************************************************************
* Function Name: USBUART_ClearEndpointHalt
********************************************************************************
* Summary:
*   This routine handles clear endpoint halt
*   
* Parameters:  
*   None
*******************************************************************************/
uint8 USBUART_ClearEndpointHalt()
{
    uint8 ep, ri;
    uint8 bRequestHandled = USBUART_FALSE;

    /* Clear endpoint halt */
    ep = (CY_GET_REG8(USBUART_wIndexLo) & 0x07);
    ri = ((ep  - 1) << 4);

    if (ep != 0)
    {
        /* Set the endpoint Halt */
        USBUART_EP[ep].bHWEPState &= ~(USBUART_ENDPOINT_STATUS_HALT);
        
        /* Clear the data toggle */
        USBUART_EP[ep].bEPToggle = 0;
        
        if (USBUART_EP[ep].bAddr & USBUART_DIR_IN)
        {
            /* IN Endpoint */
            USBUART_EP[ep].bAPIEPState = USBUART_NO_EVENT_PENDING;
            CY_SET_REG8(&USBUART_SIE_EP1_CR0[ri], USBUART_MODE_NAK_IN);
        }
        else
        {
            /* OUT Endpoint */
            USBUART_EP[ep].bAPIEPState = USBUART_EVENT_PENDING;
            CY_SET_REG8(&USBUART_SIE_EP1_CR0[ri], USBUART_MODE_ACK_OUT);
        }
        bRequestHandled = USBUART_InitNoDataControlTransfer();
    }
    return bRequestHandled;
}
/*******************************************************************************
* Function Name: USBUART_ClearEndpointHalt
********************************************************************************
* Summary:
*   Validates (and records) a SET INTERFACE request
*   
* Parameters:  
*   None
*******************************************************************************/
uint8 USBUART_ValidateAlternateSetting(void)
{
    uint8 bRequestHandled = USBUART_TRUE;

	// TODO: Validate interface setting, stall if invalid.
	USBUART_bInterfaceSetting[CY_GET_REG8(USBUART_wIndexLo)] = CY_GET_REG8(USBUART_wValueLo);
	return bRequestHandled;
}
