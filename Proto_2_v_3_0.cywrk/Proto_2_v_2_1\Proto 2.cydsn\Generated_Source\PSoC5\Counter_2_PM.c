/*******************************************************************************
* File Name: Counter_2_PM.c  
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

#include "Counter_2.h"

static Counter_2_backupStruct Counter_2_backup;


/*******************************************************************************
* Function Name: Counter_2_SaveConfig
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
*  Counter_2_backup:  Variables of this global structure are modified to 
*  store the values of non retention configuration registers when Sleep() API is 
*  called.
*
* Reentrant:
*    No
*
*******************************************************************************/
void Counter_2_SaveConfig(void)
{
    #if (! Counter_2_UsingFixedFunction)    
        #if(!Counter_2_ControlRegRemoved)
            Counter_2_backup.control = Counter_2_ReadControlRegister();
        #endif
        
        Counter_2_backup.counterUdb = Counter_2_ReadCounter();
        Counter_2_backup.captureValue = Counter_2_ReadCapture();
    #endif
}


/*******************************************************************************
* Function Name: Counter_2_RestoreConfig
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
*  Counter_2_backup:  Variables of this global structure are used to restore 
*  the values of non retention registers on wakeup from sleep mode.
*
* Reentrant:
*  Yes
*
*******************************************************************************/
void Counter_2_RestoreConfig(void) 
{      
    #if (! Counter_2_UsingFixedFunction)     
        #if(!Counter_2_ControlRegRemoved)
            Counter_2_WriteControlRegister(Counter_2_backup.control);
        #endif
        
        Counter_2_WriteCounter(Counter_2_backup.counterUdb);        
        CY_SET_REG8 (Counter_2_STATICCOUNT_LSB_PTR, Counter_2_backup.captureValue);
    #endif
}


/*******************************************************************************
* Function Name: Counter_2_Sleep
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
*  Counter_2_backup.enableState:  Is modified depending on the enable state
*  of the block before entering sleep mode.
*
* Reentrant:
*  No
*
*******************************************************************************/
void Counter_2_Sleep(void)
{
    #if(!Counter_2_ControlRegRemoved)
        /* Save Counter's enable state */
        if(Counter_2_CTRL_ENABLE == (Counter_2_CONTROL & Counter_2_CTRL_ENABLE))
        {
            /* Counter is enabled */
            Counter_2_backup.enableState = 1u;
        }
        else
        {
            /* Counter is disabled */
            Counter_2_backup.enableState = 0u;
        }
    #endif
    
    Counter_2_Stop();
    Counter_2_SaveConfig();
}


/*******************************************************************************
* Function Name: Counter_2_Wakeup
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
*  Counter_2_backup.enableState:  Is used to restore the enable state of 
*  block on wakeup from sleep mode.
*
* Reentrant:
*  Yes
*
*******************************************************************************/
void Counter_2_Wakeup(void) 
{
    Counter_2_RestoreConfig();
    
    #if(!Counter_2_ControlRegRemoved)
        if(Counter_2_backup.enableState == 1u)
        {
            /* Enable Counter's operation */
            Counter_2_Enable();
    
        } /* Do nothing if Counter was disabled before */    
    #endif
}


/* [] END OF FILE */
