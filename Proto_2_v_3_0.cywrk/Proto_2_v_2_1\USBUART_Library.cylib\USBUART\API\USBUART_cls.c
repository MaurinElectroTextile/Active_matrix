/*******************************************************************************
* File Name: `$INSTANCE_NAME`_cls.c
* Version `$CY_MAJOR_VERSION`.`$CY_MINOR_VERSION`
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
#include "`$INSTANCE_NAME`.h"

/*******************************************************************************
* External references
********************************************************************************/
#if defined(`$INSTANCE_NAME`_ENABLE_HID_CLASS)
uint8 `$INSTANCE_NAME`_DispatchHIDClassRqst(void);
#endif
/*******************************************************************************
* User Implemented Class Driver Declarations.
********************************************************************************/
uint8 `$INSTANCE_NAME`_InitControlRead(void);
uint8 `$INSTANCE_NAME`_InitControlWrite(void);
uint8 `$INSTANCE_NAME`_InitNoDataControlTransfer(void);
uint8 `$INSTANCE_NAME`_DispatchClassRqst(void);
extern T_`$INSTANCE_NAME`_TD CurrentTD;
uint8 `$INSTANCE_NAME`_ValidateAlternateSetting(void);
extern uint8 bLineCoding[7];
/* `#START USER_DEFINED_CLASS_DECLARATIONS` Place your declaration here */

/* `#END` */

/*******************************************************************************
* Function Name: `$INSTANCE_NAME`_DispatchClassRqst
********************************************************************************
* Summary:
*   This routine dispatches class requests
*   
* Parameters:  
*   None
*******************************************************************************/
uint8 `$INSTANCE_NAME`_DispatchClassRqst()
{
	uint8 bRequestHandled = FALSE;
	uint8 mybuf[] = {0,0,0,0,0,0,0};	
/* TODO: Dispatch requests based on the interface number */
#if defined(`$INSTANCE_NAME`_ENABLE_HID_CLASS)
    bRequestHandled = `$INSTANCE_NAME`_DispatchHIDClassRqst();
#endif
    if ((CY_GET_REG8(`$INSTANCE_NAME`_bmRequestType) & `$INSTANCE_NAME`_RQST_DIR_MASK) == `$INSTANCE_NAME`_RQST_DIR_D2H) {
        /* Control Read */
		switch (CY_GET_REG8(`$INSTANCE_NAME`_bRequest))  {
			case 0x21:	/* GET_LINE_CODING */
				CurrentTD.wCount    = 7;
				CurrentTD.pData     = (uint8*)&bLineCoding;
				bRequestHandled     = `$INSTANCE_NAME`_InitControlRead();
				break;	
			default:
				break;
		}
	} else {
        /* Control Write */
        switch (CY_GET_REG8(`$INSTANCE_NAME`_bRequest)) {
			case 0x20:	/* SET_LINE_CODING */
				CurrentTD.wCount    = 7;
				CurrentTD.pData     = (uint8*)&bLineCoding;				
				bRequestHandled     = `$INSTANCE_NAME`_InitControlWrite();
				break;		
			case 0x22: /* SET_CONTROL_LINE_STATE */
				bRequestHandled	= `$INSTANCE_NAME`_InitNoDataControlTransfer();
				break;
			default:
				break;
		}			
	}
    return bRequestHandled;   
}
