/*******************************************************************************
* File Name: Opamp_Vref_PM.c
* Version 1.60
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

#include "Opamp_Vref.h"

static Opamp_Vref_BACKUP_STRUCT  Opamp_Vref_backup;


/*******************************************************************************  
* Function Name: Opamp_Vref_SaveConfig
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
void Opamp_Vref_SaveConfig(void)
{
    /* Nothing to save as registers are System reset on retention flops */
}


/*******************************************************************************  
* Function Name: Opamp_Vref_RestoreConfig
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
void Opamp_Vref_RestoreConfig(void)
{
    /* Nothing to restore */
}


/*******************************************************************************   
* Function Name: Opamp_Vref_Sleep
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
*  Opamp_Vref_backup:  The structure field 'enableState' is modified 
*  depending on the enable state of the block before entering to sleep mode.
*
* Reentrant:
*  No
*
*******************************************************************************/
void Opamp_Vref_Sleep(void)
{
    /* Save OpAmp enable state */
    if(Opamp_Vref_ACT_PWR_EN == (Opamp_Vref_PM_ACT_CFG_REG & Opamp_Vref_ACT_PWR_EN))
    {
        /* Component is enabled */
        Opamp_Vref_backup.enableState = 1u;
    }
    else
    {
        /* Component is disabled */
        Opamp_Vref_backup.enableState = 0u;
    }
    /* Stops the component */
    Opamp_Vref_Stop();

    /* Saves the configuration */
    Opamp_Vref_SaveConfig();
}


/*******************************************************************************  
* Function Name: Opamp_Vref_Wakeup
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
*  Opamp_Vref_backup:  The structure field 'enableState' is used to 
*  restore the enable state of block after wakeup from sleep mode.
* 
* Reentrant:
*  No
*
*******************************************************************************/
void Opamp_Vref_Wakeup(void) 
{
    /* Restore the user configuration */
    Opamp_Vref_RestoreConfig();

    /* Enable's the component operation */
    if(Opamp_Vref_backup.enableState == 1u)
    {
        Opamp_Vref_Enable();
    } /* Do nothing if component was disable before */
}


/* [] END OF FILE */
