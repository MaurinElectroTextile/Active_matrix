/*******************************************************************************
* File Name: OpampHPF_PM.c  
* Version 1.50
*
* Description:
*  This file provides the power manager source code to the API for the PGA 
*  Component.
*
* Note:
*
********************************************************************************
* Copyright 2008-2010, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions, 
* disclaimers, and limitations in the end user license agreement accompanying 
* the software package with which this file was provided.
*******************************************************************************/

#include "OpampHPF.h"

static OpampHPF_BACKUP_STRUCT  OpampHPF_backup;


/*******************************************************************************  
* Function Name: OpampHPF_SaveConfig
********************************************************************************
*
* Summary:
*  Save the current user register configuration.
* 
* Parameters:
*  void
* 
* Return:
*  void
*
* Reentrant:
*  No.
*
*******************************************************************************/
void OpampHPF_SaveConfig(void)
{
    /* Nothing to save as registers are System reset on retention flops */
}


/*******************************************************************************  
* Function Name: OpampHPF_RestoreConfig
********************************************************************************
*
* Summary:
*  Restore the register configurations.
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
void OpampHPF_RestoreConfig(void)
{
    /* Nothing to restore */
}


/*******************************************************************************   
* Function Name: OpampHPF_Sleep
********************************************************************************
*
* Summary:
*  Disables block's operation and saves its configuration. Should be called 
*  just prior to entering sleep.
*
* Parameters:
*  None
*
* Return:
*  None
*
* Global variables:
*  OpampHPF_backup:  The structure field 'enableState' is modified 
*  depending on the enable state of the block before entering to sleep mode.
*
* Reentrant:
*  No
*
*******************************************************************************/
void OpampHPF_Sleep(void)
{
    /* Save PGA enable state */
    if(OpampHPF_ACT_PWR_EN == (OpampHPF_PM_ACT_CFG_REG & OpampHPF_ACT_PWR_EN))
    {
        /* Component is enabled */
        OpampHPF_backup.enableState = 1u;
    }
    else
    {
        /* Component is disabled */
        OpampHPF_backup.enableState = 0u;
    }

    /* Stop the configuration */
    OpampHPF_Stop();

    /* Save the configuration */
    OpampHPF_SaveConfig();
}


/*******************************************************************************  
* Function Name: OpampHPF_Wakeup
********************************************************************************
*
* Summary:
*  Enables block's operation and restores its configuration. Should be called
*  just after awaking from sleep.
*
* Parameters:
*  None
*
* Return:
*  void
*
* Global variables:
*  OpampHPF_backup:  The structure field 'enableState' is used to 
*  restore the enable state of block after wakeup from sleep mode.
*
* Reentrant:
*  No
* 
*******************************************************************************/
void OpampHPF_Wakeup(void) 
{
    /* Restore the configurations */
    OpampHPF_RestoreConfig();

     /* Enable's the component operation */
    if(OpampHPF_backup.enableState == 1u)
    {
        OpampHPF_Enable();
    } /* Do nothing if component was disable before */
}


/* [] END OF FILE */
