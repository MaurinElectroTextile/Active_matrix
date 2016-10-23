/*******************************************************************************
* File Name: Matrix_CR.c  
* Version 1.50
*
* Description:
*  This file contains API to enable firmware control of a control register.
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
#include "Matrix_CR.h"

#if !defined(Matrix_CR_ctrl_reg__REMOVED) /* Check for removal by optimization */


/*******************************************************************************
* Function Name: Matrix_CR_Write
********************************************************************************
*
* Summary:
*  Write a byte to a control register.
*
* Parameters:  
*  control:  The value to be assigned to the control register. 
*
* Return: 
*  void 
*
* Reentrant:
*  Yes
*  
*******************************************************************************/
void Matrix_CR_Write(uint8 control) 
{
    Matrix_CR_Control = control;
}


/*******************************************************************************
* Function Name: Matrix_CR_Read
********************************************************************************
*
* Summary:
*  Read the current value assigned to a control register.
*
* Parameters:  
*  void 
*
* Return: 
*  Returns the current value assigned to the control register
*
* Reentrant:
*  Yes
*  
*******************************************************************************/
uint8 Matrix_CR_Read(void) 
{
    return Matrix_CR_Control;
}

#endif /* End check for removal by optimization */
