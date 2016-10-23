/*******************************************************************************
* File Name: USBUART_vnd.c
* Version 0.0
*
*  Description:
*    USB vendor request handler.
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
* Vendor Specific Declarations
********************************************************************************/
/* `#START VENDOR_SPECIFIC_DECLARATIONS` Place your declaration here */

/* `#END` */
/*******************************************************************************
* External References
********************************************************************************/
uint8 USBUART_InitControlRead(void);
uint8 USBUART_InitControlWrite(void);

extern uint8 USBUART_CODE USBUART_MSOS_CONFIGURATION_DESCR[];

extern T_USBUART_TD CurrentTD;

/*******************************************************************************
* Function Name: USBUART_HandleVendorRqst
********************************************************************************
* Summary:
*   This routine provide users with a method to implement vendor specifc
*   requests.
*
*   To implement vendor specific requests, add your code in this function to
*   decode and disposition the request.  If the request is handled, your code
*   must set the variable "bRequestHandled" to TRUE, indicating that the 
*   request has been handled.
*   
* Parameters:  
*   None
*******************************************************************************/
uint8 USBUART_HandleVendorRqst()
{
	uint8 bRequestHandled = FALSE;
	
   if ((CY_GET_REG8(USBUART_bmRequestType) & USBUART_RQST_DIR_MASK) == USBUART_RQST_DIR_D2H)
    {
        /* Control Read */
        switch (CY_GET_REG8(USBUART_bRequest)) 
        {
        case USBUART_GET_CONFIG_DESCRIPTOR:
		  #if defined(USBUART_ENABLE_MSOS_STRING)
                CurrentTD.pData     = &USBUART_MSOS_CONFIGURATION_DESCR[0];
                CurrentTD.wCount    = USBUART_MSOS_CONFIGURATION_DESCR[0];
                bRequestHandled     = USBUART_InitControlRead();		
		  #endif		
            break;
        default:
            break;
        }
    }		
	
/* `#START VENDOR_SPECIFIC_CODE` Place your vendor specific request here */

/* `#END` */
    return bRequestHandled;   
}
/*******************************************************************************
* Additional user functions supporting Vendor Specific Requests
********************************************************************************/
/* `#START VENDOR_SPECIFIC_FUNCTIONS` Place any additional functions here */

/* `#END` */


