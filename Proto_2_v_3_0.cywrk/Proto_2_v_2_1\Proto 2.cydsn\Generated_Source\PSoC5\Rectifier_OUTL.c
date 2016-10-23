/*******************************************************************************
* File Name: Rectifier_OUTL.c  
* Version 1.50
*
* Description:
*  This file contains API to enable firmware to read the value of a status 
*  register.
*
* Note:
*
********************************************************************************
* Copyright 2008-2010, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions, 
* disclaimers, and limitations in the end user license agreement accompanying 
* the software package with which this file was provided.
********************************************************************************/

#include "cytypes.h"
#include "Rectifier_OUTL.h"

#if !defined(Rectifier_OUTL_sts_reg__REMOVED) /* Check for removal by optimization */


/*******************************************************************************
* Function Name: Rectifier_OUTL_Read
********************************************************************************
*
* Summary:
*  Read the value of the Rectifier_OUTL_Status register
*
* Parameters:  
*  void
*
* Return: 
*  The value of the Rectifier_OUTL_Status register.
*
* Reentrant:
*  Yes
* 
*******************************************************************************/
uint8 Rectifier_OUTL_Read(void) 
{ 
    return Rectifier_OUTL_Status;
}


#endif /* Check for removal by optimization */

