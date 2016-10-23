/*******************************************************************************
* File Name: Opamp_B_PM.c
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

#include "Opamp_B.h"

static Opamp_B_BACKUP_STRUCT  Opamp_B_backup;


/*******************************************************************************  
* Function Name: Opamp_B_SaveConfig
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
void Opamp_B_SaveConfig(void)
{
    /* Nothing to save as registers are System reset on retention flops */
}


/*******************************************************************************  
* Function Name: Opamp_B_RestoreConfig
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
void Opamp_B_RestoreConfig(void)
{
    /* Nothing to restore */
}


/*******************************************************************************   
* Function Name: Opamp_B_Sleep
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
*  Opamp_B_backup:  The structure field 'enableState' is modified 
*  depending on the enable state of the block before entering to sleep mode.
*
* Reentrant:
*  No
*
*******************************************************************************/
void Opamp_B_Sleep(void)
{
    /* Save OpAmp enable state */
    if(Opamp_B_ACT_PWR_EN == (Opamp_B_PM_ACT_CFG_REG & Opamp_B_ACT_PWR_EN))
    {
        /* Component is enabled */
        Opamp_B_backup.enableState = 1u;
    }
    else
    {
        /* Component is disabled */
        Opamp_B_backup.enableState = 0u;
    }
    /* Stops the component */
    Opamp_B_Stop();

    /* Saves the configuration */
    Opamp_B_SaveConfig();
}


/*******************************************************************************  
* Function Name: Opamp_B_Wakeup
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
*  Opamp_B_backup:  The structure field 'enableState' is used to 
*  restore the enable state of block after wakeup from sleep mode.
* 
* Reentrant:
*  No
*
*******************************************************************************/
void Opamp_B_Wakeup(void) 
{
    /* Restore the user configuration */
    Opamp_B_RestoreConfig();

    /* Enable's the component operation */
    if(Opamp_B_backup.enableState == 1u)
    {
        Opamp_B_Enable();
    } /* Do nothing if component was disable before */
}


/* [] END OF FILE */
