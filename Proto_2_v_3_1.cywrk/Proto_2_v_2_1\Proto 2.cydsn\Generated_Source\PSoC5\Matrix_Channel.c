/*******************************************************************************
* File Name: Matrix_Channel.c  
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
#include "Matrix_Channel.h"

#if !defined(Matrix_Channel_ctrl_reg__REMOVED) /* Check for removal by optimization */


/*******************************************************************************
* Function Name: Matrix_Channel_Write
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
void Matrix_Channel_Write(uint8 control) 
{
    Matrix_Channel_Control = control;
}


/*******************************************************************************
* Function Name: Matrix_Channel_Read
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
uint8 Matrix_Channel_Read(void) 
{
    return Matrix_Channel_Control;
}

#endif /* End check for removal by optimization */
