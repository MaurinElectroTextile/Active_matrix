/*******************************************************************************
* File Name: USBUART_hid.c
* Version 0.0
*
*  Description:
*    USB HID Class request handler.
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
#include "USBUART_hid.h"

#if defined(USBUART_ENABLE_HID_CLASS)

/*******************************************************************************
* HID Variables
********************************************************************************/
uint8	USBUART_bHID_Protocol;			/* HID device protocol status */
uint8	USBUART_bHID_IdleRate;			/* HID device idle rate value */
uint8	USBUART_bHID_ReportID;			/* HID device Report ID */

/*******************************************************************************
* Custom Declratations
********************************************************************************/
/* `#START CUSTOM_DECLARATIONS` Place your declaration here */

/* `#END` */
/*******************************************************************************
* External references
********************************************************************************/
uint8 USBUART_InitControlRead(void);
uint8 USBUART_InitControlWrite(void);
uint8 USBUART_InitNoDataControlTransfer(void);
T_USBUART_LUT *USBUART_GetConfigTablePtr(uint8 c);
T_USBUART_LUT *USBUART_GetDeviceTablePtr();
extern T_USBUART_TD CurrentTD;
extern uint8 USBUART_CODE USBUART_HIDREPORT_DESCRIPTORS[];
extern uint8 USBUART_bConfiguration;

/*******************************************************************************
* Internal references
********************************************************************************/
void USBUART_FindReport(void);

/*******************************************************************************
* Function Name: USBUART_UpdateHIDTimer
********************************************************************************
* Summary:
*   Updates the HID report timer and reloads it if it expires
*   
* Parameters:  
*   bInterface Interface Number
*******************************************************************************/
uint8 USBUART_UpdateHIDTimer(uint8 bInterface)
{
    uint8 ifc = bInterface;
    return USBUART_IDLE_TIMER_INDEFINITE;
}
/*******************************************************************************
* Function Name: USBUART_bGetProtocol
********************************************************************************
* Summary:
*   Returns the selected protocol value to the application
*   
* Parameters:  
*   bInterface Interface Number
*******************************************************************************/
uint8 USBUART_bGetProtocol(uint8 bInterface)
{
    uint8 ifc = bInterface;
    return USBUART_PROTOCOL_REPORT;
}
/*******************************************************************************
* Function Name: USBUART_DispatchHIDClassRqst
********************************************************************************
* Summary:
*   This routine dispatches class requests
*   
* Parameters:  
*   None
*******************************************************************************/
uint8 USBUART_DispatchHIDClassRqst()
{
    uint8 *pDescr;
    uint8 bRequestHandled = USBUART_FALSE;
    T_USBUART_LUT *pTmp = USBUART_GetConfigTablePtr(USBUART_bConfiguration - 1);

    if ((CY_GET_REG8(USBUART_bmRequestType) & USBUART_RQST_DIR_MASK) == USBUART_RQST_DIR_D2H)
    {   /* Control Read */
		switch (*USBUART_bRequest) 
		{
			case USBUART_GET_DESCRIPTOR:
                if (CY_GET_REG8(USBUART_wValueHi) == USBUART_DESCR_HID_CLASS)
                {
                    /* pTmp is pointing to the Config Table */
                    /* Next...                              */
				    /* Get the Config Descriptor            */
                    /*   The HID Class Descriptor resides   */
                    /*   inside the config descriptor       */
                    CurrentTD.pData     = &((uint8 *) pTmp->p_list)[18];
                    CurrentTD.wCount    = 9;
					bRequestHandled     = USBUART_InitControlRead();
                }
                else if (CY_GET_REG8(USBUART_wValueHi) == USBUART_DESCR_HID_REPORT)  
				{
                    pDescr = &USBUART_HIDREPORT_DESCRIPTORS[0];
                    CurrentTD.wCount    =  (((uint16)pDescr[1] << 8) | pDescr[0]);
                    CurrentTD.pData     = &pDescr[2];
    
                    if (CurrentTD.wCount != 0)
                    {
                        bRequestHandled     = USBUART_InitControlRead();
                    }
				}
                break;
            case USBUART_HID_GET_REPORT:
                /*******************************************************************************
                * Not Supported -- But leaving a place holder in case someone needs it
                ********************************************************************************/
                /* `#START CUSTOM_HID_GET_REPORT` Place your code */

                /* `#END` */
                USBUART_FindReport();
                bRequestHandled     = USBUART_InitControlRead();
                break;

			case USBUART_HID_GET_IDLE:
				CurrentTD.wCount    = 1;
				CurrentTD.pData     = &USBUART_bHID_IdleRate;
				bRequestHandled     = USBUART_InitControlRead();
                break;
			case USBUART_HID_GET_PROTOCOL:
				CurrentTD.wCount    = 1;
				CurrentTD.pData     = &USBUART_bHID_Protocol;
				bRequestHandled     = USBUART_InitControlRead();
                break;
            default:
                break;
        }
    }
    else if ((CY_GET_REG8(USBUART_bmRequestType) & USBUART_RQST_DIR_MASK) == USBUART_RQST_DIR_H2D)
    {   /* Control Write */
		switch (CY_GET_REG8(USBUART_bRequest)) 
		{
            case USBUART_HID_SET_REPORT:
                /*******************************************************************************
                * Not Supported -- But leaving a place holder in case someone needs it
                ********************************************************************************/
                /* `#START CUSTOM_HID_SET_REPORT` Place your code */

                /* `#END` */
                USBUART_FindReport();
                bRequestHandled     = USBUART_InitControlWrite();
                break;
            case USBUART_HID_SET_IDLE:
				USBUART_bHID_IdleRate = CY_GET_REG8(USBUART_wValueHi);
				USBUART_bHID_ReportID = CY_GET_REG8(USBUART_wValueLo);
				bRequestHandled     = USBUART_InitNoDataControlTransfer();
                break;
            case USBUART_HID_SET_PROTOCOL:
			    USBUART_bHID_Protocol  = CY_GET_REG8(USBUART_wValueHi);
				bRequestHandled     = USBUART_InitNoDataControlTransfer();
                break;
            default:
                break;
        }
    }
    return bRequestHandled;   
}
/*******************************************************************************
* Function Name: USBUART_FindReport
********************************************************************************
* Summary:
*   This routine sets up a transfer based on the Interface number, Report Type
*   and Report ID, then loads the CurrentTD structure with the address of the
*   buffer and the size.  The caller has to decide if it is a control read or
*   control write.
*   
* Parameters:  
*   None
*******************************************************************************/
void USBUART_FindReport()
{
    T_USBUART_LUT *pTmp = USBUART_GetConfigTablePtr(USBUART_bConfiguration - 1);
    T_USBUART_TD *pTD;
    /* Third entry in the LUT starts the Interface Table pointers */
    pTmp++;
    pTmp++;
    /* Now use the request interface number */
    pTmp = &pTmp[CY_GET_REG8(USBUART_wIndexLo)];
	pTmp = (T_USBUART_LUT *) pTmp->p_list;
    pTmp = (T_USBUART_LUT *) pTmp->p_list;
    /* Get the entry proper TD */
    pTD = &((T_USBUART_TD *) pTmp->p_list)[CY_GET_REG8(USBUART_wValueHi) - 1];

    CurrentTD.pData     = pTD->pData;
    CurrentTD.wCount    = pTD->wCount;
    CurrentTD.pStatusBlock = pTD->pStatusBlock;
}
#endif
