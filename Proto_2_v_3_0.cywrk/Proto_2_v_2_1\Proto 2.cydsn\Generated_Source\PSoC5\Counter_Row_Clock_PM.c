/*******************************************************************************
* File Name: Counter_Row_Clock_PM.c  
* Version 1.50
*
*  Description:
*     This file provides the power management source code to API for the
*     Counter.  
*
*   Note:
*     None
*
*******************************************************************************
* Copyright 2008-2010, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions, 
* disclaimers, and limitations in the end user license agreement accompanying 
* the software package with which this file was provided.
********************************************************************************/

#include "Counter_Row_Clock.h"

static Counter_Row_Clock_backupStruct Counter_Row_Clock_backup;


/*******************************************************************************
* Function Name: Counter_Row_Clock_SaveConfig
********************************************************************************
* Summary:
*     Save the current user configuration
*
* Parameters:  
*  void
*
* Return: 
*  void
*
* Global variables:
*  Counter_Row_Clock_backup:  Variables of this global structure are modified to 
*  store the values of non retention configuration registers when Sleep() API is 
*  called.
*
* Reentrant:
*    No
*
*******************************************************************************/
void Counter_Row_Clock_SaveConfig(void)
{
    #if (! Counter_Row_Clock_UsingFixedFunction)    
        #if(!Counter_Row_Clock_ControlRegRemoved)
            Counter_Row_Clock_backup.control = Counter_Row_Clock_ReadControlRegister();
        #endif
        
        Counter_Row_Clock_backup.counterUdb = Counter_Row_Clock_ReadCounter();
        Counter_Row_Clock_backup.captureValue = Counter_Row_Clock_ReadCapture();
    #endif
}


/*******************************************************************************
* Function Name: Counter_Row_Clock_RestoreConfig
********************************************************************************
*
* Summary:
*  Restores the current user configuration.
*
* Parameters:  
*  void
*
* Return: 
*  void
*
* Global variables:
*  Counter_Row_Clock_backup:  Variables of this global structure are used to restore 
*  the values of non retention registers on wakeup from sleep mode.
*
* Reentrant:
*  Yes
*
*******************************************************************************/
void Counter_Row_Clock_RestoreConfig(void) 
{      
    #if (! Counter_Row_Clock_UsingFixedFunction)     
        #if(!Counter_Row_Clock_ControlRegRemoved)
            Counter_Row_Clock_WriteControlRegister(Counter_Row_Clock_backup.control);
        #endif
        
        Counter_Row_Clock_WriteCounter(Counter_Row_Clock_backup.counterUdb);        
        CY_SET_REG8 (Counter_Row_Clock_STATICCOUNT_LSB_PTR, Counter_Row_Clock_backup.captureValue);
    #endif
}


/*******************************************************************************
* Function Name: Counter_Row_Clock_Sleep
********************************************************************************
* Summary:
*     Stop and Save the user configuration
*
* Parameters:  
*  void
*
* Return: 
*  void
*
* Global variables:
*  Counter_Row_Clock_backup.enableState:  Is modified depending on the enable state
*  of the block before entering sleep mode.
*
* Reentrant:
*  No
*
*******************************************************************************/
void Counter_Row_Clock_Sleep(void)
{
    #if(!Counter_Row_Clock_ControlRegRemoved)
        /* Save Counter's enable state */
        if(Counter_Row_Clock_CTRL_ENABLE == (Counter_Row_Clock_CONTROL & Counter_Row_Clock_CTRL_ENABLE))
        {
            /* Counter is enabled */
            Counter_Row_Clock_backup.enableState = 1u;
        }
        else
        {
            /* Counter is disabled */
            Counter_Row_Clock_backup.enableState = 0u;
        }
    #endif
    
    Counter_Row_Clock_Stop();
    Counter_Row_Clock_SaveConfig();
}


/*******************************************************************************
* Function Name: Counter_Row_Clock_Wakeup
********************************************************************************
*
* Summary:
*  Restores and enables the user configuration
*  
* Parameters:  
*  void
*
* Return: 
*  void
*
* Global variables:
*  Counter_Row_Clock_backup.enableState:  Is used to restore the enable state of 
*  block on wakeup from sleep mode.
*
* Reentrant:
*  Yes
*
*******************************************************************************/
void Counter_Row_Clock_Wakeup(void) 
{
    Counter_Row_Clock_RestoreConfig();
    
    #if(!Counter_Row_Clock_ControlRegRemoved)
        if(Counter_Row_Clock_backup.enableState == 1u)
        {
            /* Enable Counter's operation */
            Counter_Row_Clock_Enable();
    
        } /* Do nothing if Counter was disabled before */    
    #endif
}


/* [] END OF FILE */
