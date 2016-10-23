/*******************************************************************************
* File Name: Pin_Overflow.c  
* Version 1.50
*
* Description:
*  This file contains API to enable firmware control of a Pins component.
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
#include "Pin_Overflow.h"


/*******************************************************************************
* Function Name: Pin_Overflow_Write
********************************************************************************
* Summary:
*  Assign a new value to the digital port's data output register.  
*
* Parameters:  
*  prtValue:  The value to be assigned to the Digital Port. 
*
* Return: 
*  void 
*  
*******************************************************************************/
void Pin_Overflow_Write(uint8 value) 
{
    uint8 staticBits = Pin_Overflow_DR & ~Pin_Overflow_MASK;
    Pin_Overflow_DR = staticBits | ((value << Pin_Overflow_SHIFT) & Pin_Overflow_MASK);
}


/*******************************************************************************
* Function Name: Pin_Overflow_SetDriveMode
********************************************************************************
* Summary:
*  Change the drive mode on the pins of the port.
* 
* Parameters:  
*  mode:  Change the pins to this drive mode.
*
* Return: 
*  void
*
*******************************************************************************/
void Pin_Overflow_SetDriveMode(uint8 mode) 
{
	CyPins_SetPinDriveMode(Pin_Overflow_0, mode);
}


/*******************************************************************************
* Function Name: Pin_Overflow_Read
********************************************************************************
* Summary:
*  Read the current value on the pins of the Digital Port in right justified 
*  form.
*
* Parameters:  
*  void 
*
* Return: 
*  Returns the current value of the Digital Port as a right justified number
*  
* Note:
*  Macro Pin_Overflow_ReadPS calls this function. 
*  
*******************************************************************************/
uint8 Pin_Overflow_Read(void) 
{
    return (Pin_Overflow_PS & Pin_Overflow_MASK) >> Pin_Overflow_SHIFT;
}


/*******************************************************************************
* Function Name: Pin_Overflow_ReadDataReg
********************************************************************************
* Summary:
*  Read the current value assigned to a Digital Port's data output register
*
* Parameters:  
*  void 
*
* Return: 
*  Returns the current value assigned to the Digital Port's data output register
*  
*******************************************************************************/
uint8 Pin_Overflow_ReadDataReg(void) 
{
    return (Pin_Overflow_DR & Pin_Overflow_MASK) >> Pin_Overflow_SHIFT;
}


/* If Interrupts Are Enabled for this Pins component */ 
#if defined(Pin_Overflow_INTSTAT) 

    /*******************************************************************************
    * Function Name: Pin_Overflow_ClearInterrupt
    ********************************************************************************
    * Summary:
    *  Clears any active interrupts attached to port and returns the value of the 
    *  interrupt status register.
    *
    * Parameters:  
    *  void 
    *
    * Return: 
    *  Returns the value of the interrupt status register
    *  
    *******************************************************************************/
    uint8 Pin_Overflow_ClearInterrupt(void) 
    {
        return (Pin_Overflow_INTSTAT & Pin_Overflow_MASK) >> Pin_Overflow_SHIFT;
    }

#endif /* If Interrupts Are Enabled for this Pins component */ 


/* [] END OF FILE */ 
