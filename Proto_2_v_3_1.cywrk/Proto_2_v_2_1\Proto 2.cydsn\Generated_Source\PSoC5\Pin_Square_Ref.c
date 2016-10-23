/*******************************************************************************
* File Name: Pin_Square_Ref.c  
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
#include "Pin_Square_Ref.h"


/*******************************************************************************
* Function Name: Pin_Square_Ref_Write
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
void Pin_Square_Ref_Write(uint8 value) 
{
    uint8 staticBits = Pin_Square_Ref_DR & ~Pin_Square_Ref_MASK;
    Pin_Square_Ref_DR = staticBits | ((value << Pin_Square_Ref_SHIFT) & Pin_Square_Ref_MASK);
}


/*******************************************************************************
* Function Name: Pin_Square_Ref_SetDriveMode
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
void Pin_Square_Ref_SetDriveMode(uint8 mode) 
{
	CyPins_SetPinDriveMode(Pin_Square_Ref_0, mode);
}


/*******************************************************************************
* Function Name: Pin_Square_Ref_Read
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
*  Macro Pin_Square_Ref_ReadPS calls this function. 
*  
*******************************************************************************/
uint8 Pin_Square_Ref_Read(void) 
{
    return (Pin_Square_Ref_PS & Pin_Square_Ref_MASK) >> Pin_Square_Ref_SHIFT;
}


/*******************************************************************************
* Function Name: Pin_Square_Ref_ReadDataReg
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
uint8 Pin_Square_Ref_ReadDataReg(void) 
{
    return (Pin_Square_Ref_DR & Pin_Square_Ref_MASK) >> Pin_Square_Ref_SHIFT;
}


/* If Interrupts Are Enabled for this Pins component */ 
#if defined(Pin_Square_Ref_INTSTAT) 

    /*******************************************************************************
    * Function Name: Pin_Square_Ref_ClearInterrupt
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
    uint8 Pin_Square_Ref_ClearInterrupt(void) 
    {
        return (Pin_Square_Ref_INTSTAT & Pin_Square_Ref_MASK) >> Pin_Square_Ref_SHIFT;
    }

#endif /* If Interrupts Are Enabled for this Pins component */ 


/* [] END OF FILE */ 
