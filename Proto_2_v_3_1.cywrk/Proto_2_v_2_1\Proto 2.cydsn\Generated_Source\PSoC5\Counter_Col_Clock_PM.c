/*******************************************************************************
* File Name: Counter_Col_Clock_PM.c  
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

#include "Counter_Col_Clock.h"

static Counter_Col_Clock_backupStruct Counter_Col_Clock_backup;


/*******************************************************************************
* Function Name: Counter_Col_Clock_SaveConfig
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
*  Counter_Col_Clock_backup:  Variables of this global structure are modified to 
*  store the values of non retention configuration registers when Sleep() API is 
*  called.
*
* Reentrant:
*    No
*
*******************************************************************************/
void Counter_Col_Clock_SaveConfig(void)
{
    #if (! Counter_Col_Clock_UsingFixedFunction)    
        #if(!Counter_Col_Clock_ControlRegRemoved)
            Counter_Col_Clock_backup.control = Counter_Col_Clock_ReadControlRegister();
        #endif
        
        Counter_Col_Clock_backup.counterUdb = Counter_Col_Clock_ReadCounter();
        Counter_Col_Clock_backup.captureValue = Counter_Col_Clock_ReadCapture();
    #endif
}


/*******************************************************************************
* Function Name: Counter_Col_Clock_RestoreConfig
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
*  Counter_Col_Clock_backup:  Variables of this global structure are used to restore 
*  the values of non retention registers on wakeup from sleep mode.
*
* Reentrant:
*  Yes
*
*******************************************************************************/
void Counter_Col_Clock_RestoreConfig(void) 
{      
    #if (! Counter_Col_Clock_UsingFixedFunction)     
        #if(!Counter_Col_Clock_ControlRegRemoved)
            Counter_Col_Clock_WriteControlRegister(Counter_Col_Clock_backup.control);
        #endif
        
        Counter_Col_Clock_WriteCounter(Counter_Col_Clock_backup.counterUdb);        
        CY_SET_REG8 (Counter_Col_Clock_STATICCOUNT_LSB_PTR, Counter_Col_Clock_backup.captureValue);
    #endif
}


/*******************************************************************************
* Function Name: Counter_Col_Clock_Sleep
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
*  Counter_Col_Clock_backup.enableState:  Is modified depending on the enable state
*  of the block before entering sleep mode.
*
* Reentrant:
*  No
*
*******************************************************************************/
void Counter_Col_Clock_Sleep(void)
{
    #if(!Counter_Col_Clock_ControlRegRemoved)
        /* Save Counter's enable state */
        if(Counter_Col_Clock_CTRL_ENABLE == (Counter_Col_Clock_CONTROL & Counter_Col_Clock_CTRL_ENABLE))
        {
            /* Counter is enabled */
            Counter_Col_Clock_backup.enableState = 1u;
        }
        else
        {
            /* Counter is disabled */
            Counter_Col_Clock_backup.enableState = 0u;
        }
    #endif
    
    Counter_Col_Clock_Stop();
    Counter_Col_Clock_SaveConfig();
}


/*******************************************************************************
* Function Name: Counter_Col_Clock_Wakeup
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
*  Counter_Col_Clock_backup.enableState:  Is used to restore the enable state of 
*  block on wakeup from sleep mode.
*
* Reentrant:
*  Yes
*
*******************************************************************************/
void Counter_Col_Clock_Wakeup(void) 
{
    Counter_Col_Clock_RestoreConfig();
    
    #if(!Counter_Col_Clock_ControlRegRemoved)
        if(Counter_Col_Clock_backup.enableState == 1u)
        {
            /* Enable Counter's operation */
            Counter_Col_Clock_Enable();
    
        } /* Do nothing if Counter was disabled before */    
    #endif
}


/* [] END OF FILE */
