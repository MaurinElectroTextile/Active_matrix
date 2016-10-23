/*******************************************************************************
* File Name: Opamp_Sine_In_PM.c
* Version 1.50
*
* Description:
*  This file provides the power management source code to the API for the 
*  OpAmp (Analog Buffer) component.
*
* Note:
*
*******************************************************************************
* Copyright 2008-2010, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions, 
* disclaimers, and limitations in the end user license agreement accompanying 
* the software package with which this file was provided.
*******************************************************************************/

#include "Opamp_Sine_In.h"

static Opamp_Sine_In_BACKUP_STRUCT  Opamp_Sine_In_backup;


/*******************************************************************************  
* Function Name: Opamp_Sine_In_SaveConfig
********************************************************************************
*
* Summary:
*  Saves the current user configuration registers.
* 
* Parameters:
*  void
* 
* Return:
*  void
*
* Reentrant:
*  No
*
*******************************************************************************/
void Opamp_Sine_In_SaveConfig(void)
{
    /* Nothing to save as registers are System reset on retention flops */
}


/*******************************************************************************  
* Function Name: Opamp_Sine_In_RestoreConfig
********************************************************************************
*
* Summary:
*  Restores the current user configuration registers.
* 
* Parameters:
*  void
* 
* Return:
*  void
*
* Reentrant:
*  No
*
*******************************************************************************/
void Opamp_Sine_In_RestoreConfig(void)
{
    /* Nothing to restore */
}


/*******************************************************************************   
* Function Name: Opamp_Sine_In_Sleep
********************************************************************************
*
* Summary:
*  Disables block's operation and saves its configuration. Should be called 
*  just prior to entering sleep.
*
* Parameters:
*  void
*
* Return:
*  void
*
* Global variables:
*  Opamp_Sine_In_backup:  The structure field 'enableState' is modified 
*  depending on the enable state of the block before entering to sleep mode.
*
* Reentrant:
*  No
*
*******************************************************************************/
void Opamp_Sine_In_Sleep(void)
{
    /* Save OpAmp enable state */
    if(Opamp_Sine_In_ACT_PWR_EN == (Opamp_Sine_In_PM_ACT_CFG_REG & Opamp_Sine_In_ACT_PWR_EN))
    {
        /* Component is enabled */
        Opamp_Sine_In_backup.enableState = 1u;
    }
    else
    {
        /* Component is disabled */
        Opamp_Sine_In_backup.enableState = 0u;
    }
    /* Stops the component */
    Opamp_Sine_In_Stop();

    /* Saves the configuration */
    Opamp_Sine_In_SaveConfig();
}


/*******************************************************************************  
* Function Name: Opamp_Sine_In_Wakeup
********************************************************************************
*
* Summary:
*  Enables block's operation and restores its configuration. Should be called
*  just after awaking from sleep.
*
* Parameters:
*  void
*
* Return:
*  void
*
* Global variables:
*  Opamp_Sine_In_backup:  The structure field 'enableState' is used to 
*  restore the enable state of block after wakeup from sleep mode.
* 
* Reentrant:
*  No
*
*******************************************************************************/
void Opamp_Sine_In_Wakeup(void) 
{
    /* Restore the user configuration */
    Opamp_Sine_In_RestoreConfig();

    /* Enable's the component operation */
    if(Opamp_Sine_In_backup.enableState == 1u)
    {
        Opamp_Sine_In_Enable();
    } /* Do nothing if component was disable before */
}


/* [] END OF FILE */
