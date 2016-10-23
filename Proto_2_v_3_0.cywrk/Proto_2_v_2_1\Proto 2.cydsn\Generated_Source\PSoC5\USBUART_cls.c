/*******************************************************************************
* File Name: USBUART_cls.c
* Version 0.0
*
*  Description:
*    USB Class request handler.
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
#if defined(USBUART_ENABLE_HID_CLASS)
uint8 USBUART_DispatchHIDClassRqst(void);
#endif
/*******************************************************************************
* User Implemented Class Driver Declarations.
********************************************************************************/
uint8 USBUART_InitControlRead(void);
uint8 USBUART_InitControlWrite(void);
uint8 USBUART_InitNoDataControlTransfer(void);
uint8 USBUART_DispatchClassRqst(void);
extern T_USBUART_TD CurrentTD;
uint8 USBUART_ValidateAlternateSetting(void);
extern uint8 bLineCoding[7];
/* `#START USER_DEFINED_CLASS_DECLARATIONS` Place your declaration here */

/* `#END` */

/*******************************************************************************
* Function Name: USBUART_DispatchClassRqst
********************************************************************************
* Summary:
*   This routine dispatches class requests
*   
* Parameters:  
*   None
*******************************************************************************/
uint8 USBUART_DispatchClassRqst()
{
	uint8 bRequestHandled = FALSE;
	uint8 mybuf[] = {0,0,0,0,0,0,0};	
/* TODO: Dispatch requests based on the interface number */
#if defined(USBUART_ENABLE_HID_CLASS)
    bRequestHandled = USBUART_DispatchHIDClassRqst();
#endif
    if ((CY_GET_REG8(USBUART_bmRequestType) & USBUART_RQST_DIR_MASK) == USBUART_RQST_DIR_D2H) {
        /* Control Read */
		switch (CY_GET_REG8(USBUART_bRequest))  {
			case 0x21:	/* GET_LINE_CODING */
				CurrentTD.wCount    = 7;
				CurrentTD.pData     = (uint8*)&bLineCoding;
				bRequestHandled     = USBUART_InitControlRead();
				break;	
			default:
				break;
		}
	} else {
        /* Control Write */
        switch (CY_GET_REG8(USBUART_bRequest)) {
			case 0x20:	/* SET_LINE_CODING */
				CurrentTD.wCount    = 7;
				CurrentTD.pData     = (uint8*)&bLineCoding;				
				bRequestHandled     = USBUART_InitControlWrite();
				break;		
			case 0x22: /* SET_CONTROL_LINE_STATE */
				bRequestHandled	= USBUART_InitNoDataControlTransfer();
				break;
			default:
				break;
		}			
	}
    return bRequestHandled;   
}
