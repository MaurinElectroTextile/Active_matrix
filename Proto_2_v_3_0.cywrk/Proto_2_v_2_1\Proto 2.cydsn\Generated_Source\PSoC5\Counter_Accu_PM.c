/*******************************************************************************
* File Name: Counter_Accu_PM.c  
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

#include "Counter_Accu.h"

static Counter_Accu_backupStruct Counter_Accu_backup;


/*******************************************************************************
* Function Name: Counter_Accu_SaveConfig
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
*  Counter_Accu_backup:  Variables of this global structure are modified to 
*  store the values of non retention configuration registers when Sleep() API is 
*  called.
*
* Reentrant:
*    No
*
*******************************************************************************/
void Counter_Accu_SaveConfig(void)
{
    #if (! Counter_Accu_UsingFixedFunction)    
        #if(!Counter_Accu_ControlRegRemoved)
            Counter_Accu_backup.control = Counter_Accu_ReadControlRegister();
        #endif
        
        Counter_Accu_backup.counterUdb = Counter_Accu_ReadCounter();
        Counter_Accu_backup.captureValue = Counter_Accu_ReadCapture();
    #endif
}


/*******************************************************************************
* Function Name: Counter_Accu_RestoreConfig
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
*  Counter_Accu_backup:  Variables of this global structure are used to restore 
*  the values of non retention registers on wakeup from sleep mode.
*
* Reentrant:
*  Yes
*
*******************************************************************************/
void Counter_Accu_RestoreConfig(void) 
{      
    #if (! Counter_Accu_UsingFixedFunction)     
        #if(!Counter_Accu_ControlRegRemoved)
            Counter_Accu_WriteControlRegister(Counter_Accu_backup.control);
        #endif
        
        Counter_Accu_WriteCounter(Counter_Accu_backup.counterUdb);        
        CY_SET_REG16 (Counter_Accu_STATICCOUNT_LSB_PTR, Counter_Accu_backup.captureValue);
    #endif
}


/*******************************************************************************
* Function Name: Counter_Accu_Sleep
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
*  Counter_Accu_backup.enableState:  Is modified depending on the enable state
*  of the block before entering sleep mode.
*
* Reentrant:
*  No
*
*******************************************************************************/
void Counter_Accu_Sleep(void)
{
    #if(!Counter_Accu_ControlRegRemoved)
        /* Save Counter's enable state */
        if(Counter_Accu_CTRL_ENABLE == (Counter_Accu_CONTROL & Counter_Accu_CTRL_ENABLE))
        {
            /* Counter is enabled */
            Counter_Accu_backup.enableState = 1u;
        }
        else
        {
            /* Counter is disabled */
            Counter_Accu_backup.enableState = 0u;
        }
    #endif
    
    Counter_Accu_Stop();
    Counter_Accu_SaveConfig();
}


/*******************************************************************************
* Function Name: Counter_Accu_Wakeup
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
*  Counter_Accu_backup.enableState:  Is used to restore the enable state of 
*  block on wakeup from sleep mode.
*
* Reentrant:
*  Yes
*
*******************************************************************************/
void Counter_Accu_Wakeup(void) 
{
    Counter_Accu_RestoreConfig();
    
    #if(!Counter_Accu_ControlRegRemoved)
        if(Counter_Accu_backup.enableState == 1u)
        {
            /* Enable Counter's operation */
            Counter_Accu_Enable();
    
        } /* Do nothing if Counter was disabled before */    
    #endif
}


/* [] END OF FILE */
