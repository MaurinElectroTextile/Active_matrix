/*******************************************************************************
* File Name: USBFS_cls.c
* Version 1.50
*
* Description:
*  USB Class request handler.
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

#if(USBFS_EXTERN_CLS == USBFS_FALSE)


/***************************************
* External references
***************************************/

#if defined(USBFS_ENABLE_HID_CLASS)
    uint8 USBFS_DispatchHIDClassRqst(void);
#endif /* End USBFS_ENABLE_HID_CLASS */


/***************************************
* User Implemented Class Driver Declarations.
***************************************/
/* `#START USER_DEFINED_CLASS_DECLARATIONS` Place your declaration here */

/* `#END` */


/*******************************************************************************
* Function Name: USBFS_DispatchClassRqst
********************************************************************************
* Summary:
*  This routine dispatches class requests
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
uint8 USBFS_DispatchClassRqst()
{
    uint8 requestHandled = USBFS_FALSE;
    
    #if defined(USBFS_ENABLE_HID_CLASS)
        requestHandled = USBFS_DispatchHIDClassRqst();
    #endif /* USBFS_ENABLE_HID_CLASS */

    /* `#START USER_DEFINED_CLASS_CODE` Place your Class request here */

    /* `#END` */
    
    return(requestHandled);
}


/*******************************************************************************
* Additional user functions supporting Class Specific Requests
********************************************************************************/

/* `#START CLASS_SPECIFIC_FUNCTIONS` Place any additional functions here */

/* `#END` */

#endif /* USBFS_EXTERN_CLS */


/* [] END OF FILE */
