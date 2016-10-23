/*******************************************************************************
* File Name: Status_Reg_1.c  
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
#include "Status_Reg_1.h"

#if !defined(Status_Reg_1_sts_reg__REMOVED) /* Check for removal by optimization */


/*******************************************************************************
* Function Name: Status_Reg_1_Read
********************************************************************************
*
* Summary:
*  Read the value of the Status_Reg_1_Status register
*
* Parameters:  
*  void
*
* Return: 
*  The value of the Status_Reg_1_Status register.
*
* Reentrant:
*  Yes
* 
*******************************************************************************/
uint8 Status_Reg_1_Read(void) 
{ 
    return Status_Reg_1_Status;
}


#endif /* Check for removal by optimization */

