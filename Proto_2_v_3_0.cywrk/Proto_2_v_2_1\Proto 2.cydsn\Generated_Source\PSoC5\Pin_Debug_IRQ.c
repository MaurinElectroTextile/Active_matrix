/*******************************************************************************
* File Name: Pin_Debug_IRQ.c  
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
#include "Pin_Debug_IRQ.h"


/*******************************************************************************
* Function Name: Pin_Debug_IRQ_Write
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
void Pin_Debug_IRQ_Write(uint8 value) 
{
    uint8 staticBits = Pin_Debug_IRQ_DR & ~Pin_Debug_IRQ_MASK;
    Pin_Debug_IRQ_DR = staticBits | ((value << Pin_Debug_IRQ_SHIFT) & Pin_Debug_IRQ_MASK);
}


/*******************************************************************************
* Function Name: Pin_Debug_IRQ_SetDriveMode
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
void Pin_Debug_IRQ_SetDriveMode(uint8 mode) 
{
	CyPins_SetPinDriveMode(Pin_Debug_IRQ_0, mode);
}


/*******************************************************************************
* Function Name: Pin_Debug_IRQ_Read
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
*  Macro Pin_Debug_IRQ_ReadPS calls this function. 
*  
*******************************************************************************/
uint8 Pin_Debug_IRQ_Read(void) 
{
    return (Pin_Debug_IRQ_PS & Pin_Debug_IRQ_MASK) >> Pin_Debug_IRQ_SHIFT;
}


/*******************************************************************************
* Function Name: Pin_Debug_IRQ_ReadDataReg
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
uint8 Pin_Debug_IRQ_ReadDataReg(void) 
{
    return (Pin_Debug_IRQ_DR & Pin_Debug_IRQ_MASK) >> Pin_Debug_IRQ_SHIFT;
}


/* If Interrupts Are Enabled for this Pins component */ 
#if defined(Pin_Debug_IRQ_INTSTAT) 

    /*******************************************************************************
    * Function Name: Pin_Debug_IRQ_ClearInterrupt
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
    uint8 Pin_Debug_IRQ_ClearInterrupt(void) 
    {
        return (Pin_Debug_IRQ_INTSTAT & Pin_Debug_IRQ_MASK) >> Pin_Debug_IRQ_SHIFT;
    }

#endif /* If Interrupts Are Enabled for this Pins component */ 


/* [] END OF FILE */ 
