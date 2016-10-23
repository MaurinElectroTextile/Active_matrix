/*******************************************************************************
* File Name: Pin_Col_Clock.c  
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
#include "Pin_Col_Clock.h"


/*******************************************************************************
* Function Name: Pin_Col_Clock_Write
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
void Pin_Col_Clock_Write(uint8 value) 
{
    uint8 staticBits = Pin_Col_Clock_DR & ~Pin_Col_Clock_MASK;
    Pin_Col_Clock_DR = staticBits | ((value << Pin_Col_Clock_SHIFT) & Pin_Col_Clock_MASK);
}


/*******************************************************************************
* Function Name: Pin_Col_Clock_SetDriveMode
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
void Pin_Col_Clock_SetDriveMode(uint8 mode) 
{
	CyPins_SetPinDriveMode(Pin_Col_Clock_0, mode);
}


/*******************************************************************************
* Function Name: Pin_Col_Clock_Read
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
*  Macro Pin_Col_Clock_ReadPS calls this function. 
*  
*******************************************************************************/
uint8 Pin_Col_Clock_Read(void) 
{
    return (Pin_Col_Clock_PS & Pin_Col_Clock_MASK) >> Pin_Col_Clock_SHIFT;
}


/*******************************************************************************
* Function Name: Pin_Col_Clock_ReadDataReg
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
uint8 Pin_Col_Clock_ReadDataReg(void) 
{
    return (Pin_Col_Clock_DR & Pin_Col_Clock_MASK) >> Pin_Col_Clock_SHIFT;
}


/* If Interrupts Are Enabled for this Pins component */ 
#if defined(Pin_Col_Clock_INTSTAT) 

    /*******************************************************************************
    * Function Name: Pin_Col_Clock_ClearInterrupt
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
    uint8 Pin_Col_Clock_ClearInterrupt(void) 
    {
        return (Pin_Col_Clock_INTSTAT & Pin_Col_Clock_MASK) >> Pin_Col_Clock_SHIFT;
    }

#endif /* If Interrupts Are Enabled for this Pins component */ 


/* [] END OF FILE */ 
