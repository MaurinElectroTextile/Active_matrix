/*******************************************************************************
* File Name: Reg_Debug_OUT.c  
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
#include "Reg_Debug_OUT.h"

#if !defined(Reg_Debug_OUT_sts_reg__REMOVED) /* Check for removal by optimization */


/*******************************************************************************
* Function Name: Reg_Debug_OUT_Read
********************************************************************************
*
* Summary:
*  Read the value of the Reg_Debug_OUT_Status register
*
* Parameters:  
*  void
*
* Return: 
*  The value of the Reg_Debug_OUT_Status register.
*
* Reentrant:
*  Yes
* 
*******************************************************************************/
uint8 Reg_Debug_OUT_Read(void) 
{ 
    return Reg_Debug_OUT_Status;
}


#endif /* Check for removal by optimization */

