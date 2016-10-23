/*******************************************************************************
* File Name: Opamp_HPF_PM.c
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

#include "Opamp_HPF.h"

static Opamp_HPF_BACKUP_STRUCT  Opamp_HPF_backup;


/*******************************************************************************  
* Function Name: Opamp_HPF_SaveConfig
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
void Opamp_HPF_SaveConfig(void)
{
    /* Nothing to save as registers are System reset on retention flops */
}


/*******************************************************************************  
* Function Name: Opamp_HPF_RestoreConfig
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
void Opamp_HPF_RestoreConfig(void)
{
    /* Nothing to restore */
}


/*******************************************************************************   
* Function Name: Opamp_HPF_Sleep
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
*  Opamp_HPF_backup:  The structure field 'enableState' is modified 
*  depending on the enable state of the block before entering to sleep mode.
*
* Reentrant:
*  No
*
*******************************************************************************/
void Opamp_HPF_Sleep(void)
{
    /* Save OpAmp enable state */
    if(Opamp_HPF_ACT_PWR_EN == (Opamp_HPF_PM_ACT_CFG_REG & Opamp_HPF_ACT_PWR_EN))
    {
        /* Component is enabled */
        Opamp_HPF_backup.enableState = 1u;
    }
    else
    {
        /* Component is disabled */
        Opamp_HPF_backup.enableState = 0u;
    }
    /* Stops the component */
    Opamp_HPF_Stop();

    /* Saves the configuration */
    Opamp_HPF_SaveConfig();
}


/*******************************************************************************  
* Function Name: Opamp_HPF_Wakeup
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
*  Opamp_HPF_backup:  The structure field 'enableState' is used to 
*  restore the enable state of block after wakeup from sleep mode.
* 
* Reentrant:
*  No
*
*******************************************************************************/
void Opamp_HPF_Wakeup(void) 
{
    /* Restore the user configuration */
    Opamp_HPF_RestoreConfig();

    /* Enable's the component operation */
    if(Opamp_HPF_backup.enableState == 1u)
    {
        Opamp_HPF_Enable();
    } /* Do nothing if component was disable before */
}


/* [] END OF FILE */
